#include "drv_dc-motor.h"

#include "hideo.h"

#include "drv_pwm.h"


#define DC_MOTOR_PORT			PORTC
#define DC_MOTOR_DDR			DDRC
#define DC_MOTOR_MASK			0b00111100
#define DC_MOTOR_DDR_CONFIG		0b00101100

#define DC_MOTOR_ENABLE			GET_BIT(PORTD).bit5

void
dc_motor_init(void)
{
	register_set(DC_MOTOR_DDR, DC_MOTOR_DDR_CONFIG, DC_MOTOR_MASK);
	register_set(DC_MOTOR_PORT, 0x00, DC_MOTOR_MASK);

	pwm_init();
	pwm_A_set(0);
}

void
dc_motor_move(sint8 speed)
{
	if (speed < 0) {
		register_set(DC_MOTOR_PORT, 0x20, DC_MOTOR_MASK);
		speed = -speed;
	} else {
		register_set(DC_MOTOR_PORT, 0x04, DC_MOTOR_MASK);
	}
	pwm_A_set(speed);
}

void
dc_motor_stop(void)
{
	register_set(DC_MOTOR_PORT, 0x00, DC_MOTOR_MASK);
	pwm_A_set(0);
}
