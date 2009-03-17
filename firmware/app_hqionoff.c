#include <avr/pgmspace.h>

#include "app_keyboard.h"
#include "keyboard.h"

#include "lcd.h"
#include "relay.h"
#include "windowmanager.h"
#include "drv_pwm.h"

static char etahqi;

void
_app_hqionoff_init(void *data)
{
//	lcd_gotoxy(0, 0);
}

void
_app_hqionoff_event_handler(const event_t event)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_display_string(PSTR("LAMPE "));


	switch (event.code) {
		case E_KEY_PRESSED:
	//		if(RELAY0==1) lcd_display_line(PSTR("  ON"));
	//		if(RELAY0==0) lcd_display_line(PSTR("  OFF"));
			
			switch (event.data) {
				case KEYBOARD_UP:
					lcd_gotoxy(0, 2);
					lcd_display_line(PSTR("> ON"));
					lcd_gotoxy(0, 3);
					lcd_display_line(PSTR("  OFF"));
					etahqi=1;
					break;
				case KEYBOARD_DOWN:
					lcd_gotoxy(0, 2);
					lcd_display_line(PSTR("  ON"));
					lcd_gotoxy(0, 3);
					lcd_display_line(PSTR("> OFF"));
					etahqi=0;
					break;

				case KEYBOARD_LEFT:
					VENTIL1=0;
					VENTIL2=0;
					pwm_B_set(0);
		//			lcd_gotoxy(0, 3);
		//			lcd_display_line(PSTR("VENTILO OFF"));
					break;
				case KEYBOARD_RIGHT:
					VENTIL1=1;
					VENTIL2=1;
					pwm_B_set(99);
		//			lcd_gotoxy(0, 3);
		//			lcd_display_line(PSTR("VENTILO ON"));
					break;

				case KEYBOARD_MENU_LEFT:
					windowmanager_exit();
					
					break;
				case KEYBOARD_MENU_RIGHT:
			//		lcd_gotoxy(0, 0);
			//		lcd_display_string(PSTR("LAMPE "));
					if(etahqi==1){
							RELAY0 = 1;
			//				lcd_display_line(PSTR("  ON"));
							}
					if(etahqi==0){
							RELAY0 = 0;
			//				lcd_display_line(PSTR("  OFF"));
							}
					break;
			}
			break;


			}
//			break;
//		default:
//			break;
	
}


