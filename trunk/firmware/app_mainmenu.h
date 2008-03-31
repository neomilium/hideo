#ifndef __APP_MAINMENU_H__
#define __APP_MAINMENU_H__

#include "application.h"
#include "eventmanager.h"

#define app_mainmenu_init()		menu_t mainmenu;																		\
															menu_item_t mainmenu_items[] = {										\
																{ PSTR("Date"), NULL },														\
																{ PSTR("Motor"), &app_motor },											\
																{ PSTR("Mouse"), &app_mouse },											\
															};																									\
															mainmenu.menu_items = mainmenu_items;								\
															mainmenu.item_count = sizeof(mainmenu_items)/sizeof(menu_item_t);													\
															application_t app_mainmenu = { &_app_mainmenu_init, &_app_mainmenu_event_handler, &mainmenu }

void _app_mainmenu_init(void*);
void _app_mainmenu_event_handler(const event_t);


#endif
