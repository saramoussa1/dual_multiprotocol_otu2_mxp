/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_pca9547.c
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains procedures to access and configure a PCA9547 I2C multiplexer.
 *
 * The PCA9547 is a 8-channel I2C multiplexer from NXP.
 * @see http://www.nxp.com/documents/data_sheet/PCA9547.pdf
 */
#include "../inc/ipg_pca9547.h"

#define I2C_MUX_EN_BIT				BIT3	//!<Output enable bit mask for the multiplexer
#define PCA9547_CONTROL_REGISTER	0		//!<Address of the control register of the multiplexer

/**This function unselects all the channels of a PCA9547 I2C multiplexer.
 * @param[in,out] mux pointer to the multiplexer device.
 * @return None
 */
void unselect_all_pca9547_channels(pca9547_device_t* mux){
	alt_u8 txBuff[1] = {0};
	// all channels of this mux are unselected by clearing bit3 of the control register
	sls_write(mux->i2c_dev.bus->fd, mux->i2c_dev.address, (char*)txBuff, PCA9547_CONTROL_REGISTER, 1);
	Delay_ms(1);
}

/**This function selects a channel in a PCA9547 I2C multiplexer.
 *
 * @param[in,out] mux 	pointer to the multiplexer device.
 * @param[in] channel	channel to select, the multiplexer has 8 possible input channels
 * @return None
 */
void select_pca9547_channel(pca9547_device_t* mux, int channel){
	alt_u8 tx[1];
	// a new channel has to be selected
	tx [0] = (I2C_MUX_EN_BIT | channel);
	sls_write(mux->i2c_dev.bus->fd, mux->i2c_dev.address, (char*)tx, PCA9547_CONTROL_REGISTER, 1);
}

