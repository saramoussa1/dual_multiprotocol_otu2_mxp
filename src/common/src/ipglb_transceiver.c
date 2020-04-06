/*
 * ipglb_transceiver.c
 *
 *  Created on: May 4, 2018
 *      Author: dcham
 */

#include <alt_types.h>

#include "..\inc\ipglb_transceiver.h"
#include "..\inc\ipg_bits.h"
#include <math.h>
//#include "../inc/ipg_constants.h"
//#include "..\inc\ipg_types.h"

#if ENABLE_SFPP_CODE
#include "../inc/ipg_sfpp.h"
#endif
#if ENABLE_CFP_CODE
#include "../inc/ipg_cfp.h"
#endif
#if ENABLE_SFP_CODE
#include "../inc/ipg_sfp.h"
#endif
#if ENABLE_XFP_CODE
#include "../inc/ipg_xfp.h"
#endif
#if ENABLE_QSFP_CODE
#include "../inc/ipg_qsfp.h"
#endif
#include "string.h"
#include "stdlib.h"
#include <HAL/inc/io.h>
#include <stdio.h>
#include "../inc/ipg_utils.h"
#include "../inc/ipg_bits.h"

#ifndef __weak
#define __weak __attribute__((weak))
#endif

#define SET_TRANS_IO(alarm_io, v)	((v)?(SET_IO(_trans->device.alarm_io)):(CLEAR_IO(_trans->device.alarm_io)))
#define GET_TRANS_IO(alarm_io)		GET_IO(_trans->device.alarm_io)
#define PREVIOUS(alarm)			_trans->monitor.alarm.previous
#define CURRENT(alarm)			_trans->monitor.alarm.current
#define CHANGE(alarm)			_trans->monitor.alarm.change
#define UPDATE_IO(alarm)		PREVIOUS(alarm) = CURRENT(alarm); \
								CURRENT(alarm) = GET_TRANS_IO(alarm);\
								CHANGE(alarm) = CURRENT(alarm) ^ PREVIOUS(alarm);
#define UPDATE_STATE(v)			PREVIOUS(state) = CURRENT(state); \
								CURRENT(state) = v;\
								CHANGE(state) = CURRENT(state) ^ PREVIOUS(state);

#define RD_IF_TRANS_DYNAMIC_RQST_LEN			5	//!<Length of interface transceiver dynamic data read request
#define RD_IF_TRANS_DYNAMIC_RPLY_LEN			12	//!<min length of interface transceiver dynamic data read reply (4 header + 1 index + 1 status + 2 alarms + 4 temperature + variable number for optical power)

#define RD_IF_TRANS_INFO_RQST_LEN			5	//!<Length of transceiver ID read request (4 header + 1 index)
#define RD_IF_TRANS_INFO_RPLY_LEN			71	//!<Length of transceiver ID read reply (4 header + 1 index + 1 number of optical IOs + 4 capabilities + 61 ID)
#define RD_IF_TRANS_OPT_PWR_THR_RQST_LEN		5   	//!<Length of transceiver optical threshold read request 	(4 header + 1 index)
#define RD_IF_TRANS_OPT_PWR_THR_RPLY_LEN		37  	//!<min length of transceiver optical threshold read reply 		(4 header + 1 index + variable number for optical power thresholds)

#define WR_IF_TRANS_TXDIS_RQST_LEN		6   	//!<Length of transceiver TX_DIS request 	(4 header + 1 index + 1 TXDIS value)
#define WR_IF_TRANS_TXDIS_RPLY_LEN		6  		//!<Length of transceiver TX_DIS reply 		(4 header + 1 index + 1 TXDIS value)

#define WR_IF_TRANS_TXPOWER_RQST_LEN		9   	//!<Length of transceiver TX Power SET request 	(4 header + 1 index + 4 TX POWER value)
#define WR_IF_TRANS_TXPOWER_RPLY_LEN		9  		//!<Length of transceiver TX Power SET reply 		(4 header + 1 index + 4 TX POWER value)
#define IF_TRANS_FREQ_RQST_LEN				14   	//!<Length of transceiver Frequency request 	(4 header + 1 index|TX/RX + 1 STATIC/DYNAMIC + 4 TX frequency value + 4 RX frequency value)
#define IF_TRANS_FREQ_RPLY_LEN				18  	//!<Length of transceiver Frequency reply 		(4 header + 1 index +  1 STATIC/DYNAMIC + [4 TX frequency/4 first frequency value] + [4 RX frequency value/4 last frequency value + [DUMMY/4 grid spacing])

#define ABSENT		1
#define PRESENT 	0
#define ERROR		1
#define NORMAL		0

ret_code_t read_if_transceiver_diagnostics(transceiver_t* _trans);
static void on_transceiver_los_change(transceiver_t* _trans);
static void on_transceiver_state_change(transceiver_t* _trans);
static void on_transceiver_plug_unplug_event(transceiver_t* _trans);
static alt_u32 timer_isr(void* context);

static void run_transceiver_absent_state(transceiver_t* _trans);
static void run_transceiver_com_check_state(transceiver_t* _trans);
static void run_transceiver_monitor_state(transceiver_t* _trans);
static void run_transceiver_depdendent_monitor_state(transceiver_t* _trans);
static void run_transceiver_fail_state(transceiver_t* _trans);
static void run_transceiver_query_state(transceiver_t* _trans);
/**
 * Default initialization of a transceiver object
 * @param _trans
 */
void create_transceiver(transceiver_t* _trans){
	int chnl = 0;
	CURRENT(state) = TRANSCEIVER_STATE_ABSENT;
	CURRENT(absent)= ABSENT;
	CURRENT(fault) = ERROR;
	CURRENT(los) = ERROR;
#if ENABLE_XFP_CODE
	CURRENT(not_ready) = ERROR;
#endif
	_trans->monitor.temperature.value = DEF_TEMP_HMIN;
	_trans->control.tx_disable.current = NORMAL;
	memset(&_trans->info.capabilities, 0, sizeof(transceiver_capabilities_t));
	_trans->info.capabilities.optical_ios_number = 1; // by default all transceivers have at least one output and one input
	custom_init_transceiver_data(_trans);
	reset_transceiver_id(_trans);
	reset_transceiver_optical_thresholds(_trans);
	reset_transceiver_temperature_thresholds(_trans);
	for(chnl = 0; chnl < _trans->info.capabilities.optical_ios_number; chnl++){
		_trans->monitor.input_power[chnl].value = _trans->monitor.input_power[chnl].physical_min;
		_trans->monitor.output_power[chnl].value = _trans->monitor.output_power[chnl].physical_min;
	}
	_trans->additional = malloc(sizeof(transceiver_state_machine_t));
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	sm->current_state.current = TRANSCEIVER_STATE_ABSENT;
	sm->state_handler = run_transceiver_absent_state;
}

void reset_transceiver_id(transceiver_t* _trans){
	memset(_trans->info.ID, 0, sizeof(_trans->info.ID));
#ifdef KURS_PROTOCOL
	memset(_trans->info.VN, 0, sizeof(_trans->info.VN));
	memset(_trans->info.SN, 0, sizeof(_trans->info.SN));
	memset(_trans->info.HN, 0, sizeof(_trans->info.HN));
	memset(_trans->info.FN, 0, sizeof(_trans->info.FN));
#endif
	switch(_trans->info.type){
#if ENABLE_QSFP_CODE || ENABLE_SFP_CODE || ENABLE_SFPP_CODE
	case TRANS_QSFP:
	case TRANS_SFP_OPTICAL:
	case TRANS_SFP_ELECTRICAL:
	case TRANS_SFPP_OPTICAL:
	case TRANS_SFPP_ELECTRICAL:
		memcpy(_trans->info.ID, "|||", 3);
		break;
#endif
#if ENABLE_CFP_CODE || ENABLE_OM5x7_CODE
	case TRANS_5x7:
	case TRANS_CFP:
		memcpy(_trans->info.ID, "||||", 4);
		break;
#endif
	default:
		memcpy(_trans->info.ID, "|||", 3);
		break;
	}
}

void reset_transceiver_temperature_thresholds(transceiver_t* _trans){
	_trans->monitor.temperature.warning_min = DEF_TEMP_WMIN;
	_trans->monitor.temperature.warning_max = DEF_TEMP_WMAX;
	_trans->monitor.temperature.critical_min = DEF_TEMP_CMIN;
	_trans->monitor.temperature.critical_max = DEF_TEMP_CMAX;
#ifdef KURS_PROTOCOL
	_trans->monitor.temperature.value = DEF_TEMP_HMIN;
#else
	_trans->monitor.temperature.value = -infinityf();
#endif
}

void reset_transceiver_optical_thresholds(transceiver_t* _trans){
	int chnl = 0;
	for(chnl = 0; chnl < _trans->info.capabilities.optical_ios_number; chnl++){
		switch(_trans->info.type){
#if ENABLE_QSFP_CODE
		case TRANS_QSFP:
			_trans->monitor.input_power[chnl].physical_max = DEF_QSFP_PIN_HMAX;
			_trans->monitor.input_power[chnl].physical_min = DEF_QSFP_PIN_HMIN;
			_trans->monitor.input_power[chnl].critical_max = DEF_QSFP_PIN_CMAX;
			_trans->monitor.input_power[chnl].critical_min = DEF_QSFP_PIN_CMIN;
			_trans->monitor.input_power[chnl].warning_max = DEF_QSFP_PIN_WMAX;
			_trans->monitor.input_power[chnl].warning_min = DEF_QSFP_PIN_WMIN;
			_trans->monitor.output_power[chnl].physical_max = DEF_QSFP_POUT_HMAX;
			_trans->monitor.output_power[chnl].physical_min = DEF_QSFP_POUT_HMIN;
			_trans->monitor.output_power[chnl].critical_max = DEF_QSFP_POUT_CMAX;
			_trans->monitor.output_power[chnl].critical_min = DEF_QSFP_POUT_CMIN;
			_trans->monitor.output_power[chnl].warning_max = DEF_QSFP_POUT_WMAX;
			_trans->monitor.output_power[chnl].warning_min = DEF_QSFP_POUT_WMIN;
			break;
#endif
#if ENABLE_CFP_CODE
		case TRANS_CFP:
			_trans->monitor.input_power[chnl].physical_max = DEF_CFP_PIN_HMAX;
			_trans->monitor.input_power[chnl].physical_min = DEF_CFP_PIN_HMIN;
			_trans->monitor.input_power[chnl].critical_max = DEF_CFP_PIN_CMAX;
			_trans->monitor.input_power[chnl].critical_min = DEF_CFP_PIN_CMIN;
			_trans->monitor.input_power[chnl].warning_max = DEF_CFP_PIN_WMAX;
			_trans->monitor.input_power[chnl].warning_min = DEF_CFP_PIN_WMIN;
			_trans->monitor.output_power[chnl].physical_max = DEF_CFP_POUT_HMAX;
			_trans->monitor.output_power[chnl].physical_min = DEF_CFP_POUT_HMIN;
			_trans->monitor.output_power[chnl].critical_max = DEF_CFP_POUT_CMAX;
			_trans->monitor.output_power[chnl].critical_min = DEF_CFP_POUT_CMIN;
			_trans->monitor.output_power[chnl].warning_max = DEF_CFP_POUT_WMAX;
			_trans->monitor.output_power[chnl].warning_min = DEF_CFP_POUT_WMIN;
			break;
#endif
#if ENABLE_SFPP_CODE
		case TRANS_SFPP_OPTICAL:
			_trans->monitor.input_power[chnl].physical_max = DEF_SFPP_PIN_HMAX;
			_trans->monitor.input_power[chnl].physical_min = DEF_SFPP_PIN_HMIN;
			_trans->monitor.input_power[chnl].critical_max = DEF_SFPP_PIN_CMAX;
			_trans->monitor.input_power[chnl].critical_min = DEF_SFPP_PIN_CMIN;
			_trans->monitor.input_power[chnl].warning_max = DEF_SFPP_PIN_WMAX;
			_trans->monitor.input_power[chnl].warning_min = DEF_SFPP_PIN_WMIN;
			_trans->monitor.output_power[chnl].physical_max = DEF_SFPP_POUT_HMAX;
			_trans->monitor.output_power[chnl].physical_min = DEF_SFPP_POUT_HMIN;
			_trans->monitor.output_power[chnl].critical_max = DEF_SFPP_POUT_CMAX;
			_trans->monitor.output_power[chnl].critical_min = DEF_SFPP_POUT_CMIN;
			_trans->monitor.output_power[chnl].warning_max = DEF_SFPP_POUT_WMAX;
			_trans->monitor.output_power[chnl].warning_min = DEF_SFPP_POUT_WMIN;
			break;
#endif
#if ENABLE_SFP_CODE
		case TRANS_SFP_OPTICAL:
			_trans->monitor.input_power[chnl].physical_max = DEF_SFP_PIN_HMAX;
			_trans->monitor.input_power[chnl].physical_min = DEF_SFP_PIN_HMIN;
			_trans->monitor.input_power[chnl].critical_max = DEF_SFP_PIN_CMAX;
			_trans->monitor.input_power[chnl].critical_min = DEF_SFP_PIN_CMIN;
			_trans->monitor.input_power[chnl].warning_max = DEF_SFP_PIN_WMAX;
			_trans->monitor.input_power[chnl].warning_min = DEF_SFP_PIN_WMIN;
			_trans->monitor.output_power[chnl].physical_max = DEF_SFP_POUT_HMAX;
			_trans->monitor.output_power[chnl].physical_min = DEF_SFP_POUT_HMIN;
			_trans->monitor.output_power[chnl].critical_max = DEF_SFP_POUT_CMAX;
			_trans->monitor.output_power[chnl].critical_min = DEF_SFP_POUT_CMIN;
			_trans->monitor.output_power[chnl].warning_max = DEF_SFP_POUT_WMAX;
			_trans->monitor.output_power[chnl].warning_min = DEF_SFP_POUT_WMIN;
			break;
#endif
		default:
			break;

		}
	}
}

/**
 * @brief  Sends a command to the NiOS processor to read client optical power threshold (command through UART).
 * @param[in] client client of which ID is to be read
 * @return \c RET_SUCCESS in case of success
 * @since 0.0.1
 */
__weak ret_code_t read_if_transceiver_info(transceiver_t* _trans){
	ret_code_t ret = RET_FAIL;
switch(_trans->info.type){
#if ENABLE_SFPP_CODE
	case TRANS_SFPP_OPTICAL:
		return get_sfpp_info(_trans);
#endif
#if ENABLE_SFP_CODE
	case TRANS_SFP_OPTICAL:
	case TRANS_SFP_ELECTRICAL:
		return get_sfp_info(_trans);
		break;
#endif
#if ENABLE_QSFP_CODE
	case TRANS_QSFP:
		break;
#endif
#if ENABLE_CFP_CODE
	case TRANS_CFP:
	{
		cfp_device_t* _cfp = (cfp_device_t*)(_trans->device.device);
		return get_cfp_info(_cfp);
	}
#endif
#if ENABLE_OM5x7_CODE
	case TRANS_5x7:
		break;
#endif
	default:
		break;
	}
	return ret;
}

ret_code_t read_if_transceiver_opt_pwr_thresholds(transceiver_t* _trans){
	ret_code_t ret = RET_SUCCESS;
	switch(_trans->info.type){
#if ENABLE_SFPP_CODE
	case TRANS_SFPP_OPTICAL:
		return get_sfpp_optical_power_thresholds(_trans);
#endif
#if ENABLE_SFP_CODE
	case TRANS_SFP_OPTICAL:
		_trans->monitor.input_power->physical_min = DEF_SFP_PIN_HMIN;
		_trans->monitor.output_power->physical_min = DEF_SFP_PIN_HMIN;
		return get_sfp_optical_power_thresholds(_trans);
		break;
#endif
#if ENABLE_QSFP_CODE
	case TRANS_QSFP:
		break;
#endif
#if ENABLE_CFP_CODE
	case TRANS_CFP:
	{
		cfp_device_t* _cfp = (cfp_device_t*)(_trans->device.device);
		return cfp_get_optical_power_thresholds(_cfp);
	}
		break;
#endif
#if ENABLE_OM5x7_CODE
	case TRANS_5x7:
		break;
#endif
	default:
		break;
	}
	return ret;
}

ret_code_t read_if_transceiver_diagnostics(transceiver_t* _trans){
	ret_code_t rc = RET_SUCCESS;;
	switch(_trans->info.type){
#if ENABLE_SFPP_CODE
	case TRANS_SFPP_OPTICAL:
		rc = get_sfpp_diagnostics(_trans);
		break;
#endif
#if ENABLE_SFP_CODE
	case TRANS_SFP_OPTICAL:
	case TRANS_SFP_ELECTRICAL:
		rc = get_sfp_diagnostics(_trans);
		break;
#endif
#if ENABLE_QSFP_CODE
	case TRANS_QSFP:
		break;
#endif
#if ENABLE_CFP_CODE
	case TRANS_CFP:
	{
		cfp_device_t* _cfp = (cfp_device_t*)(_trans->device.device);
		rc = get_cfp_diagnostics(_cfp);
	}
	break;
#endif
#if ENABLE_OM5x7_CODE
	case TRANS_5x7:
		break;
#endif
	default:
		break;
	}
	return rc;
}

static void run_transceiver_fail_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	_trans->monitor.state.current = TRANSCEIVER_STATE_FAIL;
	sm->state_handler = run_transceiver_com_check_state;
}

static void run_transceiver_depdendent_monitor_state(transceiver_t* _trans){
	switch(_trans->info.type){
#if ENABLE_CFP_CODE
	case TRANS_CFP:
		run_cfp_monitor_state((cfp_device_t*)_trans->device.device);
		break;
#endif
#if ENABLE_OM5x7_CODE
	case TRANS_5x7:
		break;
#endif
#if ENABLE_QSFP_CODE
	case TRANS_QSFP:
		break;
#endif
#if ENABLE_SFPP_CODE
	case TRANS_SFPP_ELECTRICAL:
	case TRANS_SFPP_OPTICAL:
#endif
#if ENABLE_SFP_CODE
	case TRANS_SFP_ELECTRICAL:
	case TRANS_SFP_OPTICAL:
#endif
#if ENABLE_XFP_CODE
		case TRANS_XFP:
#endif
		/*for SFPs, SFP+s, if the TX_FAULT is deasserted, we can consider it is operational.*/
		UPDATE_STATE(TRANSCEIVER_STATE_OK);
		break;
	case TRANS_UNKNOWN:
	default:
		break;
	}
}

ret_code_t is_in_monitor_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	ret_code_t rc = RET_FAIL;
	if(sm->state_handler == run_transceiver_monitor_state){
		rc = RET_SUCCESS;
	}
	return (rc);
}

ret_code_t is_in_query_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	ret_code_t rc = RET_FAIL;
	if(sm->state_handler == run_transceiver_query_state){
		rc = RET_SUCCESS;
	}
	return (rc);
}

ret_code_t is_in_comcheck_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	ret_code_t rc = RET_FAIL;
	if(sm->state_handler == run_transceiver_com_check_state){
		rc = RET_SUCCESS;
	}
	return (rc);
}

ret_code_t is_in_absent_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	ret_code_t rc = RET_FAIL;
	if(sm->state_handler == run_transceiver_absent_state){
		rc = RET_SUCCESS;
	}
	return (rc);
}

ret_code_t is_in_booting_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	ret_code_t rc = RET_FAIL;
	if(sm->state_handler == run_transceiver_booting_state){
		rc = RET_SUCCESS;
	}
	return (rc);
}

ret_code_t is_in_fail_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	ret_code_t rc = RET_FAIL;
	if(sm->state_handler == run_transceiver_fail_state){
		rc = RET_SUCCESS;
	}
	return (rc);
}

static void run_transceiver_monitor_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	state_cb _new_state;
	if(CURRENT(absent)){
		UPDATE_STATE(TRANSCEIVER_STATE_ABSENT);
		_new_state = run_transceiver_absent_state;
	}else if(sm->input_boot_start){
		_new_state = run_transceiver_booting_state;
	}else{
		UPDATE_IO(fault);
		if(CURRENT(fault)){
			if(CHANGE(fault)){
				UPDATE_STATE(TRANSCEIVER_STATE_NOT_READY);
			}
		}else{
			/* Every transceiver has different set of parameters to look at to decide whether it is operational or not
			 * CFPs for example have IOs as well as internal state registers that we can read.
			 * */
			run_transceiver_depdendent_monitor_state(_trans);
		}
		UPDATE_IO(los);
		if(CHANGE(los)){
			on_transceiver_los_change(_trans);
		}
		read_if_transceiver_diagnostics(_trans);
		_new_state = run_transceiver_monitor_state; /*don't change state*/
	}
	if(_new_state != sm->state_handler){
		sm->state_handler = _new_state;
		on_transceiver_state_change(_trans);
	}
}

static void run_transceiver_query_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	state_cb _new_state;
	if(CURRENT(absent)){
		UPDATE_STATE(TRANSCEIVER_STATE_ABSENT);
		_new_state = run_transceiver_absent_state;
	}else if(sm->input_boot_start){
		_new_state = run_transceiver_booting_state;
	}else{
		ret_code_t rc = read_if_transceiver_info(_trans);
		if(rc == RET_SUCCESS){
//			printf("DBG <%d, %d>:Transceiver INFO:\n", _trans->index, alt_nticks());
//			printf("\tCapabilities listing:\n");
			if(_trans->info.capabilities.OPTICAL_POWER_MONITOR){
				/* re-allocate input power and output power arrays*/
				if(_trans->monitor.input_power != NULL){
					free(_trans->monitor.input_power);
				}
				if(_trans->monitor.output_power != NULL){
					free(_trans->monitor.output_power);
				}
				_trans->monitor.input_power = malloc(_trans->info.capabilities.optical_ios_number * sizeof(diagnostics_t));
				_trans->monitor.output_power = malloc(_trans->info.capabilities.optical_ios_number * sizeof(diagnostics_t));

//				if(_trans->control.threshold_mode == THRESHOLD_TRANSCEIVER){
					rc = read_if_transceiver_opt_pwr_thresholds(_trans);
//				}
				if(rc == RET_SUCCESS){
					/*TBD*/
				}else{
					/*TBD*/
				}
//				printf("\t\t\tDDI %d\n", _trans->info.capabilities.OPTICAL_POWER_MONITOR);
			}
			if(_trans->info.capabilities.tx_tunable){
				/*restore channels by setting their corresponding change flags*/
				_trans->control.tx_freq.change = 1;
//				printf("\t\t\tTX Tuning %d\n", _trans->info.capabilities.OPTICAL_POWER_MONITOR);
			}
			if(_trans->info.capabilities.rx_tunable){
//				printf("\t\t\tRX Tuning %d\n", _trans->info.capabilities.OPTICAL_POWER_MONITOR);
				_trans->control.rx_freq.change = 1;
			}
			if(_trans->info.capabilities.tx_power_control){
				_trans->control.tx_power.change = 1;
			}
			_new_state = run_transceiver_monitor_state;
		}else{
			_new_state = run_transceiver_com_check_state;
		}
	}
	if(_new_state != sm->state_handler){
		sm->state_handler = _new_state;
		on_transceiver_state_change(_trans);
	}
}

void run_transceiver_com_check_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	if(CURRENT(absent)){
		UPDATE_STATE(TRANSCEIVER_STATE_ABSENT);
		sm->state_handler = run_transceiver_absent_state;
	}else if(sm->input_boot_start){
		sm->state_handler = run_transceiver_booting_state;
	}else{
		ret_code_t rc;
		switch(_trans->info.type){
#if ENABLE_CFP_CODE
		case TRANS_CFP:
			rc = cfp_com_check((cfp_device_t*)(_trans->device.device));
			break;
#endif
#if ENABLE_SFPP_CODE
		case TRANS_SFPP_OPTICAL:
		case TRANS_SFPP_ELECTRICAL:
			rc = sfpp_com_check((sfpp_device_t*)(_trans->device.device));
			break;
#endif
#if ENABLE_SFP_CODE
		case TRANS_SFP_OPTICAL:
		case TRANS_SFP_ELECTRICAL:
			rc = sfp_com_check((sfp_device_t*)(_trans->device.device));
			break;
#endif
#if ENABLE_XFP_CODE
		case TRANS_XFP:
			break;
#endif
		default:
			break;
		}
		if(rc == RET_SUCCESS){
			sm->state_handler = run_transceiver_query_state;
			sm->input_com_lost = 0;
//			printf("DBG <%d, %d>:COMM check OK\n", _trans->index, alt_nticks());
		}else{
			sm->input_com_lost++;
			if(sm->input_com_lost == 3){
				sm->state_handler = run_transceiver_fail_state;
//				printf("DBG <%d, %d>:COMM LOST\n", _trans->index, alt_nticks());
			}
		}
	}
}

void run_transceiver_booting_state(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	if(sm->input_boot_start){
		_trans->monitor.state.current = TRANSCEIVER_STATE_REBOOTING;
		_trans->control.reboot.change = 1;
		sm->input_boot_done = 0;
		sm->input_boot_start = 0;
	}else if(sm->input_boot_done){
		sm->state_handler = run_transceiver_com_check_state;
		sm->input_boot_done = 0;
	}
}

void run_transceiver_absent_state(transceiver_t* _trans){
	if(CURRENT(absent) && !PREVIOUS(absent)){
		_trans->monitor.state.current = TRANSCEIVER_STATE_ABSENT;
		int chnl = 0;
		CURRENT(fault) = ERROR;
		CURRENT(los) = ERROR;
#if ENABLE_XFP_CODE
		CURRENT(not_ready) = ERROR;
#endif
		_trans->monitor.temperature.value = DEF_TEMP_HMIN;
		reset_transceiver_id(_trans);
		reset_transceiver_optical_thresholds(_trans);
		reset_transceiver_temperature_thresholds(_trans);
		for(chnl = 0; chnl < _trans->info.capabilities.optical_ios_number; chnl++){
			_trans->monitor.input_power[chnl].value = _trans->monitor.input_power[chnl].physical_min;
			_trans->monitor.output_power[chnl].value = _trans->monitor.output_power[chnl].physical_min;
		}
		on_transceiver_state_change(_trans);
//		printf("DBG <%d, %d>:Transceiver unplugged\n", _trans->index, alt_nticks());
	}
}

/**
 * Do not modify this function, implement it in ipglb_adapter.c
 */
__weak ret_code_t read_transceiver_dynamic_data(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	/*has the transceiver been plugged or unplugged*/
	UPDATE_IO(absent);
	if(CHANGE(absent)){
		on_transceiver_plug_unplug_event(_trans);
	}
	sm->state_handler(_trans);
	return RET_SUCCESS;
}

static void on_transceiver_plug_unplug_event(transceiver_t* _trans){
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*)_trans->additional;
	/*on unplug event*/
	if(CURRENT(absent)){
		sm->state_handler = run_transceiver_absent_state;
	}else{
		sm->input_boot_start = 1;
		sm->state_handler = run_transceiver_booting_state;
	}
	callback_on_transceiver_plug_change(_trans);
}

/**
 * Custom handling of a transceiver plug event.
 * @param[in,out] _trans pointer to the transceiver object
 */
__weak void callback_on_transceiver_plug_change(transceiver_t* _trans){
	/*
	 * Do not modify, implement in main application.
	 */
}

/**
 * Transceiver change of status event handler.
 * @param _trans pointer to the transceiver structure
 * @note do not modify this function, for any added functionality, implement callback_on_transceiver_status_change
 */
static void on_transceiver_state_change(transceiver_t* _trans){
	/*additional tasks*/
	callback_on_transceiver_state_change(_trans);
}

static alt_u32 timer_isr(void* context){
	transceiver_t* _trans = (transceiver_t*)context;
	transceiver_state_machine_t* sm = (transceiver_state_machine_t*) _trans->additional;
	if(_trans != NULL){
		sm->input_boot_done = 1;
	}
	return 0;
}

/**
 * In case additional tasks are needed on the event of a transceiver's state change, implement this in ipglb_adapter.c.
 * @param _trans pointer to the transceiver object
 */
__weak void callback_on_transceiver_state_change(transceiver_t* _trans){
  /* NOTE : This is function could be implemented in user file
            when the callback is needed,
  */

}

/**
 * Transceiver change in LOS state event handler.
 * @param _trans pointer to the transceiver structure
 * @note do not modify this function, for any added functionality implement callback_on_transceiver_los_change
 */
static void on_transceiver_los_change(transceiver_t* _trans){
	callback_on_transceiver_los_change(_trans);
}

/**
 * Implement this in main.c in case you need additional functionality on transceiver LOS changed event.
 * @param _trans pointer to the transceiver object
 */
__weak void callback_on_transceiver_los_change(transceiver_t* _trans){
  /* NOTE : This is function could be implemented in user file
            when the callback is needed,
  */

}
/**
 * Project dependent initialization of a transceiver.
 * @param _trans pointer to the transceiver object
 */
__weak void custom_init_transceiver_data(transceiver_t* _trans){
  /* NOTE : This is function could be implemented in user file
            when the callback is needed,
  */

}

__weak alt_u32 get_transceiver_los(transceiver_t* _trans){
	return GET_TRANS_IO(los);
}

__weak alt_u32 get_transceiver_txfault(transceiver_t* _trans){
	return GET_TRANS_IO(fault);
}

/**
 *
 */
__weak alt_u32 get_transceiver_absent(transceiver_t* _trans){
	return GET_TRANS_IO(absent);
}

#if ENABLE_XFP_CODE
/**
 *
 */
__weak alt_u32 get_transceiver_not_ready(transceiver_t* _trans){
	return GET_TRANS_IO(not_ready);
}
#endif

/**
 * @brief  Enables or disables the transmitter of a transceiver by modifying the corresponding GPIO.
 * @note the calling application has to make sure that the address and bit of the IO have been properly mapped.
 * @param[in, out] _trans pointer to the transceiver object
 * @since 0.0.1
 */
__weak void set_transceiver_txdisable(transceiver_t* _trans)
{
	SET_TRANS_IO(tx_disable, _trans->control.tx_disable.next);
	if(_trans->control.tx_disable.next != _trans->control.tx_disable.current)
	{
		callback_on_transceiver_txdisable_set(_trans);
	}
}

/**
 * @brief  Gets the state of a transciver's transmitter by reading the corresponding GPIO.
 * @note the calling application has to make sure that the address and bit of the IO have been properly mapped.
 * @param[in, out] _trans pointer to the transceiver object
 * @return the bit state
 * @since 0.0.1
 */
__weak alt_u32 get_if_transceiver_txdisable(transceiver_t* _trans){
	return GET_TRANS_IO(tx_disable);
}

/**
 * @brief  Callback function for additional tasks when TX_DIS is set.
 * @note DO not modify this function, implement in main application.
 * @param[in, out] _trans pointer to the transceiver object
 * @since 0.0.1
 */
__weak void callback_on_transceiver_txdisable_set(transceiver_t* _trans){
	/*
	 * Do not modify, implement in main application.
	 */
}

ret_code_t set_if_transceiver_txpower(transceiver_t* _trans){
	switch(_trans->info.type){
#if ENABLE_QSFP_CODE
		case TRANS_QSFP:
			break;
#endif
#if ENABLE_CFP_CODE
		case TRANS_CFP:
		{
			cfp_device_t* _cfp = (cfp_device_t*)(_trans->device.device);
			cfp_set_desired_tx_power(_cfp, _trans->control.tx_power.next);
		}
		break;
#endif
#if ENABLE_OM5x7_CODE
		case TRANS_5x7:
			break;
#endif
#if ENABLE_XFP_CODE
		case TRANS_XFP:
			break;
#endif
		default:
			break;
		}
	return RET_SUCCESS;
}

ret_code_t set_if_transceiver_tx_freq(transceiver_t* _trans){
	if(!_trans->info.capabilities.tx_tunable){
		return RET_INVALID_PARAMS;
	}else{
	switch(_trans->info.type){
#if ENABLE_QSFP_CODE
		case TRANS_QSFP:
			break;
#endif
#if ENABLE_CFP_CODE
		case TRANS_CFP:
		{
			cfp_device_t* _cfp = (cfp_device_t*)(_trans->device.device);
			return cfp_set_tx_freq(_cfp, _trans->control.tx_freq.next);
		}
#endif
#if ENABLE_OM5x7_CODE
		case TRANS_5x7:
			break;
#endif
#if ENABLE_XFP_CODE
		case TRANS_XFP:
			break;
#endif
#if ENABLE_SFPP_CODE
		case TRANS_SFPP_OPTICAL:
			break;
#endif
		default:
			return RET_INVALID_PARAMS;
		}
	}
	return RET_SUCCESS;
}

ret_code_t set_if_transceiver_rx_freq(transceiver_t* _trans){
	if(!_trans->info.capabilities.rx_tunable){
		return RET_INVALID_PARAMS;
	}else{
		switch(_trans->info.type){
#if ENABLE_QSFP_CODE
		case TRANS_QSFP:
			break;
#endif
#if ENABLE_CFP_CODE
		case TRANS_CFP:
		{
			cfp_device_t* _cfp = (cfp_device_t*)(_trans->device.device);
			return cfp_set_rx_freq(_cfp, _trans->control.rx_freq.next);
		}
		break;
#endif
#if ENABLE_OM5x7_CODE
		case TRANS_5x7:
			break;
#endif
#if ENABLE_SFPP_CODE
		case TRANS_SFPP_OPTICAL:
			break;
#endif
		default:
			return RET_INVALID_PARAMS;
		}
	}
	return RET_SUCCESS;
}

/**
 * This function bringups a transceiver.
 * @param _trans pointer to the transceiver object
 * @return \c RET_SUCCESS on success.
 */
ret_code_t boot_transceiver(transceiver_t* _trans)
{
	switch(_trans->info.type)
	{
#if ENABLE_SFPP_CODE
		case TRANS_SFPP_ELECTRICAL:
		case TRANS_SFPP_OPTICAL:
#endif
#if ENABLE_SFP_CODE
		case TRANS_SFP_ELECTRICAL:
		case TRANS_SFP_OPTICAL:
#endif
#if ENABLE_XFP_CODE
		case TRANS_XFP:
#endif
#if ENABLE_QSFP_CODE
		case TRANS_QSFP:
#endif
		/*
		 * for these transceivers, we should only start a 1 second timer which
		 * is the wake up time according to the MSA
		 */
		if(alt_alarm_start(&_trans->timer, alt_ticks_per_second(), timer_isr, _trans) != 0){
			/*TODO handle error cases*/
		}
		return RET_SUCCESS;
#if ENABLE_OM5x7_CODE
		case TRANS_5x7:
		break;
#endif
#if ENABLE_CFP_CODE
		case TRANS_CFP:
		return boot_cfp((cfp_device_t*)_trans->device.device);
#endif
	default:
		break;
	}
	return RET_SUCCESS;
}
/**
 * This function reads the TX and RX frequency of a tunable transceiver.
 * @param _trans pointer to the transceiver object
 * @return \c RET_SUCCESS in case of success
 */
ret_code_t read_if_transceiver_freq(transceiver_t* _trans)
{
	return RET_SUCCESS;
}
