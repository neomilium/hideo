#include "drv_videocontroller.h"

#include "windowmanager.h"
#include "eventmanager.h"

#include "a2d.h"
#include "hqi_control.h"
#include "display_control.h"

typedef enum {
	VIDEOCONTROLLER_STATUS_OFF,
	VIDEOCONTROLLER_STATUS_ON,
}		videocontroller_status_t;

#define VIDEOCONTROLLER_ADC_CHANNEL ADC_CH_ADC5

static videocontroller_status_t _videocontroller_status = VIDEOCONTROLLER_STATUS_OFF;

static application_t * _app_poweron;
static application_t * _app_poweroff;

void _drv_videocontroller_poll(void);

void
drv_videocontroller_init(void)
{
	register_set(DDRA, 0x00, 0b00100000);
	register_set(PORTA, 0x00, 0b00100000);

	eventmanager_add_polling_fct(_drv_videocontroller_poll);
}

videocontroller_status_t
drv_videocontroller_status(void)
{
	if(a2dConvert10bit(VIDEOCONTROLLER_ADC_CHANNEL) > 200) {
		return VIDEOCONTROLLER_STATUS_ON;
	} else {
		return VIDEOCONTROLLER_STATUS_OFF;
	}
}

void
drv_videocontroller_hook_app_poweron(application_t * app)
{
	_app_poweron = app;
}

void
drv_videocontroller_hook_app_poweroff(application_t * app)
{
	_app_poweroff = app;
}

void
_drv_videocontroller_poll(void)
{
	videocontroller_status_t videocontroller_status = drv_videocontroller_status();

	if (videocontroller_status != _videocontroller_status) {
		_videocontroller_status = videocontroller_status;

		switch(videocontroller_status) {
			case VIDEOCONTROLLER_STATUS_OFF:
				windowmanager_screensaver_disable();	// Prevent from already running screensaver
				windowmanager_launch(_app_poweroff);
				break;
			case VIDEOCONTROLLER_STATUS_ON:
				windowmanager_screensaver_disable();	// Prevent from already running screensaver
				windowmanager_launch(_app_poweron);
				break;
		}
	}
}
