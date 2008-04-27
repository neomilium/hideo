#include "drv_24lc256.h"

#include "i2c.h"

#define DRV_24LC256_I2C_ADDR   0b10100000

static uint16		_position	= 0;

void
drv_24lc256_init(void)
{
	/* Nothing to do */
}

void
drv_24lc256_setpos(const uint16 pos)
{
	i2c_master_send_ni(DRV_24LC256_I2C_ADDR, 2, (uint8 *) &pos);
	_position = pos;
}

void
drv_24lc256_read(void *buf, uint16 nbytes)
{
	i2c_master_receive_ni(DRV_24LC256_I2C_ADDR, nbytes, buf);
	_position += nbytes;
}

void
drv_24lc256_write(const void *buf, uint16 nbytes)
{
	i2c_master_hsend_ni(DRV_24LC256_I2C_ADDR, 2, (uint8 *)&_position, nbytes, buf);
	_position += nbytes;
}
