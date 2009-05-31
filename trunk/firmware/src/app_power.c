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

#define APPLICATION_TITLE		"     Power"

static power_state_t _state;
static uint8 _remaining_time_before_startup_process = POWER_STARTUP_CANCEL_DELAY;
static uint8 _remaining_time_before_shutdown_process = POWER_SHUTDOWN_CANCEL_DELAY;

void _app_power_display(void);

void
_app_power_init(void *data)
{
	_state = *((power_state_t*)data);
	switch(_state) {
		case POWER_STATE_STARTING:
			switch(hqi_mode()) {
				case HQI_MODE_READY:
					_remaining_time_before_startup_process = POWER_STARTUP_CANCEL_DELAY;
					break;
				case HQI_MODE_RUNNING:
					_state = POWER_STATE_ON;	// HQI is already started, nothing to do.
					break;
				case HQI_MODE_COOLING:
					_state = POWER_STATE_OFF;	// HQI could not be enabled, nothing to do.
			}
			break;
		case POWER_STATE_SHUTDOWNING:
			switch(hqi_mode()) {
				case HQI_MODE_READY:
					_state = POWER_STATE_OFF;	// HQI is already off, nothing to do.
					break;
				case HQI_MODE_RUNNING:
					_remaining_time_before_shutdown_process = POWER_SHUTDOWN_CANCEL_DELAY;
					break;
				case HQI_MODE_COOLING:
					_state = POWER_STATE_OFF;	// HQI is already going to be off, nothing to do.
			}
			break;
		default:
			// It should never happends since app_power should not be launched without "objective".
			break;
	}
	_app_power_display();
}

void
_app_power_display(void)
{
	switch(_state) {
		case POWER_STATE_STARTING:
			switch (hqi_mode()) {
				case HQI_MODE_READY:
					windowmanager_screensaver_disable();
					lcd_clear();
					lcd_gotoxy(0,0);
					lcd_display_string(PSTR(APPLICATION_TITLE));
					lcd_gotoxy(0,2);
					lcd_display_string(PSTR(" Startup in"));
					lcd_gotoxy(0,5);
					lcd_display_string(PSTR("Key to cancel"));
					break;
				case HQI_MODE_COOLING:
					lcd_clear();
					lcd_gotoxy(0,0);
					lcd_display_string(PSTR(APPLICATION_TITLE));
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
					_state = POWER_STATE_ON;
				break;
			}
		case POWER_STATE_ON:
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR(APPLICATION_TITLE));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR(" HiDeO is on"));
			lcd_gotoxy(0,3);
			lcd_display_string(PSTR(" Hit any key"));
			windowmanager_screensaver_enable();

		case POWER_STATE_SHUTDOWNING:
			switch (hqi_mode()) {
				case HQI_MODE_RUNNING:
					windowmanager_screensaver_disable();
					lcd_clear();
					lcd_gotoxy(0,0);
					lcd_display_string(PSTR(APPLICATION_TITLE));
					lcd_gotoxy(0,2);
					lcd_display_string(PSTR(" Shutdown in"));
					lcd_gotoxy(0,5);
					lcd_display_string(PSTR("Key to cancel"));
					break;
				case HQI_MODE_COOLING:
					lcd_clear();
					lcd_gotoxy(0,0);
					lcd_display_string(PSTR(APPLICATION_TITLE));
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
				case HQI_MODE_READY:
					_state = POWER_STATE_OFF;
				break;
			}
			break;
		case POWER_STATE_OFF:
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_display_string(PSTR(APPLICATION_TITLE));
			lcd_gotoxy(0,2);
			lcd_display_string(PSTR(" HiDeO is off"));
			lcd_gotoxy(0,3);
					lcd_display_string(PSTR(" Hit any key"));
					windowmanager_screensaver_enable();

	}

}

void
_app_power_event_handler(const event_t event)
{
	switch (_state) {
		case POWER_STATE_STARTING:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_remaining_time_before_startup_process--;
					if(0 == _remaining_time_before_startup_process) {
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
		case POWER_STATE_SHUTDOWNING:
			switch (event.code) {
				case E_SCHEDULER_TICK:
					_remaining_time_before_shutdown_process--;
					if(0 == _remaining_time_before_shutdown_process) {
						hqi_stop();
						display_lens_park();
						_app_power_display();
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
		case POWER_STATE_OFF:
		case POWER_STATE_ON:
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
