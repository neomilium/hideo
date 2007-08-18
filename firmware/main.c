#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//#include "rtc.h"

//#include "app_date.h"

#include "nokia_driver.h"

/** relay.h */
#include <avr/io.h>
#include "bit_field.h"

#define RELAY0    GET_BIT(PORTA).bit5
/** EOF **/

int main (void)
{
	DDRA = 0xFF; //met le port A en sortie
	DDRB = 0x00;

	nokia_init();

	nokia_clear();
/*	nokia_display_char('H');
	nokia_display_string(PSTR("RTC"));*/
// 	nokia_gotoxy(0,1);
	nokia_display_string(PSTR("HIDEO"));
//	windowmanager_init();
//	app_date_init();


	while (1)
	{
/*		nokia_gotoxy(0,4);
		rtc_datetime now = rtc_read_datetime();
		display_rtc_date( now );
		nokia_gotoxy(0,5);
		display_rtc_time( now );
		_delay_us(25);
*/
//		windowmanager_process_events();
/*		uint8 i;
		uint8 j;

		for( i=0; i < 10; i++) {
			for( j=0; j < 100; j++) {
				_delay_us(1000);
			}
		}
		RELAY0 = 0;
		for( i=0; i < 10; i++) {
			for( j=0; j < 100; j++) {
				_delay_us(1000);
			}
		}
		RELAY0 = 1;*/
	}
	return 1;
}

