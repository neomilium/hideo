/*! \file i2c.h \brief I2C interface using AVR Two-Wire Interface (TWI) hardware. */
/* ***************************************************************************** */
/*  */
/* File Name	: 'i2c.h' */
/* Title		: I2C interface using AVR Two-Wire Interface (TWI) hardware */
/* Author		: Pascal Stang - Copyright (C) 2002-2003 */
/* Created		: 2002.06.25 */
/* Revised		: 2003.03.03 */
/* Version		: 0.9 */
/* Target MCU	: Atmel AVR series */
/* Editor Tabs	: 4 */
/*  */
/* /	\ingroup driver_avr */
/* / \defgroup i2c I2C Serial Interface Function Library (i2c.c) */
/* / \code #include "i2c.h" \endcode */
/* / \par Overview */
/* /		This library provides the high-level functions needed to use the I2C */
/* /	serial interface supported by the hardware of several AVR processors. */
/* / The library is functional but has not been exhaustively tested yet and is */
/* / still expanding. Thanks to the standardization of the I2C protocol and */
/* /	register access, the send and receive commands are everything you need to */
/* / talk to thousands of different I2C devices including: EEPROMS, Flash memory, */
/* / MP3 players, A/D and D/A converters, electronic potentiometers, etc. */
/* / */
/* / \par About I2C */
/* /			I2C (pronounced "eye-squared-see") is a two-wire bidirectional */
/* /		network designed for easy transfer of information between a wide variety */
/* /		of intelligent devices.  Many of the Atmel AVR series processors have */
/* /		hardware support for transmitting and receiving using an I2C-type bus. */
/* /		In addition to the AVRs, there are thousands of other parts made by */
/* /		manufacturers like Philips, Maxim, National, TI, etc that use I2C as */
/* /		their primary means of communication and control.  Common device types */
/* /		are A/D & D/A converters, temp sensors, intelligent battery monitors, */
/* /		MP3 decoder chips, EEPROM chips, multiplexing switches, etc. */
/* / */
/* /		I2C uses only two wires (SDA and SCL) to communicate bidirectionally */
/* /		between devices.  I2C is a multidrop network, meaning that you can have */
/* /		several devices on a single bus.  Because I2C uses a 7-bit number to */
/* /		identify which device it wants to talk to, you cannot have more than */
/* /		127 devices on a single bus. */
/* / */
/* /		I2C ordinarily requires two 4.7K pull-up resistors to power (one each on */
/* /		SDA and SCL), but for small numbers of devices (maybe 1-4), it is enough */
/* /		to activate the internal pull-up resistors in the AVR processor.  To do */
/* /		this, set the port pins, which correspond to the I2C pins SDA/SCL, high. */
/* /		For example, on the mega163, sbi(PORTC, 0); sbi(PORTC, 1);. */
/* / */
/* /		For complete information about I2C, see the Philips Semiconductor */
/* /		website.  They created I2C and have the largest family of devices that */
/* /		work with I2C. */
/* / */
/* / \Note: Many manufacturers market I2C bus devices under a different or generic */
/* /		bus name like "Two-Wire Interface".  This is because Philips still holds */
/* /		"I2C" as a trademark.  For example, SMBus and SMBus devices are hardware */
/* /		compatible and closely related to I2C.  They can be directly connected */
/* /		to an I2C bus along with other I2C devices are are generally accessed in */
/* /		the same way as I2C devices.  SMBus is often found on modern motherboards */
/* /		for temp sensing and other low-level control tasks. */
/*  */
/* This code is distributed under the GNU Public License */
/* which can be found at http://www.gnu.org/licenses/gpl.txt */
/*  */
/* ***************************************************************************** */

#ifndef I2C_H
#define I2C_H

#include "global.h"

/* include project-specific configuration */
#include "i2cconf.h"

/* TWSR values (not bits) */
/* (taken from avr-libc twi.h - thank you Marek Michalkiewicz) */
/* Master */
#define TW_START					0x08
#define TW_REP_START				0x10
/* Master Transmitter */
#define TW_MT_SLA_ACK				0x18
#define TW_MT_SLA_NACK				0x20
#define TW_MT_DATA_ACK				0x28
#define TW_MT_DATA_NACK				0x30
#define TW_MT_ARB_LOST				0x38
/* Master Receiver */
#define TW_MR_ARB_LOST				0x38
#define TW_MR_SLA_ACK				0x40
#define TW_MR_SLA_NACK				0x48
#define TW_MR_DATA_ACK				0x50
#define TW_MR_DATA_NACK				0x58
/* Slave Transmitter */
#define TW_ST_SLA_ACK				0xA8
#define TW_ST_ARB_LOST_SLA_ACK		0xB0
#define TW_ST_DATA_ACK				0xB8
#define TW_ST_DATA_NACK				0xC0
#define TW_ST_LAST_DATA				0xC8
/* Slave Receiver */
#define TW_SR_SLA_ACK				0x60
#define TW_SR_ARB_LOST_SLA_ACK		0x68
#define TW_SR_GCALL_ACK				0x70
#define TW_SR_ARB_LOST_GCALL_ACK	0x78
#define TW_SR_DATA_ACK				0x80
#define TW_SR_DATA_NACK				0x88
#define TW_SR_GCALL_DATA_ACK		0x90
#define TW_SR_GCALL_DATA_NACK		0x98
#define TW_SR_STOP					0xA0
/* Misc */
#define TW_NO_INFO					0xF8
#define TW_BUS_ERROR				0x00

/* defines and constants */
#define TWCR_CMD_MASK		0x0F
#define TWSR_STATUS_MASK	0xF8

/* return values */
#define I2C_OK				0x00
#define I2C_ERROR_NODEV		0x01

/* types */
typedef enum {
	I2C_IDLE = 0, I2C_BUSY = 1,
	I2C_MASTER_TX = 2, I2C_MASTER_RX = 3,
	I2C_SLAVE_TX = 4, I2C_SLAVE_RX = 5
} e_i2c_state_type;

/* functions */

/** Initialize I2C (TWI) interface */
void		i2c_init   (void);

/** Set the I2C transaction bitrate (in KHz) */
void		i2c_set_bitrate(uint16_t bitrateKHz);

/* I2C setup and configurations commands */
/** Set the local (AVR processor's) I2C device address */
void		i2c_set_local_device_addr(uint8_t device_addr, uint8_t gen_call_en);

/** Set the user function which handles receiving (incoming) data as a slave */
void		i2c_set_slave_receive_handler(void (*i2c_slave_rx_func) (uint8_t receive_data_length, uint8_t * recieve_data));
/** Set the user function which handles transmitting (outgoing) data as a slave */
void		i2c_set_slave_transmit_handler(uint8_t(*i2c_slave_tx_func) (uint8_t transmit_data_length_max, uint8_t * transmit_data));

/* Low-level I2C transaction commands  */
/** Send an I2C start condition in Master mode */
void		i2c_send_start(void);
/** Send an I2C stop condition in Master mode */
void		i2c_send_stop(void);
/** Wait for current I2C operation to complete */
void		i2c_wait_for_complete(void);
/** Send an (address|R/W) combination or a data uint8_t over I2C */
void		i2c_send_uint8_t (uint8_t data);
/** Receive a data uint8_t over I2C   */
/* ackFlag = true if recevied data should be ACK'ed */
/* ackFlag = false if recevied data should be NACK'ed */
void		i2c_receive_uint8_t (uint8_t ack_flag);
/** Pick up the data that was received with i2cReceiveByte() */
uint8_t		i2c_get_received_uint8_t (void);
/** Get current I2c bus status from TWSR */
uint8_t		i2c_get_status(void);

/* high-level I2C transaction commands */

/** send I2C data to a device on the bus */
void		i2c_master_send(uint8_t device_addr, uint8_t length, uint8_t * data);
/** receive I2C data from a device on the bus */
void		i2c_master_receive(uint8_t device_addr, uint8_t length, uint8_t * data);

/** send I2C data to a device on the bus (non-interrupt based) */
uint8_t		i2c_master_send_ni(uint8_t device_addr, uint8_t length, uint8_t * data);
uint8_t		i2c_master_hsend_ni(uint8_t device_addr, uint8_t hlength, uint8_t *header, uint8_t length, uint8_t * data);
/** receive I2C data from a device on the bus (non-interrupt based) */
uint8_t		i2c_master_receive_ni(uint8_t device_addr, uint8_t length, uint8_t * data);

/** Get the current high-level state of the I2C interface */
e_i2c_state_type	i2c_get_state(void);

#endif
