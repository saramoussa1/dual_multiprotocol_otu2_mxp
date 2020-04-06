/*
 * ipglb_otu2_brick_dual.c
 *
 *  Created on: Jan 19, 2018
 *      Author: Closers_2
 */


#include "ipglb_otu2_brick_dual.h"

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif


void map_otu2_brick_dual_regs(otu2_brick_dual_t* brick, alt_u32 base){
	// this brick's top level register mapping
	brick->top_level.otu2_brick_li_gen_status_reg = (volatile union OTU2_BRICK_LINE_GEN_STATUS_REG*)((base + 0x00) | DE_CACHE_EN_BIT);
	brick->top_level.otu2_brick_reset_ctrl_reg = (volatile union OTU2_BRICK_RESET_CTRL_REG*)((base + 0x04)|DE_CACHE_EN_BIT);
	brick->top_level.otu2_brick_li_gen_ctrl_reg = (volatile union OTU2_BRICK_LINE_GENERAL_CTRL_REG*)((base + 0x08)|DE_CACHE_EN_BIT);
	// this brick's framers registers mapping
	map_otu2_framer_regs(&brick->framer1, base + 0x100);
	map_otu2_framer_regs(&brick->framer2, base + 0x180);
#ifdef aps
	map_aps_regs(&brick->aps, base + 0x00);
#endif
}
