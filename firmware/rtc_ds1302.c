#include "rtc_ds1302.h"

#include <avr/io.h>
#include <util/delay.h>

void rtc_ds1302_init(void)
{
	unsigned char seconds = rtc_ds1302_read_data(RTC_SECOND);
	if(seconds & 0x80) { // Clock Halt flag is 1
		rtc_ds1302_write_data( RTC_SECOND , seconds & 0x7F );
	}
	unsigned char hours = rtc_ds1302_read_data(RTC_HOUR);
	if(hours & 0x80) { // 24h mode
		rtc_ds1302_write_data( RTC_HOUR , hours & 0x7F );
	}

	/* Initial setup */
/*
	rtc_write_data( RTC_SECOND , 0x42 );
	rtc_write_data( RTC_MINUTE , 0x23 );
	rtc_write_data( RTC_HOUR , 0x18 );
	rtc_write_data( RTC_DATE , 0x07 );
	rtc_write_data( RTC_MONTH , 0x05 );
	rtc_write_data( RTC_YEAR , 0x07 );
*/
}

////////////////////////////////////////////////////////////////////////////////
void rtc_ds1302_write(unsigned char data)            // serial write data or command subroutine
{
	unsigned char bit;
	for (bit=0;bit<8;bit++) {
		RTC_SCLK=0;
		if (data & 0x01) {
			RTC_OUTPUT=1;
		} else {
			RTC_OUTPUT=0;
		}
		RTC_SCLK=1;
		_delay_us(1);

		data = data >> 1;
	}
}

///////////////////////////////////////////////////////////////////////////////
unsigned char rtc_ds1302_read(void)			// serial write data or command subroutine
{
	DDRA = 0xBF;	// single byte read
	unsigned char data = 0x00;
	unsigned char bit;
	for (bit=0;bit<8;bit++) {
		data = data >> 1;
		RTC_SCLK=0;
		_delay_us(1);

//		if(PORTA & 0x40){
		if(RTC_INPUT == 1) {
			data |= 0x80;
		}
		RTC_SCLK=1;
		_delay_us(1);
	}
	DDRA = 0xFF;
	return data;
}

//////////////////////////////////////////////////////////////////////////////////
void rtc_ds1302_write_data(unsigned char register_addr, unsigned char data)
{
// 	if(Reg_add >= 0xC0) 		byteforrtc = Reg_add;
	RTC_SCLK=0;
	RTC_RST=1;
	_delay_us(1);	//  1s
	rtc_ds1302_write(register_addr | RTC_MODE_WRITE);

	_delay_us(1);	//  1s
	rtc_ds1302_write(data);
	RTC_RST=0;
}

unsigned char rtc_ds1302_read_data(unsigned char register_addr)
{
//	if(Reg_add >= 0xC0) 	byteforrtc = Reg_add;
	unsigned char data;
	RTC_SCLK=0;
	RTC_RST=1;
	_delay_us(2);
//	rtc_write_dorc();
	rtc_ds1302_write( register_addr | RTC_MODE_READ );

	_delay_us(1);
//	rtc_read_dorc();
	data = rtc_ds1302_read();
	RTC_RST=0;

	return( data );
}

/*
void rtc_read_datas(unsigned char register_addr, void* destination, const uint8 size)
{
	unsigned char data;
	RTC_SCLK=0;
	RTC_RST=1;
	_delay_us(2);
	rtc_write( register_addr | RTC_MODE_READ );

	uint8 i;
	for(i=0; i < size; i++){
		_delay_us(1);
		((byte*)(destination[i])) = rtc_read();
	}
	RTC_RST=0;
}
*/
rtc_datetime rtc_ds1302_read_datetime(void)
{
	rtc_datetime now;
	now.rtc_sec = rtc_ds1302_read_data(RTC_SECOND);
	now.rtc_min = rtc_ds1302_read_data(RTC_MINUTE);
	now.rtc_hour = rtc_ds1302_read_data(RTC_HOUR);
	now.rtc_day = rtc_ds1302_read_data(RTC_DAY);
	now.rtc_month = rtc_ds1302_read_data(RTC_MONTH);
	now.rtc_year = rtc_ds1302_read_data(RTC_YEAR);
	return now;
}

uint8 bcd_to_uint8( const byte bcd )
{
	unsigned char lsb = bcd & 0x0F;
	unsigned char msb = (bcd & 0xF0) >> 4;

	return( msb * 10 + lsb );
}
