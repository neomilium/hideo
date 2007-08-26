#include "menus.h"

#include "lcd.h"
#include "keyboard.h"

#define MENU_MAX_DEPTH 3

typedef struct {
	menu_item_t *menu_items;
	uint8 item_count;
	uint8 active_line;
} menu_stack_t;

static menu_stack_t _menus[MENU_MAX_DEPTH];
static sint8 _current_depth = -1;

uint8 menus_enter_menu(const menu_t *menu)
{
	++_current_depth;
	_menus[_current_depth].menu_items  = menu->menu_items;
	_menus[_current_depth].item_count  = menu->item_count;
	_menus[_current_depth].active_line = 0;

	menus_display();
	return 0;
}

void menus_leave_menu(void)
{
	if (0 != _current_depth) {
		--_current_depth;
		menus_display();
	}
}

void menus_display(void)
{
	uint8 i;
	lcd_clear();
	for(i=0; i<_menus[_current_depth].item_count; i++) {
		lcd_gotoxy(0,i);

		if(i==_menus[_current_depth].active_line) {
			lcd_set_mode(LCD_MODE_INVERTED);
		}
		lcd_display_string(_menus[_current_depth].menu_items[i].title);
		lcd_finish_line();
		lcd_set_mode(LCD_MODE_NORMAL);
	}
}

void menus_init(void)
{
}

void menus_shutdown_lcd(void)
{
	lcd_clear();
}

void menus_process_key(const byte key)
{
	switch(key) {
		case KEYBOARD_NONE:
			break;
		case KEYBOARD_UP:
			_menus[_current_depth].active_line = ( _menus[_current_depth].active_line - 1 + _menus[_current_depth].item_count ) % _menus[_current_depth].item_count;
			menus_display();
			break;
		case KEYBOARD_DOWN:
			_menus[_current_depth].active_line = ( _menus[_current_depth].active_line + 1 ) % _menus[_current_depth].item_count;
			menus_display();
			break;
		case KEYBOARD_MENU_LEFT:
			menus_leave_menu();
			break;
		case KEYBOARD_MENU_RIGHT:
			_menus[_current_depth].menu_items[_menus[_current_depth].active_line].function(_menus[_current_depth].menu_items[_menus[_current_depth].active_line].user_data);
			break;
		default:
			42;
	}
}
