#ifndef _PS2_H_
#define _PS2_H_

typedef void (*ps2_data_read_t)(uint8 data);

void ps2_init(void);
void ps2_set_data_read(ps2_data_read_t data_read);
uint8 ps2_write(uint8 d);

#endif /* !_PS2_H_ */
