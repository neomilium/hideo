#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "types.h"
#include "menus.h"

typedef struct t_application {
	uint8 id;
	t_menu menu;
	PGM_P * title;
} t_application;

#endif
