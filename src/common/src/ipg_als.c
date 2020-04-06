/*
 * ipg_als.c
 *
 *  Created on: May 29, 2019
 *      Author: Clara
 */

#include "../inc/ipg_als.h"
#include "sys/alt_alarm.h"
#include "../inc/ipg_constants.h"
#include "stdlib.h"
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#if ENABLE_ALS_CODE

#define ENABLE_ALS(als) SET_IO(als->als_enable)
#define DISABLE_ALS(als) CLEAR_IO(als->als_enable)
#define ENABLE_AUTO_ALS(als) SET_IO(als->als_auto_enable)
#define DISABLE_AUTO_ALS(als) CLEAR_IO(als->als_auto_enable)
#define RESTART_ALS(als) SET_IO(als->als_manual_restart)
#define ALS_ENABLED(als) GET_IO(als->io_enable)//State 1 => check ALS TX disable, State 0 => Disabled
#define ALS_STATE(als) GET_IO(als->io_active)//State 1 => Active, State 0 => Passive
/**
 * List of possible ALS modes.
 */
enum ALS_MODE{
	ALS_MODE_AUTOMATIC = 0, /*!< ALS_AUTOMATIC_MODE*/
	ALS_MODE_LINE = 1,		/*!< ALS_LINE_MODE     */
	ALS_MODE_REMOTE = 2, 	/*!< ALS_REMOTE_MODE   */
};

void* init_als_data(als_t* als){
	if(als == NULL){
		als = (als_t*)malloc(sizeof(als_t));
	}
//	map_als_regs(als);
//	als->als_mode.current = ALS_AUTOMATIC_MODE;
	return als;
}

void deinit_als(als_t* als){
	free(als);
	als = NULL;
}

void set_als_index(als_t* als, int index){
	als->index = index;
}

alt_u32 get_als_index(als_t* als){
	return als->index;
}

als_status_t als_get_state (als_t* _als){
	als_status_t _status = ALS_STATUS_UNKNOWN;
	if(!ALS_ENABLED(_als)){
		_status = ALS_STATUS_DISABLED;
	}else if(ALS_STATE(_als)){
		_status = ALS_STATUS_ACTIVE;
	}else{
		_status = ALS_STATUS_PASSIVE;
	}
	return (_status);
}

ret_code_t als_enable(als_t* _als){
	ret_code_t err = RET_SUCCESS;
	SET_IO(_als->io_enable);
	if(!IS_IO_SET(_als->io_enable)){
		err = RET_FAIL;
	}
	return (err);
}

ret_code_t als_disable(als_t* _als){
	ret_code_t err = RET_SUCCESS;
	CLEAR_IO(_als->io_enable);
	if(!IS_IO_CLEAR(_als->io_enable)){
		err = RET_FAIL;
	}
	return (err);
}
#endif // ENABLE_ALS_CODE
