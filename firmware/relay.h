#ifndef __RELAY_H__
#define __RELAY_H__

#include <avr/io.h>
#include "bit_field.h"

#define RELAY0    GET_BIT(PORTD).bit2
#define RELAY1    GET_BIT(PORTD).bit3

#endif /* !__RELAY_H__ */
