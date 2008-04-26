#ifndef __RTC_H__
#define __RTC_H__

struct rtc_datetime {
	bcd_byte seconds;
	bcd_byte minutes;
	bcd_byte hours;
	bcd_byte day;
	bcd_byte date;
	bcd_byte month;
	bcd_byte year;
};

typedef struct rtc_datetime rtc_datetime_t;

#include "drv_ds1307.h"

#define rtc_init() drv_ds1307_init()
#define rtc_write( X ) drv_ds1307_write( X )
#define rtc_read() drv_ds1307_read()

#endif

