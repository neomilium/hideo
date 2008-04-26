#ifndef __PS2_H__
#define __PS2_H__

void ps2_init(void);
uint8 ps2_write(uint8 data);
uint8 ps2_read(uint8 *data);
void ps2_flush(void);

#endif /* !__PS2_H__ */
