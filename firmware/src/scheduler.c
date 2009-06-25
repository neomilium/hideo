#include "scheduler.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "hideo.h"
#include "eventmanager.h"


static volatile uint8_t _scheduler_logical_counter = 0;

#define SCHEDULER_TICK_SECOND 0x00

ISR (TIMER0_OVF_vect) {
	TCNT0 = 256 - 250;		// Interrupt occurs (8 000 000 / 256) / 250 = 125 hz
	_scheduler_logical_counter++;
}

void
scheduler_init(void) {
	register_set(TCCR0, (1<<CS02)|(0<<CS01)|(0<<CS00), 0b00000111); //	CS02 = 1; CS01 = 0; CS00 = 0; // Timer1 clock use prescaled clock at clk/256 (i.e. 8 000 000 / 256 = 31250)
	TCNT0 = 256 - 250;		// Interrupt occurs (8 000 000 / 256) / 250 = 125 hz

	TIFR  &= ~(1 << TOV0);	/* Reset interrupt flag */
	TIMSK |= (1 << TOIE0);	/* Enable interrupt */

	eventmanager_add_polling_fct(scheduler_poll);
}

void
scheduler_poll(void) {
	if( _scheduler_logical_counter > 125 ) { // ((8 000 000 / 256) / 250) / 125 = 1hz
		event_t		event = {
			.code = E_SCHEDULER_TICK,
			.data = SCHEDULER_TICK_SECOND
		};
		eventmanager_push(event);
		_scheduler_logical_counter = 0;
	}
}

