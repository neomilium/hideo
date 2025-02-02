#include <avr/pgmspace.h>

#include "app_motor.h"
#include "drv_stepper-motor.h"
#include "drv_dc-motor.h"
#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

static int16_t dc_motor_speed = 0;

void
_app_motor_init(void *data)
{
	lcd_clear();
}

void
_app_motor_event_handler(const event_t event)
{
	lcd_gotoxy(0, 0);
	lcd_display_line(PSTR("  Motors"));

	switch (event.code) {
		case E_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_UP:
					lcd_gotoxy(0, 2);
					lcd_display_line(PSTR("stepper: FWD"));
					stepper_motor_move(5);
					break;
				case KEYBOARD_DOWN:
					lcd_gotoxy(0, 2);
					lcd_display_line(PSTR("stepper: REV"));
					stepper_motor_move(-5);
					break;
				case KEYBOARD_LEFT:
					lcd_gotoxy(0, 3);
					lcd_display_string(PSTR("dc-motor: UP "));
					dc_motor_speed += 10;
					lcd_display_number(dc_motor_speed);
					dc_motor_move(dc_motor_speed);
					break;
				case KEYBOARD_RIGHT:
					lcd_gotoxy(0, 3);
					lcd_display_string(PSTR("dc-motor: DOWN "));
					dc_motor_speed -= 10;
					lcd_display_number(dc_motor_speed);
					dc_motor_move(dc_motor_speed);
					break;
				case KEYBOARD_MENU_RIGHT:
					lcd_gotoxy(0, 3);
					lcd_display_line(PSTR("dc-motor: STOP"));
					dc_motor_stop();
					break;
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
			}
			break;
		default:
			break;
	}
}
