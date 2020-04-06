/*
 * ipglb_port.h
 *
 *  Created on: Jun 12, 2018
 *      Author: dcham
 */

#ifndef IPGLB_CARD_H_
#define IPGLB_CARD_H_

#include "ipg_types.h"
#include "ipglb_interface.h"

typedef struct{
	interface_t 	interfaces[INTERFACE_COUNT]; // indexed on 1-based index
#ifdef STM32F207xx
	ipglb_system_net_t	mgt_if;
	ipg_status_t		alarm1;
	ipg_status_t		alarm2;
	ipg_u32_control_t	reboot;
#endif
	diagnostics_t		temperature;
	ipglb_system_info_t	info;
	ipg_control32_t	    reset_counters;
	ipg_control32_t	    reset_settings;
#ifdef KURS_PROTOCOL
	ipg_status_t		alarm1;
	ipg_status_t		alarm2;
	ipg_control32_t 	reboot;
//	ipg_status_t		block_color;
	ipg_status_t		block_state;	/*!<can be a value of @ref KURS_block_state_t*/
	alt_u32				sys_uptime;
#endif
}ipglb_card_t;

#endif /* IPGLB_PORT_H_ */
