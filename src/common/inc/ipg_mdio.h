/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_mdio.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains prototypes of functions implemented in ipg_mdio.c as well as type definitions.
 * @version 0.0.2
 */
#ifndef IPG_MDIO_H_
#define IPG_MDIO_H_

#include <io.h>
#include <system.h>
#include "alt_types.h"


/**Representation a MDIO interface.
 *
 */
typedef struct mdio_device{
	alt_u32 mdio_base;		//!<base address of the MIDO interface in our system
	alt_u8 port_address;	//!<port address of the device connected to this interface
}mdio_device_t;

void MDIO_Write(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address, alt_u16 data);
void MDIO_Write_advanced(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address, alt_u16 data, alt_u8 clause_22);
alt_u32 MDIO_Read(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address);
alt_u32 MDIO_Read_advanced(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address, alt_u8 clause_22);
#endif /* IPG_MDIO_H_ */
