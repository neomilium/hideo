#include "app_eeprom.h"

#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

void
_app_eeprom_init(void *data)
{
	byte buf0 = 0x00;
	byte buf1 = 0x00;
	lcd_clear();

	lcd_display_string(PSTR("   EEPROM"));
	lcd_finish_line();

	/* First read cycle */
	buf0 = eeprom_read(0);
	buf1 = eeprom_read(1);
	lcd_display_string(PSTR("r : "));
	lcd_display_hex(buf0);
	lcd_display_char(' ');
	lcd_display_hex(buf1);
	lcd_finish_line();

	/* Write cycle */
	buf0 = buf0 + 1;
	buf1 = buf1 + 3;
	eeprom_write(0, buf0);
	eeprom_write(1, buf1);
	lcd_display_string(PSTR("w : "));
	lcd_display_hex(buf0);
	lcd_display_char(' ');
	lcd_display_hex(buf1);
	lcd_finish_line();

	/* Re-read cycle */
	buf0 = 0x00;
	buf1 = 0x00;
	buf0 = eeprom_read(0);
	buf1 = eeprom_read(1);
	lcd_display_string(PSTR("r : "));
	lcd_display_hex(buf0);
	lcd_display_char(' ');
	lcd_display_hex(buf1);
	lcd_finish_line();
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
