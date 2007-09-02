#include "app_date.h"

#include "rtc.h"
#include "lcd.h"
#include "keyboard.h"
#include "windowmanager.h"

#include <util/delay.h>

static uint8 _current_menu_item = 0;

uint8 app_date_show_datetime(void *user_data)
{
	for(;;){
		lcd_clear();
		lcd_gotoxy(0,0);

		const rtc_datetime datetime = ds1307_read();

		lcd_display_string(PSTR("20"));
		lcd_display_bcd(datetime.year);
		lcd_display_char('-');
		lcd_display_bcd(datetime.month);
		lcd_display_char('-');
		lcd_display_bcd(datetime.date);

		lcd_display_char(' ');
		lcd_display_bcd(datetime.hours);
		lcd_display_char(':');
		lcd_display_bcd(datetime.minutes);
		lcd_display_char(':');
		lcd_display_bcd(datetime.seconds);

		_delay_ms(1000);
	}
	return 0;
}

uint8 app_date_ajust_datetime(void *user_data)
{
	return 0;
}

uint8 app_date_exec(void *user_data)
{
	menus_display(&date_menu, _current_menu_item);
	return 0;
}

void _app_date_init(void)
{
	rtc_init();
}

void app_date_event_handler(const byte event)
{
	switch(event) {
		case KEYBOARD_NONE:
			break;
		case KEYBOARD_UP:
			_current_menu_item = ( _current_menu_item - 1 + _current_menu.item_count ) % _current_menu.item_count;
			break;
		case KEYBOARD_DOWN:
			_current_menu_item = ( _current_menu_item + 1 ) % _current_menu.item_count;
			break;
		case KEYBOARD_MENU_LEFT:
			windowmanager_exit_current_application();
			break;
		case KEYBOARD_MENU_RIGHT:
			windowmanager_launch(_current_menu.menu_items[_current_menu_item].application);
			break;
		default:
			42;
	}
}
