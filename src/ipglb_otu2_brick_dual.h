/*
 * ipglb_otu2_brick_dual.h
 *
 *  Created on: Jan 19, 2018
 *      Author: Closers_2
 */

#ifndef IPGLB_OTU2_BRICK_DUAL_H_
#define IPGLB_OTU2_BRICK_DUAL_H_

#include "ipglb_otu2_framer.h"
#include "ipg_aps.h"

#define OTU2_BRICK_BASE_LINE_1  ODUK_MPIF_BRICK_0_BASE + 0x40000
#define OTU2_BRICK_BASE_LINE_2  0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x40000

/**ODU2 wrapper registers.
 *
 */

//ODUT2 brick register
/*
 * 1- Line status register
 */
struct OTU2_BRICK_LINE_GEN_STATUS_BITS
{
	 volatile unsigned int  LI1_LOS:1;  		// bit 0
	 volatile unsigned int  :1;                 // bit 1
	 volatile unsigned int  LI1_XFP_NOT_RDY:1;	// bit 2
	 volatile unsigned int  LI1_XFP_ABSENT:1; 	// bit 3
	 volatile unsigned int  :12;    			// bit 4 to bit 15
	 volatile unsigned int  LI2_LOS:1;  		// bit 16
	 volatile unsigned int  :1;                 // bit 17
	 volatile unsigned int  LI2_XFP_NOT_RDY:1;	// bit 18
	 volatile unsigned int  LI2_XFP_ABSENT:1; 	// bit 19
	 volatile unsigned int  :12;     			// bit 20 to bit 31
};
union OTU2_BRICK_LINE_GEN_STATUS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct OTU2_BRICK_LINE_GEN_STATUS_BITS bits;
};

/*
 * 2- Reset control register
 */
struct OTU2_BRICK_RESET_CTRL_BITS
{
 volatile unsigned int LI1_RESET:1; 		// bit 0
 volatile unsigned int LI2_RESET:1; 		// bit 1
 volatile unsigned int :4;					// bit 2 to 5
 volatile unsigned int LED_RESET:1; 		// bit 6
 volatile unsigned int APS_RESET:1; 		// bit 7
 volatile unsigned int LOS_PERIOD:6;  		// bit 8 to 13
 volatile unsigned int  :2;     			// bit 14 to bit 15
 volatile unsigned int INSERT_TIME_OUT:16; 	// bit 16 to bit 31
};
union OTU2_BRICK_RESET_CTRL_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct OTU2_BRICK_RESET_CTRL_BITS bits;
};

/*
 * 3- Line general control register
 */
struct OTU2_BRICK_LINE_GENERAL_CTRL_BITS
{
 volatile unsigned int  LI1_TX_DIS:1;  		// bit 0
 volatile unsigned int  :1;     			// bit 1
 volatile unsigned int  LI1_MOD_DES:1;  	// bit 2
 volatile unsigned int  :1;     			// bit 3
 volatile unsigned int  LI1_RESET_GXB:1;  	// bit 4
 volatile unsigned int  :1;     			// bit 5
 volatile unsigned int  LI1_LOCK_TO_REF:1;	// bit 6
 volatile unsigned int  :5;     			// bit 7 to bit 11
 volatile unsigned int  LI1_LINE_LOOPBACK:1;// bit 12
 volatile unsigned int  :3;     			// bit 13 to bit 15
 volatile unsigned int  LI2_TX_DIS:1;  		// bit 16
 volatile unsigned int  :1;     			// bit 17
 volatile unsigned int  LI2_MOD_DES:1;  	// bit 18
 volatile unsigned int  :1;     			// bit 19
 volatile unsigned int  LI2_RESET_GXB:1;  	// bit 20
 volatile unsigned int  :1;     			// bit 21
 volatile unsigned int  LI2_LOCK_TO_REF:1;	// bit 22
 volatile unsigned int  :5;     			// bit 23 to bit 27
 volatile unsigned int  LI2_LINE_LOOPBACK:1;// bit 28
 volatile unsigned int  :3;     			// bit 29 to bit 31
};
union OTU2_BRICK_LINE_GENERAL_CTRL_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct OTU2_BRICK_LINE_GENERAL_CTRL_BITS bits;
};


typedef struct{
	volatile union OTU2_BRICK_LINE_GEN_STATUS_REG* otu2_brick_li_gen_status_reg;// 0x00 from  from  0x1040000
	volatile union OTU2_BRICK_RESET_CTRL_REG* otu2_brick_reset_ctrl_reg;  		// 0x04 from  from  0x1040000
	volatile union OTU2_BRICK_LINE_GENERAL_CTRL_REG* otu2_brick_li_gen_ctrl_reg; // 0x08 from  from  0x1040000
}otu2_brick_dual_registers_t;


typedef struct{
	otu2_brick_dual_registers_t top_level;
	otu2_framer_t framer1;
	otu2_framer_t framer2;
	aps_module_t  aps;
}otu2_brick_dual_t;


void map_otu2_brick_dual_regs(otu2_brick_dual_t* brick, alt_u32 base);
#endif /* IPGLB_OTU2_BRICK_DUAL_H_ */
