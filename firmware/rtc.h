#ifndef __RTC_H__
#define __RTC_H__

typedef struct {
	bcd_byte seconds;
	bcd_byte minutes;
	bcd_byte hours;
	bcd_byte day;
	bcd_byte date;
	bcd_byte month;
	bcd_byte year;
} rtc_datetime;

#include "ds1307.h"

#define rtc_init() ds1307_init()
#define rtc_write( X ) ds1307_write( X )
#define rtc_read() ds1307_read()

#endif

