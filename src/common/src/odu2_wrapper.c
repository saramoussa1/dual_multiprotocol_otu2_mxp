/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file odu2_wrapper.c
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 5, 2015
 * @brief Contains procedures to access, monitor and configure the ODU2 wrapper developed by IPG.
 * @version 0.0.2 Created the odu2_wrapper_t type which contains the odu2_wrapper_regsiters_t
 */
#include "../inc/odu2_wrapper.h"

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif

/*
 * local prototypes
 */

/**This function runs the default initialization of an ODU2 wrapper device.
 * @param[in,out] wrapper pointer to the ODU2 wrapper device registers
 */
void init_odu2_wrapper(odu2_wrapper_t* wrapper){
}

/**This function maps the ODU2 wrapper registers to our application variables.
 *
 * @param[in,out] wrapper pointer to the application ODU2 wrapper variables
 * @param wrapper_base	base address of the wrapper in memory
 * @return None
 */
void map_odu2_wrapper_regs(odu2_wrapper_t* wrapper, alt_u32 base){
	map_odu2_wrapper_regs_ol(&wrapper->regs, base);
}

void map_odu2_wrapper_regs_ol(odu2_wrapper_registers_t* wrapper, alt_u32 base){
	wrapper->live_sts_reg 			= (volatile union ODU2_WRAPPER_LIVE_STS_REG*) 		((base + 0x00) | DE_CACHE_EN_BIT);
	wrapper->int_sts_reg 			= (volatile union ODU2_WRAPPER_INT_STS_REG*) 		((base + 0x04) | DE_CACHE_EN_BIT);
	wrapper->int_en_reg			= (volatile union ODU2_WRAPPER_INT_EN_REG*) 		((base + 0x08) | DE_CACHE_EN_BIT);
	wrapper->general_ctrl_reg 		= (volatile union ODU2_WRAPPER_GENERAL_CTRL_REG*) 	((base + 0x0C) | DE_CACHE_EN_BIT);
	wrapper->tx_epxstat_reg 		= (volatile union ODU2_WRAPPER_TX_EXPSTATPT_REG*) 	((base + 0x20) | DE_CACHE_EN_BIT);
	wrapper->rx_epxstat_reg 		= (volatile union ODU2_WRAPPER_RX_EXPSTATPT_REG*) 	((base + 0x24) | DE_CACHE_EN_BIT);
	wrapper->bip8_cnt_reg 			= (volatile unsigned int*) 							((base + 0x2C) | DE_CACHE_EN_BIT);
}
odu2_wrapper_t* create_odu2_wrapper_regs(alt_u32 base){
	odu2_wrapper_t* wrapper = malloc(sizeof(odu2_wrapper_t));
	map_odu2_wrapper_regs_ol(wrapper, base);
	return wrapper;
}


