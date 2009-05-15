#include "clist.h"

#define clist_next_r(name) ((name->r + 1) % name->size)
#define clist_next_w(name) ((name->w + 1) % name->size)


/**
 * @fn clist_write(uint8 data)
 *
 * @param uint8 *data Data to write to buffer.
 *
 * @return 1 on success, 0 otherwise.
 */
uint8
_clist_write(clist_t * clist, byte data)
{
	if (clist_next_w(clist) == clist->r) {
		/* Out of space ! */
		return (0);
	}
	clist->items[clist->w] = data;
	clist->w = clist_next_w(clist);

	return (1);
}

/**
 * @fn clist_read(uint8 *data)
 *
 * @param uint8 *data Data read from buffer.
 *
 * @return 1 on success, 0 otherwise.
 */
uint8
_clist_read(clist_t * clist, byte * data)
{
	if (clist->w == clist->r) {
		return (0);
	}
	*data = clist->items[clist->r];
	clist->r = clist_next_r(clist);

	return (1);
}

void
_clist_flush(clist_t * clist)
{
	clist->w = clist->r = 0;
}
