
PROG= hideo.out

APP_SRC=	app_eeprom.c app_date.c app_keyboard.c app_lcd.c app_display.c app_mainmenu.c app_motor.c app_mouse.c app_temperature.c app_hqi.c display_control.c
DRV_SRC=	drv_dc-motor.c drv_ds1307.c drv_24lcxxx.c drv_keyboard.c drv_nokia.c drv_ps2.c drv_ps2_mouse.c drv_pwm.c drv_stepper-motor.c
CONTRIB_SRC=	a2d.c i2c.c
OS_SRC=		buffer.c clist.c eventmanager.c lcd.c main.c menus.c windowmanager.c

SRCS=	${APP_SRC} ${DRV_SRC} ${OS_SRC} ${CONTRIB_SRC}

CTAGS=	ctags

HEX= ${PROG:R}.hex

LDFLAGS= -Wa

CC= avr-gcc
CFLAGS= -fgnu89-inline -funsigned-bitfields -fpack-struct -fshort-enums -std=gnu99 -mmcu=atmega8535 -mcall-prologues -Wall -Os -DF_CPU=8000000UL
NO_MAN=

###########################################

AVRDUDE=avrdude

PROGRAMMER= stk200
MCU= m32

###########################################

SUDO= sudo

all: ${HEX}

${HEX}:  hideo.out
	avr-objcopy -R .eeprom -O ihex ${.ALLSRC} ${.TARGET}

flash: ${HEX}
	${SUDO} ${AVRDUDE} -p ${MCU} -c ${PROGRAMMER} -U flash:w:${HEX}

ALLHC!= ls *.[hc]
reindent:
.for SRC in ${ALLHC}
	@sed -I '' -e 's///' -e 's#//[ 	]*\(.*\)#/* \1 */#g' ${SRC}
	@indent -cli1 -nfc1 ${SRC} && rm ${SRC}.BAK
.endfor

.include <bsd.prog.mk>
