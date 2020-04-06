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
 * @file ipg_sfpp.h
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @date Jan 20, 2015
 * @brief Header file for ipg_sfpp.c.
 *
 * Contains prototypes and type definitions for SFP+ needed by other files.
 * @version 0.0.4
 */

#ifndef _IPG_SFPP_H_
#define _IPG_SFPP_H_

#define IPGLB_LEGACY_CODE 0
#include "../../configs.h"

#ifdef ENABLE_SFPP_CODE
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

typedef enum{
	SFPP_CONNECTOR_UNKNOWN = 0,
	SFPP_CONNECTOR_SC,
	SFPP_CONNECTOR_LC = 7
}sfpp_connector_t;

/**Type representing the optical power of an SFP+ device.
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
	float input_cmin;
	float input_wmin;
	float input_cmax;
	float input_wmax;
	float output_cmin;
	float output_wmin;
	float output_cmax;
	float output_wmax;
	float output;
	float input;
}optical_pwr_t;

#if IPGLB_LEGACY_CODE
/**Type representing an SFP+ device.
 *
 */
typedef struct{
	transceiver_t* _trans;
	i2c_device_t i2c_dev;		/**<The I2C properties of the SFP+ device.*/
	// static
	char vendor_name[17];		/**<Pointer to the vendor name string.*/
	char serial_number[17];		/**<Pointer to the serial number string.*/
	char part_number[17];		/**<Pointer to the part number string.*/
	char revision_number[5];	/**<Pointer to the revision number string.*/
	char ID[55];				/**<Pointer to the ID string which is the combination of VN, SN, PN and RN (appended in our code).*/
	// control
	ipg_control_t tx_disable;	/**<Control variable for TX disable of the SFP+ device.*/
	// status
	ipg_status_t los;			/**<Status variable for the LOS alarm detected by the SFP+.*/
	ipg_status_t absent;		/**<Status variable for the absent flag of the SFP+.*/
	ipg_status_t tx_fault;		/**<Status variable for the TX fault alarm of the SFP+.*/
	// dynamic
	optical_pwr_t optical_pwr;/**<Instance of the optical power data structure of the SFP+.*/
	int index;					/**<Index of the SFP+ on our platform.*/
	sfpp_connector_t connector;
	float temperature;
	transceiver_capabilities_t	capabilities;
	tuning_info_t				tuning;
}sfpp_device_t;
#else
typedef struct sfpp_device sfpp_device_t;
struct sfpp_device{
	int index;					/**<Index of the SFP+ on our platform.*/
	transceiver_t* super;
	i2c_device_t i2c_dev;		/**<The I2C properties of the SFP+ device.*/
	// static
	char vendor_name[17];		/**<Pointer to the vendor name string.*/
	char serial_number[17];		/**<Pointer to the serial number string.*/
	char part_number[17];		/**<Pointer to the part number string.*/
	char revision_number[5];	/**<Pointer to the revision number string.*/
	sfpp_connector_t connector;
};
#endif
/*
 * exported prototypes
 */
void init_sfpp_data(sfpp_device_t* sfpp);
ret_code_t sfpp_read_data (const sfpp_device_t* sfpp, alt_u8 address, alt_u8* data);
ret_code_t sfpp_write_data(const sfpp_device_t* sfpp, alt_u8 address, alt_u8 data);
ret_code_t sfpp_write_block(const sfpp_device_t* sfpp, alt_u8 offset, int count, alt_u8* data);
ret_code_t sfpp_read_data_block (const sfpp_device_t* sfpp, alt_u8 start_address, int count, alt_u8* data);
ret_code_t handle_user_sfpp_read(sfpp_device_t* sfpp, alt_u8 msa, alt_u8 address, alt_u8* data);
ret_code_t handle_user_sfpp_write(sfpp_device_t* sfpp, alt_u8 msa, alt_u8 address, alt_u8 data);
ret_code_t get_sfpp_pn(sfpp_device_t* sfpp);
ret_code_t get_sfpp_sn(sfpp_device_t* sfpp);
ret_code_t get_sfpp_vn(sfpp_device_t* sfpp);
ret_code_t get_sfpp_rn(sfpp_device_t* sfpp);
#if IPGLB_LEGACY_CODE
ret_code_t get_sfpp_optical_power(sfpp_device_t* sfpp);
ret_code_t get_sfpp_optical_power_thresholds(sfpp_device_t* sfpp);
ret_code_t get_sfpp_id(sfpp_device_t* _sfpp);
ret_code_t get_sfpp_temperature(sfpp_device_t* _sfpp);
#else
ret_code_t get_sfpp_optical_power(transceiver_t* _trans);
ret_code_t get_sfpp_optical_power_thresholds(transceiver_t* _trans);
ret_code_t get_sfpp_id(transceiver_t* _trans);
ret_code_t get_sfpp_temperature(transceiver_t* _trans);
#endif
ret_code_t get_sfpp_info(transceiver_t* _trans);
ret_code_t get_sfpp_diagnostics(transceiver_t* _trans);
ret_code_t sfpp_com_check(sfpp_device_t* sfpp);
char* get_sfpp_driver_version(void);
ret_code_t sfpp_select_page(sfpp_device_t* sfpp, alt_u32 page);
void sfpp_select_msa(sfpp_device_t* sfpp, alt_u8 msa);

#endif /*ENABLE_SFPP_CODE*/
#endif /*_IPG_SFPP_H_*/
