/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_delay.c
 * @author Clara Zaiter (czaayter@ipgphotonics.com)
 * @author Dany Chamoun (dchamoun@ipgphotonics.com)
 * @author Grace Khayat (gkhayat@ipgphotonics.com)
 * @date Oct 7, 2015
 * @brief contains the functions used to create system delays.
 *
 */
#include "../inc/ipg_delay.h"

#if 1
#define alt_ticks_per_ms (alt_ticks_per_second() /1000UL);

/**This function creates a delay in seconds.
 * This function should not be called from within an ISR.
 * @param[in] delay_in_s desired delay in seconds
 * @return None
 * @since 1.0.0
 */
void Delay_s (alt_u32 delay_in_s){
	alt_u32 snapshot_in_time = alt_nticks();
	alt_u32 delay_in_ticks = delay_in_s * alt_ticks_per_second();
	alt_u32 current_ticks = 0 ;
	do{
		current_ticks = alt_nticks() ;
	} while ((current_ticks - snapshot_in_time) < delay_in_ticks );
}

/** Creates a delay in milliseconds.
 * This function should not be called from within an ISR.
 * @param[in] delay_in_ms	desired delay in ms
 * @return None
 * @since 1.0.0
 */
void Delay_ms (alt_u32 delay_in_ms){
	alt_u32 snapshot_in_time = alt_nticks();
	alt_u32 delay_in_ticks = delay_in_ms * alt_ticks_per_ms ;
	alt_u32 current_ticks = 0 ;
		do{
			current_ticks = alt_nticks() ;
		} while ((current_ticks - snapshot_in_time) < delay_in_ticks );
}

#if WATCHDOG_US_ACCURATE
/// <summary>
/// Creates a delay in the order of milliseconds
/// </summary>
/// <param name="ms">length of the delay in ms</param>
void Delay_us (alt_u32 delay_in_us)
{
	alt_u32 alt_ticks_per_us  = (WATCHDOG_TIMER_TICKS_PER_SEC /1000000UL);
	alt_u32 snapshot_in_time = alt_nticks();
	alt_u32 delay_in_ticks = delay_in_us * alt_ticks_per_us ;
	alt_u32 current_ticks = 0 ;
	do
	{
		current_ticks = alt_nticks() ;
	} while (current_ticks - snapshot_in_time < delay_in_ticks );
}
#endif
#endif
