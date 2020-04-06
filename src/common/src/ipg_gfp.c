/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_gfp.c
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @date March 28, 2017
 * @brief This file contains functions to control/monitor a GE GFP IP.
 *
 */
#include "../inc/ipg_gfp.h"

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif

/*
 * local prototypes
 */
static void map_gfp_regs_ol(gfp_registers_t* _gfp, alt_u32 base);

void disable_gfp_err_flag(gfp_t* gfp);
void enable_gfp_pass_error(gfp_t* gfp);


/**
 * This function creates an instance of the GFP registers.
 * @param base
 * @return
 */
gfp_registers_t* create_gfp_registers(alt_u32 base){
	gfp_registers_t* _gfp = malloc(sizeof(gfp_registers_t));
	if(_gfp != NULL){
		map_gfp_regs_ol(_gfp, base);
	}
	return _gfp;
}

/**This function runs the default initialization of a 10GE GFP device.
 * @param[in,out] gfp pointer to the GFP registers
 */
void init_gfp(gfp_t* gfp){
//	HardResetGfp(gfpIndex);
	gfp->regs.station_reg->all = 0x0000E0E0;
	gfp->regs.config_reg_1->all = 0x008132A1;

	// change some default settings
	disable_gfp_err_flag(gfp);
	enable_gfp_pass_error(gfp);
}

/**This function maps the GFP registers to our application variables.
 *
 * @param[in,out] gfp pointer to the application GFP variables
 * @param base base address of the GFP HW in memory
 * @return None
 */
void map_gfp_regs(gfp_t* gfp, alt_u32 base){
	gfp->regs.station_reg = (volatile union GFP_STATION_ADDRESS_REG*)((base + 0x00) | DE_CACHE_EN_BIT);
	gfp->regs.config_reg_1 = (volatile union GFP_CFG_1_REG*)((base + 0x04) | DE_CACHE_EN_BIT);
	gfp->regs.config_reg_2 = (volatile union GFP_CFG_2_REG*)((base + 0x08) | DE_CACHE_EN_BIT);
	gfp->regs.status_reg = (volatile union GFP_STATUS_REG*)((base + 0x0C) | DE_CACHE_EN_BIT);
}

static void map_gfp_regs_ol(gfp_registers_t* _gfp, alt_u32 base){
	alt_u32 gfp_base = base;
	_gfp->station_reg = (volatile union GFP_STATION_ADDRESS_REG*)((gfp_base + 0x00) | DE_CACHE_EN_BIT);
	_gfp->config_reg_1 = (volatile union GFP_CFG_1_REG*)((gfp_base + 0x04) | DE_CACHE_EN_BIT);
	_gfp->config_reg_2 = (volatile union GFP_CFG_2_REG*)((gfp_base + 0x08) | DE_CACHE_EN_BIT);
	_gfp->status_reg = (volatile union GFP_STATUS_REG*)((gfp_base + 0x0C) | DE_CACHE_EN_BIT);
}

/**This function disables the ge GFP error checking.
 *
 * @param[in,out] gfp pointer to the ge GFP registers
 */
void disable_gfp_err_flag(gfp_t* gfp){
	gfp->regs.config_reg_2->bits.ERR_FLAG_EN = 0;
}

/**This function enables the ge GFP error passing.
 *
 * @param[in,out] gfp pointer to the ge GFP registers
 */
void enable_gfp_pass_error(gfp_t* gfp){
	gfp->regs.config_reg_2->bits.PASS_ERR_EN = 1;
}

