#ifndef __MENUS_H__
#define __MENUS_H__

#include "types.h"

#include <avr/pgmspace.h>

#define MENUS_DISPLAY_MODE_FULL         0
#define MENUS_DISPLAY_MODE_CURSOR_LINE  1

typedef struct {
	prog_char *title;
	uint8 (*function)(void*);
	void *user_data;
} menu_item_t;

typedef struct {
	menu_item_t *menu_items;
	uint8 item_count;
} menu_t;

//#define menus_enter_menu(menu) _menus_enter_menu(menu, sizeof(menu) / sizeof(menu_item_t))

void menus_init(void);

void menus_enter_menu(const menu_t *menu);
void menus_leave_menu(void);

void menus_display(byte mode);
void menus_process_key(const byte key);

#endif
