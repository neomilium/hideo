#include <avr/pgmspace.h>

#include "app_poweroff.h"

#include "lcd.h"
#include "leds.h"
#include "relay.h"
#include "fans.h"

#include "windowmanager.h"

#include "display_control.h"
#include "hqi_control.h"

#define PO_SHUTDOWN_CANCEL_DELAY	10

typedef enum {
	PO_MODE_INTERACTIVE,
	PO_MODE_READONLY,
}		poweroff_mode_t;

static poweroff_mode_t _mode;
static uint8 _remaining_time_before_shutdown_process = PO_SHUTDOWN_CANCEL_DELAY;

void _app_poweroff_display(void);

void
_app_poweroff_init(void *data)
{
	if(hqi_mode() == HQI_MODE_RUNNING) {
		_mode = PO_MODE_INTERACTIVE;
	} else {
		_mode = PO_MODE_READONLY;
	}

	if(_mode == PO_MODE_INTERACTIVE) {
		_remaining_time_before_shutdown_process = PO_SHUTDOWN_CANCEL_DELAY;
	}
	_app_poweroff_display();
}

void
_app_poweroff_display(void)
{
	switch (hqi_mode()) {
		case HQI_MODE_RUNNING:
			windowmanager_screensaver_disable();
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Poweroff"));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR("Shutdown in"));
			lcd_gotoxy(0,5);
			lcd_display_string(PSTR("Key to cancel"));
			break;
		case HQI_MODE_COOLING:
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Poweroff"));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR("Cooling HQI..."));
			lcd_gotoxy(24,3);
			lcd_display_number(hqi_remaining_time_before_ready());
			lcd_display_string(PSTR(" sec"));
			lcd_finish_line();
			break;
		case HQI_MODE_READY:
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Poweroff"));
			lcd_gotoxy(0,3);
			lcd_display_string(PSTR(" Hit any key"));
			windowmanager_screensaver_enable();
		break;
	}
}

void
_app_poweroff_event_handler(const event_t event)
{
	switch (_mode) {
		case PO_MODE_INTERACTIVE:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_remaining_time_before_shutdown_process--;
					if(0 == _remaining_time_before_shutdown_process) {
						_mode = PO_MODE_READONLY;
						hqi_stop();
						display_lens_park();
						_app_poweroff_display();
					} else {
						lcd_gotoxy(24,3);
						lcd_display_number(_remaining_time_before_shutdown_process);
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
		case PO_MODE_READONLY:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_app_poweroff_display();
					break;
				case E_KEY_PRESSED:
					windowmanager_exit();
					break;
				default: // Don't do anything before HQI cooling process done.
					break;
			}
		break;
	}
}
