#ifndef __PS2_MOUSE_H__
#define __PS2_MOUSE_H__

#define MOUSE_BUTTON_LEFT      0x01
#define MOUSE_BUTTON_RIGHT     0x02
#define MOUSE_BUTTON_MIDDLE    0x04

void		ps2_mouse_init(void);
void		ps2_mouse_poll(void);

#endif
