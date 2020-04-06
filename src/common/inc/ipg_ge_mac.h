/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_ge_mac.h
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @date January 12, 2016
 * @brief Contains prototypes of functions defined in ipg_ge_mac.c in addition to registers and bit definitions.
 *
 * Here goes a more detailed description.
 */

#ifndef GE_MAC_H_
#define GE_MAC_H_

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
/**Bit definitions for GE_MAC_COMMAND_CONFIG_BITS.
 *
 */
struct GE_MAC_COMMAND_CONFIG_BITS{
	 volatile unsigned int  TX_EN:1;   		//!<b'0
	 volatile unsigned int  RX_EN:1;   		//!<b'1
	 volatile unsigned int  :1;      		//!<b'2
	 volatile unsigned int  ETH_SPEED:1;    //!<b'3
	 volatile unsigned int  :9;      		//!<b'12:4
	 volatile unsigned int  SW_RESET:1;     //!<b'13
	 volatile unsigned int  :17;      		//!<b'30:14
	 volatile unsigned int  RST_CNTRS:1;   	//!<b'31
};
/**Allows register access for GE_MAC_COMMAND_CONFIG_REG.
 * Located at \b 0x08 from GE MAC base.
 */
union GE_MAC_COMMAND_CONFIG_REG{
	 alt_u32 all;								//!<used to write or read from the whole register
	 struct GE_MAC_COMMAND_CONFIG_BITS bits;	//!<used for bit access
};

/**Allows register access for GE_MAC_MAC0_REG.
 * Located at \b 0x0C from GE MAC base.
 */
union GE_MAC_MAC0_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_MAC1_REG.
 * Located at \b 0x10 from GE MAC base.
 */
union GE_MAC_MAC1_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_MAX_PACKET_LENGTH_REG.
 * Located at \b 0x14 from GE MAC base.
 */
union GE_MAC_MAX_PACKET_LENGTH_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_PAUSE_QUANT_REG.
 * Located at \b 0x18 from GE MAC base.
 */
union GE_MAC_PAUSE_QUANT_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_RX_SECTION_EMPTY_REG.
 * Located at \b 0x1C from GE MAC base.
 */
union GE_MAC_RX_SECTION_EMPTY_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_RX_SECTION_FULL_REG.
 * Located at \b 0x20 from GE MAC base.
 */
union GE_MAC_RX_SECTION_FULL_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_TX_SECTION_EMPTY_REG.
 * Located at \b 0x24 from GE MAC base.
 */
union GE_MAC_TX_SECTION_EMPTY_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_TX_SECTION_FULL_REG.
 * Located at \b 0x28 from GE MAC base.
 */
union GE_MAC_TX_SECTION_FULL_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_RX_ALMOST_EMPTY_REG.
 * Located at \b 0x2C from GE MAC base.
 */
union GE_MAC_RX_ALMOST_EMPTY_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_RX_ALMOST_FULL_REG.
 * Located at \b 0x30 from GE MAC base.
 */
union GE_MAC_RX_ALMOST_FULL_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_TX_ALMOST_EMPTY_REG.
 * Located at \b 0x34 from GE MAC base.
 */
union GE_MAC_TX_ALMOST_EMPTY_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_TX_ALMOST_FULL_REG.
 * Located at \b 0x38 from GE MAC base.
 */
union GE_MAC_TX_ALMOST_FULL_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_TX_IPG_LENGTH_REG.
 * Located at \b 0x5C from GE MAC base.
 */
union GE_MAC_TX_IPG_LENGTH_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for GE_MAC_TRANSMIT_COMMAND_REG.
 * Located at \b 0xE8 from GE MAC base.
 */
union GE_MAC_TRANSMIT_COMMAND_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for BRICK_GE_TX_FRM_OK_REG.
 * Located at \b 0x68 from GE MAC base.
 */
union BRICK_GE_TX_FRM_OK_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for BRICK_GE_RX_FRM_OK_REG.
 * Located at \b 0x6C from GE MAC base.
 */
union BRICK_GE_RX_FRM_OK_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for BRICK_GE_RX_FCS_ERR_REG.
 * Located at \b 0x70 from GE MAC base.
 */
union BRICK_GE_RX_FCS_ERR_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for BRICK_GE_TX_OCTET_OK_REG.
 * Located at \b 0x78 from GE MAC base.
 */
union BRICK_GE_TX_OCTET_OK_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for BRICK_GE_RX_OCTET_OK_REG.
 * Located at \b 0x7C from GE MAC base.
 */
union BRICK_GE_RX_OCTET_OK_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**GE MAC registers
 *
 */
typedef struct{
	alt_u32 base_address;
	alt_u64 mac_address;
	volatile union GE_MAC_COMMAND_CONFIG_REG* command_config_reg; 		// 0x08 from MAC base
	volatile union GE_MAC_MAC0_REG* mac0_reg; 							// 0x0C from MAC base
	volatile union GE_MAC_MAC1_REG* mac1_reg;  							// 0x10 from MAC base
	volatile union GE_MAC_MAX_PACKET_LENGTH_REG* max_pkt_length_reg; 	// 0x14 from MAC base
	volatile union GE_MAC_PAUSE_QUANT_REG* pause_quant_reg; 			// 0x18 from MAC base
	volatile union GE_MAC_RX_SECTION_EMPTY_REG* rx_section_empty_reg; 	// 0x1C from MAC base
	volatile union GE_MAC_RX_SECTION_FULL_REG* rx_section_full_reg;     // 0x20 from MAC base
	volatile union GE_MAC_TX_SECTION_EMPTY_REG* tx_section_empty_reg;   // 0x24 from MAC base
	volatile union GE_MAC_TX_SECTION_FULL_REG* tx_section_full_reg;     // 0x28 from MAC base
	volatile union GE_MAC_RX_ALMOST_EMPTY_REG* rx_almost_empty_reg;     // 0x2C from MAC base
	volatile union GE_MAC_RX_ALMOST_FULL_REG* rx_almost_full_reg; 		// 0x30 from MAC base
	volatile union GE_MAC_TX_ALMOST_EMPTY_REG* tx_almost_empty_reg; 	// 0x34 from MAC base
	volatile union GE_MAC_TX_ALMOST_FULL_REG* tx_almost_full_reg; 		// 0x38 from MAC base
	volatile union GE_MAC_TX_IPG_LENGTH_REG* tx_ipg_length_reg; 		// 0x5c from MAC base
	volatile union GE_MAC_TRANSMIT_COMMAND_REG* transmit_command_reg; 	// 0xE8 from MAC base
	volatile union BRICK_GE_TX_FRM_OK_REG* tx_frm_ok_reg; 				// 0x68 from MAC base
	volatile union BRICK_GE_RX_FRM_OK_REG* rx_frm_ok_reg; 				// 0x6C from MAC base
	volatile union BRICK_GE_RX_FCS_ERR_REG* rx_fcs_err_reg; 			// 0x70 from MAC base
	volatile union BRICK_GE_TX_OCTET_OK_REG* tx_oct_ok_reg; 			// 0x78 from MAC base
	volatile union BRICK_GE_RX_OCTET_OK_REG* rx_oct_ok_reg; 			// 0x7C from MAC base
}ge_mac_registers_t ;

typedef struct{
	ge_mac_registers_t registers;
	alt_u64 mac_address;
}ge_mac_t;

ge_mac_registers_t* create_gemac_registers(alt_u32 base);
void map_ge_mac_regs(ge_mac_t* _mac);
void init_ge_mac(ge_mac_t* _mac);
void init_ge_mac_ol(ge_mac_registers_t* mac);
void set_ge_mac_address(ge_mac_t* _mac);
void enable_ge_mac_tx(ge_mac_t* _mac);
void enable_ge_mac_rx(ge_mac_t* _mac);
void disable_ge_mac_tx(ge_mac_t* _mac);
void disable_ge_mac_rx(ge_mac_t* _mac);
void clear_ge_counters_ol(ge_mac_registers_t* _mac);
#endif /* GE_MAC_H_ */
