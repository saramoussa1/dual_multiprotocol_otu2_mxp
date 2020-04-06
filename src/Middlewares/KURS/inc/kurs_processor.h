/*
 * kurs_protocol.h
 *
 *  Created on: Jun 18, 2019
 *      Author: dcham
 */

#ifndef KURS_LISTENER_H_
#define KURS_LISTENER_H_

#include "stdint.h"
#include "../../../common/inc/ipg_types.h"

struct KURS_processor_attributes{
	void* 	context;
	alt_u8	sop;
};
typedef struct KURS_processor KURS_processor_t;

KURS_processor_t* kurs_processor_init(struct KURS_processor_attributes attr);
ret_code_t kurs_processor_set_sop(KURS_processor_t* this, alt_u8 _sop);
void kurs_receive(KURS_processor_t *this, alt_u8 data);
void kurs_service(KURS_processor_t *this);
void kurs_send_packet(KURS_processor_t *this, alt_u8* _reply, alt_u32 reply_len);
void kurs_rx_ok_callback(KURS_processor_t *this, alt_u8 data);
void kurs_pkt_ok_callback(KURS_processor_t *this, alt_u8* data);
void* kurs_processor_get_context(const KURS_processor_t *this);
#endif /* KURS_PARSER_H_ */
