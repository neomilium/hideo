#include "drv_24lcxxx.h"

#include "i2c.h"

#define _EEPROM_ADDR   0b10100000

void
drv_24lcxxx_init(void)
{
	/* Nothing to do */
}


byte
drv_24lcxxx_read_byte(const uint16 memory_address)
{
	byte packet[2];
	// prepare address
	packet[0] = (memory_address >> 8);
	packet[1] = (memory_address & 0x00FF);
	// send memory address we wish to access to the memory chip
	i2c_master_send_ni(_EEPROM_ADDR, 2, packet);
	// retrieve the data at this memory address
	i2c_master_receive_ni(_EEPROM_ADDR, 1, packet);
	// return data
	return packet[0];
}

void
drv_24lcxxx_write_byte(const uint16 memory_address, byte data)
{
	u08 packet[3];
	// prepare address + data
	packet[0] = (memory_address >> 8);
	packet[1] = (memory_address & 0x00FF);
	packet[2] = data;
	// send memory address we wish to access to the memory chip
	// along with the data we wish to write
	while( i2c_master_send_ni(_EEPROM_ADDR, 3, packet) != I2C_OK ) { };
}
