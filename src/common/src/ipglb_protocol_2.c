/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipglb_protocol_2.c
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains procedures used by the IPGLBII protocol.
 * @version 0.0.2
 *
 */
#include "../inc/ipglb_protocol_2.h"
#include "../inc/ipglb_card.h"
#include "../inc/ipglb_transceiver.h"
#include "../inc/ipglb_port.h"
#include "../inc/ipglb_interface.h"
#include "../inc/ipg_pfc.h"
#include "../inc/ipg_constants.h"
#include "../inc/ipg_utils.h"
#include "string.h"
/*
 * local defines
 */
#ifndef NULL
#define NULL	0
#endif

extern ipglb_card_t card;

void assign_function_handlers(void) {
	handlers[0x00] = handle_mmap_func;		//!<handler for function 0x00 (MMAP)
	handlers[0x01] = handle_i2c_func;//!<handler for function 0x01 (I2C communication)
	handlers[0x03] = handle_mdio_func;			//!<handler for function 0x03
	handlers[0x0E] = handle_fw_rev_func;//!<handler for function 0x0E (GET_FW_REV)
	handlers[0x0F] = handle_ping_func;		//!<handler for function 0x0F (PING)
#ifndef KURS_PROTOCOL
	/*space for card specific custom commands*/
	handlers[0x10] = handle_brd_vitals_func;	//!<handler for function 0x10 (FUNC_CARD_VITALS)
	handlers[0x11] = handle_user_led_func;		//!<handler for function FUNC_FP_LEDS (0x11)
	/*space for transceiver specific custom commands*/
	handlers[0x20] = handle_if_transceiver_dynamic_data_func;	//!<handler for function FUNC_IF_TRANS_DYNAMIC_DATA (0x20)
	handlers[0x21] = handle_if_transceiver_info_func;			//!<handler for function FUNC_IF_TRANS_INFO (0x21)
	handlers[0x22] = handle_if_transceiver_txdis_func;			//!<handler for function FUNC_IF_TRANS_TX_DIS (0x22)
	handlers[0x23] = handle_if_transceiver_opt_thresholds; 		//!<handler for function FUNC_IF_TRANS_OPT_PWR_THR (0x23)
	handlers[0x24] = handle_if_transceiver_tx_power_func; 		//!<handler for function FUNC_IF_TRANS_TXPOWER (0x24)
	handlers[0x25] = handle_if_transceiver_freq_func;			//!<handler for function FUNC_IF_TRANS_FREQ (0X25)
	handlers[0x26] = handle_if_transceiver_reboot_func; 		//!<handler for function FUNC_IF_TRANS_REBOOT (0X26)
	/*space for network specific custom commands*/
	handlers[0x30] = NULL;						//!<handler for custom function FUNC_NETWORK_SETTINGS (0x30)
	handlers[0x31] = NULL;						//!<handler for custom function FUNC_SNMP_SETTINGS (0x31)
	/*space for port specific custom commands*/
	handlers[0x40] = handle_if_port_dynamic_data_func; 			//!<handler for function FUNC_IF_PORT_DYNAMIC_DATA (0X40)
	handlers[0x41] = handle_if_port_rate_func; 					//!<handler for function FUNC_IF_PORT_RATE (0X41)
	handlers[0x42] = handle_if_port_lb_func; 					//!<handler for function FUNC_IF_PORT_LB (0X42)
	handlers[0x43] = handle_if_port_fectype_func; 				//!<handler for function FUNC_IF_PORT_FECTYPE (0X43)
	handlers[0x44] = handle_if_port_pfc; 						//!<handler for function FUNC_IF_PORT_PFC (0X44)
	handlers[0x45] = handle_if_port_cntrs; 						//!<handler for function FUNC_IF_PORT_COUNTERS (0X45)
	handlers[0x46] = handle_if_port_info; 						//!<handler for function FUNC_IF_PORT_COUNTERS (0X46)
	handlers[0x47] = handle_if_port_eth_control;				//!<handler for function FUNC_PORT_ETH_CTRL (0X47)

	custom_assign_function_handlers();
#endif
}

/**
 * Project dependent handlers assignments.
 * Implement in main.c if new function handlers are needed.
 */
__weak void custom_assign_function_handlers(void){

}
/**  This is the common piece of the protocol's handling function.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
ret_code_t handle_common(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr){
	ret_code_t ret = RET_SUCCESS;
	if (!uart_crc_ok(request)){
		// the request is not good, fail
		return 		(RET_CRC_MISMATCH);
	}
	// start building the reply in the same time as we process the request
	reply[(*reply_ptr)++] = 0; // this is the length field, filled  with 0 now, should be updated later
	reply[(*reply_ptr)++] = request[UART_PACKET_FUNCTION_INDEX]; // send back the same function
	switch (request[UART_PACKET_COMMAND_INDEX] & 0x7F /*ignore NACK/ACK in requests*/){
	   case CMD_READ:
		   reply[(*reply_ptr)++] = CMD_WRITE;
		   break;
	   case CMD_WRITE:
		   reply[(*reply_ptr)++] = CMD_READ;
		   break;
	   case CMD_CONFIG:
		   reply[(*reply_ptr)++] = CMD_CONFIG;
		   break;
	}
	// now handle the function and command
	//	make sure the requested FUNC exists and that a handler has been assigned to it
	if(handlers[(int)request[UART_PACKET_FUNCTION_INDEX]] != NULL){
		ret = handlers[(int)request[UART_PACKET_FUNCTION_INDEX]](COM, request, reply, reply_ptr);
	}else{
		ret = RET_FAIL;
	}
	if(ret != RET_SUCCESS){
		// NACK the request
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
	}
	//finally calculate the CRC
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return 	(ret);
}

/** This function calculates the CRC of a packet.
 * @param[in,out] buff the packet we need to calculate its CRC
 * @return the calculated CRC
 */
alt_u8 uart_calculate_crc(alt_u8 *buff){
	alt_u8 crc = 0, length = 0;
	int i = 0;
	length = buff[0];
	for (i = 0; i < (length - 1); i++) crc ^= buff[i];
	return 	(crc);
}

/** Checks if the calculated CRC is the correct CRC available in the packet.
 * @param[in,out] buff  the packet we need to check its CRC
 * @return \c  1 if CRC check passed \c 0 if it failed
 */
alt_u8 uart_crc_ok(alt_u8 *buff){
	alt_u8 crc = 0, len = 0;
	len = buff[UART_PACKET_LENGTH_INDEX];
	crc = uart_calculate_crc(buff);
	if (crc == buff[len - 1]) return 	(1);
	else return 	(0);
}

#ifndef KURS_PROTOCOL
/**This function handles requests to read a transceiver's static information.
 * The data includes the transceiver's ID, capabilities and TX/RX tuning values if available (0 if not supported).
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak ret_code_t handle_if_transceiver_info_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t ret = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		ret = RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
		reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];

		memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1], card.interfaces[if_index].transceiver.info.ID, TRANSCEIVER_ID_LEN);
		memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + TRANSCEIVER_ID_LEN], &card.interfaces[if_index].transceiver.info.capabilities, sizeof(alt_u32));
		memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + TRANSCEIVER_ID_LEN + 4], &card.interfaces[if_index].transceiver.info.tuning, sizeof(tuning_info_t));
		memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + TRANSCEIVER_ID_LEN + 4 + sizeof(tuning_info_t)], &card.interfaces[if_index].transceiver.info.tuning, sizeof(tuning_info_t));
		//TX tuning is similar to RX tuning that's why i am sending the same parameter twice
	}

	(*reply_ptr) += 114; /*(1 <index> + 4 <capabilities> + 61 <ID> + 12 <TX tunability> + 12 <RX tunability>)*/
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (ret);
}

/**
 * This function handles requests to modify PFC monitoring period in seconds.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak  ret_code_t handle_if_port_pfc(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr){
	ret_code_t rc = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =  RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	if(rc == RET_SUCCESS){
		if(request[UART_PACKET_PARAMS_INDEX] == CMD_WRITE){
			alt_u32 period = 0;
			memcpy(&period, &request[UART_PACKET_PARAMS_INDEX + 1], 4);
			set_pfc_monitor_period(card.interfaces[if_index].port.pfc, period);

			reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];
		}
	}
	(*reply_ptr) += 1;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

/**This function handles requests to reset counters of a certain port.
 * @param[in] COM pointer to the COM interface
 * The \c next and \c change values of the \c clear field will be updated.
 * The actual operation to clear the counters will be executed in the main loop.
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak ret_code_t handle_if_port_cntrs(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t rc = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =  RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	if(rc == RET_SUCCESS){
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			card.interfaces[if_index].port.control.reset_counters.change = 1;
			// send back some of the received values to the host
			reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];
		}else{
			rc = RET_CMD_MISMATCH;
		}
	}
	(*reply_ptr) += 1;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}


/**Generic handler to read/modify a transceiver's frequencies.
 * The \c next and \c change values of the \c om.lb_enable clear field will be updated.
 * The actual operation to change between loopback and normal modes will be executed in the main loop.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak ret_code_t handle_if_transceiver_freq_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	int if_index = 0;
	ret_code_t rc = RET_SUCCESS;
	if_index = request[UART_PACKET_PARAMS_INDEX];
	// validate args
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =  RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	if(rc == RET_SUCCESS){
#define TRANS_DYNAMIC_DATA	0
#define TRANS_STATIC_DATA	1
		if(request[UART_PACKET_COMMAND_INDEX] == CMD_READ){
			if(request[UART_PACKET_PARAMS_INDEX+1] == TRANS_DYNAMIC_DATA){
				if(!card.interfaces[if_index].transceiver.info.capabilities.tx_tunable && !card.interfaces[if_index].transceiver.info.capabilities.rx_tunable){
					rc = RET_FAIL;
				}else{
					memcpy(&reply[UART_PACKET_PARAMS_INDEX + 2], &card.interfaces[if_index].transceiver.control.tx_freq.current, sizeof(double));
					memcpy(&reply[UART_PACKET_PARAMS_INDEX + 10], &card.interfaces[if_index].transceiver.control.rx_freq.current, sizeof(double));
					memcpy(&reply[UART_PACKET_PARAMS_INDEX + 18], &card.interfaces[if_index].transceiver.monitor.tx_freq, sizeof(double));
					memcpy(&reply[UART_PACKET_PARAMS_INDEX + 26], &card.interfaces[if_index].transceiver.monitor.rx_freq, sizeof(double));
				}
			}else{//STATIC DATA
				if(!card.interfaces[if_index].transceiver.info.capabilities.tx_tunable && !card.interfaces[if_index].transceiver.info.capabilities.rx_tunable){
					rc = RET_FAIL;
				}else{
					memcpy(&reply[UART_PACKET_PARAMS_INDEX + 2], &card.interfaces[if_index].transceiver.info.tuning.first_freq, sizeof(double));
					memcpy(&reply[UART_PACKET_PARAMS_INDEX + 10], &card.interfaces[if_index].transceiver.info.tuning.last_freq, sizeof(double));
					memcpy(&reply[UART_PACKET_PARAMS_INDEX + 18], &card.interfaces[if_index].transceiver.info.tuning.grid_spacing, sizeof(double));}
				}
		}else{//Write command
			double desired_tx_freq, desired_rx_freq = 0;
			memcpy(&desired_tx_freq, &request[UART_PACKET_PARAMS_INDEX + 2], sizeof(double));
			memcpy(&desired_rx_freq, &request[UART_PACKET_PARAMS_INDEX + 10], sizeof(double));
			if(request[UART_PACKET_PARAMS_INDEX+1] & BIT0){
				if(!card.interfaces[if_index].transceiver.info.capabilities.tx_tunable){
					rc = RET_FAIL;
				}else{
					if((double_compare_lt(desired_tx_freq, card.interfaces[if_index].transceiver.info.tuning.first_freq,card.interfaces[if_index].transceiver.info.tuning.grid_spacing*0.1)) ||
						(double_compare_gt(desired_tx_freq,card.interfaces[if_index].transceiver.info.tuning.last_freq, card.interfaces[if_index].transceiver.info.tuning.grid_spacing*0.1))){
						rc = RET_FAIL;
					}else{
						memcpy(&card.interfaces[if_index].transceiver.control.tx_freq.next, &request[UART_PACKET_PARAMS_INDEX+2], sizeof(double));
						card.interfaces[if_index].transceiver.control.tx_freq.change = 1;
					}
				}
			}
			if(request[UART_PACKET_PARAMS_INDEX+1] & BIT1){
				if(!card.interfaces[if_index].transceiver.info.capabilities.rx_tunable){
					rc = RET_FAIL;
				}else{
					if((double_compare_lt(desired_rx_freq, card.interfaces[if_index].transceiver.info.tuning.first_freq, card.interfaces[if_index].transceiver.info.tuning.grid_spacing*0.1)) ||
						(double_compare_gt(desired_rx_freq,card.interfaces[if_index].transceiver.info.tuning.last_freq,card.interfaces[if_index].transceiver.info.tuning.grid_spacing*0.1))){
						rc = RET_FAIL;
					}else{
						memcpy(&card.interfaces[if_index].transceiver.control.rx_freq.next, &request[UART_PACKET_PARAMS_INDEX+10], sizeof(double));
						card.interfaces[if_index].transceiver.control.rx_freq.change = 1;
					}
				}
			}
		}
		// send back some of the received values to the host
		memcpy(&reply[UART_PACKET_PARAMS_INDEX], &request[UART_PACKET_PARAMS_INDEX], 2);
	}
	(*reply_ptr) += 34;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}


/**This function handles user command to enable/disable a transceiver's transmitter.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak ret_code_t handle_if_transceiver_txdis_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t rc = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc = RET_FAIL;
	}
	else
	{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;

		reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX]; //send back the index
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_READ){
			reply[UART_PACKET_PARAMS_INDEX + 1] = get_if_transceiver_txdisable(&card.interfaces[if_index].transceiver);

		}else if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			card.interfaces[if_index].transceiver.control.tx_disable.next = request[UART_PACKET_PARAMS_INDEX + 1] & 0x01;
			card.interfaces[if_index].transceiver.control.tx_disable.change = 1;
			reply[UART_PACKET_PARAMS_INDEX + 1] = request[UART_PACKET_PARAMS_INDEX + 1];
		}
	}
	(*reply_ptr) += 2;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

__weak ret_code_t handle_brd_vitals_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr){
	/*
	 * Do not modify, if needed implement in main application
	 */
	return RET_FAIL;
}

__weak ret_code_t handle_user_led_func(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr) {
	/*
	 * Do not modify, if needed implement in main application
	 */
	return RET_FAIL;
}
/**
 * Generic handler for request to read a port dynamic data.
 * Application independent.
 * @param COM		communication interface
 * @param request	pointer to the incoming request
 * @param reply		pointer to the outgoing reply
 * @param reply_ptr	pointer to the reply's length
 * @return	\c RET_SUCCESS on success
 */
__weak ret_code_t handle_if_port_info(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t rc = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc = RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	int ptr = UART_PACKET_PARAMS_INDEX;
	if(rc == RET_SUCCESS){
		reply[ptr] = request[UART_PACKET_PARAMS_INDEX];
		ptr +=1;
		/*port stats*/
		memcpy(&reply[ptr], &card.interfaces[if_index].port.info.capabilities, sizeof(port_capabilities_t));
		ptr += sizeof(port_capabilities_t);
	}
	(*reply_ptr) += ptr - 3;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

__weak ret_code_t handle_if_port_dynamic_data_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t rc = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc = RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	int ptr = UART_PACKET_PARAMS_INDEX;
	if(rc == RET_SUCCESS){
		reply[ptr] = request[UART_PACKET_PARAMS_INDEX];
		ptr +=1;
		reply[ptr] = card.interfaces[if_index].port.monitor.rate;
		ptr += 1;
		/*port stats*/
		memcpy(&reply[ptr], &card.interfaces[if_index].port.monitor.statistics, sizeof(card.interfaces[if_index].port.monitor.statistics));
		ptr += sizeof( card.interfaces[if_index].port.monitor.statistics);
		/*fec statistics*/
		if(card.interfaces[if_index].port.info.capabilities.FEC_CAPABLE){
			memcpy(&reply[ptr], &card.interfaces[if_index].port.monitor.fec_statistics, sizeof(fec_statistics_t));
			ptr += sizeof(fec_statistics_t);
		}
		/*pfc statistics*/
		float t[3];
		alt_u32 d = 0;
		if(card.interfaces[if_index].port.info.capabilities.PFC_CAPABLE){
			if(card.interfaces[if_index].port.pfc != NULL){
				d = get_pfc_uas(card.interfaces[if_index].port.pfc);
				t[0] = get_pfc_bber(card.interfaces[if_index].port.pfc);
				t[1] = get_pfc_esr(card.interfaces[if_index].port.pfc);
				t[2] = get_pfc_sesr(card.interfaces[if_index].port.pfc);
			}
			memcpy(&reply[ptr], &d, sizeof(alt_u32));
			ptr += sizeof(alt_u32);
			memcpy(&reply[ptr], &t[0], sizeof(float));
			ptr+=sizeof(float);
			memcpy(&reply[ptr], &t[1], sizeof(float));
			ptr+=sizeof(float);
			memcpy(&reply[ptr], &t[2], sizeof(float));
			ptr+=sizeof(float);

		}
	}
	(*reply_ptr) += ptr - 3;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

/**
 * This function handles requests to read/write an ETHERNET port's controls.
 * @param COM
 * @param request
 * @param reply
 * @param reply_ptr
 * @return
 */
__weak ret_code_t handle_if_port_eth_control(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr) {

	int if_index = request[UART_PACKET_PARAMS_INDEX];
	ret_code_t rc = RET_SUCCESS;
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =  RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	if(rc == RET_SUCCESS){
		reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_READ){
			memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1],  &card.interfaces[if_index].port.control.protocol_control.eth_ctrl, sizeof(protocol_eth_control_t));
			(*reply_ptr) += sizeof(protocol_eth_control_t);
		}else if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			protocol_eth_control_t temp_ctrl;
			memcpy(&temp_ctrl, &request[UART_PACKET_PARAMS_INDEX + 1], sizeof(protocol_eth_control_t));
			if(temp_ctrl.llcf_enable.current != card.interfaces[if_index].port.control.protocol_control.eth_ctrl.llcf_enable.current){
				memcpy(&card.interfaces[if_index].port.control.protocol_control.eth_ctrl, &request[UART_PACKET_PARAMS_INDEX + 1], sizeof(protocol_eth_control_t));
				card.interfaces[if_index].port.control.protocol_control.eth_ctrl.llcf_enable.change = 1;
			}
		}else{
			rc = RET_CMD_MISMATCH;
		}
	}
	(*reply_ptr) += 1; /*for the byte consumed by the index*/
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);

}

/**
 * This function handles requests to read/write a port's FEC settings.
 * @param COM
 * @param request
 * @param reply
 * @param reply_ptr
 * @return
 */
__weak ret_code_t handle_if_port_fectype_func(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr) {
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	ret_code_t rc = RET_SUCCESS;
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =  RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	if(rc == RET_SUCCESS){
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_READ){
			reply[UART_PACKET_PARAMS_INDEX + 1] = card.interfaces[if_index].port.monitor.fec_statistics.type;
			reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];
		}else if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			// TODO: validate requested FEC type
			if(card.interfaces[if_index].port.info.capabilities.FEC_CAPABLE){
				card.interfaces[if_index].port.control.fec_control.type.next = request[UART_PACKET_PARAMS_INDEX + 1];

				card.interfaces[if_index].port.control.fec_control.type.change = (card.interfaces[if_index].port.control.fec_control.type.next != card.interfaces[if_index].port.monitor.fec_statistics.type);
				// send back some of the received values to the host
			}else{
				rc = RET_INVALID_PARAMS;
			}
			memcpy(&reply[UART_PACKET_PARAMS_INDEX], &request[UART_PACKET_PARAMS_INDEX], 2);
		}else{
			rc = RET_CMD_MISMATCH;
		}
	}
	(*reply_ptr) += 2;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

/**Generic handler to modify an interface's loopback.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak ret_code_t handle_if_port_lb_func(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr) {
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	ret_code_t rc = RET_SUCCESS;
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =  RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	if(rc == RET_SUCCESS){
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_READ){
			reply[UART_PACKET_PARAMS_INDEX + 1] = card.interfaces[if_index].port.control.loopback_mode.current;
			reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];
		}else if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			// TODO: validate requested loopback mode
			card.interfaces[if_index].port.control.loopback_mode.next = request[UART_PACKET_PARAMS_INDEX + 1];
			card.interfaces[if_index].port.control.loopback_mode.change = 1;
			// send back some of the received values to the host
			memcpy(&reply[UART_PACKET_PARAMS_INDEX], &request[UART_PACKET_PARAMS_INDEX], 2);
		}else{
			rc = RET_CMD_MISMATCH;
		}
	}
	(*reply_ptr) += 2;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}
#endif

/**Generic handler for requests to execute memory register operations.
 * Based on the \a command field of the request, it dispatches the appropriate helper function.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
ret_code_t handle_mmap_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	// get the base and offset
	alt_u32 base = 0, offset = 0, value = 0;
	alt_u16 msb = 0, lsb = 0;

	msb = ((alt_u8) request[6] << 8);
	msb = msb | ((alt_u8) request[5]);
	lsb = ((alt_u8) request[4] << 8);
	lsb = lsb | ((alt_u8) request[3]);
	base = (((alt_u32) msb) << 16) | ((alt_u32) lsb);

	msb = lsb = 0;
	msb = ((alt_u8) request[10] << 8);
	msb = msb | (alt_u8) (request[9]);
	lsb = ((alt_u8) request[8] << 8);
	lsb = lsb | ((alt_u8) request[7]);
	offset = (((alt_u32) msb) << 16) | ((alt_u32) lsb);

	memcpy(&reply[(*reply_ptr)], &request[UART_PACKET_PARAMS_INDEX], 8);

	(*reply_ptr) += 8; // add in the base and offset count
	if(request[UART_PACKET_COMMAND_INDEX] == CMD_READ){
		handle_mem_rd(base, offset, &value);
		reply[(*reply_ptr)++] = value & 0xFF;
		reply[(*reply_ptr)++] = (value >> 8) & 0xFF;
		reply[(*reply_ptr)++] = (value >> 16) & 0xFF;
		reply[(*reply_ptr)++] = (value >> 24) & 0xFF;
	}else if(request[UART_PACKET_COMMAND_INDEX] == CMD_WRITE){
		msb = lsb = 0;
		lsb = lsb | (alt_u8) request[11];
		lsb = lsb | ((alt_u8) request[12] << 8);
		msb = msb | (alt_u8) request[13];
		msb = msb | ((alt_u8) request[14] << 8);
		value = (((alt_u32) msb) << 16) | ((alt_u32) lsb);
		handle_mem_wr(base, offset, value);
	}
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (RET_SUCCESS);
}

__weak ret_code_t handle_mem_rd(alt_u32 base, alt_u32 offset, alt_u32* value) {
	*value = 0;
	return RET_SUCCESS;
}

__weak ret_code_t handle_mem_wr(alt_u32 base, alt_u32 offset, alt_u32 value){
	return RET_FAIL;
}

/**Generic handler for requests to execute I2C operations.
 * Based on the \a command field of the request, it dispatches the appropriate helper function.
 * @param[in] COM pointer to the COM interface
 * * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
ret_code_t handle_i2c_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	// get the base and offset
	ret_code_t ret = RET_SUCCESS;
	alt_u8 i2c_device_id = 0;
	i2c_device_id = request[UART_PACKET_PARAMS_INDEX];
	memcpy(&reply[(*reply_ptr)], &request[UART_PACKET_PARAMS_INDEX], 1);
	(*reply_ptr) += 1; //!< add in the arguments count
	if(request[UART_PACKET_COMMAND_INDEX] == CMD_READ){
		alt_u8 i2c_value = 0;
		ret = handle_i2c_rd(i2c_device_id, request, reply, reply_ptr, &i2c_value);
		reply[(*reply_ptr)++] = i2c_value;
	}else if(request[UART_PACKET_COMMAND_INDEX] == CMD_WRITE){
		ret = handle_i2c_wr(i2c_device_id, request, reply, reply_ptr);
	}else if(request[UART_PACKET_COMMAND_INDEX] == CMD_CONFIG){
		ret = handle_user_i2c_config(i2c_device_id, request, reply, reply_ptr);
	}
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; //!< update the length field
	return (ret);
}

__weak ret_code_t handle_i2c_wr(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply, int* reply_ptr){
	/*
	 * Do not modify here
	 */
	return RET_FAIL;
}

__weak ret_code_t handle_i2c_rd(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply, int* reply_ptr, alt_u8 *i2c_value) {
	/*
	 * Do not modify here
	 */
	return RET_FAIL;
}

__weak ret_code_t handle_user_i2c_config(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply, int* reply_ptr){
	/*
	 * Do not modify here
	 */
	return RET_FAIL;
}
/**This function handles requests to read the NiOS processor firmware revision string.
 * The firmware revision string is 32 bytes long.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
ret_code_t handle_fw_rev_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	memcpy(&reply[(*reply_ptr)], card.info.sw_number, SW_NUM_LEN);
	(*reply_ptr) += 32; // size of the
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (RET_SUCCESS);
}

/**Generic handler of requests to ping the NiOS processor.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
ret_code_t handle_ping_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (RET_SUCCESS);
}

#ifndef KURS_PROTOCOL
/**This function handles users' requests to read clients' dynamic data.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak ret_code_t handle_if_transceiver_dynamic_data_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t rc = RET_SUCCESS;
	int cnt = 0;
	alt_u8 if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index > INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc = (RET_INVALID_PARAMS);
	}else{
		if(card.interfaces[if_index].transceiver.busy){
			reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
			rc = RET_BUSY;
		}else{
			reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
			// index (1 byte)
			reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];
			cnt++;
			// status (1 byte)
			reply[UART_PACKET_PARAMS_INDEX + cnt] = 	card.interfaces[if_index].transceiver.monitor.state.current;
			cnt++;
			// alarms (16-bit)
			alt_u16 alarms = 0;
			(card.interfaces[if_index].transceiver.monitor.los.current)?(SET_BIT(alarms, BIT0)):(CLEAR_BIT(alarms, BIT0));
			memcpy(&reply[UART_PACKET_PARAMS_INDEX + cnt], &alarms, sizeof(alt_u16));
			cnt += sizeof(alt_u16);
			// temperature
			memcpy(&reply[UART_PACKET_PARAMS_INDEX + cnt], &card.interfaces[if_index].transceiver.monitor.temperature.value, sizeof(float));
			cnt += sizeof(float);

			int i = 0;
			for(i = 0; i < card.interfaces[if_index].transceiver.info.capabilities.optical_ios_number; i++){
				// get the TX optical power for each channel
				memcpy(&reply[UART_PACKET_PARAMS_INDEX + cnt], &card.interfaces[if_index].transceiver.monitor.output_power[i].value, sizeof(float));
				cnt += sizeof(float);
				// get the RX optical power for each channel
				memcpy(&reply[UART_PACKET_PARAMS_INDEX + cnt], &card.interfaces[if_index].transceiver.monitor.input_power[i].value, sizeof(float));
				cnt += sizeof(float);
			}
		}
	}

	(*reply_ptr) += 12+2*sizeof(float)*card.interfaces[if_index].transceiver.info.capabilities.optical_ios_number;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

__weak ret_code_t handle_if_transceiver_opt_thresholds(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr){
	ret_code_t rc = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_READ){
			reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX];
		}else{
			rc = RET_FAIL;
			reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		}
	}

	// get the TX optical power for each channel
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1	], &card.interfaces[if_index].transceiver.monitor.output_power[0].critical_min, sizeof(float));
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + 1*  sizeof(float)], &card.interfaces[if_index].transceiver.monitor.output_power[0].warning_min, sizeof(float));
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + 2*  sizeof(float)], &card.interfaces[if_index].transceiver.monitor.output_power[0].warning_max, sizeof(float));
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + 3*  sizeof(float)], &card.interfaces[if_index].transceiver.monitor.output_power[0].critical_max, sizeof(float));

	// get the RX optical power for each channel
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + 4*	 sizeof(float)], &card.interfaces[if_index].transceiver.monitor.input_power[0].critical_min, sizeof(float));
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + 5*  sizeof(float)], &card.interfaces[if_index].transceiver.monitor.input_power[0].warning_min, sizeof(float));
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + 6*  sizeof(float)], &card.interfaces[if_index].transceiver.monitor.input_power[0].warning_max, sizeof(float));
	memcpy(&reply[UART_PACKET_PARAMS_INDEX + 1 + 7*  sizeof(float)], &card.interfaces[if_index].transceiver.monitor.input_power[0].critical_max, sizeof(float));

	(*reply_ptr) += 33; /*1 index + every channel has 4 float thresholds for TX and 4 others for RX*/
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

__weak ret_code_t handle_if_transceiver_tx_power_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t rc = RET_SUCCESS;
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc =RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			float tx_power = 0;
			memcpy(&tx_power, &request[UART_PACKET_PARAMS_INDEX+1], sizeof(float)); /*change the TX output power*/

			// send back the parameters for confirmation
			memcpy(&reply[UART_PACKET_PARAMS_INDEX], &request[UART_PACKET_PARAMS_INDEX], 1+sizeof(float));

			card.interfaces[if_index].transceiver.control.tx_power.next = tx_power;
			card.interfaces[if_index].transceiver.control.tx_power.change = 1;
		}
	}
	(*reply_ptr) += 5; /*1 index + 4 tx power (float)*/
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

/**This function handles user command to reboot OM.
 * @param[in] COM pointer to the COM interface
 * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
__weak ret_code_t handle_if_transceiver_reboot_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	ret_code_t rc= RET_SUCCESS;
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc = RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
		reply[UART_PACKET_PARAMS_INDEX] = request[UART_PACKET_PARAMS_INDEX]; //send back the index
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_READ){
			(card.interfaces[if_index].transceiver.monitor.state.current == TRANSCEIVER_STATE_REBOOTING)? (reply[UART_PACKET_PARAMS_INDEX + 1] = 1) : (reply[UART_PACKET_PARAMS_INDEX + 1] = 0);
		}else if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			if(request[UART_PACKET_PARAMS_INDEX + 1]){
				transceiver_state_machine_t* sm = (transceiver_state_machine_t*)card.interfaces[if_index].transceiver.additional;
				sm->input_boot_start = 1;
				sm->state_handler = run_transceiver_booting_state;
			}
			reply[UART_PACKET_PARAMS_INDEX + 1] = request[UART_PACKET_PARAMS_INDEX + 1];
		}
	}
	(*reply_ptr) += 2;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}

/**
 * Generic handler for request to read/write a port's rate.
 * Application independent.
 * @param COM		communication interface
 * @param request	pointer to the incoming request
 * @param reply		pointer to the outgoing reply
 * @param reply_ptr	pointer to the reply's length
 * @return	\c RET_SUCCESS on success
 */
__weak ret_code_t handle_if_port_rate_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	int if_index = request[UART_PACKET_PARAMS_INDEX];
	ret_code_t rc = RET_SUCCESS;
	if(if_index >= INTERFACE_COUNT){
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_NACK;
		rc = RET_FAIL;
	}else{
		reply[UART_PACKET_COMMAND_INDEX] |= OPCODE_ACK;
	}
	if(rc == RET_SUCCESS){
		if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_READ){
			reply[UART_PACKET_PARAMS_INDEX] 		= request[UART_PACKET_PARAMS_INDEX];
			reply[UART_PACKET_PARAMS_INDEX + 1] 	= card.interfaces[if_index].port.monitor.rate;
			reply[UART_PACKET_PARAMS_INDEX + 2] 	= card.interfaces[if_index].port.monitor.prot;
		}else if((request[UART_PACKET_COMMAND_INDEX] & 0x7F) == CMD_WRITE){
			card.interfaces[if_index].port.control.rate.next = request[UART_PACKET_PARAMS_INDEX + 1];
			card.interfaces[if_index].port.control.rate.change = 1;
			// TODO: validate port value
			memcpy(&reply[UART_PACKET_PARAMS_INDEX], &request[UART_PACKET_PARAMS_INDEX], 2);
		}
	}
	(*reply_ptr) += 3;
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	reply[(*reply_ptr)] = uart_calculate_crc(reply);
	return (rc);
}
#endif
/**Generic handler requests to execute MDIO operations.
 * Based on the \a command field of the request, it dispatches the appropriate helper function.
 * @param[in] COM pointer to the COM interface
 * * @param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2 protocol definitions.
 * \note 	The function, command and arguments used in this transaction is defined in the
 * 			software design document of this application.
 */
ret_code_t handle_mdio_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) {
	ret_code_t ret = RET_SUCCESS;
	alt_u8 mdio_device_id = 0, mdio_device_index = 0, phy_address = 0, device_address = 0;
	alt_u16 mdio_data = 0;
	alt_u16 mdio_register_address = 0 ;

	mdio_device_id = request[UART_PACKET_PARAMS_INDEX];
	mdio_device_index = request[UART_PACKET_PARAMS_INDEX + 1];
	mdio_register_address = (request[UART_PACKET_PARAMS_INDEX+3] << 8) |  request[UART_PACKET_PARAMS_INDEX+2];
	device_address = request[UART_PACKET_PARAMS_INDEX + 4];
	phy_address = request[UART_PACKET_PARAMS_INDEX + 5];

	memcpy(&reply[(*reply_ptr)], &request[UART_PACKET_PARAMS_INDEX], 6);
	(*reply_ptr) += 6; // add in the arguments count
	if (request[UART_PACKET_COMMAND_INDEX] == CMD_READ){
		ret = handle_mdio_rd(mdio_device_id, mdio_device_index, phy_address,device_address, mdio_register_address, &mdio_data);
		reply[(*reply_ptr)++] = mdio_data;
		reply[(*reply_ptr)++] = mdio_data >> 8;
	}
	else if (request[UART_PACKET_COMMAND_INDEX] == CMD_WRITE){
		mdio_data = request[UART_PACKET_PARAMS_INDEX + 6];
		mdio_data |= (request[UART_PACKET_PARAMS_INDEX + 7] << 8);
		ret = handle_mdio_wr(mdio_device_id, mdio_device_index, phy_address,device_address, mdio_register_address, mdio_data);
	}
	reply[UART_PACKET_LENGTH_INDEX] = (*reply_ptr) + 1; // update the length field
	return ret;
}

__weak ret_code_t handle_mdio_rd(alt_u8 mdio_device_id, alt_u8 mdio_device_index, alt_u8 phy_address, alt_u8 device_address, alt_u16 offset, alt_u16 *value){

	/*
	 * Do not modify, if needed implement in main application
	 */
	return RET_FAIL;
}

__weak ret_code_t handle_mdio_wr(alt_u8 mdio_device_id, alt_u8 mdio_device_index, alt_u8 phy_address, alt_u8 device_address, alt_u16 offset, alt_u16 value){

	/*
	 * Do not modify, if needed implement in main application
	 */
	return RET_FAIL;
}
