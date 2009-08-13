#ifndef __LCD_H__
#define __LCD_H__

#include <avr/pgmspace.h>
#include "drv_pcd8544.h"

#define lcd_init() pcd8544_init()
#define lcd_clear() pcd8544_clear()
#define lcd_display_char( X ) pcd8544_display_char( X )
#define lcd_display_string( X ) pcd8544_display_string( X )
#define lcd_gotoxy( X, Y ) pcd8544_gotoxy( X, Y )
#define lcd_finish_line() pcd8544_finish_line()

#define LCD_MODE_NORMAL   0
#define LCD_MODE_INVERTED 1
#define lcd_set_mode(mode) pcd8544_set_mode(mode)

#define lcd_display_line( X ) pcd8544_display_string( X ); pcd8544_finish_line()

void		lcd_display_number(int16_t number);
void		lcd_display_hex(const uint8_t hex);
void		lcd_display_bcd(const uint8_t bcd);
/* void lcd_display_string( const char *string); */

#endif
