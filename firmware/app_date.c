#include "app_date.h"

#include "rtc.h"
#include "lcd.h"
#include "drv_keyboard.h"
#include "windowmanager.h"

void
_app_date_init(void *data)
{
	app_date_show_datetime();
}

void
_app_date_event_handler(const event_t event)
{
	switch(event.code) {
		case E_KEY_PRESSED:
			switch(event.data) {
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
					break;
				default:
					break;
			}
			break;
		default:
			app_date_show_datetime();
			break;
	}
}

void
app_date_show_datetime(void)
{
	lcd_clear();
	lcd_gotoxy(0,0);

	const rtc_datetime_t datetime = rtc_read();

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
}
