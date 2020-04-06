/****************************************************************************
 * * IPG Photonics Corporation
 * COPYRIGHT (C) 2015 IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *
 * IPG Photonics Corporation
 * 50 Old Webster Road
 * Oxford, MA 01540, USA
 ****************************************************************************/

/**
 * @file configs.h
 * @author d_chamoun
 * @date Jan 13, 2015
 * @brief This file contains precompile flags used in our project.
 *
 * @todo This file also contains some constant definitions that we need all
 * along this project. These have to be moved to a separate file.
 */
#ifndef CONFIGS_H_
#define CONFIGS_H_

#include "Middlewares/KURS/inc/kurs.h"

#ifndef __weak
#define __weak __attribute__((weak))
#endif
#define REDUCE_MEM
#define IPGLB_LEGACY_CODE  	0

#define KURS_PROTOCOL		1
#if KURS_PROTOCOL
#define KURS_DVT_CODE		1
#else
#define KURS_DVT_CODE		0
#endif
#define PFC_VER				2
#define PORT_VER			2
#define XGE_MAC_VER			2

#define ENABLE_ALS_CODE		1
#define ENABLE_FEC_CODE		1
#define ENABLE_PFC_CODE		1
#define ENABLE_OTUx_CODE	1
#define ENABLE_ETH_CODE		1
#define ENABLE_STMx_CODE	1
#define ENABLE_FCx_CODE		1
#define ENABLE_SDHx_CODE	1

#define ENABLE_XFP_CODE		0
#define ENABLE_OM5x7_CODE	0
#define ENABLE_SFP_CODE		1
#define ENABLE_QSFP_CODE	0
#define ENABLE_CFP_CODE		0
#define ENABLE_SFPP_CODE	1

#define ENABLE_ALT_PRBS_CODE 	0
#define ENABLE_BCM5248_CODE 	0

#if ENABLE_CFP_CODE
#define ENABLE_MENARA_CFP_CODE 1
#endif

/*#define BRINGUP */
#ifndef BRINGUP
/*!<Set to 1 to force OF alignment regardless of PPM handling */
#define OF_ALIGNMENT_ON				1
/*!<Set to 1 to force OF alignment regardless of PPM handling */
#define ENABLE_ARM7_REQUESTS		1
/*!<Uncomment to enable NiOS to reply to generic dvt requests */
#define ENABLE_DVT_CODE				1
/*!<Uncomment to enable NiOS to run periodic tasks */
#define ENABLE_PERIODIC_TASKS
/*#define PPM_ADJUSTMENT_ENABLED */
#ifdef PPM_ADJUSTMENT_ENABLED
#define PPM_ADJUSTMENT_SW
#endif
#endif

#define DVT_COM					0
#define ARM1_COM				1
//
#define INTERFACE_COUNT			18
#define IF_INDEX_START			0
#define IF_INDEX_END			INTERFACE_COUNT-1
#define CLIENTS_IF_COUNT		16
#define LINES_IF_COUNT			2
#define CL_INDEX_START			0
#define CL_INDEX_END			15
#define LN_INDEX_START			16
#define LN_INDEX_END			17
#define LN_INDEX_OFFSET			2
#define MAX_OPTICAL_INOUT_COUNT	1

#define SFP_COUNT_BUS			8
 /*from parallel flash base, should align with sector size (128*128KB)*/
#define PREFERENCES_OFFSET 		0x00840000
/*from parallel flash base, should align with sector size (128*128KB)*/
#define LNG_FILE_OFFSET 		0x00640000
#define PAGE_SIZE				0x20000
#ifdef PROF_V5
#define DEV_CLASS				"mxt70"
#define DEV_PREFIX				"mxtbr"
#else
#define DEV_CLASS				"tbd"
#define DEV_PREFIX				"tbd"
#endif
#ifdef PROF_V5
#ifndef PROF_VERSION
#define PROF_VERSION	"5.0"
#endif
#else
#ifndef PROF_VERSION
#define PROF_VERSION	"3.0"
#endif
#endif
/**
 * I2C addresses
 */
/*!<I2C address of the I2C multiplexer 0(U19). Check schematics for more
 * details.*/
#define	PCA9547_I2C_ADDRESS	0xEE

/**I2C address of the I2C SI-5338 quad-clock generator.
 * We have 5 of them on board, all of them having the same I2C address, but each
 * connected to a separate channel on a separate I2C multiplexer. We have to do
 * proper I2C selection to talk to these SILAB devices.
 * @see I2C_CHANNELS
 * */
#define SI5338_I2C_ADDRESS		0xE0
/**<I2C address of the I2C MCP9808 temperature sensor.
 * This is a sensor from Microchip.
 * We have to do proper I2C selection to talk to this device.
 * Check schematics for more details.
 * @see I2C_CHANNELS
 * */
#define MCP_9808_I2C_ADDRESS	0x30
/**I2C address of the I2C SI-5326 clock multiplier.
 * We have to do proper I2C selection to talk to this device.
 * Check schematics for more details (U26)
 * @see I2C_CHANNELS
 * */
#define SI5326_I2C_ADDRESS 		0xD6
/**I2C address of the second I2C SI-5326 clock multiplier.
 * We have to do proper I2C selection to talk to this device.
 * Check schematics for more details (U2)
 * @see I2C_CHANNELS
 * */
#define SI5326_2_I2C_ADDRESS 	0xD2
/**I2C address of the I2C SFP+.
 * We have 10 of them on board, all of them having the same I2C address,
 * but each connected to a separate channel on a separate I2C multiplexer.
 * We have to do proper I2C selection to talk to these SILAB devices.
 * @see I2C_CHANNELS
 * */
#define SFP_MSA_0_I2C_ADDRESS	0xA0
#define SFPP_MSA_0_I2C_ADDRESS	0xA0


enum SFPP_I2C_CHANNELS
{
	CHANNEL_SFPP_0 = 0,
	CHANNEL_SFPP_1 ,
};

enum SI5375_I2C_CHANNELS
{
	CHANNEL_SI5375_0 = 0,
	CHANNEL_SI5375_1 ,
};
/**Enum of the I2C channels.
 * We have two I2C multiplexers (being I2C devices themselves) with 8 channels
 * each and an I2C hub with 4 channels.
 * This allows us to connect an additional 20 I2C devices.
 * The software will consider all of these channels as channels of a virtual
 * I2C multiplexer.
 */
enum I2C_CHANNELS
{
	CHANNEL_NONE = -1,      /*!<No channel of virtual mux is selected*/
	CHANNEL_SFP_SI5338_0 = 0,
	CHANNEL_SFP_SI5338_1,
	CHANNEL_SFP_SI5338_2,
	CHANNEL_SFP_3,
	CHANNEL_SFP_4,
	CHANNEL_SFP_5,
	CHANNEL_SFP_6,
	CHANNEL_SFP_7,
	CHANNEL_MCP9808_1, /*MCP9808 temperature sensor*/
	CHANNEL_MCP9808_2, /*MCP9808 temperature sensor*/
};


#endif /* CONFIGS_H_ */
