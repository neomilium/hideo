#ifndef __STEPPER_MOTOR_H__
#define __STEPPER_MOTOR_H__

#include "types.h"

void stepper_motor_init(void);
void stepper_motor_move(sint16 steps);

#endif /* !__STEPPER_MOTOR_H__ */
