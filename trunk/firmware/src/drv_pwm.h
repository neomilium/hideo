#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>

void		pwm_init  (void);
void		pwm_A_set (uint8_t percents);
void		pwm_B_set (uint8_t percents);

#endif
