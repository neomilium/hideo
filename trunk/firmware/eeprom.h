#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "drv_24lcxxx.h"

#define eeprom_init	drv_24lcxxx_init
#define eeprom_read	drv_24lcxxx_read_byte
#define eeprom_write	drv_24lcxxx_write_byte

#endif /* !_EEPROM_H_ */
