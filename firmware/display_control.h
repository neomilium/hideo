#ifndef __LENS_CONTROL_H__
#define __LENS_CONTROL_H__

#include "types.h"

void		display_init (void);
void		display_lens_set_position(sint16 position);
/* sint16		display_lens_get_position(void); */
void		display_trapezoid_set_position(sint16 position);


#endif
