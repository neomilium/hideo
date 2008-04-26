#include "drv_stepper-motor.h"

#include <avr/io.h>
#include <util/delay.h>

#include "bit_field.h"

/* #include "lcd.h" */

/* 0b00000100 */
#define STEPPER_ELECTROMAGNETS_POSITION1		0x04
/* 0b00001100 */
#define STEPPER_ELECTROMAGNETS_POSITION2		0x0C
/* 0b00001000 */
#define STEPPER_ELECTROMAGNETS_POSITION3		0x08
/* 0b00011000 */
#define STEPPER_ELECTROMAGNETS_POSITION4		0x18
/* 0b00010000 */
#define STEPPER_ELECTROMAGNETS_POSITION5		0x10
/* 0b00110000 */
#define STEPPER_ELECTROMAGNETS_POSITION6		0x30
/* 0b00100000 */
#define STEPPER_ELECTROMAGNETS_POSITION7		0x20
/* 0b00100100 */
#define STEPPER_ELECTROMAGNETS_POSITION8		0x24

static const byte _stepper_motor_positions[] = {
	STEPPER_ELECTROMAGNETS_POSITION1,
	STEPPER_ELECTROMAGNETS_POSITION2,
	STEPPER_ELECTROMAGNETS_POSITION3,
	STEPPER_ELECTROMAGNETS_POSITION4,
	STEPPER_ELECTROMAGNETS_POSITION5,
	STEPPER_ELECTROMAGNETS_POSITION6,
	STEPPER_ELECTROMAGNETS_POSITION7,
	STEPPER_ELECTROMAGNETS_POSITION8
};

static uint8	_stepper_motor_current_position = 0;

#define STEPPER_MOTOR_ENABLE         GET_BIT(PORTC).bit6

void
stepper_motor_init(void)
{
	STEPPER_MOTOR_DDR = (STEPPER_ELECTROMAGNETS_MASK | STEPPER_MOTORS_ENABLE_MASK);
	STEPPER_MOTOR_ENABLE = 0;
}

void
stepper_motor_increment(void)
{
	byte		port = STEPPER_MOTOR_PORT & (~STEPPER_ELECTROMAGNETS_MASK);
	port |= ((_stepper_motor_positions[_stepper_motor_current_position]) & STEPPER_ELECTROMAGNETS_MASK);
	STEPPER_MOTOR_PORT = port;
	_stepper_motor_current_position = (_stepper_motor_current_position + 1) % 8;
}

void
stepper_motor_decrement(void)
{
	byte		port = STEPPER_MOTOR_PORT & (~STEPPER_ELECTROMAGNETS_MASK);
	port |= ((_stepper_motor_positions[_stepper_motor_current_position]) & STEPPER_ELECTROMAGNETS_MASK);
	STEPPER_MOTOR_PORT = port;
	_stepper_motor_current_position = (_stepper_motor_current_position + 8 - 1) % 8;
}

void
stepper_motor_move(sint16 steps)
{
	STEPPER_MOTOR_ENABLE = 1;
	if (steps >= 0) {
		for (sint16 i = steps; i > 0; i--) {
			stepper_motor_increment();
			for (uint8 d = 0; d < 100; d++)
				_delay_us(10);
		}
	} else {
		for (sint16 i = steps; i < 0; i++) {
			stepper_motor_decrement();
			for (uint8 d = 0; d < 100; d++)
				_delay_us(10);
		}
	}
	/*
	 * lcd_gotoxy(0,2); lcd_display_string(PSTR("Port:"));
	 * lcd_display_hex(STEPPER_MOTOR_PORT); lcd_gotoxy(0,3);
	 * lcd_display_string(PSTR("Pos:"));
	 * lcd_display_number(_stepper_motor_current_position);
	 * lcd_gotoxy(0,4); lcd_display_string(PSTR("DDR:"));
	 * lcd_display_hex(STEPPER_MOTOR_DDR);
	 */
	STEPPER_MOTOR_ENABLE = 0;
}
