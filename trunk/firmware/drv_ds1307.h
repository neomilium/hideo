#ifndef __DS1307_H__
#define __DS1307_H__

#include "types.h"
#include "rtc.h"

void ds1307_init(void);
// void ds1307_write(uint8 data);
rtc_datetime ds1307_read(void);
// void ds1307_write_data( X, Y )
// ds1307_read_data( X )
// ds1307_read_datetime()


#endif
