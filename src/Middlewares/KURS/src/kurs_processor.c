/*
 * kurs_protocol.c
 *
 *  Created on: Jun 18, 2019
 *      Author: dcham
 */

#include "../inc/kurs_processor.h"
#include "stdlib.h"
#include "string.h"
#include "sys/alt_irq.h"
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#ifndef NULL
#define NULL	0
#endif

/*
 * Local macros
 */
#define KURS_MAX_BUFFER	1024
#define DOUBLE_BUFFER	1

typedef struct KURS_processor{
	void*				context;
	alt_u32				SOP;
	alt_u8*				buff;
	alt_u32				_cntr;
	alt_u8 				request_ready;				//!<flag indicating a request on this interface is ready to be processed
	alt_u8 				buff_1[KURS_MAX_BUFFER];	//!<ARM request buffer
#if DOUBLE_BUFFER
	alt_u8 				buff_2[KURS_MAX_BUFFER];	//!<ARM request buffer
#endif
	alt_u8* 			buff_copy;					//!<ARM request buffer
	void (*receive)(struct KURS_processor*, alt_u8);
};

static void listen_eop(KURS_processor_t* this, alt_u8 c);
static void listen_sop(KURS_processor_t* this, alt_u8 c);
static void listen_payload(KURS_processor_t* this, alt_u8 c);

#define KURS_EOP				0x0D

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
KURS_processor_t* kurs_processor_init(struct KURS_processor_attributes attr){
	KURS_processor_t* _new = malloc(sizeof(struct KURS_processor));
	if(_new != NULL){
		_new->receive = listen_eop;
		_new->SOP = attr.sop;
		_new->context = attr.context;
		_new->request_ready = 0;
		_new->buff = _new->buff_1;
		_new->_cntr = 0;
	}
	return (_new);
}


ret_code_t kurs_processor_set_sop(KURS_processor_t* this, alt_u8 _sop){
	this->SOP = _sop;
	return (RET_SUCCESS);
}

/**
 * This function should be called whenever a KURS character is received.
 * @param[in, out]	this	pointer to the KURS processor.
 */
void kurs_receive(KURS_processor_t *this, alt_u8 data){
	alt_irq_context _cntx = alt_irq_disable_all();
	if(this != NULL){
		this->receive(this, (alt_u8)data);
	}
	alt_irq_enable_all(_cntx);
}

/**
 * This function listens for the KURS EOP.
 * When the EOP character is received, it will
 * switch the _handler pointer to @ref hunt_sop
 * otherwise it will stay in the current state.
 * param[in, out] 	this	pointer to the protocol handler instance.
 * param[in]		c		the received character
 */
static void listen_eop(KURS_processor_t *this, alt_u8 c){
	if(c == KURS_EOP){
		this->receive = listen_sop;
	}else{
		this->receive = listen_eop;
	}
}

/**
 * This function hunts for the KURS SOP character.
 * If in this state, the SOP character is received, it will
 * switch the _handler pointer to @ref hunt_payload.
 * Otherwise, it will start hunting for the EOP and the
 * _handler pointer will point to @ref hunt_eop.
 * param[in, out] 	this	pointer to the protocol handler instance.
 * param[in]		c		the received character
 */
static void listen_sop(KURS_processor_t *this, alt_u8 c){
	if(c == this->SOP){
		this->receive = listen_payload;
		this->buff[this->_cntr++] = c;
	}else{
		this->receive = listen_eop;
		this->_cntr = 0;
	}
}

/**
 * This function hunts for the KURS payload.
 * When the EOP character is received, it will
 * switch the handler pointer to @ref hunt_sop
 * param[in, out] 	this	pointer to the protocol handler instance.
 * param[in]		c		the received character
 */
static void listen_payload(KURS_processor_t *this, alt_u8 c){
	this->buff[this->_cntr++] = c;
	if(c == KURS_EOP){
		this->buff[this->_cntr] = 0;
		this->receive = listen_sop;
		this->request_ready = 1;
		this->_cntr = 0;
		this->buff_copy = this->buff;
#if DOUBLE_BUFFER
		(this->buff == this->buff_1)?(this->buff = this->buff_2):(this->buff = this->buff_1);
#endif
		memset(this->buff, 0, KURS_MAX_BUFFER);
	}else{
		this->receive = listen_payload;
	}
}

__weak void kurs_rx_ok_callback(KURS_processor_t *this, alt_u8 data){

}

__weak void kurs_pkt_ok_callback(KURS_processor_t *this, alt_u8* data){

}

__weak void kurs_send_packet(KURS_processor_t *this, alt_u8* _reply, alt_u32 reply_len){

}

/**
 * Call this function periodically to service incoming
 * KURS requests
 * @param this	pointer to the KURS processor object.
 */
void kurs_service(KURS_processor_t *this){
	if(this->request_ready){
		uint8_t* request_buff;
		uint8_t tx_buff[2048];
		uint32_t tx_len = 0;
		memset(tx_buff, 0, sizeof(tx_buff));
		request_buff = this->buff_copy;
		//		  printf("@%d - Request %.10s \n", systicks, request_buff);
		KURS_HandleRequests(request_buff, tx_buff, &tx_len);
		tx_buff[tx_len] = 0;
		//		  printf("@%d - Reply(%d) %s \n", systicks, tx_len, tx_buff);
		kurs_send_packet(this, tx_buff, tx_len);
		// cleanup
		this->request_ready = 0;
	}
}

/**
 * This function returns the context parameter of
 * a KURS processor object (typically the communication device
 * used to receive/transmit KURS characters).
 * @param[in,out] this	pointer to the processor object
 * @return pointer to the context object.
 */
void* kurs_processor_get_context(const KURS_processor_t *this){
	return (this->context);
}
