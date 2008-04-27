#include <avr/pgmspace.h>

#include "app_eeprom.h"
#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

void
_app_eeprom_init(void *data)
{
	byte buf[2] = { 0x00, 0x11 };
	lcd_clear();
	eeprom_setpos(0);
	eeprom_write(&buf, 2);
	eeprom_setpos(0);
	buf[0] = 0x22;
	buf[1] = 0x33;
	eeprom_read(&buf, 2);
	lcd_display_hex(buf[0]);
	lcd_display_hex(buf[1]);
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
