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
//#include "app_date.h"

#include "app_mouse.h"
#include "app_motor.h"

int main (void)
{
	cli();

//	app_date_init();
	app_mouse_init();
	app_motor_init();
	app_mainmenu_init();

//	app_temperature_init();

	//DDRB = 0xFF; // XXX
	//LED0 = LED1 = 1;

	RELAY0 = 1;

	windowmanager_init();

	sei();     /* Enable interrupts */

	windowmanager_launch(&app_mainmenu);

	for(;;) {
		windowmanager_process_events();
	}

	return 1;
}

