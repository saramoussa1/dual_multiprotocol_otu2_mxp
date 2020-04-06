/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file xcvr_reconfig.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 5, 2015
 * @brief Contains procedures to access, monitor and control XCVR blocks.
 *
 * Other high level functions are also implemented such as enabling/disabling loopbacks, POSTTAP and PRETAP setters and getters.
 * @version 0.2
 *
 * @note 0.2 added functions for MIF streaming
 */

#ifndef XCVER_RECONFIG_H_
#define XCVER_RECONFIG_H_

#include "ipg_types.h"
#define PRE_CDR 	0x20
#define POST_CDR	0x21

/**Allows register access for XCVR_RECONFIG_LOGICAL_CHANNEL_REG.
 * Located at \b 0x100 from an XCVR sub-block.
 */
union XCVR_RECONFIG_LOGICAL_CHANNEL_REG{
	alt_u32 all;		//!<used to write or read from the whole register
};

/**Bit definitions for XCVR_RECONFIG_CTRL_STATUS_BIT.
 *
 */
struct XCVR_RECONFIG_CTRL_STATUS_BIT{
	volatile unsigned int  	WRITE:1;		//!<b'0 RW, when set initiates a write
	volatile unsigned int  	READ:1;			//!<b'1 RW, when set initiates a read
	volatile unsigned int	RESERVED_1:6;	//!<b'7:2 reserved for future use
	volatile unsigned int  	BUSY:1;			//!<b'8 RO, when set to 1, indicates the block is busy
	volatile unsigned int  	ERROR:1;		//!<b'1 RO, when set to 1, initiates the last operation failed
	volatile unsigned int  	RESERVED_2:22;	//!<b'31:10 reserved for future use
};
/**Allows register access for XCVR_RECONFIG_CTRL_STATUS_REG.
 * Located at \b 0x08 from an XCVR sub-block.
 */
union XCVR_RECONFIG_CTRL_STATUS_REG{
	alt_u32 all;								//!<used to write or read from the whole register
	struct XCVR_RECONFIG_CTRL_STATUS_BIT bits;	//!<Used for bit access
};

/**Allows register access for XCVR_RECONFIG_OFFSET_REG.
 * Located at \b 0x0C from an XCVR sub-block.
 */
union XCVR_RECONFIG_OFFSET_REG{
	alt_u32 all;								//!< used to write or read from the whole register
};

/**Allows register access for XCVR_RECONFIG_DATA_REG.
 * Located at \b 0x10 from an XCVR sub-block.
 */
union XCVR_RECONFIG_DATA_REG{
	alt_u32 all;								//!<used to write or read from the whole register
};

/**XCVR sub-blocks common registers
 *
 */
typedef struct{
	volatile union XCVR_RECONFIG_LOGICAL_CHANNEL_REG* logical_channel;	//!<offset 0x00 from sub-block base
	volatile union XCVR_RECONFIG_CTRL_STATUS_REG* ctrl_sts;				//!<offset 0x08 from sub-block base
	volatile union XCVR_RECONFIG_OFFSET_REG* offset;					//!<offset 0x0C from sub-block base
	volatile union XCVR_RECONFIG_DATA_REG* data;						//!<offset 0x10 from sub-block base
}xcvr_common_regs_t;

/**PMA Analog Control sub-blocks registers.
 *
 */
typedef struct{
	xcvr_common_regs_t pma_analog_control;							// offset 0x20 from the XCVR RECONFIG BLOCK base
}transceiver_block_t;

/**PLL reconfig sub-blocks registers.
 *
 */
typedef struct{
	xcvr_common_regs_t atx_tuning;									// offset 0xC0 from the XCVR RECONFIG BLOCK base
}pll_reconfig_block_t;

/**MIF streamer sub-blocks registers.
 *
 */
typedef struct{
	xcvr_common_regs_t streamer;
}streamer_module_block_t;// offset 0xE0 from the XCVR RECONFIG BLOCK base

/**XCVR reconfig block sub-blocks registers.
 *
 */
typedef struct{
	transceiver_block_t 	transceiver_block;
	pll_reconfig_block_t 	pll_reconfig_block;
	streamer_module_block_t 	streamer_block;
}xcvr_reconfig_regs_t;

ret_code_t xcvr_disable_loopback(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, int loopback_mode);
ret_code_t xcvr_enable_loopback(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, int loopback_mode);
ret_code_t xcvr_atxpll_tuning(xcvr_reconfig_regs_t* xvr_block, int gxb_index);
ret_code_t xcvr_atxpll_tuning(xcvr_reconfig_regs_t* _xvr_block, int gxb_index);
ret_code_t xcvr_set_vod(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_u8 vod);
ret_code_t xcvr_get_vod(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_u8* vod);
ret_code_t xcvr_set_pretap(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_8 pretap);
ret_code_t xcvr_get_pretap(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_8* pretap);
ret_code_t xcvr_set_posttap_1(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_u8 posttap);
ret_code_t xcvr_get_posttap_1(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_u8* posttap);
ret_code_t xcvr_set_posttap_2(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_8 posttap);
ret_code_t xcvr_get_posttap_2(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_8* posttap);
void map_xcvr_reconfig(xcvr_reconfig_regs_t* _xvr_block, alt_u32 base);
ret_code_t xcvr_mif_xcvr_reconfiguration(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_u32 mif_location);
ret_code_t xcvr_mif_atxpll_reconfiguration(xcvr_reconfig_regs_t* _xvr_block, int gxb_index, alt_u32 mif_location);
#endif /* XCVER_RECONFIG_H_ */
