/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) 2015 IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *
 * IPG Photonics Corporation
 * 50 Old Webster Road
 * Oxford, MA 01540, USA
 ****************************************************************************/

/**
 * @file ipg_si5338.h
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @date Jan 27, 2015
 * @brief This file contains type definitions and prototypes used by other code files to access SI-5338 devices.
 *
 */
#ifndef __IPG_SI5338_H__
#define __IPG_SI5338_H__

#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include "ipg_i2c.h"
#include "ipg_delay.h"

/**Representation a SI-5338 device.
 *
 */
typedef struct{
	i2c_device_t i2c_dev;		//!<This field represents the I2C properties of the SI-5338 device.
	int configuration[179][2];	//!<This array represents the list of registers/values pairs used to configure a SI-5338 device.
}si5338_device_t;

/**This enum type represents the possible clock speed operations that we can apply to a clock output.
 *
 */
typedef enum{
	SLOW_DOWN = 0,//!<Constant used to slow down an output.
	SPEED_UP = 1  //!<Constant used to speed up an output.
}silab_clk_out_speed_t;

/*
 * exported prototypes
 */
ret_code_t ipg_si5338_config(si5338_device_t* silab);
ret_code_t ipg_si5338_wr(si5338_device_t* silab, alt_u8 si_register, alt_u8 value);
ret_code_t ipg_si5338_rd(si5338_device_t* silab, alt_u8 si_register, alt_u8 *value);
ret_code_t ipg_si5338_change_speed(si5338_device_t* silab, alt_u8 which_clk_output, silab_clk_out_speed_t clk_operation );
ipg_bool_t ipg_si5338_locked(si5338_device_t* silab);
void ipg_si5338_disable_freq_inc_dec(si5338_device_t* _silab, alt_u8 which_clk_output);
void ipg_si5338_enable_freq_inc_dec(si5338_device_t* _silab, alt_u8 which_clk_output);
void ipg_si5338_clr_pll_locked_bit(si5338_device_t* silab);
ret_code_t handle_user_si5338_write(si5338_device_t* silab, alt_u8 i2c_address, alt_u8 i2c_value);
ret_code_t handle_user_si5338_read(si5338_device_t* silab, alt_u8 offset, alt_u8 *value);
ret_code_t handle_user_si5338_config(si5338_device_t* silab);
ret_code_t ipg_si5338_recenter(si5338_device_t* _silab, alt_u8 which_clk_output);
ret_code_t ipg_si5338_change_step(si5338_device_t* _silab, alt_u8 which_clk_output, alt_u16 size);
#endif	// _IPG_SI5338_H_
