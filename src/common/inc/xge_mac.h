/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file xge_mac.h
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 5, 2015
 * @brief Contains prototypes of functions defined in xge_mac.c in addition to registers and bit defintions.
 *
 * Here goes a more detailed description.
 */
#ifndef _XGE_MAC_H_
#define _XGE_MAC_H_

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

#define XGE_MAC_V1	1
#define XGE_MAC_V2	2
#ifndef XGE_MAC_VER
#define XGE_MAC_VER	XGE_MAC_V1
#endif

/**Bit definitions for XGE_MAC_RX_PADCRC_CTRL_BITS.
 *
 */
struct XGE_MAC_RX_PADCRC_CTRL_BITS{
	 volatile unsigned int  CRC_REMOVAL_EN:1;		//!<b'0 RW, when set to 1, enables CRC removal from RX frames
	 volatile unsigned int  PADDING_REMOVAL_EN:1;  	//!<b'1 RW, when set to 1, enables padding removal from RX frames
	 volatile unsigned int  RESERVED:30;      		//!<b'31:2 reserved for future use
};
/**Allows register access for XGE_MAC_RX_PADCRC_CTRL_REG.
 * Located at \b 0x100 from MAC framer base.
 */
union XGE_MAC_RX_PADCRC_CTRL_REG{
	 alt_u32 all;								//!<used to write or read from the whole register
	 struct XGE_MAC_RX_PADCRC_CTRL_BITS bits;	//!<used for bit access
};

/**Bit definitions for XGE_MAC_RX_TRANSFER_CTRL_BITS.
 *
 */
struct XGE_MAC_RX_TRANSFER_CTRL_BITS{
	 volatile unsigned int  EN_RECEIVE_PATH:1;  //!<b'0 RW, when set to 1, enables the receiver of the XGE MAC
	 volatile unsigned int  RESERVED:31;     	//!<b'31:1 reserved for future use
};
/**Allows register access for XGE_MAC_RX_TRANSFER_CTRL_REG.
 * Located at \b 0x000 from MAC framer base.
 */
union XGE_MAC_RX_TRANSFER_CTRL_REG{
	 alt_u32 all;								//!<used to write or read from the whole register
	 struct XGE_MAC_RX_TRANSFER_CTRL_BITS bits;	//!<used for bit access
};

/**Bit definitions for XGE_MAC_TX_TRANSFER_CTRL_BITS.
 *
 */
struct XGE_MAC_TX_TRANSFER_CTRL_BITS{
	 volatile unsigned int  EN_TRANSMIT_PATH:1;	//!<b'0 RW, when set to 1, enables the transmitter of the XGE MAC
	 volatile unsigned int  RESERVED:31;      	//!<b'31:1 reserved for future use
};
/**Allows register access for XGE_MAC_TX_TRANSFER_CTRL_REG.
 * Located at \b 0x4000 from MAC framer base.
 */
union XGE_MAC_TX_TRANSFER_CTRL_REG{
	 alt_u32 all;									//!<used to write or read from the whole register
	 struct XGE_MAC_TX_TRANSFER_CTRL_BITS bits;		//!<used for bit access
};

/**Bit definitions for XGE_MAC_TX_UNIDIR_CTRL_BITS.
 *
 */
struct XGE_MAC_TX_UNIDIR_CTRL_BITS{
	 volatile unsigned int  EN_UNIDIRECTIONAL_MODE:1;			//!<b'0 RW, when set to 1, enables TX unidirectional mode
	 volatile unsigned int  DIS_REMOTE_FAULT_SEQ_GENERATION:1;	//!<b'1 RW, when set to 0, means don't turn OFF link on the TX when there is no LINK on the RX.
	 volatile unsigned int  RESERVED:30;      	//!<b'31:2 reserved for future use
};
/**Allows register access for XGE_MAC_TX_UNIDIR_CTRL_REG.
 * Located at \b 0x4480 from MAC framer base.
 */
union XGE_MAC_TX_UNIDIR_CTRL_REG{
	 alt_u32 all;									//!<used to write or read from the whole register
	 struct XGE_MAC_TX_UNIDIR_CTRL_BITS bits;		//!<used for bit access
};

/**Bit definitions for XGE_MAC_RX_STAT_CLR_BITS.
 *
 */
struct XGE_MAC_RX_STAT_CLR_BITS{
};
/**Allows register access for XGE_MAC_TX_TRANSFER_CTRL_REG.
 * Located at \b 0x3000 from MAC framer base.
 */
union XGE_MAC_RX_STAT_CLR_REG{
	 alt_u32 all;							//!< used to write or read from the whole register
	 struct XGE_MAC_RX_STAT_CLR_BITS bits; 	//!<used for bit access
};

/**Bit definitions for BRICK_MAC_TX_STAT_CLR_BITS.
 *
 */
struct BRICK_MAC_TX_STAT_CLR_BITS{
};
/**Allows register access for XGE_MAC_TX_STAT_CLR_REG.
 * Located at \b 0x7000 from MAC framer base.
 */
union XGE_MAC_TX_STAT_CLR_REG{
	alt_u32 all;							//!<used to write or read from the whole register
	struct BRICK_MAC_TX_STAT_CLR_BITS bits; //!<used for bit access
};

/**Allows register access for XGE_MAC_RX_FRAME_MAXLENGTH_REG.
 * Located at \b 0x2004 from MAC framer base.
 */
union XGE_MAC_RX_FRAME_MAXLENGTH_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_FRAME_OK_MSB_REG.
 * Located at \b 0x300C from MAC framer base.
 */
union XGE_MAC_RX_FRAME_OK_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_FRAME_OK_LSB_REG.
 * Located at \b 0x3008 from MAC framer base.
 */
union XGE_MAC_RX_FRAME_OK_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

#if XGE_MAC_VER == XGE_MAC_V2
/**Allows register access for XGE_MAC_RX_FRAME_CRC_ERR_MSB_REG.
 * Located at 0x7018\b  from MAC framer base.
 */
union XGE_MAC_TX_FRAME_CRC_ERR_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_FRAME_CRC_ERR_LSB_REG.
 * Located at 0x701C\b  from MAC framer base.
 */
union XGE_MAC_TX_FRAME_CRC_ERR_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_OCTS_MSB_REG.
 * Located at \b 0x3024 from MAC framer base.
 */
union XGE_MAC_RX_OCTS_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_OCTS_LSB_REG.
 * Located at \b 0x3020 from MAC framer base.
 */
union XGE_MAC_RX_OCTS_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_OCTS_MSB_REG.
 * Located at \b 0x7024 from MAC framer base.
 */
union XGE_MAC_TX_OCTS_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_OCTS_LSB_REG.
 * Located at \b 0x7020 from MAC framer base.
 */
union XGE_MAC_TX_OCTS_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};
#endif

/**Allows register access for XGE_MAC_TX_FRAME_OK_MSB_REG.
 * Located at \b 0x700C from MAC framer base.
 */
union XGE_MAC_TX_FRAME_OK_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_FRAME_OK_LSB_REG.
 * Located at \b 0x7008 from MAC framer base.
 */
union XGE_MAC_TX_FRAME_OK_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

#ifndef REDUCE_MEM
/**Allows register access for XGE_MAC_RX_BYTES_OK_MSB_REG.
 * Located at \b 0x3020 from MAC framer base.
 */
union XGE_MAC_RX_BYTES_OK_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole registe
};
/**Allows register access for XGE_MAC_RX_BYTES_OK_LSB_REG.
 * Located at \b 0x3024 from MAC framer base.
 */
union XGE_MAC_RX_BYTES_OK_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};
/**Allows register access for XGE_MAC_TX_BYTES_OK_MSB_REG.
 * Located at \b 0x7020 from MAC framer base.
 */
union XGE_MAC_TX_BYTES_OK_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_BYTES_OK_LSB_REG.
 * Located at \b 0x7024 from MAC framer base.
 */
union XGE_MAC_TX_BYTES_OK_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};
/**Allows register access for XGE_MAC_RX_FRAME_ERR_MSB_REG.
 * Located at \b 0x3010 from MAC framer base.
 */
union XGE_MAC_RX_FRAME_ERR_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_FRAME_ERR_LSB_REG.
 * Located at \b 0x3014 from MAC framer base.
 */
union XGE_MAC_RX_FRAME_ERR_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};
/**Allows register access for XGE_MAC_TX_FRAME_ERR_MSB_REG.
 * Located at \b 0x7010 from MAC framer base.
 */
union XGE_MAC_TX_FRAME_ERR_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_FRAME_ERR_LSB_REG.
 * Located at \b 0x7014 from MAC framer base.
 */
union XGE_MAC_TX_FRAME_ERR_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};
/**Allows register access for XGE_MAC_TX_FRAME_CRC_ERR_MSB_REG.
 * Located at \b 0x7018 from MAC framer base.
 */
union XGE_MAC_TX_FRAME_CRC_ERR_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_FRAME_CRC_ERR_LSB_REG.
 * Located at \b 0x301C from MAC framer base.
 */
union XGE_MAC_TX_FRAME_CRC_ERR_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};


/**Allows register access for XGE_MAC_RX_FRAME_IF_ERR_MSB_REG.
 * Located at \b 0x3030 from MAC framer base.
 */
union XGE_MAC_RX_FRAME_IF_ERR_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_FRAME_IF_ERR_LSB_REG.
 * Located at \b 0x3034 from MAC framer base.
 */
union XGE_MAC_RX_FRAME_IF_ERR_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_FRAME_IF_ERR_MSB_REG.
 * Located at \b 0x7030 from MAC framer base.
 */
union XGE_MAC_TX_FRAME_IF_ERR_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_TX_FRAME_IF_ERR_LSB_REG.
 * Located at \b 0x7034 from MAC framer base.
 */
union XGE_MAC_TX_FRAME_IF_ERR_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};
#endif

/**Allows register access for XGE_MAC_RX_FRAME_CRC_ERR_MSB_REG.
 * Located at \b 0x3018 from MAC framer base.
 */
union XGE_MAC_RX_FRAME_CRC_ERR_MSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Allows register access for XGE_MAC_RX_FRAME_CRC_ERR_LSB_REG.
 * Located at \b 0x301C from MAC framer base.
 */
union XGE_MAC_RX_FRAME_CRC_ERR_LSB_REG{
	alt_u32 all;			//!<used to write or read from the whole register
};

/**Bit definitions for XGE_MAC_TX_CRCINS_CTRL_BITS.
 *
 */
struct XGE_MAC_TX_CRCINS_CTRL_BITS{
	 volatile unsigned int  RESERVED_1:1;     	//!<b'0 reserved for future use
	 volatile unsigned int  CRC_INSERTION_EN:1;	//!<b'1 RW, when set to 1, allows the XGE MAC transmitter to insert CRC in transmitted frames
	 volatile unsigned int  RESERVED_2:30;		//!<b'31:2 reserved for future use
};
/**Allows register access for XGE_MAC_TX_CRCINS_CTRL_REG.
 * Located at \b 0x4200 from MAC framer base.
 */
union XGE_MAC_TX_CRCINS_CTRL_REG{
	alt_u32 all;								//!<used to write or read from the whole register
    struct XGE_MAC_TX_CRCINS_CTRL_BITS bits ;	//!<used for bit access
};

/**Allows register access for XGE_MAC_TX_FRAME_MAXLENGTH_REG.
 * Located at \b 0x600C from MAC framer base.
 */
union XGE_MAC_TX_FRAME_MAXLENGTH_REG{
	alt_u32 all;				//!<used to write or read from the whole register
};

/**XGE MAC registers
 *
 */
typedef struct{
	volatile union XGE_MAC_RX_TRANSFER_CTRL_REG* rx_transfer_ctrl; 				// 0x000 from MAC base
	volatile union XGE_MAC_TX_TRANSFER_CTRL_REG* tx_transfer_ctrl; 				// 0x4000 from MAC base
	volatile union XGE_MAC_RX_PADCRC_CTRL_REG* rx_padcrc_ctrl_reg;  				// 0x100 from MAC base
	volatile union XGE_MAC_RX_STAT_CLR_REG* rx_stat_clr_reg; 						// 0x3000 from MAC base
	volatile union XGE_MAC_TX_STAT_CLR_REG* tx_stat_clr_reg; 						// 0x7000 from MAC base
	volatile union XGE_MAC_RX_FRAME_MAXLENGTH_REG* rx_frame_maxlen_reg; 			// 0x2004 from MAC base
	volatile union XGE_MAC_RX_FRAME_OK_MSB_REG* rx_frame_ok_msb_reg;     			// 0x300C from MAC base
	volatile union XGE_MAC_RX_FRAME_OK_LSB_REG* rx_frame_ok_lsb_reg;    			// 0x3008 from MAC base
	volatile union XGE_MAC_RX_FRAME_CRC_ERR_MSB_REG* rx_frame_crc_err_msb_reg; 	// 0x3018 from MAC base
	volatile union XGE_MAC_RX_FRAME_CRC_ERR_LSB_REG* rx_frame_crc_err_lsb_reg; 	// 0x301c from MAC base
#if XGE_MAC_VER == XGE_MAC_V2
	volatile union XGE_MAC_TX_FRAME_CRC_ERR_MSB_REG* tx_frame_crc_err_msb_reg; 	// 0x7018 from MAC base
	volatile union XGE_MAC_TX_FRAME_CRC_ERR_LSB_REG* tx_frame_crc_err_lsb_reg; 	// 0x701C from MAC base
	volatile union XGE_MAC_RX_OCTS_MSB_REG* rx_octs_msb_reg;     			// 0x3024 from MAC base
	volatile union XGE_MAC_RX_OCTS_LSB_REG* rx_octs_lsb_reg;    			// 0x3020 from MAC base
	volatile union XGE_MAC_TX_OCTS_MSB_REG* tx_octs_msb_reg;     			// 0x7024 from MAC base
	volatile union XGE_MAC_TX_OCTS_LSB_REG* tx_octs_lsb_reg;    			// 0x7020 from MAC base
#endif
#ifndef REDUCE_MEM
	volatile union XGE_MAC_RX_BYTES_OK_MSB_REG* rx_bytes_ok_msb_reg;     			// 0x3020 from MAC base
	volatile union XGE_MAC_RX_BYTES_OK_LSB_REG* rx_bytes_ok_lsb_reg;     			// 0x3024 from MAC base
	volatile union XGE_MAC_RX_FRAME_ERR_MSB_REG* rx_frame_err_msb_reg; 			// 0x3010 from MAC base
	volatile union XGE_MAC_RX_FRAME_ERR_LSB_REG* rx_frame_err_lsb_reg; 			// 0x3014 from MAC base
	volatile union XGE_MAC_RX_FRAME_IF_ERR_MSB_REG* rx_frame_if_err_msb_reg; 		// 0x3030 from MAC base
	volatile union XGE_MAC_RX_FRAME_IF_ERR_LSB_REG* rx_frame_if_err_lsb_reg; 		// 0x3034 from MAC base
#endif
	volatile union XGE_MAC_TX_FRAME_MAXLENGTH_REG* tx_frame_maxlen_reg; 			// 0x600C from MAC base
	volatile union XGE_MAC_TX_FRAME_OK_MSB_REG* tx_frame_ok_msb_reg;     			// 0x700C from MAC base
	volatile union XGE_MAC_TX_FRAME_OK_LSB_REG* tx_frame_ok_lsb_reg;     			// 0x7008 from MAC base
	volatile union XGE_MAC_TX_CRCINS_CTRL_REG* tx_crcins_ctrl_reg;  				// 0x4200 from MAC base
	volatile union XGE_MAC_TX_UNIDIR_CTRL_REG* tx_unidir_ctrl_reg;  				// 0x4480 from MAC base
#ifndef REDUCE_MEM
	volatile union XGE_MAC_TX_BYTES_OK_MSB_REG* tx_bytes_ok_msb_reg;     			// 0x7020 from MAC base
	volatile union XGE_MAC_TX_BYTES_OK_LSB_REG* tx_bytes_ok_lsb_reg;     			// 0x7024 from MAC base
	volatile union XGE_MAC_TX_FRAME_ERR_MSB_REG* tx_frame_err_msb_reg; 			// 0x7010 from MAC base
	volatile union XGE_MAC_TX_FRAME_ERR_LSB_REG* tx_frame_err_lsb_reg; 			// 0x7014 from MAC base
	volatile union XGE_MAC_TX_FRAME_CRC_ERR_MSB_REG* tx_frame_crc_err_msb_reg; 	// 0x7018 from MAC base
	volatile union XGE_MAC_TX_FRAME_CRC_ERR_LSB_REG* tx_frame_crc_err_lsb_reg; 	// 0x701c from MAC base
	volatile union XGE_MAC_TX_FRAME_IF_ERR_MSB_REG* tx_frame_if_err_msb_reg; 		// 0x7030 from MAC base
	volatile union XGE_MAC_TX_FRAME_IF_ERR_LSB_REG* tx_frame_if_err_lsb_reg; 		// 0x7034 from MAC base
#endif
}xge_mac_registers_t ;

/**Representation of XGE MAC statistics.
 *
 */
typedef struct{
	ipg_control_t 	llcf_enable;	//!<Variable to enable/disable LLCF functionality
	ipg_status_t 	link ;			//!<Status of the LINK (down or up)
	ipg_status_t 	remote_link ;	//!<Status of the remote LINK (down or up)
	alt_u32			arr_rx_frames_ok[2];
	alt_u32			arr_tx_frames_ok[2];
	alt_u32			arr_err_frames_crc[2];
	ipg_counter64_t  rx_frames_ok;		//!<36-bit counter of successfully received frames
	ipg_counter64_t  tx_frames_ok;		//!<36-bit counter of successfully transmitted frames
	ipg_counter64_t  err_frames_crc;		//!<36-bit counter of received frames with CRC errors
	ipg_control_t	clear;			//!<control to clear and reset the statistics
}xge_mac_statistics_t;

typedef struct{
	xge_mac_statistics_t 	stats;
	xge_mac_registers_t		regs;
}xge_mac_t;

//#define clear_xge_mac_tx_stats(x) __generic(x, xge_mac_t, clear_xge_mac_tx_counters, clear_xge_mac_tx_counters_ol)
//#define clear_xge_mac_rx_stats(x) __generic(x, xge_mac_t, clear_xge_mac_rx_counters, clear_xge_mac_rx_counters_ol)
//#define map_xge_mac_registers(x, base) __generic(x, xge_mac_t, map_xge_mac_regs, map_xge_mac_regs_ol)
//#define init_xge_mac_registers(x) 	__generic(*x, xge_mac_t, init_xge_mac, init_xge_mac_ol)

void map_xge_mac_regs(xge_mac_t* mac, alt_u32 mac_base);
void map_xge_mac_regs_ol(xge_mac_registers_t* mac, alt_u32 mac_base);
xge_mac_registers_t* create_xge_registers(alt_u32 mac_base);
void init_xge_mac(xge_mac_t* mac);
void init_xge_mac_ol(xge_mac_registers_t* mac);
void clear_xge_mac_tx_counters (xge_mac_t* mac);
void clear_xge_mac_rx_counters (xge_mac_t* mac);
void enable_xge_mac_tx_unidir_mode(xge_mac_t* _mac);
void disable_xge_mac_tx_unidir_mode(xge_mac_t* _mac);
void disable_xge_mac_tx_remote_fault_generation(xge_mac_t* _mac);
void enable_xge_mac_tx_remote_fault_generation(xge_mac_t* _mac);
void get_llcf_mode(xge_mac_t* mac);
void set_llcf_mode(xge_mac_t* mac);
void get_xge_mac_rx_frames_ok(xge_mac_t* _mac);
void get_xge_mac_tx_frames_ok(xge_mac_t* _mac);
void get_xge_mac_err_frames_crc(xge_mac_t* _mac);
void get_xge_mac_link(xge_mac_t* _mac);
void clear_xge_mac_rx_counters_ol(xge_mac_registers_t* mac);
void clear_xge_mac_tx_counters_ol(xge_mac_registers_t* mac);
alt_u64 read_xge_counter_rx_crc(xge_mac_registers_t* _mac);
alt_u64 read_xge_counter_rx_ok(xge_mac_registers_t* _mac);
alt_u64 read_xge_counter_tx_ok(xge_mac_registers_t* _mac);
#if XGE_MAC_VER == XGE_MAC_V2
alt_u64 read_xge_counter_rx_octs(xge_mac_registers_t* _mac);
alt_u64 read_xge_counter_tx_octs(xge_mac_registers_t* _mac);
alt_u64 read_xge_counter_tx_crc(xge_mac_registers_t* _mac);
#endif
#endif	//_XGE_MAC_H_
