#ifndef __APP_KEYBOARD_H__
#define __APP_KEYBOARD_H__

#include "application.h"
#include "eventmanager.h"

#define app_keyboard_init()	application_t app_keyboard = { &_app_keyboard_init, &_app_keyboard_event_handler, NULL }

void _app_keyboard_init(void*);
void _app_keyboard_event_handler(const event_t event);

#endif /* !__APP_KEYBOARD_H__ */
