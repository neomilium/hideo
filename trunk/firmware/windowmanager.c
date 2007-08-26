#include "windowmanager.h"

#include "lcd.h"
#include "keyboard.h"

void windowmanager_init(const menu_t *menu)
{
	lcd_init();
	keyboard_init();
	menus_init();

	menus_enter_menu(menu);
}

void windowmanager_process_events(void)
{
	byte key = keyboard_key();
	if(key != KEYBOARD_NONE)
		menus_process_key(key);
}
