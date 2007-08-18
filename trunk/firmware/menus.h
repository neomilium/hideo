#ifndef __MENUS_H__
#define __MENUS_H__

#include "types.h"
#include <avr/pgmspace.h>

#define MENU_ROOT 0
#define MENU_NONE 0

typedef struct t_menu {
	uint8 level;
	PGM_P * lines;
	void (**functions)(void);
	uint8 line_count;
	uint8 *submenus;
} t_menu;

void menus_init(void);
void menus_process(void);
void menus_add_submenu(const t_menu menu);

#endif
