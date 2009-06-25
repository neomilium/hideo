#ifndef __STEPPER_MOTOR_H__
#define __STEPPER_MOTOR_H__

#include <stdint.h>

void		stepper_motor_init(void);
void		stepper_motor_move(int16_t steps);

#endif				/* !__STEPPER_MOTOR_H__ */
