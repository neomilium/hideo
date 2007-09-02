#ifndef __APP_DATE_H__
#define __APP_DATE_H__

#include "menus.h"

menu_t date_menu;

#define app_date_init()		menu_item_t date_menu_items[] = { 																			\
														{ PSTR("Show datetime"), NULL },																			\
														{ PSTR("Ajust datetime"), NULL }																			\
													};																																			\
													date_menu.menu_items = date_menu_items;																	\
													date_menu.item_count = sizeof(date_menu_items)/sizeof(menu_item_t);			\
													application_t app_date = { app_date_exec, NULL, app_date_process_key };	\
													_app_date_init()

void _app_date_init(void);
void app_date_process_key(const byte key);

uint8 app_date_exec(void *user_data);
uint8 app_date_show_datetime(void *user_data);
uint8 app_date_ajust_datetime(void *user_data);

#endif
