#ifndef _APP_DATE_H_
#define _APP_DATE_H_

#include "application.h"
#include "eventmanager.h"

#define app_date_init()	application_t app_date = { &_app_date_init, &_app_date_event_handler, NULL }

void		_app_date_init(void *data);
void		_app_date_event_handler(const event_t event);
void		app_date_show_datetime(void);

#endif
