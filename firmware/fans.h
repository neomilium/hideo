#ifndef _FANS_H_
#define _FANS_H_

#include <avr/io.h>
#include "hideo.h"

#define FAN0      GET_BIT(PORTD).bit4
#define FAN1      GET_BIT(PORTD).bit6

#endif				/* !_RELAY_H_ */
