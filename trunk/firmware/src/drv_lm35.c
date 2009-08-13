#include "drv_lm35.h"

#include <avr/io.h>
#include "bitfield.h"

void
lm35_init(void)
{
	register_set(DDRA, 0x00, 0b11000000);
	register_set(PORTA, 0x00, 0b11000000);

	a2dInit();
	a2dSetReference(ADC_REFERENCE_AVCC);
}

uint8_t 
lm35_read(uint8_t channel)
{
	return(a2dConvert10bit(channel) / 2 );
}
