#ifndef __APP_POWER_H__
#define __APP_POWER_H__

#include "application.h"
#include "eventmanager.h"

#define app_power_init()	application_t app_power = { &_app_power_init, &_app_power_event_handler, NULL }

void		_app_power_init(void *);
void		_app_power_event_handler(const event_t event);

#endif				/* !__APP_POWER_H__ */
