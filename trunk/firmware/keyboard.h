#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <avr/io.h>
#include "bit_field.h"

#include "types.h"

#define KEYBOARD_PORT_MASK     0xF3

#define KEYBOARD_PORT          PINB
#define KEYBOARD_DDR           DDRB



#define KEYBOARD_UP            0x20
#define KEYBOARD_DOWN          0x40
#define KEYBOARD_LEFT          0x01
#define KEYBOARD_RIGHT         0x80

#define KEYBOARD_MENU_LEFT     0x02
#define KEYBOARD_MENU_RIGHT    0x10


void keyboard_init(void);
void keyboard_poll(void);

#endif
