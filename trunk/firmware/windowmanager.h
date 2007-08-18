#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include "types.h"
#include "application.h"

void windowmanager_init(void);
void windowmanager_record_application(const t_application app);
uint8 windowmanager_get_id(void);
void windowmanager_process_events(void);

#endif
