/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file odu2_wrapper.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 5, 2015
 * @brief Contains prototypes of functions implemented in odu2_wrapper.c as well as registers and bit definitions for the ODU2 wrapper.
 *
 */
#ifndef _ODU2_WRAPPER_H_
#define _ODU2_WRAPPER_H_

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

/**Bit definitions for ODU2_WRAPPER_GENERAL_CTRL_REG.
 *
 */
struct ODU2_WRAPPER_GENERAL_CTRL_BITS{
	volatile unsigned int  RESERVED_1:2;		//!<b'1:0 reserved for future use
	volatile unsigned int  TX_AIS:1;			//!<b'2
	volatile unsigned int  RESERVED_2:3;		//!<b'5:3 reserved for future use
	volatile unsigned int  PL_SMOOTH_DIS:1;		//!<b'6 TBA
	volatile unsigned int  RESERVED_3:3;		//!<b'9:7 reserved for future use
	volatile unsigned int  GFP_MAPPER_EN:1;		//!<b'10 RW, when set to 1, the GFP mapper function is enabled
	volatile unsigned int  RESERVED_4:10;		//!<b'20:11 reserved for future use
	volatile unsigned int  GFP_PAUSE_COUNT:4;	//!<b'24:21 RW, GFP pause count
	volatile unsigned int  RESERVED_5:7;		//!<b'31:25 reserved for future use
};
/**Allows register access for ODU2_WRAPPER_GENERAL_CTRL_REG.
 * Located at \b 0x00 from ODU2 wrapper base.
 */
union ODU2_WRAPPER_GENERAL_CTRL_REG{
	alt_u32 all;								//!<used to write or read from the whole register
	struct ODU2_WRAPPER_GENERAL_CTRL_BITS bits;	//!<used for bit access
};

/**Bit definitions for ODU2_WRAPPER_LIVE_STS_REG.
 *
 */
struct ODU2_WRAPPER_LIVE_STS_BITS{
	volatile unsigned int  OOF:1;			//!<b'0 RO, live alarm status for OOF
	volatile unsigned int  LOFLOM:1;		//!<b'1 RO, live alarm status for LOF/LOM
	volatile unsigned int  PM_BIP8_ERR:1;	//!<b'2 RO
	volatile unsigned int  PM_BDI:1;		//!<b'3 RO
	volatile unsigned int  RESERVED_1:11;	//!<b'14:4 RO reserved for future use
	volatile unsigned int  RX_AIS:1;		//!<b'15 RO, live alarm status for RX AIS
	volatile unsigned int  RESERVED_2:16;	//!<b'31:16 reserved for future use
};
/**Allows register access for ODU2_WRAPPER_GENERAL_CTRL_REG.
 * Located at \b 0x00 from ODU2 wrapper base.
 */
union ODU2_WRAPPER_LIVE_STS_REG{
	alt_u32 all;							//!<used to write or read from the whole register
	struct ODU2_WRAPPER_LIVE_STS_BITS bits;	//!<used for bit access
};
/**Bit definitions for ODU2_WRAPPER_TX_EXPSTATPT_REG.
 *
 */
struct ODU2_WRAPPER_TX_EXPSTATPT_BITS{
	volatile unsigned int  RESERVED_1:12;	//!<b'11:0 RO reserved for future use
	volatile unsigned int  TXCSF:1;			//!<b'12 RO
	volatile unsigned int  RESERVED_2:3;	//!<b'15:13 reserved for future use
	volatile unsigned int  TxEXP:1;			//!<b'16
	volatile unsigned int  RESERVED_3:15;	//!<b'31:17 reserved for future use
};
/**Allows register access for ODU2_WRAPPER_TX_EXPSTATPT_REG.
 * Located at \b 0x20 from ODU2 wrapper base.
 */
union ODU2_WRAPPER_TX_EXPSTATPT_REG{
	alt_u32 all;							//!<used to write or read from the whole register
	struct ODU2_WRAPPER_TX_EXPSTATPT_BITS bits;	//!<used for bit access
};
/**Bit definitions for ODU2_WRAPPER_RX_EXPSTATPT_REG.
 *
 */
struct ODU2_WRAPPER_RX_EXPSTATPT_BITS{
	volatile unsigned int  RESERVED_1:12;	//!<b'11:0 RO reserved for future use
	volatile unsigned int  RXCSF:1;			//!<b'12 RO
	volatile unsigned int  RESERVED_2:3;	//!<b'15:13 reserved for future use
	volatile unsigned int  RxEXP:1;			//!<b'16
	volatile unsigned int  RESERVED_3:15;		//!<b'31:17 reserved for future use
};
/**Allows register access for ODU2_WRAPPER_RX_EXPSTATPT_REG.
 * Located at \b 0x24 from ODU2 wrapper base.
 */
union ODU2_WRAPPER_RX_EXPSTATPT_REG{
	alt_u32 all;							//!<used to write or read from the whole register
	struct ODU2_WRAPPER_RX_EXPSTATPT_BITS bits;	//!<used for bit access
};

/**Bit definitions for ODU2_WRAPPER_INT_STS_REG.
 *
 */
struct ODU2_WRAPPER_INT_STS_BITS{
	volatile unsigned int  SM_BDI_ASSERT:1;		//!<b'0 RO, interrupt alarm status for SM BDI, set to 1 when SM BDI is asserted
	volatile unsigned int  SM_BDI_DEASSERT:1;	//!<b'1 RO, interrupt alarm status for SM BDI, set to 1 when SM BDI is de-asserted
	volatile unsigned int  PM_BDI_ASSERT:1;		//!<b'2 RO, interrupt alarm status for PM BDI, set to 1 when PM BDI is asserted
	volatile unsigned int  PM_BDI_DEASSERT:1;	//!<b'3 RO, interrupt alarm status for PM BDI, set to 1 when PM BDI is de-asserted
	volatile unsigned int  IAE_ASSERT:1;		//!<b'4 RO, interrupt alarm status for IAE, set to 1 when IAE is asserted
	volatile unsigned int  LOM:1;				//!<b'5 RO, interrupt alarm status for LOM
	volatile unsigned int  BIAE_ASSERT:1;		//!<b'6 RO, interrupt alarm status for BIAE, set to 1 when BIAE is asserted
	volatile unsigned int  BIAE_DEASSERT:1;		//!<b'7 RO, interrupt alarm status for BIAE, set to 1 when BIAE is de-asserted
	volatile unsigned int  ODU_AIS_ASSERT:1;	//!<b'8 RO, interrupt alarm status for ODU AIS, set to 1 when ODU AIS is asserted
	volatile unsigned int  ODU_AIS_DEASSERT:1;	//!<b'9 RO, interrupt alarm status for ODU AIS, set to 1 when ODU AIS is de-asserted
	volatile unsigned int  ODU_LCK_ASSERT:1;	//!<b'10 RO, interrupt alarm status for ODU LCK, set to 1 when ODU LCK is asserted
	volatile unsigned int  ODU_LCK_DEASSERT:1;	//!<b'11 RO, interrupt alarm status for ODU LCK, set to 1 when ODU LCK is de-asserted
	volatile unsigned int  ODU_OCI_ASSERT:1;	//!<b'12 RO, interrupt alarm status for ODU OCI, set to 1 when ODU OCI is asserted
	volatile unsigned int  ODU_OCI_DEASSERT:1;	//!<b'13 RO, interrupt alarm status for ODU OCI, set to 1 when ODU OCI is de-asserted
	volatile unsigned int  GDAIS_ASSERT:1;		//!<b'14 RO, interrupt alarm status for GDAIS, set to 1 when GDAIS is asserted
	volatile unsigned int  GDAIS_DEASSERT:1;	//!<b'15 RO, interrupt alarm status for GDAIS, set to 1 when GDAIS is de-asserted
	volatile unsigned int  LOF_ASSERT:1;		//!<b'16 RO, interrupt alarm status for LOF, set to 1 when LOF is asserted
	volatile unsigned int  LOF_DEASSERT:1;		//!<b'17 RO, interrupt alarm status for LOF, set to 1 when LOF is de-asserted
	volatile unsigned int  OOF_ASSERT:1;		//!<b'18 RO, interrupt alarm status for OOF, set to 1 when OOF is asserted
	volatile unsigned int  OOF_DEASSERT:1;		//!<b'19 RO, interrupt alarm status for OOF, set to 1 when OOF is de-asserted
	volatile unsigned int  LOS_ASSERT:1;		//!<b'20 RO, interrupt alarm status for LOS, set to 1 when LOS is asserted
	volatile unsigned int  LOS_DEASSERT:1;		//!<b'21 RO, interrupt alarm status for LOS, set to 1 when LOS is de-asserted
	volatile unsigned int  GCC0_F:1;			//!<b'22 TBA
	volatile unsigned int  GCC1_F:1;			//!<b'23 TBA
	volatile unsigned int  GCC2_F:1;			//!<b'24 TBA
	volatile unsigned int  OOM:1;				//!<b'25 RO, interrupt alarm status for OOM
	volatile unsigned int  TX_OVERFLOW:1;		//!<b'26 TBA
	volatile unsigned int  RX_OVERFLOW:1;		//!<b'27 TBA
	volatile unsigned int  SM_BIP8_ERROR:1;		//!<b'28 TBA
	volatile unsigned int  PM_BIP8_ERROR:1;		//!<b'29 TBA
	volatile unsigned int  SM_BEI_ALARM:1;		//!<b'30 TBA
	volatile unsigned int  PM_BEI_ALARM:1;		//!<b'31 TBA
};
/**Allows register access for ODU2_WRAPPER_INT_STS_REG.
 * Located at \b 0x7C from ODU2 wrapper base.
 * @note this is a Clear On Read register
 */
union ODU2_WRAPPER_INT_STS_REG{
	alt_u32 all;							//!<used to write or read from the whole register
	struct ODU2_WRAPPER_INT_STS_BITS bits;	//!<used for bit access
};

/**Bit definitions for ODU2_WRAPPER_INT_STS_REG.
 *
 */
struct ODU2_WRAPPER_INT_EN_BITS{
	volatile unsigned int  EN_SM_BDI_ASSERT:1;		//!<b'0 RW, interrupt enable for SM BDI assert
	volatile unsigned int  EN_SM_BDI_DEASSERT:1;	//!<b'1 RW, interrupt enable for SM BDI de-assert
	volatile unsigned int  EN_PM_BDI_ASSERT:1;		//!<b'2 RW, interrupt enable for PM BDI assert
	volatile unsigned int  EN_PM_BDI_DEASSERT:1;	//!<b'3 RW, interrupt enable for PM BDI de-assert
	volatile unsigned int  EN_IAE_ASSERT:1;			//!<b'4 RW, interrupt enable for IAE assert
	volatile unsigned int  EN_LOM:1;				//!<b'5 RW, interrupt enable for LOM
	volatile unsigned int  EN_BIAE_ASSERT:1;		//!<b'6 RW, interrupt enable for BIAE assert
	volatile unsigned int  EN_BIAE_DEASSERT:1;		//!<b'7 RW, interrupt enable for BIAE de-assert
	volatile unsigned int  EN_ODU_AIS_ASSERT:1;		//!<b'8 RW, interrupt enable for ODU AIS assert
	volatile unsigned int  EN_ODU_AIS_DEASSERT:1;	//!<b'9 RW, interrupt enable for ODU AIS de-assert
	volatile unsigned int  EN_ODU_LCK_ASSERT:1;		//!<b'10 RW, interrupt enable for ODU LCK assert
	volatile unsigned int  EN_ODU_LCK_DEASSERT:1;	//!<b'11 RW, interrupt enable for ODU LCK de-assert
	volatile unsigned int  EN_ODU_OCI_ASSERT:1;		//!<b'12 RW, interrupt enable for ODU OCI assert
	volatile unsigned int  EN_ODU_OCI_DEASSERT:1;	//!<b'13 RW, interrupt enable for ODU OCI de-assert
	volatile unsigned int  EN_GDAIS_ASSERT:1;		//!<b'14 RW, interrupt enable for GDAIS assert
	volatile unsigned int  EN_GDAIS_DEASSERT:1;		//!<b'15 RW, interrupt enable for GDAIS de-assert
	volatile unsigned int  EN_LOF_ASSERT:1;			//!<b'16 RW, interrupt enable for LOF assert
	volatile unsigned int  EN_LOF_DEASSERT:1;		//!<b'17 RW, interrupt enable for LOF de-assert
	volatile unsigned int  EN_OOF_ASSERT:1;			//!<b'18 RW, interrupt enable for OOF assert
	volatile unsigned int  EN_OOF_DEASSERT:1;		//!<b'19 RW, interrupt enable for OOF de-assert
	volatile unsigned int  EN_LOS_ASSERT:1;			//!<b'20 RW, interrupt enable for LOS assert
	volatile unsigned int  EN_LOS_DEASSERT:1;		//!<b'21 RW, interrupt enable for LOS de-assert
	volatile unsigned int  EN_GCC0_F:1;				//!<b'22 TBA
	volatile unsigned int  EN_GCC1_F:1;				//!<b'23 TBA
	volatile unsigned int  EN_GCC2_F:1;				//!<b'24 TBA
	volatile unsigned int  EN_OOM:1;				//!<b'25 RW, interrupt enable for LOS de-assert
	volatile unsigned int  EN_TX_OVERFLOW:1;		//!<b'26 TBA
	volatile unsigned int  EN_RX_OVERFLOW:1;		//!<b'27 TBA
	volatile unsigned int  EN_SM_BIP8_ERROR:1;		//!<b'28 TBA
	volatile unsigned int  EN_PM_BIP8_ERROR:1;		//!<b'29 TBA
	volatile unsigned int  EN_SM_BEI_ALARM:1;		//!<b'30 TBA
	volatile unsigned int  EN_PM_BEI_ALARM:1;		//!<b'31 TBA
};
/**Allows register access for ODU2_WRAPPER_INT_EN_REG.
 * Located at \b 0x0C from ODU2 wrapper base.
 */
union ODU2_WRAPPER_INT_EN_REG{
	alt_u32 all;							//!<used to write or read from the whole register
	struct ODU2_WRAPPER_INT_EN_BITS bits;	//!<used for bit access
};

/**ODU2 wrapper registers.
 *
 */
typedef struct{
	volatile union ODU2_WRAPPER_LIVE_STS_REG* live_sts_reg;				// 0x00 from ODU2
	volatile union ODU2_WRAPPER_INT_STS_REG* int_sts_reg;				// 0x04 from ODU2
	volatile union ODU2_WRAPPER_INT_EN_REG* int_en_reg;					// 0x08 from ODU2
	volatile union ODU2_WRAPPER_GENERAL_CTRL_REG* general_ctrl_reg;		// 0x0C from ODU2
	volatile union ODU2_WRAPPER_TX_EXPSTATPT_REG* tx_epxstat_reg;		// 0x20 from ODU2
	volatile union ODU2_WRAPPER_RX_EXPSTATPT_REG* rx_epxstat_reg;		// 0x24 from ODU2
	volatile unsigned int* bip8_cnt_reg;								// 0x2C from ODU2

}odu2_wrapper_registers_t;

typedef struct{
	odu2_wrapper_registers_t regs;
}odu2_wrapper_t;

#define map_odu2_wrapper_registers(x, base) __generic(x, odu2_wrapper_t, map_odu2_wrapper_regs, map_odu2_wrapper_regs_ol)

void map_odu2_wrapper_regs(odu2_wrapper_t* wrapper, alt_u32 base);
void map_odu2_wrapper_regs_ol(odu2_wrapper_registers_t* wrapper, alt_u32 base);
odu2_wrapper_t* create_odu2_wrapper_regs(alt_u32 base);
void init_odu2_wrapper(odu2_wrapper_t* wrapper);
void enable_odu2_wrapper_gfp_mapping(odu2_wrapper_t* wrapper);
void set_odu2_wrapper_gfp_pause_count(odu2_wrapper_t* wrapper);

#endif /* _ODU2_WRAPPER_H_ */
