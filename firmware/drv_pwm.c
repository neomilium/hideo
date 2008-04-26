#include "drv_pwm.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
/* #include "bit_field.h" */

#define PWM_PERIOD		0x06E2
void
pwm_init(void)
{
	TCCR1A = 0xA2;		/* */
	TCCR1B = 0x1B;		/* */

	ICR1 = PWM_PERIOD;	/* period */
	OCR1A = 0x0000;		/* Up time for OC1A */
	OCR1B = 0x0000;		/* Up time for OC1B */
	DDRD = 0xFF;
}

void
pwm_A_set(uint8 percents)
{
	OCR1A = (PWM_PERIOD / 100) * percents;
}

void
pwm_B_set(uint8 percents)
{
	OCR1B = (PWM_PERIOD / 100) * percents;
}
