/*
 * ipglb_port.h
 *
 *  Created on: Jun 12, 2018
 *      Author: dcham
 */

#ifndef IPGLB_PORT_H_
#define IPGLB_PORT_H_

#include "ipg_types.h"
#if ENABLE_PFC_CODE
#include "ipg_pfc.h"
#endif
#if ENABLE_ALS_CODE
#include "ipg_als.h"
#endif

#define PORT_V1	1
#define PORT_V2	2
#ifndef PORT_VER
#define PORT_VER	PORT_V2
#endif

/**
 * List of possible protocols.
 */
typedef enum{
	PORT_PROTOCOL_ETHERNET = 0x01,     //!< IPG_DATA_PROTOCOL_ETHERNET
	PORT_PROTOCOL_OTU = 0x02,          //!< IPG_DATA_PROTOCOL_OTU
	PORT_PROTOCOL_SONET = 0x03,        //!< IPG_DATA_PROTOCOL_SONET
	PORT_PROTOCOL_SDH = 0x04,          //!< IPG_DATA_PROTOCOL_SDH
	PORT_PROTOCOL_FIBER_CHANNEL = 0x05,//!< IPG_DATA_PROTOCOL_FIBER_CHANNEL
	// keep at the bottom
	PORT_PROTOCOL_UNKNOWN = 255        //!< IPG_DATA_PROTOCOL_UNKNOWN
}port_protocol_t;

/**
 * List of possible data protocol rates.
 */
typedef enum{
	PORT_RATE_FE = 0x01,   //!< IPG_DATA_RATE_FE
	PORT_RATE_GE = 0x02,   //!< IPG_DATA_RATE_GE
	PORT_RATE_XGE = 0x03,  //!< PORT_RATE_XGE
	PORT_RATE_100GE = 0x04,//!< IPG_DATA_RATE_100GE
	PORT_RATE_OC3 = 0x05,  //!< IPG_DATA_RATE_OC3
	PORT_RATE_OC12 = 0x06, //!< IPG_DATA_RATE_OC12
	PORT_RATE_OC48 = 0x07, //!< IPG_DATA_RATE_OC48
	PORT_RATE_OC192 = 0x08,//!< IPG_DATA_RATE_OC192
	PORT_RATE_STM1 = 0x09, //!< IPG_DATA_RATE_STM1
	PORT_RATE_STM4 = 0x0A, //!< IPG_DATA_RATE_STM4
	PORT_RATE_STM16 = 0x0B,//!< IPG_DATA_RATE_STM16
	PORT_RATE_STM64 = 0x0C,//!< PORT_RATE_STM64
	PORT_RATE_FC1 = 0x0D,  //!< IPG_DATA_RATE_FC1
	PORT_RATE_FC2 = 0x0E,  //!< IPG_DATA_RATE_FC2
	PORT_RATE_OTU1 = 0x0F, //!< IPG_DATA_RATE_OTU1
	PORT_RATE_OTU2 = 0x10, //!< IPG_DATA_RATE_OTU2
	PORT_RATE_OTU4 = 0x11, //!< PORT_RATE_OTU4
	// keep at the bottom
	PORT_RATE_UNKNOWN = 255//!< IPG_DATA_RATE_UNKNOWN
}port_rate_t;

/**
 * List of possible port operations.
 */
typedef enum{
	PORT_OPERATION_INVALID = -1, //!< PORT_OPERATION_INVALID
	PORT_OPERATION_CLIENT = 0x01,//!< PORT_OPERATION_CLIENT
	PORT_OPERATION_LINE = 0x02,  //!< PORT_OPERATION_LINE
}port_operation_mode_t;

typedef struct{
	volatile unsigned int FEC_CAPABLE:1;
	volatile unsigned int PFC_CAPABLE:1;
	volatile unsigned int MULTI_RATE:1;
	volatile unsigned int ALS_CAPABLE:1;
	volatile unsigned int reserved_1:29;
}port_capabilities_t;

#if ENABLE_ETH_CODE
/**
 * Definition of the Ethernet object alarms
 */
#ifdef KURS_PROTOCOL
typedef struct
{
	union{
		struct bits_eth{
			volatile unsigned int LOS:1;
			volatile unsigned int LINK_DOWN:1;
			volatile unsigned int FCS:1;
			volatile unsigned int reserved:29;
		} bits;
		alt_u32 all;
	};
}protocol_eth_alarms_t;
#else
/**
 * Definition of the Ethernet object alarms
 */
typedef struct{
	volatile unsigned int LINK_DOWN:1;			/*!<LINK down alarm status.*/
	volatile unsigned int REMOTE_LINK_DOWN:1;	/*!<remote LINK down alarm status.*/
	volatile unsigned int RESERVED_1:30;		/*!< Reserved FFU*/
}protocol_eth_alarms_t;
#endif
/**
 * Definition of the Ethernet object statistics
 */
typedef struct{
	protocol_eth_alarms_t alarms;		/*!<Instance of the current ETH alarms*/
	ipg_counter64_t  rx_ok;				/*!<counter (31:0) of a 32-bit counter of successfully received frames*/
	ipg_counter64_t  tx_ok;				/*!<counter (31:0) of a 32-bit counter of successfully transmitted frames*/
	ipg_counter64_t  rx_crc;			/*!<counter (31:0) of a 32-bit counter of received frames with CRC errors*/
#if PORT_VER == PORT_V2
	ipg_counter64_t  tx_crc;			/*!<counter (31:0) of a 32-bit counter of transmitted frames with CRC errors*/
	ipg_counter64_t  rx_octs;			/*!<counter (31:0) of a 32-bit counter of successfully received octets*/
	ipg_counter64_t  tx_octs;			/*!<counter (31:0) of a 32-bit counter of successfully transmitted octets*/
#endif
}__attribute__((packed)) protocol_eth_statistics_t;

typedef struct
{
	union{
		struct bits_generic{
		} bits;
		alt_u32 all;
	};
}generic_alarms_t;

typedef struct{
	generic_alarms_t alarms;		/*!<Instance of the current ETH alarms*/

}__attribute__((packed)) generic_statistics_t;

/**
 * Definition of the Ethernet object control
 */
typedef struct{
	ipg_control32_t 	llcf_enable;	/*!<Specifies a request to enable/disable LLCF feature.*/
}protocol_eth_control_t;
#endif

#if ENABLE_OTUx_CODE
/**
 * Definition of the OTUx object alarms
 */
#ifdef KURS_PROTOCOL
typedef struct
{
	union{
		struct bits_{
			volatile unsigned int	LOS:1;
			volatile unsigned int	LOF:1;
			volatile unsigned int	LOM:1;
			volatile unsigned int	SM_BIP:1;
			volatile unsigned int	SM_IAE:1;
			volatile unsigned int	SM_BEI_BIAE:1;
			volatile unsigned int	SM_BDI:1;
			volatile unsigned int	reserved:25;
		} bits;
		alt_u32 all;
	};
}protocol_otux_alarms_t;
#else
/**
 * Definition of the OTUx object alarms
 */
typedef struct{
	volatile unsigned int BDI:1;
	volatile unsigned int LOM:1;
	volatile unsigned int LOF:1;
	volatile unsigned int SM_IAE:1;
	volatile unsigned int SM_BIP:1;
	volatile unsigned int SM_BEI_BIAE:1;
	volatile unsigned int RESERVED_1:26;
}protocol_otux_alarms_t;
#endif
/**Custom defined type grouping the different alarm statuses of the OTU4 component inside the FPGA.
 * Definition of the OTU statistics object.
 */
typedef struct{
	protocol_otux_alarms_t alarms;
	ipg_counter64_t bip_errors;	/*!<Number of BIP errors detected by the OTU.*/
}__attribute__((packed))protocol_otux_statistics_t;

/**
 * Definition of the OTU control object.
 */
typedef struct{
}protocol_otux_control_t;
#endif

#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
#ifdef KURS_PROTOCOL
typedef struct
{
	union{
		struct stmx_alarms_bits{
			volatile unsigned int LOS:1;
			volatile unsigned int LOF:1;
			volatile unsigned int RS_BIP:1;
			volatile unsigned int RS_TIM:1;
			volatile unsigned int MS_BIP:1;
			volatile unsigned int MS_AIS:1;
			volatile unsigned int MS_RDI:1;
			volatile unsigned int MS_REI:1;
			volatile unsigned int reserved:24;
		} bits;
		alt_u32 all;
	};
}protocol_stmx_alarms_t;
#else
/**
 * Definition of the STMx object alarms
 */
typedef struct{

	volatile unsigned int MS_AIS:1;
	volatile unsigned int MS_RDI:1;
	volatile unsigned int LOF:1;
	volatile unsigned int RS_BIP:1;
	volatile unsigned int RESERVED_1:28;
}protocol_stmx_alarms_t;
#endif
/**Custom defined type grouping the different alarm statuses of the OTU4 component inside the FPGA.
 * Definition of the STM statistics object.
 */
typedef struct{
	protocol_stmx_alarms_t alarms;	/*!<Specifies the current alarms on the STMx object*/
	ipg_counter64_t bip_errors;		/*!<Specifies the number of BIP errors detected by the STMx object.*/
}__attribute__((packed))protocol_stmx_statistics_t;

/**
 * Definition of the STM control object.
 */
typedef struct{
}protocol_stmx_control_t;
#endif

#if ENABLE_FCx_CODE
/**
 * Definition of the FCx statistics object.
 */
typedef struct{
}protocol_fcx_statistics_t;
/**
 * Definition of the FCx control object.
 */
typedef struct{
}protocol_fcx_control_t;
#endif

union protocol_statistics{
	generic_statistics_t			generic_sts;
#if ENABLE_ETH_CODE
		protocol_eth_statistics_t 	eth_sts;	/*!<Specifies an instance of the Ethernet statistics.*/
#endif
#if ENABLE_OTUx_CODE
		protocol_otux_statistics_t	otux_sts;	/*!<Specifies an instance of the OTU statistics.*/
#endif
#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
		protocol_stmx_statistics_t	stmx_sts;	/*!<Specifies an instance of the STM statistics.*/
#endif
#if ENABLE_FCx_CODE
		protocol_fcx_statistics_t	fcx_sts;	/*!<Specifies an instance of the FC statistics.*/
#endif
};

/**
 * Definition of an interface's port monitor object.
 */
typedef struct{
	port_protocol_t 			prot;			/*!<Specifies the protocol of the port. It can be a value of @ref if_data_protocol_t*/
	alt_u32  					rate;			/*!<Specifies the rate of the port. It can be a value of @ref if_data_rate_t*/
	union protocol_statistics	statistics;
#if ENABLE_FEC_CODE
	fec_statistics_t			fec_statistics;
#endif
}port_monitor_t;

union protocol_control{
#if ENABLE_ETH_CODE
		protocol_eth_control_t 		eth_ctrl;
#endif
#if ENABLE_OTUx_CODE
		protocol_otux_control_t		otux_ctrl;
#endif
#if ENABLE_STMx_CODE
		protocol_stmx_control_t		stmx_ctrl;
#endif
#if ENABLE_FCx_CODE
		protocol_fcx_control_t		fcx_ctrl;
#endif
};
/**
 * Definition of the port control object.
 */
typedef struct {
	ipg_control32_t 			rate;			/*!<Specifies the data rate of the port's protocol. It can be a value of @ref if_data_protocol_rate_t*/
	ipg_control32_t				loopback_mode;	/*!<Specifies the loopback mode of the port. It can be a value of @ref loopback_mode_t*/
	union protocol_control protocol_control;
#if ENABLE_FEC_CODE
	fec_control_t	fec_control;
#endif
	ipg_control32_t 	reset_counters;	/*!<Specifies a request to clear all of the port's counters*/
}port_control_t;

typedef struct{
	alt_u32 operation; /*!<Specifies the operation mode of the port. It can be a value of @ref port_operation_mode_t*/
	port_capabilities_t	capabilities;
}port_info_t;

/**
 * Definition of a port object.
 */
typedef struct{
	int					index;			/*!<Specifies the index of the port.*/
	void*				mmap;
	port_info_t 		info;
	port_control_t		control;
	port_monitor_t		monitor;
#if ENABLE_PFC_CODE
	performance_counters_t* pfc;
#endif
#if ENABLE_ALS_CODE
	als_t*					als;
#endif
}port_t;

#if ENABLE_OTUx_CODE
/**Representation of available OTU2 statistics.
 * @deprecated use protocol_otux_statistics_t instead
 */
typedef protocol_otux_statistics_t otu2_statistics_t;
/**Representation of available OTU4 statistics.
 * @deprecated use protocol_otux_statistics_t instead
 */
typedef protocol_otux_statistics_t otu4_statistics_t;
#endif


#if ENABLE_STMx_CODE  || ENABLE_SDHx_CODE
/**Representation of available STM64 statistics.
 * @deprecated use protocol_otux_statistics_t instead
 */
typedef protocol_stmx_statistics_t stm_statistics_t;
#endif
#if ENABLE_ETH_CODE
/**Representation of available XGE statistics.
 * @deprecated use protocol_eth_statistics_t instead
 */
typedef protocol_eth_statistics_t ge_statistics_t;
#endif

void init_port_data(port_t* _port);
void custom_init_port_data(port_t* _port);
void custom_port_mmap(port_t* _port);
void reset_port_counters(port_t* _port);
ret_code_t read_if_port_rate(port_t* _port);
#if ENABLE_FEC_CODE
void reset_port_fec_statistics(port_t* _port);
void update_port_fec_stats(port_t* _port);
ret_code_t set_if_port_fectype(port_t* _port);
ret_code_t get_if_port_fectype(port_t* _port);
ret_code_t set_if_port_rate(port_t* _port);
#endif
void reset_port_statistics(port_t* _port);
ret_code_t set_if_port_loopback(port_t* _port, loopback_mode_t _lb_mode);
#if ENABLE_ETH_CODE
void update_port_ethernet_stats(port_t* _port);
#endif
#if ENABLE_OTUx_CODE
void update_port_otux_stats(port_t* _port);
#endif
#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
void update_port_stmx_stats(port_t* _port);
#endif
#endif /* IPGLB_PORT_H_ */
