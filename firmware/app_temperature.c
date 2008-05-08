#include "app_temperature.h"

#include <avr/pgmspace.h>

#include "types.h"

#include "lcd.h"
#include "keyboard.h"

#include "windowmanager.h"
#include "a2d.h"


#define _TEMPERATURE_ADC_HQI		ADC_CH_ADC6
#define _TEMPERATURE_ADC_LCD		ADC_CH_ADC7

void
_app_temperature_display(void)
{
	const uint8 _temperature_hqi = (a2dConvert10bit(_TEMPERATURE_ADC_HQI) / 2) + 2;
	const uint8 _temperature_lcd = (a2dConvert10bit(_TEMPERATURE_ADC_LCD) / 2) + 2;
	const uint8 _temperature_ref = a2dConvert10bit(ADC_CH_122V) / 2;

	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("Temp. HQI: "));
	lcd_display_number(_temperature_hqi);
	lcd_finish_line();
	lcd_display_string(PSTR("Temp. LCD: "));
	lcd_display_number(_temperature_lcd);
	lcd_finish_line();
	lcd_display_string(PSTR("Temp. Ref: "));
	lcd_display_number(_temperature_ref);
}

void
_app_temperature_init(void *data)
{
	register_set(DDRA, 0x00, 0b11000000);
	register_set(PORTA, 0x00, 0b11000000);

	a2dInit();
	a2dSetReference(ADC_REFERENCE_AREF);

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
