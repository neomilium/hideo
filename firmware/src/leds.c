#include "leds.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "bitfield.h"

#define LED0    GET_BIT(PORTC).bit2
#define LED1    GET_BIT(PORTC).bit3
#define LED2    GET_BIT(PORTC).bit4

#define LED_ENABLED	0
#define LED_DISABLED	1


enum { UP, DOWN };

#define TIMER2_PWM_INIT		_BV(WGM21) | _BV(WGM20) | _BV(COM21) | _BV(CS21) | _BV(CS20)
#define OCR			OCR2
#define DDROC			DDRD
#define OC2			PD7
#define TIMER2_TOP		255

ISR (TIMER2_OVF_vect)
{
    static uint16_t_t pwm;
    static uint8_t direction;

	switch (direction)
	{
	case UP:
		if (++pwm == TIMER2_TOP)
		direction = DOWN;
		break;
	
	case DOWN:
		if (--pwm == 0)
		direction = UP;
		break;
	}
	
	OCR = pwm;
}

void 
leds_init(void)
{
	/* Enable LEDs port as output. */
	DDRC |= (_BV(PC2) | _BV(PC3) | _BV(PC4));
	DDRB |= _BV(PB3);

	LED0 = LED_ENABLED;
	LED1 = LED_ENABLED;
	LED2 = LED_ENABLED;


	/* Enable OC2 as output. */
	DDROC = _BV(OC2);

	/* Timer 2 is 8-bit PWM. */
	TCCR2 = TIMER2_PWM_INIT;

	/* Set PWM value to 0. */
	OCR = 0;

	/* Enable timer 2 overflow interrupt. */
	TIFR &= ~(_BV (TOV2));
	TIMSK |= _BV(TOIE2);
}
