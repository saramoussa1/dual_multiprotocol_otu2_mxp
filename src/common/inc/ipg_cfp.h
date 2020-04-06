/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_cfp.h
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @date Oct 10, 2016
 * @brief Contains prototypes of functions implemented in ipg_cfp.c as well as type definitions.
 *
 */

#ifndef IPG_CFP_H_
#define IPG_CFP_H_

#include "../../configs.h"
#include "ipg_mdio.h"
#include "ipg_bits.h"
#include "ipg_types.h"
#include <sys/alt_alarm.h>
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "ipg_delay.h"
#include "ipg_types.h"
#include "ipglb_transceiver.h"
#include "io.h"
#include "alt_types.h"
#include "math.h"
#include "ipg_utils.h"
#include "ipg_constants.h"

#if ENABLE_CFP_CODE
#if ENABLE_MENARA_CFP_CODE
/*List of supported distances*/
#define DIST_0_KM  0x00
#define DIST_600_KM 0x01
#define DIST_2700_KM 0x02
#define DIST_4000_KM 0x03
#define DIST_1200_KM 0x04

enum MENARA_CLI_COMMANDS{
	CLI_COMMAND_NO_OP = 0,
	CLI_COMMAND_RD = 1,
	CLI_COMMAND_WR = 2,
	CLI_COMMAND_RD_ERR_COUNT = 25,
	CLI_COMMAND_RD_LN_OTN_STATUS = 43,
	CLI_COMMAND_RD_CL_OTN_STATUS = 44,
	CLI_COMMAND_RD_CL_100GbE_STATUS = 45,
	CLI_COMMAND_RD_CL_100GbE_INFO = 46,
	CLI_COMMAND_RD_CL_100GbE_RFC1757_INFO = 47,
};

/**Enumeration of some registers to read from the CFP.
 * These registers can be accessed through MDIO.
 */
#define	 CFP_100GbE_MODE	0		//!<Value that indicates that the CFP is in 100GbE mode
#define CFP_OTU4_MODE		1	    //!<Value that indicates that the CFP is in OTU4 mode

#endif

/**Enumeration of some registers to read from the CFP.
 * These registers can be accessed through MDIO.
 */
#define	CFP_MSA_ID_REG				 		0x8000	//!<MDIO register offset containing the device ID.
#define	CFP_MSA_EXT_IDENTIFIER_REG			0x8001	//!<MDIO register offset containing the device ID.
#define	CFP_MSA_NUM_LANES_REG				0x8009	//!<MDIO register offset
#define	CFP_MSA_DEVICE_TECH_2_REG			0x8019	//!<MDIO register offset
#define	CFP_MSA_VN_REG						0x8021	//!<MDIO register offset where to read the first character of the device vendor name string.
#define	MDIO_CFP_PN_REG				 		0x8034 //!<MDIO register offset where to read the first character of the device part number string.
#define	MDIO_CFP_SN_REG 				 	0x8044 //!<MDIO register offset where to read the first character of the device serial number string.
#define	MDIO_CFP_FW_REG				 		0x806C //!<MDIO register offset where to read the FW revision number.
#define	MDIO_CFP_HW_REG				 		0x806A //!<MDIO register offset where to read the HW revision number.
#define	CFP_MSA_DDM_TYPE_REG				0x806E //!<MDIO register offset
#define	CFP_MSA_DDM_CAPABILITY_1_REG		0x806F //!<MDIO register offset
#define	CFP_MSA_DDM_CAPABILITY_2_REG		0x8070 //!<MDIO register offset
#define	MDIO_CFP_TX_PWR_HA_MSB	 			0x80B0	//!<MDIO register offset where to read the output optical power High Alarm.
#define	MDIO_CFP_RX_PWR_HA_MSB	 			0x80C0	//!<MDIO register offset where to read the input optical power High Alarm.
#define	CFP_MSA_MIN_LASER_FREQ_1_MSB_REG		0x818A
#define	CFP_MSA_MIN_LASER_FREQ_1_LSB_REG		0x818B
#define	CFP_MSA_MIN_LASER_FREQ_2_MSB_REG		0x818C
#define	CFP_MSA_MIN_LASER_FREQ_2_LSB_REG		0x818D
#define	CFP_MSA_MAX_LASER_FREQ_1_MSB_REG		0x818E
#define	CFP_MSA_MAX_LASER_FREQ_1_LSB_REG		0x818F
#define	CFP_MSA_MAX_LASER_FREQ_2_MSB_REG		0x8190
#define	CFP_MSA_MAX_LASER_FREQ_2_LSB_REG		0x8191
#define	CFP_MSA_LASER_TUNING_CAPABILITY_MSB_REG		0x8196
#define	CFP_MSA_LASER_TUNING_CAPABILITY_LSB_REG		0x8197
#define	MDIO_CFP_MOD_GEN_CNTL_REG			0xA010	//!<MDIO register offset where to read/write the module general control
#define	MDIO_NETWORK_LANE_TX_CTRL_REG		0xA011 //!<MDIO register offset where to set the TX rate select to OTU4
#define	MDIO_NETWORK_LANE_RX_CTRL_REG		0xA012 //!<MDIO register offset where to set the RX rate select to OTU4
#define	MDIO_CFP_RX_STS_REG					0xA01D	//!<MDIO register offset where to read status signals (LOF, LOM, ...)
#define	MDIO_CFP_TEMP_REG			 		0xA02F	//!<MDIO register offset where to read the module's temperature.
#define	MDIO_CFP_OPTICAL_TX_PWR_REG	 		0xA2B0	//!<MDIO register offset where to read the output optical power.
#define	MDIO_CFP_LASER_TEMP_REG		 		0xA2C0	//!<MDIO register offset where to read the laser's temperature.
#define	MDIO_CFP_OPTICAL_RX_PWR_REG			0xA2D0	//!<MDIO register offset where to read the input optical power.
#define	CFP_MSA_TX_FREQ_1_REG		 		0xB450	//!<MDIO register offset
#define	CFP_MSA_TX_FREQ_2_REG		 		0xB460	//!<MDIO register offset
#define	CFP_MSA_RX_FREQ_1_REG		 		0xB470	//!<MDIO register offset
#define	CFP_MSA_RX_FREQ_2_REG		 		0xB480	//!<MDIO register offset
#define	MDIO_CFP_FEC_CORR_BITS_48_32_REG	 0xB5C0	//!<MDIO register offset where to read bits 47 to 32 of the FEC corrected bits counter.
#define	MDIO_CFP_FEC_CORR_BITS_31_16_REG	 0xB5C1	//!<MDIO register offset where to read bits 31 to 16 of the FEC corrected bits counter.
#define	MDIO_CFP_FEC_CORR_BITS_15_0_REG		 0xB5C2	//!<MDIO register offset where to read bits 15 to 0 of the FEC corrected bits counter.
#define	MDIO_CFP_FEC_UNCORR_CW_31_16_REG	 0xB5C3	//!<MDIO register offset where to read bits 31 to 16 of the FEC uncorrected codewords counter.
#define	MDIO_CFP_FEC_UNCORR_CW_15_0_REG	 0xB5C4	//!<MDIO register offset where to read bits 31 to 16 of the FEC uncorrected codewords counter.
#define	MDIO_CFP_BIP8_ERR_CNT_31_16_REG	 0xB5C5	//!<MDIO register offset where to read bits 31 to 16 of the FEC uncorrected codewords counter.
#define	MDIO_CFP_BIP8_ERR_CNT_15_0_REG	 0xB5C6	//!<MDIO register offset where to read bits 31 to 16 of the FEC uncorrected codewords counter.
#define	MDIO_CFP_NTWK_CUR_BER_31_16_REG	 0x9050	//!<MDIO register offset where to read the upper word of floating point number
#define	MDIO_CFP_NTWK_CUR_BER_15_0_REG	 0x9051	//!<MDIO register offset where to read the lower word of floating point number
#define	MDIO_CFP_NTWK_AVG_BER_31_16_REG	 0x9052	//!<MDIO register offset where to read the upper word of floating point number
#define	MDIO_CFP_NTWK_AVG_BER_15_0_REG	 0x9053	//!<MDIO register offset where to read the lower word of floating point number
#define	CFP_MSA_TX_CHANNEL_CTRL_REG	 	0xB400	//!<MDIO register offset for TX ITU channel control.
#define	CFP_MSA_RX_CHANNEL_CTRL_REG	 	0xB420	//!<MDIO register offset for RX ITU channel control.
#define	MDIO_CFP_MOD_STATE_REG  		0xB016
#define CFP_MSA_TX_OUTPUT_PWR_REG 		0xB410	//!<MDIO register offset where to read/write the desired TX output power
#define MDIO_CFP_NTWK_RX_STS_REG  		0xB580  //!<MDIO register offset where to read/write the RX OTN status
#if ENABLE_MENARA_CFP_CODE
#define    MENARA_CLI_EXECUTE_REG 0x9000 		//!<Execute Flags. Bit 0 – Execute flag (1 – Execute command set by Host to tell the CFP there is a command to execute.
    									 	 	//The bit is cleared when CFP completes the operation)
#define	MENARA_CLI_COMMAND_REG  0x9001 		//!<Command to be executed
#define	MENARA_CLI_DEV_IND_REG  0x9002 		//!<Device index -- Specifies the index of a particular device type
#define	MENARA_CLI_ADDRESS_REG_HIGH  0x9003 	//!<Address -- high 2 bytes
#define	MENARA_CLI_ADDRESS_REG_LOW  0x9004 	//!<Address -- low 2 bytes
#define	MENARA_CLI_DATA_LENGTH_REG  0x9005 	//!<Length in bytes to read/write
#define	MENARA_CLI_COMMAND_STATUS_REG  0x9006 //!<Command status.  0: success; 1: failed;   2: in progress
#define	MENARA_CLI_COMMAND_DATA_REG  0x9007   //!<Register where to read data from or write data to
#define	MENARA_CLI_PASS_1_REG  0x9089 		//!<First part of password
#define	MENARA_CLI_PASS_2_REG  0x908A 		//!<Second part of password
#define	MENARA_SPECIAL_REG_9231 0x9231 		//!<
#define	MENARA_SPECIAL_REG_9232 0x9232 		//!<
#define	MENARA_SPECIAL_REG_9233 0x9233 		//!<
#define	MENARA_SPECIAL_REG_9238 0x9238 		//!<
#define	MENARA_SPECIAL_REG_9230 0x9230 		//!<
#endif

/**Representation of the SD FEC module
 *
 */
typedef struct cfp_sd_fec{
	ipg_control_t tx_fec_en;		//!<controls enabling or disabling the TX encoder FEC
	ipg_control_t rx_fec_en;		//!<controls enabling or disabling the RX decoder FEC
	ipg_counter_t corrected_bits;	//!<counter of the detected number of corrected bits
	ipg_counter_t uncorrected_cw;	//!<counter of the detected number of uncorrected codewords
}cfp_sd_fec_t;

/**
 * List of MOD states
 */
typedef enum {
	CFP_MOD_INTERNAL_STATE_INIT  		= 0x00,
	CFP_MOD_INTERNAL_STATE_LOW_PWR  	= 0x02,
	CFP_MOD_INTERNAL_STATE_HI_PWR_UP  	= 0x04,
	CFP_MOD_INTERNAL_STATE_TX_OFF  	= 0x08,
	CFP_MOD_INTERNAL_STATE_TX_TURN_ON  	= 0x10,
	CFP_MOD_INTERNAL_STATE_READY 		= 0x20,
	CFP_MOD_INTERNAL_STATE_FAULT 		= 0x40,
	CFP_MOD_INTERNAL_STATE_TX_TURN_OFF 	= 0x80,
	CFP_MOD_INTERNAL_STATE_HI_PWR_DWN  	= 0x100
}CFP_MOD_INTERNAL_STATE_t;


#define	_1_OVER_16 0
#define _1_OVER_64 1

// rate select values
#define	RATE_SELECT_GbE  	0//10.31
#define	RATE_SELECT_SDH  	1//9.95
#define	RATE_SELECT_OTU3  	2//10.7
#define	RATE_SELECT_OTU4  	3//11.2
#define	RATE_SELECT_OTU3e1	4//11.14
#define	RATE_SELECT_OTU3e2	5//11.15
#define	RATE_SELECT_RES_1	6
#define	RATE_SELECT_RES_2	7

typedef enum {
	GRID_SPACING_100_GHZ = 0,
	GRID_SPACING_50_GHZ = 1,
	GRID_SPACING_33_GHZ = 2,
	GRID_SPACING_25_GHZ = 3,
	GRID_SPACING_12_5_GHZ = 4,
	GRID_SPACING_6_25_GHZ = 5
}cfp_grid_spacing_t;

/**Representation of a CFP device
 *
 */
typedef struct{
	alt_u8 index;				//!<Index of the CFP on our platform
	transceiver_t* 	base;//!<pointer to the parent transceiver
	mdio_device_t mdio;			//!<MDIO interface to which this device is connected.
	CFP_TYPE_T type;			//!<detected type of the device
	char vn[17];				//!<Vendor name string
	char pn[17];				//!<Part number string
	char sn[17];				//!<Serial number string
	char fw_number[3];		//!<Firmware revision string
	char hw_number[3];		//!<Hardware revision string
	fec_statistics_t fec;				//!<the SD FEC module of the device
	ipg_bool_t enable_rxmclk;	//!<Used to enable RX MCLK or not
	alt_u16 state;		//!<MSA module state
	alt_u8	rate_sel;
	alt_u8 ref_clk_sel;
#if ENABLE_MENARA_CFP_CODE
	alt_u8 cfp_mode;			//!<controls setting the IPG MENARA CFP type to 100GbE or OTU4
#endif
	ipglb_io_t reset;
	ipglb_io_t low_power;
	ipglb_io_t prog_alarm[3];
	ipglb_io_t prog_control[3];
	int rebt_cntr;
	char busy; /*set to 1 when a request to change TX frequency is received*/
	alt_alarm local_timer;
	CFP_MOD_INTERNAL_STATE_t module_state;
	char cli_disabled;
}cfp_device_t;

/*
 * exported functions
 */
void init_cfp_data(cfp_device_t* cfp);
alt_u32 cfp_rd_register(cfp_device_t* cfp, alt_u16 RegisterAddress);
void cfp_wr_register(cfp_device_t* cfp, alt_u16 RegisterAddress,  alt_u16 Data);
void cfp_get_status(cfp_device_t *_om);
void cfp_get_vn(cfp_device_t* cfp);
void cfp_get_sn(cfp_device_t* cfp);
void cfp_get_pn(cfp_device_t* cfp);
void cfp_get_fw_hw_number(cfp_device_t* cfp);
void cfp_softreset(cfp_device_t* _cfp);

ret_code_t boot_cfp(cfp_device_t* _cfp);

ret_code_t cfp_get_optical_power(cfp_device_t* cfp);
ret_code_t cfp_get_temperature(cfp_device_t* cfp);
ret_code_t cfp_get_optical_power_thresholds(cfp_device_t* cfp);
ret_code_t get_cfp_diagnostics(cfp_device_t* _cfp);
void cfp_set_desired_tx_power(cfp_device_t* _cfp, float tx_power);
ret_code_t cfp_set_tx_freq(cfp_device_t* _cfp, double freq);
ret_code_t cfp_set_rx_freq(cfp_device_t* _cfp, double freq);
ret_code_t cfp_set_optical_grid_spacing(cfp_device_t* _cfp, cfp_grid_spacing_t gs);

#if ENABLE_MENARA_CFP_CODE
void menara_cfp_cli_cmd_rd(cfp_device_t* _cfp, alt_u16 command, alt_u16 length_in_bytes, alt_u16* data);
ret_code_t menara_cfp_cli_command(cfp_device_t* _cfp, alt_u16 command, alt_u16 length_in_bytes);
ret_code_t menara_cfp_cli_wr(cfp_device_t* _cfp, alt_u16 device_index, alt_u16 high_address, alt_u16 low_address, alt_u16 data);
ret_code_t menara_cfp_cli_rd(cfp_device_t* _cfp, alt_u16 device_index, alt_u16 high_address, alt_u16 low_address, alt_u8 nbre_of_bytes, alt_u16* data);
void menara_config_client(cfp_device_t* _cfp);
void menara_set_password(cfp_device_t* _cfp);
void menara_set_fec_type(cfp_device_t* _cfp, fec_type_t fec_type);
void update_menara_cfp_otu4_fec_stats(cfp_device_t* _cfp);
alt_u32 update_cfp_alarms(cfp_device_t* _cfp);
alt_u32 update_cfp_bip8_counter(cfp_device_t* _cfp);
fec_type_t menara_get_fec_configuration(cfp_device_t* _cfp);
#endif
ret_code_t cfp_get_current_frequencies(cfp_device_t* cfp);
ret_code_t get_cfp_info(cfp_device_t* cfp);
ret_code_t cfp_get_optical_power_thresholds(cfp_device_t* cfp);
void run_cfp_monitor_state(cfp_device_t* _cfp);
ret_code_t cfp_com_check(cfp_device_t* _cfp);
ret_code_t cfp_get_fec_corrected_bits(cfp_device_t* _cfp);
#endif /*ENABLE_CFP_CODE*/
#endif /* IPG_CFP_H_ */
