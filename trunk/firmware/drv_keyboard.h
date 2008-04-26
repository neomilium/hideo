#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <avr/io.h>
#include "hideo.h"

#include "types.h"

#define KEYBOARD_MASK					0xF3

#define KEYBOARD_PIN					PINB
#define KEYBOARD_PORT					PORTB
#define KEYBOARD_DDR					DDRB



#define KEYBOARD_UP            0x10
#define KEYBOARD_DOWN          0x40
#define KEYBOARD_LEFT          0x20
#define KEYBOARD_RIGHT         0x80

#define KEYBOARD_MENU_LEFT     0x02
#define KEYBOARD_MENU_RIGHT    0x01


void keyboard_init(void);
void keyboard_poll(void);

#endif
