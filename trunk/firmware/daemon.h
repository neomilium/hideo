#ifndef __DAEMON_H__
#define __DAEMON_H__

#include "types.h"

typedef struct {
	void (*fn_init)(void *);
	void (*fn_poll)(void);
} daemon_t;

#endif
