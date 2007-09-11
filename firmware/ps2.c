
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "bit_field.h"

#include "types.h"
#include "ps2.h"

#include "lcd.h"

#define PS2_DATA_IN     GET_BIT(PINB).bit3
#define PS2_DATA_OUT    GET_BIT(PORTB).bit3
#define PS2_DATA_DDR    GET_BIT(DDRB).bit3

#define PS2_CLK_IN      GET_BIT(PINB).bit2
#define PS2_CLK_OUT     GET_BIT(PORTB).bit2
#define PS2_CLK_DDR     GET_BIT(DDRB).bit2

#define data_set(value)  do { PS2_DATA_DDR  = 1; PS2_DATA_OUT  = value; } while (0)
#define clock_set(value) do { PS2_CLK_DDR = 1; PS2_CLK_OUT = value; } while (0)

#define data_get()      (PS2_DATA_DDR = 0, PS2_DATA_OUT = 0, PS2_DATA_IN)
#define data_release()  (PS2_DATA_DDR = 0, PS2_DATA_OUT = 0)
#define clock_get()     (PS2_CLK_DDR  = 0, PS2_CLK_OUT = 0, PS2_CLK_IN)
#define clock_release() (PS2_CLK_DDR  = 0, PS2_CLK_OUT = 0)

#define BUFFER_SIZE 20
#define BUFFER_NEXT(i) ((i + 1) % BUFFER_SIZE)

static volatile uint8 _buffer[BUFFER_SIZE];
static volatile uint8 _buffer_w, _buffer_r;

static volatile uint8 _data;                 /* Byte being read */
static volatile uint8 _bit_read;             /* Number of bits already read in _data */
static volatile uint8 _parity;               /* Received bit parity */
static volatile uint8 _cparity;              /* Computed parity bit */
static ps2_data_read_t _data_read;  /* Function to call when _data is complete */

static volatile char _dump[] = "XXXXXXXXXXXXY\0";

static volatile enum {PS_START, PS_DATA, PS_PARITY, PS_STOP} ps2_state = PS_START;

/* Private members */
inline void ps2_bit_available(void);
inline void ps2_inhib(void);


/**
 * @fn ps2_buffer_write(uint8 data)
 *
 * @param uint8 *data Data to write to buffer.
 *
 * @return 1 on success, 0 otherwise.
 */
uint8
ps2_buffer_write(uint8 data)
{
	if (BUFFER_NEXT(_buffer_w) == _buffer_r) {
		/* Out of space ! */
		return(0);
	}
	_buffer[_buffer_w] = data;
	_buffer_w = BUFFER_NEXT(_buffer_w);
	return(1);
}

/**
 * @fn ps2_buffer_read(uint8 *data)
 *
 * @param uint8 *data Data read from buffer.
 *
 * @return 1 on success, 0 otherwise.
 */
uint8
ps2_buffer_read(uint8 *data)
{
	if (_buffer_w == _buffer_r) {
		return(0);
	}

	*data = _buffer[_buffer_r];
	_buffer_r = BUFFER_NEXT(_buffer_r);

	return(1);
}

ISR(INT2_vect) {
	//lcd_display_string(PSTR("I"));
	ps2_bit_available();
	GIFR &= ~(1 << INT2); /* XXX This is not used to disable interrupt !! */
}

void
ps2_debug(void)
{
	char *c = _dump;
	lcd_gotoxy(0,0);
	while (*c) {
		lcd_display_char(*c);
		c++;
	}
}

void
ps2_init(void)
{
	/* Set up an interrupt for calling ps2_bit_available when clk 1 -> 0 */
	GICR   &= ~(1 << INT2);            /* Disable interrupt */
	clock_release();
	data_release();
	//ps2_write(0xFF, 1);
	MCUCSR &= ~(1 << ISC2);            /* Falling edge trigger interrupt */
	GICR   |=  (1 << INT2);            /* Enable interrupt */
	GIFR &= ~(1 << INT2);
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
	_dump[12] = 'i';

  bit = data_get();

	switch (ps2_state) {
		case PS_START:
			_dump[0] = bit + '0';
			if (0 == bit) {
				/* Prepare for reading next byte */
				_data     = 0;
				_bit_read = 0;
				_cparity  = 1;
				ps2_state = PS_DATA;
			}
			break;
		case PS_DATA:
			/* Least significant bits first */
			_data |= (bit << _bit_read);
			_bit_read++;
  		_cparity ^= bit;
			_dump[_bit_read] = bit + '0';
			if (8 == _bit_read) {
				ps2_state = PS_PARITY;
			}
			break;
		case PS_PARITY:
			_parity = bit;
			_dump[9] = bit + '0';
			ps2_state = PS_STOP;
			break;
		case PS_STOP:
			_dump[10] = bit + '0';
			if ((1 == bit) && (_parity == _cparity)) {
				/* Send byte */
/* XXX
				if (_data_read) {
					_data_read(_data);
				}
*/
				ps2_buffer_write(_data);
				_dump[11] = ' ';
			} else {
				if (!bit) {
					_dump[11] = 'S';
				} else {
					_dump[11] = 'P';
				}
				//ps2_write(0xFE, 1); /* Same player shoot again */
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
	clock_release();
}

uint8
ps2_write(uint8 d, uint8 from_int)
{
	uint8 current_bit = 8;
	uint8 parity = 1;
	uint8 ack;


	if (!from_int) {
		GICR &= ~(1 << INT2);              /* Disable interrupt */
	}

	data_get();	
	clock_set(0);
	_delay_us(60);
	data_set(0);
	_delay_us(60);
	clock_release();

	/* start */
	while (clock_get()) ;

	/* PS2_DATA */
	while (current_bit) {
		uint8 bit = (d >> (current_bit - 1)) & 0x01;
		data_set(bit);
		parity ^= bit;
		while(!clock_get());
		while(clock_get());
		current_bit--;
	}

	/* parity */
	data_set(parity);
	while(!clock_get());
	while(clock_get());

	/* stop */
	data_release();
	while(!clock_get());
	while(clock_get());


	/* ack */
	while(!clock_get());
	while(clock_get());
	/* data_get() should return 0 */
	ack = data_get();
	
	while(clock_get() || data_get());

// 	lcd_display_string(PSTR("w"));

	if (!from_int) {
		GICR |= (1 << INT2);               /* Enable interrupt */
	}

	return(!ack);
}

