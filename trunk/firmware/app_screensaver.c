#include "app_screensaver.h"

#include "lcd.h"
#include "leds.h"

#include "windowmanager.h"

void
_app_screensaver_init(void *data)
{
	lcd_clear();
	lcd_gotoxy(0, 0);
	LED0 = 0; // Backlight ON
}

void
_app_screensaver_event_handler(const event_t event)
{
	switch (event.code) {
		case E_KEY_PRESSED:
			LED0 = 1; // Backlight ON
			windowmanager_exit();
		break;
		default:
			break;
	}
}
