#include "windowmanager.h"

#include "lcd.h"
#include "keyboard.h"

#include "menus.h"

void windowmanager_init(void)
{
	lcd_init();
	keyboard_init();
	menus_init();
}

void windowmanager_record_application(const t_application app)
{
	menus_add_submenu(app.menu);
}

uint8 windowmanager_get_id(void)
{
	return 0;
}

void windowmanager_process_events(void)
{
	menus_process();
}
