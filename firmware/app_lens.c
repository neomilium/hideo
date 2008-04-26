#include "app_lens.h"

#include <avr/pgmspace.h>

#include "lens_control.h"
#include "drv_ps2_mouse.h"
#include "lcd.h"

#include "drv_keyboard.h"

#include "windowmanager.h"

static sint16	_wanted_position = 0;

void
_app_lens_init(void *data)
{
}

void
_app_lens_event_handler(const event_t event)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("   Lens"));

	lcd_gotoxy(0, 2);
	lcd_display_string(PSTR("position: "));
	lcd_gotoxy(0, 3);
	lcd_display_string(PSTR("wanted: "));
	lcd_display_number(_wanted_position);

	switch (event.code) {
	case E_MOUSE_Y_REV:
	case E_MOUSE_Y_FWD:
		lcd_gotoxy(60, 2);
		lcd_display_number(lens_get_position());
		lcd_finish_line();
		break;
	case E_KEY_PRESSED:
		switch (event.data) {
		case KEYBOARD_UP:
			_wanted_position += 10;
			lens_set_position(_wanted_position);
			break;
		case KEYBOARD_DOWN:
			_wanted_position -= 10;
			lens_set_position(_wanted_position);
			break;
		case KEYBOARD_MENU_LEFT:
			windowmanager_exit();
		}
		break;
	default:
		break;
	}
}
