#include "drv_pwm.h"

#include <avr/io.h>

#define PWM_PERIOD		3333
void
pwm_init(void)
{
	TCCR1A = 0b10100010;	/* */
	TCCR1B = 0b00011011;	/* Prescaler = clock / 8
				 * Fast PWM mode */

	ICR1 = PWM_PERIOD;	/* period */
	OCR1A = 0x0000;		/* Up time for OC1A */
	OCR1B = 0x0000;		/* Up time for OC1B */
	DDRD = 0xFF;
}

void
pwm_A_set(uint8_t percents)
{
	OCR1A = (PWM_PERIOD / 100) * percents;
}

void
pwm_B_set(uint8_t percents)
{
	OCR1B = (PWM_PERIOD / 100) * percents;
}
