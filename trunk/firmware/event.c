#include "event.h"

#include "types.h"

#define EVENT_MAX_POLLING_FCT 10

DECLARE_CLIST(event_queue, EVENT_QUEUE_LENGTH*2);

typedef void (*_event_polling_fct)(void);

static uint8 _event_polling_fct_count = 0;
static _event_polling_fct _event_polling_fcts[EVENT_MAX_POLLING_FCT];

void event_init(void)
{
	for( uint8 i=0; i<EVENT_MAX_POLLING_FCT; i++)
		_event_polling_fcts[i] = NULL;
}

void event_push( const event_t event )
{
	clist_write(event_queue, event.code);
	clist_write(event_queue, event.data);
}

event_t event_pop( void )
{
	event_t event = {
		.code = E_NONE
	};

	if ( clist_read(event_queue, &(event.code)) ) {
		clist_read(event_queue, &(event.data));
	}

	return event;
}

void event_add_polling_fct( void (*fct)(void) )
{
	_event_polling_fcts[_event_polling_fct_count++] = fct;
}

void event_poll(void)
{
	for(uint8 i=0; i<_event_polling_fct_count; i++) {
		_event_polling_fcts[i]();
	}
}
