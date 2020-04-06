/**
 * ipg_als.h
 *
 *  Created on: May 29, 2019
 *      Author: Clara
 * @revision 0.0.1
 */

#ifndef IPG_ALS_H_
#define IPG_ALS_H_


#include "ipg_types.h"
#include "sys/queue.h"

#if ENABLE_ALS_CODE

/**
 * List of possible ALS set states.
 */
typedef enum{
	ALS_CONTROL_DISABLED = 0,  	/*!< KURS_ALS_CONTROL_DISABLED*/
	ALS_CONTROL_LINE = 1,			/*!< KURS_ALS_CONTROL_LINE    */
	ALS_CONTROL_CLIENT = 2, 		/*!< KURS_ALS_CONTROL_CLIENT  */
	ALS_CONTROL_REMOTE = 3,		/*!< KURS_ALS_CONTROL_REMOTE  */
}als_mode_t;

/**
 * List of possible ALS get states.
 */
typedef enum{
	ALS_STATUS_UNKNOWN = -1,  		/*!< ALS_GET_DISABLED*/
	ALS_STATUS_DISABLED = 0,  		/*!< ALS_GET_DISABLED*/
	ALS_STATUS_PASSIVE = 1,			/*!< ALS_GET_PASSIVE */
	ALS_STATUS_ACTIVE = 2, 			/*!< ALS_GET_ACTIVE  */
}als_status_t;

typedef struct{
	int index;
	/**
	 * RO bit.
	 * If this bit is set, then the ALS is active and has shutdown the laser.
	 * */
	ipglb_io_t io_active;
	/**
	 * RW bit.
	 * ALS is <b>enabled</b> when bit is <b>1</>, <b>disabled</b> when <b>0</b>
	 * */
	ipglb_io_t io_enable;
	/**
	 * RW IO
	 */
	ipglb_io_t io_auto_enable;	   //should be set to 1 in order to enable the automatic TX enable
							   //after a certain period of time when the cause of shutdown no longer exists
	ipglb_io_t io_manual_restart; /*!<it should be "0". If "auto_enable" is cleared, the system recovers from TX*/
							   //disable only when this bit is set.
	void(*set_hold)(alt_u32 _hold_sec);
	void(*set_restart_pulse)(alt_u32 _pulse_ms);
	ret_code_t(*set_mode)(void* _als, als_mode_t _mode);
	als_mode_t	control;
}als_t;

void* init_als_data(als_t* als);
void deinit_als(als_t* als);
void set_als_index(als_t* als, int index);
alt_u32 get_als_index(als_t* als);
als_status_t als_get_state (als_t* _als);
ret_code_t als_enable(als_t* _als);
ret_code_t als_disable(als_t* _als);
#endif	/*ENABLE_ALS_CODE*/

#endif /* IPG_ALS_H_ */
