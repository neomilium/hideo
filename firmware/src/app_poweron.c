#include <avr/pgmspace.h>

#include "app_poweron.h"

#include "lcd.h"
#include "leds.h"
#include "relay.h"
#include "fans.h"

#include "windowmanager.h"

#include "display_control.h"
#include "hqi_control.h"

#define PO_CANCEL_DELAY	5

typedef enum {
	PO_MODE_INTERACTIVE,
	PO_MODE_READONLY,
} poweron_mode_t;

static poweron_mode_t _mode;
static uint8_t _remaining_time_before_startup_process = PO_CANCEL_DELAY;

void _app_poweron_display(void);

void
_app_poweron_init(void *data)
{
	if(hqi_mode() == HQI_MODE_READY) {
		_mode = PO_MODE_INTERACTIVE;
	} else {
		_mode = PO_MODE_READONLY;
	}

	if(_mode == PO_MODE_INTERACTIVE) {
		_remaining_time_before_startup_process = PO_CANCEL_DELAY;
	}
	_app_poweron_display();
}

void
_app_poweron_display(void)
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
			uint16_t hqi_remaining_time = hqi_remaining_time_before_ready();
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
_app_poweron_event_handler(const event_t event)
{
	switch (_mode) {
		case PO_MODE_INTERACTIVE:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_remaining_time_before_startup_process--;
					if(0 == _remaining_time_before_startup_process) {
						_mode = PO_MODE_READONLY;
						hqi_start();
						display_lens_load_position();
						_app_poweron_display();
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
		case PO_MODE_READONLY:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_app_poweron_display();
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
