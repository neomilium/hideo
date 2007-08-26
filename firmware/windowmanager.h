#ifndef __WINDOWMANAGER_H__
#define __WINDOWMANAGER_H__

#include "types.h"

#include "menus.h"

// void windowmanager_init(void);
void windowmanager_init(const menu_t *menu);
void windowmanager_process_events(void);

#endif
