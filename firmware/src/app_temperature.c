#include "app_temperature.h"

#include <avr/pgmspace.h>

#include "lcd.h"
#include "keyboard.h"
#include "thermal-sensors.h"

#include "windowmanager.h"

void
_app_temperature_display(void)
{
	const uint8_t _temperature_hqi = thermal_sensors_read(LM35_ADC_CHANNEL_HQI);
	const uint8_t _temperature_lcd = thermal_sensors_read(LM35_ADC_CHANNEL_LCD);

	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("Temp. HQI: "));
	lcd_display_number(_temperature_hqi);
	lcd_finish_line();
	lcd_display_string(PSTR("Temp. LCD: "));
	lcd_display_number(_temperature_lcd);
	lcd_finish_line();
}

void
_app_temperature_init(void *data)
{
	_app_temperature_display();
}

void
_app_temperature_event_handler(const event_t event)
{
	switch (event.code) {
		case E_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_MENU_RIGHT:
					_app_temperature_display();
					break;
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
			}
			break;
		default:
			break;
	}
}
