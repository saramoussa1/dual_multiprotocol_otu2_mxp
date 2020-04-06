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
 * @file ipg_si5326.c
 * @author Clara Zaiter	(czaayter@ipgphotonics.com)
 * @date June 11, 2018
 * @brief This file contains the functions needed to control/monitor/access a SI-5326 clock multiplier.
 * @version 0.0.3
 * @modified by Clara Zaiter (czaayter@ipgphotonics.com)
 * @see http://www.silabs.com/Support%20Documents/TechnicalDocs/Si5326.pdf
 */

#include "../inc/ipg_si5326.h"
#warning "Make sure rd/wr return type is compatible with version 0.0.3"

/**This function is used to write to a single register of a SI-5326 device.
 * @param[in,out] silab		pointer to the SI5326 device
 * @param[in] si_register	the register offset we want to write to
 * @param[in] value			value to be written
 * @return				\c RET_SUCCESS on success
 */
ret_code_t ipg_si5326_wr(si5326_device_t* silab, alt_u8 si_register, alt_u8 value){
	alt_u8 txBuff[1] = {value};
	select_i2c_channel_on_bus(silab->i2c_dev.bus, silab->i2c_dev.channel);
	Delay_ms(1);
	if (sls_write(silab->i2c_dev.bus->fd, silab->i2c_dev.address, (char*)txBuff, si_register, 1)){
		return RET_SUCCESS;
	}else{
		return RET_FAIL;
	}
}

/**This function is used to read a single register from a SI-5326 device.
 * @param[in,out] silab		pointer to the SI5326 device
 * @param[in] si_register	the register offset we want to read from
 * @param[out] value		pointer to where the read value will be stored
 * @return				\c RET_SUCCESS on success
 */
ret_code_t ipg_si5326_rd(si5326_device_t* silab, alt_u8 si_register, alt_u8 *value){
	select_i2c_channel_on_bus(silab->i2c_dev.bus, silab->i2c_dev.channel);
	Delay_ms(1);
	if(sls_read(silab->i2c_dev.bus->fd, silab->i2c_dev.address, value, si_register, 1,1)){
		return RET_SUCCESS;
	}else{
		return RET_FAIL;
	}
}

/**This function configures a SI-5326 device.
 * The configuration is assigned to the device when creating it.
 * @param[in,out] silab		pointer to the SI5326 device
 * @return			\c RET_SUCCESS on success
 */
ret_code_t ipg_si5326_config(si5326_device_t* silab){
	int i = 0;
	for(i = 0; i < 45; i++){
		ipg_si5326_wr(silab, silab->configuration[i][0], silab->configuration[i][1]);
	}
	return 	(RET_SUCCESS);
}

/**This helper function handles users' requests to write to a register to the SI-5326.
 * It is dispatched from the I2C write handler (\a handle_i2c_wr()) with the SI-5326 as argument.
 * @param[in] silab SILAB to read from
 * @param[in] offset offset of the register to read from
 * @param[in] value pointer where the read data will be stored
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_si5326_write(si5326_device_t* silab, alt_u8 offset, alt_u8 value){
	return 	(ipg_si5326_wr(silab, offset, value));
}


/**This helper function reads a register from the SI-5326.
 * It is dispatched from the I2C read handler (\a handle_i2c_rd()) with the SI-5326 as argument.
 * @param[in] silab SI-5326 to read from
 * @param[in] offset offset of the register to read from
 * @param[out] value pointer where the read data will be stored
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_si5326_read(si5326_device_t* silab, alt_u8 offset, alt_u8 *value){
	return 	(ipg_si5326_rd(silab, offset, value));
}



/**This helper function configures the SI-5326.
 * It is dispatched from the I2C config handler (\a handle_i2c_confgi()) with the SI-5326 as argument.
 * <br>Each SILAB has a configuration associated to it inside \a build_i2c_tree() function:
 * his is the configuration that will be used.
 * @param[in] silab SILAB to configure
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_si5326_config(si5326_device_t* silab){
	// in our case, silabs are bound to a specific configuration
	return 	(ipg_si5326_config(silab));
}
