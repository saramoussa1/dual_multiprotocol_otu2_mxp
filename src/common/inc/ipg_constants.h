/*
 * ipg_constants.h
 *
 *  Created on: Jun 29, 2018
 *      Author: dcham
 */

#ifndef IPG_CONSTANTS_H_
#define IPG_CONSTANTS_H_

#ifndef DE_CACHE_EN_BIT
#define DE_CACHE_EN_BIT		0x80000000 //BIT31
#endif

#define TRANSCEIVER_ID_LEN		61
#define VN_LENGTH				17					//!<Constant representing the length of a transceiver vendor name string.
#define SN_LENGTH				17					//!<Constant representing the length of transceiver serial number string.
#define PN_LENGTH				17					//!<Constant representing the length of transceiver part number string.
#define RN_LENGTH				5					//!<Constant representing the length of transceiver revision number string.

#ifdef KURS_PROTOCOL
#define HN_LENGTH				8					//!<Constant representing the length of transceiver revision number string.
#define FN_LENGTH				8					//!<Constant representing the length of transceiver revision number string.

#define HW_NUM_LEN				65
#define SW_NUM_LEN				33
#define SW_NUM_FPGA_LEN			8
#define PID_LEN					65
#define SR_NUM_LEN				17
#define IF_DESTINATION_MAX_LEN	33
#define IF_DESCRIPTION_MAX_LEN	33

#define PORT_INFO_SIZE			33					//!<length of a port info string


/*Default thresholds definitions*/
//Default threshold for the FEC load
#define DEF_OTU4_FECLOAD_HMAX       100  //!<default WMax threshold for FecLoad parameter
#define DEF_OTU4_FECLOAD_CMAX       40   //!<default CMax threshold for FecLoad parameter
#define DEF_OTU4_FECLOAD_WMAX       25   //!<default WMax threshold for FecLoad parameter
#endif

#endif /* IPG_CONSTANTS_H_ */
