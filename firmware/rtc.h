#ifndef __RTC_H__
#define __RTC_H__

#include "rtc_ds1302.h"

#define rtc_init() rtc_ds1302_init()
#define rtc_write( X ) rtc_ds1302_write( X )
#define rtc_read() rtc_ds1302_read()
#define rtc_write_data( X, Y ) rtc_ds1302_write_data( X, Y )
#define rtc_read_data( X ) rtc_ds1302_read_data( X )
#define rtc_read_datetime() rtc_ds1302_read_datetime()

#endif

