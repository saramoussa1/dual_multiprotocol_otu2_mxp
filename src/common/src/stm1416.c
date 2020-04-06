/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) 2015 IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *
 * IPG Photonics Corporation
 * 50 Old Webster Road
 * Oxford, MA 01540, USA
 ****************************************************************************/

/**
 * @file xge_mac.c
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @date Jan 23, 2015
 * @brief This file contains functions to control/monitor a 10GE (XGE) MAC ALTERA IP.
 * @version 0.0.3
 * Control, monitor, initialization and mapping functions are available.
 */


#include "../inc/stm1416.h"
#include <sys/alt_alarm.h>

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif

/*
 * local prototypes
 */


/**This function maps the STM 1/4/16 registers to our application variables.
 *
 * @param[in,out] _stm 		pointer to the STM registers
 * @param mac_base	base 	address of the STM logic
 * @return None
 */
void map_stm1416_regs(stm1416_registers_t* _stm, alt_u32 stm_base){
	_stm->live_status 	= (volatile union STM_1416_LIVE_STS_REG*)		((stm_base + 0x00) | DE_CACHE_EN_BIT);
	_stm->int_status 	= (volatile union STM_1416_INT_STS_REG*)		((stm_base + 0x04) | DE_CACHE_EN_BIT);
	_stm->config 		= (volatile union STM_1416_CONFIG_REG*)		((stm_base + 0x0C) | DE_CACHE_EN_BIT);
	_stm->reset 		= (volatile union STM_1416_RESET_REG*)		((stm_base + 0x10) | DE_CACHE_EN_BIT);
	_stm->tx_lrdi 		= (volatile union STM_1416_TX_LRDI_REG*)		((stm_base + 0x1C) | DE_CACHE_EN_BIT);
	_stm->b1_err 		= (volatile union STM_1416_B1_ERRORS_REG*)	((stm_base + 0xC0) | DE_CACHE_EN_BIT);
	_stm->b2_err		= (volatile union STM_1416_B2_ERRORS_REG*)	((stm_base + 0xC4) | DE_CACHE_EN_BIT);
	_stm->rei_err		= (volatile union STM_1416_REI_ERRORS_REG*)	((stm_base + 0xC8) | DE_CACHE_EN_BIT);
}

/**
 * This function creates an instance of the STM registers.
 * @param[in] stm1416_base	base location of the STM instance.
 * @return pointer to the created instance
 */
stm1416_registers_t* create_stm1416_registers(alt_u32 stm1416_base){
	stm1416_registers_t* _stm = malloc(sizeof(stm1416_registers_t));
	map_stm1416_regs(_stm, stm1416_base);
	return _stm;
}


