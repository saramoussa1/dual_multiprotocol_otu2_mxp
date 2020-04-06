/*
 * xge_phy.h
 *
 *  Created on: Nov 25, 2014
 *      Author: d_chamoun
 */

#ifndef _XGE_PHY_H_
#define _XGE_PHY_H_

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

union XGE_PHY_BER_COUNT_REG{
	alt_u32 all;// used to write or read from the whole register
};

struct XGE_PHY_STS_BITS{
	volatile unsigned int  LINK:1; 			// bit 0
	volatile unsigned int  HI_BER:1; 		// bit 1
	volatile unsigned int  BLOCK_LOCK:1; 	// bit 2
	volatile unsigned int  :4; 				// bit 6:3
	volatile unsigned int  RX_DATA_RDY:1; 	// bit 7
	volatile unsigned int  :24;				// bit 31:8
};

union XGE_PHY_STS_REG{
	alt_u32 all;// used to write or read from the whole register
	struct XGE_PHY_STS_BITS bits;
};

struct XGE_PHY_RST_CTRL_BITS{
	volatile unsigned int  TX_DIGITAL_RST:1; 		// bit 0
	volatile unsigned int  RX_DIGITAL_RST:1; 		// bit 1
	volatile unsigned int  :30;						// bit 2 to bit 31
};

union XGE_PHY_RST_CTRL_REG{
	alt_u32 all;// used to write or read from the whole register
	struct XGE_PHY_RST_CTRL_BITS bits;
};

struct XGE_PHY_PMA_RX_SET_LOCKTODATA_BITS{
	volatile unsigned int  LOCK_TO_DATA:1; 	// bit 0
	volatile unsigned int  :31;				// bit 31:1
};

union XGE_PHY_PMA_RX_SET_LOCKTODATA_REG{
	alt_u32 all;// used to write or read from the whole register
	struct XGE_PHY_PMA_RX_SET_LOCKTODATA_BITS bits;
};

struct XGE_PHY_PMA_RX_SET_LOCKTOREF_BITS{
	volatile unsigned int  LOCK_TO_REF:1; 	// bit 0
	volatile unsigned int  :31;				// bit 31:1
};

union XGE_PHY_PMA_RX_SET_LOCKTOREF_REG{
	alt_u32 all;// used to write or read from the whole register
	struct XGE_PHY_PMA_RX_SET_LOCKTOREF_BITS bits;
};

struct XGE_PHY_SERIAL_LOOPBACK_BITS{
	volatile unsigned int  EN_LB:1; 		// bit 0
	volatile unsigned int  :31;				// bit 31:1
};

union XGE_PHY_SERIAL_LOOPBACK_REG{
	alt_u32 all;// used to write or read from the whole register
	struct XGE_PHY_SERIAL_LOOPBACK_BITS bits;
};

typedef struct{
	volatile union XGE_PHY_RST_CTRL_REG* 				rst_ctrl;			// 0x108 from PHY
	volatile union XGE_PHY_SERIAL_LOOPBACK_REG* 		serial_loopback;	// 0x184 from PHY
	volatile union XGE_PHY_PMA_RX_SET_LOCKTODATA_REG* 	pma_rx_ltd;			// 0x190 from PHY
	volatile union XGE_PHY_PMA_RX_SET_LOCKTOREF_REG* 	pma_rx_ltr;			// 0x194 from PHY
	volatile union XGE_PHY_STS_REG* 					sts;				// 0x208 from PHY
	volatile union XGE_PHY_BER_COUNT_REG* 				ber_count;			// 0x20C from PHY
}xge_phy_registers_t;

void map_xge_phy_regs(xge_phy_registers_t* phy, alt_u32 phy_base);
void init_xge_phy(xge_phy_registers_t* phy);
void reset_xge_phy_dig_tx(xge_phy_registers_t* phy);
void reset_xge_phy_dig_rx(xge_phy_registers_t* phy);
char get_xge_phy_link(xge_phy_registers_t* phy);
void enable_phy_serial_loopback(xge_phy_registers_t* _phy);
void disable_phy_serial_loopback(xge_phy_registers_t* _phy);
#endif /* _XGE_PHY_H_ */
