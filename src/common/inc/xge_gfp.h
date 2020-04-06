/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file xge_gfp.h
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 5, 2015
 * @brief Contains prototypes for functions implemented in xge_gfp.c as well as registers and bit definitions.
 *
 */
#ifndef _XGE_GFP_H_
#define _XGE_GFP_H_

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

/**Bit definitions for XGE_GFP_CFG_2_BITS.
 *
 */
struct XGE_GFP_CFG_2_BITS{
	volatile unsigned int  :6;
	volatile unsigned int  ERR_FLAG_EN:1; // bit 6
	volatile unsigned int  PASS_ERR_EN:1; // bit 7
	volatile unsigned int  :24;
};
/**Allows register access for XGE_GFP_CFG_2_REG.
 * Located at \b 0x08 from XGE GFP framer base.
 */
union XGE_GFP_CFG_2_REG{
	alt_u32 all;					//!<used to write or read from the whole register
	struct XGE_GFP_CFG_2_BITS bits;	//!<used for bit access
};

/**Bit definitions for XGE_GFP_STS_BITS.
 *
 */
struct XGE_GFP_STS_BITS{
	volatile unsigned int  	LOF:1;	//b'0 RO LOF
	volatile unsigned int  	RESERVED_1:24;
};
/**Allows register access for XGE_GFP_STS_REG.
 * Located at \b 0x0C from XGE GFP framer base.
 */
union XGE_GFP_STS_REG{
	alt_u32 all;					//!<used to write or read from the whole register
	struct XGE_GFP_STS_BITS bits;	//!<used for bit access
};

/**XGE GFP registers
 *
 */
typedef struct{
	volatile union XGE_GFP_CFG_2_REG* config_reg_2;		// 0x08 from GFP
	volatile union XGE_GFP_STS_REG* status_reg;		// 0x0C from GFP
}xge_gfp_registers_t;

typedef struct{
	xge_gfp_registers_t regs;
}xge_gfp_t;

/*
 * global prototypes
 */
void map_xge_gfp_regs(xge_gfp_t* gfp, alt_u32 gfp_base);
void init_xge_gfp(xge_gfp_t* gfp);
#endif	// _XGE_GFP_H_
