#ifndef _APP_TEMPLATE_H_
#define _APP_TEMPLATE_H_

#include "application.h"
#include "eventmanager.h"

#define app_template_init()	application_t app_template = { &_app_template_init, &_app_template_event_handler, NULL }


void		_app_template_init(void *data);
void		_app_template_event_handler(const event_t event);

#endif				/* !_APP_TEMPLATE_H_ */
