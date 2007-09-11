#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

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

#include "app_mainmenu.h"
//#include "app_temperature.h"
// #include "app_date.h"
#include "app_mouse.h"

int main (void)
{
	cli();
	
// 	menu_t foo_menu;
// 	/// app_foo
// 	menu_item_t foo_menu_items[] = {
// 		{ PSTR("plop"), &app_bar },
// 		{ PSTR("toglut"), &app_bar },
// 		{ PSTR("kartaglutek"), &app_bar }
// 	};
// 	foo_menu.menu_items = foo_menu_items;
// 	foo_menu.item_count = sizeof(foo_menu_items)/sizeof(menu_item_t);
// 	application_t app_foo = { NULL, (void*)&foo_menu, NULL };

	/// app_date
// 	app_date_init();
//	app_mainmenu_init();

//	app_temperature_init();
	app_mouse_init();

	//DDRB = 0xFF; // XXX
	//LED0 = LED1 = 1;

	RELAY0 = 1;

	windowmanager_init();
	//menus_init();
	//windowmanager_launch(&app_temperature);

	sei();     /* Enable interrupts */

	windowmanager_launch(&app_mouse);

	for(;;) {
		windowmanager_process_events();
	}

	return 1;
}

