/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_mdio.c
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains procedures to write and read through an MDIO interface (IP by Altera).
 * @version 0.0.2
 */
#include "../inc/ipg_mdio.h"

#define MDIO_DEVAD_SHIFT		0  		//!<[4:0] bit mask for MDIO_DEVAD
#define MDIO_PRTAD_SHIFT		8  		//!<[12:8] bit mask for	MDIO_PRTAD
#define MDIO_REGAD_SHIFT		16 		//!<[31:16] bit mask for MDIO_REGAD

#define MDIO_ACCESS_REG 		0x80 	//!<Address of the ACCESS register of the MDIO interface, this is where the data to be written or the data that was read is stored
#define MDIO_CONFIG_REG 		0x84 	//!<Address of the CONFIG register of the MDIO interface, this is where the operation type (R or W) and the device settings are written
#define MDIO_ACCESS_REG_CL_22	0x00 	//!<Address of the ACCESS register of the MDIO interface, this is where the data to be written or the data that was read is stored (in Clause 22 mode)
#define MDIO_CONFIG_REG_CL_22	0x84 	//!<Address of the CONFIG register of the MDIO interface, this is where the operation type (R or W) and the device settings are written (in Clause 22 mode)

/**This function executes an MDIO write operation.
 * @param[in]	mdio_device			pointer to the MDIO interface
 * @param[in]	device_address		MDIO address of the device connected to the MDIO interface
 * @param[in]	register_address	address of the register in the MDIO device to write to
 * @param[in]	data				data to be written to the MDIO device
 * @since 0.0.1
 * @deprecated
 */
void MDIO_Write(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address, alt_u16 data){
   alt_u32 reg_value = ((device_address & 0x1F) << MDIO_DEVAD_SHIFT) | ((mdio_device.port_address & 0x1F) << MDIO_PRTAD_SHIFT) | ((register_address & 0xFFFF) << MDIO_REGAD_SHIFT);
   IOWR_32DIRECT(mdio_device.mdio_base, MDIO_CONFIG_REG, reg_value);
   IOWR_32DIRECT(mdio_device.mdio_base, MDIO_ACCESS_REG, data);
}

/**This function executes an MDIO read operation.
 * @param[in]	mdio_device			pointer to the MDIO interface
 * @param[in]	device_address		MDIO address of the device connected to the MDIO interface
 * @param[in]	register_address	address of the register in the MDIO device to read from
 * @return the data that was read
 * @since 0.0.1
 * @deprecated
 */
alt_u32 MDIO_Read(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address){
	 alt_u32 reg_value = ((device_address & 0x1F) << MDIO_DEVAD_SHIFT) | ((mdio_device.port_address & 0x1F) << MDIO_PRTAD_SHIFT) | ((register_address & 0xFFFF) << MDIO_REGAD_SHIFT);
	 IOWR_32DIRECT(mdio_device.mdio_base, MDIO_CONFIG_REG, reg_value);
	 return	(IORD_32DIRECT(mdio_device.mdio_base, MDIO_ACCESS_REG));
}

/**This function executes an MDIO write operation according to the clause 22 or clause 45 mode.
 * @param[in]	mdio_device			pointer to the MDIO interface
 * @param[in]	device_address		MDIO address of the device connected to the MDIO interface
 * @param[in]	register_address	address of the register in the MDIO device to write to
 * @param[in]	data				data to be written to the MDIO device
 * @param[in]	clause_22			set to 1 to use controller in clause 22 mode
 * @since 0.0.2
 */
void MDIO_Write_advanced(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address, alt_u16 data, alt_u8 clause_22){
	if(clause_22){
		IOWR_32DIRECT(mdio_device.mdio_base, MDIO_CONFIG_REG_CL_22, mdio_device.port_address & 0x1F);
		IOWR_32DIRECT(mdio_device.mdio_base, MDIO_ACCESS_REG_CL_22 + register_address, data);
	}else { /*clause 45 mode*/
		alt_u32 reg_value = ((device_address & 0x1F) << MDIO_DEVAD_SHIFT) | ((mdio_device.port_address & 0x1F) << MDIO_PRTAD_SHIFT) | ((register_address & 0xFFFF) << MDIO_REGAD_SHIFT);
		IOWR_32DIRECT(mdio_device.mdio_base, MDIO_CONFIG_REG, reg_value);
		IOWR_32DIRECT(mdio_device.mdio_base, MDIO_ACCESS_REG, data);
	}
}

/**This function executes an MDIO read operation according to the clause 22 or clause 45 mode.
 * @param[in]	mdio_device			pointer to the MDIO interface
 * @param[in]	device_address		MDIO address of the device connected to the MDIO interface
 * @param[in]	register_address	address of the register in the MDIO device to read from
 * @param[in]	clause_22			set to 1 to use controller in clause 22 mode
 * @return the data that was read
 * @since 0.0.2
 */
alt_u32 MDIO_Read_advanced(mdio_device_t mdio_device, alt_u8 device_address, alt_u16 register_address, alt_u8 clause_22){
	if(clause_22){
		IOWR_32DIRECT(mdio_device.mdio_base, MDIO_CONFIG_REG_CL_22, mdio_device.port_address & 0x1F);
		return	(IORD_32DIRECT(mdio_device.mdio_base, MDIO_ACCESS_REG_CL_22 + register_address));
	}else{ /*in clause 45 mode*/
		 alt_u32 reg_value = ((device_address & 0x1F) << MDIO_DEVAD_SHIFT) | ((mdio_device.port_address & 0x1F) << MDIO_PRTAD_SHIFT) | ((register_address & 0xFFFF) << MDIO_REGAD_SHIFT);
		 IOWR_32DIRECT(mdio_device.mdio_base, MDIO_CONFIG_REG, reg_value);
		 return	(IORD_32DIRECT(mdio_device.mdio_base, MDIO_ACCESS_REG));
	}
}

