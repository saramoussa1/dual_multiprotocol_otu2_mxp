/**
 * kurs_protocol.h
 *
 *  Created on: Jun 18, 2019
 *      Author: dcham
 *      @revision 0.0.1
 */

#ifndef IPGLB_PROT2_PROCESSOR_H_
#define IPGLB_PROT2_PROCESSOR_H_

#include "stdint.h"
#include "../inc/ipg_types.h"

typedef enum{
	IPGLB_PROT2_MASTER = 0,
	IPGLB_PROT2_SLAVE = 1,
}IPGLB_PROT2_MODE_t;

struct IPGLB_PROT2_processor_attributes{
	void* 				context;
	IPGLB_PROT2_MODE_t	mode;	/*!<Mode of operation can be a value of @refIPGLB_PROT2_MODE_t*/
};

typedef struct IPGLB_PROT2_processor IPGLB_PROT2_processor_t;

IPGLB_PROT2_processor_t* ipglb_prot2_processor_init(struct IPGLB_PROT2_processor_attributes attr);
void  ipglb_prot2_receive(IPGLB_PROT2_processor_t *this, alt_u8 data);
void  ipglb_prot2_service(IPGLB_PROT2_processor_t *this);
void  ipglb_prot2_send_packet(IPGLB_PROT2_processor_t *this, alt_u8* _reply, alt_u32 reply_len);
void  ipglb_prot2_rx_ok_callback(IPGLB_PROT2_processor_t *this, alt_u8 data);
void  ipglb_prot2_pkt_ok_callback(IPGLB_PROT2_processor_t *this, alt_u8* data);
void*  ipglb_prot2_processor_get_context(const IPGLB_PROT2_processor_t *this);
alt_u8 ipglb_prot2_get_reply_ready(IPGLB_PROT2_processor_t* this);
alt_u8 ipglb_prot2_clear_reply_ready(IPGLB_PROT2_processor_t* this);
void ipglb_prot2_cleanup(IPGLB_PROT2_processor_t* this);
void* ipglb_prot2_get_packet(IPGLB_PROT2_processor_t* this);
#endif /* IPGLB_PROT2_PROCESSOR_H_ */
