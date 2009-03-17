#ifndef __APP_HQIONOFF_H__
#define __APP_HQIONOFF_H__

#include "application.h"
#include "eventmanager.h"

#define app_hqionoff_init()	application_t app_hqionoff = { &_app_hqionoff_init, &_app_hqionoff_event_handler, NULL }

void		_app_hqionoff_init(void *);
void		_app_hqionoff_event_handler(const event_t event);

#endif				/* !__APP_HQIONOFF_H__ */
