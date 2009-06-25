#include "app_status.h"

#include "windowmanager.h"

#include "lcd.h"
#include "keyboard.h"
#include "thermal-sensors.h"
#include "hqi_control.h"

void _app_status_display(void);

void
_app_status_init(void *data)
{
	windowmanager_screensaver_disable();
	_app_status_display();
}

void
_app_status_event_handler(const event_t event)
{
	switch (event.code) {
		case E_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_MENU_LEFT:
					windowmanager_screensaver_enable();
					windowmanager_exit();
					break;
				default:
					_app_status_display();
					break;
			}
			break;
		case E_SCHEDULER_TICK:
			_app_status_display();
			break;
		default:
			break;
	}
}

void
_app_status_display(void)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_set_mode(LCD_MODE_INVERTED);
	lcd_display_string(PSTR("     HQI"));
	lcd_finish_line();
	lcd_set_mode(LCD_MODE_NORMAL);

	lcd_gotoxy(0, 1);
	lcd_display_string(PSTR("Status "));
	switch(hqi_mode()) {
		case HQI_MODE_READY:
		lcd_display_string(PSTR("  Ready"));
		break;
		case HQI_MODE_RUNNING:
		lcd_display_string(PSTR("Running"));
		break;
		case HQI_MODE_COOLING:
		lcd_display_string(PSTR("Cooling"));
		break;
	}

	lcd_gotoxy(0, 2);
	lcd_display_string(PSTR("Uptime  "));
	uint32_t _hqi_uptime = hqi_uptime();
	if(_hqi_uptime < 60) {
		lcd_display_string(PSTR("   "));
		lcd_display_number(_hqi_uptime);
		lcd_display_string(PSTR("s"));
	} else if (_hqi_uptime < 3600) {
		lcd_display_string(PSTR("   "));
		lcd_display_number(_hqi_uptime / 60);
		lcd_display_string(PSTR("m"));
	} else {
		if((_hqi_uptime / 3600) < 10000) lcd_display_string(PSTR(" "));
		if((_hqi_uptime / 3600) < 1000) lcd_display_string(PSTR(" "));
		if((_hqi_uptime / 3600) < 100) lcd_display_string(PSTR(" "));
		if((_hqi_uptime / 3600) < 10) lcd_display_string(PSTR(" "));

		lcd_display_number(_hqi_uptime / 3600);
		lcd_display_string(PSTR("h"));
	}

	lcd_gotoxy(0, 3);
	lcd_display_string(PSTR("Temp.     "));
	lcd_display_number(hqi_temperature());
	lcd_display_string(PSTR("`C"));

	lcd_gotoxy(0, 4);
	lcd_set_mode(LCD_MODE_INVERTED);
	lcd_display_string(PSTR("     LCD"));
	lcd_finish_line();
	lcd_set_mode(LCD_MODE_NORMAL);


	lcd_gotoxy(0, 5);
	lcd_display_string(PSTR("Temp.     "));
	/* @TODO This code should not be here */
	lcd_display_number(thermal_sensors_read(LM35_ADC_CHANNEL_LCD));

	lcd_display_string(PSTR("`C"));
}
