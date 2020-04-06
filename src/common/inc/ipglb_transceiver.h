/**
 * ipglb_transceiver.h
 *
 *  Created on: May 31, 2018
 *      Author: dcham
 *	@version 0.0.1
 */

#ifndef IPGLB_TRANSCEIVER_H_
#define IPGLB_TRANSCEIVER_H_

#include "ipg_types.h"

#include "sys/alt_alarm.h"

#define DEF_TEMP_HMIN         	 -5    //!<default HMin threshold for temperature parameter
#define DEF_TEMP_CMIN         	  0    //!<default CMin threshold for temperature parameter
#define DEF_TEMP_WMIN         	  10   //!<default WMin threshold for temperature parameter
#define DEF_TEMP_WMAX         	  60   //!<default WMax threshold for temperature parameter
#define DEF_TEMP_CMAX         	  70   //!<default CMax threshold for temperature parameter
#define DEF_TEMP_HMAX         	  75   //!<default HMax threshold for temperature parameter

//default thresholds for SFP+ parameters
#define DEF_SFPP_PIN_HMAX		3     	//!<default HMax threshold for SFP+ Pin parameter
#define DEF_SFPP_PIN_CMAX		-0.5    //!<default CMax threshold for SFP+ Pin parameter
#define DEF_SFPP_PIN_WMAX		-2      //!<default WMax threshold for SFP+ Pin parameter
#define DEF_SFPP_PIN_WMIN		-14     //!<default WMin threshold for SFP+ Pin parameter
#define DEF_SFPP_PIN_CMIN		-17     //!<default CMin threshold for SFP+ Pin parameter
#define DEF_SFPP_PIN_HMIN		-40     //!<default HMin threshold for SFP+ Pin parameter

#define DEF_SFPP_POUT_HMAX		5       //!<default HMax threshold for SFP+ Pout parameter
#define DEF_SFPP_POUT_CMAX		1       //!<default CMax threshold for SFP+ Pout parameter
#define DEF_SFPP_POUT_WMAX		-1      //!<default WMax threshold for SFP+ Pout parameter
#define DEF_SFPP_POUT_WMIN		-6      //!<default WMin threshold for SFP+ Pout parameter
#define DEF_SFPP_POUT_CMIN		-8      //!<default CMin threshold for SFP+ Pout parameter
#define DEF_SFPP_POUT_HMIN		-40     //!<default HMin threshold for SFP+ Pout parameter

//default thresholds for SFP parameters
#define DEF_SFP_PIN_HMAX		3     	//!<default HMax threshold for SFP Pin parameter
#define DEF_SFP_PIN_CMAX		-0.5    //!<default CMax threshold for SFP Pin parameter
#define DEF_SFP_PIN_WMAX		-2      //!<default WMax threshold for SFP Pin parameter
#define DEF_SFP_PIN_WMIN		-14     //!<default WMin threshold for SFP Pin parameter
#define DEF_SFP_PIN_CMIN		-17     //!<default CMin threshold for SFP Pin parameter
#define DEF_SFP_PIN_HMIN		-40     //!<default HMin threshold for SFP Pin parameter

#define DEF_SFP_POUT_HMAX		5       //!<default HMax threshold for SFP Pout parameter
#define DEF_SFP_POUT_CMAX		1       //!<default CMax threshold for SFP Pout parameter
#define DEF_SFP_POUT_WMAX		-1      //!<default WMax threshold for SFP Pout parameter
#define DEF_SFP_POUT_WMIN		-6      //!<default WMin threshold for SFP Pout parameter
#define DEF_SFP_POUT_CMIN		-8      //!<default CMin threshold for SFP Pout parameter
#define DEF_SFP_POUT_HMIN		-40     //!<default HMin threshold for SFP Pout parameter

//default thresholds for CFP parameters
#define DEF_CFP_PIN_HMIN         -40  //!<default HMin threshold for QSFP Pin parameter
#define DEF_CFP_PIN_CMIN         -30  //!<default CMin threshold for CFP Pin parameter
#define DEF_CFP_PIN_WMIN         -16  //!<default WMin threshold for CFP Pin parameter
#define DEF_CFP_PIN_WMAX         -2   //!<default WMax threshold for CFP Pin parameter
#define DEF_CFP_PIN_CMAX         -1   //!<default CMax threshold for CFP Pin parameter
#define DEF_CFP_PIN_HMAX          0   //!<default HMin threshold for QSFP Pin parameter

#define DEF_CFP_POUT_HMIN        -40   //!<default CMax threshold for CFP Pout parameter
#define DEF_CFP_POUT_CMIN        -1   //!<default CMin threshold for CFP Pout parameter
#define DEF_CFP_POUT_WMIN         0   //!<default WMin threshold for CFP Pout parameter
#define DEF_CFP_POUT_WMAX         5   //!<default WMax threshold for CFP Pout parameter
#define DEF_CFP_POUT_CMAX         6   //!<default CMax threshold for CFP Pout parameter
#define DEF_CFP_POUT_HMAX         7   //!<default CMax threshold for CFP Pout parameter

//default thresholds for QSFP parameters
#define DEF_QSFP_PIN_HMIN        -40  //!<default HMin threshold for QSFP Pin parameter
#define DEF_QSFP_PIN_CMIN        -20  //!<default CMin threshold for QSFP Pin parameter
#define DEF_QSFP_PIN_WMIN        -15  //!<default WMin threshold for QSFP Pin parameter
#define DEF_QSFP_PIN_WMAX         0   //!<default WMax threshold for QSFP Pin parameter
#define DEF_QSFP_PIN_CMAX         1   //!<default CMax threshold for QSFP Pin parameter
#define DEF_QSFP_PIN_HMAX         2   //!<default HMax threshold for QSFP Pin parameter

#define DEF_QSFP_POUT_HMIN       -40   //!<default HMin threshold for QSFP Pout parameter
#define DEF_QSFP_POUT_CMIN       -3   //!<default CMin threshold for QSFP Pout parameter
#define DEF_QSFP_POUT_WMIN       -1   //!<default WMin threshold for QSFP Pout parameter
#define DEF_QSFP_POUT_WMAX        0   //!<default WMax threshold for QSFP Pout parameter
#define DEF_QSFP_POUT_CMAX        1   //!<default CMax threshold for QSFP Pout parameter
#define DEF_QSFP_POUT_HMAX        2   //!<default HMax threshold for QSFP Pout parameter

/**
 * Definition of the possible states of a transceiver.
 */
typedef enum{
	TRANSCEIVER_STATE_ABSENT = 0x00,   /*!< TRANSCEIVER_STATE_ABSENT transceiver not plugged*/
	TRANSCEIVER_STATE_OK = 0x01,       /*!< TRANSCEIVER_STATE_OK transceiver plugged and operational*/
	TRANSCEIVER_STATE_NOT_READY = 0x02,/*!< TRANSCEIVER_STATE_NOT_READY transceiver plugged, communication with it OK but not operational*/
	TRANSCEIVER_STATE_FAIL = 0x03,     /*!< TRANSCEIVER_STATE_FAIL transceiver plugged but not operational, also cannot communicate with it*/
	TRANSCEIVER_STATE_REBOOTING = 0x04,/*!< TRANSCEIVER_STATE_REBOOTING transceiver is in the middle of the booting process, transitional state*/
	TRANSCEIVER_STATE_BOOTED = 0x05,	/*!< TRANSCEIVER_STATE_BOOTED booting process is finished, transceiver can be read and configured now*/
	/*keep this line at the bottom*/
	TRANSCEIVER_STATE_UNKNOWN = 0xFF   /*!< TRANSCEIVER_STATE_UNKNOWN*/
}transceiver_state_t;

/** List of supported transceiver types.
 *
 */
typedef enum{
	TRANS_SFP_ELECTRICAL = 1,
	TRANS_SFP_OPTICAL = 2,
	TRANS_SFPP_ELECTRICAL = 3,
	TRANS_SFPP_OPTICAL = 4,
	TRANS_5x7 = 5,
	TRANS_XFP  = 6,
	TRANS_CFP = 7,
	TRANS_QSFP = 8,
	// always keep at the bottom
	TRANS_UNKNOWN = 255
}transceiver_type_t;

/**
 * Definition of a tunable object.
 */
typedef struct{
	double				first_freq;		/*!minimum frequency in THz*/
	double				last_freq;		/*!maximum frequency in THz*/
	double				grid_spacing; 	/*!grid spacing in THz*/
}tuning_info_t;

typedef struct{
	volatile unsigned int tx_tunable:1;
	volatile unsigned int rx_tunable:1;
	volatile unsigned int TEMPERATURE_MONITOR:1;
	volatile unsigned int OPTICAL_POWER_MONITOR:1;
	volatile unsigned int tx_power_control:1;
	volatile unsigned int user_eeprom:1;
	volatile unsigned int tune_by_channel:1;
	volatile unsigned int tune_by_wl:1;
	volatile unsigned int reserved_1:20;
	volatile unsigned int optical_ios_number:4;
}transceiver_capabilities_t;

typedef struct{
	ipg_status_t 				absent;
	ipg_status_t 				fault;
	ipg_status_t 				los;
	ipg_status_t				state;//!<@ref transceiver_state_t
#if ENABLE_XFP_CODE
	ipg_status_t 				not_ready;
#endif
	diagnostics_t*				input_power;
	diagnostics_t*				output_power;
	diagnostics_t				temperature;
	double						tx_freq;
	double						rx_freq;
}transceiver_monitor_t;

typedef struct{
	transceiver_type_t 			type;
	char						ID[TRANSCEIVER_ID_LEN];
	transceiver_capabilities_t	capabilities;
	tuning_info_t				tuning;
#ifdef KURS_PROTOCOL
	char						VN[VN_LENGTH];
	char 						SN[SN_LENGTH];
	char						HN[HN_LENGTH];
	char						FN[FN_LENGTH];
#endif
}transceiver_info_t;

typedef struct{
	ipg_control32_t				tx_disable;
	ipg_float_control_t			tx_power;
	ipg_control32_t				reboot;
	ipg_control32_t				threshold_mode_reset;
	threshold_mode_t			threshold_mode;
	ipg_double_control_t		tx_freq;
	ipg_double_control_t		rx_freq;
}transceiver_control_t;

typedef struct{
	void* device;
	ipglb_io_t absent;
	ipglb_io_t los;
	ipglb_io_t tx_disable;
	ipglb_io_t fault;
#if ENABLE_XFP_CODE
	ipglb_io_t not_ready;
#endif
}transceiver_physical_device_t;

typedef struct{
	int 						index;			//!< 0 based index
	transceiver_info_t			info;
	transceiver_monitor_t		monitor;
	transceiver_control_t		control;
	transceiver_physical_device_t	device;
	alt_alarm					timer;
	char						busy;
	void*						additional;
}transceiver_t;

typedef void (*state_cb)(transceiver_t*);

typedef struct{
	char input_com_ok;
	char input_com_lost;
	char input_boot_done;
	char input_boot_start;
	ipg_status_t current_state;
	state_cb state_handler;
}transceiver_state_machine_t;

void create_transceiver(transceiver_t* _trans);
void custom_init_transceiver_data(transceiver_t* _trans);
ret_code_t boot_transceiver(transceiver_t* _trans);
ret_code_t read_transceiver_dynamic_data(transceiver_t* _trans);
void reset_transceiver_id(transceiver_t* _trans);
void reset_transceiver_temperature_thresholds(transceiver_t* _trans);
void reset_transceiver_optical_thresholds(transceiver_t* _trans);
ret_code_t boot_transceiver(transceiver_t* _trans);
void callback_on_transceiver_state_change(transceiver_t* _trans);
void callback_on_transceiver_los_change(transceiver_t* _trans);
void callback_on_transceiver_txdisable_set(transceiver_t* _trans);
void callback_on_transceiver_lowpwer_set(transceiver_t* _trans);
void callback_on_transceiver_reset_set(transceiver_t* _trans);
void callback_on_transceiver_lowpower_set(transceiver_t* _trans);
void callback_on_transceiver_plug_change(transceiver_t* _trans);
void update_transceiver_state(transceiver_t* _trans);
ret_code_t set_if_transceiver_freq(transceiver_t* _trans);
void set_transceiver_txdisable(transceiver_t* _trans);
alt_u32 get_if_transceiver_txdisable(transceiver_t* _trans);
void set_transceiver_lowpower(transceiver_t* _trans);
alt_u32 get_if_transceiver_lowpower(transceiver_t* _trans);
ret_code_t set_if_transceiver_tx_freq(transceiver_t* _trans);
ret_code_t set_if_transceiver_rx_freq(transceiver_t* _trans);
ret_code_t set_if_transceiver_txpower(transceiver_t* _trans);
extern void run_transceiver_booting_state(transceiver_t* _trans);
ret_code_t is_in_monitor_state(transceiver_t* _trans);
ret_code_t is_in_query_state(transceiver_t* _trans);
ret_code_t is_in_comcheck_state(transceiver_t* _trans);
ret_code_t is_in_absent_state(transceiver_t* _trans);
ret_code_t is_in_booting_state(transceiver_t* _trans);
ret_code_t is_in_fail_state(transceiver_t* _trans);
#endif /* IPGLB_TRANSCEIVER_H_ */
