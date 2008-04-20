#ifndef __LENS_CONTROL_H__
#define __LENS_CONTROL_H__

#include "types.h"

void lens_init(void);
void lens_set_position(uint16 position);
uint16 lens_get_position(void);


#endif
