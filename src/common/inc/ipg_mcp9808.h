/*
 * ipg_mcp9808.h
 *
 *  Created on: Feb 2, 2013
 *      Author: Clara Z.M
 *  Modified on: March 15, 2013
 *  	Author: Dany Chamoun
 *  	To adapt code for 10G Muxponder
 */
/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_mcp9808.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains prototypes of functions implemented in ipg_mcp9808.c as well as type definitions.
 *
 */
#ifndef IPG_MCP9808_H_
#define IPG_MCP9808_H_

#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include "ipg_delay.h"
#include "ipg_i2c.h"

/**Representation of an MCP9808 device.
 *
 */
typedef struct mcp9808_device{
	i2c_device_t i2c_dev;
	alt_8 temperature;
}mcp9808_device_t;

ret_code_t mcp9808_get_temperature(mcp9808_device_t* mcp9808);
ret_code_t ipg_mcp9808_rd (mcp9808_device_t mcp, alt_u8 reg, alt_u8* data);
ret_code_t ipg_mcp9808_wr (mcp9808_device_t mcp, alt_u8 reg, alt_u8* data);
#endif /* IPG_MCP9808_H_ */
