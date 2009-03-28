#include "windowmanager.h"

#include "lcd.h"
#include "keyboard.h"
#include "leds.h"

#include "app_screensaver.h"

#define WM_MAX_APPLICATION_STACK_DEPTH 3
#define WM_SCREENSAVER_DELAY 10

static application_t *_application_stack[WM_MAX_APPLICATION_STACK_DEPTH];
static uint8	_current_depth = -1;

static uint8	_inactivity_counter = 0;

// dirty hack ^^
static app_screensaver_init();

static uint8 _screesaver_is_enabled = 1;

void
windowmanager_init(void)
{
	lcd_init();
	keyboard_init();

	// Backlight
	LED0 = 1;
}

void
windowmanager_process_events(const event_t event)
{
	if( _screesaver_is_enabled ) {
		if(event.code == E_SCHEDULER_TICK) {
			_inactivity_counter++;
		} else if((event.code == E_KEY_PRESSED) || (event.code == E_KEY_RELEASED)) {
			_inactivity_counter = 0;
		}
	
		if((_inactivity_counter > WM_SCREENSAVER_DELAY) && (_application_stack[_current_depth] != &app_screensaver)) {
			windowmanager_launch(&app_screensaver);
		}
	}

	_application_stack[_current_depth]->fn_event_handler(event);
}

void
windowmanager_screensaver_disable(void)
{
	_screesaver_is_enabled = 0;
}

void
windowmanager_screensaver_enable(void)
{
	_screesaver_is_enabled = 1;
	_inactivity_counter = 0;
}

void
windowmanager_launch(application_t * app)
{
	if (NULL != app) {
		++_current_depth;
		_application_stack[_current_depth] = app;
		_application_stack[_current_depth]->fn_init(_application_stack[_current_depth]->user_data);
	}
}

void
windowmanager_exit(void)
{
	if (0 != _current_depth) {
		--_current_depth;
		_application_stack[_current_depth]->fn_init(_application_stack[_current_depth]->user_data);
	}
}
