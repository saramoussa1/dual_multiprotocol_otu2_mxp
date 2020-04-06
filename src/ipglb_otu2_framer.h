/*
 * ipglb_otu2_framer.h
 *
 *  Created on: Jan 19, 2018
 *      Author: Closers_2
 */

#ifndef IPGLB_OTU2_FRAMER_H_
#define IPGLB_OTU2_FRAMER_H_

#include <stdint.h>
#include "common/inc/ipg_types.h"
#include "common/inc/ipg_constants.h"

#define OTU2_FRAMER1_BASE	    ODUK_MPIF_BRICK_0_BASE + 0x20100
#define OTU2_FRAMER2_BASE	    ODUK_MPIF_BRICK_0_BASE + 0x20180
/*
 * 1- OTU2 Framer live status register
 */
struct OTU2_FRAMER_LIVE_STS_BITS
{
 volatile unsigned int  OOF:1;   	    // bit 0 L_OOF
 volatile unsigned int  :1;  		    // bit 1
 volatile unsigned int  PM_BIP8:1;      // bit 2
 volatile unsigned int  PM_BDI:1;       // bit 3
 volatile unsigned int  :4;				// bit 4 to bit 7
 volatile unsigned int  PM_BEI_CNT:4;   // bit 8 to bit 11
 volatile unsigned int  LOF:1;  	    // bit 12 L_LOF
 volatile unsigned int  OOM:1;  	    // bit 13 L_OOM
 volatile unsigned int  LOM:1;  	    // bit 14 L_LOM
 volatile unsigned int  AIS:1;   	    // bit 15 L_GDAIS
 volatile unsigned int  :16;   	        // bit 16 to bit 31
};
union OTU2_FRAMER_LIVE_STS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct OTU2_FRAMER_LIVE_STS_BITS bits;
};
/*
 * 2- OTU2 Mapper interrupt status register
 */
struct OTU2_FRAMER_INTR_STS_BITS
{
	volatile unsigned int  OOF:1;   	// bit 0 L_OOF
	volatile unsigned int  :11;  	    // bit 1 to bit 11
	volatile unsigned int  LOF:1;  	    // bit 12 L_LOF
	volatile unsigned int  OOM:1;  	    // bit 13 L_OOM
	volatile unsigned int  LOM:1;    	// bit 14 L_LOM
	volatile unsigned int  AIS:1;   	// bit 15 L_GDAIS
	volatile unsigned int  :16;  		// bit 16 to bit 31
};
union OTU2_FRAMER_INTR_STS_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct OTU2_FRAMER_INTR_STS_BITS bits;
};


/*
 * 4- OTU2 Framer Line config register
 */
struct OTU2_FRAMER_LINE_CONFIG_BITS
{
 volatile unsigned int  :3;    		    // bit 0 to bit 2
 volatile unsigned int  SW_RST:1;  		// bit 3
 volatile unsigned int  :3;             // bit 4 to bit 6
 volatile unsigned int  FEC_DEC_DIS:1;  // bit 7
 volatile unsigned int  :12;            // bit 8 to bit 19
 volatile unsigned int  FEC_SEL:1;      // bit 20
 volatile unsigned int  :11;            // bit 21 to 31
};
union OTU2_FRAMER_LINE_CONFIG_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct OTU2_FRAMER_LINE_CONFIG_BITS bits;
};

/*
 * 4- OTU2 Framer Line rx exp stat pt reg
 */
struct OTU2_FRAMER_RX_EXP_STS_PT_BITS
{
 volatile unsigned int  :11;    		    // bit 0 to bit 10
 volatile unsigned int  LI_RX_LCK:1;  		// bit 11
 volatile unsigned int  LI_RX_OCI:1;        // bit 12
 volatile unsigned int  :19;                // bit 13 to bit 31
};
union OTU2_FRAMER_RX_EXP_STS_PT_REG
{
 alt_u32 all;// used to write or read from the whole register
 struct OTU2_FRAMER_RX_EXP_STS_PT_BITS bits;
};


/*
 * 11- OTU2 Framer status register
 */
struct OTU2_FRAMER_REGA_STATUS_BITS
{
 volatile unsigned int  :2;    			// bit 0 to bit 1
 volatile unsigned int  S_BIP8:1;  		// bit 2
 volatile unsigned int  S_BDI:1;  		// bit 3
 volatile unsigned int  :4;  			// bit 4 to bit 7
 volatile unsigned int  S_BEI:1;  		// bit 8 to bit 11
 volatile unsigned int  :20;        	// bit 12 to 31

};

union OTU2_FRAMER_REGA_STATUS_REG
{
	 alt_u32 all;// used to write or read from the whole register
	 struct OTU2_FRAMER_REGA_STATUS_BITS bits;
};
/*
 * 12- OTU2 framer BIP8 register
 */
union OTU2_FRAMER_SM_BIP8_REG
{
	alt_u32 all;
};

/*
 * 13- OTU2 framer FEC corrected words register
 */
union OTU2_FRAMER_FEC_CORRECTED_BITS_REG
{
	alt_u32 all;
};

/*
 * 14- OTU2 framer FEC uncorrected words register
 */
union OTU2_FRAMER_FEC_UNCORRECTED_REG
{
	alt_u32 all;
};


/*
 * 15- OTU2 framer FEC BIT ERRORS register
 */
union OTU2_FRAMER_FEC_BIT_ERRORS_REG
{
	alt_u32 all;
};


typedef struct{
	 volatile union OTU2_FRAMER_LIVE_STS_REG* live_sts_reg;    						// 0x00 from  0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_INTR_STS_REG* int_sts_reg;     						// 0x04 from  0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_REGA_STATUS_REG* otu2_sts_reg;                      // 0x28 for   0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_LINE_CONFIG_REG* line_config_reg;  					// 0x0C from  0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_RX_EXP_STS_PT_REG* rx_exp_sts_pt_reg;         		// 0x24 from  0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_SM_BIP8_REG* sm_bip8_errors_reg;         			// 0x2C from  0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_FEC_CORRECTED_BITS_REG* fec_corr_bits_reg;        	// 0x30 from  0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_FEC_UNCORRECTED_REG* uncorrected_reg;        		// 0x34 from  0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_FEC_BIT_ERRORS_REG * fec_bit_errors;			    // 0x38 for   0x1040100 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_RX_GCC2_FIFO_FILL_REG* rx_gcc2_fifo_fill_register; 	// 0x68 from  0x1040400 for framer1 and 0x1040180 for framer2
	 volatile union OTU2_FRAMER_GCC2_WORD_READ_REG* gcc2_fifo_word_read_register; 	// 0x74 from  0x1040400 for framer1 and 0x1040180 for framer2

}otu2_framer_registers_t;

typedef struct{
	otu2_framer_registers_t regs;
}otu2_framer_t;
void map_otu2_framer_regs(otu2_framer_t* framer, alt_u32 base);

#endif /* IPGLB_OTU2_FRAMER_H_ */
