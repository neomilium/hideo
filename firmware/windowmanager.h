#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include "types.h"

#include "application.h"

void windowmanager_init(void);
void windowmanager_process_events(void);
void windowmanager_launch(application_t *app);
void windowmanager_exit(void);

#endif
