#include <avr/pgmspace.h>

#include "app_poweroff.h"

#include "lcd.h"
#include "leds.h"
#include "relay.h"
#include "fans.h"

#include "windowmanager.h"

#include "display_control.h"

#define PO_SHUTDOWN_CANCEL_DELAY	10
#define PO_COOLING_HQI_DURATION		30

typedef enum {
	PO_MODE_SHUTDOWN,
	PO_MODE_COOLING_HQI,
	PO_MODE_OFF
}		poweroff_mode_t;

static poweroff_mode_t _mode = PO_MODE_SHUTDOWN;
static uint8 _remaining_time_before_shutdown_process = PO_SHUTDOWN_CANCEL_DELAY;
static uint16 _remaining_time_before_cooling_done = PO_COOLING_HQI_DURATION;

void _app_poweroff_display(void);

void
_app_poweroff_init(void *data)
{
	if(_mode == PO_MODE_SHUTDOWN) {
		_remaining_time_before_shutdown_process = PO_SHUTDOWN_CANCEL_DELAY;
		_remaining_time_before_cooling_done = PO_COOLING_HQI_DURATION;
	}
	_app_poweroff_display();
}

void
_app_poweroff_display(void)
{
	switch (_mode) {
		case PO_MODE_SHUTDOWN:
			windowmanager_screensaver_disable();
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Poweroff"));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR("Shutdown in"));
			lcd_gotoxy(0,5);
			lcd_display_string(PSTR("Key to cancel"));
			break;
		case PO_MODE_COOLING_HQI:
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR("   Poweroff"));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR("Cooling HQI..."));
			break;
		case PO_MODE_OFF:
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
		case PO_MODE_SHUTDOWN:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_remaining_time_before_shutdown_process--;
					if(_remaining_time_before_shutdown_process == 0) {
						_mode = PO_MODE_COOLING_HQI;
						_app_poweroff_display();

						RELAY0 = 0; // HQI Off
						display_lens_park();
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
		case PO_MODE_COOLING_HQI:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_remaining_time_before_cooling_done--;
					if(_remaining_time_before_cooling_done == 0) {
						_mode = PO_MODE_OFF;

						_app_poweroff_display();

						FAN0 = 0;
						FAN1 = 0;
					} else {
						lcd_gotoxy(24,3);
						lcd_display_number(_remaining_time_before_cooling_done);
						lcd_display_string(PSTR(" sec"));
						lcd_finish_line();
					}
					break;
				default: // Don't do anything before HQI cooling process done.
					break;
			}
		break;
		case PO_MODE_OFF:
			switch (event.code) {
				case E_KEY_PRESSED:
					_mode = PO_MODE_SHUTDOWN;
					windowmanager_exit();
					break;
				default:
					break;
			}
		break;
	}
}
