#ifndef _DS1307_H_
#define _DS1307_H_

#include "types.h"
#include "rtc.h"

void		drv_ds1307_init(void);
/* void ds1307_write(uint8 data); */
rtc_datetime_t	drv_ds1307_read(void);
/* void ds1307_write_data( X, Y ) */
/* ds1307_read_data( X ) */
/* ds1307_read_datetime() */


#endif
