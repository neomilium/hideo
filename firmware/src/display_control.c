#include "display_control.h"

#include "hideo.h"
#include "eeprom.h"

#include "eventmanager.h"
#include "drv_ps2_mouse.h"
#include "drv_dc-motor.h"
#include "drv_stepper-motor.h"

void		_display_poll(void);
void		_display_event_handler(const event_t event);

/* Lens */
#define DISPLAY_LENS_MODE_INIT			0x00
#define DISPLAY_LENS_MODE_RUN			0x01

static volatile int16_t _display_lens_current_position = 0;
static volatile int16_t _display_lens_wanted_position = 0;
static volatile uint8_t _display_lens_mode = DISPLAY_LENS_MODE_INIT;

#define DISPLAY_LENS_LIMIT_PIN			PINC
#define DISPLAY_LENS_LIMIT_PORT			PORTC
#define DISPLAY_LENS_LIMIT_DDR			DDRC
#define DISPLAY_LENS_LIMIT_MASK			0b10000000

#define DISPLAY_LENS_SPEED_MAX			75
#define DISPLAY_LENS_SPEED_MIN			30

#define DISPLAY_LENS_TOLERANCE			3

/* Trapezoid */
#define DISPLAY_TRAPEZOID_MODE_INIT		0x00
#define DISPLAY_TRAPEZOID_MODE_RUN		0x01

static volatile int16_t _display_trapezoid_current_position = 0;
static volatile int16_t _display_trapezoid_wanted_position = 0;
static volatile uint8_t _display_trapezoid_mode = DISPLAY_TRAPEZOID_MODE_INIT;


void
display_init(void)
{
	/* Lens */
	dc_motor_move(DISPLAY_LENS_SPEED_MAX);
	display_lens_park();

	/* Trapezoid */
	eeprom_read( EEPROM_MEMMAP__DISPLAY_TRAPEZOID, sizeof(_display_trapezoid_wanted_position), (void*)&_display_trapezoid_wanted_position );

	/* Reset EEPROM */
// 	display_trapezoid_set_position(0);
// 	display_lens_set_position(0);

	/* Common */
	eventmanager_add_polling_fct(_display_poll);
	eventmanager_add_handling_fct(_display_event_handler);
}

void
display_lens_load_position(void)
{
	eeprom_read( EEPROM_MEMMAP__DISPLAY_LENS, sizeof(_display_lens_wanted_position), (void*)&_display_lens_wanted_position );
}

void
display_lens_park(void)
{
	_display_lens_wanted_position = 0;
}

void
_display_poll(void)
{
	/* Lens */
	int8_t _display_lens_poll_speed = 0;

	if (_display_lens_mode == DISPLAY_LENS_MODE_RUN) {
		if (_display_lens_current_position != _display_lens_wanted_position) {
			int16_t _display_lens_poll_position_diff = _display_lens_current_position - _display_lens_wanted_position;
	
			if (_display_lens_poll_position_diff < 0) {
				_display_lens_poll_position_diff = -(_display_lens_poll_position_diff);
			}
	
			if (_display_lens_poll_position_diff > DISPLAY_LENS_TOLERANCE) {
				if (_display_lens_poll_position_diff > 20) {
					if (_display_lens_poll_position_diff > 100) {
						_display_lens_poll_speed = DISPLAY_LENS_SPEED_MAX;
					} else {
						_display_lens_poll_speed = (((_display_lens_poll_position_diff * (DISPLAY_LENS_SPEED_MAX - DISPLAY_LENS_SPEED_MIN)) / 100) + DISPLAY_LENS_SPEED_MIN);
					}
				} else {
					_display_lens_poll_speed = DISPLAY_LENS_SPEED_MIN;
				}
			} else {
				_display_lens_poll_speed = 0;
			}
	
			if((_display_lens_current_position - _display_lens_wanted_position) < 0) {
				_display_lens_poll_speed = -(_display_lens_poll_speed);
			}
		} else {
			_display_lens_poll_speed = 0;
		}
		dc_motor_move(_display_lens_poll_speed);

		/* Trapezoid */
		if ( _display_lens_poll_speed == 0 ) {
			switch (_display_trapezoid_mode) {
				case DISPLAY_TRAPEZOID_MODE_INIT:
					stepper_motor_move(5);
				break;
				case DISPLAY_TRAPEZOID_MODE_RUN:
					if (_display_trapezoid_current_position != _display_trapezoid_wanted_position) {
						if ((_display_trapezoid_current_position - _display_trapezoid_wanted_position) > 0) {
							stepper_motor_move(5);
							_display_trapezoid_current_position -= 5;
						} else {
							stepper_motor_move(-5);
							_display_trapezoid_current_position += 5;
						}
					}
				break;
				default:
				break;
			}
		}
	}

}

void
_display_event_handler(const event_t event)
{
	/* Lens */
	switch (_display_lens_mode) {
		case DISPLAY_LENS_MODE_INIT:
			switch (event.code) {
				case E_MOUSE_BUTTON_PRESSED:
					switch (event.data) {
						case MOUSE_BUTTON_RIGHT:
							_display_lens_mode = DISPLAY_LENS_MODE_RUN;
							dc_motor_stop();
					}
					break;
				default:
					break;
			}
			break;
		case DISPLAY_LENS_MODE_RUN:
			switch (event.code) {
				case E_MOUSE_BUTTON_PRESSED:
					switch (event.data) {
						case MOUSE_BUTTON_RIGHT:
							_display_lens_current_position = 0;
							dc_motor_stop();
					}
					break;
				case E_MOUSE_Y_REV:
					_display_lens_current_position += (unsigned)event.data;
					break;
				case E_MOUSE_Y_FWD:
					_display_lens_current_position -= (unsigned)event.data;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	/* Trapezoid */
	switch (_display_trapezoid_mode) {
		case DISPLAY_TRAPEZOID_MODE_INIT:
			switch (event.code) {
				case E_MOUSE_BUTTON_PRESSED:
					switch (event.data) {
						case MOUSE_BUTTON_MIDDLE:
							_display_trapezoid_mode = DISPLAY_LENS_MODE_RUN;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void
display_lens_set_position(int16_t position)
{
	_display_lens_wanted_position = position;
	eeprom_write( EEPROM_MEMMAP__DISPLAY_LENS, (void*)&_display_lens_wanted_position, sizeof(_display_lens_wanted_position) );
}

/*
int16_t
display_lens_get_position(void)
{
	return _display_lens_current_position;
}
*/

void
display_trapezoid_set_position(int16_t position)
{
	_display_trapezoid_wanted_position = position;
	eeprom_write( EEPROM_MEMMAP__DISPLAY_TRAPEZOID, (void*)&_display_trapezoid_wanted_position, sizeof(_display_trapezoid_wanted_position) );
}

/*
int16_t
display_lens_get_position(void)
{
	return _display_lens_current_position;
}
*/
