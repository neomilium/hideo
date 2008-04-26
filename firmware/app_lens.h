#ifndef _APP_LENS_H_
#define _APP_LENS_H_

#include "application.h"
#include "eventmanager.h"

#define app_lens_init()	application_t app_lens = { &_app_lens_init, &_app_lens_event_handler, NULL }


void		_app_lens_init(void *);
void		_app_lens_event_handler(const event_t event);

#endif				/* !_APP_LENS_H_ */
