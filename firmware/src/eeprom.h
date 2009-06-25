#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "drv_24lcxxx.h"

#define eeprom_init	drv_24lcxxx_init
#define eeprom_read	drv_24lcxxx_read
#define eeprom_write	drv_24lcxxx_write

/* lens value's size = 2 uint8_t s */
#define EEPROM_MEMMAP__DISPLAY_LENS		0x00
/* trapezoid value's size = 2 uint8_t s */
#define EEPROM_MEMMAP__DISPLAY_TRAPEZOID	0x02
/* HQI lifetime value's size = 7 uint8_t s */
#define EEPROM_MEMMAP__HQI_LIFETIME		0x04

#endif /* !_EEPROM_H_ */
