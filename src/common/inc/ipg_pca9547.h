/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_pca9547.h
 * @author d_chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains prototypes of functions implemented in ipg_pca9547.c as well as type definitions.
 *
 */
#ifndef __IPG_PCA9547_H__
#define __IPG_PCA9547_H__

#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include "ipg_i2c.h"
#include "ipg_delay.h"

/**Representation a PCA9547 device.
 *
 */
typedef struct pca9547_device{
	i2c_device_t i2c_dev;	//!<This field represents the I2C properties of the PCA9547 device.
}pca9547_device_t;

void unselect_all_pca9547_channels(pca9547_device_t* mux);
void select_pca9547_channel(pca9547_device_t* mux, int channel);

#endif	//__IPG_PCA9547_H__
