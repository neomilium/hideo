#include "hqi_control.h"

#include "eventmanager.h"

#include "eeprom.h"
#include "rtc.h"

#include "fans.h"
#include "relay.h"

#define HQI_COOLING_DURATION		30

static hqi_mode_t _hqi_mode = HQI_MODE_READY;
static uint16 _remaining_time_before_cooling_done = HQI_COOLING_DURATION;

void _hqi_event_handler(const event_t event);

void
hqi_init(void)
{
	rtc_stop();		// Make sure RTC doesn't count after a failure on power supply.
	eventmanager_add_handling_fct(_hqi_event_handler);

	/* Debug */
	//hqi_lifetime_counter_reset();
}

void
hqi_lifetime_counter_reset(void)
{
	rtc_datetime_t hqi_lifetime = {
		.seconds = 0,
		.minutes = 0,
		.hours = 0,
		.day = 0,
		.date = 0,
		.month = 0,
		.year = 0
	};
	eeprom_write(EEPROM_MEMMAP__HQI_LIFETIME, &hqi_lifetime, sizeof(rtc_datetime_t));
	rtc_write(hqi_lifetime);
}

void
_hqi_lifetime_counter_start(void)
{
	rtc_datetime_t hqi_lifetime;
	eeprom_read(EEPROM_MEMMAP__HQI_LIFETIME, sizeof(rtc_datetime_t), &hqi_lifetime);
	rtc_write(hqi_lifetime);
	rtc_start();
}

void
_hqi_lifetime_counter_stop(void)
{
	rtc_datetime_t hqi_lifetime;
	rtc_stop();
	hqi_lifetime = rtc_read();
	eeprom_write(EEPROM_MEMMAP__HQI_LIFETIME, &hqi_lifetime, sizeof(rtc_datetime_t));
}

void
hqi_start(void)
{
	switch(_hqi_mode) {
		case HQI_MODE_READY:
			FAN0 = 1;	// Enable HQI fan
		
			/* Maybe FAN1 is not supposed to be here : not related to HQI */
			FAN1 = 1;	// Enable LCD fan
		
			RELAY0 = 1;	// Enable HQI

			_hqi_lifetime_counter_start();

			_hqi_mode = HQI_MODE_RUNNING;
			break;
		default:
			break;
	}
}

void
hqi_stop(void)
{
	switch(_hqi_mode) {
		case HQI_MODE_RUNNING:
			RELAY0 = 0; 	// Disable HQI
			_remaining_time_before_cooling_done = HQI_COOLING_DURATION;

			_hqi_lifetime_counter_stop();
			_hqi_mode = HQI_MODE_COOLING;
			break;
		default:
			break;
	}
}

hqi_mode_t
hqi_mode(void)
{
	return _hqi_mode;
}

uint16
hqi_remaining_time_before_ready(void)
{
	return _remaining_time_before_cooling_done;
}

void
_hqi_event_handler(const event_t event)
{
	switch(event.code) {
		case E_SCHEDULER_TICK:
			switch(_hqi_mode) {
				case HQI_MODE_COOLING:
					_remaining_time_before_cooling_done--;
					if(_remaining_time_before_cooling_done == 0) {
						FAN0 = 0;	// Disable HQI FAN
						FAN1 = 0;	// Disable LCD FAN
						_hqi_mode = HQI_MODE_READY;
					}
					break;

					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}