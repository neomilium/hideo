#include "app_date.h"

#include "rtc.h"
#include "lcd.h"
#include <util/delay.h>

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

uint8 app_date(void *user_data)
{
	menus_enter_menu(&date_menu);
	return 0;
}

void _app_date_init(void)
{
	rtc_init();
}

