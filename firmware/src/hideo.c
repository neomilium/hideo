#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "eventmanager.h"
#include "scheduler.h"
#include "windowmanager.h"
#include "menus.h"

#include "lcd.h"
#include "relay.h"
#include "i2c.h"
#include "rtc.h"
#include "eeprom.h"
#include "keyboard.h"
#include "thermal-sensors.h"
#include "drv_dc-motor.h"
#include "drv_stepper-motor.h"
#include "drv_ps2_mouse.h"
#include "drv_videocontroller.h"

/* Applications */
#include "app_mainmenu.h"

#include "app_poweron.h"
#include "app_status.h"
#include "app_display.h"
#include "app_poweroff.h"

/* Debug apps */
// #include "app_temperature.h"
// #include "app_date.h"
// #include "app_hqi.h"
// #include "app_mouse.h"
// #include "app_motor.h"
// #include "app_keyboard.h"

/* Daemons */
#include "display_control.h"
#include "hqi_control.h"

int
main(void)
{
	cli();

	/*
	 * eventmanager_init() must be called before all modules that use
	 * events...
	 */
	eventmanager_init();
	scheduler_init();

	/* Devices part */
	stepper_motor_init();
	dc_motor_init();
	i2c_init();
	rtc_init();			/* /!\ Require i2c_init() */
	eeprom_init();			/* /!\ Require i2c_init() */
	ps2_mouse_init();
	thermal_sensors_init();
	drv_videocontroller_init();

	/* Daemons part */
	display_init();		/* /!\ Require	dc_motor_init(),
				 *		stepper_motor_init(),
				 *		ps2_mouse_init(),
				 *		eeprom_init();
				 */
	hqi_init();		/* /!\ Require	rtc_init(),
						eeprom_init();
						lm35_init();
				*/

	/* Debug applications part */
// 	app_keyboard_init();
// 	app_motor_init();
// 	app_mouse_init();
// 	app_eeprom_init();
// 	app_temperature_init();
// 	app_date_init();
// 	app_hqi_init();

// 	/* Applications part */
	app_poweron_init();
	app_status_init();
	app_display_init();
	app_poweroff_init();

	app_mainmenu_init();

	windowmanager_init();

	sei();			/* Enable interrupts */

	drv_videocontroller_hook_app_poweron(&app_poweron);
	drv_videocontroller_hook_app_poweroff(&app_poweroff);

	windowmanager_launch(&app_mainmenu);
	eventmanager_add_handling_fct(windowmanager_process_events);

	for (;;) {
		eventmanager_process();
	}

	return 1;
}
