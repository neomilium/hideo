
#include <avr/io.h>
#include "bit_field.h"

#include "types.h"
#include "ps2.h"

#define data      GET_BIT(PORTB).bit1
#define clock     GET_BIT(PORTB).bit2
#define data_ddr  GET_BIT(DDRB).bit1
#define clock_ddr GET_BIT(DDRB).bit2

#define data_set(value)  do { data_ddr  = 0; data  = value; } while (0)
#define clock_set(value) do { clock_ddr = 0; clock = value; } while (0)

#define data_get()  (data_ddr  = 1, data)
#define clock_get() (clock_ddr = 1, clock)


static uint8 _data;               /* Byte being read */
static uint8 _bit_read;           /* Number of bits already read in _data */
static ps2_data_read_t _data_read;  /* Function to call when _data is complete */


void
ps2_init(void)
{
  /* TODO: set up an interrupt for calling ps2_bit_available when clk 1 -> 0 */
}

void
ps2_set_data_read(ps2_data_read_t data_read)
{
  _data_read = data_read;
}

void
ps2_bit_available(void)
{
  _bit_read++;
  /* Least significant bits first */
  _data |= (data_get() << _bit_read);

  if (8 == _bit_read) {
    /* Send byte */
    if (_data_read) {
      _data_read(_data);
    }

    /* Prepare for reading next byte */
    _data     = 0;
    _bit_read = 0;
  }
}

void
ps2_write(uint8 d)
{
  uint8 current_bit = 8;
  uint8 parity = 0;

  /* TODO: Disable ps2_bit_available interupt */

  clock_set(0);
  /* XXX wait_ms(100); */
  data_set(0);

  /* start */
  clock_set(1);
  while (clock_get()) ;

  /* data */
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
  data_set(0);
  while(!data_get());
  while(!clock_get());

  /* ack */
  while(clock_get() || data_get());

  /* TODO: Enable ps2_bit_available interupt */
}

