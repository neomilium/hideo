/**
 * @file i2c.c
 * @brief I2C interface using AVR Two-Wire Interface (TWI) hardware.
 *
 *
 * File Name	: 'i2c.c'
 * Title	: I2C interface using AVR Two-Wire Interface (TWI) hardware
 * Author	: Pascal Stang - Copyright (C) 2002-2003
 * Created	: 2002.06.25
 * Revised	: 2003.03.02
 * Version	: 0.9
 * Target MCU	: Atmel AVR series
 * Editor Tabs	: 4
 *
 * This code is distributed under the GNU Public License
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c.h"

/* Standard I2C bit rates are: */
/* 100KHz for slow speed */
/* 400KHz for high speed */

/* #define I2C_DEBUG */

/* I2C state and address variables */
static volatile e_i2c_state_type _i2c_state;
static u08	_i2c_device_addr_rw;
/* send/transmit buffer (outgoing data) */
static u08	_i2c_send_data[I2C_SEND_DATA_BUFFER_SIZE];
static u08	_i2c_send_data_index;
static u08	_i2c_send_data_length;
/* receive buffer (incoming data) */
static u08	_i2c_receive_data[I2C_RECEIVE_DATA_BUFFER_SIZE];
static u08	_i2c_receive_data_index;
static u08	_i2c_receive_data_length;

/* function pointer to i2c receive routine */
/* ! _i2cSlaveReceive is called when this processor */
/* is addressed as a slave for writing */
static void     (*i2c_slave_receive) (u08 receive_data_length, u08 * receive_data);
/* ! _i2cSlaveTransmit is called when this processor */
/* is addressed as a slave for reading */
static		u08     (*i2c_slave_transmit) (u08 transmit_data_length_max, u08 * transmit_data);

/* functions */
void
i2c_init(void)
{
	/* set pull-up resistors on I2C bus pins */
	/* TODO: should #ifdef these */
	sbi(PORTC, 0);		/* i2c SCL on ATmega163,323,16,32,etc */
	sbi(PORTC, 1);		/* i2c SDA on ATmega163,323,16,32,etc */
// 	sbi(PORTD, 0);		/* i2c SCL on ATmega128,64 */
// 	sbi(PORTD, 1);		/* i2c SDA on ATmega128,64 */

	/* clear SlaveReceive and SlaveTransmit handler to null */
	i2c_slave_receive = 0;
	i2c_slave_transmit = 0;
	/* set i2c bit rate to 100KHz */
	i2c_set_bitrate(100);
	/* enable TWI (two-wire interface) */
	sbi(TWCR, TWEN);
	/* set state */
	_i2c_state = I2C_IDLE;
	/* enable TWI interrupt and slave address ACK */
	sbi(TWCR, TWIE);
	sbi(TWCR, TWEA);
	/* outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA)); */
	/* enable interrupts */
	sei();
}

void
i2c_set_bitrate(u16 bitrateKHz)
{
	u08		bitrate_div;
	/* set i2c bitrate */
	/* SCL freq = F_CPU/(16+2*TWBR)) */
#ifdef TWPS0
	/* for processors with additional bitrate division (mega128) */
	/* SCL freq = F_CPU/(16+2*TWBR*4^TWPS) */
	/* set TWPS to zero */
	cbi(TWSR, TWPS0);
	cbi(TWSR, TWPS1);
#endif
	/* calculate bitrate division	 */
	bitrate_div = ((F_CPU / 1000l) / bitrateKHz);
	if (bitrate_div >= 16)
		bitrate_div = (bitrate_div - 16) / 2;
	outb(TWBR, bitrate_div);
}

void
i2c_set_local_device_addr(u08 device_addr, u08 gen_call_en)
{
	/* set local device address (used in slave mode only) */
	outb(TWAR, ((device_addr & 0xFE) | (gen_call_en ? 1 : 0)));
}

void
i2c_set_slave_receive_handler(void (*i2c_slave_rx_func) (u08 receive_data_length, u08 * recieve_data))
{
	i2c_slave_receive = i2c_slave_rx_func;
}

void
i2c_set_slave_transmit_handler(u08(*i2c_slave_tx_func) (u08 transmit_data_length_max, u08 * transmit_data))
{
	i2c_slave_transmit = i2c_slave_tx_func;
}

inline void
i2c_send_start(void)
{
	/* send start condition */
	outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWSTA));
}

inline void
i2c_send_stop(void)
{
	/* transmit stop condition */
	/* leave with TWEA on for slave receiving */
	outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA) | BV(TWSTO));
}

inline void
i2c_wait_for_complete(void)
{
	/* wait for i2c interface to complete operation */
	while (!(inb(TWCR) & BV(TWINT)));
}

inline void
i2c_send_byte(u08 data)
{
	/* save data to the TWDR */
	outb(TWDR, data);
	/* begin send */
	outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
}

inline void
i2c_receive_byte(u08 ack_flag)
{
	/* begin receive over i2c */
	if (ack_flag) {
		/* ackFlag = TRUE: ACK the recevied data */
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
	} else {
		/* ackFlag = FALSE: NACK the recevied data */
		outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
	}
}

inline		u08
i2c_get_received_byte(void)
{
	/* retieve received data byte from i2c TWDR */
	return (inb(TWDR));
}

inline		u08
i2c_get_status(void)
{
	/* retieve current i2c status from i2c TWSR */
	return (inb(TWSR));
}

void
i2c_master_send(u08 device_addr, u08 length, u08 * data)
{
	u08		i;
	/* wait for interface to be ready */
	while (_i2c_state);
	/* set state */
	_i2c_state = I2C_MASTER_TX;
	/* save data */
	_i2c_device_addr_rw = (device_addr & 0xFE);	/* RW cleared: write
						 * operation */
	for (i = 0; i < length; i++)
		_i2c_send_data[i] = *data++;
	_i2c_send_data_index = 0;
	_i2c_send_data_length = length;
	/* send start condition */
	i2c_send_start();
}

void
i2c_master_receive(u08 device_addr, u08 length, u08 * data)
{
	u08		i;
	/* wait for interface to be ready */
	while (_i2c_state);
	/* set state */
	_i2c_state = I2C_MASTER_RX;
	/* save data */
	_i2c_device_addr_rw = (device_addr | 0x01);	/* RW set: read operation */
	_i2c_receive_data_index = 0;
	_i2c_receive_data_length = length;
	/* send start condition */
	i2c_send_start();
	/* wait for data */
	while (_i2c_state);
	/* return data */
	for (i = 0; i < length; i++)
		*data++ = _i2c_receive_data[i];
}

u08
i2c_master_send_ni(u08 device_addr, u08 length, u08 * data)
{
	return(i2c_master_hsend_ni(device_addr, 0, 0, length, data));
}
u08
i2c_master_hsend_ni(u08 device_addr, u08 hlength, u08 *header, u08 length, u08 * data)
{
	u08		retval = I2C_OK;

	/* disable TWI interrupt */
	cbi(TWCR, TWIE);

	/* send start condition */
	i2c_send_start();
	i2c_wait_for_complete();

	/* send device address with write */
	i2c_send_byte(device_addr & 0xFE);
	i2c_wait_for_complete();

	/* check if device is present and live */
	if (inb(TWSR) == TW_MT_SLA_ACK) {
		/* send header */
		while (hlength) {
			i2c_send_byte(*header++);
			i2c_wait_for_complete();
			hlength--;
		}
		/* send data */
		while (length) {
			i2c_send_byte(*data++);
			i2c_wait_for_complete();
			length--;
		}
	} else {
		/* device did not ACK it's address, */
		/* data will not be transferred */
		/* return error */
		retval = I2C_ERROR_NODEV;
	}

	/* transmit stop condition */
	/* leave with TWEA on for slave receiving */
	i2c_send_stop();
	while (!(inb(TWCR) & BV(TWSTO)));

	/* enable TWI interrupt */
	sbi(TWCR, TWIE);

	return retval;
}

u08
i2c_master_receive_ni(u08 device_addr, u08 length, u08 * data)
{
	u08		retval = I2C_OK;

	/* disable TWI interrupt */
	cbi(TWCR, TWIE);

	/* send start condition */
	i2c_send_start();
	i2c_wait_for_complete();

	/* send device address with read */
	i2c_send_byte(device_addr | 0x01);
	i2c_wait_for_complete();

	/* check if device is present and live */
	if (inb(TWSR) == TW_MR_SLA_ACK) {
		/* accept receive data and ack it */
		while (length > 1) {
			i2c_receive_byte(TRUE);
			i2c_wait_for_complete();
			*data++ = i2c_get_received_byte();
			/* decrement length */
			length--;
		}

		/* accept receive data and nack it (last-byte signal) */
		i2c_receive_byte(FALSE);
		i2c_wait_for_complete();
		*data++ = i2c_get_received_byte();
	} else {
		/* device did not ACK it's address, */
		/* data will not be transferred */
		/* return error */
		retval = I2C_ERROR_NODEV;
	}

	/* transmit stop condition */
	/* leave with TWEA on for slave receiving */
	i2c_send_stop();

	/* enable TWI interrupt */
	sbi(TWCR, TWIE);

	return retval;
}
#if 0
void
i2cMasterTransferNI(u08 deviceAddr, u08 sendlength, u08 * senddata, u08 receivelength, u08 * receivedata)
{
	/* disable TWI interrupt */
	cbi(TWCR, TWIE);

	/* send start condition */
	i2cSendStart();
	i2cWaitForComplete();

	/* if there's data to be sent, do it */
	if (sendlength) {
		/* send device address with write */
		i2cSendByte(deviceAddr & 0xFE);
		i2cWaitForComplete();

		/* send data */
		while (sendlength) {
			i2cSendByte(*senddata++);
			i2cWaitForComplete();
			sendlength--;
		}
	}
	/* if there's data to be received, do it */
	if (receivelength) {
		/* send repeated start condition */
		i2cSendStart();
		i2cWaitForComplete();

		/* send device address with read */
		i2cSendByte(deviceAddr | 0x01);
		i2cWaitForComplete();

		/* accept receive data and ack it */
		while (receivelength > 1) {
			i2cReceiveByte(TRUE);
			i2cWaitForComplete();
			*receivedata++ = i2cGetReceivedByte();
			/* decrement length */
			receivelength--;
		}

		/* accept receive data and nack it (last-byte signal) */
		i2cReceiveByte(TRUE);
		i2cWaitForComplete();
		*receivedata++ = i2cGetReceivedByte();
	}
	/* transmit stop condition */
	/* leave with TWEA on for slave receiving */
	i2cSendStop();
	while (!(inb(TWCR) & BV(TWSTO)));

	/* enable TWI interrupt */
	sbi(TWCR, TWIE);
}
#endif

/* ! I2C (TWI) interrupt service routine */
SIGNAL(SIG_2WIRE_SERIAL)
{
	/* read status bits */
	u08		status = inb(TWSR) & TWSR_STATUS_MASK;

	switch (status) {
			/* Master General */
		case TW_START:	/* 0x08: Sent start condition */
		case TW_REP_START:	/* 0x10: Sent repeated start
					 * condition */
			/* send device address */
			i2c_send_byte(_i2c_device_addr_rw);
			break;

			/* Master Transmitter & Receiver status codes */
		case TW_MT_SLA_ACK:	/* 0x18: Slave address acknowledged */
		case TW_MT_DATA_ACK:	/* 0x28: Data acknowledged */
			if (_i2c_send_data_index < _i2c_send_data_length) {
				/* send data */
				i2c_send_byte(_i2c_send_data[_i2c_send_data_index++]);
			} else {
				/* transmit stop condition, enable SLA ACK */
				i2c_send_stop();
				/* set state */
				_i2c_state = I2C_IDLE;
			}
			break;
		case TW_MR_DATA_NACK:	/* 0x58: Data received, NACK reply
					 * issued */
			/* store final received data byte */
			_i2c_receive_data[_i2c_receive_data_index++] = inb(TWDR);
			/* continue to transmit STOP condition */
		case TW_MR_SLA_NACK:	/* 0x48: Slave address not
					 * acknowledged */
		case TW_MT_SLA_NACK:	/* 0x20: Slave address not
					 * acknowledged */
		case TW_MT_DATA_NACK:	/* 0x30: Data not acknowledged */
			/* transmit stop condition, enable SLA ACK */
			i2c_send_stop();
			/* set state */
			_i2c_state = I2C_IDLE;
			break;
		case TW_MT_ARB_LOST:	/* 0x38: Bus arbitration lost */
			/* release bus */
			outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
			/* set state */
			_i2c_state = I2C_IDLE;
			/* release bus and transmit start when bus is free */
			/*
			 * outb(TWCR,
			 * (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWSTA));
			 */
			break;
		case TW_MR_DATA_ACK:	/* 0x50: Data acknowledged */
			/* store received data byte */
			_i2c_receive_data[_i2c_receive_data_index++] = inb(TWDR);
			/* fall-through to see if more bytes will be received */
		case TW_MR_SLA_ACK:	/* 0x40: Slave address acknowledged */
			if (_i2c_receive_data_index < (_i2c_receive_data_length - 1))
				/*
				 * data byte will be received, reply with ACK
				 * (more bytes in transfer)
				 */
				i2c_receive_byte(TRUE);
			else
				/*
				 * data byte will be received, reply with
				 * NACK (final byte in transfer)
				 */
				i2c_receive_byte(FALSE);
			break;

			/* Slave Receiver status codes */
		case TW_SR_SLA_ACK:	/* 0x60: own SLA+W has been received,
					 * ACK has been returned */
		case TW_SR_ARB_LOST_SLA_ACK:	/* 0x68: own SLA+W has been
						 * received, ACK has been
						 * returned */
		case TW_SR_GCALL_ACK:	/* 0x70:     GCA+W has been received,
					 * ACK has been returned */
		case TW_SR_ARB_LOST_GCALL_ACK:	/* 0x78:     GCA+W has been
						 * received, ACK has been
						 * returned */
			/*
			 * we are being addressed as slave for writing (data
			 * will be received from master)
			 */
			/* set state */
			_i2c_state = I2C_SLAVE_RX;
			/* prepare buffer */
			_i2c_receive_data_index = 0;
			/* receive data byte and return ACK */
			outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
			break;
		case TW_SR_DATA_ACK:	/* 0x80: data byte has been received,
					 * ACK has been returned */
		case TW_SR_GCALL_DATA_ACK:	/* 0x90: data byte has been
						 * received, ACK has been
						 * returned */
			/* get previously received data byte */
			_i2c_receive_data[_i2c_receive_data_index++] = inb(TWDR);
			/* check receive buffer status */
			if (_i2c_receive_data_index < I2C_RECEIVE_DATA_BUFFER_SIZE) {
				/* receive data byte and return ACK */
				i2c_receive_byte(TRUE);
				/*
				 * outb(TWCR,
				 * (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA
				 * ));
				 */
			} else {
				/* receive data byte and return NACK */
				i2c_receive_byte(FALSE);
				/*
				 * outb(TWCR,
				 * (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
				 */
			}
			break;
		case TW_SR_DATA_NACK:	/* 0x88: data byte has been received,
					 * NACK has been returned */
		case TW_SR_GCALL_DATA_NACK:	/* 0x98: data byte has been
						 * received, NACK has been
						 * returned */
			/* receive data byte and return NACK */
			i2c_receive_byte(FALSE);
			/* outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)); */
			break;
		case TW_SR_STOP:	/* 0xA0: STOP or REPEATED START has
					 * been received while addressed as
					 * slave */
			/* switch to SR mode with SLA ACK */
			outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
			/* i2c receive is complete, call i2cSlaveReceive */
			if (i2c_slave_receive)
				i2c_slave_receive(_i2c_receive_data_index, _i2c_receive_data);
			/* set state */
			_i2c_state = I2C_IDLE;
			break;

			/* Slave Transmitter */
		case TW_ST_SLA_ACK:	/* 0xA8: own SLA+R has been received,
					 * ACK has been returned */
		case TW_ST_ARB_LOST_SLA_ACK:	/* 0xB0:     GCA+R has been
						 * received, ACK has been
						 * returned */
			/*
			 * we are being addressed as slave for reading (data
			 * must be transmitted back to master)
			 */
			/* set state */
			_i2c_state = I2C_SLAVE_TX;
			/* request data from application */
			if (i2c_slave_transmit)
				_i2c_send_data_length = i2c_slave_transmit(I2C_SEND_DATA_BUFFER_SIZE, _i2c_send_data);
			/* reset data index */
			_i2c_send_data_index = 0;
			/* fall-through to transmit first data byte */
		case TW_ST_DATA_ACK:	/* 0xB8: data byte has been
					 * transmitted, ACK has been received */
			/* transmit data byte */
			outb(TWDR, _i2c_send_data[_i2c_send_data_index++]);
			if (_i2c_send_data_index < _i2c_send_data_length)
				/* expect ACK to data byte */
				outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
			else
				/* expect NACK to data byte */
				outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT));
			break;
		case TW_ST_DATA_NACK:	/* 0xC0: data byte has been
					 * transmitted, NACK has been
					 * received */
		case TW_ST_LAST_DATA:	/* 0xC8: */
			/* all done */
			/* switch to open slave */
			outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWEA));
			/* set state */
			_i2c_state = I2C_IDLE;
			break;

			/* Misc */
		case TW_NO_INFO:	/* 0xF8: No relevant state
					 * information */
			/* do nothing */
			break;
		case TW_BUS_ERROR:	/* 0x00: Bus error due to illegal
					 * start or stop condition */
			/* reset internal hardware and release bus */
			outb(TWCR, (inb(TWCR) & TWCR_CMD_MASK) | BV(TWINT) | BV(TWSTO) | BV(TWEA));
			/* set state */
			_i2c_state = I2C_IDLE;
			break;
	}
}

e_i2c_state_type
i2c_get_state(void)
{
	return _i2c_state;
}
