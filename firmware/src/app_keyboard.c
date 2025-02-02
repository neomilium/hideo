#include <avr/pgmspace.h>

#include "app_keyboard.h"
#include "keyboard.h"

#include "lcd.h"

#include "windowmanager.h"

void
_app_keyboard_init(void *data)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("KEYBOARD"));
}

void
_app_keyboard_event_handler(const event_t event)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("KEY "));

	switch (event.code) {
		case E_KEY_PRESSED:
			lcd_display_string(PSTR("P:"));
			switch (event.data) {
				case KEYBOARD_UP:
					lcd_display_line(PSTR("UP"));
					break;
				case KEYBOARD_DOWN:
					lcd_display_line(PSTR("DOWN"));
					break;
				case KEYBOARD_LEFT:
					lcd_display_line(PSTR("LEFT"));
					break;
				case KEYBOARD_RIGHT:
					lcd_display_line(PSTR("RIGHT"));
					break;
				case KEYBOARD_MENU_LEFT:
					lcd_display_line(PSTR("MENU LEFT"));
					break;
				case KEYBOARD_MENU_RIGHT:
					lcd_display_line(PSTR("MENU RIGHT"));
					break;
			}
			break;

		case E_KEY_RELEASED:
			lcd_display_string(PSTR("R:"));
			switch (event.data) {
				case KEYBOARD_UP:
					lcd_display_line(PSTR("UP"));
					break;
				case KEYBOARD_DOWN:
					lcd_display_line(PSTR("DOWN"));
					break;
				case KEYBOARD_LEFT:
					lcd_display_line(PSTR("LEFT"));
					break;
				case KEYBOARD_RIGHT:
					lcd_display_line(PSTR("RIGHT"));
					break;
				case KEYBOARD_MENU_LEFT:
					lcd_display_line(PSTR("MENU LEFT"));
					break;
				case KEYBOARD_MENU_RIGHT:
					lcd_display_line(PSTR("MENU RIGHT"));
					break;
			}
			break;
		default:
			break;
	}
}
