#include <avr/pgmspace.h>

#include "app_mouse.h"
#include "ps2.h"
#include "lcd.h"

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
		if (ps2_buffer_read(&data)) {
			lcd_display_hex(data);
			lcd_display_string(PSTR(";"));
		}
	}
}

void
_app_mouse_event_handler(const event_t event)
{

}