#ifndef IPG_TYPES_H_
#define IPG_TYPES_H_

#include <system.h>
#include "../../configs.h"
#include "io.h"
#include "alt_types.h"
#include "ipg_bits.h"
#include "ipg_constants.h"

/**Representation of a 64-bit statistics counter.
 *
 */
typedef struct{
	alt_u64 current;		//!<the current value of the counter.
	alt_u64 accumulated;	//!<the accumulated value of the counter.
}ipg_counter64_t;

/**Representation of a 32-bit statistics counter.
 *
 */
typedef struct{
	alt_u32 current;		//!<the current value of the counter.
	alt_u32 accumulated;	//!<the accumulated value of the counter.
}ipg_counter32_t;

/**List of possible function states of a device.
 *
 */
typedef enum{
	DISABLE = 0,     //!< DISABLE   module disabled.
	ENABLE = !DISABLE//!< ENABLE    module enabled.
}functional_state_t;

/**List of possible states of an GCC message.
 *
 */
typedef enum ipg_gcc_message_state{
	GCC_NOT_AVAILABLE = 0,//!< GCC_NOT_AVAILABLE no valid GCC has arrived.
	GCC_AVAILABLE = 1,    //!< GCC_AVAILABLE     a valid GCC message was received.
}ipg_gcc_message_state_t;

/**Custom boolean type.
 *
 */
typedef enum ipg_bool{
    False = 0,//!< False
    True      //!< True
}ipg_bool_t;

/**Representation of an alarm status.
 *
 */
typedef struct{
   alt_u32 sticky;   //!<history state of the alarm, cleared when read from ARM7.
   alt_u32 previous; //!<previous state of the alarm.
   alt_u32 current;  //!<current state of the alarm.
   alt_u32 change;   //!<change in the state of the alarm.
}ipg_status_t;

/**Representation of a control of type u32
 *
 */
typedef struct{
   alt_u32 next;     //!<next value of the control, generally requested by the ARM7.
   alt_u32 current;  //!<current value of the control.
   alt_u32 change;   //!<change in the state of this control.
}ipg_control32_t;

/**Custom defined type used for a control of type float.
 *
 */
typedef struct{
	float previous;	//!<previous value of the control.
	float current;	//!<current value of the control.
	float next;     //!<change in the state of this control.
	char change;    //!<next value of the control, generally requested by the ARM7.
	alt_u32 time_of_event;     //!<time this event last happened.
}ipg_float_control_t;

typedef struct{
	double previous;	//!<previous value of the control.
	double current;	//!<current value of the control.
	double next;     //!<change in the state of this control.
	char change;    //!<next value of the control, generally requested by the ARM7.
}ipg_double_control_t;

/**List of supported functions return codes.
 *
 */
typedef enum{
	RET_SUCCESS = 0,   //!< RET_SUCCESS			function has succeeded.
	RET_FAIL,          //!< RET_FAIL			function has failed for unknown reason.
	RET_CRC_MISMATCH,  //!< RET_CRC_MISMATCH	function has failed due to a CRC check in the IPGLB protocol packet.
	RET_FUNC_MISMATCH, //!< RET_FUNC_MISMATCH	function has failed due to a \c FUNCTION mismatch in the IPGLB protocol packet.
	RET_CMD_MISMATCH,  //!< RET_CMD_MISMATCH	function has failed due to a \c COMMAND mismatch in the IPGLB protocol packet.
	RET_ARG_MISMATCH,  //!< RET_ARG_MISMATCH	function has failed due to unexpected arguments values in the IPGLB protocol packet.
	RET_TIMEOUT,       //!< RET_TIMEOUT			function has failed due to a timeout in the IPGLB protocol.
	RET_INVALID_PARAMS,//!< RET_INVALID_PARAMS	function has failed due to invalid arguments that were passed to it by the callee.
	RET_UNKNOWN_DEVICE,//!< RET_UNKNOWN_DEVICE	function has failed due to a communication with an unknown device.
	RET_IO_FAIL,  	   //!< RET_IO_FAIL			function has failed due to an IO communication error.
	RET_BUSY , 		   //!< RET_BUSY			function has failed because the device it was trying to communicate with is busy.
    RET_ABSENT, 	//!< RET_ABSENT			function has failed because the device is absent.
	RET_INV_CMD,	//!< RET_INV_CMD			function has failed because the device is not capable of such action.
}ret_code_t;

/**List of possible OM types that can be detected.
 * @deprecated don't use in new projects
 */
typedef enum{
	OM_TYPE_UNKNOWN,              //!< OM_UNKNOWN				unknown module.
	OM_TYPE_ABSENT,               //!< OM_ABS					module is not plugged.
	OM_TYPE_ELECTRICAL_LOOPBACK,  //!< OM_ELECTRICAL_LOOPBACK	electrical loopback board is plugged.
	OM_TYPE_MODULE                //!< OM_MODULE				optical module plugged.
}OM_TYPE_T;

/**List of possible CFP types that can be detected.
 * @deprecated don't use in new projects
 */
typedef enum{
	CFP_TYPE_UNKNOWN = 0,              //!< CFP_UNKNOWN				unknown module.
	CFP_TYPE_ABSENT,               //!< CFP_ABS					module is not plugged.
	CFP_TYPE_MODULE                //!< CFP_MODULE				CFP module plugged.
}CFP_TYPE_T;

/**List of possible FEC encoders/decoders state.
 * @deprecated don't use in new projects
 */
typedef enum{
	FEC_ON = 0,//!< FEC_ON	FEC encoder/decoder ON.
	FEC_OFF    //!< FEC_OFF FEC encoder/decoder OFF.
}fec_enable_t;


/**
 * Definition for a diagnostics object.
 */
typedef struct{
	float	value;			/*!< Specifies the current value of the object.*/
	float	physical_min; 	/*!< Specifies the lowest physical value that can be supported before HW breakdown.*/
	float	physical_max; 	/*!< Specifies the highest physical value that can be supported before HW breakdown.*//*RW - threshold for Critical low boundary*/
	float	critical_min; 	/*!< Specifies the low critical threshold.*/
	float	critical_max;	/*!< Specifies high low critical threshold.*/
	float	warning_min;	/*!< Specifies the low warning threshold.*/
	float	warning_max;	/*!< Specifies the low warning threshold.*/
	ipg_status_t	alarm;	/*!< Specifies if an alarm should be raised.*/
}diagnostics_t;

#if ENABLE_FEC_CODE
/**
 * List of possible FEC types
 */
typedef enum {
	FEC_TYPE_GFEC = 0x00,       //!< FEC_TYPE_GFEC
	FEC_TYPE_HGFEC = 0x01,      //!< FEC_TYPE_HGFEC
	FEC_TYPE_SDFEC_GFEC = 0x02, //!< FEC_TYPE_SDFEC_GFEC
	FEC_TYPE_SDFEC_HGFEC = 0x03,//!< FEC_TYPE_SDFEC_HGFEC
	FEC_TYPE_SDFEC = 0x04,      //!< FEC_TYPE_SDFEC
	FEC_TYPE_NOFEC 	= 0x05,     //!< FEC_TYPE_NOFEC
	FEC_TYPE_RSFEC 	= 0x06,     //!< FEC_TYPE_RSFEC
	FEC_TYPE_EFEC 	= 0x07      //!< FEC_TYPE_EFEC
}fec_type_t;

/**
 * Definition of the statistics reported by a FEC object.
 */
typedef struct {
	ipg_counter64_t corrected_bits;   	/*!<Number of detected corrected bits*/
	ipg_counter64_t uncorrected_cw;     /*!<Number of detected uncorrected codewords*/
	ipg_status_t fec_fail;            	/*!<Specified FEC failure. This alarm is raised when at least one uncorrected codeword is detected*/
	diagnostics_t fec_load;				/*!<Percentage of corrected bits relatively to the maximum number of bits that can be corrected*/
	float prefec_ber;		      		/*!<Network side pre-fec BER*/
	float corrected_ber;		      	/*!<Network side pre-fec BER*/
	float postfec_ber;		      		/*!<Network side post-fec BER*/
	alt_u32 type;          				/*!<Specifies the selected FEC type. This parameter can be a value of @ref FEC_TYPE*/
}__attribute__((packed)) fec_statistics_t;

/**
 * Definition of a FEC object control.
 */
typedef struct {
	ipg_control32_t type;          	/*!<Specifies the selected FEC type. This parameter can be a value of @ref FEC_TYPE*/
}fec_control_t;
#endif

/**List of possible parts of a transceiver.
 *
 */
typedef enum transciever_side{
	TX = 0,//!< TX transmitter part of a transceiver
	RX     //!< RX receiver part of a transceiver
}transceiver_side_t;

/**
 * List of possible loopback configurations.
 */
typedef enum{
	MODE_LB_NORMAL = 0,	/*!< MODE_LB_NORMAL 	port placed in normal operation mode*/
	MODE_LB_CL = 1, 	/*!< MODE_LB_CL			port placed in loop to client mode*/
	MODE_LB_LN = 2, 	/*!< MODE_LB_LN			port placed in loop to line mode*/
}loopback_mode_t;

typedef enum{
	COLOR_OFF = 0,		//!<constant value for OFF
	COLOR_GREEN = 1,	//!<constant value for GREEN
	COLOR_RED = 2,		//!<constant value for RED
	COLOR_YELLOW = 3,	//!<constant value for YELLOW
}led_color_t;

/**Custom defined type representing the LEDs on board.
 *
 */
typedef struct board_leds{
#ifdef KURS_PROTOCOL
	ipg_status_t power_led_g;			//!<front panel power green LED
	ipg_status_t status_led;			//!<front panel status red/yellow LED
	ipg_status_t led_tx[10];			//!<TX LEDs for the 10 available clients
	ipg_status_t led_rx[10];			//!<RX LEDs for the 10 available clients
#else
	ipg_status_t led_g;			//!<front panel green LED
	ipg_status_t led_r;			//!<front panel red LED
	ipg_status_t led_y;			//!<front panel yellow LED
#endif
}board_leds_t;

typedef struct{
	alt_u32 address;
	alt_u32 bit;
}ipglb_io_t;

#ifdef KURS_PROTOCOL
typedef struct{
	volatile unsigned int APR_CAPABLE:1;
	volatile unsigned int APS_CAPABLE:1;
	volatile unsigned int reserved_1:30;
}card_capabilities_t;
#endif

typedef struct{

	void*  		mmap;
	char 		sw_number[SW_NUM_LEN];
	char 		fpga_sw_number[SW_NUM_FPGA_LEN];
#ifdef KURS_PROTOCOL
	char 		hw_number[HW_NUM_LEN];
	char		pid[PID_LEN];
	char		serial_number[SR_NUM_LEN];
	alt_u32		reboot_cnt;
	card_capabilities_t	capabilities;
#endif
#ifdef STM32F207xx
	char 	hw_number[HW_NUM_LEN];
	char 	nios_sw_number[SW_NUM_LEN];
	char	pid[PID_LEN];
	char	serial_number[SR_NUM_LEN];
	uint32_t		uptime;
#endif
}ipglb_system_info_t;

#ifdef STM32F207xx
typedef struct{
	uint32_t    trap_ip_addr[SNMP_TRAP_DESTINATIONS];
	char		community_read[128];
	char 		community_write[128];
	char 		community_trap[128];
	char		version; // SNMPv1 or SNMPv2 (default)
}ipglb_snmp_settings_t;

typedef struct{
	char					dhcp;
	uint32_t 				ip_addr;
	uint32_t 				net_addr;
	uint32_t 				gw_addr;
	ipglb_snmp_settings_t	snmp;
}ipglb_system_net_t;
#endif


/**
 * List of client loopback modes.
 * @deprecated
 */
typedef enum{
	LB_CL_NORMAL = 0,//!< LB_CL_NORMAL 	client placed in normal operation mode
	LB_CL_TO_CL = 1, //!< LB_CL_TO_CL	client placed in loop to client mode
	LB_CL_TO_LN = 2, //!< LB_CL_TO_LN	client placed in loop to line mode
	LB_CL_TO_BOTH = 3    //!< LB_CL_TO_BOTH	both client to client and client to line loopbacks
}client_lb_mode_t;

/**List of line loopback modes
 * @deprecated
 */
typedef enum{
	LB_LN_NORMAL = 0,//!< LB_LN_NORMAL	line placed in normal operation mode
	LB_LN_TO_LN = 1  //!< LB_LN_TO_LN	line placed in loop to line mode
}line_lb_mode_t;

#ifdef KURS_PROTOCOL
typedef enum{
	ACTIVITY_MONITOR_OFF = 0,
	ACTIVITY_MONITOR_ON = 1,
}activity_monitor_t;

enum IF_SERVICE_STATE{
	IF_SERVICE_STATE_OOS = 0, 	/*!< Out of Service*/
	IF_SERVICE_STATE_MT = 1,	/*!< In maintenance*/
	IF_SERVICE_STATE_IS = 2,						/*!< In Service*/
};

/**
 * List of supported threshold modes.
 *
 */
typedef enum{
	THRESHOLD_TRANSCEIVER = 0,
	THRESHOLD_USER = 1
}threshold_mode_t;

#endif

typedef ipg_counter32_t ipg_counter_t;
typedef ipg_control32_t ipg_control_t;

#endif /*IPG_TYPES_H_*/
