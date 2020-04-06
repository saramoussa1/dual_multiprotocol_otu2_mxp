/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_gfp.h
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @date March 28, 2017
 * @brief Contains prototypes for functions implemented in ipg_gfp.c as well as registers and bit definitions.
 *
 */
#ifndef _IPG_GFP_H_
#define _IPG_GFP_H_

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

/**Bit definitions for GFP_STATUS_BITS.
 *
 */
struct GFP_STATUS_BITS{
	volatile unsigned int  RES_1:12;
	volatile unsigned int  OUT_FIFO_OF_ERR:1; // bit 12
	volatile unsigned int  RES_2:19;		  // b31:13
};
/**Allows register access for GFP_STATUS_REG.
 * Located at \b 0x0C from GFP framer base.
 */
union GFP_STATUS_REG{
	alt_u32 all;					//!<used to write or read from the whole register
	struct GFP_STATUS_BITS bits;	//!<used for bit access
};

/**Bit definitions for GFP_CFG_2_BITS.
 *
 */
struct GFP_CFG_2_BITS{
	volatile unsigned int  :6;
	volatile unsigned int  ERR_FLAG_EN:1; // bit 6
	volatile unsigned int  PASS_ERR_EN:1; // bit 7
	volatile unsigned int  :7;
	volatile unsigned int  SW_RST:1; 	  // bit 15
	volatile unsigned int  :16;			  // b31:16
};
/**Allows register access for GFP_CFG_2_REG.
 * Located at \b 0x08 from GFP framer base.
 */
union GFP_CFG_2_REG{
	alt_u32 all;					//!<used to write or read from the whole register
	struct GFP_CFG_2_BITS bits;	//!<used for bit access
};

/**Allows register access for GFP_STATION_ADDRESS_REG.
 * Located at \b 0x00 from GFP framer base.
 */
union GFP_STATION_ADDRESS_REG{
	alt_u32 all;					//!<used to write or read from the whole register
};

/**Allows register access for GFP_CFG_1_REG.
 * Located at \b 0x04 from GFP framer base.
 */
union GFP_CFG_1_REG{
	alt_u32 all;					//!<used to write or read from the whole register
};

/**GFP registers
 *
 */
typedef struct{
	volatile union GFP_STATION_ADDRESS_REG* station_reg;		// 0x00 from GFP
	volatile union GFP_CFG_1_REG* config_reg_1;					// 0x04 from GFP
	volatile union GFP_CFG_2_REG* config_reg_2;					// 0x08 from GFP
	volatile union GFP_STATUS_REG* status_reg;					// 0x0C from GFP
}gfp_registers_t;

typedef struct{
	alt_u32 base_address;
	gfp_registers_t regs;
}gfp_t;

/*
 * global prototypes
 */
void map_gfp_regs(gfp_t* gfp, alt_u32 gfp_base);
void init_gfp(gfp_t* gfp);
gfp_registers_t* create_gfp_registers(alt_u32 base);
#endif	// _IPG_GFP_H_
