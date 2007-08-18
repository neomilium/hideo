#include "app_lcd.h"
#include <avr/pgmspace.h>

#include "lcd.h"

const char l_m_root_lcd_test_text[] PROGMEM = "AZERTYUIOP";
void menus_test_lcd(void)
{
	lcd_clear();
	lcd_display_string(l_m_root_lcd_test_text);
}

