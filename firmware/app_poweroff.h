#ifndef __APP_POWEROFF_H__
#define __APP_POWEROFF_H__

#include "application.h"
#include "eventmanager.h"

#define app_poweroff_init()	application_t app_poweroff = { &_app_poweroff_init, &_app_poweroff_event_handler, NULL }

void		_app_poweroff_init(void *);
void		_app_poweroff_event_handler(const event_t event);

#endif				/* !__APP_POWEROFF_H__ */
