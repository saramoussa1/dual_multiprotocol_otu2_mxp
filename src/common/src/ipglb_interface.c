/*
 * ipglb_interface.c
 *
 *  Created on: May 8, 2018
 *      Author: dcham
 */

#include "../inc/ipglb_interface.h"
#include "../inc/ipglb_transceiver.h"
#include "../inc/ipglb_port.h"
#include "string.h"

#ifndef __weak
#define __weak	__attribute__((weak))
#endif

/**
 * Default initialization oGf an interface.
 * This is the project independent initialization.
 *@param _if	pointer to the interface
 */
void init_interface_data(interface_t* _if){

	custom_init_interface_data(_if);
	// initialize port data
	_if->port.index = _if->index;
	init_port_data(&_if->port);

	// initialize transceiver data
	_if->transceiver.index = _if->index; // transceiver index is 0 based whereas
	create_transceiver(&_if->transceiver);

}

/**
 * Custom initialization of an interface.
 * This is the project dependent initialization. It should be implemented in ipglb_adapter.c.
 * @param _if pointer to the interface
 */
__weak void custom_init_interface_data(interface_t* _if){
	  /* NOTE : This is function could be implemented in user file
	            when the callback is needed,
	  */
}

/**
 * Common implementation of updating an interface dynamic data.
 * @param _if pointer to the interface object
 */
void update_if_dynamic_data(interface_t* _if_ptr) {
//	int index = _if->index;
	read_transceiver_dynamic_data(&_if_ptr->transceiver);
	// if transceiver is plugged, proceed with updating other parameters
	if(_if_ptr->transceiver.monitor.state.current != TRANSCEIVER_STATE_ABSENT){
		read_if_port_rate(&_if_ptr->port);
		switch(_if_ptr->port.monitor.prot)
		{
			#if ENABLE_ETH_CODE
				case PORT_PROTOCOL_ETHERNET:
					update_port_ethernet_stats(&_if_ptr->port);
					break;
			#endif
			#if ENABLE_OTUx_CODE
				case PORT_PROTOCOL_OTU:
					update_port_otux_stats(&_if_ptr->port);
					break;
			#endif
			#if ENABLE_STMx_CODE
				case PORT_PROTOCOL_SONET:
			#endif
			#if ENABLE_SDHx_CODE
				case PORT_PROTOCOL_SDH:
			#endif
			update_port_stmx_stats(&_if_ptr->port);
			break;
			#if ENABLE_FCx_CODE
				case PORT_PROTOCOL_FIBER_CHANNEL:
					update_port_fcx_stats(&_if_ptr->port);
					break;
			#endif
			default:
				break;
		}
	}
	if(_if_ptr->port.info.capabilities.FEC_CAPABLE){
		update_port_fec_stats(&_if_ptr->port);
	}
	/*note: PFC counters are updated in the background and are reported inside handler function*/
	callback_update_if_dynamic_data(_if_ptr);
}

/**
 * Callback for project dependent or additional behavior to update an interface dynamic data.
 * This function should not be called by the main application, it will be called automatically by
 * update_if_dynamic_data.
 * @param _if pointer to the interface object
 */
__weak void callback_update_if_dynamic_data(interface_t* _if)
{
	/*
	 * Do not modify, implement in main application
	 */
}
