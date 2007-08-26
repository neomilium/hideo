#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <avr/io.h>
#include "bit_field.h"

#define KEYBOARD_PORT    PINB
#define KEYBOARD_DDR     DDRB

#define KEYBOARD_NONE					0
#define KEYBOARD_UP						1
#define KEYBOARD_DOWN					2
#define KEYBOARD_LEFT					3
#define KEYBOARD_RIGHT				4
#define KEYBOARD_MENU_RIGHT		5
#define KEYBOARD_MENU_LEFT		6

#include "types.h"

void keyboard_init(void);
byte keyboard_key(void);

#endif
