#ifndef _CLIST_H_
#define _CLIST_H_

#include <stdint.h>

typedef struct {
	uint8_t 		size;
	uint8_t 		r       , w;
	uint8_t 		items    [1];
}		clist_t;

#define DECLARE_CLIST(name, s)						\
																					\
struct name##_clist_t {										\
	uint8_t size;															\
	uint8_t r;																\
	uint8_t w;																\
	uint8_t items[s];													\
};																				\
																					\
struct name##_clist_t name##_clist = {		\
	.size        = s,												\
	.r           = 0,												\
	.w           = 0												\
}

uint8_t 		_clist_read(clist_t * clist, uint8_t * data);
uint8_t 		_clist_write(clist_t * clist, uint8_t data);
void		_clist_flush(clist_t * clist);

#define clist_read(clist, data) _clist_read((clist_t *)&clist##_clist, data)
#define clist_write(clist, data) _clist_write((clist_t *)&clist##_clist, data)
#define clist_flush(clist) _clist_flush((clist_t *)&clist##_clist)


#endif				/* !_CLIST_H_ */
