/****************************************************************************
 * IPG Photonics Corporation
 * copyright COPYRIGHT (C) 2015 IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *
 * IPG Photonics Corporation
 * 50 Old Webster Road
 * Oxford, MA 01540, USA
 ****************************************************************************/
/**
 * @file ipg_sfpp.c
 * @author Clara Zaiter	(czaayter@ipgphotonics.com)
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @date Feb 20, 2020
 * @version 0.0.8
 *
 * @brief This file contains the functions needed to access an SFP+ device and get information from it.
 *
 * @see http://www.sfplustransceiver.com/pdf/MSA-SFF-8431.pdf
 */

#include "../../configs.h"

#if ENABLE_SFPP_CODE
#include "../inc/ipg_sfpp.h"
#include "../inc/ipg_utils.h"
#include "../inc/ipg_constants.h"
#include "../inc/ipg_bits.h"
#include "string.h"


#ifndef IPGLB_LEGACY_CODE
#define IPGLB_LEGACY_CODE  1
#endif
/*
 * local defines
 */
#define MSA_0						0xA0//!<Constant used as SFP+ I2C address when we want to read from the SID table.
#define MSA_1						0xA2//!<Constant used as SFP+ I2C address when we want to read from the DDI table.
#define SID							MSA_0
#define DDI							MSA_1
#define PAGE_TUNABLE_SFPP_CONTROL_STATUS		0x02	/*ID of the TUnable SFP+ Control/Status page. @see SFF-8690*/

/**
 * This function changes the device's MSA.
 * @param[in, out] 	sfpp	pointer to the SFP+ device.
 * @param[in]		msa		msa table to be selected.
 */
void sfpp_select_msa(sfpp_device_t* sfpp, alt_u8 msa)
{
	sfpp->i2c_dev.address = msa;
}

/**Enumeration of the SDI table registers.
 * The SDI table (common to both SFP and SFP+) can be accessed through I2C address \c MSA_0 (0xA0).
 */
enum SFPP_SID_REGISTERS{
	SFPP_SID_VENDOR_NAME_REG = 20,	/**<Offset of the SFP+ vendor name's first character. Vendor name string is 16 characters long*/
	SFPP_SID_PART_NUMBER_REG = 40,	/**<Offset of the SFP+ part number's first character. Part number string is 16 characters long*/
	SFPP_SID_REVISION_NUMBER_REG = 56,/**<Offset of the SFP revision number's first character. Revision number string is 4 characters long*/
	SFPP_SID_SERIAL_NUMBER_REG = 68,	/*!<Offset of the SFP+ serial number's first character. Serial number string is 16 characters long*/
	SFPP_SID_OPTIONS_VALUE_REG = 65,	/*!<Offset of the SFP+ OPTIONS value register. @see SFF-8472 and @see SFF-8690 (https://doc.xdevs.com/doc/Seagate/SFF-8690.PDF)*/
	SFPP_SID_DIAG_MONITORING_TYPE_REG = 92,	/**<Offset of the SFP+ Diagnostics Monitoring Type register. @see SFF-8472 (http://www.10gtek.com/templates/wzten/pdf/SFF-8472-(Diagnostic%20Monitoring%20Interface).pdf)*/
	SFPP_SID_SFF_8472_COMPLIANCE_REG = 94,	/**<Offset of the SFP+ serial number's first character. Serial number string is 16 characters long*/
	SFPP_SID_PAGE_SELECT_REG = 127	/**<Offset of the SFP+ Page Select Byte.*/
};

#define SFPP_USER_EEPROM_REGISTER	128

/**Enumeration of the DDI table registers.
 * Compared to an SFP, an SFP+ has an additional table that can be accessed through I2C address \c MSA_1 (0xA2).
 */
enum SFPP_DDI_REGISTERS{
	SFPP_DDI_TX_PWR_HA_MSB 	= 24,	/**<Offset of the SFP+ DDI table where the MSB part of the TX optical power High Alarm is stored.*/
	SFPP_DDI_TEMPERATURE_MSB_REG = 96,
	SFPP_DDI_TX_PWR_MSB_REG = 102,	/**<Offset of the SFP+ DDI table where the MSB part of the TX optical power is stored.*/
	SFPP_DDI_TX_PWR_LSB_REG = 103,	/**<Offset of the SFP+ DDI table where the LSB part of the TX optical power is stored.*/
	SFPP_DDI_RX_PWR_MSB_REG = 104,	/**<Offset of the SFP+ DDI table where the MSB part of the RX optical power is stored.*/
	SFPP_DDI_RX_PWR_LSB_REG = 105,	/**<Offset of the SFP+ DDI table where the LSB part of the RX optical power is stored.*/
	SFPP_DDI_DITHERING_REG = 128,	/**<Offset of the SFP+ Dithering register.*/
	SFPP_DDI_MODULE_CAPABILITIES_REG = 132,	/**<Base offset of the SFP+ module capabilities, this is where the Lasers first frequency is set if it's a tunable transmitter*/
};

/*
 * local prototypes
 */
#if IPGLB_LEGACY_CODE
ret_code_t get_sfpp_tuning_info(sfpp_device_t* sfpp);
ret_code_t get_sfpp_capabilities(sfpp_device_t* sfpp);
#else
ret_code_t get_sfpp_tuning_info(transceiver_t* _trans);
ret_code_t get_sfpp_capabilities(transceiver_t* _trans);
#endif
/*
 * extern variables
 */
static const char* version = "0.0.8";
/**
 * This function gets the driver's current version.
 * Implemented in revision 0.0.5
 */
char* get_sfpp_driver_version(void){
	return (version);
}

/**This function reads a single byte from a register from an SFP+
 * @param[in] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @param[in] offset address of register in the SFP+ table to read from
 * @param[out] data location where to store the read data
 * @return \c RET_SUCCESS on success
 */
ret_code_t sfpp_read_data (const sfpp_device_t* sfpp, alt_u8 offset, alt_u8* data){
	int result = sls_read(sfpp->i2c_dev.bus->fd, sfpp->i2c_dev.address, data, offset, 1, 1);
	if(result == 0){
		*data = 0;
		return 	(RET_IO_FAIL);
	}
	return 	(RET_SUCCESS);
}

/**This function reads a block of registers from an SFP+.
 * It reads a number of successive registers in a single I2C transaction.
 * @param[in] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @param[in] start_address the first register address to be read
 * @param[in] count	number of successive registers to be read
 * @param[out] data location where to store the read data
 * @return \c RET_SUCCESS on success
 */
ret_code_t sfpp_read_data_block (const sfpp_device_t* sfpp, alt_u8 start_address, int count, alt_u8* data){
	int result = sls_read(sfpp->i2c_dev.bus->fd, sfpp->i2c_dev.address, data, start_address, count, 2);
	if(result == 0){
		memset(data, 0, count);
		return (RET_IO_FAIL);
	}
	return (RET_SUCCESS);
}

/**This function Writes a single data byte to a register in an SFP+.
 * @param[in] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @param[in] offset address of register in the SFP+ table to write to
 * @param[in] data data that has to be written
 * @return \c RET_SUCCESS on success
 */
ret_code_t sfpp_write_data(const sfpp_device_t* sfpp, alt_u8 offset, alt_u8 data){
	int result = sls_write(sfpp->i2c_dev.bus->fd, sfpp->i2c_dev.address, (char*)&data, offset, 1);
	if(result ==0){
		return 		(RET_IO_FAIL);
	}
	return 	(RET_SUCCESS);
}
/**This function Writes a bloc of data to registers inside an SFP+.
 * @param[in] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @param[in] offset address of register in the SFP+ table to write to
 * @param[in] data data that has to be written
 * @return \c RET_SUCCESS on success
 */
ret_code_t sfpp_write_block(const sfpp_device_t* sfpp, alt_u8 offset, int count, alt_u8* data){
	int result = sls_write(sfpp->i2c_dev.bus->fd, sfpp->i2c_dev.address, (char*)&data, offset, count);
	if(result ==0){
		return 		(RET_IO_FAIL);
	}
	return 	(RET_SUCCESS);
}

/**
 * This function selects a specific page in the SFP+.
 * @param[in, out] 	sfpp_device_t	pointer to the SFP+
 * @param[in] 		page	page to be selected.
 */
ret_code_t sfpp_select_page(sfpp_device_t* sfpp, alt_u32 page){
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	/*pages only exist in MSA1 (0xA2)*/
	sfpp_select_msa(sfpp, MSA_1);
	return sfpp_write_data(sfpp, SFPP_SID_PAGE_SELECT_REG, page);
}

/**This helper function handles users' requests to read a byte from an SFP++.
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @param[in] msa which MSA table to read from
 *  @arg MSA_0
 *  @arg MSA_1
 * @param[in] offset address of register in the SFP+ table to read from
 * @param[out] data location where to store the read data
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_sfpp_read(sfpp_device_t *sfpp, alt_u8 msa, alt_u8 offset, alt_u8* data){
#if IPGLB_LEGACY_CODE
	if(sfpp->absent.current)return (RET_IO_FAIL);
#endif
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	sfpp_select_msa(sfpp, msa);
	return 	(sfpp_read_data(sfpp, offset, data));
}

/**This helper function handles users' requests to write a byte to an SFP++.
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @param[in] msa which MSA table to read from
 *  @arg MSA_0
 *  @arg MSA_1
 * @param[in] offset address of register in the SFP+ table to write to
 * @param[in] data data to be written
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_sfpp_write(sfpp_device_t *sfpp, alt_u8 msa, alt_u8 offset, alt_u8 data){
#if IPGLB_LEGACY_CODE
	if(sfpp->absent.current)return (RET_IO_FAIL);
#endif
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	sfpp_select_msa(sfpp, msa);
	return 	(sfpp_write_data(sfpp, offset, data));
}

/**This function reads the vendor name info of an SFP+.
 * It then updates the proper field inside the SFP+ data structure
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_vn(sfpp_device_t* sfpp){
	ret_code_t ret = RET_FAIL;
#if IPGLB_LEGACY_CODE
	if(sfpp->absent.current)return (RET_IO_FAIL);
#endif
	alt_u8 sfp_vn[VN_LENGTH];
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_0);
	ret = sfpp_read_data_block(sfpp, SFPP_SID_VENDOR_NAME_REG, VN_LENGTH, sfp_vn);
	if(ret != RET_SUCCESS){
		//printf("SFP VN fail: i2c block read failed\n");
		memset(sfpp->vendor_name, 0, VN_LENGTH);
	}
	memcpy(sfpp->vendor_name, sfp_vn, VN_LENGTH);
	/*terminate the string*/
	sfpp->vendor_name[VN_LENGTH-1] = 0;
	return 	(ret);
}

/**This function reads the serial number info of an SFP+.
 * It then updates the proper field inside the SFP+ data structure
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_sn(sfpp_device_t* sfpp){
	ret_code_t ret = RET_FAIL;
#if IPGLB_LEGACY_CODE
	if(sfpp->absent.current)return (RET_IO_FAIL);
#endif
	alt_u8 sfp_sn[SN_LENGTH];
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_0);
	ret = sfpp_read_data_block(sfpp, SFPP_SID_SERIAL_NUMBER_REG, SN_LENGTH, sfp_sn);
	if(ret != RET_SUCCESS)	{
		// printf("SFP SN fail: i2c block read failed\n");
		memset(sfpp->serial_number, 0, SN_LENGTH);
	}
	memcpy(sfpp->serial_number, sfp_sn, SN_LENGTH);
	sfpp->serial_number[SN_LENGTH-1] = 0;
	return 	(ret);
}

/**This function reads the part number info of an SFP+.
 * It then updates the proper field inside the SFP+ data structure
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_pn(sfpp_device_t* sfpp){
	ret_code_t ret = RET_FAIL;
#if IPGLB_LEGACY_CODE
	if(sfpp->absent.current)return (RET_IO_FAIL);
#endif
	alt_u8 sfp_pn[PN_LENGTH];
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_0);
	ret = sfpp_read_data_block(sfpp, SFPP_SID_PART_NUMBER_REG, PN_LENGTH, sfp_pn);
	if(ret != RET_SUCCESS){
		//printf("SFP PN fail: i2c block read failed\n");
		memset(sfpp->part_number, 0, PN_LENGTH);
	}
	memcpy(sfpp->part_number, sfp_pn, PN_LENGTH);
	sfpp->part_number[PN_LENGTH-1] = 0;
	return 	(ret);
}

/**This function reads the revision number info of an SFP.
 * It then updates the proper field inside the SFP data structure
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_rn(sfpp_device_t* sfpp){
	ret_code_t ret = RET_FAIL;
#if IPGLB_LEGACY_CODE
	if(sfpp->absent.current)return (RET_IO_FAIL);
#endif
	alt_u8 sfp_rn[RN_LENGTH];
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_0);
	ret = sfpp_read_data_block(sfpp, SFPP_SID_REVISION_NUMBER_REG, RN_LENGTH, sfp_rn);
	if(ret != RET_SUCCESS){
		memset(sfpp->revision_number, 0, RN_LENGTH);
	}
	memcpy(sfpp->revision_number, sfp_rn, RN_LENGTH);
	sfpp->revision_number[RN_LENGTH-1] = 0;
	return 	(ret);
}
#if IPGLB_LEGACY_CODE
/**Initializes the data structures holding the SFP static and dynamic information
 * @param[in,out] sfpp pointer to the SFP+ data structure that we need to initialize
 * @return None
 */
void init_sfpp_data(sfpp_device_t* sfpp){
	sfpp->absent.previous = sfpp->absent.current = 1;
	sfpp->los.previous = 1;
	sfpp->tx_fault.previous = 1;
	sfpp->connector = SFPP_CONNECTOR_UNKNOWN;
	sfpp->tx_disable.current = sfpp->tx_disable.next = 0;
	memset(&sfpp->optical_pwr, 	0, 	sizeof(optical_pwr_t)) ;
	memset(sfpp->vendor_name, 	0, 	sizeof(sfpp->vendor_name));
	memset(sfpp->part_number, 	0, 	sizeof(sfpp->part_number));
	memset(sfpp->serial_number, 	0, 	sizeof(sfpp->serial_number));
	memset(sfpp->revision_number, 	0, 	sizeof(sfpp->revision_number));
	memset(sfpp->ID, 	0, 	sizeof(sfpp->ID));
	sprintf(sfpp->ID, "|||");
}
/**This function reads the RX and TX optical power alarms and warning levels of the selected SFP+
 * 			and updates the corresponding fields in the data structures
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 * @since 0.0.2
 */
ret_code_t get_sfpp_optical_power_thresholds(sfpp_device_t* sfpp){
	ret_code_t retval;
	alt_u8 power_data[16];
	if(sfpp->absent.current){ // if the SFP is absent then all optical power should be set to 0
		memset(sfpp->optical_pwr.pwr_thr, 0, 16);
		sfpp->optical_pwr.input_cmax = -infinityf();
		sfpp->optical_pwr.input_cmin = -infinityf();
		sfpp->optical_pwr.input_wmax = -infinityf();
		sfpp->optical_pwr.input_wmin = -infinityf();
		sfpp->optical_pwr.output_cmax = -infinityf();
		sfpp->optical_pwr.output_cmin = -infinityf();
		sfpp->optical_pwr.output_wmax = -infinityf();
		sfpp->optical_pwr.output_wmin = -infinityf();
		return 		(RET_SUCCESS);
	}
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_1);
	retval = sfpp_read_data_block(sfpp, SFPP_DDI_TX_PWR_HA_MSB, 16, power_data);
	if(retval == RET_SUCCESS){
		// store the raw data values
		memcpy(sfpp->optical_pwr.pwr_thr, power_data, sizeof(power_data));
		// convert the values to dBm
		float fval = 0.00;
		fval = ((sfpp->optical_pwr.pwr_thr[0] << 8) | (sfpp->optical_pwr.pwr_thr[1])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.output_cmax = fval;
		fval = ((sfpp->optical_pwr.pwr_thr[2] << 8) | (sfpp->optical_pwr.pwr_thr[3])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.output_cmin = fval;
		fval = ((sfpp->optical_pwr.pwr_thr[4] << 8) | (sfpp->optical_pwr.pwr_thr[5])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.output_wmax = fval;
		fval = ((sfpp->optical_pwr.pwr_thr[6] << 8) | (sfpp->optical_pwr.pwr_thr[7])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.output_wmin = fval;

		fval = ((sfpp->optical_pwr.pwr_thr[8] << 8) | (sfpp->optical_pwr.pwr_thr[9])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.input_cmax = fval;
		fval = ((sfpp->optical_pwr.pwr_thr[10] << 8) | (sfpp->optical_pwr.pwr_thr[11])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.input_cmin = fval;
		fval = ((sfpp->optical_pwr.pwr_thr[12] << 8) | (sfpp->optical_pwr.pwr_thr[13])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.input_wmax = fval;
		fval = ((sfpp->optical_pwr.pwr_thr[14] << 8) | (sfpp->optical_pwr.pwr_thr[15])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.input_wmin = fval;
	}
	return 	(retval);
}


/**This function reads the internal temperature of an SFP+.
 * It then updates the proper field inside the SFP data structure
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_temperature(sfpp_device_t* sfpp){
	ret_code_t retval;
	alt_u8 t[2];;
	if(sfpp->absent.current){ // if the SFP is absent then all optical power should be set to 0
		return 		(RET_IO_FAIL);
	}
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_1);
	retval = sfpp_read_data_block(sfpp, SFPP_DDI_TEMPERATURE_MSB_REG, 2, t);
	if(retval == RET_SUCCESS){
		sfpp->temperature = t[0] + t[1]/256.0;
	}
	return 	(retval);
}

/**This function reads the TX and RX optical power values of an SFP+.
 * It then updates the proper field inside the SFP+ data structure
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_optical_power(sfpp_device_t* sfpp){
	ret_code_t retval;
	alt_u8 power_data[4];
	if(sfpp->absent.current){ // if the SFP+ is absent then all optical power should be set to 0
		memset(sfpp->optical_pwr.pwr, 0, 4);
		sfpp->optical_pwr.input = -infinityf();
		sfpp->optical_pwr.output = -infinityf();
		return 		(RET_SUCCESS);
	}
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_1);
	retval = sfpp_read_data_block(sfpp, SFPP_DDI_TX_PWR_MSB_REG, 4, power_data);
	if(retval == RET_SUCCESS){
		// store raw data values
		memcpy(sfpp->optical_pwr.pwr, power_data, sizeof(power_data));
		// convert the values to dBm
		float fval = 0.00;
		fval = ((sfpp->optical_pwr.pwr[0] << 8) | (sfpp->optical_pwr.pwr[1])) * 0.1f;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.output = fval;
		fval = ((sfpp->optical_pwr.pwr[2] << 8) | (sfpp->optical_pwr.pwr[3])) * 0.1;
		fval = uW_to_dBm(fval);
		sfpp->optical_pwr.input = fval;
	}
	return 	(retval);
}

ret_code_t get_sfpp_id(sfpp_device_t* _sfpp){
	if(_sfpp->absent.current)return (RET_IO_FAIL);
	if(get_sfpp_vn(_sfpp) != RET_SUCCESS){
		return (RET_FAIL);
	}
	if(get_sfpp_sn(_sfpp) != RET_SUCCESS){
		return (RET_FAIL);
	}
	if(get_sfpp_pn(_sfpp) != RET_SUCCESS){
		return (RET_FAIL);
	}
	if(get_sfpp_rn(_sfpp) != RET_SUCCESS){
		return (RET_FAIL);
	}
	rtrim(_sfpp->vendor_name);
	rtrim(_sfpp->serial_number);
	rtrim(_sfpp->part_number);
	rtrim(_sfpp->revision_number);
	sprintf(_sfpp->ID, "%s|%s|%s|%s", _sfpp->vendor_name, _sfpp->serial_number, _sfpp->part_number, _sfpp->revision_number);
	return (RET_SUCCESS);

}
#if IPGLB_LEGACY_CODE
/**
 * This function reads the tuning information of an SFP+ transceiver.
 * If the SFP+ is tunable, this function reads the laser's first and last frequency as
 * well as the grid spacing.
 * @param[in,out] _trans pointer to the SFP+ transceiver object
 * @return \c RET_SUCCESS on success.
 * @note the function has to select the Tunable SFP+ Control/Status page prior to
 * reading these information from bytes 128-255 in DDI.
 *
 */
ret_code_t get_sfpp_tuning_info(sfpp_device_t* sfpp){
	if(sfpp->absent.current)
	{
		memset(&sfpp->tuning, 0, sizeof(sfpp->tuning));
		return (RET_IO_FAIL);
	}
	ret_code_t retval;
	alt_u8 tuning_info[10];
	retval = sfpp_select_page(sfpp, PAGE_TUNABLE_SFPP_CONTROL_STATUS);
	if(retval != RET_SUCCESS){
		return retval;
	}
	/*tuning methods support*/
	alt_u8 val = 0;
	Delay_ms(1);
	retval = sfpp_read_data(sfpp, SFPP_DDI_MODULE_CAPABILITIES_REG, &val);
	if(retval != RET_SUCCESS){
		return retval;
	}
	sfpp->capabilities.tune_by_channel = BIT_IS_SET(val, BIT1);
	sfpp->capabilities.tune_by_wl = BIT_IS_SET(val, BIT0);
	Delay_ms(1);
	/*laser's first, last frequencies and grid spacing*/
	retval = sfpp_read_data_block(sfpp, SFPP_DDI_MODULE_CAPABILITIES_REG, 10, tuning_info);
	if(retval == RET_SUCCESS){
		sfpp->tuning.first_freq = ((tuning_info[0] << 8) | tuning_info[1]) + ((tuning_info[2] << 8) | tuning_info[3])/10000.0;
		sfpp->tuning.last_freq = ((tuning_info[4] << 8) | tuning_info[5]) + ((tuning_info[6] << 8) | tuning_info[7])/10000.0;
		sfpp->tuning.grid_spacing = ((tuning_info[8] << 8) | tuning_info[9])/10000.0;
	}
	/*go back to the default page*/
	return 	sfpp_select_page(sfpp, 0);
}

ret_code_t get_sfpp_capabilities(sfpp_device_t* sfpp){
	ret_code_t retval = RET_FAIL;
	if(sfpp->absent.current)
	{
		return (RET_IO_FAIL);
	}
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	alt_u8 val;
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_0);
	retval = sfpp_read_data(sfpp, SFPP_SID_DIAG_MONITORING_TYPE_REG, &val);
	if(retval == RET_SUCCESS){
		sfpp->capabilities.TEMPERATURE_MONITOR = BIT_IS_SET(val, BIT6); /*if b6 is set, that means digital diagnostics monitoring is implemented @see SFF-8472 http://www.10gtek.com/templates/wzten/pdf/SFF-8472-(Diagnostic%20Monitoring%20Interface).pdf*/
		sfpp->capabilities.OPTICAL_POWER_MONITOR = sfpp->capabilities.TEMPERATURE_MONITOR;
		/*does it support tuning?*/
		Delay_ms(1);
		retval = sfpp_read_data(sfpp, SFPP_SID_OPTIONS_VALUE_REG, &val);
		if(retval == RET_SUCCESS){
			sfpp->capabilities.tx_tunable = BIT_IS_SET(val, BIT6); /*if b6 is set, that means transmitter is tunable*/
			if(sfpp->capabilities.tx_tunable){
				retval = get_sfpp_tuning_info(sfpp);
			}
			sfpp->capabilities.rx_tunable = 0;
		}
	}
	/*SFP+ have a single internal optical IO*/
	sfpp->capabilities.optical_ios_number = 1;
	return retval;
}
#endif
#else
/**Initializes the data structures holding the SFP static and dynamic information
 * @param[in,out] sfpp pointer to the SFP+ data structure that we need to initialize
 * @return None
 */
void init_sfpp_data(sfpp_device_t* sfpp){
	sfpp->connector = SFPP_CONNECTOR_UNKNOWN;
	memset(sfpp->vendor_name, 	0, 	sizeof(sfpp->vendor_name));
	memset(sfpp->part_number, 	0, 	sizeof(sfpp->part_number));
	memset(sfpp->serial_number, 	0, 	sizeof(sfpp->serial_number));
	memset(sfpp->revision_number, 	0, 	sizeof(sfpp->revision_number));
}

ret_code_t get_sfpp_id(transceiver_t* _trans){
	sfpp_device_t* _sfpp = (sfpp_device_t*)(_trans->device.device);
	ret_code_t rc = RET_SUCCESS;
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfpp_vn(_sfpp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfpp_sn(_sfpp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfpp_pn(_sfpp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfpp_rn(_sfpp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	rtrim(_sfpp->vendor_name);
	rtrim(_sfpp->serial_number);
	rtrim(_sfpp->part_number);
	rtrim(_sfpp->revision_number);
	sprintf(_trans->info.ID, "%s|%s|%s|%s", _sfpp->vendor_name, _sfpp->serial_number, _sfpp->part_number, _sfpp->revision_number);
#ifdef KURS_PROTOCOL
	sprintf(_trans->info.VN, "%s", _sfpp->vendor_name);
	sprintf(_trans->info.SN, "%s", _sfpp->serial_number);
#endif
	return rc;
}

ret_code_t get_sfpp_temperature(transceiver_t* _trans){
	if(_trans->monitor.absent.current){
		return RET_ABSENT;
	}
	sfpp_device_t* _sfpp = (sfpp_device_t*)(_trans->device.device);
	ret_code_t retval;
	alt_u8 t[2];
	select_i2c_channel_on_bus(_sfpp->i2c_dev.bus, _sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(_sfpp, MSA_1);
	retval = sfpp_read_data_block(_sfpp, SFPP_DDI_TEMPERATURE_MSB_REG, 2, t);
	if(retval == RET_SUCCESS){
		_trans->monitor.temperature.value = (alt_8)t[0] + t[1]/256.0;
	}
	return retval;
}

/**This function reads the RX and TX optical power alarms and warning levels of the selected SFP+
 * 			and updates the corresponding fields in the data structures
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 * @since 0.0.2
 */
ret_code_t get_sfpp_optical_power_thresholds(transceiver_t* _trans){
	sfpp_device_t* sfpp = (sfpp_device_t*)(_trans->device.device);
	ret_code_t retval;
	alt_u8 power_data[16];
	if(_trans->monitor.absent.current){ // if the SFP is absent then all optical power should be set to 0
		_trans->monitor.input_power[0].critical_max = -infinityf();
		_trans->monitor.input_power[0].critical_min = -infinityf();
		_trans->monitor.input_power[0].warning_max = -infinityf();
		_trans->monitor.input_power[0].warning_min = -infinityf();
		_trans->monitor.output_power[0].critical_max = -infinityf();
		_trans->monitor.output_power[0].critical_min = -infinityf();
		_trans->monitor.output_power[0].warning_max = -infinityf();
		_trans->monitor.output_power[0].warning_min = -infinityf();
		return 		(RET_SUCCESS);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_1);
	retval = sfpp_read_data_block(sfpp, SFPP_DDI_TX_PWR_HA_MSB, 16, power_data);
	if(retval == RET_SUCCESS){
		// convert the values to dBm
		float fval = 0.00;
		fval = ((power_data[0] << 8) | power_data[1]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.output_power[0].critical_max = fval;
		fval = ((power_data[2] << 8) | power_data[3]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.output_power[0].critical_min = fval;
		fval = ((power_data[4] << 8) | power_data[5]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.output_power[0].warning_max = fval;
		fval = ((power_data[6] << 8) | power_data[7]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.output_power[0].warning_min = fval;

		fval = ((power_data[8] << 8) | power_data[9]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.input_power[0].critical_max = fval;
		fval = ((power_data[10] << 8) | power_data[11]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.input_power[0].critical_min = fval;
		fval = ((power_data[12] << 8) | power_data[13]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.input_power[0].warning_max = fval;
		fval = ((power_data[14] << 8) | power_data[15]) * 0.1;
		fval = uW_to_dBm(fval);
		_trans->monitor.input_power[0].warning_min = fval;
	}
	return 	(retval);
}

/**This function reads the TX and RX optical power values of an SFP+.
 * It then updates the proper field inside the SFP+ data structure
 * @param[in,out] sfpp pointer to the SFP+ data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_optical_power(transceiver_t* _trans){
	sfpp_device_t* sfpp = (sfpp_device_t*)(_trans->device.device);
	ret_code_t retval;
	alt_u8 power_data[4];
	if(_trans->monitor.absent.current){ // if the SFP+ is absent then all optical power should be set to 0
		_trans->monitor.input_power[0].value = -infinityf();
		_trans->monitor.output_power[0].value = -infinityf();
		return 		(RET_ABSENT);
	}
	_trans->busy = 1;
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_1);
	retval = sfpp_read_data_block(sfpp, SFPP_DDI_TX_PWR_MSB_REG, 4, power_data);
	if(retval == RET_SUCCESS){
		// convert the values to dBm
//		float fval = ((power_data[0] << 8) | power_data[1]) * 0.1;
//		_trans->monitor.output_power[0].value = uW_to_dBm(fval);
//		fval = ((power_data[2] << 8) | power_data[3]) * 0.1;
//		_trans->monitor.input_power[0].value = uW_to_dBm(fval);
		_trans->monitor.output_power[0].value = UW_TO_DBM(((power_data[0] << 8) | power_data[1]) * 0.1);
		_trans->monitor.input_power[0].value = UW_TO_DBM(((power_data[2] << 8) | power_data[3]) * 0.1);
	}
	_trans->busy = 0;
	return 	(retval);
}

ret_code_t get_sfpp_diagnostics(transceiver_t* _trans){
	ret_code_t retval;
	if(_trans->monitor.absent.current){
		return RET_ABSENT;
	}
	if(_trans->info.capabilities.OPTICAL_POWER_MONITOR){
		retval = get_sfpp_optical_power(_trans);
		if(retval != RET_SUCCESS){
			return retval;
		}
		return get_sfpp_temperature(_trans);
	}
	return RET_INV_CMD;
}

/**
 * This function reads the tuning information of an SFP+ transceiver.
 * If the SFP+ is tunable, this function reads the laser's first and last frequency as
 * well as the grid spacing.
 * @param[in,out] _trans pointer to the SFP+ transceiver object
 * @return \c RET_SUCCESS on success.
 * @note the function has to select the Tunable SFP+ Control/Status page prior to
 * reading these information from bytes 128-255 in DDI.
 *
 */
ret_code_t get_sfpp_tuning_info(transceiver_t* _trans){
	sfpp_device_t* sfpp = (sfpp_device_t*)(_trans->device.device);
	ret_code_t retval;
	alt_u8 tuning_info[10];
	if(_trans->monitor.absent.current){ // if the SFP+ is absent then all optical power should be set to 0
		memset(&_trans->info.tuning, 0, sizeof(_trans->info.tuning));
		return 		(RET_ABSENT);
	}
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	Delay_ms(1);
	/*to access the tuning information, we need to select page 2 (TUNABLE SFP+ Control/Status) in DDI*/
	sfpp_select_msa(sfpp, MSA_1);
	retval = sfpp_select_page(sfpp, PAGE_TUNABLE_SFPP_CONTROL_STATUS);
	if(retval != RET_SUCCESS){
		return retval;
	}
	/*tuning methods support*/
	alt_u8 val = 0;
	Delay_ms(1);
	retval = sfpp_read_data(sfpp, SFPP_DDI_MODULE_CAPABILITIES_REG, &val);
	if(retval != RET_SUCCESS){
		return retval;
	}
	_trans->info.capabilities.tune_by_channel = BIT_IS_SET(val, BIT1);
	_trans->info.capabilities.tune_by_wl = BIT_IS_SET(val, BIT0);
	Delay_ms(1);
	/*laser's first, last frequencies and grid spacing*/
	retval = sfpp_read_data_block(sfpp, SFPP_DDI_MODULE_CAPABILITIES_REG, 10, tuning_info);
	if(retval == RET_SUCCESS){
//#error the first/last frequencies and grid spacing decimal parts are multiple of 0.1GHZ and should be divided by an additional 1000
		_trans->info.tuning.first_freq = ((tuning_info[0] << 8) | tuning_info[1]) + ((tuning_info[2] << 8) | tuning_info[3])/10.0;
		_trans->info.tuning.last_freq = ((tuning_info[4] << 8) | tuning_info[5]) + ((tuning_info[6] << 8) | tuning_info[7])/10.0;
		_trans->info.tuning.grid_spacing = ((tuning_info[8] << 8) | tuning_info[9])/10.0;
	}
	/*go back to the default page*/
	return 	sfpp_select_page(sfpp, 0);
}


ret_code_t get_sfpp_capabilities(transceiver_t* _trans){
	ret_code_t retval = RET_FAIL;
	sfpp_device_t* sfpp = (sfpp_device_t*)(_trans->device.device);
	if(_trans->monitor.absent.current){
			return RET_ABSENT;
		}
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	alt_u8 val;
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_0);
	retval = sfpp_read_data(sfpp, SFPP_SID_DIAG_MONITORING_TYPE_REG, &val);
	if(retval == RET_SUCCESS){
		_trans->info.capabilities.TEMPERATURE_MONITOR = BIT_IS_SET(val, BIT6); /*if b6 is set, that means digital diagnostics monitoring is implemented @see SFF-8472 http://www.10gtek.com/templates/wzten/pdf/SFF-8472-(Diagnostic%20Monitoring%20Interface).pdf*/
		_trans->info.capabilities.OPTICAL_POWER_MONITOR = _trans->info.capabilities.TEMPERATURE_MONITOR;
		/*does it support tuning?*/
		Delay_ms(1);
		retval = sfpp_read_data(sfpp, SFPP_SID_OPTIONS_VALUE_REG, &val);
		if(retval == RET_SUCCESS){
			_trans->info.capabilities.tx_tunable = BIT_IS_SET(val, BIT6); /*if b6 is set, that means transmitter is tunable*/
			if(_trans->info.capabilities.tx_tunable){
				retval = get_sfpp_tuning_info(_trans);
			}
			_trans->info.capabilities.rx_tunable = 0;
		}
	}
	/*SFP+ have a single internal optical IO*/
	_trans->info.capabilities.optical_ios_number = 1;
	return retval;
}
/**This function reads information  of an SFP+.
 * This include ID, capabilities and tuning parameters (if SFP+ is tunable).
 * @param[in,out] _trans pointer to the transceiver object
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfpp_info(transceiver_t* _trans){
	ret_code_t retval = RET_FAIL;
	if(_trans->monitor.absent.current){
		memset(&_trans->info.capabilities, 0, sizeof(transceiver_capabilities_t));
		return RET_IO_FAIL;
	}
	/*first read SFP+ ID*/
	retval = get_sfpp_id(_trans);
	if(retval == RET_SUCCESS){
		/*does it support digital diagnostics i.e optical power and temperature measurements*/
		retval = get_sfpp_capabilities(_trans);
	}
	return 	retval;
}
#define RD_LOS(sfpp)(get_bit_state(sfpp->io_los.address, sfpp->io_los.bit))
#define RD_ABSENT(sfpp)(get_bit_state(sfpp->io_absent.address, sfpp->io_absent.bit))


ret_code_t sfpp_com_check(sfpp_device_t* sfpp){
#define SPECIAL_CODE "CEDRUS"
#define SPECIAL_CODE_LEN 6
	ret_code_t retval = RET_FAIL;
	select_i2c_channel_on_bus(sfpp->i2c_dev.bus, sfpp->i2c_dev.channel);
	alt_u8 special_code[SPECIAL_CODE_LEN];
	Delay_ms(1);
	sfpp_select_msa(sfpp, MSA_1);
	retval = sfpp_read_data_block(sfpp, SFPP_USER_EEPROM_REGISTER, SPECIAL_CODE_LEN, special_code);
	if(retval == RET_SUCCESS){
		if(memcmp(special_code, SPECIAL_CODE, SPECIAL_CODE_LEN) == 0){
			return RET_SUCCESS;
		}else{
			/*maybe this SFP+ has been plugged for the first time*/
			memcpy(special_code, SPECIAL_CODE, SPECIAL_CODE_LEN);
			/*so we will write to write the special code into user EEPROM*/
			sfpp_write_block(sfpp, SFPP_USER_EEPROM_REGISTER, SPECIAL_CODE_LEN, special_code);
			/*if we read back what we wrote than*/
			sfpp_read_data_block(sfpp, SFPP_USER_EEPROM_REGISTER, SPECIAL_CODE_LEN, special_code);
			if(memcmp(special_code, SPECIAL_CODE, SPECIAL_CODE_LEN) == 0){
				return RET_SUCCESS;
			}
			/*TODO: if reading is successful but the checking fails then retval will be RET_SUCCESS
			 * and this has to be fixed
			 * */
		}
	}
	return retval;
}
#endif


#endif /*ENABLE_SFPP_CODE*/

