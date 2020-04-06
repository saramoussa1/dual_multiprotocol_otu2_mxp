/*
 * ipglb_adapter.h
 *
 *  Created on: Jun 6, 2018
 *      Author: dcham
 */

#ifndef IPGLB_ADAPTER_H_
#define IPGLB_ADAPTER_H_
#include "configs.h"
#include "common/inc/ipg_types.h"
typedef struct
{
	union
	{
		struct odux_alarms_bits
		{
			volatile unsigned int AIS:1;
			volatile unsigned int OCI:1;
			volatile unsigned int LCK:1;
			volatile unsigned int PM_BIP:1;
			volatile unsigned int PM_BEI:1;
			volatile unsigned int PM_BDI:1;
			volatile unsigned int RESERVED_1:26;
		} bits;
		alt_u32 all;
	};
}protocol_odux_alarms_t;

extern protocol_odux_alarms_t odux_alarms[INTERFACE_COUNT];
void update_diagnostic_alarms(diagnostics_t* diagnostic_ptr);
/*void update_fec_load_alarms(diagnostics_t* fec_load); */
void update_alarms(void);
void update_card_alarm1(void);
void update_card_alarm2(void);
#endif /* IPGLB_ADAPTER_H_ */
