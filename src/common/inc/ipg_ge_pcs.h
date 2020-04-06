/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_ge_pcs.h
 * @author d_chamoun (dchamoun@ipgphotonics.com)
 * @date Jan 23, 2016
 * @brief Here goes a brief description.
 *
 * Here goes a more detailed description.
 */

#ifndef IP_GE_PCS_H_
#define IP_GE_PCS_H_

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

/**Bit definitions for GE_PCS_CTRL_BITS.
 *
 */
struct GE_PCS_CTRL_BITS{
	volatile unsigned int :9;  						//!<b'8:0
	volatile unsigned int  RESTART_AUTO_NEG:1;      //!<b'9
	volatile unsigned int  :2;      				//!<b'11:10
	volatile unsigned int  AUTO_NEG:1;      		//!<b'12
	volatile unsigned int :19; 						//!<b'31:13
};
/**Allows register access for GE_PCS_CTRL_REG.
 * Located at \b 0x00 from GE PCS base.
 */
union GE_PCS_CTRL_REG{
	 alt_u32 all;						//!<used to write or read from the whole register
	 struct GE_PCS_CTRL_BITS bits;	//!<used for bit access
};

/**Bit definitions for GE_PCS_STS_BITS.
 *
 */
struct GE_PCS_STS_BITS{
	volatile unsigned int RESERVED_1:2;  	//!<b'1:0
	volatile unsigned int LINK:1;      		//!<b'2
	volatile unsigned int RESERVED_2:2;     //!<b'4:3
	volatile unsigned int AUTO_NEG_COMP:1;  //!<b'5
	volatile unsigned int RESERVED_3:26;	//!<b'31:6
};

/**Allows register access for GE_PCS_STS_REG.
 * Located at \b 0x004 from GE PCS base.
 */
union GE_PCS_STS_REG{
	alt_u32 all;			//!<used to write or read from the whole register
	struct GE_PCS_STS_BITS bits;	//!<used for bit access
};

/**Bit definitions for GE_PCS_PARTNER_ABILITY_BITS.
 *
 */
struct GE_PCS_PARTNER_ABILITY_BITS{
	volatile unsigned int RESERVED_1:15;  	//!<b'14:0
	volatile unsigned int  LINK:1;      	//!<b'15
	volatile unsigned int RESERVED_2:16;	//!<b'31:16
};

/**Allows register access for GE_PCS_PARTNER_ABILITY_REG.
 * Located at \b 0x014 from GE PCS base.
 */
union GE_PCS_PARTNER_ABILITY_REG{
	alt_u32 all;			//!<used to write or read from the whole register
	struct GE_PCS_PARTNER_ABILITY_BITS bits;	//!<used for bit access
};

/**Allows register access for GE_PCS_AUTO_NEG_LINK_TIMER_REG1.
 * Located at \b 0x048 from GE PCS base.
 */
union GE_PCS_AUTO_NEG_LINK_TIMER_REG1{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_AUTO_NEG_LINK_TIMER_REG2.
 * Located at \b 0x04C from GE MAC base.
 */
union GE_PCS_AUTO_NEG_LINK_TIMER_REG2{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_IF_MODE_REG.
 * Located at \b 0x050 from GE MAC base.
 */
union GE_PCS_IF_MODE_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_PCS_DEV_ABILITY_REG.
 * Located at \b 0x010 from GE MAC base.
 */
union GE_PCS_DEV_ABILITY_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

typedef struct{
	alt_u32 base_address;
	volatile union GE_PCS_CTRL_REG* ctrl_reg; 							// 0x00 from PCS base
	volatile union GE_PCS_STS_REG* sts_reg; 							// 0x04 from PCS base
	volatile union GE_PCS_PARTNER_ABILITY_REG* partner_ability_reg;		// 0x14 from PCS base
	volatile union GE_PCS_AUTO_NEG_LINK_TIMER_REG1* link_timer1_reg; 	// 0x48 from PCS base
	volatile union GE_PCS_AUTO_NEG_LINK_TIMER_REG2* link_timer2_reg; 	// 0x4C from PCS base
	volatile union GE_PCS_IF_MODE_REG* if_mode_reg; 					// 0x50 from PCS base
	volatile union GE_PCS_DEV_ABILITY_REG* dev_ability_reg; 			// 0x10 from PCS base
}ge_pcs_registers_t ;

typedef struct{
	ge_pcs_registers_t registers;
	ipg_control_t auto_negotiation;
	ipg_status_t link;
	alt_u8 sfp_type; // we need this information so we know where to read the link status
}ge_pcs_t;

ge_pcs_registers_t* create_gepcs_registers(alt_u32 base);
void map_ge_pcs_regs(ge_pcs_t* _pcs);
void map_ge_pcs_regs_ol(ge_pcs_registers_t* _pcs);
void init_ge_pcs(ge_pcs_t* _pcs);
void init_ge_pcs_ol(ge_pcs_registers_t* _pcs);
void enable_ge_pcs_an(ge_pcs_t* _pcs);
void disable_ge_pcs_an(ge_pcs_t* _pcs);
int get_ge_pcs_link(void* dev);
ipg_bool_t is_ge_pcs_an_on(ge_pcs_t* _pcs);
void init_ge_pcs_ol(ge_pcs_registers_t* _pcs);
#endif /* IPG_GE_PCS_H_ */
