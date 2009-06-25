#ifndef __LCDNOKIA_H_DRIVER__
#define __LCDNOKIA_H_DRIVER__

#include <avr/io.h>
#include "hideo.h"

#define _DRV_NOKIA_DDR			DDRA
#define _DRV_NOKIA_MASK			0b00011111

#define NOK_SCLK    GET_BIT(PORTA).bit4		/* nokia lcd sclk */
#define NOK_SDA     GET_BIT(PORTA).bit3		/* nokia lcd sda */
#define NOK_DC      GET_BIT(PORTA).bit2		/* nokia lcd d/!c	 */
#define NOK_CS      GET_BIT(PORTA).bit1		/* nokia lcd !cs */
#define NOK_RES     GET_BIT(PORTA).bit0		/* nokia lcd !res */

#define NOK_SCREEN_WIDTH 84

#include <stdint.h>

/* Fonctions qui concernent les manipulations hardware (ceci est le driver) */
void		nokia_init(void);
void		nokia_reset_DDRAM(void);
void		nokia_write(uint8_t data);
void		nokia_send_command(const uint8_t command);
void		nokia_send_data(const uint8_t data);

/* Fonctions qui gerent l'affichage (API : Application Programming Interface) */
void		nokia_clear(void);
void		nokia_gotoxy(const uint8_t x, const uint8_t y);
void		nokia_display_char(const uint8_t ascii);
void		nokia_display_string(const char *string);
void		nokia_set_mode(uint8_t mode);
void		nokia_finish_line(void);
#endif
