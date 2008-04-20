#include "dc-motor.h"
#include "pwm.h"

#define DC_MOTOR_PORT				PORTC
#define DC_MOTOR_DDR				DDRC
#define DC_MOTOR_MASK				0b00111100

#define DC_MOTOR_ENABLE                        GET_BIT(PORTD).bit5

void dc_motor_init(void)
{
	DC_MOTOR_PORT = 0x00;

	DC_MOTOR_DDR = 0b00101100; //DC_MOTOR_MASK;

	pwm_init();
	pwm_A_set(0);
}

void dc_motor_move(sint8 speed)
{
	if(speed < 0) {
		DC_MOTOR_PORT = 0x20;
		speed = -speed;
	} else {
		DC_MOTOR_PORT = 0x04;
	}
	pwm_A_set(speed);
}

void dc_motor_stop(void)
{
	DC_MOTOR_PORT = 0x00;
	pwm_A_set(0);
}

