#ifndef _APP_STATUS_H_
#define _APP_STATUS_H_

#include "application.h"
#include "eventmanager.h"

#define app_status_init()	application_t app_status = { &_app_status_init, &_app_status_event_handler, NULL }


void		_app_status_init(void *data);
void		_app_status_event_handler(const event_t event);

#endif				/* !_APP_TEMPLATE_H_ */
