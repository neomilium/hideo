#ifndef _APP_EEPROM_H_
#define _APP_EEPROM_H_

#include "application.h"
#include "eventmanager.h"

#include "eeprom.h"

#define app_eeprom_init()	application_t app_eeprom = { &_app_eeprom_init, &_app_eeprom_event_handler, NULL }

void		_app_eeprom_init(void *data);
void		_app_eeprom_event_handler(const event_t event);

#endif				/* !_APP_EEPROM_H_ */
