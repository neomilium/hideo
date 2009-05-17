#include "windowmanager.h"

#include "lcd.h"
#include "keyboard.h"
#include "leds.h"

#define WM_MAX_APPLICATION_STACK_DEPTH 3
#define WM_SCREENSAVER_DELAY 10

static application_t *_application_stack[WM_MAX_APPLICATION_STACK_DEPTH];
static uint8	_current_depth = -1;

static uint8	_inactivity_counter = 0;


typedef enum {
	WM_SCREENSAVER_STATE_DISABLED,
	WM_SCREENSAVER_STATE_ENABLED,
	WM_SCREENSAVER_STATE_ACTIVATED,
}		wm_screensaver_state_t;
static wm_screensaver_state_t _wm_screensaver_state;

void
windowmanager_init(void)
{
	lcd_init();
	keyboard_init();

	// Backlight
	LED0 = 1;

	_wm_screensaver_state = WM_SCREENSAVER_STATE_ENABLED;
}

void
windowmanager_process_events(const event_t event)
{
	switch( _wm_screensaver_state ) {
		case WM_SCREENSAVER_STATE_DISABLED:
			_application_stack[_current_depth]->fn_event_handler(event);
			break;
		case WM_SCREENSAVER_STATE_ENABLED:
			if(event.code == E_SCHEDULER_TICK) {
				_inactivity_counter++;
			} else if((event.code == E_KEY_PRESSED) || (event.code == E_KEY_RELEASED)) {
				_inactivity_counter = 0;
			}

			if((_inactivity_counter > WM_SCREENSAVER_DELAY)) {
				lcd_clear();
				LED0 = 0; // Backlight OFF
				_wm_screensaver_state = WM_SCREENSAVER_STATE_ACTIVATED;
			} else {
				_application_stack[_current_depth]->fn_event_handler(event);
			}
			break;
		case WM_SCREENSAVER_STATE_ACTIVATED:
			switch (event.code) {
				case E_KEY_PRESSED:
					LED0 = 1; // Backlight ON
					_wm_screensaver_state = WM_SCREENSAVER_STATE_ENABLED;
					_application_stack[_current_depth]->fn_init(_application_stack[_current_depth]->user_data);
				break;
				default:
					break;
			}
			break;
	}

}

void
windowmanager_screensaver_disable(void)
{
	if(_wm_screensaver_state == WM_SCREENSAVER_STATE_ACTIVATED) {
		LED0 = 1; // Backlight ON
		windowmanager_exit();
	}
	_wm_screensaver_state = WM_SCREENSAVER_STATE_DISABLED;
}

void
windowmanager_screensaver_enable(void)
{
	_inactivity_counter = 0;
	_wm_screensaver_state = WM_SCREENSAVER_STATE_ENABLED;
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
