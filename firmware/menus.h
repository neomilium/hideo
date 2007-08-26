#ifndef __MENUS_H__
#define __MENUS_H__

#include "types.h"

#include <avr/pgmspace.h>

typedef struct {
	prog_char *title;
	uint8 (*function)(void*);
	void *user_data;
} menu_item_t;

typedef struct {
	menu_item_t *menu_items;
	uint8 item_count;
} menu_t;

void menus_init(void);

uint8 menus_enter_menu(const menu_t *menu);
void menus_leave_menu(void);

void menus_display();
void menus_process_key(const byte key);

#endif
