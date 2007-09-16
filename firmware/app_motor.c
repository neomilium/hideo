#include <avr/pgmspace.h>

#include "app_motor.h"
#include "stepper_motor.h"
#include "lcd.h"

#include "keyboard.h"

#include "windowmanager.h"

void
_app_motor_init(void* data)
{
	lcd_clear();
	stepper_motor_init();
}

void
_app_motor_event_handler(const event_t event)
{
	lcd_gotoxy(0,0);
	lcd_display_string(PSTR("P:"));

	switch(event.code) {
/*
		case E_MOUSE_X_REV:
			_motor_x += (unsigned)event.data;
			lcd_gotoxy(12,2);
			lcd_display_number(_motor_x);
			lcd_finish_line();
		break;
		case E_MOUSE_X_FWD:
			_motor_x -= (unsigned)event.data;
			lcd_gotoxy(12,2);
			lcd_display_number(_motor_x);
			lcd_finish_line();
		break;
		case E_MOUSE_Y_REV:
			_motor_y += (unsigned)event.data;
			lcd_gotoxy(12,3);
			lcd_display_number(_motor_y);
			lcd_finish_line();
		break;
		case E_MOUSE_Y_FWD:
			_motor_y -= (unsigned)event.data;
			lcd_gotoxy(12,3);
			lcd_display_number(_motor_y);
			lcd_finish_line();
		break;
*/
		case E_KEY_PRESSED:
			switch(event.data) {
				case KEYBOARD_UP:
					lcd_display_line(PSTR("UP"));
					stepper_motor_move(10);
				break;
				case KEYBOARD_DOWN:
					lcd_display_line(PSTR("DOWN"));
					stepper_motor_move(-10);
				break;
				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
			}
		break;
	}
}
