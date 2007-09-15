#include <avr/pgmspace.h>

#include "app_mouse.h"
#include "ps2_mouse.h"
#include "lcd.h"

#include "keyboard.h"

void
_app_mouse_init(void)
{
	ps2_mouse_init();
}

void
_app_mouse_event_handler(const event_t event)
{
	lcd_gotoxy(4,4);
	lcd_display_char('n');
	switch(event.code){
		case E_MOUSE_BUTTON_PRESSED:
			lcd_gotoxy(4,4);
			lcd_display_char('X');
			break;
		case E_MOUSE_BUTTON_RELEASED:
			lcd_gotoxy(4,4);
			lcd_display_char(' ');
			break;
	}
}
