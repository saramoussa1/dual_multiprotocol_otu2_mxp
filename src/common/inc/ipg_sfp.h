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
 * @file ipg_sfp.h
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @date Jan 14, 2016
 * @brief Header file for ipg_sfp.c.
 *
 * Contains prototypes and type definitions for SFP needed by other files.
 */

#ifndef _IPG_SFP_H_
#define _IPG_SFP_H_

#include "../../configs.h"
#if ENABLE_SFP_CODE
#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include "math.h"
#include "stdlib.h"
#include "ipg_i2c.h"
#include "ipg_delay.h"
#include "ipglb_transceiver.h"

#ifndef IPGLB_LEGACY_CODE
#define IPGLB_LEGACY_CODE  1
#endif

#define MSA_0						0xA0//!<Constant used as SFP I2C address when we want to read from the SID table.
#define MSA_1						0xA2//!<Constant used as SFP I2C address when we want to read from the DDI table.
#define MSA_2						0xAC//!<Constant used as SFP I2C address when we want to read from the PHY table.

typedef enum{
	SFP_CONNECTOR_UNKNOWN = 0,
	SFP_CONNECTOR_SC,
	SFP_CONNECTOR_LC = 7
}sfp_connector_t;
/**Type representing the optical power of an SFP device.
 *
 */
typedef struct{
	// optical data split in arrays, used to reduce load when sending data to ARM7
	alt_u8 pwr[4]; 		// starting by POUT_MSB down to PIN_LSB
	alt_u8 pwr_thr[16]; // starting by POUT_HA_MSB down to PIN_LWLSB
	// concatenated optical power values, needed for local calculations
	alt_u16 pout;
	alt_u16 pin;
	alt_u16 pout_ha;
	alt_u16 pout_la;
	alt_u16 pout_hw;
	alt_u16 pout_lw;
	alt_u16 pin_ha;
	alt_u16 pin_la;
	alt_u16 pin_hw;
	alt_u16 pin_lw;
	float	output_power_value;
	float	input_power_value;
}optical_power_t;

/**Type representing an SFP device.
 *
 */
#if IPGLB_LEGACY_CODE
typedef struct{
	i2c_device_t i2c_dev;		/**<The I2C properties of the SFP device.*/
	// static
	char vendor_name[17];		/**<Pointer to the vendor name string.*/
	char serial_number[17];		/**<Pointer to the serial number string.*/
	char part_number[17];		/**<Pointer to the part number string.*/
	char revision_number[5];	/**<Pointer to the revision number string.*/
	char ID[55];				/**<Pointer to the ID string which is the combination of VN, SN, PN and RN (appended in our code).*/
	char destination[IF_DESTINATION_MAX_LEN];
	// control
	ipg_control_t tx_disable;	/**<Control variable for TX disable of the SFP device.*/
	// status
	ipg_status_t los;			/**<Status variable for the LOS alarm detected by the SFP.*/
	ipg_status_t absent;		/**<Status variable for the absent flag of the SFP.*/
	ipg_status_t tx_fault;		/**<Status variable for the TX fault alarm of the SFP.*/

	ipg_status_t stable_los;    /**<Status variable for the Stable LOS (LOS ORed with ABSENT) alarm of the SFP.*/
	// dynamic
	optical_power_t optical_pwr;/**<Instance of the optical power data structure of the SFP.*/
	int index;					/**<Index of the SFP on our platform.*/
	sfp_connector_t connector;
	diagnostics_t				temperature;
}sfp_device_t;
#else
typedef struct sfp_device sfp_device_t;
struct sfp_device{
	int index;					/**<Index of the SFP+ on our platform.*/
	transceiver_t* super;
	i2c_device_t i2c_dev;		/**<The I2C properties of the SFP+ device.*/
	// static
	char vendor_name[17];		/**<Pointer to the vendor name string.*/
	char serial_number[17];		/**<Pointer to the serial number string.*/
	char part_number[17];		/**<Pointer to the part number string.*/
	char revision_number[5];	/**<Pointer to the revision number string.*/
	sfp_connector_t connector;
};
#endif
/**SFP registers
 *
 */
enum SFP_REGS_OFFSET
{
    SFP_PHY_CTRL                     = 0x00,
    SFP_PHY_STS          			 = 0x01,
    SFP_PHY_AUTONEG_ADVER            = 0x04,
    SFP_PHY_1000_BASE_CTRL           = 0x09,
    SFP_PHY_EXT_PHY_STAT             = 0x1B,
    SFP_PHY_SELECT_PAGE			 	 = 0x16,

};

/*
 * function prototypes
 */
void init_sfp_data(sfp_device_t* sfp);
ret_code_t sfp_read_data (sfp_device_t* sfp, alt_u8 address, alt_u8* data);
ret_code_t sfp_write_data(sfp_device_t* sfp, alt_u8 address, alt_u8 data);
ret_code_t sfp_write_word(sfp_device_t* sfp, alt_u8 offset, alt_u16 data);
ret_code_t handle_user_sfp_read(sfp_device_t* sfp, alt_u8 msa, alt_u8 address, alt_u8* data);
ret_code_t handle_user_sfp_write(sfp_device_t* sfp, alt_u8 msa, alt_u8 offset, alt_u8 data);
void handle_user_phy_sfp_write(sfp_device_t* sfp, alt_u8 offset, alt_u16 data);
alt_u16 handle_user_phy_sfp_read( sfp_device_t* sfp, alt_u8 offset);
ret_code_t get_sfp_pn(sfp_device_t* sfp);
ret_code_t get_sfp_sn(sfp_device_t* sfp);
ret_code_t get_sfp_vn(sfp_device_t* sfp);
ret_code_t get_sfp_rn(sfp_device_t* sfp);
void enable_sfp_ge(sfp_device_t* sfp);
void enable_sfp_sgmii(sfp_device_t* _sfp);
void get_sfp_connector_value(sfp_device_t*_sfp);
ret_code_t sfp_read_data_block(sfp_device_t* sfp, alt_u8 start_address, int count, alt_u8* data);
ret_code_t handle_user_sfp_read_block(sfp_device_t *sfp, alt_u8 msa, alt_u8 start_address, int count, alt_u8* data);
#if IPGLB_LEGACY_CODE
ret_code_t get_sfp_id(sfp_device_t* _sfp);
ret_code_t get_sfp_temperature(sfp_device_t* sfp);
ret_code_t get_sfp_optical_power(sfp_device_t* sfp);
ret_code_t get_sfp_optical_power_thresholds(sfp_device_t* sfp);
#else
ret_code_t get_sfp_optical_power(transceiver_t* _trans);
ret_code_t get_sfp_optical_power_thresholds(transceiver_t* _trans);
ret_code_t get_sfp_id(transceiver_t* _trans);
ret_code_t get_sfp_temperature(transceiver_t* _trans);
#endif
ret_code_t get_sfp_info(transceiver_t* _trans);
ret_code_t get_sfp_diagnostics(transceiver_t* _trans);
ret_code_t sfp_com_check(sfp_device_t* sfp);
char* get_sfp_driver_version(void);
ret_code_t get_sfp_capabilities(transceiver_t* _trans);
#endif /*ENABLE_SFP_CODE*/
#endif /*_IPG_SFP_H_*/
