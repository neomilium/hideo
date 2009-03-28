#ifndef __LM35_H__
#define __LM35_H__

#include "types.h"
#include "a2d.h"

#define LM35_ADC_CHANNEL_HQI		ADC_CH_ADC6
#define LM35_ADC_CHANNEL_LCD		ADC_CH_ADC7

void		lm35_init(void);
uint8		lm35_read(uint8 channel);

#endif				/* !__LM35_H__ */


