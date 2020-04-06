/*
 * ipglb_otu2_framer.c
 *
 *  Created on: Jan 19, 2018
 *      Author: Closers_2
 */

#include "ipglb_otu2_framer.h"

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif


void map_otu2_framer_regs(otu2_framer_t* framer, alt_u32 base){

	framer->regs.live_sts_reg = (volatile union OTU2_FRAMER_LIVE_STS_REG*)((base + 0x00)|DE_CACHE_EN_BIT);
	framer->regs.int_sts_reg = (volatile union OTU2_FRAMER_INTR_STS_REG*)((base + 0x04)|DE_CACHE_EN_BIT);
	framer->regs.line_config_reg = (volatile union OTU2_FRAMER_LINE_CONFIG_REG*)((base + 0x0C)|DE_CACHE_EN_BIT);
	framer->regs.rx_exp_sts_pt_reg = (volatile union OTU2_FRAMER_RX_EXP_STS_PT_REG*)((base + 0x24)|DE_CACHE_EN_BIT);
	framer->regs.otu2_sts_reg = (volatile union OTU2_FRAMER_REGA_STATUS_REG*)((base + 0x28)|DE_CACHE_EN_BIT);
	framer->regs.sm_bip8_errors_reg = (volatile union OTU2_FRAMER_SM_BIP8_REG*)((base + 0x2C)|DE_CACHE_EN_BIT);
	framer->regs.fec_corr_bits_reg = (volatile union OTU2_FRAMER_FEC_CORRECTED_BITS_REG*)((base + 0x30)|DE_CACHE_EN_BIT);
	framer->regs.uncorrected_reg = (volatile union OTU2_FRAMER_FEC_UNCORRECTED_REG*)((base + 0x34)|DE_CACHE_EN_BIT);
	framer->regs.fec_bit_errors = (volatile union OTU2_FRAMER_FEC_BIT_ERRORS_REG*)((base + 0x38)|DE_CACHE_EN_BIT);
}

