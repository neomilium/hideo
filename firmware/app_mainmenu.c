#include "app_mainmenu.h"
#include "menus.h"
#include "keyboard.h"

static uint8 _menu_current_item = 0;

static menu_t* _menu;

void _app_mainmenu_init(void* menu)
{
	_menu = menu;
	menus_display(_menu, _menu_current_item);
}

void _app_mainmenu_event_handler(const event_t event)
{
	if ( E_KEY_PRESSED == event.code ) {
		switch(event.data) {
			case KEYBOARD_UP:
				_menu_current_item = ( _menu_current_item - 1 + _menu->item_count ) % _menu->item_count;
				menus_display(_menu, _menu_current_item);
				break;
			case KEYBOARD_DOWN:
				_menu_current_item = ( _menu_current_item + 1 ) % _menu->item_count;
				menus_display(_menu, _menu_current_item);
				break;
			case KEYBOARD_MENU_LEFT:
	// 			windowmanager_exit_current_application();
				break;
			case KEYBOARD_MENU_RIGHT:
	// 			windowmanager_launch(_current_menu.menu_items[_current_menu_item].application);
				break;
		}
	}
}
