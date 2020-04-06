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
 * @file ipg_sfp.c
 * @author Clara Zaiter	(czaayter@ipgphotonics.com)
 * @date Jan 14, 2016
 * @version 0.0.7
 * @brief This file contains the functions needed to access an SFP device and get information from it.
 *
 */
#include "../../configs.h"

#if ENABLE_SFP_CODE
#include "../inc/ipg_sfp.h"
#include "string.h"
#include "../inc/ipg_utils.h"
/*
 * local defines
 */
/*#define VN_LENGTH	17					//!<Constant representing the length of an SFP vendor name string.
#define SN_LENGTH	17					//!<Constant representing the length of an SFP serial number string.
#define PN_LENGTH	17					//!<Constant representing the length of an SFP part number string.
#define RN_LENGTH	5	 */				//!<Constant representing the length of an SFP revision number string.

/*
 * local macros
 */
#define SELECT_MSA(sfp, msa) sfp->i2c_dev.address = msa	//!<Macro used to select the MSA table of an SFP that we will be communicating with.

/**Enumeration of the SDI table registers.
 * The SDI table (common to both SFP and SFP) can be accessed through I2C address \c MSA_0 (0xA0).
 */
enum SFP_SID_REGISTERS{
	SFP_SID_IDENTIIFER_REG = 0,			/*!<Offset of the SFP identifier field*/
	SFP_SID_VENDOR_NAME_REG = 20,		/*!<Offset of the SFP vendor name's first character. Vendor name string is 16 characters long*/
	SFP_SID_PART_NUMBER_REG = 40,		/*!<Offset of the SFP part number's first character. Part number string is 16 characters long*/
	SFP_SID_REVISION_NUMBER_REG = 56,	/*!<Offset of the SFP revision number's first character. Revision number string is 4 characters long*/
	SFP_SID_CC_BASE_REG = 63,			/*!<Offset of the SFP CC BASE register (Check Code for base registers 0 to 62)*/
	SFP_SID_OPTIONS_VALUE_REG = 65,	/*!<Offset of the SFP OPTIONS value register. @see SFF-8472 and @see SFF-8690 (https://doc.xdevs.com/doc/Seagate/SFF-8690.PDF)*/
	SFP_SID_SERIAL_NUMBER_REG = 68,		/*!<Offset of the SFP serial number's first character. Serial number string is 16 characters long*/
	SFP_SID_DIAG_MONITORING_TYPE_REG = 92,	/*!<Offset of the SFP+ Diagnostics Monitoring Type register. @see SFF-8472 (http://www.10gtek.com/templates/wzten/pdf/SFF-8472-(Diagnostic%20Monitoring%20Interface).pdf)*/
};

/**Enumeration of the DDI table registers.
 * Compared to an SFP, an SFP has an additional table that can be accessed through I2C address \c MSA_1 (0xA2).
 */
enum SFP_DDI_REGISTERS{
	SFP_DDI_TX_PWR_HA_MSB 	= 24,	/**<Offset of the SFP DDI table where the MSB part of the TX optical power High Alarm is stored.*/
	SFP_DDI_TX_PWR_MSB_REG = 102,	/**<Offset of the SFP DDI table where the MSB part of the TX optical power is stored.*/
	SFP_DDI_TX_PWR_LSB_REG = 103,	/**<Offset of the SFP DDI table where the LSB part of the TX optical power is stored.*/
	SFP_DDI_RX_PWR_MSB_REG = 104,	/**<Offset of the SFP DDI table where the MSB part of the RX optical power is stored.*/
	SFP_DDI_RX_PWR_LSB_REG = 105,	/**<Offset of the SFP DDI table where the LSB part of the RX optical power is stored.*/
	SFP_DDI_TEMPERATURE_MSB_REG = 96,
};

const char* version = "0.0.4";
/**
 * This function gets the driver's current version.
 * Implemented in revision 0.0.4
 */
char* get_sfp_driver_version(void){
	return version;
}
/*
 * local prototypes
 */

/*
 * extern variables
 */

#if IPGLB_LEGACY_CODE
/**Initializes the data structures holding the SFP static and dynamic information
 * @param[in,out] sfp pointer to the SFP data structure that we need to initialize
 * @return None
 */
void init_sfp_data(sfp_device_t* sfp)
{
	sfp->absent.previous = sfp->absent.current = sfp->absent.sticky = 1;
	sfp->stable_los.previous = sfp->stable_los.current = sfp->stable_los.sticky = 1;
	sfp->los.previous = sfp->los.current = sfp->los.sticky = 1;
	sfp->tx_fault.previous = sfp->tx_fault.current = sfp->tx_fault.sticky = 1;
	sfp->connector = SFP_CONNECTOR_UNKNOWN;
	sfp->tx_disable.next = sfp->tx_disable.current = 0;
	memset(&sfp->optical_pwr, 	0, 	sizeof(sfp->optical_pwr)) ;
	memset(sfp->vendor_name, 	0, 	sizeof(sfp->vendor_name));
	memset(sfp->part_number, 	0, 	sizeof(sfp->part_number));
	memset(sfp->serial_number, 	0, 	sizeof(sfp->serial_number));
	memset(sfp->revision_number, 0, sizeof(sfp->revision_number));
	memset(sfp->ID, 0,  sizeof(sfp->ID));
	sprintf(sfp->ID, "|||");
}

ret_code_t get_sfp_id(sfp_device_t* _sfp){
	 if(_sfp->absent.current)
	 {
		 sprintf(_sfp->ID, "|||");
		 return (RET_IO_FAIL);
	 }
	 if(get_sfp_vn(_sfp) != RET_SUCCESS){
		 sprintf(_sfp->ID, "|||");
		 return (RET_FAIL);
	 }
	 if(get_sfp_sn(_sfp) != RET_SUCCESS){
		 sprintf(_sfp->ID, "|||");
		 return (RET_FAIL);
	 }
	 if(get_sfp_pn(_sfp) != RET_SUCCESS){
		 sprintf(_sfp->ID, "|||");
		 return (RET_FAIL);
	 }
	 if(get_sfp_rn(_sfp) != RET_SUCCESS){
		 sprintf(_sfp->ID, "|||");
		 return (RET_FAIL);
	 }
	 rtrim(_sfp->vendor_name);
	 rtrim(_sfp->serial_number);
	 rtrim(_sfp->part_number);
	 rtrim(_sfp->revision_number);
	 sprintf(_sfp->ID, "%s|%s|%s|%s", _sfp->vendor_name, _sfp->serial_number, _sfp->part_number, _sfp->revision_number);
	 return (RET_SUCCESS);
}

/**This function reads the TX and RX optical power values of an SFP.
 * It then updates the proper field inside the SFP data structure
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_optical_power(sfp_device_t* sfp){
	ret_code_t retval;
	alt_u8 power_data[4];
	if(sfp->absent.current){ // if the SFP is absent then all optical power should be set to 0
		memset(sfp->optical_pwr.pwr, 0, 4);
		return 		(RET_SUCCESS);
	}
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_1);
	retval = sfp_read_data_block(sfp, SFP_DDI_TX_PWR_MSB_REG, 4, power_data);
	if(retval == RET_SUCCESS){
///		memcpy(sfp->optical_pwr.pwr, power_data, sizeof(power_data));
		sfp->optical_pwr.output_power_value = uW_to_dBm(((power_data[0] << 8) | power_data[1]) * 0.1);
		sfp->optical_pwr.input_power_value = uW_to_dBm(((power_data[2] << 8) | power_data[3]) * 0.1);
	}
	return 	(retval);
}

/**This function reads the RX and TX optical power alarms and warning levels of the selected SFP
 * 			and updates the corresponding fields in the data structures
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 * @since 0.0.2
 */
ret_code_t get_sfp_optical_power_thresholds(sfp_device_t* sfp){
	ret_code_t retval;
	alt_u8 power_data[16];
	if(sfp->absent.current) // if the SFP is absent then all optical power should be set to 0
	{
		memset(sfp->optical_pwr.pwr_thr, 0, 16);
		return 		(RET_SUCCESS);
	}
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_1);
	retval = sfp_read_data_block(sfp, SFP_DDI_TX_PWR_HA_MSB, 16, power_data);
	if(retval == RET_SUCCESS){
		memcpy(sfp->optical_pwr.pwr_thr, power_data, sizeof(power_data));
	}
	return 	(retval);
}

ret_code_t get_sfp_temperature(sfp_device_t* sfp){
	if(sfp->absent.current)
		return 		RET_ABSENT;
	ret_code_t retval;
	alt_u8 t[2];
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_1);
	retval = sfp_read_data_block(sfp, SFP_DDI_TEMPERATURE_MSB_REG, 2, t);
	if(retval == RET_SUCCESS){
//	_trans->monitor.temperature.value = (alt_8)t[0] + t[1]/256.0;
		sfp->temperature.value = (alt_8)t[0] + t[1]/256.0;
}
return retval;
}

#endif

void get_sfp_connector_value(sfp_device_t*_sfp){
	select_i2c_channel_on_bus(_sfp->i2c_dev.bus, _sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(_sfp, MSA_0);
	alt_u8 temp = 0;
	sfp_read_data(_sfp, 0x02, &temp);
	switch(temp){
	case 0:
		_sfp->connector = SFP_CONNECTOR_UNKNOWN;
		break;
	case 7:
		_sfp->connector = SFP_CONNECTOR_LC;
		break;
	default:
		_sfp->connector = SFP_CONNECTOR_UNKNOWN;
		break;
	}
	return;
}

/**This function enables the SFP in SGMII mode.
 *
 * @param[in,out] _sfp	pointer to the SFP
 * @since 0.0.2
 */
void enable_sfp_sgmii(sfp_device_t* _sfp){
	select_i2c_channel_on_bus(_sfp->i2c_dev.bus, _sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(_sfp, MSA_2);
	Delay_ms(1);
	sfp_write_word(_sfp, SFP_PHY_EXT_PHY_STAT, 0x9084);
	Delay_ms(1);
	sfp_write_word(_sfp, SFP_PHY_CTRL, 0x9140); // reset
	Delay_ms(1);
}

/**This function enables the GE mode in an SFP.
 *
 * @param[in,out] _sfp pointer to the SFP
 * @since 0.0.2
 */
void enable_sfp_ge(sfp_device_t* _sfp){
	select_i2c_channel_on_bus(_sfp->i2c_dev.bus, _sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(_sfp, MSA_2);
	Delay_ms(1);
	sfp_write_word(_sfp,SFP_PHY_SELECT_PAGE,0x00);

	Delay_ms(1);
	sfp_write_word(_sfp,SFP_PHY_1000_BASE_CTRL,0x0F00);

	Delay_ms(1);
	sfp_write_word(_sfp,SFP_PHY_AUTONEG_ADVER,0x0de1); // Advertise 100 base full duplex

	Delay_ms(1);
	sfp_write_word(_sfp,SFP_PHY_CTRL,0x9140); // enable AN

	Delay_ms(1);
}

/**This function reads a single byte from a register from an SFP
 * @param[in] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] offset address of register in the SFP table to read from
 * @param[out] data location where to store the read data
 * @return \c RET_SUCCESS on success
 */
ret_code_t sfp_read_data (sfp_device_t* sfp, alt_u8 offset, alt_u8* data){
	int result = sls_read(sfp->i2c_dev.bus->fd, sfp->i2c_dev.address, data, offset, 1, 1);
	if(result == 0){
		*data = 0;
		return 	(RET_IO_FAIL);
	}
	return 	(RET_SUCCESS);
}

/**This function reads a block of registers from an SFP.
 * It reads a number of successive registers in a single I2C transaction.
 * @param[in] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] start_address the first register address to be read
 * @param[in] count	number of successive registers to be read
 * @param[out] data location where to store the read data
 * @return \c RET_SUCCESS on success
 */
ret_code_t sfp_read_data_block (sfp_device_t* sfp, alt_u8 start_address, int count, alt_u8* data){
	int result = sls_read(sfp->i2c_dev.bus->fd, sfp->i2c_dev.address, data, start_address, count, 2);
	if(result == 0){
		memset(data, 0, count);
		return (RET_IO_FAIL);
	}
	return (RET_SUCCESS);
}

/**This function Writes a single data byte to a register in an SFP.
 * @param[in] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] offset address of register in the SFP table to write to
 * @param[in] data data that has to be written
 * @return \c RET_SUCCESS on success
 */
ret_code_t sfp_write_data(sfp_device_t* sfp, alt_u8 offset, alt_u8 data){
	int result = sls_write(sfp->i2c_dev.bus->fd, sfp->i2c_dev.address, (char*)&data, offset, 1);
	if(result == 0){
		return 		(RET_IO_FAIL);
	}
	return 	(RET_SUCCESS);
}

/**Writes a word (2-bytes) to a register of an SFP.
 * It is implemented by executing two consecutive writes.
 * Normally, this is only needed for table 0xAC.
 * @param[in,out] sfp	pointer to the SFP
 * @param[in] offset offset to write to
 * @param[in] data data to write
 * @return \c RET_SUCCESS if operation is successful, \c RET_FAIL otherwise
 */
ret_code_t sfp_write_word(sfp_device_t* sfp, alt_u8 offset, alt_u16 data){
	if(sfp_write_data(sfp,offset, (alt_u8)(data>>8)) != RET_SUCCESS){
		return RET_FAIL;
	}
	Delay_ms(5);
	if(sfp_write_data(sfp,offset, (alt_u8)data) != RET_SUCCESS){
		return RET_FAIL;
	}
	Delay_ms(10);
	return RET_SUCCESS;
}

/**This function reads the vendor name info of an SFP.
 * It then updates the proper field inside the SFP data structure
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_vn(sfp_device_t* sfp){
	ret_code_t ret = RET_FAIL;
	alt_u8 sfp_vn[VN_LENGTH];
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_0);
	ret = sfp_read_data_block(sfp, SFP_SID_VENDOR_NAME_REG, VN_LENGTH, sfp_vn);
	if(ret != RET_SUCCESS){
		//printf("SFP VN fail: i2c block read failed\n");
		memset(sfp->vendor_name, 0, VN_LENGTH);
	}
	memcpy(sfp->vendor_name, sfp_vn, VN_LENGTH);
	/*terminate the string*/
	sfp->vendor_name[VN_LENGTH-1] = 0;
	return 	(ret);
}

/**This function reads the revision number info of an SFP.
 * It then updates the proper field inside the SFP data structure
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_rn(sfp_device_t* sfp){
	ret_code_t ret = RET_FAIL;
	alt_u8 sfp_rn[RN_LENGTH];
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_0);
	ret = sfp_read_data_block(sfp, SFP_SID_REVISION_NUMBER_REG, RN_LENGTH, sfp_rn);
	if(ret != RET_SUCCESS){
		memset(sfp->revision_number, 0, RN_LENGTH);
	}
	memcpy(sfp->revision_number, sfp_rn, RN_LENGTH);
	sfp->revision_number[RN_LENGTH-1] = 0;
	return 	(ret);
}



/**This function reads the serial number info of an SFP.
 * It then updates the proper field inside the SFP data structure
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_sn(sfp_device_t* sfp){
	ret_code_t ret = RET_FAIL;
	alt_u8 sfp_sn[SN_LENGTH];
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_0);
	ret = sfp_read_data_block(sfp, SFP_SID_SERIAL_NUMBER_REG, SN_LENGTH, sfp_sn);
	if(ret != RET_SUCCESS)	{
		// printf("SFP SN fail: i2c block read failed\n");
		memset(sfp->serial_number, 0, SN_LENGTH);
	}
	memcpy(sfp->serial_number, sfp_sn, SN_LENGTH);
	sfp->serial_number[SN_LENGTH-1] = 0;
	return 	(ret);
}

/**This function reads the part number info of an SFP.
 * It then updates the proper field inside the SFP data structure
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_pn(sfp_device_t* sfp){
	ret_code_t ret = RET_FAIL;
	alt_u8 sfp_pn[PN_LENGTH];
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_0);
	ret = sfp_read_data_block(sfp, SFP_SID_PART_NUMBER_REG, PN_LENGTH, sfp_pn);
	if(ret != RET_SUCCESS){
		//printf("SFP PN fail: i2c block read failed\n");
		memset(sfp->part_number, 0, PN_LENGTH);
	}
	memcpy(sfp->part_number, sfp_pn, PN_LENGTH);
	sfp->part_number[PN_LENGTH - 1] = 0;
	return 	(ret);
}



/**This helper function handles users' requests to read a byte from an SFP+.
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] msa which MSA table to read from
 *  @arg MSA_0
 *  @arg MSA_1
 * @param[in] offset address of register in the SFP table to read from
 * @param[out] data location where to store the read data
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_sfp_read(sfp_device_t *sfp, alt_u8 msa, alt_u8 offset, alt_u8* data){
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, msa);
	return 	(sfp_read_data(sfp, offset, data));
}

/**This helper function handles users' requests to write a byte to an SFP+.
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] msa which MSA table to read from
 *  @arg MSA_0
 *  @arg MSA_1
 *  @arg MSA_2
 * @param[in] offset address of register in the SFP table to write to
 * @param[in] data data to be written
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_sfp_write(sfp_device_t* sfp, alt_u8 msa, alt_u8 offset, alt_u8 data){
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, msa);
	return 	(sfp_write_data(sfp, offset, data));
}

/**This function writes any register to MSA 0xAC table of the SFP
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] offset address of register in the SFP table to write to
 * @param[in] data 16-bit value to write
 * @return	None
 */
void handle_user_phy_sfp_write(sfp_device_t* sfp, alt_u8 offset, alt_u16 data){
	alt_u16 msb = data & 0xFF00 ;
	msb = msb>>8;
	alt_u16 lsb  = data & 0x00FF;

	Delay_ms(5);
	handle_user_sfp_write(sfp,MSA_2,offset, (alt_u8)msb);
	Delay_ms(5);
	handle_user_sfp_write(sfp,MSA_2,offset, (alt_u8)lsb);
	Delay_ms(5);
}

/**This function reads any register from MSA 0xAC table of the SFP
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] offset address of register in the SFP table to read from
 * @return	the 16-bit value read
 */
alt_u16 handle_user_phy_sfp_read( sfp_device_t* sfp, alt_u8 offset)
{
	alt_u8 msb = 0 ;
	alt_u8 lsb = 0;

	Delay_ms(1);
	handle_user_sfp_read(sfp,MSA_2,offset, &msb);
	Delay_ms(1);
	handle_user_sfp_read(sfp,MSA_2,offset, &lsb);
	Delay_ms(1);
	return msb<<8 | lsb ;
}

/**This helper function handles users' requests to read a block of bytes from an SFP.
 * @param[in,out] sfp pointer to the SFP data structure representing the device that we will be accessing
 * @param[in] msa which MSA table to read from
 *  @arg MSA_0
 *  @arg MSA_1
 * @param[in] start_address the first register address to be read
 * @param[in] count	number of successive registers to be read
 * @param[out] data location where to store the read data
 * @return \c RET_SUCCESS on success
 */
ret_code_t handle_user_sfp_read_block(sfp_device_t *sfp, alt_u8 msa, alt_u8 start_address, int count, alt_u8* data){
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, msa);
	return sfp_read_data_block(sfp, start_address, count, data);
}

/**
 * This function verifies communication with the SFP transceiver.
 * @param[in,out] sfp	pointer to the SFP object
 * @return \c RET_SUCCESS if communication is good
 */
ret_code_t sfp_com_check(sfp_device_t* sfp){
#define MSA_BASE_REGS_COUNT 64 /*from 0 to 63*/
	ret_code_t retval = RET_FAIL;
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	alt_u8 msa_base[MSA_BASE_REGS_COUNT];
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_0);
	retval = sfp_read_data_block(sfp, SFP_SID_IDENTIIFER_REG, MSA_BASE_REGS_COUNT, msa_base);
	if(retval == RET_SUCCESS){
		/*calculate the check code of the base registers, which is a one byte
		 * used to verify communication with the SFP.
		 * It is the sum of all bytes from 0 to 62
		 */
		uint8_t cc_base = 0;
		uint8_t i = 0;
		for(i = 0; i < MSA_BASE_REGS_COUNT - 1; i++){
			cc_base += msa_base[i];
		}
		if(cc_base != msa_base[SFP_SID_CC_BASE_REG]){
			retval = RET_IO_FAIL;
		}
	}
	return (retval);
}

/**Initializes the data structures holding the SFP static and dynamic information
 * @param[in,out] sfp pointer to the SFP data structure that we need to initialize
 * @return None
 */
void init_sfp_data(sfp_device_t* sfp){
	sfp->connector = SFP_CONNECTOR_UNKNOWN;
	memset(sfp->vendor_name, 	0, 	sizeof(sfp->vendor_name));
	memset(sfp->part_number, 	0, 	sizeof(sfp->part_number));
	memset(sfp->serial_number, 	0, 	sizeof(sfp->serial_number));
	memset(sfp->revision_number, 	0, 	sizeof(sfp->revision_number));
}

ret_code_t get_sfp_id(transceiver_t* _trans){
	sfp_device_t* _sfp = (sfp_device_t*)(_trans->device.device);
	ret_code_t rc = RET_SUCCESS;
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfp_vn(_sfp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfp_sn(_sfp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfp_pn(_sfp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	rc = get_sfp_rn(_sfp);
	if(rc != RET_SUCCESS){
		return (rc);
	}
	rtrim(_sfp->vendor_name);
	rtrim(_sfp->serial_number);
	rtrim(_sfp->part_number);
	rtrim(_sfp->revision_number);
	sprintf(_trans->info.ID, "%s|%s|%s|%s", _sfp->vendor_name, _sfp->serial_number, _sfp->part_number, _sfp->revision_number);
#ifdef KURS_PROTOCOL
	sprintf(_trans->info.VN, "%s", _sfp->vendor_name);
	sprintf(_trans->info.SN, "%s", _sfp->serial_number);
	sprintf(_trans->info.HN, "%s", _sfp->part_number);
#endif
	return rc;
}

/**
 * This function reads the temperature of the SFP.
 * @param _trans	pointer to the generic transceiver object
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_temperature(transceiver_t* _trans){
	if(_trans->monitor.absent.current){
		return RET_ABSENT;
	}
	sfp_device_t* _sfp = (sfp_device_t*)(_trans->device.device);
	ret_code_t retval;
	alt_u8 t[2];
	select_i2c_channel_on_bus(_sfp->i2c_dev.bus, _sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(_sfp, MSA_1);
	retval = sfp_read_data_block(_sfp, SFP_DDI_TEMPERATURE_MSB_REG, 2, t);
	if(retval == RET_SUCCESS){
		_trans->monitor.temperature.value = (alt_8)t[0] + t[1]/256.0;
	}
	return retval;
}

/**This function reads the RX and TX optical power alarms and warning levels of the selected SFP+
 * 			and updates the corresponding fields in the data structures
 * @param[in,out] _trans pointer to the generic transceiver object
 * @return \c RET_SUCCESS on success
 * @since 0.0.4
 */
ret_code_t get_sfp_optical_power_thresholds(transceiver_t* _trans){
	sfp_device_t* sfp = (sfp_device_t*)(_trans->device.device);
	ret_code_t retval;
	alt_u8 power_data[16];
	if(_trans->monitor.absent.current){ // if the SFP is absent then all optical power should be set to 0
		_trans->monitor.input_power[0].physical_max = -infinityf();
		_trans->monitor.input_power[0].physical_min = -infinityf();
		_trans->monitor.input_power[0].critical_max = -infinityf();
		_trans->monitor.input_power[0].critical_min = -infinityf();
		_trans->monitor.input_power[0].warning_max = -infinityf();
		_trans->monitor.input_power[0].warning_min = -infinityf();
		_trans->monitor.output_power[0].physical_max = -infinityf();
		_trans->monitor.output_power[0].physical_min = -infinityf();
		_trans->monitor.output_power[0].critical_max = -infinityf();
		_trans->monitor.output_power[0].critical_min = -infinityf();
		_trans->monitor.output_power[0].warning_max = -infinityf();
		_trans->monitor.output_power[0].warning_min = -infinityf();
		return 		(RET_SUCCESS);
	}
	if(_trans->monitor.absent.current)return (RET_ABSENT);
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_1);
	retval = sfp_read_data_block(sfp, SFP_DDI_TX_PWR_HA_MSB, 16, power_data);
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

/**This function reads the TX and RX optical power values of an SFP.
 * It then updates the proper field inside the SFP+ data structure
 * @param[in,out] _trans pointer to the generic transceiver object
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_optical_power(transceiver_t* _trans){
	sfp_device_t* sfp = (sfp_device_t*)(_trans->device.device);
	ret_code_t retval;
	alt_u8 power_data[4];
	if(_trans->monitor.absent.current){ // if the SFP+ is absent then all optical power should be set to 0
		_trans->monitor.input_power[0].value = -infinityf();
		_trans->monitor.output_power[0].value = -infinityf();
		return 		(RET_ABSENT);
	}
	_trans->busy = 1;
	select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
	Delay_ms(1);
	SELECT_MSA(sfp, MSA_1);
	retval = sfp_read_data_block(sfp, SFP_DDI_TX_PWR_MSB_REG, 4, power_data);
	if(retval == RET_SUCCESS){
		// convert the values to dBm
		_trans->monitor.output_power[0].value = UW_TO_DBM(((power_data[0] << 8) | power_data[1]) * 0.1);
		_trans->monitor.input_power[0].value = UW_TO_DBM(((power_data[2] << 8) | power_data[3]) * 0.1);
	}
	_trans->busy = 0;
	return 	(retval);
}

/*ret_code_t get_sfp_diagnostics(transceiver_t* _trans){
	ret_code_t retval = RET_INV_CMD;
	if(_trans != NULL){
		if(_trans->monitor.absent.current){
			retval = RET_ABSENT;
		}else {
			if(_trans->info.capabilities.OPTICAL_POWER_MONITOR){
				retval = get_sfp_optical_power(_trans);
			}
			if(_trans->info.capabilities.TEMPERATURE_MONITOR){
				retval = get_sfp_temperature(_trans);
			}
		}
	}
	return (retval);
} */

ret_code_t get_sfp_diagnostics(transceiver_t* _trans){
	ret_code_t retval = RET_INV_CMD;
	if(_trans != NULL){
		if(_trans->monitor.absent.current){
			retval = RET_ABSENT;
		}else {
			if(_trans->info.capabilities.OPTICAL_POWER_MONITOR){
				retval = get_sfp_optical_power(_trans);
			}
			if(_trans->info.capabilities.TEMPERATURE_MONITOR){
				retval = get_sfp_temperature(_trans);
			}
		}
	}
	return (retval);
}

/**This function reads information  of an SFP
 * This include ID, capabilities and tuning parameters (if SFP+ is tunable).
 * @param[in,out] _trans pointer to the transceiver object
 * @return \c RET_SUCCESS on success
 */
ret_code_t get_sfp_info(transceiver_t* _trans){
	ret_code_t retval = RET_SUCCESS;
	if(_trans != NULL){
		sfp_device_t* _sfp = (sfp_device_t*)_trans->device.device;
		if(_trans->monitor.absent.current){
			memset(&_trans->info.capabilities, 0, sizeof(transceiver_capabilities_t));
			retval = RET_IO_FAIL;
		}
		if(retval == RET_SUCCESS){
			/*first read SFP+ ID*/
			retval = get_sfp_id(_trans);
			if(retval == RET_SUCCESS){
				/*does it support digital diagnostics i.e optical power and temperature measurements*/
				retval = get_sfp_capabilities(_trans);
			}
			if(retval == RET_SUCCESS){
				get_sfp_connector_value(_sfp);
				if(_sfp->connector == SFP_CONNECTOR_LC){
					_trans->info.type = TRANS_SFP_OPTICAL;
				}else{
					_trans->info.type = TRANS_SFP_ELECTRICAL;
				}
			}
		}
	}
	return 		(retval);
}

ret_code_t get_sfp_capabilities(transceiver_t* _trans){
	ret_code_t retval = RET_SUCCESS;
	sfp_device_t* sfp = (sfp_device_t*)(_trans->device.device);
	if(_trans->monitor.absent.current){
		retval = RET_ABSENT;
	}
	if(retval == RET_SUCCESS){
		select_i2c_channel_on_bus(sfp->i2c_dev.bus, sfp->i2c_dev.channel);
		alt_u8 val;
		Delay_ms(1);
		SELECT_MSA(sfp, MSA_0);
		retval = sfp_read_data(sfp, SFP_SID_DIAG_MONITORING_TYPE_REG, &val);
		if(retval == RET_SUCCESS){
			_trans->info.capabilities.TEMPERATURE_MONITOR = BIT_IS_SET(val, BIT6); /*if b6 is set, that means digital diagnostics monitoring is implemented @see SFF-8472 http://www.10gtek.com/templates/wzten/pdf/SFF-8472-(Diagnostic%20Monitoring%20Interface).pdf*/
			_trans->info.capabilities.OPTICAL_POWER_MONITOR = _trans->info.capabilities.TEMPERATURE_MONITOR;
			/*does it support tuning?*/
			Delay_ms(1);
			retval = sfp_read_data(sfp, SFP_SID_OPTIONS_VALUE_REG, &val);
			if(retval == RET_SUCCESS){
				_trans->info.capabilities.tx_tunable = BIT_IS_SET(val, BIT6); /*if b6 is set, that means transmitter is tunable*/
				if(_trans->info.capabilities.tx_tunable){
					/*any SFP supports tuning info?*/
					/*retval = get_sfp_tuning_info(_trans);*/
				}
				_trans->info.capabilities.rx_tunable = 0;
			}
		}
		/*SFPs have a single internal optical IO*/
		_trans->info.capabilities.optical_ios_number = 1;
	}
	return (retval);
}
#endif	/*ENABLE_SFP_CODE*/
