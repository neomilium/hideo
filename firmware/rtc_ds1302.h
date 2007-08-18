#ifndef __RTC_DS1302_H_DRIVER__
#define __RTC_DS1302_H_DRIVER__

#include "bit_field.h"

/* RTC pins */
#define RTC_SCLK      GET_BIT(PORTA).bit7
#define RTC_OUTPUT    GET_BIT(PORTA).bit6
#define RTC_RST       GET_BIT(PORTA).bit5
#define RTC_INPUT     GET_BIT(PINA).bit6

/* Register addresses */
#define RTC_SECOND            0x80
#define RTC_MINUTE            0x82
#define RTC_HOUR              0x84
#define RTC_DATE              0x86
#define RTC_MONTH             0x88
#define RTC_DAY               0x8A
#define RTC_YEAR              0x8C
#define RTC_CONTROL           0x8E
#define RTC_TRICKLE_CHARGER   0x90
#define RTC_CLOCK_BURST       0xBE

/* Modes */
#define RTC_MODE_READ         0x01
#define RTC_MODE_WRITE        0x00

#include "types.h"

typedef struct rtc_datetime {
	byte rtc_sec;
	byte rtc_min;
	byte rtc_hour;
	byte rtc_day;
	byte rtc_month;
	byte rtc_year;
} rtc_datetime;

// 
// Fonctions qui concernent les manipulations hardware (ceci est le driver)
void rtc_ds1302_init(void);
void rtc_ds1302_write(unsigned char data);
unsigned char rtc_ds1302_read(void);

void rtc_ds1302_write_data(unsigned char register_addr, unsigned char data);
unsigned char rtc_ds1302_read_data(unsigned char register_addr);

// Fonctions qui gerent l'affichage (API : Application Programming Interface)
rtc_datetime rtc_ds1302_read_datetime(void);

#endif
