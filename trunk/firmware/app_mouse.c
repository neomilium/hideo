#include <avr/pgmspace.h>

#include "app_mouse.h"
#include "ps2_mouse.h"
#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

static sint16 _mouse_x = 0;
static sint16 _mouse_y = 0;

void
_app_mouse_init(void* data)
{
	lcd_clear();
}

void
_app_mouse_event_handler(const event_t event)
{
	lcd_gotoxy(0,0);
	lcd_display_string(PSTR("L:"));
	lcd_gotoxy(24,0);
	lcd_display_string(PSTR("M:"));
	lcd_gotoxy(48,0);
	lcd_display_string(PSTR("R:"));

	lcd_gotoxy(0,2);
	lcd_display_string(PSTR("X:"));
	lcd_gotoxy(0,3);
	lcd_display_string(PSTR("Y:"));

	switch(event.code) {
		case E_MOUSE_BUTTON_PRESSED:
			switch(event.data) {
				case MOUSE_BUTTON_LEFT:
					lcd_gotoxy(12,0);
					lcd_display_char('1');
				break;
				case MOUSE_BUTTON_MIDDLE:
					lcd_gotoxy(36,0);
					lcd_display_char('1');
				break;
				case MOUSE_BUTTON_RIGHT:
					lcd_gotoxy(60,0);
					lcd_display_char('1');
				break;
			}
		break;
		case E_MOUSE_BUTTON_RELEASED:
			switch(event.data) {
				case MOUSE_BUTTON_LEFT:
					lcd_gotoxy(12,0);
					lcd_display_char('0');
				break;
				case MOUSE_BUTTON_MIDDLE:
					lcd_gotoxy(36,0);
					lcd_display_char('0');
				break;
				case MOUSE_BUTTON_RIGHT:
					lcd_gotoxy(60,0);
					lcd_display_char('0');
				break;
			}
		break;
		case E_MOUSE_X_REV:
			_mouse_x += (unsigned)event.data;
			lcd_gotoxy(12,2);
			lcd_display_number(_mouse_x);
			lcd_finish_line();
		break;
		case E_MOUSE_X_FWD:
			_mouse_x -= (unsigned)event.data;
			lcd_gotoxy(12,2);
			lcd_display_number(_mouse_x);
			lcd_finish_line();
		break;
		case E_MOUSE_Y_REV:
			_mouse_y += (unsigned)event.data;
			lcd_gotoxy(12,3);
			lcd_display_number(_mouse_y);
			lcd_finish_line();
		break;
		case E_MOUSE_Y_FWD:
			_mouse_y -= (unsigned)event.data;
			lcd_gotoxy(12,3);
			lcd_display_number(_mouse_y);
			lcd_finish_line();
		break;
		case E_KEY_PRESSED:
			switch(event.data) {
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
			}
		break;
    default:
		break;
	}
}
