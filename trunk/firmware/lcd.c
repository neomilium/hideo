#include "lcd.h"

////////////////////////////////////////////////////////////////////////////////
/*
MODULO %
-21347 * -1 aff -
21347 / 10000 = 2   aff 2
21347 % 10000 = 1347 / 1000 = 1 aff 1 
21347 % 1000 = 347 / 100 = 3 aff 3
21347 % 100 = 47 / 10 = 4 aff 4
21347 % 10 = 7 aff 7
*/
void lcd_display_number( const signed char n )
{
  signed char number;
  number = n;

  if ( number < 0 ) {
    lcd_display_char( '-' );
    number = 0-number;
    }

  if( number > 99 ){
    lcd_display_char( ( number / 100 ) + '0' );
    number = number % 100;
  }
  if( number > 9 ){
    lcd_display_char( ( number / 10 ) + '0' );
    number = number % 10;
  }
  lcd_display_char( ( number ) + '0' );
}

void lcd_display_hex( const byte hex )
{
	unsigned char lsb = hex & 0x0F;
	unsigned char msb = (hex & 0xF0) >> 4;

	lcd_display_char( '0' );
	lcd_display_char( 'x' );

	if(msb < 0x0A)
		lcd_display_char( msb + '0' );
	else
		lcd_display_char( (msb - 0x0A) + 'A' );

	if(lsb < 0x0A)
		lcd_display_char( lsb + '0' );
	else
		lcd_display_char( (lsb - 0x0A) + 'A' );
}

void lcd_display_bcd( const byte bcd )
{
	unsigned char lsb = bcd & 0x0F;
	unsigned char msb = (bcd & 0xF0) >> 4;

	lcd_display_char( msb + '0' );
	lcd_display_char( lsb + '0' );
}
