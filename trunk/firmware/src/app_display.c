#include "app_display.h"

#include <avr/pgmspace.h>

#include "display_control.h"
#include "drv_ps2_mouse.h"

#include "lcd.h"
#include "keyboard.h"
#include "eeprom.h"

#include "windowmanager.h"

static sint16	_lens_wanted_position = 0;
static sint16	_trapezoid_wanted_position = 0;

void _app_display_update_display(void);

void
_app_display_init(void *data)
{
	eeprom_read( EEPROM_MEMMAP__DISPLAY_LENS, sizeof(_lens_wanted_position), &_lens_wanted_position );
	eeprom_read( EEPROM_MEMMAP__DISPLAY_TRAPEZOID, sizeof(_trapezoid_wanted_position), &_trapezoid_wanted_position );

	_app_display_update_display();
}

void _app_display_update_display(void)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("   Display"));

	lcd_gotoxy(0, 2);
	lcd_display_string(PSTR("focus: "));
	lcd_display_number(_lens_wanted_position);
	lcd_finish_line();
	lcd_display_string(PSTR("trapezoid: "));
	lcd_display_number(_trapezoid_wanted_position);

/*
	lcd_gotoxy(0, 5);
	lcd_display_number(lens_get_position());
*/
}

void
_app_display_event_handler(const event_t event)
{
	switch (event.code) {
		case E_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_LEFT:
					_lens_wanted_position += 10;
					display_lens_set_position(_lens_wanted_position);
					_app_display_update_display();
					break;
				case KEYBOARD_RIGHT:
					_lens_wanted_position -= 10;
					display_lens_set_position(_lens_wanted_position);
					_app_display_update_display();
					break;
				case KEYBOARD_UP:
					_trapezoid_wanted_position += 5;
					display_trapezoid_set_position(_trapezoid_wanted_position);
					_app_display_update_display();
					break;
				case KEYBOARD_DOWN:
					_trapezoid_wanted_position -= 5;
					display_trapezoid_set_position(_trapezoid_wanted_position);
					_app_display_update_display();
					break;
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
			}
			break;
		default:
			break;
	}
}
