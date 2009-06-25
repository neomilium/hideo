#ifndef __PS2_H__
#define __PS2_H__

void		ps2_init  (void);
uint8_t		ps2_write(uint8_t data);
uint8_t		ps2_read (uint8_t * data);
void		ps2_flush (void);

#endif				/* !__PS2_H__ */
