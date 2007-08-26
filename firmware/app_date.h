#ifndef __APP_DATE_H__
#define __APP_DATE_H__

#include "menus.h"

menu_t date_menu;

#define app_date_init()		menu_item_t date_menu_items[] = { 																\
														{ PSTR("Show datetime"), app_date_show_datetime, NULL },				\
														{ PSTR("Ajust datetime"), app_date_ajust_datetime, NULL}				\
													};																																\
													date_menu.menu_items = date_menu_items;															\
													date_menu.item_count = sizeof(date_menu_items)/sizeof(menu_item_t);	\
													_app_date_init()

void _app_date_init(void);
void app_date_process_key(const byte key);

uint8 app_date(void *user_data);
uint8 app_date_show_datetime(void *user_data);
uint8 app_date_ajust_datetime(void *user_data);

#endif
