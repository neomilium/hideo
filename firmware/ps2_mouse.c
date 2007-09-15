#include "event.h"
#include "types.h"
#include "ps2.h"
#include "lcd.h"

#include "ps2_mouse.h"

#define MOUSE_PACKET_SIZE 3

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
	ps2_init();

	/* FIXME Wait for mouse to start up */

	//_ps2_mouse_write(0xF4);       /* Enable Data Reporting */
}

event_t
ps2_mouse_poll(void)
{
	byte  _mouse_packet[MOUSE_PACKET_SIZE];
	uint8 _mouse_bytes_read = 0;

	event_t result = E_NONE;

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
	lcd_gotoxy(0,0);
	lcd_display_hex(_mouse_packet[0]);
	lcd_gotoxy(0,1);
	lcd_display_hex(_mouse_packet[1]);
	lcd_gotoxy(0,2);
	lcd_display_hex(_mouse_packet[2]);

	return(result);
}
