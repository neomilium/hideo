#ifndef _APP_MAINMENU_H_
#define _APP_MAINMENU_H_

#include "application.h"
#include "eventmanager.h"

#define app_mainmenu_init()	menu_t mainmenu;								\
					menu_item_t mainmenu_items[] = {					\
					{ PSTR("Power on"), &app_poweron },					\
					{ PSTR("Status"), &app_status },					\
					{ PSTR("Display"), &app_display },					\
					{ PSTR("Power off"), &app_poweroff },					\
				};										\
				mainmenu.menu_items = mainmenu_items;						\
				mainmenu.item_count = sizeof(mainmenu_items)/sizeof(menu_item_t);		\
				application_t app_mainmenu = { &_app_mainmenu_init, &_app_mainmenu_event_handler, &mainmenu }

void		_app_mainmenu_init(void *);
void		_app_mainmenu_event_handler(const event_t);


#endif
