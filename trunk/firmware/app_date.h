#ifndef __APP_DATE_H__
#define __APP_DATE_H__

#include "menus.h"

#include "eventmanager.h"

menu_t date_menu;

#define app_date_init()		menu_item_t date_menu_items[] = { 																			\
														{ PSTR("Show datetime"), NULL },																			\
														{ PSTR("Ajust datetime"), NULL }																			\
													};																																			\
													date_menu.menu_items = date_menu_items;																	\
													date_menu.item_count = sizeof(date_menu_items)/sizeof(menu_item_t);			\
													application_t app_date = { &_app_date_init, &_app_date_event_handler, NULL }

void _app_date_init(void*);
void _app_date_event_handler(const event_t event);

#endif
