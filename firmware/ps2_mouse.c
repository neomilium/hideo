#include "event.h"
#include "types.h"
#include "ps2.h"

#include "ps2_mouse.h"

#define MOUSE_PACKET_SIZE 3

static byte _mouse_button_status = 0x00;

uint8
_ps2_mouse_write(byte data)
{
	byte rdata;

	ps2_write(data);

	/* Wait for acknowledge */
	while (!ps2_read(&rdata));
	return (rdata == 0xFA);
}

void
ps2_mouse_init(void)
{
	event_add_polling_fct(ps2_mouse_poll);

	ps2_init();
	/* FIXME Wait for mouse to start up */

	//_ps2_mouse_write(0xF4);       /* Enable Data Reporting */
}

void
ps2_mouse_poll(void)
{
	byte  _mouse_packet[MOUSE_PACKET_SIZE];
	uint8 _mouse_bytes_read = 0;

	/* Empty buffer */
	ps2_flush();

	_ps2_mouse_write(0xEB);       /* Read Data */

	/* Read packet */
	while (_mouse_bytes_read < MOUSE_PACKET_SIZE) {
		if (ps2_read(&_mouse_packet[_mouse_bytes_read])) {
			_mouse_bytes_read++;
		}
	}

	/* Handle packet */
	event_t event = {
		.code = E_NONE,
		.data = 0x00
	};

	/* Error handling */
	if( (_mouse_packet[0] & 0xC0) != 0x00 )	{
		if ( (_mouse_packet[0] & 0x80) != 0x00 )	{
			event.code = E_MOUSE_Y_OVERFLOW;
		}

		if ( (_mouse_packet[0] & 0x40) != 0x00 )	{
			event.code = E_MOUSE_X_OVERFLOW;
		}
		event_push( event );
	}

	/* Mouse buttons handling */
	if( (_mouse_packet[0] & 0x07) != _mouse_button_status ) {
		for( uint8 mask = 1; mask < 0x80; mask = mask << 1 ) {
			if ( (_mouse_packet[0] & mask) != (_mouse_button_status & mask) ) {
				event.data = mask;
				if ( (_mouse_packet[0] & mask) ) {
					event.code = E_MOUSE_BUTTON_PRESSED;
				} else {
					event.code = E_MOUSE_BUTTON_RELEASED;
				}
				event_push(event);
			}
		}
		_mouse_button_status = _mouse_packet[0];
	}

	/* X axis handling */
	if( _mouse_packet[1] != 0x00 ) {
		if ( _mouse_packet[0] & 0x10 ) {
			event.code = E_MOUSE_X_REV;
			event.data = ~_mouse_packet[1];
		} else {
			event.code = E_MOUSE_X_FWD;
			event.data = _mouse_packet[1];
		}
		event_push( event );
	}

	/* Y axis handling */
	if( _mouse_packet[2] != 0x00 ) {
		if ( _mouse_packet[0] & 0x20 ) {
			event.code = E_MOUSE_Y_REV;
			event.data = ~_mouse_packet[2];
		} else {
			event.code = E_MOUSE_Y_FWD;
			event.data = _mouse_packet[2];
		}
		event_push( event );
	}
}
