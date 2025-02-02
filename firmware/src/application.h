#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <stdint.h>
#include "eventmanager.h"

typedef struct {
	void            (*fn_init) (void *);
	void            (*fn_event_handler) (const event_t);
	void           *user_data;
}		application_t;

#endif
