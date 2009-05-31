#ifndef __APP_POWER_H__
#define __APP_POWER_H__

#include "application.h"
#include "eventmanager.h"

typedef enum {
	POWER_STATE_STARTING,
	POWER_STATE_ON,
	POWER_STATE_SHUTDOWNING,
	POWER_STATE_OFF,
}		power_state_t;

#define app_power_init()	application_t app_power = { &_app_power_init, &_app_power_event_handler, NULL }

void		_app_power_init(void *);
void		_app_power_event_handler(const event_t event);

#endif				/* !__APP_POWER_H__ */
