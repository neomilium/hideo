#ifndef __MENUS_H__
#define __MENUS_H__

#include <stdint.h>
#include "application.h"

#include <avr/pgmspace.h>

typedef struct {
	prog_char      *title;
	application_t  *application;
} menu_item_t;

typedef struct {
	menu_item_t    *menu_items;
	uint8_t 		item_count;
} menu_t;

void		menus_init(void);
void		menus_display(const menu_t * menu, const uint8_t current_menu_item);

#endif
