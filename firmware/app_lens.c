#include "app_lens.h"

#include <avr/pgmspace.h>

#include "lens_control.h"
#include "drv_ps2_mouse.h"
#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

static sint16	_wanted_position = 0;

void _app_lens_update_display(void);

void
_app_lens_init(void *data)
{
	_app_lens_update_display();
}

void _app_lens_update_display(void)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("   Display"));

	lcd_gotoxy(0, 2);
	lcd_display_string(PSTR("focus: "));

	lcd_display_number(_wanted_position);

/*
	lcd_gotoxy(0, 3);
	lcd_display_number(lens_get_position());
*/
}

void
_app_lens_event_handler(const event_t event)
{
	switch (event.code) {
		case E_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_UP:
					_wanted_position += 10;
					lens_set_position(_wanted_position);
					_app_lens_update_display();
					break;
				case KEYBOARD_DOWN:
					_wanted_position -= 10;
					lens_set_position(_wanted_position);
					_app_lens_update_display();
					break;
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
			}
			break;
		default:
			break;
	}
}
