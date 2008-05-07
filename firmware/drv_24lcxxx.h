#ifndef _DRV_24LCXXX_H_
#define _DRV_24LCXXX_H_

#include "types.h"

void		drv_24lcxxx_init(void);
byte		drv_24lcxxx_read_byte(const uint16 memory_address);
void		drv_24lcxxx_write_byte(const uint16 memory_address, byte data);

#endif				/* !_DRV_24LCXXX_H_ */
