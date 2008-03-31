#include "keyboard.h"

#include "eventmanager.h"

static uint8 _keyboard_status = 0x00;

void
keyboard_init(void)
{
	KEYBOARD_DDR = 0x00;
	KEYBOARD_PORT = 0xFF;
	eventmanager_add_polling_fct(keyboard_poll);
	_keyboard_status = KEYBOARD_PORT & KEYBOARD_MASK;
}

void
keyboard_poll(void)
{
	if( (KEYBOARD_PIN & KEYBOARD_MASK) != _keyboard_status ) {
		event_t event = {
			.code = E_NONE,
			.data = 0x00
		};
		for(uint8 i=0; i<8; i++) {
			uint8 current_key_mask = (1 << i);
			if( KEYBOARD_MASK & current_key_mask ) {
				if( (KEYBOARD_PIN & current_key_mask) && !(_keyboard_status & current_key_mask) ) {
					event.code = E_KEY_RELEASED;
					event.data = current_key_mask;
					eventmanager_push(event);
				}
				if( !(KEYBOARD_PIN & current_key_mask) && (_keyboard_status & current_key_mask) ) {
					event.code = E_KEY_PRESSED;
					event.data = current_key_mask;
					eventmanager_push(event);
				}
			}
		}
		_keyboard_status = KEYBOARD_PIN & KEYBOARD_MASK;
	}
}
