#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <avr/io.h>
#include "hideo.h"

#include <stdint.h>

void		dc_motor_init(void);
void		dc_motor_move(int8_t speed);
void		dc_motor_stop(void);

#endif
