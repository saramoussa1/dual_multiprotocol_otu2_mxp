/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_pca9516.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains prototypes of functions implemented in ipg_9516.c as well as type definitions.
 *
 */
#ifndef IPG_PCA9516_H_
#define IPG_PCA9516_H_

#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include "ipg_delay.h"

typedef void (*pfPCA9516SelectChannel)(int channel);
typedef void (*pfPCA9516UnselectAll)(void);

/**Representation a PCA9516 device.
 *
 */
typedef struct pca9516_device{
	pfPCA9516SelectChannel 	select_channel;	//!<callback function to select a channel.
	pfPCA9516UnselectAll	unselect_all;	//!<callback function to unselect a channel.
}pca9516_device_t;

void unselect_all_pca9516_channels(pca9516_device_t* hub);
void select_pca9516_channel(pca9516_device_t* hub, int channel);

#endif /* IPG_PCA9516_H_ */
