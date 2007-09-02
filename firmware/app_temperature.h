#ifndef _APP_TEMPERATURE_H_
#define _APP_TEMPERATURE_H_

#include "application.h"

#define app_temperature_init()	application_t app_temperature = { &_app_temperature_init, &_app_temperature_event_handler, NULL }

void _app_temperature_init(void *data);
void _app_temperature_event_handler(const event_t event);


#endif /* !_APP_TEMPERATURE_H_ */
