
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "bit_field.h"

#include "types.h"
#include "ps2.h"

#include "lcd.h"

#define PS2_DATA        GET_BIT(PORTB).bit3
#define PS2_DDR_DATA    GET_BIT(DDRB).bit3

#define PS2_CLK         GET_BIT(PORTB).bit2
#define PS2_DDR_CLK     GET_BIT(DDRB).bit2

#define data_set(value)  do { PS2_DDR_DATA  = 1; PS2_DATA  = value; } while (0)
#define clock_set(value) do { PS2_DDR_CLK = 1; PS2_CLK = value; } while (0)

#define data_get()      (PS2_DDR_DATA  = 0, PS2_DATA = 0, PS2_DATA)
#define data_release()  (PS2_DDR_DATA  = 0, PS2_DATA = 0)
#define clock_get()     (PS2_DDR_CLK = 0, PS2_CLK = 0, PS2_CLK)
#define clock_release() (PS2_DDR_CLK = 0, PS2_CLK = 0)


static volatile uint8 _data;                 /* Byte being read */
static volatile uint8 _bit_read;             /* Number of bits already read in _data */
static volatile uint8 _parity;               /* Received bit parity */
static ps2_data_read_t _data_read;  /* Function to call when _data is complete */

static enum {PS_START, PS_DATA, PS_PARITY, PS_STOP} ps2_state = PS_START;

/* Private members */
inline void ps2_bit_available(void);
inline void ps2_inhib(void);

ISR(INT2_vect) {
	lcd_display_string(PSTR("I"));
	ps2_bit_available();
	GIFR &= ~INT2;
}

void
ps2_init(void)
{

  /* Set up an interrupt for calling ps2_bit_available when clk 1 -> 0 */
	GICR &= ~INT2;              /* Disable interrupt */
	MCUCSR &= ~ISC2;            /* Falling edge trigger interrupt */
	GICR |= INT2;               /* Enable interrupt */

	clock_get();
	data_get();
}

void
ps2_set_data_read(ps2_data_read_t data_read)
{
  _data_read = data_read;
}

inline void
ps2_bit_available(void)
{
	uint8 bit;

  bit = data_get();

	switch (ps2_state) {
		case PS_START:
			lcd_display_string(PSTR("S"));
			if (0 == bit) {
				/* Prepare for reading next byte */
				_data     = 0;
				_bit_read = 0;
				_parity   = 0;

				ps2_state = PS_DATA;
			} else {
				ps2_inhib();
			}
			break;
		case PS_DATA:
			lcd_display_string(PSTR("D"));
			_bit_read++;
			/* Least significant bits first */
			_data |= (bit << _bit_read);
  		_parity ^= bit;
			if (8 == _bit_read) {
				ps2_state = PS_PARITY;
			}
			break;
		case PS_PARITY:
			lcd_display_string(PSTR("P"));
			if (bit == _parity) {
				ps2_state = PS_STOP;
			} else {
				ps2_state = PS_START;
				ps2_inhib();
			}
			break;
		case PS_STOP:
			lcd_display_line(PSTR("X"));
			if (1 == bit) {
				/* Send byte */
				if (_data_read) {
					_data_read(_data);
				}
			} else {
				ps2_inhib();
			}
			ps2_state = PS_START;
			break;
	}
}

inline void
ps2_inhib(void)
{
	clock_set(0);
	_delay_us(60);
	_delay_us(60);
	clock_set(1);
}

void
ps2_write(uint8 d)
{
	uint8 current_bit = 8;
	uint8 parity = 0;

	lcd_display_string(PSTR("W"));

	GICR &= ~INT2;              /* Disable interrupt */

	data_get();	
	clock_set(0);
	_delay_us(60);
	_delay_us(60);
	data_set(0);
	_delay_us(60);
	_delay_us(60);
	clock_release();



// 	clock_set(0);
// 	/* XXX wait_ms(100); */
// 	_delay_us(60);
// 	_delay_us(60);
// 	data_set(0);
// 	_delay_us(5);

	/* start */
	lcd_display_string(PSTR("S"));
	while (clock_get()) ;

	/* PS2_DATA */
	while (current_bit) {
		lcd_display_string(PSTR("D"));
		uint8 bit = (d >> (current_bit - 1)) & 0x01;
		data_set(bit);
		parity ^= bit;
		lcd_display_string(PSTR("L"));
		while(!clock_get());
		lcd_display_string(PSTR("H"));
		while(clock_get());
		current_bit--;
	}

	/* parity */
	lcd_display_string(PSTR("P"));
	data_set(parity);
	lcd_display_string(PSTR("L"));
	while(!clock_get());
	lcd_display_string(PSTR("H"));
	while(clock_get());

	/* stop */
	lcd_display_string(PSTR("s"));
	//data_set(0);
	while(!clock_get());
	while(clock_get());
	/* data_get() should return 0 */

	/* ack */
	lcd_display_string(PSTR("A"));
	while(clock_get() || data_get());

	GICR |= INT2;               /* Enable interrupt */
}

