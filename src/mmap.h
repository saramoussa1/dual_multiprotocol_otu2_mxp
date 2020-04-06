/*
 * mmap.h
 *
 *  Created on: Jun 26, 2018
 *      Author: dcham
 */

#ifndef MMAP_H_
#define MMAP_H_


#include <system.h>
#include "common/inc/ipg_ge_mac.h"
#include "common/inc/ipg_ge_pcs.h"
#include "common/inc/ipg_gfp.h"
#include "common/inc/stm1416.h"
#include "ipglb_otu2_brick_dual.h"

#define ODTUG2_BASE_1 ODUK_MPIF_BRICK_0_BASE + 0x24000
#define ODTUG2_BASE_2 0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x24000

#define OTU2_BRICK_BASE_1	ODUK_MPIF_BRICK_0_BASE + 0x40000
#define OTU2_BRICK_BASE_2   0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x40000

//Containing all the GE/STM16...
#define ODTU_BRICK_SPAN			0x4000
#define STM1_4_FRAMER_OFFSET	0x400
#define STM16_FRAMER_OFFSET		0x800
#define GE_MAC_OFFSET   		0x1000
#define GFP_FRAMER_OFFSET      	0x1400
#define GE_BRICK_OFFSET     	0x1440

#define ODTU_BRICK_0_BASE_1			ODUK_MPIF_BRICK_0_BASE + ODTU_BRICK_SPAN
#define ODTU_GE_MAC_BASE_1			ODTU_BRICK_0_BASE_1 + GE_MAC_OFFSET
#define ODTU_GE_BRICK_BASE_1		ODTU_BRICK_0_BASE_1 + GE_BRICK_OFFSET
#define ODTU_GFP_FRAMER_BASE_1		ODTU_BRICK_0_BASE_1 + GFP_FRAMER_OFFSET
#define ODTU_STM1_4_FRAMER_BASE_1	ODTU_BRICK_0_BASE_1 + STM1_4_FRAMER_OFFSET
#define ODTU_STM16_FRAMER_BASE_1	ODTU_BRICK_0_BASE_1 + STM16_FRAMER_OFFSET

#define ODTU_BRICK_0_BASE_2		    0x10000000 + ODUK_MPIF_BRICK_0_BASE + ODTU_BRICK_SPAN //Containing all the GE/STM16...
#define ODTU_GE_MAC_BASE_2			0x10000000 + ODTU_BRICK_0_BASE_1 + GE_MAC_OFFSET
#define ODTU_GE_BRICK_BASE_2		0x10000000 + ODTU_BRICK_0_BASE_1 + GE_BRICK_OFFSET
#define ODTU_GFP_FRAMER_BASE_2		0x10000000 + ODTU_BRICK_0_BASE_1 + GFP_FRAMER_OFFSET
#define ODTU_STM1_4_FRAMER_BASE_2	0x10000000 + ODTU_BRICK_0_BASE_1 + STM1_4_FRAMER_OFFSET
#define ODTU_STM16_FRAMER_BASE_2	0x10000000 + ODTU_BRICK_0_BASE_1 + STM16_FRAMER_OFFSET

/*
 * GE brick registers
 */
/*
 * 1- GE general status register
 */
struct BRICK_GE_GENERAL_STS_BITS
{
 volatile unsigned int  GE_LINK:1;        /* bit 0 */
 volatile unsigned int  :5;    	  		  /* bit 1 to bit 5 */
 volatile unsigned int  LED_RX_ON:1;   	  /* bit 6 */
 volatile unsigned int  LED_TX_ON:1;      /* bit 7 */
 volatile unsigned int  :24;    	      /* bit 8 to bit 31 */
};
union BRICK_GE_GENERAL_STS_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct BRICK_GE_GENERAL_STS_BITS bits;
};

/*
 * 2- GE general control register
 */
struct BRICK_GE_GENERAL_CTRL_BITS
{
 volatile unsigned int  GE_LB_EN:1;     	/* bit 0 */
 volatile unsigned int  FC_EN:1;      		/* bit 1 */
 volatile unsigned int  :9;     			/* bit 2 to bit 10 */
 volatile unsigned int  FORCE_GFP_EN:1;   	/* bit 11 */
 volatile unsigned int  :20;     			/* bit 12 to bit 31 */
};
union BRICK_GE_GENERAL_CTRL_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct BRICK_GE_GENERAL_CTRL_BITS bits;
};

/*
 * 3- GE reset control register
 */
struct BRICK_GE_RESET_CTRL_BITS
{
	volatile unsigned int  :1;    			/* bit 0 */
	volatile unsigned int  GE_MAC_RST:1;    /* bit 1 */
	volatile unsigned int  GFP_RST:1;       /* bit 2 */
	volatile unsigned int  :29;    			/* bit 3 to bit 31 */
};
union BRICK_GE_RESET_CTRL_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct BRICK_GE_RESET_CTRL_BITS bits;
};

/*GE brick at ODUK_MPIF_BRICK_0_BASE + 0x2000 + (0x2000 * X) + 0x1440 where X
 *can be from 0 to 7
 */
typedef struct
{
	/* 0x00 from ODUK_MPIF_BRICK_0_BASE + (0x4000 * X) + 0x1440
	 * where X can be from 0 to 7 */
	 volatile union BRICK_GE_GENERAL_STS_REG* general_sts_reg_1;
	 /* 0x00 from 0x11000000 + ODUK_MPIF_BRICK_0_BASE + (0x4000 * X) + 0x1440
	 * where X can be from 0 to 7 */
	 volatile union BRICK_GE_GENERAL_STS_REG* general_sts_reg_2;
	 /* 0x04 from ODUK_MPIF_BRICK_0_BASE + (0x4000 * X) + 0x1440
	  * where X can be from 0 to 7 */
	 volatile union BRICK_GE_GENERAL_CTRL_REG* general_ctrl_reg_1;
	 /* 0x04 from 0x11000000 + ODUK_MPIF_BRICK_0_BASE + (0x4000 * X) + 0x1440
	  * where X can be from 0 to 7 */
	 volatile union BRICK_GE_GENERAL_CTRL_REG* general_ctrl_reg_2;
	 /* 0x08 from ODUK_MPIF_BRICK_0_BASE + (0x4000 * X) + 0x1440
	  * where X can be from 0 to 7 */
	 volatile union BRICK_GE_RESET_CTRL_REG* reset_ctrl_reg_1;
	 /* 0x08 from 0x11000000 + ODUK_MPIF_BRICK_0_BASE + (0x4000 * X) + 0x1440
	  * where X can be from 0 to 7 */
	 volatile union BRICK_GE_RESET_CTRL_REG* reset_ctrl_reg_2;
}ge_brick_t;

/*
 * GMP brick registers
 */
/*
 * 1- general config register
 */
struct GMP_ALIGNER_GENERAL_CFG_BITS
{
 volatile unsigned int  :31;        	  /* bit 0 to Bit 30 */
 volatile unsigned int  SW_RESET:1;   	  /* bit 31 */
};
union GMP_ALIGNER_GENERAL_CFG_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct GMP_ALIGNER_GENERAL_CFG_BITS bits;
};

typedef struct
{
	 volatile union GMP_ALIGNER_GENERAL_CFG_REG* general_cfg_reg;
}gmp_aligner_t;

/*todo remove otu1 section and replace it with odtu */
/**Bit definitions for OTU1_BRICK_CTRL_REG.
 *
 */
struct OTU1_BRICK_CTRL_BITS{
	volatile unsigned int	PORT_TX_DIS:1;		/*!<b'0 */
	volatile unsigned int	SFP_PWR_OFF:1;		/*!<b'1 */
	volatile unsigned int	PORT_RS:2;			/*!<b'3:2 */
	volatile unsigned int	LED_RX_OFF:1;		/*!<b'4 */
	volatile unsigned int	LED_TX_OFF:1;		/*!<b'5 */
	volatile unsigned int	LOS_DBNCE_DIS:1;	/*!<b'6 */
	volatile unsigned int	ABSCENT_DBNCE_DIS:1;/*!<b'7 */
	volatile unsigned int	GXB_RST:1;			/*!<b'8 */
	volatile unsigned int	FORCE_LTR:1;		/*!<b'9 */
	volatile unsigned int	:1;					/*!<b'10 */
	volatile unsigned int	DP_FORCE_RST:1;		/*!<b'11 */
	volatile unsigned int	ENABLE_LTD:1;		/*!<b'12 */
	volatile unsigned int	RX_ARST:1;			/*!<b'13 */
	volatile unsigned int	:1;					/*!<b'14 */
	volatile unsigned int	LNS_RST_DISABLE:1;	/*!<b'15 */
	volatile unsigned int	:16;				/*!<b'31:16 */
};
/**Allows register access for OTU1_BRICK_CTRL_REG.
 */
union OTU1_BRICK_CTRL_REG{
	alt_u32 all;/* used to write or read from the whole register */
	struct OTU1_BRICK_CTRL_BITS bits;
};


/**Bit definitions for OTU1_BRICK_STATUS_REG.
 *
 */
struct OTU1_BRICK_STATUS_BITS{
	volatile unsigned int	PORT_TX_FAULT:1;	/*!<b'0 */
	volatile unsigned int	:4;					/*!<b'4:1 */
	volatile unsigned int	PORT_LOS:1;			/*!<b'5 */
	volatile unsigned int	PORT_ABS:1;			/*!<b'6 */
	volatile unsigned int	:6;					/*!<b'12:7 */
	volatile unsigned int	LN_INC:1;			/*!<b'13 */
	volatile unsigned int	LN_DEC:1;			/*!<b'14 */
	volatile unsigned int	CL0_INC:1;			/*!<b'15 */
	volatile unsigned int	CL0_DEC:1;			/*!<b'16 */
	volatile unsigned int	CL1_INC:1;			/*!<b'17 */
	volatile unsigned int	CL1_DEC:1;			/*!<b'18 */
	volatile unsigned int	:13;				/*!<b'31:19 */

};
/**Allows register access for OTU1_BRICK_STATUS_REG.
 */
union OTU1_BRICK_STATUS_REG{
	alt_u32 all;/* used to write or read from the whole register */
	struct OTU1_BRICK_STATUS_BITS bits;
};

/*
 * ODTU brick registers
 */
/*
 * 1- ODTU control register
 */
struct ODTU_CTRL_BITS
{
 volatile unsigned int  PORT_TX_DIS:1;    	// bit 0
 volatile unsigned int  SFP_PWR_OFF:1;    	// bit 1
 volatile unsigned int  :14;    	      	// bit 2 to bit 7
 volatile unsigned int  FORCE_LTR:1;    	// bit 8
 volatile unsigned int  :7;    	      	    // bit 9 to bit 15
 volatile unsigned int  RESET_GXB:1;      	// bit 16
 volatile unsigned int  :2;    	      		// bit 17 to bit 18
 volatile unsigned int  RX_SERIAL_LB_EN:1;	// bit 19
 volatile unsigned int  :9;    	      		// bit 20 to bit 28
 volatile unsigned int  ODU1_RST:1;    	  	// bit 29
 volatile unsigned int  :1;    	      	  	// bit 30
 volatile unsigned int  ODTU12_RST:1;     	// bit 31
};
union ODTU_CTRL_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct ODTU_CTRL_BITS bits;
};

/*
 * 2- ODTU status register
 */
struct ODTU_STATUS_BITS
{
 volatile unsigned int  SFP_TX_FAULT:1;     // bit 0
 volatile unsigned int  :1;     			// bit 1
 volatile unsigned int  SFP_LOS:1;      	// bit 2
 volatile unsigned int  SFP_ABSENT:1;      	// bit 3
 volatile unsigned int  :12;      			// bit 4 to bit 15
 volatile unsigned int  PPM_INCR:1;      	// bit 16
 volatile unsigned int  PPM_DECR:1;     	// bit 17
 volatile unsigned int  :14;     			// bit 18 to bit 31
};
union ODTU_STATUS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct ODTU_STATUS_BITS bits;
};


/**Bit definitions for \c ALS_CTRL_BITS.
 */
struct ALS_CTRL_BITS{
	/**ALS Hold restart time in seconds
	 * bit 7:0 RW
	 */
	volatile unsigned int 	ALS_HOLD_RES_SEC:8;				//!<b'7:0
	/**Reserved for future use.
	 * bit 8 RO
	 */
	volatile unsigned int  	RESERVED_1:1;					//!<b'8
	volatile unsigned int  	ALS_MANUAL_RESTART:1;			//!<b'9
	volatile unsigned int  	ALS_AUTO_ENABLE:1;				//!<b'10
	volatile unsigned int  	ALS_ENABLE:1;					//!<b'11
	volatile unsigned int 	ALS_RES_PULSE_MS:17;			//!<b'28:12
	volatile unsigned int 	ALS_MODE:2;						//!<b'30:29
	volatile unsigned int   ALS_TX_DIS:1;					//!<b'31
};

/**Allows register access for ALS_CTRL_REG.
 * Maps to register 0x28 from brick top base address.
 */
union ALS_CTRL_REG{
	alt_u32 all;									//!<Used to read/write the whole register
	struct ALS_CTRL_BITS bits;    //!<Used for bit access
};

typedef struct
{
	 /* 0x00 from ODUK_MPIF_BRICK_0_BASE + (0x4000 * X)  where X can be from 0 to 7*/
	 volatile union ODTU_CTRL_REG* odtu_ctrl_reg_1;
	 /* 0x00 from 0x11000000 + ODUK_MPIF_BRICK_0_BASE + (0x4000 * X)  where X can be from 0 to 7*/
	 volatile union ODTU_CTRL_REG* odtu_ctrl_reg_2;
	 /* 0x04 from ODUK_MPIF_BRICK_0_BASE + (0x4000 * X)  where X can be from 0 to 7*/
	 volatile union ODTU_STATUS_REG* odtu_sts_reg_1;
	 /* 0x04 from 0x11000000 + ODUK_MPIF_BRICK_0_BASE  + (0x4000 * X)  where X can be from 0 to 7*/
	 volatile union ODTU_STATUS_REG* odtu_sts_reg_2;
	 /* 0x44 from ODUK_MPIF_BRICK_0_BASE  + (0x4000 * X)  where X can be from 0 to 7*/
	 volatile union ALS_CTRL_REG*  als_ctrl_reg_1;
	 /* 0x44 from 0x11000000 + ODUK_MPIF_BRICK_0_BASE  + (0x4000 * X)  where X can be from 0 to 7*/
	 volatile union ALS_CTRL_REG*  als_ctrl_reg_2;
}odtu_brick_register_t;


/**Bit definitions for OTU1_RESET_CTRL_REG.
 *
 */
struct OTU1_RESET_CTRL_BITS{
	volatile unsigned int	OTU1_RESET:1;				/*!<b'0 */
	volatile unsigned int	LED_RESET:1;				/*!<b'1 */
	volatile unsigned int	SERIAL_LB:1;				/*!<b'2 */
	volatile unsigned int	:13;						/*!<b'15:3 */
	volatile unsigned int	DEBNCE_PERIOD:10;			/*!<b'25:16 */
	volatile unsigned int	INSERT_TIMEOUT_PERIOD:6;	/*!<b'31:26 */
};
/**Allows register access for OTU1_RESET_CTRL_REG.
 */
union OTU1_RESET_CTRL_REG{
	alt_u32 all;/* used to write or read from the whole register */
	struct OTU1_RESET_CTRL_BITS bits;
};
/**Bit definitions for OTU1_LNTX_CLRX_REG.
 *
 */
struct OTU1_LNTX_CLRX_BITS{
	volatile unsigned int	LI_TX_PPM:16;				/*!<b'15:0 */
	volatile unsigned int	CL_RX_PPM:16;				/*!<b'31:16 */
};
/**Allows register access for OTU1_LNTX_CLRX_REG.
 */
union OTU1_LNTX_CLRX_REG{
	alt_u32 all;/* used to write or read from the whole register */
	struct OTU1_LNTX_CLRX_BITS bits;
};

/**Bit definitions for OTU1_LNRX_CLTX_REG.
 *
 */
struct OTU1_LNRX_CLTX_BITS{
	volatile unsigned int	LI_RX_PPM:16;				/*!<b'15:0 */
	volatile unsigned int	CL_TX_PPM:16;				/*!<b'31:16 */
};
/**Allows register access for OTU1_LNRX_CLTX_REG.
 */
union OTU1_LNRX_CLTX_REG{
	alt_u32 all;/* used to write or read from the whole register */
	struct OTU1_LNRX_CLTX_BITS bits;
};

typedef struct{
	/*!< offset 0x00 from ODUK_MPIF_BRICK_0_BASE + ( 0x4000(FOR BRICK 0) OR
	 * 0x10000(FOR BRICK1))*/
	volatile union OTU1_BRICK_CTRL_REG* 			ctrl_reg;
	/*!< offset 0x04 from ODUK_MPIF_BRICK_0_BASE + ( 0x8000(FOR BRICK 0) OR
	 * 0x10000(FOR BRICK1) */
	volatile union OTU1_BRICK_STATUS_REG* 			status_reg;
	/*!< offset 0x08 from ODUK_MPIF_BRICK_0_BASE + ( 0x8000(FOR BRICK 0) OR
	 * 0x10000(FOR BRICK1))*/
	volatile union OTU1_RESET_CTRL_REG* 			reset_ctrl_reg;
	/*!< offset 0x0C from ODUK_MPIF_BRICK_0_BASE + ( 0x8000(FOR BRICK 0) OR
	 *  0x10000(FOR BRICK1))*/
	volatile union OTU1_LNTX_CLRX_REG* 				lntx_clrx_reg;
	/*!< offset 0x10 from ODUK_MPIF_BRICK_0_BASE + ( 0x8000(FOR BRICK 0) OR
	 * 0x10000(FOR BRICK1))*/
	volatile union OTU1_LNRX_CLTX_REG* 				lnrx_cltx_reg;
}otu1_registers_t;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**Bit definitions for OTU1_MAPPER_LN_CFG_REG.
 *
 */
//struct OTU1_MAPPER_LN_CFG_BITS{
//	volatile unsigned int   :2;     			/*!<b'1:0 */
//	volatile unsigned int	RST_IN_OUT_FIFO:2;	/*!<b'3:2 */
//	volatile unsigned int	:20;				/*!<b'23:4 */
//	volatile unsigned int  	FRAMER_MODE:2;  	/*!<b'25:24 */
//	volatile unsigned int	FC2_MODE:1;			/*!<b'26 */
//	volatile unsigned int   :1;     			/*!<b'27 */
//	volatile unsigned int	PFC_EOP:1;			/*!<b'28 */
//	volatile unsigned int   :1;     			/*!<b'29 */
//	volatile unsigned int	ENC_RST:1;			/*!<b'30 */
//	volatile unsigned int	SW_RST:1;			/*!<b'31 */
//
//};
///**Allows register access for OTU1_MAPPER_LN_CFG_REG.
// * Located at \b 0x00 from ODUK_MPIF_BRICK_0_BASE + (0x40000*i) + 0x2000.
// */
//union OTU1_MAPPER_LN_CFG_REG{
//	alt_u32 all;/* used to write or read from the whole register */
//	struct OTU1_MAPPER_LN_CFG_BITS bits;
//};
/**Bit definitions for OTU1_MAPPER_LN_CFG1_REG.
 *
 */
//struct OTU1_MAPPER_LN_CFG1_BITS{
//	volatile unsigned int	:23;			/*!<b'22:0 */
//	volatile unsigned int	LOAD_GCC2:1;	/*!<b'23 */
//	volatile unsigned int  	ENABLE_APS:1;  	/*!<b'24 */
//	volatile unsigned int   :7;     		/*!<b'31:25 */
//};
///**Allows register access for OTU1_MAPPER_LN_CFG1_REG.
// * Located at \b 0x04 from ODUK_MPIF_BRICK_0_BASE + (0x40000*i) + 0x2000.
// */
//union OTU1_MAPPER_LN_CFG1_REG{
//	alt_u32 all;/* used to write or read from the whole register */
//	struct OTU1_MAPPER_LN_CFG1_BITS bits;
//};



/**Bit definitions for OTU1_LIVE_STS_REG.
 *
 */
struct OTU1_LIVE_STS_BITS
{
	 volatile unsigned int  :1;    			 			/*!<b'0 */
	 volatile unsigned int  L_LOF:1;  					/*!<b'1 */
	 volatile unsigned int  :1;    			 			/*!<b'2 */
	 volatile unsigned int  L_AIS:1;    			 	/*!<b'3 */
	 volatile unsigned int  L_OCI:1;    			 	/*!<b'4 */
	 volatile unsigned int  L_LCK:1;    			 	/*!<b'5 */
	 volatile unsigned int  :4;  						/*!<b'9:6 */
	 volatile unsigned int  L_SBDI:1;  					/*!<b'10 */
	 volatile unsigned int  PM_BDI:1;  					/*!<b'11 */
	 volatile unsigned int  SM_IAE:1;  					/*!<b'12 */
	 volatile unsigned int  SM_BEI_BIAE:1;  			/*!<b'13 */
	 volatile unsigned int  :1;  						/*!<b'14 */
	 volatile unsigned int  L_LOM:1;  					/*!<b'15 */
	 volatile unsigned int  :16;  						/*!<b'31:16 */

};
/**Allows register access for OTU1_LIVE_STS_REG.
 * Located at \b ODUK_MPIF_BRICK_0_BASE + (0x40000*i) + 0x20000 + 0x20
 */
union OTU1_LIVE_STS_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct OTU1_LIVE_STS_BITS bits;
};
/**Bit definitions for OTU1_INT_STS_REG.
 *
 */
struct OTU1_INT_STS_BITS
{
	 volatile unsigned int  :1;    			 			/*!<b'0 */
	 volatile unsigned int  L_LOF:1;  					/*!<b'1 */
	 volatile unsigned int  :8;  						/*!<b'9:2 */
	 volatile unsigned int  L_SBDI:1;  					/*!<b'10 */
	 volatile unsigned int  :3;     					/*!<b'13:11 */
	 volatile unsigned int  SM_BIP:1;  					/*!<b'14 */
	 volatile unsigned int  PM_BIP:1;  					/*!<b'15 */
	 volatile unsigned int  :1;     					/*!<b'16 */
	 volatile unsigned int  L_LOM:1;     				/*!<b'17 */
	 volatile unsigned int  :5;     					/*!<b'22:18 */
	 volatile unsigned int  PM_BEI:1;     				/*!<b'23 */
	 volatile unsigned int  :8;     					/*!<b'31:24 */
};
/**Allows register access for OTU1_INT_STS_REG.
 * Located at ODUK_MPIF_BRICK_0_BASE + (0x40000*i) + 0x2000 + 0x68
 */
union OTU1_INT_STS_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct OTU1_INT_STS_BITS bits;
};

/**Bit definitions for ODU1_INT_STS_REG.
 *
 */
//struct ODU1_INT_STS_BITS
//{
//	 volatile unsigned int  :15;    			 		/*!<b'14:0 */
//	 volatile unsigned int  PM_BIP:1;  					/*!<b'15 */
//	 volatile unsigned int  :16;     					/*!<b'31:16 */
//};
//union ODU1_INT_STS_REG
//{
// alt_u32 all;/* used to write or read from the whole register */
// struct ODU1_INT_STS_BITS bits;
//};
/**Bit definitions for OTU1_SM_BIP8_REG.
 *
 */
struct OTU1_SM_BIP8_BITS
{
	 volatile unsigned int  :32;    			 		/*!<b'31:0 */
};
/**Allows register access for OTU1_SM_BIP8_REG.
 * Located at \b 0x00 from ODUK_MPIF_BRICK_0_BASE + 0x20000 + 0x70
 */
union OTU1_SM_BIP8_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct OTU1_SM_BIP8_BITS bits;
};
/**Bit definitions for OTU1_UNCORR_CW_REG.
 *
 */
struct OTU1_UNCORR_CW_BITS
{
	 volatile unsigned int  :32;    			 		/*!<b'31:0 */
};
/**Allows register access for OTU1_UNCORR_CW_REG.
 * Located at \b 0x00 from ODUK_MPIF_BRICK_0_BASE + 0x20000 + 0x5C
 */
union OTU1_UNCORR_CW_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct OTU1_UNCORR_CW_BITS bits;
};
/**Bit definitions for OTU1_FEC_CORR_BITS_REG.
 *
 */
struct OTU1_FEC_CORR_BITS
{
	 volatile unsigned int  :32;    			 		/*!<b'31:0*/
};
/**Allows register access for OTU1_FEC_CORR_BITS_REG.
 * Located at \b 0x00 from ODUK_MPIF_BRICK_0_BASE + 0x20000 + 0x64
 */
union OTU1_FEC_CORR_BITS_REG
{
 alt_u32 all;/* used to write or read from the whole register*/
 struct OTU1_FEC_CORR_BITS bits;
};

typedef struct{
	/*!< offset 0x00 from ODUK_MPIF_BRICK_0_BASE + 0x40000*i + 0x2000*/
//	volatile union OTU1_MAPPER_LN_CFG_REG* 			ln_cfg_reg;
//	/*!< offset 0x00 from ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
//	volatile union OTU1_MAPPER_LN_CFG1_REG* 		ln_cfg1_reg;
	/*!< offset 0x20 from ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_LIVE_STS_REG* 				live_status_1;
	/*!< offset 0x20 from 0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_LIVE_STS_REG* 				live_status_2;
	/*!< offset 0x68 from ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_INT_STS_REG* 				int_status_1;
	/*!< offset 0x68 from 0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_INT_STS_REG* 				int_status_2;
	/*!< offset 0x74 from ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_SM_BIP8_REG* 				sm_bip8_1;
	/*!< offset 0x74 from 0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_SM_BIP8_REG* 				sm_bip8_2;
	/*!< offset 0x6C from ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_UNCORR_CW_REG* 				uncorr_cw_1;
	/*!< offset 0x6C from 0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_UNCORR_CW_REG* 			    uncorr_cw_2;
	/*!< offset 0x70 from ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_FEC_CORR_BITS_REG* 			fec_corr_bits_1;
	/*!< offset 0x70 from 0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	volatile union OTU1_FEC_CORR_BITS_REG* 			fec_corr_bits_2;
	/*!< offset 0x68 from ODUK_MPIF_BRICK_0_BASE + 0x40000*i  + 0x2000*/
	//volatile union ODU1_INT_STS_REG* 				odu1_int_status;
}otu1_mapper_registers_t;

struct LOCAL_REG0_CTRL_REG_BITS
{
	volatile unsigned int PORT_TX_DIS:1;		/* bit 0 */
	volatile unsigned int SFP_PWR_OFF:1;		/* bit 1 */
	volatile unsigned int :15;    			 	/* bit 2 to bit 16 */
	volatile unsigned int PPM_EN:1 ; 			/* bit 17 */
	volatile unsigned int :1;   				/* bit 18 */
	volatile unsigned int RX_SERIALLPBK_EN:1;   /* bit 19 */
	volatile unsigned int :4;   				/* bit 20 to bit 23 */
	volatile unsigned int GXB_RST:1;   			/* bit 24 */
	volatile unsigned int GE_BRICK_RST:1 ; 		/* bit 25 */
	volatile unsigned int :6;   				/* bit 26 to bit 31 */
};
union LOCAL_REG0_CTRL_REG
{
	alt_u32 all;/* used to write or read from the whole register */
	struct LOCAL_REG0_CTRL_REG_BITS bits;
};

struct LOCAL_REG1_STS_BITS
{
	 volatile unsigned int  SFP_TX_FAULT:1;     /* bit 0 */
	 volatile unsigned int  :1;     			/* bit 1 */
	 volatile unsigned int  SFP_LOS:1;      	/* bit 2 */
	 volatile unsigned int  SFP_ABSENT:1;      	/* bit 3 */
	 volatile unsigned int  :12;      			/* bit 4 to bit 15 */
	 volatile unsigned int 	PPM_INC:1 ; 		/* bit 16 */
	 volatile unsigned int 	PPM_DEC:1 ; 		/* bit 17 */
/*	 volatile unsigned int  LINE_B1_ERRORS:14;  // bit 18 to bit 31 */
	 volatile unsigned int  :5;     			/* bit 18 to bit 22 */
	 volatile unsigned int 	FC_TX_LINK:1 ; 		/* bit 23 */
	 volatile unsigned int  :8;     			/* bit 24 to bit 31 */
};
union LOCAL_REG1_STS_REG
{
	alt_u32 all;/* used to write or read from the whole register */
	struct LOCAL_REG1_STS_BITS bits;
};

/**Bit definitions for \c BRICK_TOP_RATE_STATUS_BITS.
 */
struct BRICK_TOP_RATE_STATUS_BITS{
	/**Configured client rate.
	 * bit 2:0 RO
	 */
	volatile unsigned int  	RATE_SELECTED:3;		/*!<b'2:0 */
	/**Reserved for future use.
	 * bit 31:3 RO
	 */
	volatile unsigned int  	RESERVED_1:29;			/*!<b'31:3 */
};

/**Allows register access for BRICK_TOP_CL_GB_FIFO_DEPTH_REG.
 * Maps to register 0x14 from brick top base address.
 */
union BRICK_TOP_RATE_STATUS_REG{
	 /*!<Used to read/write the whole register */
	alt_u32 all;
	struct BRICK_TOP_RATE_STATUS_BITS bits;    /*!<Used for bit access */
};


/**MISC registers of a brick.
 * When mapped to the proper memory locations, these variables will allow us easy access to registers and bits.
 */
typedef struct{
	volatile union LOCAL_REG1_STS_REG* general_status_live;	/*!<Variable that represent BRICK_TOP_GENERAL_STATUS_LIVE_REG register mapped to offset 0x00 from MISC base */
	volatile union LOCAL_REG0_CTRL_REG* general_control;		/*!<Variable that represent BRICK_TOP_GENERAL_CONTROL_REG register mapped to offset 0x0C from MISC base */
	volatile union BRICK_TOP_RATE_STATUS_REG* rate_status;				/*!<Variable that represent BRICK_TOP_RATE_STATUS_BITS register mapped to offset 0x14 from MISC base */
}brick_top_registers_t;

/*
 * ODUk Muxponder Top Level MISC REGISTERS
 */

/*
 * 1- General control status register
 */
struct TOP_MISC_GENERAL_CTRL_STS_BITS
{
	volatile unsigned int	SI5375_CS_A:1;		/*bit 0 */
	volatile unsigned int	SI5375_CS_B:1;		/*bit 1 */
	volatile unsigned int	SI5375_CS_C:1;		/*bit 2 */
	volatile unsigned int	SI5375_CS_D:1;		/*bit 3 */
	volatile unsigned int	SI5375_RST_A:1;		/*bit 4 */
	volatile unsigned int	SI5375_RST_B:1;		/*bit 5 */
	volatile unsigned int	SI5375_RST_C:1;		/*bit 6 */
	volatile unsigned int	SI5375_RST_D:1;		/*bit 7 */
	volatile unsigned int	MUX1_SEL0:1;		/*bit 8 //DO NOT EVER TOUCH.. It is a RESET */
	volatile unsigned int	:7;					/*bit 9 to bit 15*/
	volatile unsigned int   FP_INIT_DONE:1; 	/*bit 16 */
	volatile unsigned int   NiOS_PLL_LOCKED:1;	/*bit 17 */
	volatile unsigned int   SPARE_INPUT:1;		/*bit 18 ---> This bit is driven by the PIC to reflect the power state; */
												/*if power is good, this bit is set. otherwise it is cleared */
//	volatile unsigned int   APS1_SEL:1;			/*bit 19 ---> APS1 SELECT; i.e: 1- Line 0 Selected, 0- Line 1 Selected */
//	volatile unsigned int   APS2_SEL:1;			/*bit 20 ---> APS1 SELECT; i.e: 1- Line 2 Selected, 0- Line 3 Selected */
	volatile unsigned int   :8;					/*bit 19 to 26 */
	volatile unsigned int   PUSK_ADDRESS:5;		/*bit 27 to 31 */
};
union TOP_MISC_GENERAL_CTRL_STS_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct TOP_MISC_GENERAL_CTRL_STS_BITS bits;
};

/*
 * 2- LCD register
 */
struct TOP_MISC_LCD_BITS
{
	volatile unsigned int  LCD_DATA_LSB:4;
	volatile unsigned int  LCD_DATA_MSB:4;
	volatile unsigned int  LCD_RS:1 ;			/* BIT8 */
	volatile unsigned int  LCD_RWN:1;			/* BIT9 */
	volatile unsigned int  LCD_EN:1; 			/* BIT10 */
	volatile unsigned int  :1;					/* BIT11 */
	volatile unsigned int  OTU2_RESET:1;		/* BIT12 */
	volatile unsigned int  :15;
	volatile unsigned int  SW_PLL_SWITCH:1;		/* 	BIT28 */
	volatile unsigned int  PLL_MUX_CTRL:1;		/* 	BIT29 */
	volatile unsigned int  SW_MUX_SWITCH:1;		/* 	BIT30 */
	volatile unsigned int  MUX_CTRL:1;			/* 	BIT31 */
};

union TOP_MISC_LCD_REG
{
	alt_u32 all;/* used to write or read from the whole register */
    struct TOP_MISC_LCD_BITS bits ;
};

/*
 * 2.5- LCD register
 */
struct TOP_MISC_CTRL_BITS
{
	volatile unsigned int  xSI5375_CS_A:1;		/*bit 0 */
	volatile unsigned int  xSI5375_CS_B:1;		/*bit 1 */
	volatile unsigned int  xSI5375_CS_C:1;		/*bit 2 */
	volatile unsigned int  xSI5375_CS_D:1;		/*bit 3 */
	volatile unsigned int  xSI5375_RST_A:1;		/*bit 4 */
	volatile unsigned int  xSI5375_RST_B:1;		/*bit 5 */
	volatile unsigned int  xSI5375_RST_C:1;		/*bit 6 */
	volatile unsigned int  xSI5375_RST_D:1;		/*bit 7 */
	volatile unsigned int  ySI5375_CS_A:1;		/*bit 8 */
	volatile unsigned int  ySI5375_CS_B:1;		/*bit 9 */
	volatile unsigned int  ySI5375_CS_C:1;		/*bit 10 */
	volatile unsigned int  ySI5375_CS_D:1;		/*bit 11 */
	volatile unsigned int  ySI5375_RST_A:1;		/*bit 12 */
	volatile unsigned int  ySI5375_RST_B:1;		/*bit 13 */
	volatile unsigned int  ySI5375_RST_C:1;		/*bit 14 */
	volatile unsigned int  ySI5375_RST_D:1;		/*bit 15 */
	volatile unsigned int  RESERVED_1:8;		/*bit 23:16 */
	volatile unsigned int  CL_0_ALS_OVERRIDE:1;		/*bit 28 1 means SW can control TX DISABLE even if ALS is enabled */
	volatile unsigned int  CL_1_ALS_OVERRIDE:1;		/*bit 29 1 means SW can control TX DISABLE even if ALS is enabled */
	volatile unsigned int  CL_2_ALS_OVERRIDE:1;		/*bit 30 1 means SW can control TX DISABLE even if ALS is enabled */
	volatile unsigned int  CL_3_ALS_OVERRIDE:1;		/*bit 31 1 means SW can control TX DISABLE even if ALS is enabled */
	volatile unsigned int  LN_0_ALS_OVERRIDE:1;		/*bit 28 1 means SW can control TX DISABLE even if ALS is enabled */
	volatile unsigned int  LN_1_ALS_OVERRIDE:1;		/*bit 29 1 means SW can control TX DISABLE even if ALS is enabled */
	volatile unsigned int  LN_2_ALS_OVERRIDE:1;		/*bit 30 1 means SW can control TX DISABLE even if ALS is enabled */
	volatile unsigned int  LN_3_ALS_OVERRIDE:1;		/*bit 31 1 means SW can control TX DISABLE even if ALS is enabled */
};

union TOP_MISC_CTRL_REG
{
	alt_u32 all;/* used to write or read from the whole register */
    struct TOP_MISC_CTRL_BITS bits ;
};


/*
 * 3- Config register
 */
struct TOP_MISC_CONFIG_BITS
{
	volatile unsigned int  PORT_0_RATE_SELECT:3;	/*Bit 0 to bit 2 */
	volatile unsigned int  PORT_0_RECONFIG_WR:1;	/*Bit 3 */
	volatile unsigned int  PORT_1_RATE_SELECT:3;	/*Bit 4 to bit 6 */
	volatile unsigned int  PORT_1_RECONFIG_WR:1;	/*Bit 7 */
	volatile unsigned int  PORT_2_RATE_SELECT:3;	/*Bit 8 to bit 9 */
	volatile unsigned int  PORT_2_RECONFIG_WR:1;	/*Bit 10 */
	volatile unsigned int  PORT_3_RATE_SELECT:3;	/*Bit 11 to bit 13 */
	volatile unsigned int  PORT_3_RECONFIG_WR:1;	/*Bit 14 */
	volatile unsigned int  PORT_4_RATE_SELECT:3;	/*Bit 15 to bit 17 */
	volatile unsigned int  PORT_4_RECONFIG_WR:1;	/*Bit 18 */
	volatile unsigned int  PORT_5_RATE_SELECT:3;	/*Bit 19 to bit 21 */
	volatile unsigned int  PORT_5_RECONFIG_WR:1;	/*Bit 22 */
	volatile unsigned int  PORT_6_RATE_SELECT:3;	/*Bit 23 to bit 25 */
	volatile unsigned int  PORT_6_RECONFIG_WR:1;	/*Bit 26 */
	volatile unsigned int  PORT_7_RATE_SELECT:3;	/*Bit 27 to bit 29 */
	volatile unsigned int  PORT_7_RECONFIG_WR:1;	/*Bit 31 */
};

union TOP_MISC_CONFIG_REG
{
	alt_u32 all;/* used to write or read from the whole register */
    struct TOP_MISC_CONFIG_BITS bits ;
};

/*
 * 4 - Port Status register
 */
struct TOP_MISC_STS_BITS
{
	volatile unsigned int  SI5375_A_LOL:1;			/*bit 0 */
	volatile unsigned int  SI5375_B_LOL:1;			/*bit 1 */
	volatile unsigned int  SI5375_C_LOL:1;			/*bit 2 */
	volatile unsigned int  SI5375_D_LOL:1;			/*bit 3 */
	volatile unsigned int  SI5375_A_INTR:1;			/*bit 4 */
	volatile unsigned int  SI5375_B_INTR:1;			/*bit 5 */
	volatile unsigned int  SI5375_C_INTR:1;			/*bit 6 */
	volatile unsigned int  SI5375_D_INTR:1;			/*bit 7 */
	volatile unsigned int  :4;						/*bit 8 to bit 11 */
	volatile unsigned int  CL_RECONFIG_DONE:1;		/*bit 12 */
	volatile unsigned int  CL_RECONFIG_BUSY:1;		/*bit 13 */
	volatile unsigned int  CL_RECONFIG_ERROR:1;		/*bit 14 */
	volatile unsigned int  :9;						/*bit 15 to bit 23 */
	volatile unsigned int  DIP_0:1;					/*bit 24 */
	volatile unsigned int  DIP_1:1;					/*bit 25 */
	volatile unsigned int  DIP_2:1;					/*bit 26 */
	volatile unsigned int  DIP_3:1;					/*bit 27 */
	volatile unsigned int  FP_SI1_INTR:1;			/*bit 28 */
	volatile unsigned int  FP_SI2_INTR:1;			/*bit 29 */
	volatile unsigned int  FP_SI3_INTR:1;			/*bit 30 */
	volatile unsigned int  FP_SI4_INTR:1;			/*bit 31 */
};

union TOP_MISC_STS_REG
{
	alt_u32 all;/* used to write or read from the whole register */
    struct TOP_MISC_STS_BITS bits ;
};

struct TOP_MISC_REG6_STS_BITS
{
	volatile unsigned int  RESERVED:32;			/*bit 31:0 */
};

union TOP_MISC_REG6_STS_REG
{
	alt_u32 all;/* used to write or read from the whole register */
    struct TOP_MISC_REG6_STS_BITS bits ;
};




/*
 * 5- Port SDH rate config register
 */
struct TOP_MISC_SDH_RATE_BITS
{
//	volatile unsigned int  PORT_0_SDH_RATE:1;		/*Bit 0 */
//	volatile unsigned int  PORT_1_SDH_RATE:1;		/*Bit 1 */
//	volatile unsigned int  PORT_2_SDH_RATE:1;		/*Bit 2 */
//	volatile unsigned int  PORT_3_SDH_RATE:1;		/*Bit 3 */
//	volatile unsigned int  ALS_CL_0_AUTO_RESTART:1;	/*Bit 4 */
//	volatile unsigned int  ALS_CL_1_AUTO_RESTART:1;	/*Bit 5 */
//	volatile unsigned int  ALS_CL_2_AUTO_RESTART:1;	/*Bit 6 */
//	volatile unsigned int  ALS_CL_3_AUTO_RESTART:1;	/*Bit 7 */
//	volatile unsigned int  ALS_LN_0_MANUAL_RESTART:1;	/*Bit 8 */
//	volatile unsigned int  ALS_LN_1_MANUAL_RESTART:1;	/*Bit 9 */
//	volatile unsigned int  ALS_LN_2_MANUAL_RESTART:1;	/*Bit 10 */
//	volatile unsigned int  ALS_LN_3_MANUAL_RESTART:1;	/*Bit 11 */
//	volatile unsigned int  ALS_LN_0_AUTO_RESTART:1;	/*Bit 12 */
//	volatile unsigned int  ALS_LN_1_AUTO_RESTART:1;	/*Bit 13 */
//	volatile unsigned int  ALS_LN_2_AUTO_RESTART:1;	/*Bit 14 */
//	volatile unsigned int  ALS_LN_3_AUTO_RESTART:1;	/*Bit 15 */
//	volatile unsigned int  ALS_CL_0_MODE:2;			/*Bit 17:16 */
//	volatile unsigned int  ALS_CL_1_MODE:2;			/*Bit 19:18 */
//	volatile unsigned int  ALS_CL_2_MODE:2;			/*Bit 21:20 */
//	volatile unsigned int  ALS_CL_3_MODE:2;			/*Bit 23:22 */
//	volatile unsigned int  ALS_CL_0_EN:1;			/*Bit 24 */
//	volatile unsigned int  ALS_CL_1_EN:1;			/*Bit 25 */
//	volatile unsigned int  ALS_CL_2_EN:1;			/*Bit 26 */
//	volatile unsigned int  ALS_CL_3_EN:1;			/*Bit 27 */
//	volatile unsigned int  ALS_LN_0_EN:1;			/*Bit 28 */
//	volatile unsigned int  ALS_LN_1_EN:1;			/*Bit 29 */
//	volatile unsigned int  ALS_LN_2_EN:1;			/*Bit 30 */
//	volatile unsigned int  ALS_LN_3_EN:1;			/*Bit 31 */
	volatile unsigned int  PORT_0_SDH_RATE:1;	/*Bit 0 */
	volatile unsigned int  PORT_1_SDH_RATE:1;	/*Bit 1 */
	volatile unsigned int  PORT_2_SDH_RATE:1;   /*BIT 2 */
	volatile unsigned int  PORT_3_SDH_RATE:1;	/*Bit 3 */
	volatile unsigned int  PORT_4_SDH_RATE:1;	/*Bit 4 */
	volatile unsigned int  PORT_5_SDH_RATE:1;   /*BIT 5 */
	volatile unsigned int  PORT_6_SDH_RATE:1;	/*Bit 6 */
	volatile unsigned int  PORT_7_SDH_RATE:1;	/*Bit 7 */
	volatile unsigned int  xSI3575_A_CS:1;   	/*BIT 8 */
	volatile unsigned int  xSI3575_B_CS:1;   	/*BIT 9 */
	volatile unsigned int  xSI3575_C_CS:1;   	/*BIT 10 */
	volatile unsigned int  xSI3575_D_CS:1;   	/*BIT 11 */
	volatile unsigned int  xSI3575_A_RST:1;   	/*BIT 12 */
	volatile unsigned int  xSI3575_B_RST:1;   	/*BIT 13 */
	volatile unsigned int  xSI3575_C_RST:1;   	/*BIT 14 */
	volatile unsigned int  xSI3575_D_RST:1;   	/*BIT 15 */
	volatile unsigned int  ySI3575_A_CS:1;   	/*BIT 16 */
	volatile unsigned int  ySI3575_B_CS:1;   	/*BIT 17 */
	volatile unsigned int  ySI3575_C_CS:1;   	/*BIT 18 */
	volatile unsigned int  ySI3575_D_CS:1;   	/*BIT 19 */
	volatile unsigned int  ySI3575_A_RST:1;   	/*BIT 20 */
	volatile unsigned int  ySI3575_B_RST:1;   	/*BIT 21 */
	volatile unsigned int  ySI3575_C_RST:1;   	/*BIT 22 */
	volatile unsigned int  ySI3575_D_RST:1;   	/*BIT 23 */
	volatile unsigned int  :8;   				/*BIT 24 to BIT 31 */

};

union TOP_MISC_SDH_RATE_REG
{
	alt_u32 all;/* used to write or read from the whole register */
    struct TOP_MISC_SDH_RATE_BITS bits ;
};


/*Top level ODUk Muxponder at offset 0x00 from ODUK_MPIF_BRICK_0_BASE 0x1000000
 * */
typedef struct
{
	/* 0x00 from  from  0x1000000 */
	volatile union TOP_MISC_GENERAL_CTRL_STS_REG* 	gen_ctrl_sts_reg_1;
	/* 0x00 from  from  0x11000000 */
	volatile union TOP_MISC_GENERAL_CTRL_STS_REG* 	gen_ctrl_sts_reg_2;
	/* 0x04 from  from  0x1000000 */
    volatile union TOP_MISC_LCD_REG* 				lcd_reg_1 ;
    /* 0x04 from  from  0x11000000 */
	volatile union TOP_MISC_LCD_REG* 				lcd_reg_2 ;
    /* 0x08 from  from  0x1000000 */
    volatile union TOP_MISC_CTRL_REG* 				ctrl_reg ;
    /* 0x0C from  from  0x1000000 */
    volatile union TOP_MISC_CONFIG_REG* 			config_1 ;
    /* 0x0C from  from  0x11000000 */
    volatile union TOP_MISC_CONFIG_REG* 			config_2 ;
    /* 0x1C from  from  0x1000000 */
    volatile union TOP_MISC_SDH_RATE_REG* 			sdh_rate_config_1 ;
    /* 0x1C from  from  0x11000000 */
	volatile union TOP_MISC_SDH_RATE_REG* 			sdh_rate_config_2 ;
}top_level_misc_reg_t ;

top_level_misc_reg_t top_level_misc_reg;

/*
 * ODTUG2 registers
 */
/*
 * 1- RegA port M Rate register
 */
struct ODTUG2_REGA_PORTM_RATE_BITS
{
	volatile unsigned int  PORT_0_RATE_RX:4;    				// bit 0 to bit 3
	volatile unsigned int  PORT_1_RATE_RX:4;    				// bit 4 to bit 7
	volatile unsigned int  PORT_2_RATE_RX:4;    				// bit 8 to bit 11
	volatile unsigned int  PORT_3_RATE_RX:4;    				// bit 12 to bit 15
	volatile unsigned int  PORT_4_RATE_RX:4;    				// bit 16 to bit 19
	volatile unsigned int  PORT_5_RATE_RX:4;    				// bit 20 to bit 23
	volatile unsigned int  PORT_6_RATE_RX:4;    				// bit 24 to bit 27
	volatile unsigned int  PORT_7_RATE_RX:4;    				// bit 28 to bit 31
};
union ODTUG2_REGA_PORTM_RATE_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct ODTUG2_REGA_PORTM_RATE_BITS bits;
};

/*
 * 2- Config register
 */
struct ODTUG2_CONFIG_BITS
{
	volatile unsigned int  :2;    				// bit 0 to bit 1
	volatile unsigned int  UPDATE_MSI:1;    	// bit 2
	volatile unsigned int  :29;    				// bit 3 to bit 31
};
union ODTUG2_CONFIG_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct ODTUG2_CONFIG_BITS bits;
};
/*ODTUG2 at ODUK_MPIF_BRICK_0_BASE + 0x12000*/
typedef struct
{
	 volatile union ODTUG2_REGA_PORTM_RATE_REG* regA_portM_rate_reg_1;  //0x24 from  ODUK_MPIF_BRICK_0_BASE + 0x12000
	 volatile union ODTUG2_REGA_PORTM_RATE_REG* regA_portM_rate_reg_2;  //0x24 from  0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x12000

	 volatile union ODTUG2_CONFIG_REG* config_reg_1;  //0x00 from  ODUK_MPIF_BRICK_0_BASE + 0x12000
	 volatile union ODTUG2_CONFIG_REG* config_reg_2;  //0x00 from  0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x12000
}odtug2_t;

odtug2_t odtug2;


typedef struct{
	//brick_top_registers_t* 		misc;	/*!<Instance of the MISC register */
	stm1416_registers_t*		stm_14_1;
	stm1416_registers_t*		stm_14_2;
	stm1416_registers_t*		stm_16_1;
	stm1416_registers_t*		stm_16_2;
	ge_mac_registers_t*			gemac_1;
	ge_mac_registers_t*			gemac_2;
	ge_brick_t*					ge;
	//gmp_aligner_t*				gmp;
	otu1_mapper_registers_t*	otu1;
	odtu_brick_register_t* 		odtu_brick;
	//gfp_registers_t*			gfp;
	/*warning check if i will need gepcs if yes i have to added for both fpgas*/
	ge_pcs_registers_t*			gepcs_1;
	ge_pcs_registers_t*			gepcs_2;
	/**/
}client_mmap_t;

typedef struct{
//	otu1_registers_t* 			otu1;
//	otu1_mapper_registers_t* 	otu1_mapper;
	otu2_brick_dual_t 			otu2_brick_1;
	otu2_brick_dual_t 			otu2_brick_2;
}line_mmap_t;


#define CLIENT_0_PFC_BASE		0x1000048
/*#warning fix these base after teddy gave you line pfc map*/
#define LINE_0_PFC_BASE			0x100A08C
#define LINE_1_PFC_BASE			0x101208C
/**/
#endif /* MMAP_H_ */
