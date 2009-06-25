#ifndef _HQI_CONTROL_H_
#define _HQI_CONTROL_H_

#include <stdint.h>

typedef enum {
	HQI_MODE_READY,
	HQI_MODE_RUNNING,
	HQI_MODE_COOLING,
} hqi_mode_t;

void		hqi_init (void);
void		hqi_start(void);
void		hqi_stop(void);
hqi_mode_t	hqi_mode(void);
uint16_t		hqi_remaining_time_before_ready(void);
uint32_t		hqi_uptime(void);
uint8_t 		hqi_temperature(void);
void		hqi_lifetime_counter_reset(void);

#endif /* ! _HQI_CONTROL_H_ */
