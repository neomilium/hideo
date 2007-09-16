#ifndef _APP_MOTOR_H_
#define _APP_MOTOR_H_

#include "application.h"
#include "event.h"

#define app_motor_init()	application_t app_motor = { &_app_motor_init, &_app_motor_event_handler, NULL }


void _app_motor_init(void*);
void _app_motor_event_handler(const event_t event);

#endif /* !_APP_MOTOR_H_ */
