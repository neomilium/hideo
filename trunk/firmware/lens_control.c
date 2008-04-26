#include "lens_control.h"

#include "eventmanager.h"
#include "drv_ps2_mouse.h"
#include "drv_dc-motor.h"

void		_lens_poll(void);
void		_lens_event_handler(const event_t event);

#define LENS_POSITION_MAX		60000

#define LENS_MODE_INIT			0x00
#define LENS_MODE_RUN				0x01

static volatile uint16 _lens_current_position = 0;
static volatile uint16 _lens_wanted_position = 0;
static volatile uint8 _lens_mode = LENS_MODE_INIT;

#define LENS_LIMIT_SWITCH			GET_BIT(PINC).bit7

void
lens_init(void)
{
	dc_motor_move(-50);

	DDRC &= 0b01111111;
	PORTC |= 0b10000000;

	while (LENS_LIMIT_SWITCH) {
	};
	dc_motor_stop();

	eventmanager_add_polling_fct(_lens_poll);
	eventmanager_add_handling_fct(_lens_event_handler);
}

void
_lens_poll(void)
{
	if (_lens_current_position != _lens_wanted_position) {
		if ((_lens_current_position - _lens_wanted_position) > 0) {
			dc_motor_move(50);
		} else {
			dc_motor_move(-50);
		}
	} else {
		dc_motor_stop();
	}
}

void
_lens_event_handler(const event_t event)
{
	switch (_lens_mode) {
		case LENS_MODE_INIT:
		_lens_mode = LENS_MODE_RUN;
		break;
	case LENS_MODE_RUN:
		switch (event.code) {
		case E_MOUSE_Y_REV:
			_lens_current_position += (unsigned)event.data;
			break;
		case E_MOUSE_Y_FWD:
			_lens_current_position -= (unsigned)event.data;
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
lens_set_position(uint16 position)
{
	_lens_wanted_position = position;
}

uint16
lens_get_position(void)
{
	return _lens_current_position;
}
