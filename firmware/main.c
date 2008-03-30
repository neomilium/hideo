#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "windowmanager.h"
#include "menus.h"

#include "lcd.h"
#include "relay.h"

#define LED0    GET_BIT(PORTD).bit7

#include "app_mainmenu.h"
#include "app_date.h"
// #include "app_mouse.h"
#include "app_motor.h"
// #include "app_keyboard.h"


int main (void)
{
	cli();

	app_date_init();
//	app_mouse_init();
//	app_keyboard_init();
//	app_temperature_init();
	app_motor_init();
	app_mainmenu_init();


	//DDRB = 0xFF; // XXX
	LED0 = 1;

	RELAY0 = 1;

	windowmanager_init();

	sei();     /* Enable interrupts */

	windowmanager_launch(&app_mainmenu);

	for(;;) {
		windowmanager_process_events();
	}

	return 1;
}

