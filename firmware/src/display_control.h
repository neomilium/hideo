#ifndef _DISPLAY_CONTROL_H_
#define _DISPLAY_CONTROL_H_

#include <stdint.h>

void		display_init (void);
void		display_lens_set_position(int16_t position);
/* int16_t		display_lens_get_position(void); */
void		display_trapezoid_set_position(int16_t position);

void		display_lens_park(void);
void		display_lens_load_position(void);


#endif /* ! _DISPLAY_CONTROL_H_ */
