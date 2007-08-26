#ifndef __LCDNOKIA_H_DRIVER__
#define __LCDNOKIA_H_DRIVER__

#include <avr/io.h>
#include "bit_field.h"

#define NOK_DDR			DDRA

#define NOK_SCLK    GET_BIT(PORTA).bit4		// nokia lcd sclk
#define NOK_SDA     GET_BIT(PORTA).bit3		// nokia lcd sda
#define NOK_DC      GET_BIT(PORTA).bit2		// nokia lcd d/!c	
#define NOK_CS      GET_BIT(PORTA).bit1		// nokia lcd !cs
#define NOK_RES     GET_BIT(PORTA).bit0		// nokia lcd !res

#include "types.h"

// Fonctions qui concernent les manipulations hardware (ceci est le driver)
void nokia_init(void);
void nokia_reset_DDRAM(void);
void nokia_write(unsigned char data);
void nokia_send_command(const unsigned char command);
void nokia_send_data(const unsigned char data);

// Fonctions qui gerent l'affichage (API : Application Programming Interface)
void nokia_clear(void);
void nokia_gotoxy(const unsigned char x, const unsigned char y);
void nokia_display_char(const unsigned char ascii);
void nokia_display_string(const char *string);
void nokia_set_mode(uint8 mode);
void nokia_finish_line(void);
#endif

