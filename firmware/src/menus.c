#include "menus.h"

#include "lcd.h"
#include "keyboard.h"
#include "windowmanager.h"

void
menus_init(void)
{
}

void
menus_display(const menu_t * menu, const uint8_t current_menu_item)
{
	uint8_t i;
	lcd_clear();
	for (i = 0; i < menu->item_count; i++) {
		lcd_gotoxy(0, i);

		if (i == current_menu_item) {
			lcd_set_mode(LCD_MODE_INVERTED);
		}
		lcd_display_line(menu->menu_items[i].title);
		lcd_set_mode(LCD_MODE_NORMAL);
	}
}
