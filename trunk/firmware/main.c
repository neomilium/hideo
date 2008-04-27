#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "eventmanager.h"
#include "windowmanager.h"
#include "menus.h"

#include "lcd.h"
#include "relay.h"

#define LED0    GET_BIT(PORTD).bit7

#include "app_mainmenu.h"
#include "app_mouse.h"
#include "app_motor.h"
#include "app_lens.h"
#include "app_date.h"

#include "lens_control.h"
#include "drv_dc-motor.h"
#include "drv_stepper-motor.h"
#include "drv_ps2_mouse.h"
#include "i2c.h"
#include "rtc.h"

int
main(void)
{
	cli();

	/*
	 * eventmanager_init() must be called before all modules that use
	 * events...
	 */
	eventmanager_init();

	/* Devices part */
	ps2_mouse_init();
/* stepper_motor_init(); */
	dc_motor_init();
	i2c_init();
	rtc_init();		/* /!\ Require i2c_init() */

	/* Daemons part */
	lens_init();		/* /!\ Require dc_motor_init(),
				 * ps2_mouse_init() */

	/* Applications part */
/* app_keyboard_init(); */
/* app_temperature_init(); */
	app_motor_init();
	app_mouse_init();
	app_lens_init();
	app_date_init();

	app_mainmenu_init();

	LED0 = 1;
	RELAY0 = 1;

	windowmanager_init();

	sei();			/* Enable interrupts */

	windowmanager_launch(&app_mainmenu);
	eventmanager_add_handling_fct(windowmanager_process_events);

	for (;;) {
		eventmanager_process();
	}

	return 1;
}
