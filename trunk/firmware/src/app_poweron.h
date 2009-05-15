#ifndef __APP_POWERON_H__
#define __APP_POWERON_H__

#include "application.h"
#include "eventmanager.h"

#define app_poweron_init()	application_t app_poweron = { &_app_poweron_init, &_app_poweron_event_handler, NULL }

void		_app_poweron_init(void *);
void		_app_poweron_event_handler(const event_t event);

#endif				/* !__APP_POWERON_H__ */
