#include <avr/io.h>

#include "windowmanager.h"
#include "menus.h"

#include "lcd.h"

/** relay.h */
#include <avr/io.h>
#include "bit_field.h"

#define RELAY0    GET_BIT(PORTA).bit5
/** EOF **/

// XXX
#define LED0    GET_BIT(PORTB).bit2
#define LED1    GET_BIT(PORTB).bit3

menu_t main_menu;
menu_t foo_menu;

uint8 foo(void *user_data)
{
	menus_enter_menu(&foo_menu);
	return 0;
}

uint8 bar(void *user_data)
{
	return 0;
}

int main (void)
{
	menu_item_t main_menu_items[] = {
		{ PSTR("foo"), foo, NULL },
		{ PSTR("bar"), bar, NULL }
	};
	
	menu_item_t foo_menu_items[] = {
		{ PSTR("plop"), bar, NULL },
		{ PSTR("toglut"), bar, NULL},
		{ PSTR("kartaglutek"), bar, NULL }
	};

	main_menu.menu_items = main_menu_items;
	main_menu.item_count = sizeof(main_menu_items)/sizeof(menu_item_t);

	foo_menu.menu_items = foo_menu_items;
	foo_menu.item_count = sizeof(foo_menu_items)/sizeof(menu_item_t);

	DDRB = 0xFF; // XXX
	LED0 = LED1 = 1;

	RELAY0 = 1;

	windowmanager_init(&main_menu);

	for(;;) {
		windowmanager_process_events();
	}

	return 1;
}

