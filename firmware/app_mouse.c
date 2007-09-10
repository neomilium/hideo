#include <avr/pgmspace.h>

#include "app_mouse.h"
#include "ps2.h"
#include "lcd.h"

void
_app_mouse_data_read(uint8 data)
{
	lcd_display_string(PSTR("D:"));
	lcd_display_hex(data);
}

void
_app_mouse_init(void)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	//lcd_display_line(PSTR("Init"));
	ps2_set_data_read(_app_mouse_data_read);
	ps2_init();
	//lcd_display_line(PSTR("Reset"));
	//ps2_write(0xFF);           /* Mouse software reset */

	lcd_display_line(PSTR("Done :P"));
}

void
_app_mouse_event_handler(const event_t event)
{

}