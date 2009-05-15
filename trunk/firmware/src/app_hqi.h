#ifndef __APP_HQIONOFF_H__
#define __APP_HQIONOFF_H__

#include "application.h"
#include "eventmanager.h"

#define app_hqi_init() menu_t hqimenu;                \
          menu_item_t hqimenu_items[] = {          \
          { PSTR("Start HQI"), NULL },            \
          { PSTR("Stop HQI"), NULL },          \
        };                    \
        hqimenu.menu_items = hqimenu_items;           \
        hqimenu.item_count = sizeof(hqimenu_items)/sizeof(menu_item_t);   \
        application_t app_hqi = { &_app_hqi_init, &_app_hqi_event_handler, &hqimenu }

void		_app_hqi_init(void *);
void		_app_hqi_event_handler(const event_t event);

#endif				/* !__APP_HQIONOFF_H__ */
