
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "bit_field.h"

#include "types.h"
#include "ps2.h"
#include "clist.h"

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

static volatile byte _data;                  /* Byte being read */
static volatile uint8 _bit_read;             /* Number of bits already read in _data */
static volatile uint8 _bit_write;            /* Number of bits already wrote in _data */
static volatile uint8 _parity;               /* Received bit parity */
static volatile uint8 _cparity;              /* Computed parity bit */

DECLARE_CLIST(ps2, 20);

static volatile enum {PS_RX_START, PS_RX_DATA, PS_RX_PARITY, PS_RX_STOP, PS_TX_START, PS_TX_DATA, PS_TX_PARITY, PS_TX_STOP, PS_TX_ACK} ps2_state = PS_RX_START;

/* Private members */
inline void ps2_bit_available(void);
inline void ps2_inhib(void);
inline void ps2_send_bit(void);


ISR(INT2_vect) {
	if (ps2_state < PS_TX_START) {
		// RX mode
		ps2_bit_available();
	} else {
		// TX mode
		ps2_send_bit();
	}
	GIFR &= ~(1 << INT2); /* Reset interrupt flag */
}

void
ps2_init(void)
{
	/* Set up an interrupt for calling ps2_bit_available when clk 1 -> 0 */
	GICR   &= ~(1 << INT2);            /* Disable interrupt */
	clock_release();
	data_release();
	MCUCSR &= ~(1 << ISC2);            /* Falling edge trigger interrupt */
	GICR   |=  (1 << INT2);            /* Enable interrupt */
	GIFR &= ~(1 << INT2);              /* Reset interrupt flag */
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
				clist_write(ps2, _data);
			} else {
				/* Transmission error. Resend */
				ps2_write(0xFE);
			}
			ps2_state = PS_RX_START;
			break;
    default:
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
			ps2_state = PS_TX_DATA;
			break;
		case PS_TX_DATA:
			/* Least significant bits first */
			bit = (_data >> (_bit_write)) & 0x01;
			data_set(bit);
			_cparity ^= bit;
			_bit_write++;

			if (8 == _bit_write) {
				ps2_state = PS_TX_PARITY;
			}
			break;
		case PS_TX_PARITY:
			data_set(_cparity);
			ps2_state = PS_TX_STOP;
			break;
		case PS_TX_STOP:
			data_release();
			ps2_state = PS_TX_ACK;
			break;
		case PS_TX_ACK:
			bit = data_get();
			ps2_state = PS_RX_START;
      break;
    default:
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
ps2_read(uint8 *data)
{
	return(clist_read(ps2, data));
}

void
ps2_flush(void)
{
	clist_flush(ps2);
}

uint8
ps2_write(uint8 data)
{
	GICR &= ~(1 << INT2);              /* Disable interrupt */

	data_release();	
	clock_set(0);
	_delay_ms(1);
	data_set(0);
	_delay_us(50);
	_delay_us(50);
	clock_release();

	_data = data;
	ps2_state = PS_TX_START;

	GICR |= (1 << INT2);               /* Enable interrupt */

	return(0);
}
