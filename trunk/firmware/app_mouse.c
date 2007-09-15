#include <avr/pgmspace.h>

#include "app_mouse.h"
#include "ps2.h"
#include "lcd.h"

#include "keyboard.h"

static int _received_bytes = 0;

void
_app_mouse_data_read(uint8 data)
{ /* XXX REMOVE THIS */
	//lcd_gotoxy(1, 0);
	//lcd_display_string(PSTR(" R:"));
	//lcd_display_hex(data);
}

void
_app_mouse_init(void)
{
	//lcd_clear();
	//lcd_gotoxy(0, 0);
	//lcd_display_line(PSTR("Init"));
	//ps2_set_data_read(_app_mouse_data_read);
	ps2_init();
	//lcd_display_line(PSTR("Reset"));
	//ps2_write(0xFF);           /* Mouse software reset */

	//lcd_display_line(PSTR("Done :P"));
	for (;;) {
		int data;
		ps2_debug();

		lcd_gotoxy(0, 2);
		if (ps2_buffer_read(&data)) {
			lcd_display_number(++_received_bytes );
			lcd_display_char(':');
			lcd_display_hex(data);
			lcd_display_string(PSTR(";"));
		}

		byte key = keyboard_key();
		if(key != KEYBOARD_NONE) {
			if (ps2_write(0xF4, 0)) {
				lcd_display_string(PSTR("=1"));
			} else {
				lcd_display_string(PSTR("=0"));
			}
		}
	}
}

void
_app_mouse_event_handler(const event_t event)
{

}