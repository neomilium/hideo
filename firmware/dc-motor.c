#include "dc-motor.h"
#include "pwm.h"

void dc_motor_init(void)
{
	DC_MOTOR_PORT = 0x00;

	uint8 tmp = DC_MOTOR_DDR;
	tmp = tmp | (0xFF & DC_MOTOR_MASK);
	tmp = tmp & ~((~0xFF) & DC_MOTOR_MASK);
	DC_MOTOR_DDR = tmp;

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
}

