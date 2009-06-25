#include "drv_stepper-motor.h"

#include "hideo.h"

#include <avr/io.h>
#include <util/delay.h>

#define STEPPER_MOTOR_PORT			PORTC
#define STEPPER_MOTOR_DDR			DDRC

#define STEPPER_MOTOR_ENABLE         		GET_BIT(PORTC).bit6
#define STEPPER_MOTOR_ENABLE_MASK		0b01000000

#define STEPPER_ELECTROMAGNETS_MASK		0b00111100
#define STEPPER_ELECTROMAGNETS_POSITION1	0b00000100
#define STEPPER_ELECTROMAGNETS_POSITION2	0b00001100
#define STEPPER_ELECTROMAGNETS_POSITION3	0b00001000
#define STEPPER_ELECTROMAGNETS_POSITION4	0b00011000
#define STEPPER_ELECTROMAGNETS_POSITION5	0b00010000
#define STEPPER_ELECTROMAGNETS_POSITION6	0b00110000
#define STEPPER_ELECTROMAGNETS_POSITION7	0b00100000
#define STEPPER_ELECTROMAGNETS_POSITION8	0b00100100

#define STEPPER_MOTOR_INTERSTEP_DELAY	50

static const uint8_t _stepper_motor_positions[] = {
	STEPPER_ELECTROMAGNETS_POSITION1,
	STEPPER_ELECTROMAGNETS_POSITION2,
	STEPPER_ELECTROMAGNETS_POSITION3,
	STEPPER_ELECTROMAGNETS_POSITION4,
	STEPPER_ELECTROMAGNETS_POSITION5,
	STEPPER_ELECTROMAGNETS_POSITION6,
	STEPPER_ELECTROMAGNETS_POSITION7,
	STEPPER_ELECTROMAGNETS_POSITION8
};

static uint8_t	_stepper_motor_current_position = 0;

void
stepper_motor_init(void)
{
	register_set(STEPPER_MOTOR_DDR, 0xFF, STEPPER_ELECTROMAGNETS_MASK | STEPPER_MOTOR_ENABLE_MASK);
	STEPPER_MOTOR_ENABLE = 0;
}

void
stepper_motor_increment(void)
{
	_stepper_motor_current_position = (_stepper_motor_current_position + 1) % 8;
	register_set(STEPPER_MOTOR_PORT, (_stepper_motor_positions[_stepper_motor_current_position]), STEPPER_ELECTROMAGNETS_MASK);
}

void
stepper_motor_decrement(void)
{
	_stepper_motor_current_position = (_stepper_motor_current_position + 8 - 1) % 8;
	register_set(STEPPER_MOTOR_PORT, (_stepper_motor_positions[_stepper_motor_current_position]), STEPPER_ELECTROMAGNETS_MASK);
}

void
stepper_motor_move(int16_t steps)
{
	STEPPER_MOTOR_ENABLE = 1;
	if (steps >= 0) {
		for (int16_t i = steps; i > 0; i--) {
			stepper_motor_increment();
			for (uint8_t d = 0; d < 100; d++)
				_delay_us(STEPPER_MOTOR_INTERSTEP_DELAY);
		}
	} else {
		for (int16_t i = steps; i < 0; i++) {
			stepper_motor_decrement();
			for (uint8_t d = 0; d < 100; d++)
				_delay_us(STEPPER_MOTOR_INTERSTEP_DELAY);
		}
	}
	STEPPER_MOTOR_ENABLE = 0;
}
