/*
 * ipglb_interface.h
 *
 *  Created on: May 8, 2018
 *      Author: dcham
 */

#ifndef IPGLB_PFC_H_
#define IPGLB_PFC_H_

#include "ipg_types.h"
#include "sys/queue.h"

#define PFC_V1	1
#define PFC_V2	2
#ifndef PFC_VER
#define PFC_VER	PFC_V1
#endif

#if ENABLE_PFC_CODE

#ifndef DEF_PFC_PERIOD
#define DEF_PFC_PERIOD 900 //15min
#endif

typedef struct performance_counters performance_counters_t;

void* init_pfc_data(performance_counters_t* pfc, uint32_t base);
void deinit_pfc(performance_counters_t* pfc);
void stop_pfc_monitoring(performance_counters_t* pfc);
ret_code_t start_pfc_monitoring(performance_counters_t* pfc);
void reset_pfc_hw_counters(performance_counters_t* pfc);
void set_pfc_monitor_period(performance_counters_t* pfc, uint32_t period_s);
float get_pfc_bber(performance_counters_t* pfc);
float get_pfc_esr(performance_counters_t* pfc);
float get_pfc_sesr(performance_counters_t* pfc);
alt_u32 get_pfc_uas(performance_counters_t* pfc);
void reset_pfc_statistics(performance_counters_t* pfc);
void set_pfc_index(performance_counters_t* pfc, int index);
uint32_t get_pfc_index(performance_counters_t* pfc);
uint64_t get_pfc_bbe(performance_counters_t* pfc);
uint32_t get_pfc_es(performance_counters_t* pfc);
uint32_t get_pfc_ses(performance_counters_t* pfc);
void reset_pfc_statistics_callback(performance_counters_t* _pfc);
#endif	/*ENABLE_PFC_CODE*/
#endif /* IPGLB_PFC_H_ */
