
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
static volatile uint8 _bit_write;             /* Number of bits already read in _data */
static volatile uint8 _parity;               /* Received bit parity */
static volatile uint8 _cparity;              /* Computed parity bit */
static ps2_data_read_t _data_read;  /* Function to call when _data is complete */

static volatile char _dump[] = "XXXXXXXXXXXXYA\0";

static volatile enum {PS_RX_START, PS_RX_DATA, PS_RX_PARITY, PS_RX_STOP, PS_TX_START, PS_TX_DATA, PS_TX_PARITY, PS_TX_STOP, PS_TX_ACK} ps2_state = PS_RX_START;

/* Private members */
inline void ps2_bit_available(void);
inline void ps2_inhib(void);
inline void ps2_send_bit(void);




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
	if (ps2_state < PS_TX_START) {
		// RX mode
		ps2_bit_available();
	} else {
		// TX mode
		ps2_send_bit();
	}
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

  bit = data_get();

	switch (ps2_state) {
		case PS_RX_START:
			if (0 == bit) {
				/* Prepare for reading next byte */
				_data     = 0;
				_bit_read = 0;
				_cparity  = 1;
				ps2_state = PS_RX_DATA;
			}
			break;
		case PS_RX_DATA:
			/* Least significant bits first */
			_data |= (bit << _bit_read);
			_bit_read++;
  		_cparity ^= bit;
			if (8 == _bit_read) {
				ps2_state = PS_RX_PARITY;
			}
			break;
		case PS_RX_PARITY:
			_parity = bit;
			ps2_state = PS_RX_STOP;
			break;
		case PS_RX_STOP:
			if ((1 == bit) && (_parity == _cparity)) {
				/* Send byte */
				ps2_buffer_write(_data);
			} else {
/*
				if (!bit) {
					_dump[11] = 'S';
				} else {
					_dump[11] = 'P';
				}
				//ps2_write(0xFE, 1);
*/
			}
			ps2_state = PS_RX_START;
			break;
	}
}

inline void
ps2_send_bit(void)
{
	uint8 bit;

	switch (ps2_state) {
		case PS_TX_START:
			_bit_write = 0;
			_cparity  = 1;
			_dump[0] = 's';
			ps2_state = PS_TX_DATA;
			break;
		case PS_TX_DATA:
			/* Least significant bits first */
			bit = (_data >> (_bit_write)) & 0x01;
			data_set(bit);
			_dump[_bit_write+1] = bit + '0';
			_cparity ^= bit;
			_bit_write++;

			if (8 == _bit_write) {
				ps2_state = PS_TX_PARITY;
			}
			break;
		case PS_TX_PARITY:
			data_set(_cparity);
			_dump[9] = _cparity + '0';
			ps2_state = PS_TX_STOP;
			break;
		case PS_TX_STOP:
			data_release();
			_dump[10] = 'S';
			ps2_state = PS_TX_ACK;
			break;
		case PS_TX_ACK:
			bit = data_get();
			_dump[11] = bit + '0';
			ps2_state = PS_RX_START;
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

	GICR &= ~(1 << INT2);              /* Disable interrupt */

	data_release();	
	clock_set(0);
	_delay_ms(1);
	data_set(0);
	_delay_us(50);
	_delay_us(50);
	clock_release();

	_data = d;
	ps2_state = PS_TX_START;

	GICR |= (1 << INT2);               /* Enable interrupt */

	return(0);
}
