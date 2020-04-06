#include "ipg_aps.h"
#include "sys/alt_alarm.h"
#include "stdio.h"


#define APSSTATUSREG   _aps_ptr->regs.aps_status_reg
#define APSCONTROLREG  _aps_ptr->regs.aps_control_reg
#define APSTHRESHREG   _aps_ptr->regs.aps_threshold_reg
#define APSWDTHRESHREG _aps_ptr->regs.aps_work_deg_threshold

/*
 * local prototypes
 */
static ret_code_t aps_set_work_channel(aps_module_t* _aps_ptr);
static ret_code_t aps_set_protect_channel(aps_module_t* _aps_ptr);

/**
 * Function that map aps registers.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @param[in] base
 * Return none
 */
void map_aps_regs(aps_module_t* _aps_ptr, alt_u32 base)
{
	/*APS registers */
	APSSTATUSREG   = (volatile union APS_STATUS_REG*)((0x1000000 + 0x14)|
													   DE_CACHE_EN_BIT);
	APSCONTROLREG  = (volatile union APS_CONTROL_REG*)((base + 0x00)|
														DE_CACHE_EN_BIT);
	APSTHRESHREG   = (volatile union APS_THRESHOLD_REG*)((base+ 0x04)|
														DE_CACHE_EN_BIT);
	APSWDTHRESHREG = (volatile union APS_WORK_DEG_THRESHOLD*)((base + 0x08)|
															  DE_CACHE_EN_BIT);
/*	_aps_ptr->regs.aps_protect_deg_threshold =
 * (volatile union APS_PROTECT_DEG_THRESHOLD*)((base + 0x1C)|DE_CACHE_EN_BIT);*/
}
/**
 * Function to reset aps parameters to default.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * Return none
 */

void aps_reset(aps_module_t* _aps_ptr)
{
	_aps_ptr->enabled = 1;
	_aps_ptr->holdoff_timer = 0;
	_aps_ptr->wtr_timer = 20;
	_aps_ptr->revertive_mode.current = APS_REVERTIVE;
	_aps_ptr->protect_channel_index = APS_PROTECT_LINE_2;
	_aps_ptr->aps_mode.current = 0;
}
/**
 * Function to initialize aps parameters to default.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * Return none
 */
void aps_init(aps_module_t* _aps_ptr)
{
	_aps_ptr->aps_mode.current = _aps_ptr->aps_mode.next  =
								 APS_MODE_AUTO;
	_aps_ptr->aps_protectch.current = _aps_ptr->aps_protectch.next =
									  APS_PROTECT_LINE_2;
	_aps_ptr->revertive_mode.current = APS_REVERTIVE;
	/*the value in the register is in milli-seconds */
	_aps_ptr->holdoff_timer = 0;
	/*the value in the register is in milli-seconds */
	_aps_ptr->wtr_timer = 20;
	_aps_ptr->work_line.current = _aps_ptr->work_line.previous =
								  APS_PROTECT_LINE_1;
	_aps_ptr->protect_channel_index = APS_PROTECT_LINE_2;
}

__weak void aps_init_callback(aps_module_t* _aps_ptr)
{
	/**
	 * DO not modify this function.
	 * If needed implement in main application
	 */
}
/**
 * Function to set aps mode value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_set_mode(aps_module_t* _aps_ptr)
{
	ret_code_t ret = RET_SUCCESS;
	APSCONTROLREG->bits.APS_BIDIREC = 1;
	switch(_aps_ptr->aps_mode.current)
	{
		case APS_MODE_AUTO:
			/*APSCONTROLREG->bits.APS_REVERTIVE = 0; */
			APSCONTROLREG->bits.APS_MANUAL_SWITCH = 0;
			APSCONTROLREG->bits.APS_LOP = 0;
			APSCONTROLREG->bits.APS_FORCE_SWITCH = 0;
			APSCONTROLREG->bits.APS_EXCER = 0;
			break;
		case APS_MODE_MANUAL:
			APSCONTROLREG->bits.APS_MANUAL_SWITCH = 1;
			APSCONTROLREG->bits.APS_LOP = 0;
			APSCONTROLREG->bits.APS_FORCE_SWITCH = 0;
			APSCONTROLREG->bits.APS_EXCER = 0;
			break;
		case APS_MODE_LOP:
			APSCONTROLREG->bits.APS_MANUAL_SWITCH = 0;
			APSCONTROLREG->bits.APS_LOP = 1;
			APSCONTROLREG->bits.APS_FORCE_SWITCH = 0;
			APSCONTROLREG->bits.APS_EXCER = 0;
			break;
		case APS_MODE_FORCED_SWITCH:
			APSCONTROLREG->bits.APS_MANUAL_SWITCH = 0;
			APSCONTROLREG->bits.APS_LOP = 0;
			APSCONTROLREG->bits.APS_FORCE_SWITCH = 1;
			APSCONTROLREG->bits.APS_EXCER = 0;
			break;
		case APS_MODE_EXERCISE:
			APSCONTROLREG->bits.APS_MANUAL_SWITCH = 0;
			APSCONTROLREG->bits.APS_LOP = 0;
			APSCONTROLREG->bits.APS_FORCE_SWITCH = 0;
			APSCONTROLREG->bits.APS_EXCER = 1;
		break;
		default:
			ret = RET_ARG_MISMATCH;
		}
	ret = RET_SUCCESS;
	printf("(%u)>> ApsSetMode - request: %d, actual: 0x%x\n",  alt_nticks(),
			_aps_ptr->aps_mode, APSCONTROLREG->all);
	return 	(ret);
}
/**
 * Function to get aps mode value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_get_mode(aps_module_t* _aps_ptr)
{
	ret_code_t ret = RET_SUCCESS;
	if(!_aps_ptr->enabled)
	{
		ret = RET_INVALID_PARAMS;
	}
	if(1 == APSCONTROLREG->bits.APS_MANUAL_SWITCH)
	{
		_aps_ptr->aps_mode.current = APS_MODE_MANUAL;
	}
	else
	if(APSCONTROLREG->bits.APS_LOP)
	{
		_aps_ptr->aps_mode.current = APS_MODE_LOP;
	}
	else
	if(APSCONTROLREG->bits.APS_FORCE_SWITCH)
	{
		_aps_ptr->aps_mode.current = APS_MODE_FORCED_SWITCH;
	}
	else
	{
		_aps_ptr->aps_mode.current = APS_MODE_AUTO;
	}
	return (ret);
}
/**
 * Function to set aps protect channel value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
static ret_code_t aps_set_protect_channel(aps_module_t* _aps_ptr)
{
    if(APS_PROTECT_LINE_1 == _aps_ptr->protect_channel_index)
    {
	   APSCONTROLREG->bits.APS_W_LI1_SELECTED = 0;
	/*APSCONTROLREG->bits.APS_USER_W_SEL =
	 *  _aps_ptr->protect_channel_index; */
    }
    else
    {
    	APSCONTROLREG->bits.APS_W_LI1_SELECTED = 1;
    }
    printf("(%u)>> ApsSetProtectChannel - request: %d, actual: %d\n",
    		alt_nticks(), _aps_ptr->protect_channel_index - 1,
    		APSCONTROLREG->bits.APS_W_LI1_SELECTED);
	return 	(RET_SUCCESS);
}

/**
 * Function to get aps protect channel value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_get_protect_channel(aps_module_t* _aps_ptr)
{
	_aps_ptr->protect_channel_index = APSSTATUSREG->bits.APS_P_SELECTED;
	return 	(RET_SUCCESS);
}
/**
 * Function to set aps work channel value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
static ret_code_t aps_set_work_channel(aps_module_t* _aps_ptr)
{
	if(APS_PROTECT_LINE_1 == _aps_ptr->work_line.current)
	{
	  APSCONTROLREG->bits.APS_W_LI1_SELECTED = 1;
	}
	else
	{
	  APSCONTROLREG->bits.APS_W_LI1_SELECTED = 0;
	}
	printf("(%u)>> ApsSetWorkChannel - request: %d, actual: %d\n",
			alt_nticks(), _aps_ptr->work_line.current,
			APSCONTROLREG->bits.APS_W_LI1_SELECTED);
	return 	(RET_SUCCESS);
}
/**
 * Function to get aps work channel value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_get_work_channel(aps_module_t* _aps_ptr)
{
	_aps_ptr->work_line.current = APSSTATUSREG->bits.APS_W_SELECTED;
	return 	(RET_SUCCESS);
}
/**
 * Function to get aps revert mode value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_get_revert_mode(aps_module_t* _aps_ptr)
{
	_aps_ptr->revertive_mode.current = APSCONTROLREG->bits.APS_REVERTIVE;
	return 	(RET_SUCCESS);
}

/**
 * Function to set aps wait tor restore value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_set_wtr(aps_module_t* _aps_ptr)
{
	APSTHRESHREG->bits.APS_WAIT_TO_RESTORE_THRESHOLD = _aps_ptr->wtr_timer;
	printf("(%u)>> ApsSetWTRTmr - request: %d, actual: %d\n",  alt_nticks(),
			_aps_ptr->wtr_timer,
			APSTHRESHREG->bits.APS_WAIT_TO_RESTORE_THRESHOLD);
	return 	(RET_SUCCESS);
}
/**
 * Function to get aps wait to restore value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_get_WTR(aps_module_t* _aps_ptr)
{
	_aps_ptr->wtr_timer = APSTHRESHREG->bits.APS_WAIT_TO_RESTORE_THRESHOLD;
	return 	(RET_SUCCESS);
}
/**
 * Function to set aps hold off timer value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_set_ho(aps_module_t* _aps_ptr)
{
	APSTHRESHREG->bits.APS_HOLD_OFF_TIMER_THRESHOLD = _aps_ptr->holdoff_timer;
	printf("(%u)>> ApsSetHoldOffTmr - request: %d, actual: %d\n",
			alt_nticks(), _aps_ptr->holdoff_timer,
			APSTHRESHREG->bits.APS_HOLD_OFF_TIMER_THRESHOLD);
	return 	(RET_SUCCESS);
}
/**
 * Function to get aps hold off timer value.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_get_ho(aps_module_t* _aps_ptr)
{
	_aps_ptr->holdoff_timer = APSTHRESHREG->bits.APS_HOLD_OFF_TIMER_THRESHOLD;
	return 	(RET_SUCCESS);
}

/**
 * Function to disable aps.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_disable(aps_module_t* _aps_ptr)
{
	APSCONTROLREG->bits.APS_DISABLE = 1;
/*	if(_aps_ptr->assigned_aps_ptr_selected_line.current ==
 *  APS_PROTECT_LINE_1){ */
	APSCONTROLREG->bits.APS_W_LI1_SELECTED = 1;
	APSCONTROLREG->bits.APS_USER_W_SEL = 1;
/*	}else if(_aps_ptr->assigned_aps_ptr_selected_line.current ==
 *  APS_PROTECT_LINE_2){ */
/*	   APSCONTROLREG->bits.APS_W_LI1_SELECTED = 0; */
/*	   APSCONTROLREG->bits.APS_USER_W_SEL = 1; */
 /*   } */
	APSCONTROLREG->bits.APS_CH_ACTIVE = 0;
	_aps_ptr->enabled = 0;
	return 	(RET_SUCCESS);
}
/**
 * Function to enable aps.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_enable(aps_module_t* _aps_ptr)
{
	APSCONTROLREG->bits.APS_DISABLE = 0;
	APSCONTROLREG->bits.APS_CH_ACTIVE = 1;
	_aps_ptr->enabled = 1;
	return (RET_SUCCESS);
}
/**
 * Function that set aps revert mode.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t aps_set_revert_mode(aps_module_t* _aps_ptr)
{
	ret_code_t ret = RET_SUCCESS;
	switch(_aps_ptr->revertive_mode.current)
	{
		case 0:/*Enable line 1 non-revertive mode */
			_aps_ptr->revertive_mode.current = APS_NON_REVERTIVE;
			break;
		case 1:/*Disable line 1 revertive mode */
			_aps_ptr->revertive_mode.current = APS_REVERTIVE;
			break;
		default:
			ret = RET_INVALID_PARAMS;
			break;
	}
	if(ret == RET_SUCCESS)
	{
		APSCONTROLREG->bits.APS_REVERTIVE = _aps_ptr->revertive_mode.current;
	}
	return (ret);
}
/**
 * Function that set aps work/protect lines.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * @return RET_SUCCESS in case of success
 */
ret_code_t switch_working_line( aps_module_t* _aps_ptr)
{
	/*Switch working and protect lines */
	 if(APS_PROTECT_LINE_1 == _aps_ptr->aps_protectch.current)
	 {
		 _aps_ptr->work_line.current = APS_PROTECT_LINE_2;
		 _aps_ptr->protect_channel_index = APS_PROTECT_LINE_1;
	 }
	 else
	 if(APS_PROTECT_LINE_2 == _aps_ptr->aps_protectch.current)
	 {
		 _aps_ptr->work_line.current = APS_PROTECT_LINE_1;
		 _aps_ptr->protect_channel_index = APS_PROTECT_LINE_2;
	 }
	 aps_set_protect_channel(_aps_ptr);
	 aps_set_work_channel(_aps_ptr);
	printf("(%u)>> handle_line_sw_switch_request - work_line %d\n",
			alt_nticks(), _aps_ptr->work_line.current);
	return 	(RET_SUCCESS);
}
/**
 * Function to update aps dynamic data.
 * @param[in] _aps_ptr pointer to the _aps_ptr module
 * Return none
 */
void update_aps_ptr_data(aps_module_t* _aps_ptr)
{
	uint32_t temp = APSSTATUSREG->all >> (16*_aps_ptr->index); /*& 0xFF */
	struct APS_STATUS_BITS *aps_status = (struct APS_STATUS_BITS*)(&temp);
	/*APS request current state APS state is at 4 bits */
	switch(aps_status->APS_CUR_STATE )
	{
		case 0x0:
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_NR;
	      break;
		case 0x1 :
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_DNR;
		  break;
		case 0x2 :
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_RR;
		  break;
		case 0x4 :
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_EXER;
		  break;
		case 0x6 :
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_WTR;
		  break;
		case 0x8 :
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_MS;
		  break;
		case 0xA :/*0xA */
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_SD;
		  break;
		case 0xC :/*0xC */
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_SF;
		  break;
		case 0xE :/*0xE */
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_FS;
		  break;
		case 0xF :/*0xF */
			_aps_ptr->assigned_aps_request.current = APS_REQUEST_LO;
		  break;
		default :
		  break;
	}
	/*APS selected working line */
	if(1 == APSCONTROLREG->bits.APS_W_LI1_SELECTED)
	{
		if(1 == aps_status->APS_W_SELECTED)
		{
			_aps_ptr->assigned_aps_selected_line.current = APS_PROTECT_LINE_1;
		}
		else
		{
			_aps_ptr->assigned_aps_selected_line.current = APS_PROTECT_LINE_2;
		}
	}
	else
	{
		if(0 == APSCONTROLREG->bits.APS_W_LI1_SELECTED)
		{
			if(1 == aps_status->APS_W_SELECTED)
			{
				_aps_ptr->assigned_aps_selected_line.current =
				APS_PROTECT_LINE_2;
			}
			else
			{
				_aps_ptr->assigned_aps_selected_line.current =
				APS_PROTECT_LINE_1;
			}
		}
    }
	if(1 == APSCONTROLREG->bits.APS_DISABLE)
	{
		_aps_ptr->assigned_aps_selected_line.current = APS_PROTECT_LINE_1;
	}
}
