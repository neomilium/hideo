#ifndef __EVENT_H__
#define __EVENT_H__

#include "clist.h"

typedef enum {
		E_NONE = 0x00,

		E_KEY_PRESSED,
		E_KEY_RELEASED,

		E_MOUSE_X_OVERFLOW,
		E_MOUSE_Y_OVERFLOW,

		E_MOUSE_BUTTON_PRESSED,
		E_MOUSE_BUTTON_RELEASED,

		E_MOUSE_X_REV,
		E_MOUSE_X_FWD,

		E_MOUSE_Y_REV,
		E_MOUSE_Y_FWD
} event_code_t;

typedef struct {
	event_code_t code;
	byte data;
} event_t;

#define EVENT_QUEUE_LENGTH	15

void event_init(void);
void event_push( const event_t event );
event_t event_pop(void);
void event_add_polling_fct( void (*fct)(void) );
void event_poll(void);


#endif
