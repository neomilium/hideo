#ifndef _APP_DISPLAY_H_
#define _APP_DISPLAY_H_

#include "application.h"
#include "eventmanager.h"

#define app_display_init()	application_t app_display = { &_app_display_init, &_app_display_event_handler, NULL }


void		_app_display_init(void *);
void		_app_display_event_handler(const event_t event);

#endif				/* !_APP_DISPLAY_H_ */
