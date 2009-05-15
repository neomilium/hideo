#ifndef __PWM_H__
#define __PWM_H__

#include "types.h"

void		pwm_init  (void);
void		pwm_A_set (uint8 percents);
void		pwm_B_set (uint8 percents);

#endif
