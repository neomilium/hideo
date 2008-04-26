#ifndef _RELAY_H_
#define _RELAY_H_

#include <avr/io.h>
#include "hideo.h"

#define RELAY0    GET_BIT(PORTD).bit2
#define RELAY1    GET_BIT(PORTD).bit3

#endif /* !_RELAY_H_ */
