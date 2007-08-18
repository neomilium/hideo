#include "app_date.h"
#include <avr/pgmspace.h>

#include "rtc.h"

#include "windowmanager.h"
#include "application.h"
#include "lcd.h"

/*********************************** DATE MENU *********************************/
const char app_date_display_text[] PROGMEM = "Afficher date";
const char app_date_config_text[] PROGMEM = "Config date";
const char app_date_exit_text[] PROGMEM = "Retour";

#define APP_DATE_MENU_ENTRIES_COUNT 3
PGM_P app_date_menu[APP_DATE_MENU_ENTRIES_COUNT] PROGMEM = {
		app_date_display_text,
		app_date_config_text,
		app_date_exit_text
};

void app_date_display_rtc_date( const rtc_datetime datetime )
{
	lcd_display_number( 20 );
	lcd_display_bcd( datetime.rtc_year );
	lcd_display_char( '-' );
	lcd_display_bcd( datetime.rtc_month );
	lcd_display_char( '-' );
	lcd_display_bcd( datetime.rtc_day );
}

void app_date_display_rtc_time( const rtc_datetime datetime )
{
	lcd_display_bcd( datetime.rtc_hour );
	lcd_display_char( ':' );
	lcd_display_bcd( datetime.rtc_min );
	lcd_display_char( ':' );
	lcd_display_bcd( datetime.rtc_sec & 0x7F );
}

void (*app_date_functions[APP_DATE_MENU_ENTRIES_COUNT]) (void) = {
	app_date_display_date,
	app_date_display_date,
	NULL
};

uint8 app_date_submenus[APP_DATE_MENU_ENTRIES_COUNT] = {
	MENU_NONE,
	MENU_NONE,
	MENU_ROOT
};

const char app_date_title[] PROGMEM = "app_date";
void app_date_init(void)
{
	rtc_init();

	t_menu menu;

	menu.level = 1;
	menu.lines = app_date_menu;
	menu.line_count = APP_DATE_MENU_ENTRIES_COUNT;
	menu.functions = app_date_functions;
	menu.submenus = app_date_submenus;

	t_application this;
	this.menu = menu;
	this.title = app_date_title;

	windowmanager_record_application(this);
}

void app_date_display_date(void)
{
	lcd_clear();
	rtc_datetime now = rtc_read_datetime();
	lcd_gotoxy(0,0);
	app_date_display_rtc_date( now );
	lcd_gotoxy(0,1);
	app_date_display_rtc_time( now );
}

