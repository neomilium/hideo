#ifndef _HIDEO_H_
#define _HIDEO_H_

#define register_set( REGISTER, VALUE, MASK ) do { REGISTER = ( ((REGISTER) & ~(MASK)) | (VALUE) ); } while (0)

// Define the bits in the port
typedef struct
{
   unsigned char bit0 : 1,
                 bit1 : 1,
                 bit2 : 1,
                 bit3 : 1,
                 bit4 : 1,
                 bit5 : 1,
                 bit6 : 1,
                 bit7 : 1;
} bit_field;

// Define macro to get the value of each bit
#define GET_BIT(port) (*(volatile bit_field *) (_SFR_ADDR(port)))

#endif /* !_HIDEO_H_ */

