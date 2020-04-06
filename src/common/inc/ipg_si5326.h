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
 * @file ipg_si5326.h
 * @author Clara Zaayter (czaayter@ipgphotonics.com)
 * @date Feb 5, 2013
 * @brief This file contains prototypes for SI-5326 functions as well as type definitions.
 *
 */
#ifndef IPG_SI5326_H_
#define IPG_SI5326_H_

#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include "ipg_i2c.h"
#include "ipg_delay.h"

/**Representation of a SI-5326 device.
 *
 */
typedef struct si5326_device{
	i2c_device_t i2c_dev;      //!<This field represents the I2C properties of the SI-5326 device.
	int configuration[45][2]; //!<This array represents the list of registers/values pairs used to configure a SI-5326 device.
}si5326_device_t;

ret_code_t ipg_si5326_config(si5326_device_t* silab);
ret_code_t ipg_si5326_wr(si5326_device_t* silab, alt_u8 si_register, alt_u8 value);
ret_code_t ipg_si5326_rd(si5326_device_t* silab, alt_u8 si_register, alt_u8 *value);
ret_code_t handle_user_si5326_write(si5326_device_t* silab, alt_u8 i2c_address, alt_u8 i2c_value);
ret_code_t handle_user_si5326_read(si5326_device_t* silab, alt_u8 offset, alt_u8 *value);
ret_code_t handle_user_si5326_config(si5326_device_t* silab);
#endif /* IPG_SI5326_H_ */
