/*
 * ipglb_interface.h
 *
 *  Created on: May 8, 2018
 *      Author: dcham
 */

#ifndef IPGLB_INTERFACE_H_
#define IPGLB_INTERFACE_H_

#include "ipg_types.h"
#include "ipglb_port.h"
#include "ipglb_transceiver.h"

/**
 * Definition of an interface object
 */
typedef struct{
	int 				index;			/*!<Specifies the index of the interface from 0 to 99.*/
	transceiver_t 		transceiver;	/*!<Specifies the transceiver inserted in the interface.*/
	port_t				port;			/*!<Specifies the port object of the interface*/
	ipg_control32_t	    reset_counters;
	char				destination[IF_DESTINATION_MAX_LEN];
	char				description[IF_DESCRIPTION_MAX_LEN];
	activity_monitor_t	monitor;
	ipg_control32_t		service_state;	/*!<Specifies the service state of the interface, can be a value of @ref interface_service_state_t*/
}interface_t;

void init_interface_data(interface_t* _if);
void custom_init_interface_data(interface_t* _if);
void update_if_dynamic_data(interface_t* _if);
void callback_update_if_dynamic_data(interface_t* _if);
#endif /* IPGLB_INTERFACE_H_ */
