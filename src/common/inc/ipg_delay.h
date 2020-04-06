/*
 * ipg_delay.h
 *
 *  Created on: Nov 20, 2011
 *      Author: ideapad1
 */

#ifndef IPG_DELAY_H_
#define IPG_DELAY_H_

#include "alt_types.h"
#include <system.h>
#include <sys/alt_stdio.h>
#include <sys/alt_alarm.h>
#include <stdio.h>


void Delay_s (alt_u32 delay_in_s);
void Delay_ms (alt_u32 delay_in_ms);
//void Delay_us (alt_u32 delay_in_us)
#endif /* IPG_DELAY_H_ */
