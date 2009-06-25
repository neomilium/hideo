#include "drv_24lcxxx.h"

#include "i2c.h"

#define _EEPROM_ADDR   0b10100000

void
drv_24lcxxx_init(void)
{
	/* Nothing to do */
}


void
drv_24lcxxx_read(const uint16_t memory_address, const uint8_t size, const void* data)
{
	uint8_t _memory_address[2];
	// prepare address
	_memory_address[0] = (memory_address >> 8);
	_memory_address[1] = (memory_address & 0x00FF);
	// send memory address we wish to access to the memory chip
	while( i2c_master_send_ni(_EEPROM_ADDR, 2, _memory_address) != I2C_OK ) { };
	// retrieve the data at this memory address
	i2c_master_receive_ni(_EEPROM_ADDR, size, (uint8_t *)data);
}

void
drv_24lcxxx_write(const uint16_t memory_address, const void* data, const uint8_t size)
{
	uint8_t _memory_address[2];
	// prepare address + data
	_memory_address[0] = (memory_address >> 8);
	_memory_address[1] = (memory_address & 0x00FF);

	// send memory address we wish to access to the memory chip
	// along with the data we wish to write
	while( i2c_master_hsend_ni(_EEPROM_ADDR, 2, _memory_address, size, (uint8_t *)data) != I2C_OK ) { };
}
