#include "windowmanager.h"

#include "lcd.h"
#include "keyboard.h"
#include "event.h"

#define WM_MAX_APPLICATION_STACK_DEPTH 3

static application_t* _application_stack[WM_MAX_APPLICATION_STACK_DEPTH];
static uint8 _current_depth = -1;

void windowmanager_init(void)
{
	// event_init() must be called before all modules that use events...
	event_init();

	lcd_init();
	keyboard_init();
}

void windowmanager_process_events(void)
{
	event_poll();

	event_t event = event_pop();
	if(event.code != E_NONE) {
		_application_stack[_current_depth]->fn_event_handler(event);
	}
}

void windowmanager_launch(application_t *app)
{
	if( NULL != app ) {
			++_current_depth;
		_application_stack[_current_depth] = app;
		_application_stack[_current_depth]->fn_init(_application_stack[_current_depth]->user_data);
	}
}

void windowmanager_exit(void)
{
	if (0 != _current_depth) {
		--_current_depth;
		_application_stack[_current_depth]->fn_init(_application_stack[_current_depth]->user_data);
	}
}
