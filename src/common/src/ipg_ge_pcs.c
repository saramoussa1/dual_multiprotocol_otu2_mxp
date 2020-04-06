/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_ge_pcs.c
 * @author d_chamoun (dchamoun@ipgphotonics.com)
 * @date Jan 23, 2016
 * @brief Here goes a brief description.
 * @version 0.0.3
 * Here goes a more detailed description.
 */

#include "../inc/ipg_ge_pcs.h"

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif


/**
 * This function creates an instance of the GE MAC registers.
 * @param base
 * @return
 */
ge_pcs_registers_t* create_gepcs_registers(alt_u32 base){
	ge_pcs_registers_t* _gepcs = malloc(sizeof(ge_pcs_registers_t));
	if(_gepcs != NULL){
		_gepcs->base_address = base;
		map_ge_pcs_regs_ol(_gepcs);
	}
	return _gepcs;
}


/**This function maps the PCS registers to our application variables.
 *
 * @param[in,out] _pcs pointer to the application PCS device
 * @since 0.0.1
 */
void map_ge_pcs_regs(ge_pcs_t* _pcs){
	alt_u32 pcs_base = _pcs->registers.base_address;
	_pcs->registers.ctrl_reg 			= (volatile union GE_PCS_CTRL_REG*)					((pcs_base + 0x00) | DE_CACHE_EN_BIT);
	_pcs->registers.sts_reg 			= (volatile union GE_PCS_STS_REG*)					((pcs_base + 0x04) | DE_CACHE_EN_BIT);
	_pcs->registers.partner_ability_reg	= (volatile union GE_PCS_PARTNER_ABILITY_REG*)		((pcs_base + 0x14) | DE_CACHE_EN_BIT);
	_pcs->registers.link_timer1_reg 	= (volatile union GE_PCS_AUTO_NEG_LINK_TIMER_REG1*)	((pcs_base + 0x48) | DE_CACHE_EN_BIT);
	_pcs->registers.link_timer2_reg 	= (volatile union GE_PCS_AUTO_NEG_LINK_TIMER_REG2*)	((pcs_base + 0x4C) | DE_CACHE_EN_BIT);
	_pcs->registers.if_mode_reg 		= (volatile union GE_PCS_IF_MODE_REG*)				((pcs_base + 0x50) | DE_CACHE_EN_BIT);
	_pcs->registers.dev_ability_reg 	= (volatile union GE_PCS_DEV_ABILITY_REG*)			((pcs_base + 0x10) | DE_CACHE_EN_BIT);
}

void map_ge_pcs_regs_ol(ge_pcs_registers_t* _pcs){
	alt_u32 pcs_base = _pcs->base_address;
	_pcs->ctrl_reg 						= (volatile union GE_PCS_CTRL_REG*)					((pcs_base + 0x00) | DE_CACHE_EN_BIT);
	_pcs->sts_reg 						= (volatile union GE_PCS_STS_REG*)					((pcs_base + 0x04) | DE_CACHE_EN_BIT);
	_pcs->partner_ability_reg			= (volatile union GE_PCS_PARTNER_ABILITY_REG*)		((pcs_base + 0x14) | DE_CACHE_EN_BIT);
	_pcs->link_timer1_reg 				= (volatile union GE_PCS_AUTO_NEG_LINK_TIMER_REG1*)	((pcs_base + 0x48) | DE_CACHE_EN_BIT);
	_pcs->link_timer2_reg 				= (volatile union GE_PCS_AUTO_NEG_LINK_TIMER_REG2*)	((pcs_base + 0x4C) | DE_CACHE_EN_BIT);
	_pcs->if_mode_reg 					= (volatile union GE_PCS_IF_MODE_REG*)				((pcs_base + 0x50) | DE_CACHE_EN_BIT);
	_pcs->dev_ability_reg 				= (volatile union GE_PCS_DEV_ABILITY_REG*)			((pcs_base + 0x10) | DE_CACHE_EN_BIT);
}

/**This function runs the default initialization of a GE PCS device.
 * @param[in,out] _pcs pointer to the PCS
 * @since 0.0.1
 */
void init_ge_pcs(ge_pcs_t* _pcs){
	//Enable GE auto-negotiation
	(_pcs->auto_negotiation.current)? enable_ge_pcs_an(_pcs) : disable_ge_pcs_an(_pcs);
	//PCS initialization
	_pcs->registers.link_timer1_reg->all = 0x00000D40;
	_pcs->registers.link_timer2_reg->all = 0x00000003;
	_pcs->registers.if_mode_reg->all = 0x00000003;
	_pcs->registers.dev_ability_reg->all = 0x000001E0;
	_pcs->registers.ctrl_reg->all = 0x00001340;
	// PCS Reset so that changes takes effect
	_pcs->registers.ctrl_reg->all = 0x00009140;
}

/**This function runs the default initialization of a GE PCS device.
 * @param[in,out] _pcs pointer to the PCS
 * @since 0.0.1
 */
void init_ge_pcs_ol(ge_pcs_registers_t* _pcs){
	//Enable GE auto-negotiation
	_pcs->ctrl_reg->bits.AUTO_NEG = 1;
	//PCS initialization
	_pcs->link_timer1_reg->all = 0x00000D40;
	_pcs->link_timer2_reg->all = 0x00000003;
	_pcs->if_mode_reg->all = 0x00000003;
	_pcs->dev_ability_reg->all = 0x000001E0;
	_pcs->ctrl_reg->all = 0x00001340;
	// PCS Reset so that changes takes effect
	_pcs->ctrl_reg->all = 0x00009140;
}

/**This function disables GE auto-negotiation inside a GE PHY.
 *
 * @param[in,out] _pcs pointer to the GE PCS device
 * @since 0.0.1
 */
void disable_ge_pcs_an(ge_pcs_t* _pcs){
	_pcs->registers.ctrl_reg->bits.AUTO_NEG = 0;
}

/**This function enables  GE auto-negotiation inside a GE MAC.
 *
 * @param[in,out] _pcs pointer to the GE PCS device
 * @since 0.0.1
 */
void enable_ge_pcs_an(ge_pcs_t* _pcs){
	_pcs->registers.ctrl_reg->bits.AUTO_NEG = 1;
}

/**This function checks if GE auto-negotiation inside a GE MAC is ON or OFF.
 *
 * @param[in,out] _pcs pointer to the GE PCS device
 * @return the current state of the auto-negotiation \c 1 if auto-negotiation is ON, \c 0 if auto-negotiation is OFF
 * @since 0.0.3
 */
ipg_bool_t is_ge_pcs_an_on(ge_pcs_t* _pcs)
{
	return _pcs->registers.ctrl_reg->bits.AUTO_NEG;
}

/**Gets the LINK state of a PCS PHY device.
 *
 * @param[in,out] dev pointer to the PCS PHY
 * @return the current state of the PHY \c 1 if link is UP, \c 0 if link is DOWN
 * @since 0.0.2
 */
int get_ge_pcs_link(void* dev){
	ge_pcs_t* _phy = (ge_pcs_t*)dev;
	_phy->link.previous = _phy->link.current;
	if(_phy->sfp_type == 0){ // optical
		_phy->link.current = _phy->registers.sts_reg->bits.LINK;
	}else{
		_phy->link.current = _phy->registers.partner_ability_reg->bits.LINK;
	}
	return _phy->link.current;
}
