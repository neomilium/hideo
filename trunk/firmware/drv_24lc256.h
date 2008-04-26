#ifndef _DRV_24LC256_H_
#define _DRV_24LC256_H_

#include "types.h"

void 	drv_24lc256_init(void);
void	drv_24lc256_setpos(const uint16 pos);
void	drv_24lc256_read(void *buf, uint16 nbytes);
void	drv_24lc256_write(const void *buf, uint16 nbytes);

#endif /* !_DRV_24LC256_H_ */
