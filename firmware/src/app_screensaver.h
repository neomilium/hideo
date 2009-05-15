#ifndef __APP_SCREENSAVER_H__
#define __APP_SCREENSAVER_H__

#include "application.h"
#include "eventmanager.h"

#define app_screensaver_init()	application_t app_screensaver = { &_app_screensaver_init, &_app_screensaver_event_handler, NULL }

void		_app_screensaver_init(void *);
void		_app_screensaver_event_handler(const event_t event);

#endif				/* !__APP_SCREENSAVER_H__ */
