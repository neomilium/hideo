#include <avr/pgmspace.h>

#include "app_motor.h"
#include "drv_stepper-motor.h"
#include "drv_dc-motor.h"
#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

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
			/*
			 * case E_MOUSE_X_REV: _motor_x +=
			 * (unsigned)event.data; lcd_gotoxy(12,2);
			 * lcd_display_number(_motor_x); lcd_finish_line();
			 * break; case E_MOUSE_X_FWD: _motor_x -=
			 * (unsigned)event.data; lcd_gotoxy(12,2);
			 * lcd_display_number(_motor_x); lcd_finish_line();
			 * break; case E_MOUSE_Y_REV: _motor_y +=
			 * (unsigned)event.data; lcd_gotoxy(12,3);
			 * lcd_display_number(_motor_y); lcd_finish_line();
			 * break; case E_MOUSE_Y_FWD: _motor_y -=
			 * (unsigned)event.data; lcd_gotoxy(12,3);
			 * lcd_display_number(_motor_y); lcd_finish_line();
			 * break;
			 */
		case E_KEY_PRESSED:
			switch (event.data) {
				case KEYBOARD_LEFT:
					lcd_gotoxy(0, 2);
					lcd_display_line(PSTR("stepper: FWD"));
					stepper_motor_move(50);
					break;
				case KEYBOARD_RIGHT:
					lcd_gotoxy(0, 2);
					lcd_display_line(PSTR("stepper: REV"));
					stepper_motor_move(-30);
					break;
				case KEYBOARD_UP:
					lcd_gotoxy(0, 3);
					lcd_display_line(PSTR("dc-motor: FWD"));
					dc_motor_move(50);
					break;
				case KEYBOARD_DOWN:
					lcd_gotoxy(0, 3);
					lcd_display_line(PSTR("dc-motor: REV"));
					dc_motor_move(-50);
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
