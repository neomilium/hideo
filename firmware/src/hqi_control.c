#include "hqi_control.h"

#include "eventmanager.h"

#include "eeprom.h"
#include "rtc.h"
#include "thermal-sensors.h"

#include "fans.h"
#include "relay.h"

#define HQI_COOLING_DURATION		15*60

#define HQI_TEMPERATURE_MAX_AT_INIT	40

static hqi_mode_t _hqi_mode = HQI_MODE_READY;
static uint16 _remaining_time_before_cooling_done = HQI_COOLING_DURATION;

void _hqi_event_handler(const event_t event);

void
hqi_init(void)
{
	RELAY0 = 0;		// Make sure HQI is disabled after a failure on power supply.
	rtc_stop();		// Make sure RTC doesn't count after a failure on power supply.

	if(hqi_temperature() > HQI_TEMPERATURE_MAX_AT_INIT) {
		fans_hqi_start();	// Enable HQI fan

		/* Maybe FAN1 is not supposed to be here : not related to HQI */
		fans_lcd_start();	// Enable LCD fan
		
		_remaining_time_before_cooling_done = HQI_COOLING_DURATION;
		_hqi_mode = HQI_MODE_COOLING;
	}

	eventmanager_add_handling_fct(_hqi_event_handler);

	/* Debug */
// 	hqi_uptime_counter_reset();
}

void
hqi_uptime_counter_reset(void)
{
	rtc_datetime_t hqi_uptime = {
		.seconds = 0,
		.minutes = 0,
		.hours = 0,
		.day = 0,
		.date = 0,
		.month = 0,
		.year = 0
	};
	eeprom_write(EEPROM_MEMMAP__HQI_LIFETIME, &hqi_uptime, sizeof(rtc_datetime_t));
	rtc_write(hqi_uptime);
}

void
_hqi_uptime_counter_start(void)
{
	rtc_datetime_t hqi_uptime;
	eeprom_read(EEPROM_MEMMAP__HQI_LIFETIME, sizeof(rtc_datetime_t), &hqi_uptime);
	rtc_write(hqi_uptime);
	rtc_start();
}

uint32
hqi_uptime(void)
{
	rtc_datetime_t hqi_uptime;
	uint32 hqi_uptime_in_sec;

	switch(_hqi_mode) {
		case HQI_MODE_RUNNING:
			hqi_uptime = rtc_read();
			break;
		default:
			eeprom_read(EEPROM_MEMMAP__HQI_LIFETIME, sizeof(rtc_datetime_t), &hqi_uptime);
			break;
	}
	hqi_uptime_in_sec = hqi_uptime.seconds;
	hqi_uptime_in_sec += hqi_uptime.minutes * 60;
	hqi_uptime_in_sec += hqi_uptime.hours * 60 * 60;
	hqi_uptime_in_sec += hqi_uptime.date * 60 * 60 * 24;
	return hqi_uptime_in_sec;
}

uint8
hqi_temperature(void)
{
	return thermal_sensors_read(LM35_ADC_CHANNEL_HQI);
}

void
_hqi_uptime_counter_stop(void)
{
	rtc_datetime_t hqi_uptime;
	rtc_stop();
	hqi_uptime = rtc_read();
	eeprom_write(EEPROM_MEMMAP__HQI_LIFETIME, &hqi_uptime, sizeof(rtc_datetime_t));
}

void
hqi_start(void)
{
	switch(_hqi_mode) {
		case HQI_MODE_READY:
			fans_hqi_start();	// Enable HQI fan
		
			/* Maybe FAN1 is not supposed to be here : not related to HQI */
			fans_lcd_start();	// Enable LCD fan
		
			RELAY0 = 1;	// Enable HQI

			_hqi_uptime_counter_start();

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

			_hqi_uptime_counter_stop();
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
						fans_hqi_stop();	// Disable HQI FAN
						fans_lcd_stop();	// Disable LCD FAN
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