/*
 * opu4_mapper.c
 *
 *  Created on: Jan 19, 2017
 *      Author: d_chamoun
 */
#include "../inc/opu4_mapper.h"
#include "../inc/ipg_types.h"
#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT	BIT31
#endif

/*
 * local prototypes
 */

/**This function runs the default initialization of an ODU2 wrapper device.
 * @param[in,out] wrapper pointer to the ODU2 wrapper device registers
 */
void init_opu4_mapper(opu4_mapper_t* mapper){

}

/**This function maps the ODU2 wrapper registers to our application variables.
 *
 * @param[in,out] wrapper pointer to the application ODU2 wrapper variables
 * @param wrapper_base	base address of the wrapper in memory
 * @return None
 */
void map_opu4_mapper_regs(opu4_mapper_t* opu4_mapper, alt_u32 base){
	map_opu4_mapper_regs_ol(&opu4_mapper->regs, base);
}

void map_opu4_mapper_regs_ol(opu4_mapper_registers_t* opu4_mapper, alt_u32 base){
	opu4_mapper->reg0_status_ctrl 		= (volatile union OPU4_MAPPER_REG0_STAT_CNTRL*) 	((base + 0x00) | DE_CACHE_EN_BIT);
	opu4_mapper->bip8_cnt 				= (volatile union OPU4_MAPPER_BIP8_ERROR_COUNT_REG*) ((base + 0x04) | DE_CACHE_EN_BIT);
	opu4_mapper->gcc 					= (volatile union OPU4_MAPPER_GCC0_REG*) ((base + 0x10) | DE_CACHE_EN_BIT);
}


