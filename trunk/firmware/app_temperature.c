#include "app_temperature.h"

#include <avr/pgmspace.h>

#include "a2d.h"
#include "lcd.h"
#include "types.h"

sint16 _temperature = -32000;

void
_app_temperature_display(void)
{
	lcd_clear();
	lcd_gotoxy(0,0);
	lcd_display_string(PSTR("T: "));
	lcd_display_number(_temperature);
}

void
_app_temperature_init(void *data)
{
	_app_temperature_display();
}

void
_app_temperature_event_handler(const event_t event)
{
	_app_temperature_display();
}
