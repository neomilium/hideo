#ifndef __DRV_VIDEOCONTROLLER_H__
#define __DRV_VIDEOCONTROLLER_H__

#include <avr/io.h>
#include "hideo.h"

#include "types.h"

#include "application.h"

void		drv_videocontroller_init(void);
void		drv_videocontroller_hook_app_power(application_t * app);

#endif
