/*
 * ipg_pfc.c
 *
 *  Created on: Jun 22, 2018
 *      Author: dcham
 */
#include "../inc/ipg_pfc.h"
#include "sys/alt_alarm.h"
#include "../inc/ipg_constants.h"
#include "stdlib.h"
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#if ENABLE_PFC_CODE
#define TIMER_UNINIT	-1
#define PFC_TIMER_IDLE 		0
#define PFC_TIMER_RUNNING		1

static void refresh_pfc_statistics(performance_counters_t* pfc);
static void map_pfc_regs(performance_counters_t* pfc);
static alt_u32 pfc_timer_isr(void* context);
/**
 * Bit definitions for \c PFC_CONTROL_REG.
 */
struct PFC_CONTROL_BITS{
	volatile unsigned int  	PFC_EOP:1;	/*!<b'1:0  - RO:*/
	volatile unsigned int  	:31;		/*!<b'31:1 - RO: reserved*/
};

/**
 * Definition of \c  PFC_CONTROL_REG.
 * Maps to register 0x44 from brick top base address.
 */
union PFC_CONTROL_REG{
	alt_u32 all;													/*!<Used to read/write the whole register.*/
	struct PFC_CONTROL_BITS bits;	/*!<Used for bit access*/
};

/**
 * Definition of \c BRICK_TOP_PFC_ERRORED_SECS_REG.
 */
union PFC_ERRORED_SECS_REG{
	alt_u32 all;		//!<b'31:0
};

/**
 * Definition of \c BRICK_TOP_PFC_SEVERELY_ERRORED_SECS_REG.
 */
union PFC_SEVERELY_ERRORED_SECS_REG{
	alt_u32 all;		//!<b'31:0
};

/**
 * Definition of \c PFC_BACKGROUND_BLOCK_ERRORS1_SECS_REG.
 */
union PFC_BACKGROUND_BLOCK_ERRORS1_SECS_REG{
	alt_u32 all;		//!<b'31:0
};


/**
 * Bit definitions for \c BRICK_TOP_PFC_BACKGROUND_BLOCK_ERRORS2_SECS_REG.
 */
struct PFC_BACKGROUND_BLOCK_ERRORS2_SECS_BITS{
	volatile unsigned int  	PERF_CNT:13;		/*!<b'12:0 - RO: Upper 13 bits of the BBE*/
	volatile unsigned int  	RESERVED_1:19;		/*!<b'31:13 - RO: reserved FFU*/
};

/**
 * Definition of \c  PFC_BACKGROUND_BLOCK_ERRORS2_SECS_REG.
 * Maps to register 0x14 from brick top base address.
 */
union PFC_BACKGROUND_BLOCK_ERRORS2_SECS_REG{
	alt_u32 all;													/*!<Used to read/write the whole register.*/
	struct PFC_BACKGROUND_BLOCK_ERRORS2_SECS_BITS bits;	/*!<Used for bit access*/
};

/**
 * Definition of \c PFC_UNAVAILABLE_SECS_REG.
 */
union PFC_UNAVAILABLE_SECS_REG{
	alt_u32 all;		//!<b'31:0
};

typedef struct{
	alt_u32 base;
	volatile union PFC_CONTROL_REG* pfc_end_of_period;     //!<Variable that represent PFC_CONTROL_REG register mapped to offset 0x00 from MISC base
	volatile union PFC_ERRORED_SECS_REG* pfc_errored_secs;     //!<Variable that represent BRICK_TOP_PFC_ERRORED_SECS_REG register mapped to offset 0x2C from MISC base
	volatile union PFC_SEVERELY_ERRORED_SECS_REG* pfc_severely_errored_secs;     //!<Variable that represent BRICK_TOP_PFC_SEVERELY_ERRORED_SECS_REG register mapped to offset 0x30 from MISC base
	volatile union PFC_BACKGROUND_BLOCK_ERRORS1_SECS_REG* pfc_background_block_err1_secs;//!<Variable that represent BRICK_TOP_PFC_BACKGROUND_BLOCK_ERRORS1_SECS_REG register mapped to offset 0x34 from MISC base
	volatile union PFC_BACKGROUND_BLOCK_ERRORS2_SECS_REG* pfc_background_block_err2_secs;//!<Variable that represent BRICK_TOP_PFC_BACKGROUND_BLOCK_ERRORS2_SECS_BITS register mapped to offset 0x38 from MISC base
	volatile union PFC_UNAVAILABLE_SECS_REG*  pfc_unavailable_secs;//!<Variable that represent BRICK_TOP_PFC_UNAVAILABLE_SECS_REG register mapped to offset 0x3C from MISC base
}performance_counters_registers_t;


typedef struct{
	alt_alarm 		timer;
	int				state;
}ipglb_timer_t;

typedef struct{
	ipglb_timer_t	timer;				/*!<Specifies the timer object used for performance monitoring.*/
	alt_u32			monitor_period_s;	/*!<Specifies the desired performance monitoring time interval in seconds.*/
	alt_u64 		block_per_s;
}performance_counters_control_t;

typedef struct{
	alt_u32			end_of_period;
	alt_u32			elapsed_time_s;	/*!<Specifies the current performance monitoring time interval in seconds.*/
	alt_u32			err_secs;
	alt_u32			sev_err_secs;
	alt_u64			background_block_err;
	alt_u32			unavailable_secs;
	float			bber;	/*!< Specifies the background block error rate. Calculated based on BBER = cBBE/(P-UAS-SES)*/
	float			esr;   	/*!<Specifies the errored second rate. Calculated based on ESR = ES/(P-UAS)*/
	float			sesr;  	/*!<Specifies the severely errored second rate. Calculated based on SESR = cSES/(P-UAS).*/
}performance_counters_monitor_t;


struct performance_counters{
	int index;
	performance_counters_registers_t regs;
	performance_counters_monitor_t monitor;
	performance_counters_control_t control;
};

void* init_pfc_data(performance_counters_t* pfc, alt_u32 base){
	if(pfc == NULL){
		pfc = (performance_counters_t*)malloc(sizeof(performance_counters_t));
	}
	pfc->regs.base = base;
	map_pfc_regs(pfc);
	pfc->control.monitor_period_s = DEF_PFC_PERIOD;
	reset_pfc_statistics(pfc);
	pfc->control.block_per_s = 12288000;
	pfc->control.timer.state = PFC_TIMER_IDLE;
	pfc->monitor.elapsed_time_s = 0;
	return pfc;
}

void deinit_pfc(performance_counters_t* pfc){
	free(pfc);
	pfc = NULL;
}

void set_pfc_index(performance_counters_t* pfc, int index){
	pfc->index = index;
}
alt_u32 get_pfc_index(performance_counters_t* pfc){
	return pfc->index;
}

static void map_pfc_regs(performance_counters_t* pfc){
	alt_u32 base = pfc->regs.base;
	pfc->regs.pfc_errored_secs               = (volatile union PFC_ERRORED_SECS_REG*) ((base + 0x00) | DE_CACHE_EN_BIT);
	pfc->regs.pfc_severely_errored_secs      = (volatile union PFC_SEVERELY_ERRORED_SECS_REG*) ((base + 0x04) | DE_CACHE_EN_BIT);
	pfc->regs.pfc_background_block_err1_secs = (volatile union PFC_BACKGROUND_BLOCK_ERRORS1_SECS_REG*) ((base + 0x08) | DE_CACHE_EN_BIT);
	pfc->regs.pfc_background_block_err2_secs = (volatile union PFC_BACKGROUND_BLOCK_ERRORS2_SECS_REG*) ((base + 0x0C) | DE_CACHE_EN_BIT);
	pfc->regs.pfc_unavailable_secs           = (volatile union PFC_UNAVAILABLE_SECS_REG*) ((base + 0x10) | DE_CACHE_EN_BIT);
	pfc->regs.pfc_end_of_period 		     = (volatile union PFC_CONTROL_REG*) ((base + 0x1C) | DE_CACHE_EN_BIT);

}



void reset_pfc_statistics(performance_counters_t* pfc){
	pfc->monitor.bber = pfc->monitor.esr = pfc->monitor.sesr = 0;
#if PFC_VER == PFC_V2
	pfc->monitor.background_block_err = pfc->monitor.err_secs = pfc->monitor.sev_err_secs = pfc->monitor.unavailable_secs = 0;
#endif
	reset_pfc_statistics_callback(pfc->index);
	reset_pfc_hw_counters(pfc);
}

alt_u8 get_pfc_timer_state(performance_counters_t* pfc){
	return pfc->control.timer.state;
}

void set_pfc_monitor_period(performance_counters_t* pfc, alt_u32 period_s){
	if(pfc == NULL){
		return;
	}
	stop_pfc_monitoring(pfc);
	alt_u32 previous = pfc->control.monitor_period_s;
	pfc->control.monitor_period_s  = period_s;
	if(start_pfc_monitoring(pfc) != RET_SUCCESS){
		// restore the old value
		pfc->control.monitor_period_s = previous;
	}
}

/**
 * Callback function that gets called after PFC statistics have been reset.
 */
__weak void reset_pfc_statistics_callback(performance_counters_t* pfc){
	/**
	 * Do not modify. Implement in adapter file if required.
	 */
}

__weak void reset_pfc_hw_counters(performance_counters_t* pfc){
	pfc->regs.pfc_end_of_period->bits.PFC_EOP = 1;
	pfc->regs.pfc_end_of_period->bits.PFC_EOP = 0;
	/*#warning fix line section when you have the address*/
	//	LN_MMAP(index-4)->otu1_mapper->ln_cfg_reg->bits.PFC_EOP = 1;
	//	Delay_ms (1);
	//	LN_MMAP(index-4)->otu1_mapper->ln_cfg_reg->bits.PFC_EOP = 0;
}

static alt_u32 pfc_timer_isr(void* context){
	performance_counters_t* _pfc = (performance_counters_t*) context;
	_pfc->monitor.elapsed_time_s++;
	if(_pfc->monitor.elapsed_time_s == _pfc->control.monitor_period_s){
#if PFC_VER != PFC_V2
		reset_pfc_hw_counters(_pfc);
#endif
		_pfc->monitor.elapsed_time_s = 0;
	}else{
		refresh_pfc_statistics(_pfc);
	}
	return alt_ticks_per_second();
}

ret_code_t start_pfc_monitoring(performance_counters_t* pfc){
	if(pfc == NULL){
		return RET_FAIL;
	}
	stop_pfc_monitoring(pfc);
	if(alt_alarm_start (&pfc->control.timer.timer, alt_ticks_per_second(), pfc_timer_isr , pfc) == 0){
		pfc->control.timer.state = PFC_TIMER_RUNNING;
		pfc->monitor.elapsed_time_s = 0;
		return RET_SUCCESS;
	}
	return RET_FAIL;
}

void stop_pfc_monitoring(performance_counters_t* pfc){
	if(pfc == NULL)return;
	if(pfc->control.timer.state == PFC_TIMER_RUNNING){
		alt_alarm_stop(&pfc->control.timer.timer);//stop the old running timers for all the clients
		pfc->control.timer.state = PFC_TIMER_IDLE;
#if PFC_VER != PFC_V2
		reset_pfc_hw_counters(pfc);
		reset_pfc_statistics(pfc);
#endif
	}
}

/**
 * This function reads the PFC HW registers and updates the reported PFC counters.
 * It will be automatically called every one second.
 * @param[in,out] pfc pointer to the performance counter object
 */
static void refresh_pfc_statistics(performance_counters_t* pfc){
	if(pfc == NULL){
		return;
	}
	pfc->monitor.end_of_period = pfc->regs.pfc_end_of_period->bits.PFC_EOP;
	pfc->monitor.err_secs = pfc->regs.pfc_errored_secs->all;
	pfc->monitor.sev_err_secs = pfc->regs.pfc_severely_errored_secs->all;
	pfc->monitor.background_block_err = (alt_u64)(pfc->regs.pfc_background_block_err2_secs->bits.PERF_CNT << 32)|(pfc->regs.pfc_background_block_err1_secs->all);
	pfc->monitor.unavailable_secs = pfc->regs.pfc_unavailable_secs->all;
	pfc->monitor.bber = (float)pfc->monitor.background_block_err / ((pfc->control.monitor_period_s - pfc->monitor.unavailable_secs - pfc->monitor.sev_err_secs)*pfc->control.block_per_s);
	pfc->monitor.esr =(float) pfc->monitor.err_secs / (pfc->control.monitor_period_s - pfc->monitor.unavailable_secs);
	pfc->monitor.sesr = (float)pfc->monitor.sev_err_secs / (pfc->control.monitor_period_s - pfc->monitor.unavailable_secs);
}

/**
 * This function reports the BBE ratio.
 * @param[in,out] pfc pointer to the performance counter object
 * @return the BBER value.
 */
float get_pfc_bber(performance_counters_t* pfc){
	if(pfc == NULL){
		return 0;
	}
	return pfc->monitor.bber;
}

/**
 * This function reports the ES ratio.
 * @param[in,out] pfc pointer to the performance counter object
 * @return the ES ratio.
 */
float get_pfc_esr(performance_counters_t* pfc){
	if(pfc == NULL){
		return 0;
	}
	return pfc->monitor.esr;
}

/**
 * This function reports the SES ratio.
 * @param[in,out] pfc pointer to the performance counter object
 * @return the SES ratio.
 */
float get_pfc_sesr(performance_counters_t* pfc){
	if(pfc == NULL){
		return 0;
	}
	return pfc->monitor.sesr;
}

/**
 * This function reports the UAS count.
 * @param[in,out] pfc pointer to the performance counter object
 * @return the UAS count.
 */
alt_u32 get_pfc_uas(performance_counters_t* pfc){
	if(pfc == NULL){
		return 0;
	}
	return pfc->monitor.unavailable_secs;
}

/**
 * This function reports the BBE count.
 * @param[in,out] pfc pointer to the performance counter object
 * @return the BBER value.
 */
uint64_t get_pfc_bbe(performance_counters_t* pfc){
	if(pfc == NULL){
		return 0;
	}
	return pfc->monitor.background_block_err;
}

/**
 * This function reports the ES count.
 * @param[in,out] pfc pointer to the performance counter object
 * @return the ES ratio.
 */
alt_u32 get_pfc_es(performance_counters_t* pfc){
	if(pfc == NULL){
		return 0;
	}
	return pfc->monitor.err_secs;
}

/**
 * This function reports the SES count.
 * @param[in,out] pfc pointer to the performance counter object
 * @return the SES ratio.
 */
alt_u32 get_pfc_ses(performance_counters_t* pfc){
	if(pfc == NULL){
		return 0;
	}
	return pfc->monitor.sev_err_secs;
}

#endif // ENABLE_PFC_CODE
