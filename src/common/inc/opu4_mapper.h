/*
 * opu4_mapper.h
 *
 *  Created on: Jan 19, 2017
 *      Author: d_chamoun
 */

#ifndef OPU4_MAPPER_H_
#define OPU4_MAPPER_H_

#include "alt_types.h"
#include "io.h"

struct OPU4_MAPPER_REG0_STAT_CNTRL_BITS{
	volatile unsigned int RX_FRAMER_ERROR_LIVE :1;		//!<b'0 Framer Error
	volatile unsigned int RESERVED_1 :3;				//!<b'3:1 reserved for future use
	volatile unsigned int RX_BDI_ERROR_LIVE :1;			//!<b'4 RX BDI
	volatile unsigned int RX_IAE_ERROR_LIVE :1;			//!<b'5 RX IAE
	volatile unsigned int RX_LOF_ERROR_LIVE :1;			//!<b'6 Live LOF
	volatile unsigned int RESERVED_2 :5;				//!<b'11:7 reserved for future use
	volatile unsigned int RX_BDI_ERROR_INT :1;			//!<b'12 INT BDI
	volatile unsigned int RESERVED_3 :18;				//!<b'30:13 reserved for future use
	volatile unsigned int RESET :1;						//!<b'31  active low reset of the OPU4 block - RW [SC]
};
/**Allows register access for OPU4_MAPPER_REG0_STAT_CNTRL_REG.
 * Located at \b 0 from OPU4_BASE.
 */
union OPU4_MAPPER_REG0_STAT_CNTRL_REG{
	alt_u32 all;					//!<used to write/read the whole register
	struct OPU4_MAPPER_REG0_STAT_CNTRL_BITS bits;	//!<Used for bit access
};

/**Allows register access for OPU4_MAPPER_BIP8_ERROR_COUNT_REG.
 * Located at \b 0x04 from OPU4_BASE.
 */
union OPU4_MAPPER_BIP8_ERROR_COUNT_REG{
	alt_u32 all;					//!<used to write/read the whole register
};

struct OPU4_MAPPER_GCC0_BITS{
	volatile unsigned int RX_GCC0 :16;
	volatile unsigned int TX_GCC0 :16;
};
/**Allows register access for OPU4_MAPPER_GCC0_REG.
 * Located at \b 0x0C from OPU4_BASE.
 */
union OPU4_MAPPER_GCC0_REG{
	alt_u32 all;					//!<used to write/read the whole register
	struct OPU4_MAPPER_GCC0_BITS bits;	//!<Used for bit access
};

typedef struct {
	volatile union OPU4_MAPPER_REG0_STAT_CNTRL_REG* reg0_status_ctrl;	//!<offset OPU4_OFFSET + 0x00
	volatile union OPU4_MAPPER_BIP8_ERROR_COUNT_REG* bip8_cnt; 			//!<offset OPU4_OFFSET + 0x04
	volatile union OPU4_MAPPER_GCC0_REG* gcc; 							//!<offset OPU4_OFFSET + 0x10
} opu4_mapper_registers_t;

typedef struct {
	opu4_mapper_registers_t regs;
} opu4_mapper_t;

#define map_opu4_mapper_registers(x, base) __generic(x, opu4_mapper_t, map_opu4_mapper_regs, map_opu4_mapper_regs_ol)
void init_opu4_mapper(opu4_mapper_t* mapper);
void map_opu4_mapper_regs(opu4_mapper_t* mapper, alt_u32 base);
void map_opu4_mapper_regs_ol(opu4_mapper_registers_t* opu4, alt_u32 base);

#endif /* OPU4_MAPPER_H_ */
