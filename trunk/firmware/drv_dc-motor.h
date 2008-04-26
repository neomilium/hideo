#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <avr/io.h>
#include "hideo.h"

#include "types.h"

void		dc_motor_init(void);
void		dc_motor_move(sint8 speed);
void		dc_motor_stop(void);

#endif
