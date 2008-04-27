#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <avr/io.h>
#include "hideo.h"

#include "types.h"


#define KEYBOARD_UP            0x10
#define KEYBOARD_DOWN          0x40
#define KEYBOARD_LEFT          0x80
#define KEYBOARD_RIGHT         0x20

#define KEYBOARD_MENU_LEFT     0x02
#define KEYBOARD_MENU_RIGHT    0x01


void		drv_keyboard_init(void);
void		drv_keyboard_poll(void);

#endif
