#ifndef __LCD_H__
#define __LCD_H__

#include "nokia_driver.h"

#define lcd_init() nokia_init()
#define lcd_clear() nokia_clear()
#define lcd_display_char( X ) nokia_display_char( X )
#define lcd_display_string( X ) nokia_display_string( X )
#define lcd_gotoxy( X, Y ) nokia_gotoxy( X, Y )

#define LCD_MODE_NORMAL   0
#define LCD_MODE_INVERTED 1
#define lcd_set_mode(mode) nokia_set_mode(mode)

#define lcd_finish_line() nokia_finish_line()


void lcd_display_number( sint16 number );
void lcd_display_hex( const byte hex );
void lcd_display_bcd( const byte bcd );
// void lcd_display_string( const char *string);

#endif
