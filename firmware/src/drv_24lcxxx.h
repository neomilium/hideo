#ifndef _DRV_24LCXXX_H_
#define _DRV_24LCXXX_H_

#include "types.h"

void		drv_24lcxxx_init(void);
void		drv_24lcxxx_read(const uint16 memory_address, const uint8 size, const void* data);
void		drv_24lcxxx_write(const uint16 memory_address, const void* data, const uint8 size);

#endif				/* !_DRV_24LCXXX_H_ */
