#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "i2ceeprom.h"
//#include "drv_24lc256.h"
/*
#define	eeprom_init	drv_24lc256_init
#define	eeprom_setpos	drv_24lc256_setpos
#define	eeprom_read	drv_24lc256_read
#define	eeprom_write	drv_24lc256_write
*/

#define eeprom_init	i2ceepromInit
#define eeprom_read	i2ceepromReadByte
#define eeprom_write	i2ceepromWriteByte

#endif /* !_EEPROM_H_ */
