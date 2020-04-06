/*
 * kurs_protocol.c
 *
 *  Created on: Jun 18, 2019
 *      Author: dcham
 */

#include "../inc/ipglb_prot2_processor.h"
#include "../inc/ipglb_protocol_2.h"
#include "stdlib.h"
#include "string.h"
#include "sys/alt_alarm.h"
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#ifndef NULL
#define NULL	0
#endif

/*
 * Local macros
 */
#define IPGLB2_MAX_BUFFER	256
#define DOUBLE_BUFFER	1

struct IPGLB_PROT2_processor{
	void*				context;
	alt_u8*				buff;
	alt_u32				_cntr;
	uint8_t				expected_len;					/*!<expected length of the frame, this will be used to identify when a full packet has been received*/
	alt_u8 				request_ready;				//!<flag indicating a request on this interface is ready to be processed
	alt_u8 				reply_ready;				//!<flag indicating a request on this interface is ready to be processed
	alt_u8 				buff_1[IPGLB2_MAX_BUFFER];		//!<ARM request buffer
#if DOUBLE_BUFFER
	alt_u8 				buff_2[IPGLB2_MAX_BUFFER];		//!<ARM request buffer
#endif
	alt_u8* buff_copy;					//!<ARM request buffer
	void (*receive)(struct IPGLB_PROT2_processor*, alt_u8);
	IPGLB_PROT2_MODE_t	mode;
};

static void listen_length(IPGLB_PROT2_processor_t* this, alt_u8 c);
static void listen_func(IPGLB_PROT2_processor_t* this, alt_u8 c);
static void listen_cmd(IPGLB_PROT2_processor_t* this, alt_u8 c);
static void listen_reset(IPGLB_PROT2_processor_t* this);
static void listen_payload(IPGLB_PROT2_processor_t* this, alt_u8 c);

/**
 * This function initializes an instance of the KURS protocol listener.
 * @param[in, out] 	this	pointer to the listener object to initialize
 * @param[in, out]	_sop	Start of packet.
 * 							This value is dependent of the BP_ADDR pins.
 * @param[in, out] 	_buff	memory allocated for the listener to store
 * 							KURS characters.
 * 							The calling function is responsible of the allocation.
 * @return	\c RET_SUCCESS if initialization is successful.
 */
IPGLB_PROT2_processor_t* ipglb_prot2_processor_init(struct IPGLB_PROT2_processor_attributes attr)
{
	IPGLB_PROT2_processor_t* _new = malloc(sizeof(struct IPGLB_PROT2_processor));
	if(_new != NULL)
	{
		_new->receive = listen_length;
		_new->context = attr.context;
		_new->mode = attr.mode;
		listen_reset(_new);
		_new->request_ready = 0;
	}
	return (_new);
}

/**
 * This function should be called whenever a KURS character is received.
 * @param[in, out]	this	pointer to the KURS processor.
 */
void ipglb_prot2_receive(IPGLB_PROT2_processor_t *this, alt_u8 data)
{
	if(this != NULL)
	{
		this->receive(this, (alt_u8)data);
	}
}

static uint32_t start_time = 0;
static uint32_t current_time = 0;
/**
 * This function hunts for the KURS SOP character.
 * If in this state, the SOP character is received, it will
 * switch the _handler pointer to @ref hunt_payload.
 * Otherwise, it will start hunting for the EOP and the
 * _handler pointer will point to @ref hunt_eop.
 * param[in, out] 	this	pointer to the protocol handler instance.
 * param[in]		c		the received character
 */
static void listen_length(IPGLB_PROT2_processor_t *this, alt_u8 c)
{
	this->buff[this->_cntr++] = c;
	this->expected_len = c;
	this->receive = listen_func;
	ipglb_prot2_rx_ok_callback(this, c);
	current_time = alt_nticks();
	start_time = current_time;
}

static void listen_func(IPGLB_PROT2_processor_t *this, alt_u8 c)
{
	current_time = alt_nticks();
	if((current_time - start_time) > 10000)
	{
		/*10ms occurred consider it a wrong packet and restart*/
		ipglb_prot2_cleanup(this);
	}else{
		this->buff[this->_cntr++] = c;
		this->receive = listen_cmd;
		ipglb_prot2_rx_ok_callback(this, c);
		start_time = current_time;
	}
}

static void listen_cmd(IPGLB_PROT2_processor_t *this, alt_u8 c){
	this->buff[this->_cntr++] = c;
	c &= 0x7F; //clear the NACK bit
	if(	(c != CMD_WRITE) 	&&
		(c != CMD_READ)		&&
		(c != CMD_CONFIG)){
		/* this is not a valid CMD value
		 * we might have locked into the middle of a packet
		 * we have to restart*/
		ipglb_prot2_cleanup(this);
	}else{
		current_time = alt_nticks();
		if((current_time - start_time) > 10000){
			ipglb_prot2_cleanup(this);
		}else{
			this->receive = listen_payload;
			ipglb_prot2_rx_ok_callback(this, c);
			start_time = current_time;
		}
	}
}

static void listen_reset(IPGLB_PROT2_processor_t *this){
	this->receive = listen_length;
	this->_cntr = 0;
	this->reply_ready = 0;
//	this->request_ready = 0;
	this->expected_len = 0;
	this->buff = this->buff_1;
}

/**
 * This function hunts for the KURS payload.
 * When the EOP character is received, it will
 * switch the handler pointer to @ref hunt_sop
 * param[in, out] 	this	pointer to the protocol handler instance.
 * param[in]		c		the received character
 */
static void listen_payload(IPGLB_PROT2_processor_t *this, alt_u8 c){
	ipglb_prot2_rx_ok_callback(this, c);
	this->buff[this->_cntr++] = c;
//	this->_cntr++;
	if(this->_cntr == this->expected_len){
		this->receive = listen_length;
		if(this->mode == IPGLB_PROT2_MASTER){
			this->reply_ready= 1;
		}else{
			this->request_ready = 1;
		}
		this->buff_copy = this->buff;
#if DOUBLE_BUFFER
		(this->buff == this->buff_1)?(this->buff = this->buff_2):(this->buff = this->buff_1);
#endif
		memset(this->buff, 0, IPGLB2_MAX_BUFFER);
		ipglb_prot2_pkt_ok_callback(this, this->buff);
		this->_cntr = 0;
	}else{
		this->receive = listen_payload;
	}
}

__weak void ipglb_prot2_rx_ok_callback(IPGLB_PROT2_processor_t *this, alt_u8 data){

}

__weak void ipglb_prot2_pkt_ok_callback(IPGLB_PROT2_processor_t *this, alt_u8* data){

}

__weak void ipglb_prot2_send_packet(IPGLB_PROT2_processor_t *this, alt_u8* _reply, alt_u32 reply_len){

}

/**
 * Call this function periodically to service incoming
 * KURS requests
 * @param this	pointer to the KURS processor object.
 */
void ipglb_prot2_service(IPGLB_PROT2_processor_t *this){
	ret_code_t rc = RET_SUCCESS;
	if(this->mode == IPGLB_PROT2_MASTER){
		/* TODO define and implement reply receipt when in MASTER mode */
	}else{
		if(this->request_ready){
			alt_u8* request_buff;
			alt_u8 tx_buff[IPGLB2_MAX_BUFFER];
			alt_u32 tx_len = 0;
			memset(tx_buff, 0, sizeof(tx_buff));
			request_buff = this->buff_copy;
			rc = handle_common(DVT_COM, request_buff, tx_buff, &tx_len);
			if(rc != RET_CRC_MISMATCH){
				ipglb_prot2_send_packet(this, tx_buff, tx_buff[0]);
			}
			/*cleanup*/
			ipglb_prot2_cleanup(this);
		}
	}
}

/**
 * This function returns the context parameter of
 * a KURS processor object (typically the communication device
 * used to receive/transmit KURS characters).
 * @param[in,out] this	pointer to the processor object
 * @return pointer to the context object.
 */
void* ipglb_prot2_processor_get_context(const IPGLB_PROT2_processor_t *this){
	return (this->context);
}

alt_u8 ipglb_prot2_get_reply_ready(IPGLB_PROT2_processor_t* this){
	return (this->reply_ready);
}

void* ipglb_prot2_get_packet(IPGLB_PROT2_processor_t* this){
	return (this->buff_copy);
}

alt_u8 ipglb_prot2_clear_reply_ready(IPGLB_PROT2_processor_t* this){
	return (this->reply_ready);
}

void ipglb_prot2_cleanup(IPGLB_PROT2_processor_t* this){
	this->request_ready = 0;
	listen_reset(this);
}
