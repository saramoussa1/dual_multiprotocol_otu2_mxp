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
 * @file ipg_si5375.h
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @date Dec 5, 2015
 * @brief This file contains type definitions and prototypes used by other code files to access SI-5375 devices.
 *
 */
#ifndef __IPG_SI5375_H__
#define __IPG_SI5375_H__

#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include "ipg_i2c.h"
#include "ipg_delay.h"

/**Representation a SI-5375 device.
 *
 */
typedef struct{
	i2c_device_t i2c_dev;		//!<This field represents the I2C properties of the SI-5375 device.
	int portA_configuration[32][2];	//!<This array represents the list of registers/values pairs used to configure a SI-5375 device.
	int portB_configuration[32][2];	//!<This array represents the list of registers/values pairs used to configure a SI-5375 device.
	int portC_configuration[32][2];	//!<This array represents the list of registers/values pairs used to configure a SI-5375 device.
	int portD_configuration[32][2];	//!<This array represents the list of registers/values pairs used to configure a SI-5375 device.
}si5375_device_t;

typedef enum
{
	PORT_A = 0,
	PORT_B = 1,
	PORT_C = 2,
	PORT_D = 3
}si5375_port;

/*
 * exported prototypes
 */
ret_code_t ipg_si5375_config(si5375_device_t* silab, si5375_port port);
ret_code_t ipg_si5375_wr(si5375_device_t* silab, alt_u8 si_register, alt_u8 value);
ret_code_t ipg_si5375_rd(si5375_device_t* silab, alt_u8 si_register, alt_u8 *value);
ret_code_t handle_user_si5375_write(si5375_device_t* silab,si5375_port port, alt_u8 i2c_address, alt_u8 i2c_value);
ret_code_t handle_user_si5375_read(si5375_device_t* silab, si5375_port port, alt_u8 offset, alt_u8 *value);
ret_code_t handle_user_si5375_config(si5375_device_t* silab, si5375_port port);

#endif	// _IPG_SI5375_H_
