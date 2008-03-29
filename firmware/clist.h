#ifndef _CLIST_H_
#define _CLIST_H_

#include "types.h"

typedef struct {
	uint8 size;
	uint8 r, w;
	uint8 items[1];
} clist_t;

#define DECLARE_CLIST(name, s)						\
																					\
struct name##_clist_t {										\
	uint8 size;															\
	uint8 r;																\
	uint8 w;																\
	uint8 items[s];													\
};																				\
																					\
struct name##_clist_t name##_clist = {		\
	.size        = s,												\
	.r           = 0,												\
	.w           = 0												\
}

uint8 _clist_read(clist_t *clist, byte *data);
uint8 _clist_write(clist_t *clist, byte data);
void _clist_flush(clist_t *clist);

#define clist_read(clist, data) _clist_read((clist_t *)&clist##_clist, data)
#define clist_write(clist, data) _clist_write((clist_t *)&clist##_clist, data)
#define clist_flush(clist) _clist_flush((clist_t *)&clist##_clist)


#endif /* !_CLIST_H_ */
