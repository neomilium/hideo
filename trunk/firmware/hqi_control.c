#include "hqi_control.h"

#include "eventmanager.h"

#include "fans.h"
#include "relay.h"

#define HQI_COOLING_DURATION		30

static hqi_mode_t _hqi_mode = HQI_MODE_READY;
static uint16 _remaining_time_before_cooling_done = HQI_COOLING_DURATION;

void _hqi_event_handler(const event_t event);

void
hqi_init(void)
{
	eventmanager_add_handling_fct(_hqi_event_handler);
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