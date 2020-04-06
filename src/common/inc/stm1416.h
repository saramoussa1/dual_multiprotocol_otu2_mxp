/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file xge_mac.h
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 5, 2015
 * @brief Contains prototypes of functions defined in xge_mac.c in addition to registers and bit defintions.
 *
 * Here goes a more detailed description.
 */
#ifndef _STM1416_H_
#define _STM1416_H_

#include <system.h>
#include <stdlib.h>
#include <sys/alt_stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "io.h"
#include "sys/alt_irq.h"
#include <sys/alt_alarm.h>
#include <stdio.h>
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"

/**Bit definitions for STM_1416_LIVE_STS_BITS.
 *
 */
struct STM_1416_LIVE_STS_BITS
{
	 volatile unsigned int  SYS_RX_LOF:1;    			 	// bit 0
	 volatile unsigned int  SYS_RX_SEF:1;  					// bit 1
	 volatile unsigned int  SYS_RX_LAIS:1;     				// bit 2
	 volatile unsigned int  :4;     						// bit 3 to bit 6
	 volatile unsigned int  SYS_TX_FIFO_ERR:1;     			// bit 7
	 volatile unsigned int  :4;     						// bit 8 to bit 11
	 volatile unsigned int  SYS_RX_ALG_ERR:1;     			// bit 12
	 volatile unsigned int  :3;     						// bit 13 to bit 15
	 volatile unsigned int  LINE_RX_LOF:1;    			 	// bit 16
	 volatile unsigned int  LINE_RX_SEF:1;  				// bit 17
	 volatile unsigned int  LINE_RX_LAIS:1;     			// bit 18
	 volatile unsigned int  LINE_RX_LRDI:1;   				// bit 19
	 volatile unsigned int  :3;     						// bit 20 to bit 22
	 volatile unsigned int  LINE_TX_FIFO_ERR:1;   			// bit 23
	 volatile unsigned int  :4;     						// bit 24 to bit 27
	 volatile unsigned int  LINE_RX_ALG_ERR:1;     			// bit 28
	 volatile unsigned int  :3;     						// bit 29 to bit 31
};

union STM_1416_LIVE_STS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct STM_1416_LIVE_STS_BITS bits;
};

/*
 * 2- STM1/4 interrupt status register
 */
struct STM_1416_INT_STS_BITS
{
	 volatile unsigned int  SYS_RX_LOF:1;    			 	// bit 0
	 volatile unsigned int  SYS_RX_SEF:1;  					// bit 1
	 volatile unsigned int  SYS_RX_LAIS:1;     				// bit 2
	 volatile unsigned int  :4;     						// bit 3 to bit 6
	 volatile unsigned int  SYS_TX_FIFO_ERR:1;     			// bit 7
	 volatile unsigned int  :4;     						// bit 8 to bit 11
	 volatile unsigned int  SYS_RX_ALG_ERR:1;     			// bit 12
	 volatile unsigned int  :3;     						// bit 13 to bit 15
	 volatile unsigned int  LINE_RX_LOF:1;    			 	// bit 16
	 volatile unsigned int  LINE_RX_SEF:1;  				// bit 17
	 volatile unsigned int  LINE_RX_LAIS:1;     			// bit 18
	 volatile unsigned int  LINE_RX_LRDI:1;   				// bit 19
	 volatile unsigned int  :3;     						// bit 20 to bit 22
	 volatile unsigned int  LINE_TX_FIFO_ERR:1;   			// bit 23
	 volatile unsigned int  :4;     						// bit 24 to bit 27
	 volatile unsigned int  LINE_RX_ALG_ERR:1;     			// bit 28
	 volatile unsigned int  :3;     						// bit 29 to bit 31
};
union STM_1416_INT_STS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct STM_1416_INT_STS_BITS bits;
};

/*
 * 3- Config register
 */
struct STM_1416_CONFIG_BITS
{
	volatile unsigned int  SYS_RX_SCRAM_EN:1;    // bit 0
	volatile unsigned int  SYS_TX_SCRAM_EN:1;    // bit 1
	volatile unsigned int  :6;    				 // bit 2 to bit 7
	volatile unsigned int  APS_TX_TRANSPARENT:1; // bit 8
	volatile unsigned int  :3;    				 // bit 9 to bit 11
	volatile unsigned int  APS_RX_TRANSPARENT:1; // bit 12
	volatile unsigned int  :19;					 // bit 13 to bit 31
};
union STM_1416_CONFIG_REG
{
	alt_u32 all;
	struct STM_1416_CONFIG_BITS bits;
};

/*
 * 4- TX LRDI register
 */
struct STM_1416_TX_LRDI_BITS
{
	volatile unsigned int SYS_TX_LRDI:3;    	// bit 0 to bit 2
	volatile unsigned int  :13;					// bit 3 to bit 15
	volatile unsigned int LINE_TX_LRDI:3; 		// bit 16 to bit 18
	volatile unsigned int  :13;					// bit 19 to bit 31
};
union STM_1416_TX_LRDI_REG
{
	alt_u32 all;
	struct STM_1416_TX_LRDI_BITS bits;
};

/*
 * 5- STM1/4 B1 errors register
 */
struct STM_1416_B1_ERRORS_BITS
{
 volatile unsigned int  SYS_B1_ERRORS:16;    			 	// bit 0 to bit 15
 volatile unsigned int  LINE_B1_ERRORS:16;   				// bit 16 to bit 31
};
union STM_1416_B1_ERRORS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct STM_1416_B1_ERRORS_BITS bits;
};

/*
 * 6- STM1/4 B1 errors register
 */
struct STM_1416_B2_ERRORS_BITS
{
 volatile unsigned int  SYS_B2_ERRORS:16;    			 	// bit 0 to bit 15
 volatile unsigned int  LINE_B2_ERRORS:16;   				// bit 16 to bit 31
};
union STM_1416_B2_ERRORS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct STM_1416_B2_ERRORS_BITS bits;
};

/*
 * 7- STM1/4 REI errors register
 */
struct STM_1416_REI_ERRORS_BITS
{
 volatile unsigned int  SYS_REI_ERRORS:16;    			 	// bit 0 to bit 15
 volatile unsigned int  LINE_REI_ERRORS:16;   				// bit 16 to bit 31
};
union STM_1416_REI_ERRORS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct STM_1416_REI_ERRORS_BITS bits;
};

/*
 * 8- STM1/4 Soft Reset register
 */
struct STM_1416_SOFT_RESET_BITS
{
 volatile unsigned int  :28;    			 				// bit 0 to bit 27
 volatile unsigned int  SOFT_RESET:1;   					// bit 28
 volatile unsigned int  :3;    			 					// bit 29 to bit 31
};
union STM_1416_RESET_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct STM_1416_SOFT_RESET_BITS bits;
};


/**STM1/4/16 registers
 *
 */
typedef struct
{
 volatile union STM_1416_LIVE_STS_REG* 	live_status; 	/* 0x00 from  STM base*/
 volatile union STM_1416_INT_STS_REG* 	int_status;		/* 0x04 from  STM base*/
 volatile union STM_1416_CONFIG_REG* 	config;			/* 0x0C from  STM base*/
 volatile union STM_1416_RESET_REG* 	reset;			/* 0x10 from  STM base*/
 volatile union STM_1416_TX_LRDI_REG* 	tx_lrdi; 		/* 0x1C from  STM base*/
 volatile union STM_1416_B1_ERRORS_REG* b1_err;      	/* 0xC0 from  STM base*/
 volatile union STM_1416_B2_ERRORS_REG* b2_err;			/* 0xC4 from  STM base*/
 volatile union STM_1416_REI_ERRORS_REG* rei_err;		/* 0xC8 from  STM base*/
}stm1416_registers_t;

/*
 * Exported functions.
 */
void map_stm1416_regs(stm1416_registers_t* _stm, alt_u32 stm_base);
stm1416_registers_t* create_stm1416_registers(alt_u32 stm1416_base);

#endif	//_STM1416_H_
