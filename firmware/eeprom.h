#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "drv_24lc256.h"

#define	eeprom_init	drv_24lc256_init
#define	eeprom_setpos	drv_24lc256_setpos
#define	eeprom_read	drv_24lc256_read
#define	eeprom_write	drv_24lc256_write

#endif /* !_EEPROM_H_ */
