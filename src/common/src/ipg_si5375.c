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
 * @file ipg_si5375.c
 * @author Clara Zaiter(czaayter@ipgphotonics.com)
 * @date December 5, 2016
 * @brief This file contains the functions needed to control/monitor/access a SI-5375
 * which is a 4-PLL any-frequency precision clock multiplier/jitter attenuator
 *
 * Refer to: https://www.silabs.com/Support%20Documents/TechnicalDocs/Si5375.pdf
 * @version 0.0.1
 *
 */

#include "../inc/ipg_si5375.h"

#ifndef NULL
#define NULL	0
#endif

#define OUTPUT_ADDRESS_OFFSET	0x2
#define SI5375_OUTPUT_COUNT		4
// We consider  PORT A as 0xD0
//				PORT B as 0xD2
//				PORT C as 0xD4
//				PORT D as 0xD6
int silab_output_addresses[SI5375_OUTPUT_COUNT] = { 0xD0, 0xD2, 0xD4, 0xD6 };//!<Array storing the output addresses of the 4 ports of the SiLAB.

/**This function is used to write to a single register of a SI-5375 device.
 * If a \c select_i2c_channel_on_bus has been implemented, it calls it to make sure
 * the requested \a _silab is selected.
 * @param[in] _silab		instance of the SI-5375 device we want to write to
 * @param[in] si_register	the register offset we want to write to
 * @param[in] value			value to be written
 * @return				\c RET_SUCCESS on success
 * @since 0.0.1
 */
ret_code_t ipg_si5375_wr(si5375_device_t* _silab, alt_u8 si_register, alt_u8 value){
	//Update device address
//	_silab->i2c_dev.address = silab_output_addresses [port];
	ret_code_t result = RET_FAIL;
	alt_u8 txBuff[1] = { value };
	Delay_ms(1);
	select_i2c_channel_on_bus(_silab->i2c_dev.bus, _silab->i2c_dev.channel);
	result = (!sls_write(_silab->i2c_dev.bus->fd, _silab->i2c_dev.address, (char *)txBuff, si_register, 1));
	return 	(result);
}

/**This function is used to read a single register from a SI-5375 device.
 * If a \c select_i2c_channel_on_bus has been implemented, it calls it to make sure
 * the requested \a _silab is selected.
 * @param[in] _silab		instance of the SI-5375 device we want to read from
 * @param[in] si_register	the register offset we want to read from
 * @param[out] value		pointer to where the read value will be stored
 * @return					\c RET_SUCCESS on success
 * @since 0.0.1
 */
ret_code_t ipg_si5375_rd( si5375_device_t* _silab, alt_u8 si_register, alt_u8 *value){
	ret_code_t ret = RET_FAIL;
	select_i2c_channel_on_bus(_silab->i2c_dev.bus, _silab->i2c_dev.channel);
	ret = !sls_read(_silab->i2c_dev.bus->fd, _silab->i2c_dev.address, value, si_register, 1, 1);
	return 	(ret);
}

/**This function configures a SI-5375 device.
 * The configuration is the one that was bound to this device during initialization.
 * @param _silab	instance of the SI-5375 device we want to control
 * @param port	port of the SI-5375 device we want to control
 * @return			\c RET_SUCCESS on success
 * @since 0.0.1
 */
ret_code_t ipg_si5375_config(si5375_device_t* _silab, si5375_port port){
	int i = 0;
//	select_i2c_channel_on_bus(_silab->i2c_dev.bus, _silab->i2c_dev.channel);

	//Set the device address
	_silab->i2c_dev.address = silab_output_addresses [port];
	switch(port)
	{
		case PORT_A:
			for (i = 0; i < 31; i++)
			{
				if(ipg_si5375_wr(_silab, _silab->portA_configuration[i][0], _silab->portA_configuration[i][1]) != RET_SUCCESS)
				{
					Delay_ms(2);
					ipg_si5375_wr(_silab, _silab->portA_configuration[i][0], _silab->portA_configuration[i][1]);
				}
			}
			Delay_ms(10);
			ipg_si5375_wr(_silab, _silab->portA_configuration[31][0], _silab->portA_configuration[31][1]);
			Delay_ms(2);
			break;
		case PORT_B:
			for (i = 0; i < 31; i++)
			{
				if(ipg_si5375_wr(_silab, _silab->portB_configuration[i][0], _silab->portB_configuration[i][1]) != RET_SUCCESS)
				{
					Delay_ms(2);
					ipg_si5375_wr(_silab, _silab->portB_configuration[i][0], _silab->portB_configuration[i][1]);
				}
			}
			Delay_ms(10);
			ipg_si5375_wr(_silab, _silab->portB_configuration[31][0], _silab->portB_configuration[31][1]);
			Delay_ms(2);
			break;
		case PORT_C:
			for (i = 0; i < 31; i++)
			{
				if(ipg_si5375_wr(_silab, _silab->portC_configuration[i][0], _silab->portC_configuration[i][1]) != RET_SUCCESS)
				{
					Delay_ms(2);
					ipg_si5375_wr(_silab, _silab->portC_configuration[i][0], _silab->portC_configuration[i][1]);
				}
			}
			Delay_ms(10);
			ipg_si5375_wr(_silab, _silab->portC_configuration[31][0], _silab->portC_configuration[31][1]);
			Delay_ms(2);
			break;
		case PORT_D:
			for (i = 0; i < 31; i++)
			{
				if(ipg_si5375_wr(_silab, _silab->portD_configuration[i][0], _silab->portD_configuration[i][1]) != RET_SUCCESS)
				{
					Delay_ms(2);
					ipg_si5375_wr(_silab, _silab->portD_configuration[i][0], _silab->portD_configuration[i][1]);
				}
			}
			Delay_ms(10);
			ipg_si5375_wr(_silab, _silab->portD_configuration[31][0], _silab->portD_configuration[31][1]);
			Delay_ms(2);
			break;
		default:
			return RET_FAIL;
	}
	return 	(RET_SUCCESS);
}

/**This helper function handles users' requests to write to a register to one of the SI-5375s.
 * It is dispatched from the I2C write handler (\a handle_i2c_wr()) with the SI-5375 as argument.
 * @param[in] _silab SILAB to write to
 * @param[in] port port of the SI-5375 device we want to control
 * @param[in] offset offset of the register to write to
 * @param[in] value data to be written
 * @return \c RET_SUCCESS on success
 * @since 0.0.1
 */
ret_code_t handle_user_si5375_write(si5375_device_t* _silab, si5375_port port, alt_u8 offset, alt_u8 value){
	//Set the device address
	_silab->i2c_dev.address = silab_output_addresses [port];
	return 	(ipg_si5375_wr(_silab, offset, value));
}

/**This helper function reads a register from one of the SILABs.
 * It is dispatched from the I2C read handler (\a handle_i2c_rd()) with the SILAB as argument.
 * @param[in] _silab SILAB to read from
 * @param[in] port	port of the SI-5375 device we want to control
 * @param[in] offset offset of the register to read from
 * @param[out] value pointer where the read data will be stored
 * @return \c RET_SUCCESS on success
 * @since 0.0.1
 */
ret_code_t handle_user_si5375_read(si5375_device_t* _silab, si5375_port port, alt_u8 offset, alt_u8 *value){
	//Set the device address
	_silab->i2c_dev.address = silab_output_addresses [port];
	return 	(ipg_si5375_rd(_silab, offset, value));
}

/**This helper function configures one of the SI-5375s.
 * It is dispatched from the I2C config handler (\a handle_i2c_confgi()) with the SI-5375 as argument.
 * <br>Each SILAB has a configuration associated to it inside \a build_i2c_tree() function:
 * his is the configuration that will be used.
 * @param[in] _silab SILAB to configure
 * @param[in] port	port of the SI-5375 device we want to control
 * @return \c RET_SUCCESS on success
 * @since 0.0.1
 */
ret_code_t handle_user_si5375_config(si5375_device_t* _silab, si5375_port port){
	return 	(ipg_si5375_config(_silab, port));
}
