/*
 * ipglb_port.c
 *
 *  Created on: May 31, 2018
 *      Author: dcham
 */

#include "../inc/ipglb_port.h"
#include "../inc/ipg_types.h"
#include "string.h"
#include "stdlib.h"
#include "../../configs.h"

#ifndef __weak
#define __weak __attribute__((weak))
#endif

#define ABSENT		1
#define PRESENT 	0
#define ERROR		1
#define NORMAL		0

void init_port_data(port_t* _port){
	// any custom initialization
	custom_init_port_data(_port);

	_port->control.loopback_mode.current = _port->control.loopback_mode.next = MODE_LB_NORMAL;
	reset_port_statistics(_port);
#if ENABLE_FEC_CODE
	reset_port_fec_statistics(_port);
#endif
}

/**
 * Custom initialization of an interface's port data.
 * @param _port pointer to the port data structure.
 */
__weak void custom_init_port_data(port_t* _port){
	  /* NOTE : This is function could be implemented in user file
	            when the callback is needed,
	  */
}

__weak void reset_port_statistics(port_t* _port){
#warning "implement this function in main application"
#ifndef KURS_PROTOCOL
	memset(&_port->monitor.statistics, 0, sizeof(_port->monitor.statistics));
	switch(_port->monitor.prot){
#if ENABLE_ETH_CODE
	case PORT_PROTOCOL_ETHERNET:
		_port->monitor.statistics.eth_sts.alarms.LINK_DOWN = ERROR;
		_port->monitor.statistics.eth_sts.alarms.REMOTE_LINK_DOWN = ERROR;
		break;
#endif
#if ENABLE_OTUx_CODE
	case PORT_PROTOCOL_OTU:
		_port->monitor.statistics.otux_sts.alarms.LOF = ERROR;
		_port->monitor.statistics.otux_sts.alarms.BDI = ERROR;
		_port->monitor.statistics.otux_sts.alarms.LOM = ERROR;
		break;
#endif
#if ENABLE_STMx_CODE
	case PORT_PROTOCOL_SONET:
		_port->monitor.statistics.stmx_sts.alarms.LOF = ERROR;
		_port->monitor.statistics.stmx_sts.alarms.MS_AIS = ERROR;
		_port->monitor.statistics.stmx_sts.alarms.MS_RDI = ERROR;
		break;
#endif
#if ENABLE_FCx_CODE
	case PORT_PROTOCOL_FIBER_CHANNEL:
		break;
#endif
	case PORT_PROTOCOL_UNKNOWN:
	default:
		break;
	}
	if(_port->info.capabilities.PFC_CAPABLE){
		reset_pfc_statistics(_port->pfc);
	}
	if(_port->info.capabilities.FEC_CAPABLE){
		reset_port_fec_statistics(_port);
	}
#endif
}

//#ifdef KURS_PROTOCOL
//void reset_port_alarms(port_t* _port){
//	memset(&_port->monitor.alarms, 0, sizeof(_port->monitor.alarms));
//	switch(_port->monitor.prot){
//#if ENABLE_ETH_CODE
//	case PORT_PROTOCOL_ETHERNET:
//		_port->monitor.statistics.eth_sts.alarms.LINK_DOWN = ERROR;
//		break;
//#endif
//#if ENABLE_OTUx_CODE
//	case PORT_PROTOCOL_OTU:
//		//ODU alarms
//		_port->monitor.odux_alrm = odu_AIS;
//		//OTU alarms
//		_port->monitor.statistics.otux_sts.alarms.LOF = ERROR;
//		break;
//#endif
//#if ENABLE_STMx_CODE
//	case PORT_PROTOCOL_SONET:
//		_port->monitor.statistics.stmx_sts.alarms.LOF = ERROR;
//		break;
//#endif
//#if ENABLE_FCx_CODE
//	case PORT_PROTOCOL_FIBER_CHANNEL:
//		break;
//#endif
//	case PORT_PROTOCOL_UNKNOWN:
//	default:
//		break;
//	}
//}
//#endif

#if ENABLE_FEC_CODE
void reset_port_fec_statistics(port_t* _port){
	_port->monitor.fec_statistics.corrected_ber = 0;
	_port->monitor.fec_statistics.prefec_ber = 0;
	_port->monitor.fec_statistics.postfec_ber = 0;
	_port->monitor.fec_statistics.corrected_bits.current = _port->monitor.fec_statistics.corrected_bits.accumulated = 0;
	_port->monitor.fec_statistics.uncorrected_cw.current = _port->monitor.fec_statistics.uncorrected_cw.accumulated = 0;
	_port->monitor.fec_statistics.fec_fail.current = _port->monitor.fec_statistics.fec_fail.previous = 1;
}
#endif

void reset_port_counters(port_t* _port){
	switch(_port->monitor.prot){
#if ENABLE_ETH_CODE
	case PORT_PROTOCOL_ETHERNET:
		_port->monitor.statistics.eth_sts.rx_crc.accumulated = _port->monitor.statistics.eth_sts.rx_crc.current = 0;
		_port->monitor.statistics.eth_sts.rx_ok.accumulated = _port->monitor.statistics.eth_sts.rx_ok.current = 0;
		_port->monitor.statistics.eth_sts.tx_ok.accumulated = _port->monitor.statistics.eth_sts.tx_ok.current = 0;
#if PORT_VER == PORT_V2
		_port->monitor.statistics.eth_sts.rx_octs.accumulated = _port->monitor.statistics.eth_sts.rx_octs.current = 0;
		_port->monitor.statistics.eth_sts.tx_octs.accumulated = _port->monitor.statistics.eth_sts.tx_octs.current = 0;
#endif
		break;
#endif
#if ENABLE_OTUx_CODE
	case PORT_PROTOCOL_OTU:
		_port->monitor.statistics.otux_sts.bip_errors.accumulated = _port->monitor.statistics.otux_sts.bip_errors.current = 0;
		break;
#endif
#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
		case PORT_PROTOCOL_SONET:
		case PORT_PROTOCOL_SDH:
		_port->monitor.statistics.stmx_sts.bip_errors.accumulated = _port->monitor.statistics.stmx_sts.bip_errors.current = 0;
		break;
#endif
#if ENABLE_FCx_CODE
	case PORT_PROTOCOL_FIBER_CHANNEL:
		break;
#endif
	case PORT_PROTOCOL_UNKNOWN:
	default:
		break;
	}
#if ENABLE_FEC_CODE
	if(_port->info.capabilities.FEC_CAPABLE){
		_port->monitor.fec_statistics.corrected_bits.current = _port->monitor.fec_statistics.corrected_bits.accumulated = 0;
		_port->monitor.fec_statistics.uncorrected_cw.current = _port->monitor.fec_statistics.uncorrected_cw.accumulated = 0;
	}
#endif
#if ENABLE_PFC_CODE
	if(_port->info.capabilities.PFC_CAPABLE && (_port->pfc != NULL)){
		reset_pfc_statistics(_port->pfc);
		reset_pfc_hw_counters(_port->pfc);
	}
#endif
}

/**
 * This function reads the current rate of a port.
 * @param[in,out] _port	pointer to the port object.
 * @return \c RET_SUCCESS on success
 */
__weak ret_code_t read_if_port_rate(port_t* _port){
	/*
	 * Do not modify, implement in main application.
	 */
	return RET_FAIL;
}
/**
 * This function places a port in loopback or normal operation modes.
 * It is project dependent and has to be implemented in main application.
 * @param[in,out] _port pointer to the port object
 * @param[in] _lb_mode specifies the requested mode. Can be any value of loopback_mode_t.
 */
__weak ret_code_t set_if_port_loopback(port_t* _port, loopback_mode_t _lb_mode){
#warning "implement this function in main application"
	/*
	 * Do not modify this function. Implement in main application
	 */
	return RET_SUCCESS;
}

/**
 * This function changes the FEC type of a port.
 * It is project dependent and has to be implemented in main application.
 * @param[in,out] _port pointer to the port object
 * @return \c RET_SUCCESS on success
 */
__weak ret_code_t set_if_port_fectype(port_t* _port){
#warning "implement this function in main application"
	/*
	 * Do not modify this function. Implement in main application
	 */
	return RET_SUCCESS;
}

/**
 * This function reads the FEC type of a port.
 * It is project dependent and has to be implemented in main application.
 * @param[in,out] _port pointer to the port object
 * @return \c RET_SUCCESS on success
 */
__weak ret_code_t get_if_port_fectype(port_t* _port){
#warning "implement this function in main application"
	/*
	 * Do not modify this function. Implement in main application
	 */
	return RET_SUCCESS;
}

/**
 * This function changes the FEC type of a port.
 * It is project dependent and has to be implemented in main application.
 * @param[in,out] _port pointer to the port object
 * @return \c RET_SUCCESS on success
 */
__weak ret_code_t set_if_port_rate(port_t* _port){
#warning "implement this function in main application"
	/*
	 * Do not modify this function. Implement in main application
	 */
	return RET_SUCCESS;
}

__weak void custom_port_mmap(port_t* _port){
//#error "implement this function in main application"
	/*
	 * Do not modify, implement in main application
	 */
}

#if ENABLE_ETH_CODE
__weak void update_port_ethernet_stats(port_t* _port){
	/*
	 * Do not modify, implement in main application
	 */

}
#endif

#if ENABLE_OTUx_CODE
__weak void update_port_otux_stats(port_t* _port){
	/*
	 * Do not modify, implement in main application
	 */

}
#endif

#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
__weak void update_port_stmx_stats(port_t* _port){
	/*
	 * Do not modify, implement in main application
	 */
}
#endif

#if ENABLE_FCx_CODE
__weak void update_port_fcx_stats(port_t* _port){
	/*
	 * Do not modify, implement in main application
	 */
}
#endif

#if ENABLE_FEC_CODE
__weak void update_port_fec_stats(port_t* _port){
	/*
	 * Do not modify, implement in main application
	 */
}
#endif
