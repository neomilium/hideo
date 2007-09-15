#include <avr/pgmspace.h>

#include "app_mouse.h"
#include "ps2_mouse.h"
#include "lcd.h"

#include "keyboard.h"

void
_app_mouse_init(void)
{
	ps2_mouse_init();

	for (;;) {
		ps2_mouse_poll();
	}
}

void
_app_mouse_event_handler(const event_t event)
{

}
