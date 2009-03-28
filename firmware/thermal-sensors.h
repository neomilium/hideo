#ifndef __THERMAL_SENSORS_H__
#define __THERMAL_SENSORS_H__

#include "drv_lm35.h"

#define thermal_sensors_init() lm35_init()
#define thermal_sensors_read( X ) lm35_read( X )


#endif
