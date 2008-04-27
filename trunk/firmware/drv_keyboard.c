#include "keyboard.h"

#include "eventmanager.h"

#define KEYBOARD_MASK					0xF3
#define KEYBOARD_PIN					PINB
#define KEYBOARD_PORT					PORTB
#define KEYBOARD_DDR					DDRB

static uint8	_keyboard_status = 0x00;

void
drv_keyboard_init(void)
{
	KEYBOARD_DDR = 0x00;	/* Set keyboard port in input */
	KEYBOARD_PORT = KEYBOARD_MASK;	/* Set internal pull-up resistances */
	eventmanager_add_polling_fct(drv_keyboard_poll);
	_keyboard_status = KEYBOARD_PORT & KEYBOARD_MASK;
}

void
drv_keyboard_poll(void)
{
	if ((KEYBOARD_PIN & KEYBOARD_MASK) != _keyboard_status) {
		event_t		event = {
			.code = E_NONE,
			.data = 0x00
		};
		for (uint8 i = 0; i < 8; i++) {
			uint8		current_key_mask = (1 << i);
			if (KEYBOARD_MASK & current_key_mask) {
				if ((KEYBOARD_PIN & current_key_mask) && !(_keyboard_status & current_key_mask)) {
					event.code = E_KEY_RELEASED;
					event.data = current_key_mask;
					eventmanager_push(event);
				}
				if (!(KEYBOARD_PIN & current_key_mask) && (_keyboard_status & current_key_mask)) {
					event.code = E_KEY_PRESSED;
					event.data = current_key_mask;
					eventmanager_push(event);
				}
			}
		}
		_keyboard_status = KEYBOARD_PIN & KEYBOARD_MASK;
	}
}
