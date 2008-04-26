#include "drv_ds1307.h"

#include "rtc.h"

#include "i2c.h"
#define DS1307_I2C_ADDR   0xD0

void
drv_ds1307_init(void)
{
	i2cInit();

	byte		data[2];
	data[0] = 0x00;
	/* Timekeeper Registers address */
	i2cMasterSendNI(DS1307_I2C_ADDR, 1, &data[0]);
	i2cMasterReceiveNI(DS1307_I2C_ADDR, 1, &data[1]);
	data[1] &= 0x7F;
	/* Clear CH(enable clock generator) */
	i2cMasterSendNI(DS1307_I2C_ADDR, 2, &data);

	data[0] = 0x07;
	data[1] = 0x00;
	/* Clear OUT, SQWE, RS1, RS0 */
	i2cMasterSendNI(DS1307_I2C_ADDR, 2, &data);
}

rtc_datetime_t
drv_ds1307_read(void)
{
	byte		ds1307_addr[] = {0x00};
	i2cMasterSendNI(DS1307_I2C_ADDR, 1, ds1307_addr);
	rtc_datetime_t	data;
	i2cMasterReceiveNI(DS1307_I2C_ADDR, sizeof(rtc_datetime_t), (byte *) (&data));
	data.hours &= 0x3F;
	return data;
}

void
drv_ds1307_write(const rtc_datetime_t datetime)
{
	struct {
		byte		address;
		rtc_datetime_t	datetime;
	}		data;

	data.address = 0x00;
	data.datetime = datetime;

	i2cMasterSendNI(DS1307_I2C_ADDR, sizeof(data), &data);
}
