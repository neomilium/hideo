#include "keyboard.h"

static uint8 _current_key = KEYBOARD_NONE;

void keyboard_init(void)
{
	KEYBOARD_DDR = 0x00;
}

byte keyboard_key(void)
{
	byte key = KEYBOARD_NONE;
	if( !(KEYBOARD_PORT & 0x01) ){
		key = KEYBOARD_LEFT;
	} else if( !(KEYBOARD_PORT & 0x02) ){
		key = KEYBOARD_MENU_LEFT;
	} else if( !(KEYBOARD_PORT & 0x10) ){
		key = KEYBOARD_MENU_RIGHT;
	} else if( !(KEYBOARD_PORT & 0x20) ){
		key = KEYBOARD_UP;
	} else if( !(KEYBOARD_PORT & 0x40) ){
		key = KEYBOARD_DOWN;
	} else if( !(KEYBOARD_PORT & 0x80) ){
		key = KEYBOARD_RIGHT;
	}

 /* else if( !(KEYBOARD_PORT & 0x40) ){
		//	key = KEYBOARD_UP;
	} else if( !(KEYBOARD_PORT & 0x80) ){
		//	key = KEYBOARD_UP;
	}
	*/

	if( _current_key != key ) {
		_current_key = key;
		return key;
	} else {
		return KEYBOARD_NONE;
	}
}
