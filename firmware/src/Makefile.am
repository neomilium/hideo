# Copyright (c) 2004,2005  Theodore A. Roth
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of the copyright holders nor the names of
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
OBJCOPY=$(AVROBJCOPY)

noinst_PROGRAMS = hideo.elf hideo.hex

hideo.hex: hideo.elf
	$(OBJCOPY) -R .eeprom -O ihex hideo.elf hideo.hex

hideo_elf_SOURCES = \
	a2d.c	\
	app_date.c	\
	app_display.c	\
	app_eeprom.c	\
	app_hqi.c	\
	app_keyboard.c	\
	app_lcd.c	\
	app_mainmenu.c	\
	app_motor.c	\
	app_mouse.c	\
	app_poweroff.c	\
	app_poweron.c	\
	app_status.c	\
	app_temperature.c	\
	buffer.c	\
	clist.c	\
	display_control.c	\
	drv_24lcxxx.c	\
	drv_dc-motor.c	\
	drv_ds1307.c	\
	drv_keyboard.c	\
	drv_lm35.c	\
	drv_nokia.c	\
	drv_ps2.c	\
	drv_ps2_mouse.c	\
	drv_pwm.c	\
	drv_stepper-motor.c	\
	drv_videocontroller.c	\
	eventmanager.c	\
	hqi_control.c	\
	i2c.c	\
	lcd.c	\
	hideo.c	\
	menus.c	\
	scheduler.c	\
	windowmanager.c

hideo_headers = \
	a2d.h	\
	app_date.h	\
	app_display.h	\
	app_eeprom.h	\
	app_hqi.h	\
	app_keyboard.h	\
	app_lcd.h	\
	application.h	\
	app_mainmenu.h	\
	app_motor.h	\
	app_mouse.h	\
	app_poweroff.h	\
	app_poweron.h	\
	app_status.h	\
	app_temperature.h	\
	avrlibdefs.h	\
	buffer.h	\
	clist.h	\
	daemon.h	\
	date.h	\
	display_control.h	\
	drv_24lcxxx.h	\
	drv_dc-motor.h	\
	drv_ds1307.h	\
	drv_keyboard.h	\
	drv_lm35.h	\
	drv_nokia.h	\
	drv_ps2.h	\
	drv_ps2_mouse.h	\
	drv_pwm.h	\
	drv_stepper-motor.h	\
	drv_videocontroller.h	\
	eeprom.h	\
	eventmanager.h	\
	fans.h	\
	global.h	\
	hideo.h	\
	hqi_control.h	\
	i2cconf.h	\
	i2c.h	\
	keyboard.h	\
	lcd.h	\
	leds.h	\
	menus.h	\
	relay.h	\
	rtc.h	\
	scheduler.h	\
	temperature_control.h	\
	thermal-sensors.h	\
	windowmanager.h

EXTRA_DIST = \
	$(hideo_headers)

