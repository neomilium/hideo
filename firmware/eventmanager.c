#include "eventmanager.h"

#include "types.h"

#define EVENTMANAGER_MAX_POLLING_FCT		10
#define EVENTMANAGER_MAX_HANDLING_FCT		10

DECLARE_CLIST(eventmanager_queue, EVENTMANAGER_QUEUE_LENGTH * 2);

typedef void    (*_eventmanager_polling_fct) (void);
typedef void    (*_eventmanager_handling_fct) (const event_t);

static uint8	_eventmanager_polling_fct_count = 0;
static _eventmanager_polling_fct _eventmanager_polling_fcts[EVENTMANAGER_MAX_POLLING_FCT];

static uint8	_eventmanager_handling_fct_count = 0;
static _eventmanager_handling_fct _eventmanager_handling_fcts[EVENTMANAGER_MAX_HANDLING_FCT];

void
eventmanager_init(void)
{
	for (uint8 i = 0; i < EVENTMANAGER_MAX_POLLING_FCT; i++)
		_eventmanager_polling_fcts[i] = NULL;

	for (uint8 i = 0; i < EVENTMANAGER_MAX_HANDLING_FCT; i++)
		_eventmanager_handling_fcts[i] = NULL;
}

void
eventmanager_push(const event_t event)
{
	clist_write(eventmanager_queue, event.code);
	clist_write(eventmanager_queue, event.data);
}

event_t
eventmanager_pop(void)
{
	event_t		event = {
		.code = E_NONE
	};

	if (clist_read(eventmanager_queue, &(event.code))) {
		clist_read(eventmanager_queue, &(event.data));
	}
	return event;
}

void
eventmanager_add_polling_fct(void (*fct) (void))
{
	_eventmanager_polling_fcts[_eventmanager_polling_fct_count++] = fct;
}

void
eventmanager_add_handling_fct(void (*fct) (const event_t))
{
	_eventmanager_handling_fcts[_eventmanager_handling_fct_count++] = fct;
}

void
eventmanager_poll(void)
{
	for (uint8 i = 0; i < _eventmanager_polling_fct_count; i++) {
		_eventmanager_polling_fcts[i] ();
	}
}

void
eventmanager_process(void)
{
	eventmanager_poll();

	event_t		event = eventmanager_pop();

	if (event.code != E_NONE) {
		for (uint8 i = 0; i < _eventmanager_handling_fct_count; i++) {
			_eventmanager_handling_fcts[i] (event);
		}
	}
}
