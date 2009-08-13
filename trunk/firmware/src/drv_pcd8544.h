#ifndef __LCDNOKIA_H_DRIVER__
#define __LCDNOKIA_H_DRIVER__

#include <avr/io.h>
#include "bitfield.h"

#define _DRV_NOKIA_DDR			DDRA
#define _DRV_NOKIA_MASK			0b00011111

#define PCD8544_SCLK    GET_BIT(PORTA).bit4		/* pcd8544 lcd sclk */
#define PCD8544_SDA     GET_BIT(PORTA).bit3		/* pcd8544 lcd sda */
#define PCD8544_DC      GET_BIT(PORTA).bit2		/* pcd8544 lcd d/!c	 */
#define PCD8544_CS      GET_BIT(PORTA).bit1		/* pcd8544 lcd !cs */
#define PCD8544_RES     GET_BIT(PORTA).bit0		/* pcd8544 lcd !res */

#define PCD8544_SCREEN_WIDTH 84

#include <stdint.h>

/* Fonctions qui concernent les manipulations hardware (ceci est le driver) */
void		pcd8544_init(void);
void		pcd8544_reset_DDRAM(void);
void		pcd8544_write(uint8_t data);
void		pcd8544_send_command(const uint8_t command);
void		pcd8544_send_data(const uint8_t data);

/* Fonctions qui gerent l'affichage (API : Application Programming Interface) */
void		pcd8544_clear(void);
void		pcd8544_gotoxy(const uint8_t x, const uint8_t y);
void		pcd8544_display_char(const uint8_t ascii);
void		pcd8544_display_string(const char *string);
void		pcd8544_set_mode(uint8_t mode);
void		pcd8544_finish_line(void);
#endif
