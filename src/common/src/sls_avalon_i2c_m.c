/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2005 System Level Solutions India Pvt. Ltd.                   *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the Gujarat *
* state and by the laws of the India.                                         *
*                                                                             *
******************************************************************************/


#include <fcntl.h>
#include <unistd.h>

#include "sys/alt_dev.h"
#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"
#include "io.h"

#include "../inc/i2c_dev.h"
#include "../inc/sls_avalon_i2c_m.h"
#include "../inc/sls_avalon_i2c_m_regs.h"

void checKTip(unsigned int);
int checKAck(unsigned int);


static int          TransDelay;           /**< Transfer In Progress poll delay tuned to transfer speed mode. */
static unsigned int CLOCK_SOURCE;   /**< I2C clock setting used for clock division. */

#define NACK	1
#define ACK	0
#define FAIL	0
#define OK	1
int sls_avalon_i2c_init(void *next,int speed,unsigned int SysClock)
{
    unsigned int        BASE;
	sls_avalon_i2c_dev* dev;

	dev = (sls_avalon_i2c_dev*)next;

	BASE = dev->base_addr;
    CLOCK_SOURCE = SysClock;

    /* Set the default value of all registers as per reset */
    IOWR_8DIRECT(BASE, PRE_REG_LOW, 0x00);
    IOWR_8DIRECT(BASE, PRE_REG_HIGH,0x00);
    IOWR_8DIRECT(BASE, CTRL_REG,    0x00);
    IOWR_8DIRECT(BASE, TX_REG,      0x00);
    IOWR_8DIRECT(BASE, CMD_REG,     0x00);

    if (speed ==0 )
    {
		if (0 > sls_avalon_i2c_ioctl(next, 0, NULL))
		{
			return 			(-1);
        }
	}
	else if (speed==1)
	{
        if (0 > sls_avalon_i2c_ioctl(next, 1, NULL))
        {
            return 			(-1);
        }
	}
	else {
		return 		(-1);
	}

    /* Enable the IP Core */
    IOWR_8DIRECT(BASE,CTRL_REG, 0x80);

    //usleep(2000);

    return 	(0);
}


int sls_avalon_i2c_ioctl(void *next,int req, void *arg)
{
    sls_avalon_i2c_dev* dev;
    unsigned int        BASE;
    unsigned int        divisor;
    unsigned char       controlRegValue;

    dev = (sls_avalon_i2c_dev*)next;

    BASE = dev->base_addr;

    // Configure the clock divisor for the requested speed mode.
    switch (req)
    {

        case 0:
            divisor = (CLOCK_SOURCE/500000) - 1;
            TransDelay = 200;
            break;

        case 1:
            divisor = (CLOCK_SOURCE/(5*400000)) - 1;
            TransDelay = 20;
            break;

        default:
            return 			(-1);
            break;
    }

    /* Disable the IP Core when writing to prescale registers*/
    controlRegValue = IORD_8DIRECT(BASE, CTRL_REG);
    IOWR_8DIRECT(BASE, CTRL_REG, 0x00);

    // Program clock divisor.
    IOWR_8DIRECT(BASE, PRE_REG_LOW,  (unsigned char)(divisor & 0xFF));
    IOWR_8DIRECT(BASE, PRE_REG_HIGH, (unsigned char)((divisor >> 8) & 0xFF));

    /* Enable the IP Core */
    controlRegValue = IORD_8DIRECT(BASE, CTRL_REG);
    IOWR_8DIRECT(BASE,CTRL_REG, 0x80);

    return 	(1);
}


int sls_avalon_i2c_write(void* next,alt_u8 slave_addr,const char *buff,alt_u8 offset,int len)
  {
	  unsigned int BASE;
	  alt_u8 nack = 0 ;
	  sls_avalon_i2c_dev* dev;
	  dev=(sls_avalon_i2c_dev*)next;

	  BASE=dev->base_addr;

	  /* Transmit Slave address and enable Write mode */

      IOWR_8DIRECT(BASE,TX_REG,slave_addr);
      IOWR_8DIRECT(BASE,CMD_REG,0x90);
      checKTip(BASE);
      nack = checKAck(BASE);
	  if(nack == 0) // ack was received
	  {
		  /* Trandmit the word Address of the Slave device */

		  IOWR_8DIRECT(BASE,TX_REG,offset);
		  IOWR_8DIRECT(BASE,CMD_REG,0x10);
		  checKTip(BASE);
		  nack = checKAck(BASE);
		  if(nack == 0) // ack was received
		  {
				while(len>0)
				{
					/* Transmit the data you want to send */
					IOWR_8DIRECT(BASE,TX_REG,*buff);
					IOWR_8DIRECT(BASE,CMD_REG,0x10);
					checKTip(BASE);
					nack = checKAck(BASE);
					if(nack == 1) // nack received
					{
						IOWR_8DIRECT(BASE,CMD_REG,0x40);
						return 						(FAIL);
					}

					len--;
					buff++;

				}
		  }
	  }
	  /* Stop the Device in write mode */
	  IOWR_8DIRECT(BASE,CMD_REG,0x40);
	  return 	(OK); // if nack is 1, exit with a failure (0)
}


int sls_avalon_i2c_read(void* next,alt_u8 slave_addr,alt_u8 *buff,alt_u8 offset,int len,int type)
 {
     alt_u8 read_cmd, write_cmd;

     unsigned int BASE;
     alt_u8 ack_received = 0 ;

	 sls_avalon_i2c_dev* dev;
	 dev=(sls_avalon_i2c_dev*)next;

	 BASE=dev->base_addr;

     read_cmd=(slave_addr)|0x01;
     write_cmd = (slave_addr) | 0x00;

     /* Check wether Random read or Current read */

	if(type==0)
	{
		/* Read the data from the Current address offset */
		/* 	the difference with the other type of I2C read is that we don't have to write the offset where we want to read from
		 	it sends data from where its internal address pointer is*/
		while(len>0)
		{
			/* Transmit the Slave Address in Read Mode */
			IOWR_8DIRECT(BASE,TX_REG,read_cmd);
			IOWR_8DIRECT(BASE,CMD_REG,0x90);
			checKTip(BASE);
			ack_received = checKAck(BASE);
			if(ack_received == NACK)
			{
				break;
			}

			/*Receive the Data from your specified location */
			IOWR_8DIRECT(BASE,CMD_REG,0x28);
			checKTip(BASE);
			ack_received = checKAck(BASE);
			if(ack_received == NACK)
			{
				break;
			}
			*buff=IORD_8DIRECT(BASE,RX_REG);
			len--;
			buff++;
		}
		if(ack_received == NACK) // ack was not received
		{
			/* Stop the Device in receive mode */
			IOWR_8DIRECT(BASE,CMD_REG,0x40);
			return 			(FAIL);
		}
		else
		{
			/* Stop the Device in receive mode */
			IOWR_8DIRECT(BASE,CMD_REG,0x40);
		}
	}
	else if(type == 1)
	{
		/* Perform the Random read operation */
		// in this type we tell the slave which offset we want to read read from
		while(len>0)
		{
			// step 1 - write the offset to read from to slave
			//

			// step 1.a - transmit the slave address in write mode
			IOWR_8DIRECT(BASE,TX_REG,write_cmd);
			IOWR_8DIRECT(BASE,CMD_REG,0x90);
			//IOWR_8DIRECT(BASE,CMD_REG,0x40);
			checKTip(BASE);
			ack_received = checKAck(BASE);
			if(ack_received == NACK) // ack was not received
			{
				break;
			}
			// step 1.b - Transmit the offset of the Slave device from which we need to read
			IOWR_8DIRECT(BASE,TX_REG,offset);
			IOWR_8DIRECT(BASE,CMD_REG,0x10);
			checKTip(BASE);
			ack_received = checKAck(BASE);
			if(ack_received == NACK) // ack was not received
			{
				break;
			}

			// step 2 - read the data that we were asking for
			//

			// step 2.a - transmit the slave address in read Mode */
			IOWR_8DIRECT(BASE,TX_REG,read_cmd);
			IOWR_8DIRECT(BASE,CMD_REG,0x90);
			checKTip(BASE);
			ack_received = checKAck(BASE);
			if(ack_received == NACK) // ack was not received
			{
				break;
			}
			// step 2.b - read the received data
			IOWR_8DIRECT(BASE,CMD_REG,0x28);
			checKTip(BASE);
			*buff=IORD_8DIRECT(BASE,RX_REG);
			buff++;
			len--;
			offset++;
		}
		if(ack_received == NACK)
		{
			/* Stop the Device in receive mode */
			IOWR_8DIRECT(BASE,CMD_REG,0x40);
			return 			(FAIL);
		}
		else
		{
			/* Stop the Device in receive mode */
			IOWR_8DIRECT(BASE,CMD_REG,0x40);
		}
	}
	else if(type == 2)
	{
		/* in this mode, the host tells the slave where to read from and then keeps on reading data as long as needed*/
		// step 1 - write the offset to read from to slave
		//

		// step 1.a - transmit the slave address in write mode
		IOWR_8DIRECT(BASE,TX_REG,write_cmd);
		IOWR_8DIRECT(BASE,CMD_REG,0x90);

		checKTip(BASE);
		ack_received = checKAck(BASE);
		if(ack_received == NACK) // ack was not received
		{
		   return 		(FAIL);//break;
		}
		// step 1.b - Transmit the offset of the Slave device from which we need to read
		IOWR_8DIRECT(BASE,TX_REG,offset);
		IOWR_8DIRECT(BASE,CMD_REG,0x10);
		checKTip(BASE);
		ack_received = checKAck(BASE);
		if(ack_received == NACK) // ack was not received
		{
			return 			(FAIL);// break;
		}

		/* Transmit the Slave Address in Read Mode */

		IOWR_8DIRECT(BASE,TX_REG,read_cmd);
		IOWR_8DIRECT(BASE,CMD_REG,0x90);
		checKTip(BASE);
		ack_received = checKAck(BASE);
		if(ack_received == NACK) // ack was not received
		{
			return 			(FAIL);//break;
		}

		while(len>0)
		{
			len--;
			if(len > 0)
			{
				IOWR_8DIRECT(BASE,CMD_REG,0x20);// read and send ACK

			}
			else	// last byte to read we should NACK the slave
			{
				IOWR_8DIRECT(BASE,CMD_REG,0x28);// read and send NACK
			}
			checKTip(BASE);
			*buff=IORD_8DIRECT(BASE,RX_REG);
			buff++;
		}
		/* Stop the Device in receive mode */
		IOWR_8DIRECT(BASE,CMD_REG,0x40);
	}
	return 	(OK);
 }


/* This Routine is used to check whether the byte has been transferred or not and when byte has transferred this loop terminates*/

void checKTip(unsigned int BASE)
{
	alt_u8 trans;

    trans = IORD_8DIRECT(BASE,STATUS_REG)& 0x2;
    while(trans == 0x2)
    {
		usleep(TransDelay);
		trans = IORD_8DIRECT(BASE,STATUS_REG) && 0x2;
        if(trans == 0)
        {
			break;
		}
	}
}


/* This Routine is used to check the Acknowledge from the Slave if acknowledged the loop terminates */

int checKAck(unsigned int BASE)
{

	alt_u8 ack;
	ack = IORD_8DIRECT(BASE,STATUS_REG)& 0X80;
	return (ack == 0x80);
}

/*
 * Function that reads 16 bits value: MSB (8 bits) then LSB (8 bits)
 * In this project it is used for the temperature sensor MCP9808
 */
int sls_avalon_i2c_read_16_bit(void* next, alt_u8 slave_addr, alt_u8 *buff, alt_u8 offset)
 {
     alt_u8 read_addr;

     unsigned int BASE;
     alt_u8 ack_received = 0 ;

	 sls_avalon_i2c_dev* dev;
	 dev=(sls_avalon_i2c_dev*)next;

	 BASE=dev->base_addr;

     read_addr=(slave_addr)|0x01;


	 /* Transmit Slave address and enable Receive mode */
	 IOWR_8DIRECT(BASE,TX_REG,slave_addr);
	 IOWR_8DIRECT(BASE,CMD_REG,0x90);
	 checKTip(BASE);
	 ack_received = checKAck(BASE);
	 if(ack_received == 1) // ack was not received
	 {
		 return 		(0);
	 }

	 /* Transmit the word Address of the Slave device */
	 IOWR_8DIRECT(BASE,TX_REG,offset);
	 IOWR_8DIRECT(BASE,CMD_REG,0x10);
	 checKTip(BASE);
	 ack_received = checKAck(BASE);
	 if(ack_received == 1) // ack was not received
	 {
		   return 		(0);
	 }

	 /* Transmit the Slave Address in Read Mode */
	 IOWR_8DIRECT(BASE,TX_REG,read_addr);
	 IOWR_8DIRECT(BASE,CMD_REG,0x90);
	 checKTip(BASE);
	 ack_received = checKAck(BASE);
	 if(ack_received == 1) // ack was not received
	 {
		 return 		(0);
	 }


	 /*Receive the Data from your specified location */
	 //First receive the MSB and send an ACK
	 IOWR_8DIRECT(BASE,CMD_REG,0x20); //With ACK
	 checKTip(BASE);
	 *buff=IORD_8DIRECT(BASE,RX_REG);
	 buff++;

	 //Secondly receive the LSB and send a NACK
	 IOWR_8DIRECT(BASE,CMD_REG,0x28); //With NACK
	 checKTip(BASE);
	 *buff=IORD_8DIRECT(BASE,RX_REG);

	 if(ack_received == 1) // ack was not received
	 {
		 /* Stop the Device in receive mode */
		  IOWR_8DIRECT(BASE,CMD_REG,0x40);
		  return 		(0);
	 }
	 else
	 {
		 /* Stop the Device in receive mode */
		 IOWR_8DIRECT(BASE,CMD_REG,0x40);
	 }
	 return 	(1);
 }
