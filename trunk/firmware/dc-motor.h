#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <avr/io.h>
#include "bit_field.h"

#include "types.h"

#define DC_MOTOR_PORT				PORTC
#define DC_MOTOR_DDR				DDRC
#define DC_MOTOR_MASK				0b00111100
#define DC_MOTOR_ENABLE			GET_BIT(PORTD).bit5

void dc_motor_init(void);
void dc_motor_move(sint8 speed);
void dc_motor_stop(void);

#endif
