#include "keyboard.h"

void keyboard_init(void)
{
	KEYBOARD_DDR = 0x00;
}

byte keyboard_key(void)
{
	byte key = KEYBOARD_NONE;
	if( !(KEYBOARD_PORT & 0x01) ){
		key = KEYBOARD_CANCEL;
	} else if( !(KEYBOARD_PORT & 0x02) ){
		key = KEYBOARD_UP;
	} else if( !(KEYBOARD_PORT & 0x04) ){
		key = KEYBOARD_LEFT;
	} else if( !(KEYBOARD_PORT & 0x08) ){
		key = KEYBOARD_RIGHT;
	} else if( !(KEYBOARD_PORT & 0x10) ){
		key = KEYBOARD_DOWN;
	} else if( !(KEYBOARD_PORT & 0x20) ){
		key = KEYBOARD_OK;
	}

 /* else if( !(KEYBOARD_PORT & 0x40) ){
		//	key = KEYBOARD_UP;
	} else if( !(KEYBOARD_PORT & 0x80) ){
		//	key = KEYBOARD_UP;
	}
	*/

	return key;
}
