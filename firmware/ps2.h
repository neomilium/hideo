#ifndef _PS2_H_
#define _PS2_H_

void ps2_init(void);
uint8 ps2_write(uint8 data);
uint8 ps2_buffer_read(uint8 *data);

#endif /* !_PS2_H_ */
