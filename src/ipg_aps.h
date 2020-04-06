#ifndef _IPG_aps_ptr_H_
#define _IPG_aps_ptr_H_

#include <stdint.h>
#include "common/inc/ipg_types.h"
#include "common/inc/ipg_constants.h"
/*
 * APS registers
 */

/*
 * 1- APS config status register
 * TODO remove config */

struct APS_STATUS_BITS
{
	volatile unsigned int  APS_W_SELECTED:1;     	/* bit 0 */
	volatile unsigned int  APS_P_SELECTED:1;     	/* bit 1 */
	volatile unsigned int  APS_EXCER_FAIL:1;     	/* bit 2 */
	volatile unsigned int  APS_FAIL_PROTOCOL:1;     /* bit 3 */
	volatile unsigned int  APS_CUR_STATE:4;     	/* bit 4 to bit 7 */
	volatile unsigned int  :24;    				    /* bit 8 to bit 15 */
};
union  APS_STATUS_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct  APS_STATUS_BITS bits;
};

/*
 * 2- APS control register
 */
struct APS_CONTROL_BITS
{
	volatile unsigned int  APS_W_LI1_SELECTED:1;    /* bit 0 */
	volatile unsigned int  APS_USER_W_SEL:1;     	/* bit 1 */
	volatile unsigned int  APS_DISABLE:1;     	    /* bit 2 */
	volatile unsigned int  :1;    				    /* bit 3 */
	volatile unsigned int  APS_FORCE_SWITCH:1;     	/* bit 4 */
	volatile unsigned int  APS_MANUAL_SWITCH:1;     /* bit 5 */
	volatile unsigned int  APS_LOP:1;     	        /* bit 6 */
	volatile unsigned int  APS_EXCER:1;     	    /* bit 7 */
	volatile unsigned int  APS_CH_ACTIVE:1;     	/* bit 8 */
	volatile unsigned int  APS_BIDIREC:1;     	    /* bit 9 */
	volatile unsigned int  APS_REVERTIVE:1;     	/* bit 10 */
	volatile unsigned int  :5;    				    /* bit 11 to bit 15 */
	volatile unsigned int  APS_W_DEGM_INTER:4;     	/* bit 16 to bit 19 */
	volatile unsigned int  APS_P_DEGM_INTER:4;     	/* bit 20 to bit 23 */
	volatile unsigned int  :8;    				    /* bit 24 to bit 31 */
};
union  APS_CONTROL_REG
{
 alt_u32 all;/* used to write or read from the whole register */
 struct  APS_CONTROL_BITS bits;
};
/*
 * 3- APS threshold register
 */
struct APS_THRESHOLD_BITS
{
	volatile unsigned int APS_HOLD_OFF_TIMER_THRESHOLD:14; /*bit 0 to bit 13*/
	volatile unsigned int :2;    				           /*bit 14 to bit 15*/
	volatile unsigned int APS_WAIT_TO_RESTORE_THRESHOLD:14;/*bit 16 to bit 29*/
	volatile unsigned int :2;    				           /*bit 30 to bit 31*/

};
union APS_THRESHOLD_REG
{
	alt_u32 all;/* used to write or read from the whole register */
    struct APS_THRESHOLD_BITS bits ;
};

/*
 * 4- APS work deg threshold
 */
union APS_WORK_DEG_THRESHOLD
{
	alt_u32 all;
};

/*
 * 5- APS protect deg threshold
 */
union APS_PROTECT_DEG_THRESHOLD
{
	alt_u32 all;
};

/**
 * List of APS registers
 */
typedef struct{
	 /* 0x0C from ODUK_MPIF_BRICK_0_BASE + 0x21000 */
	 volatile union APS_STATUS_REG* aps_status_reg;
	 /* 0x010 from ODUK_MPIF_BRICK_0_BASE + 0x21000 */
	 volatile union APS_CONTROL_REG* aps_control_reg;
	 /* 0x014 from ODUK_MPIF_BRICK_0_BASE + 0x21000 */
	 volatile union APS_THRESHOLD_REG* aps_threshold_reg;
	 /* 0x018 from ODUK_MPIF_BRICK_0_BASE + 0x21000 */
	 volatile union APS_WORK_DEG_THRESHOLD* aps_work_deg_threshold;
	 /* 0x01C from ODUK_MPIF_BRICK_0_BASE + 0x21000 */
	 volatile union APS_PROTECT_DEG_THRESHOLD* aps_protect_deg_threshold;
}aps_registers_t;

/**
 * Definition of APS revertive modes.
 */
typedef enum{
	APS_NON_REVERTIVE = 0,/*!< APS_NON_REVERTIVE */
	APS_REVERTIVE = 1     /*!< APS_REVERTIVE */
}APS_REVERTIVE_MODE_t;

/**
 * List of APS modes.
 */
typedef enum
{
	APS_MODE_AUTO = 0,         /*!< APS_MODE_AUTO */
	APS_MODE_LOP = 1,          /*!< APS_MODE_LOP */
	APS_MODE_FORCED_SWITCH = 2,/*!< APS_MODE_FORCED_SWITCH */
	APS_MODE_MANUAL = 3,       /*!< APS_MODE_MANUAL */
	APS_MODE_EXERCISE = 4      /*!< APS_MODE_EXERCISE */
}APS_MODE_t;

/**
 * List of APS requests.
 */
typedef enum{
	APS_REQUEST_NR = 0,  	/*!< NR   APS No request value*/
	APS_REQUEST_DNR = 1, 	/*!< DNR                        */
	APS_REQUEST_RR = 2,  	/*!< RR                         */
	APS_REQUEST_EXER = 4,	/*!< EXER                       */
	APS_REQUEST_WTR = 6, 	/*!< WTR                        */
	APS_REQUEST_MS = 8,  	/*!< MS                         */
	APS_REQUEST_SD = 10, 	/*!< SD                         */
	APS_REQUEST_SF = 12, 	/*!< SF                         */
	APS_REQUEST_FS = 14, 	/*!< FS                         */
	APS_REQUEST_LO = 15,	/*!< LO APS lockout request value*/
	APS_AUTO = 16,  	 	/*!< AUTO APS  value*/
}APS_REQUEST_t;

/**
 * List of Protect channels
 */
typedef enum
{
	APS_PROTECT_NO = 0,    /*!< No */
	APS_PROTECT_LINE_1 = 1,/*!< Line_1 */
	APS_PROTECT_LINE_2 = 2 /*!< Line_2 */
}APS_PROTECT_CHANNEL_t;
/* in case we have many aps module in one application we declare this struct */
typedef struct{
	/*!<Registers of the APS engine.*/
	aps_registers_t regs;
	/*!<Index of the APS module.*/
	uint32_t		index;
	/*!<When set to 1 APS engine is enabled.*/
	uint8_t 		enabled;
	/*!<The index of the line assigned as PROTECT*/
	uint32_t		protect_channel_index;
	/*!<The index of the line assigned as WORK*/
	ipg_status_t 	work_line;
	/*!<the APS revertive mode, can be a value of @ref aps_revertive_mode_t.*/
	ipg_control32_t revertive_mode;
	/*!<the APS mode of operation, can be a value of @ref aps_mode_t.*/
	ipg_control32_t aps_mode;
	/*!<Wait To Restore timer in milliseconds.*/
	uint32_t 		wtr_timer;
	/*!<Holdoff timer in milliseconds.*/
	uint32_t 		holdoff_timer;
	/*!<the APS request detected by the APS engine*/
	ipg_status_t 	assigned_aps_request;
	/*!<the APS protect channel selected by the user*/
	ipg_control32_t aps_protectch;
	/*!<the APS protect channel selected by the hardware*/
	ipg_status_t 	assigned_aps_selected_line;
	/*!<the APS protected channel selected by user
	 * (used to display user selection in table 2)*/
	uint32_t 		assigned_aps_signal;
}aps_module_t;

void map_aps_regs(aps_module_t* _aps_ptr, alt_u32 base);
void aps_init(aps_module_t* _aps_ptr);
void aps_reset(aps_module_t* _aps_ptr);

/*Set functions */
ret_code_t aps_set_revert_mode(aps_module_t* _aps_ptr);
ret_code_t aps_set_mode(aps_module_t* _aps_ptr);
ret_code_t aps_set_wtr(aps_module_t* _aps_ptr);
ret_code_t aps_set_ho(aps_module_t* _aps_ptr);
ret_code_t aps_disable(aps_module_t* _aps_ptr);
ret_code_t aps_enable(aps_module_t* _aps_ptr);

/*Get functions */
ret_code_t aps_get_protect_channel(aps_module_t* _aps_ptr);
ret_code_t aps_get_work_channel(aps_module_t* _aps_ptr);
ret_code_t aps_get_revert_mode(aps_module_t* _aps_ptr);
ret_code_t aps_get_mode(aps_module_t* _aps_ptr);
ret_code_t aps_get_wtr(aps_module_t* _aps_ptr);
ret_code_t aps_get_ho(aps_module_t* _aps_ptr);

ret_code_t aps_set_revert_mode(aps_module_t* _aps_ptr);
ret_code_t aps_enable_disable(uint8_t on_off,aps_module_t* aps);
ret_code_t switch_working_line( aps_module_t* _aps_ptr);
void update_aps_ptr_data(aps_module_t* _aps_ptr);

/*ret_code_t APsGetProtectDegradeThreshold(aps_module_t* _aps_ptr); */

#endif /* _IPG_aps_ptr_H_ */
