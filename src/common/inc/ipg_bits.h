/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_bits.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 7, 2015
 * @revision 0.0.1
 * @brief Contains BIT definitions and bitwise operation functions.
 *
 */
#ifndef IPG_BITS_H_
#define IPG_BITS_H_

#include "HAL/inc/io.h"

enum IPG_BITS{
    BIT0    = 0x00000001,
    BIT1    = 0x00000002,
    BIT2    = 0x00000004,
    BIT3    = 0x00000008,
    BIT4    = 0x00000010,
    BIT5    = 0x00000020,
    BIT6    = 0x00000040,
    BIT7    = 0x00000080,
    BIT8    = 0x00000100,
    BIT9    = 0x00000200,
    BIT10   = 0x00000400,
    BIT11   = 0x00000800,
    BIT12   = 0x00001000,
    BIT13   = 0x00002000,
    BIT14   = 0x00004000,
    BIT15   = 0x00008000,
    BIT16   = 0x00010000,
    BIT17   = 0x00020000,
    BIT18   = 0x00040000,
    BIT19   = 0x00080000,
    BIT20   = 0x00100000,
    BIT21   = 0x00200000,
    BIT22   = 0x00400000,
    BIT23   = 0x00800000,
    BIT24   = 0x01000000,
    BIT25   = 0x02000000,
    BIT26   = 0x04000000,
    BIT27   = 0x08000000,
    BIT28   = 0x10000000,
    BIT29   = 0x20000000,
    BIT30   = 0x40000000,
    BIT31   = 0x80000000
};

/*
 * macros for bitwise operations
 */
#define SET_BIT(This_Word, This_Flag)       {(This_Word) |= (This_Flag);}					//!<Sets one or more bits in a word
#define CLEAR_BIT(This_Word, This_Flag)     {(This_Word) &= ~(This_Flag);}					//!<Clears one or more bits in a word
#define BIT_IS_SET(This_Word, This_Mask)    (( (This_Word) & (This_Mask) ) == (This_Mask))	//!<Checks whether one or more bits are set in a word
#define BIT_IS_CLEAR(This_Word, This_Mask)    !BIT_IS_SET(This_Word, This_Mask)				//!<Checks whether one or more bits are cleared in a word
/*
 * macros for IOs (register bits)
 */
#define GET_IO(io) get_bit_state(io.address, io.bit)
#define SET_IO(io) set_bit_state(io.address, io.bit, 1)
#define CLEAR_IO(io) set_bit_state(io.address, io.bit, 0)
#define IO_SET		1
#define IO_RESET	0
#define IS_IO_SET(io)	(GET_IO(io)==IO_SET)
#define IS_IO_CLEAR(io)	(GET_IO(io)==IO_RESET)
/**
 * This function gets the current state of a bit in a register.
 * @param[in] address	address of the register
 * @param[in] bit		bit to check
 * @return 1 if bit is set and 0 if it is clear
 */
static alt_u8 get_bit_state(alt_u32 address, alt_u32 bit){
	return BIT_IS_SET(IORD_32DIRECT(address, 0), bit);
}

/**
 * This function sets/clears a bit in a register.
 * @param[in] address	address of the register
 * @param[in] bit		bit to modify
 * @param[in] new_state		requested new state, 0 to clear and 1 to set
 */
static void set_bit_state(alt_u32 address, alt_u32 bit, alt_u8 new_state){
	alt_u32 t = IORD_32DIRECT(address, 0);
	(new_state)?(SET_BIT(t, bit)):(CLEAR_BIT(t, bit));
	IOWR_32DIRECT(address, 0, t);
}
#endif /*IPG_BITS_H_*/
