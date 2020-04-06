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
 * @file ipg_ge_mac.c
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @date Jan 12, 2016
 * @brief This file contains functions to control/monitor a GE MAC ALTERA IP.
 * @version 0.0.4
 * Control, monitor, initialization and mapping functions are available.
 */

#include "../inc/ipg_ge_mac.h"
#include <sys/alt_alarm.h>

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif

/*
 * local prototypes
 */
static void map_ge_mac_regs_ol(ge_mac_registers_t* _mac);

/**
 * This function creates an instance of the GE MAC registers.
 * @param base
 * @return
 */
ge_mac_registers_t* create_gemac_registers(alt_u32 base){
	ge_mac_registers_t* _gemac = malloc(sizeof(ge_mac_registers_t));
	if(_gemac != NULL){
		_gemac->base_address = base;
		map_ge_mac_regs_ol(_gemac);
	}
	return _gemac;
}

/**This function maps the MAC registers to our application variables.
 *
 * @param[in,out] mac pointer to the application MAC variables
 * @return None
 */
void map_ge_mac_regs(ge_mac_t* mac){
	alt_u32 mac_base = mac->registers.base_address;
	mac->registers.command_config_reg 		= (volatile union GE_MAC_COMMAND_CONFIG_REG*)		((mac_base + 0x08) | DE_CACHE_EN_BIT);
	mac->registers.mac0_reg 				= (volatile union GE_MAC_MAC0_REG*)					((mac_base + 0x0C) | DE_CACHE_EN_BIT);
	mac->registers.mac1_reg 				= (volatile union GE_MAC_MAC1_REG*)					((mac_base + 0x10) | DE_CACHE_EN_BIT);
	mac->registers.max_pkt_length_reg 		= (volatile union GE_MAC_MAX_PACKET_LENGTH_REG*)	((mac_base + 0x14) | DE_CACHE_EN_BIT);
	mac->registers.pause_quant_reg 			= (volatile union GE_MAC_PAUSE_QUANT_REG*)			((mac_base + 0x18) | DE_CACHE_EN_BIT);
	mac->registers.rx_section_empty_reg 	= (volatile union GE_MAC_RX_SECTION_EMPTY_REG*)		((mac_base + 0x1C) | DE_CACHE_EN_BIT);
	mac->registers.rx_section_full_reg 		= (volatile union GE_MAC_RX_SECTION_FULL_REG*)		((mac_base + 0x20) | DE_CACHE_EN_BIT);
	mac->registers.tx_section_empty_reg 	= (volatile union GE_MAC_TX_SECTION_EMPTY_REG*)		((mac_base + 0x24) | DE_CACHE_EN_BIT);
	mac->registers.tx_section_full_reg 		= (volatile union GE_MAC_TX_SECTION_FULL_REG*)		((mac_base + 0x28) | DE_CACHE_EN_BIT);
	mac->registers.rx_almost_empty_reg 		= (volatile union GE_MAC_RX_ALMOST_EMPTY_REG*)		((mac_base + 0x2C) | DE_CACHE_EN_BIT);
	mac->registers.rx_almost_full_reg 		= (volatile union GE_MAC_RX_ALMOST_FULL_REG*)		((mac_base + 0x30) | DE_CACHE_EN_BIT);
	mac->registers.tx_almost_empty_reg 		= (volatile union GE_MAC_TX_ALMOST_EMPTY_REG*)		((mac_base + 0x34) | DE_CACHE_EN_BIT);
	mac->registers.tx_almost_full_reg 		= (volatile union GE_MAC_TX_ALMOST_FULL_REG*)		((mac_base + 0x38) | DE_CACHE_EN_BIT);
	mac->registers.tx_ipg_length_reg 		= (volatile union GE_MAC_TX_IPG_LENGTH_REG*)		((mac_base + 0x5C) | DE_CACHE_EN_BIT);
	mac->registers.transmit_command_reg 	= (volatile union GE_MAC_TRANSMIT_COMMAND_REG*)		((mac_base + 0xE8) | DE_CACHE_EN_BIT);
	mac->registers.tx_frm_ok_reg 			= (volatile union BRICK_GE_TX_FRM_OK_REG*)			((mac_base + 0x68) | DE_CACHE_EN_BIT);
	mac->registers.rx_frm_ok_reg 			= (volatile union BRICK_GE_RX_FRM_OK_REG*)			((mac_base + 0x6C) | DE_CACHE_EN_BIT);
	mac->registers.rx_fcs_err_reg 			= (volatile union BRICK_GE_RX_FCS_ERR_REG*)			((mac_base + 0x70) | DE_CACHE_EN_BIT);
	mac->registers.tx_oct_ok_reg 			= (volatile union BRICK_GE_TX_OCTET_OK_REG*)		((mac_base + 0x78) | DE_CACHE_EN_BIT);
	mac->registers.rx_oct_ok_reg 			= (volatile union BRICK_GE_RX_OCTET_OK_REG*)		((mac_base + 0x7C) | DE_CACHE_EN_BIT);
}

static void map_ge_mac_regs_ol(ge_mac_registers_t* _mac){
	alt_u32 mac_base = _mac->base_address;
	_mac->command_config_reg 		= (volatile union GE_MAC_COMMAND_CONFIG_REG*)		((mac_base + 0x08) | DE_CACHE_EN_BIT);
	_mac->mac0_reg 				= (volatile union GE_MAC_MAC0_REG*)					((mac_base + 0x0C) | DE_CACHE_EN_BIT);
	_mac->mac1_reg 				= (volatile union GE_MAC_MAC1_REG*)					((mac_base + 0x10) | DE_CACHE_EN_BIT);
	_mac->max_pkt_length_reg 		= (volatile union GE_MAC_MAX_PACKET_LENGTH_REG*)	((mac_base + 0x14) | DE_CACHE_EN_BIT);
	_mac->pause_quant_reg 			= (volatile union GE_MAC_PAUSE_QUANT_REG*)			((mac_base + 0x18) | DE_CACHE_EN_BIT);
	_mac->rx_section_empty_reg 	= (volatile union GE_MAC_RX_SECTION_EMPTY_REG*)		((mac_base + 0x1C) | DE_CACHE_EN_BIT);
	_mac->rx_section_full_reg 		= (volatile union GE_MAC_RX_SECTION_FULL_REG*)		((mac_base + 0x20) | DE_CACHE_EN_BIT);
	_mac->tx_section_empty_reg 	= (volatile union GE_MAC_TX_SECTION_EMPTY_REG*)		((mac_base + 0x24) | DE_CACHE_EN_BIT);
	_mac->tx_section_full_reg 		= (volatile union GE_MAC_TX_SECTION_FULL_REG*)		((mac_base + 0x28) | DE_CACHE_EN_BIT);
	_mac->rx_almost_empty_reg 		= (volatile union GE_MAC_RX_ALMOST_EMPTY_REG*)		((mac_base + 0x2C) | DE_CACHE_EN_BIT);
	_mac->rx_almost_full_reg 		= (volatile union GE_MAC_RX_ALMOST_FULL_REG*)		((mac_base + 0x30) | DE_CACHE_EN_BIT);
	_mac->tx_almost_empty_reg 		= (volatile union GE_MAC_TX_ALMOST_EMPTY_REG*)		((mac_base + 0x34) | DE_CACHE_EN_BIT);
	_mac->tx_almost_full_reg 		= (volatile union GE_MAC_TX_ALMOST_FULL_REG*)		((mac_base + 0x38) | DE_CACHE_EN_BIT);
	_mac->tx_ipg_length_reg 		= (volatile union GE_MAC_TX_IPG_LENGTH_REG*)		((mac_base + 0x5C) | DE_CACHE_EN_BIT);
	_mac->transmit_command_reg 	= (volatile union GE_MAC_TRANSMIT_COMMAND_REG*)		((mac_base + 0xE8) | DE_CACHE_EN_BIT);
	_mac->tx_frm_ok_reg 			= (volatile union BRICK_GE_TX_FRM_OK_REG*)			((mac_base + 0x68) | DE_CACHE_EN_BIT);
	_mac->rx_frm_ok_reg 			= (volatile union BRICK_GE_RX_FRM_OK_REG*)			((mac_base + 0x6C) | DE_CACHE_EN_BIT);
	_mac->rx_fcs_err_reg 			= (volatile union BRICK_GE_RX_FCS_ERR_REG*)			((mac_base + 0x70) | DE_CACHE_EN_BIT);
	_mac->tx_oct_ok_reg 			= (volatile union BRICK_GE_TX_OCTET_OK_REG*)		((mac_base + 0x78) | DE_CACHE_EN_BIT);
	_mac->rx_oct_ok_reg 			= (volatile union BRICK_GE_RX_OCTET_OK_REG*)		((mac_base + 0x7C) | DE_CACHE_EN_BIT);
}

/**This function runs the default initialization of a GE MAC device.
 * @param[in,out] mac pointer to the MAC device
 */
void init_ge_mac(ge_mac_t* mac){
	alt_u32 temp = 0;
	set_ge_mac_address(mac);
	mac->registers.command_config_reg->all = 0x01000050;
	mac->registers.max_pkt_length_reg->all = 0x00002EE0;
	mac->registers.tx_section_empty_reg->all = 0x00000FF0;
	mac->registers.rx_section_empty_reg->all = 0x00000FF0;
	mac->registers.tx_section_full_reg->all = 0x00000010;
	mac->registers.pause_quant_reg->all = 0x0000000f;
	mac->registers.tx_ipg_length_reg->all = 0x0000000c;
	mac->registers.transmit_command_reg->all = 0x00020000;
	mac->registers.rx_section_full_reg->all = 0x000005DC;
	mac->registers.rx_almost_empty_reg->all = 0x08;
	mac->registers.rx_almost_full_reg->all = 0x08;
	mac->registers.tx_almost_empty_reg->all = 0x08;
	mac->registers.tx_almost_full_reg->all = 0x05;

	// SW_RESET The MAC
	mac->registers.command_config_reg->all = 0x01002050;

	while (mac->registers.command_config_reg->bits.SW_RESET){
		temp = mac->registers.command_config_reg->all;
	}
	// Enable TX / RX
	mac->registers.command_config_reg->all = 0x01000053;
}

void init_ge_mac_ol(ge_mac_registers_t* mac){
	mac->mac0_reg->all = (alt_u32)((mac->mac_address) & 0xFFFFFFFF);
	mac->mac1_reg->all = (alt_u32)(mac->mac_address & 0xFFFF);
	mac->command_config_reg->all = 0x01000050;
	mac->max_pkt_length_reg->all = 0x00002EE0;
	mac->tx_section_empty_reg->all = 0x00000FF0;
	mac->rx_section_empty_reg->all = 0x00000FF0;
	mac->tx_section_full_reg->all = 0x00000010;
	mac->pause_quant_reg->all = 0x0000000f;
	mac->tx_ipg_length_reg->all = 0x0000000c;
	mac->transmit_command_reg->all = 0x00020000;
	mac->rx_section_full_reg->all = 0x000005DC;
	mac->rx_almost_empty_reg->all = 0x08;
	mac->rx_almost_full_reg->all = 0x08;
	mac->tx_almost_empty_reg->all = 0x08;
	mac->tx_almost_full_reg->all = 0x05;

	// SW_RESET The MAC
	mac->command_config_reg->all = 0x01002050;

	// Enable TX / RX
	mac->command_config_reg->all = 0x01000053;
}

/**This function sets the MAC address of the MAC.
 * The MAC address is embedded in the ge_mac_t structure.
 * @param[in,out] _mac pointer to the MAC device
 * @since 0.0.1
 */
void set_ge_mac_address(ge_mac_t* _mac){
	_mac->registers.mac0_reg->all = (alt_u32)_mac->mac_address;
	_mac->registers.mac1_reg->all = (alt_u32)((_mac->mac_address >> 32) & 0xFFFF);
}

/**This function enables the MAC transmitter function.
 *
 * @param[in,out] _mac pointer to the MAC device
 * @since 0.0.2
 */
void enable_ge_mac_tx(ge_mac_t* _mac){
	_mac->registers.command_config_reg->bits.TX_EN = 1;
}

/**This function enables the MAC receiver function.
 *
 * @param[in,out] _mac pointer to the MAC device
 * @since 0.0.2
 */
void enable_ge_mac_rx(ge_mac_t* _mac){
	_mac->registers.command_config_reg->bits.RX_EN = 1;
}

/**This function disables the MAC transmitter function.
 *
 * @param[in,out] _mac pointer to the MAC device
 * @since 0.0.2
 */
void disable_ge_mac_tx(ge_mac_t* _mac){
	_mac->registers.command_config_reg->bits.TX_EN = 0;
}

/**This function disables the MAC receiver function.
 *
 * @param[in,out] _mac pointer to the MAC device
 * @since 0.0.2
 */
void disable_ge_mac_rx(ge_mac_t* _mac){
	_mac->registers.command_config_reg->bits.RX_EN = 0;
}

/**This function clears the GE counters.
 *
 * @param[in,out] _mac pointer to the MAC device
 * @since 0.0.3
 */
void clear_ge_counters(ge_mac_t* _mac){
	_mac->registers.command_config_reg->bits.RST_CNTRS = 1;
}

/**This function clears the GE counters.
 * This is an overload function
 * @param[in,out] _mac pointer to the MAC device
 * @since 0.0.4
 */
void clear_ge_counters_ol(ge_mac_registers_t* _mac){
	_mac->command_config_reg->bits.RST_CNTRS = 1;
}
