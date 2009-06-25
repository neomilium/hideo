#ifndef __DRV_VIDEOCONTROLLER_H__
#define __DRV_VIDEOCONTROLLER_H__

#include <avr/io.h>
#include "hideo.h"

#include <stdint.h>

#include "application.h"

void		drv_videocontroller_init(void);
void		drv_videocontroller_hook_app_poweron(application_t * app);
void		drv_videocontroller_hook_app_poweroff(application_t * app);

#endif
