#ifndef __STEPPER_MOTOR_H__
#define __STEPPER_MOTOR_H__

#include "types.h"

#define STEPPER_MOTOR_PORT						PORTC
#define STEPPER_MOTOR_DDR							DDRC
#define STEPPER_ELECTROMAGNETS_MASK		0x3C
#define STEPPER_MOTORS_ENABLE_MASK		0x40


void stepper_motor_init(void);
void stepper_motor_move(sint16 steps);

#endif /* !__STEPPER_MOTOR_H__ */
