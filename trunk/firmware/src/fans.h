#ifndef _FANS_H_
#define _FANS_H_

#include <avr/io.h>
#include "hideo.h"
#include "drv_pwm.h"

#define FAN0      GET_BIT(PORTD).bit4
#define FAN1      GET_BIT(PORTD).bit6

#define fans_hqi_start() FAN0 = 1
#define fans_hqi_stop() FAN0 = 0

#define fans_lcd_start()	FAN1 = 1; \
				pwm_B_set(100)

#define fans_lcd_stop()		FAN1 = 0; \
				pwm_B_set(0)


#endif				/* !_RELAY_H_ */
