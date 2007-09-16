#include "stepper_motor.h"

#include <avr/io.h>
#include <util/delay.h>

#include "bit_field.h"

#define STEPPER_MOTOR_PORT        PORTC
#define STEPPER_MOTOR_DDR         DDRC
#define STEPPER_MOTOR_MASK        0x3C | 0xC0

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

static uint8 _stepper_motor_current_position = 0;

#define STEPPER_MOTOR_1_ENABLE         GET_BIT(PORTC).bit6
#define STEPPER_MOTOR_2_ENABLE         GET_BIT(PORTC).bit7

void
stepper_motor_init(void)
{
	STEPPER_MOTOR_1_ENABLE = 1;
	STEPPER_MOTOR_2_ENABLE = 1;
	STEPPER_MOTOR_DDR = STEPPER_MOTOR_MASK;
}

void
stepper_motor_increment(void)
{
	STEPPER_MOTOR_PORT = _stepper_motor_positions[_stepper_motor_current_position];
	_stepper_motor_current_position = (_stepper_motor_current_position + 1) % 8;
}

void
stepper_motor_decrement(void)
{
	STEPPER_MOTOR_PORT = _stepper_motor_positions[_stepper_motor_current_position];
	_stepper_motor_current_position = (_stepper_motor_current_position + 8 - 1) % 8;
}

void
stepper_motor_move(sint16 steps)
{
	if ( steps >= 0 ) {
		for(uint8 i=steps; i>0; i--) {
			stepper_motor_increment();
			_delay_ms(1);
		}
	} else {
		for(uint8 i=steps; i<0; i++) {
			stepper_motor_decrement();
			_delay_ms(1);
		}
	}
}
