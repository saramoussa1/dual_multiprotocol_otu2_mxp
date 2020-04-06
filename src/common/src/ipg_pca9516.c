/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_pca9516.c
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains procedures to access and configure a PCA9516 I2C hub.
 *
 * The PCA9516A is a 5-channel I2C bus hub. By setting one of the ENABLE pins, the corresponding channel become active.
 * @see http://www.nxp.com/documents/data_sheet/PCA9516.pdf
 */
#include "../inc/ipg_pca9516.h"

/**This function unselects all the channels of a PCA9516 I2C hub.
 *
 * @param[in,out] hub pointer to the hub device
 */
void unselect_all_pca9516_channels(pca9516_device_t* hub){
	hub->unselect_all();
}

/**This function selects a channel of a PCA9516 I2C hub.
 *
 * @param[in,out] hub pointer to the hub device
 * @param[in] channel channel to select
 */
void select_pca9516_channel(pca9516_device_t* hub, int channel){
	hub->select_channel(channel);
}



