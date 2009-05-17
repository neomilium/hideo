#include <avr/pgmspace.h>

#include "app_power.h"

#include "lcd.h"
#include "leds.h"
#include "relay.h"
#include "fans.h"

#include "windowmanager.h"

#include "display_control.h"
#include "hqi_control.h"

#define POWER_STARTUP_CANCEL_DELAY	5
#define POWER_SHUTDOWN_CANCEL_DELAY	30

typedef enum {
	POWER_DISPLAY_MODE_INTERACTIVE,
	POWER_DISPLAY_READONLY,
}		power_display_mode_t;

typedef enum {
	POWER_STATE_STARTING,
	POWER_STATE_ON,
	POWER_STATE_SHUTDOWNING,
	POWER_STATE_OFF,
}		power_state_t;

static power_display_mode_t _display_mode;
static power_display_mode_t _state;
static uint8 _remaining_time_before_startup_process = POWER_STARTUP_CANCEL_DELAY;
static uint8 _remaining_time_before_shutdown_process = POWER_SHUTDOWN_CANCEL_DELAY;

void _app_power_display(void);

void
_app_power_init(void *data)
{
	_state = &data;
	if(hqi_mode() == HQI_MODE_READY) {
		_display_mode = POWER_DISPLAY_MODE_INTERACTIVE;
	} else {
		_display_mode = POWER_DISPLAY_READONLY;
	}

	if(_display_mode == POWER_DISPLAY_MODE_INTERACTIVE) {
		_remaining_time_before_startup_process = POWER_STARTUP_CANCEL_DELAY;
	}
	_app_power_display();
}

void
_app_power_display(void)
{
	switch (hqi_mode()) {
		case HQI_MODE_READY:
			windowmanager_screensaver_disable();
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Power on"));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR(" Startup in"));
			lcd_gotoxy(0,5);
			lcd_display_string(PSTR("Key to cancel"));
			break;
		case HQI_MODE_COOLING:
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Power on"));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR("Cooling HQI..."));
			uint16 hqi_remaining_time = hqi_remaining_time_before_ready();
			if(hqi_remaining_time > 60) {
				lcd_gotoxy(4,3);
				lcd_display_number(hqi_remaining_time / 60);
				lcd_display_string(PSTR(" min "));
			} else {
				lcd_gotoxy(24,3);
			}
			lcd_display_number(hqi_remaining_time % 60);
			lcd_display_string(PSTR(" sec"));
			lcd_finish_line();
			break;
		case HQI_MODE_RUNNING:
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Power on"));
			lcd_gotoxy(0,3);
			lcd_display_string(PSTR(" Hit any key"));
			windowmanager_screensaver_enable();
		break;
	}
}

void
_app_power_event_handler(const event_t event)
{
	switch (_display_mode) {
		case POWER_DISPLAY_MODE_INTERACTIVE:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_remaining_time_before_startup_process--;
					if(0 == _remaining_time_before_startup_process) {
						_display_mode = POWER_DISPLAY_READONLY;
						hqi_start();
						display_lens_load_position();
						_app_power_display();
					} else {
						lcd_gotoxy(24,3);
						lcd_display_number(_remaining_time_before_startup_process);
						lcd_display_string(PSTR(" sec"));
						lcd_finish_line();
					}
				break;
				case E_KEY_PRESSED:
					windowmanager_screensaver_enable();
					windowmanager_exit();
					break;
				default:
					break;
			}
		break;
		case POWER_DISPLAY_READONLY:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_app_power_display();
					break;
				case E_KEY_PRESSED:
					windowmanager_exit();
					break;
				default:
					break;
			}
		break;
	}
}
