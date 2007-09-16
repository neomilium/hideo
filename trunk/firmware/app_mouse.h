#ifndef _APP_MOUSE_H_
#define _APP_MOUSE_H_

#include "application.h"
#include "event.h"

#define app_mouse_init()	application_t app_mouse = { &_app_mouse_init, &_app_mouse_event_handler, NULL }


void _app_mouse_init(void*);
void _app_mouse_event_handler(const event_t event);

#endif /* !_APP_MOUSE_H_ */
