#include "app_eeprom.h"

#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

void
_app_eeprom_init(void *data)
{
	byte buf[2] = { 0x00, 0x00 };
	lcd_clear();

	lcd_display_string(PSTR("   EEPROM"));
	lcd_finish_line();

	/* First read cycle */
	eeprom_read(EEPROM_MEMMAP__DISPLAY_TRAPEZOID, 2, buf);
	lcd_display_string(PSTR("r : "));
	lcd_display_hex(buf[0]);
	lcd_display_char(' ');
	lcd_display_hex(buf[1]);
	lcd_finish_line();


	/* Write cycle */
/*
	buf[0] = buf[0] + 1;
	buf[1] = buf[1] + 3;
	eeprom_write(0x0000, buf, 2);
	lcd_display_string(PSTR("w : "));
	lcd_display_hex(buf[0]);
	lcd_display_char(' ');
	lcd_display_hex(buf[1]);
	lcd_finish_line();
*/

	/* Re-read cycle */
/*
	buf[0] = 0x00;
	buf[1] = 0x00;
	eeprom_read(0, 2, buf);
	lcd_display_string(PSTR("r : "));
	lcd_display_hex(buf[0]);
	lcd_display_char(' ');
	lcd_display_hex(buf[1]);
	lcd_finish_line();
*/
}

void
_app_eeprom_event_handler(const event_t event)
{

	switch (event.code) {
		case E_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
