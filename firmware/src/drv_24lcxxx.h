#ifndef _DRV_24LCXXX_H_
#define _DRV_24LCXXX_H_

#include <stdint.h>

void		drv_24lcxxx_init(void);
void		drv_24lcxxx_read(const uint16_t memory_address, const uint8_t size, const void* data);
void		drv_24lcxxx_write(const uint16_t memory_address, const void* data, const uint8_t size);

#endif				/* !_DRV_24LCXXX_H_ */
