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
 * @file ipg_mcp9808.c
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @author Grace Khayat (gkhayat@ipgphotonics.com)
 * @date Feb 2, 2013
 * @brief This file contains low level functions to access/monitor/control an MCP9808 temperature sensor.
 *
 * The MCP9808 is a digital temperature sensor manufactured by Microchip.
 * @see http://www.farnell.com/datasheets/1522173.pdf
 */
#include "../inc/ipg_mcp9808.h"

#define MCP9808_TEMPERATURE_REG			5 //!<Address of the temperature register

#ifndef NULL
#define NULL	0
#endif

/**This function reads the temperature from an MCP9808 temperature sensor.
 * The measurement resolution is the default value.
 * The read temperature is stored in the \c temperature field of the mcp9808 data structure.
 * @param[in,out] mcp9808  pointer to the MCP9808 device.
 * @return \c RET_SUCCESS on success
 */
ret_code_t mcp9808_get_temperature(mcp9808_device_t *mcp9808){
	int result = -1 ;
	alt_u8 msbLsb[2];
	alt_u8 msb = 0, lsb = 0;
	float tempf = 0;

	// first make the proper I2C mux selections
	select_i2c_channel_on_bus(mcp9808->i2c_dev.bus, mcp9808->i2c_dev.channel);
	Delay_ms(1);

	// Read two bytes the msb and then the lsb
	result = sls_read_16_bit(mcp9808->i2c_dev.bus->fd, mcp9808->i2c_dev.address, (alt_u8*)msbLsb, MCP9808_TEMPERATURE_REG);
	if(result == 0)
	{
		return RET_FAIL;
	}

	// The first byte read is the MSB of the temperature register
	// The second byte read is the LSB of the temperature register
	msb = msbLsb[0];
	lsb = msbLsb[1];

	//Zero bit 7, 6 and 5 (flag bits)
	msb &= 0x1F;

	if ((msb & 0x10) == 0x10)//TA < 0°C
	{
		msb = msb & 0x0F; //Clear SIGN
		tempf = (msb * 16 + lsb / 16) - 256;
	}
	else //TA >= 0°C
	{
		tempf = (msb * 16 + lsb / 16);
	}

	(mcp9808->temperature) = (alt_8)(tempf);
	return 	(RET_SUCCESS);
}

/**This function reads a register from the MCP9808 device.
 * @param[in] 	mcp 	the MCP9808 device instance
 * @param[in] 	reg 	address of the register to be read
 * @param[out] 	data 	pointer to the location where to store the data that was read
 * @return \c RET_SUCCESS upon success
 */
ret_code_t ipg_mcp9808_rd (mcp9808_device_t mcp, alt_u8 reg, alt_u8* data){
	// first make the proper I2C mux selections
	select_i2c_channel_on_bus(mcp.i2c_dev.bus, mcp.i2c_dev.channel);
	Delay_ms(1);
	return 	(sls_read_16_bit(mcp.i2c_dev.bus->fd, mcp.i2c_dev.address, data, reg));
}

/**This function writes a 16-bit data word to a register in the MCP9808 device.
 * @param[in] 	mcp 	the MCP9808 device instance
 * @param[in] 	reg 	address of the register to write to
 * @param[out] 	data 	pointer to the data to be written
 * @return \c RET_SUCCESS upon success
 */
ret_code_t ipg_mcp9808_wr (mcp9808_device_t mcp, alt_u8 reg, alt_u8* data){
	alt_u8 txBuff[2];
	txBuff[0] = data[0];
	txBuff[1] = data[1];
	// first make the proper I2C mux selections
	select_i2c_channel_on_bus(mcp.i2c_dev.bus, mcp.i2c_dev.channel);
	return 	(sls_write(mcp.i2c_dev.bus->fd, mcp.i2c_dev.address, (char*)txBuff, reg, 2));
}
