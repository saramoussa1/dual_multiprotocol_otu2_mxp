/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) 2015 IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *
 * IPG Photonics Corporation
 * 50 Old Webster Road
 * Oxford, MA 01540, USA
 ****************************************************************************/

/**
 * @file main.c
 * @brief This file contains the functions implementations for our application.
 *
 */
#include "main.h"
#include "common/inc/ipglb_transceiver.h"
#include "common/inc/ipglb_port.h"
#include "common/inc/ipglb_interface.h"
#include "common/inc/ipglb_card.h"
#include "common/inc/ipg_pfc.h"
#include "common/inc/ipg_si5375.h"
#include "mmap.h"
#include "common/inc/ipg_sfp.h"
#include "common/inc/ipg_uart.h"
#include <system.h>
#include "altera_avalon_cfi_flash.h"
#include "cfi_flash.h"
#ifdef KURS_PROTOCOL
#include "Middlewares/KURS/inc/kurs.h"
#include "Middlewares/KURS/inc/kurs_core.h"
#include "Middlewares/KURS/inc/kurs_remote_update.h"
#include "profile.h"
#include "ipglb_adapter.h"
#include "kurs_constants.h"
#include "configs.h"
#include "kurs_sections.h"
#else
#include "common/inc/user_settings.h"
#endif
#include "ring_buffer.h"
#include "Middlewares/KURS/inc/S29GLxxxP.h"
#include "mux_otu1_types.h"
#include "cfi_flash.h"
#include "common/inc/ipg_als.h"
#include "ipg_aps.h"
#include "logger.h"
#include "Middlewares/KURS/inc/kurs_processor.h"
#include "common/inc/ipglb_prot2_processor.h"
#include "common/inc/ipg_uart.h"
#include "altera_avalon_cfi_flash.h"
#include "cfi_flash.h"
#include "altera_avalon_timer_regs.h"
#include "scheduler.h"
#include"ipglb_otu2_brick_dual.h"
/**
 * macros
 */
/**Executes a reset of the processor
 *
 */
#define SW_RESET() \
  NIOS2_WRITE_STATUS(0); \
  NIOS2_WRITE_IENABLE(0); \
  ((void (*) (void)) NIOS2_RESET_ADDR) ()
/*!<Releases the free pin to indicate to the STM32F207 that NiOS is ready to
 *  receive requests */
#define SET_NIOS_FREE_PIN()	(top.reg1_config->bits.FPGA_INT1 = 1)
/*!<Sets the free pin to busy to indicate to the STM32F207 that NiOS is not yet
 *  ready to receive requests */
#define SET_NIOS_BUSY_PIN()	(top.reg1_config->bits.FPGA_INT1 = 0)
/**
 * defines
 */
/*!< Constant for upper threshold of PPM difference */
#define PPM_POSITIVE_DIFF	7
/*!< Constant for lower threshold of PPM difference */
#define PPM_NEGATIVE_DIFF	PPM_POSITIVE_DIFF
#define _COM		0
#define ARM1_COM	1
#ifdef CHASSIS_B8
#define ARM2_COM	2
#endif

#define IORD_32DIRECT_ORBIT31   IORD_32DIRECT(0x1000000,4)|BIT31)
#define IORD_32DIRECT_ANDBIT31  IORD_32DIRECT(0x1000000,4)&~BIT31)
#define HEARTBEAT_LED_ON()(IOWR_32DIRECT(0x1000000, 4, IORD_32DIRECT_ORBIT31)
#define HEARTBEAT_LED_OFF()(IOWR_32DIRECT(0x1000000, 4, IORD_32DIRECT_ANDBIT31)

#ifdef KURS_PROTOCOL
/*!<default value for serial number of device */
#define DEF_SR_NUM					"14-100\0"
/*!<default value for PID */
#define DEF_PID						"AGG-2x2,5BS-SFP/SFP-H4\0"
/*!<default value for HW revision */
#define DEF_HW_NUM					"CEBIUCX0727xxxxx\0"
/*!<default HMin threshold for FEC load parameter */
#define DEF_FEC_LOAD_HMIN         0
/*!<default CMin threshold for FEC load parameter */
#define DEF_FEC_LOAD_CMIN         0
/*!<default WMin threshold for FEC load parameter */
#define DEF_FEC_LOAD_WMIN         0
/*!<default WMax threshold for FEC load parameter */
#define DEF_FEC_LOAD_WMAX         50
/*!<default CMax threshold for FEC load parameter */
#define DEF_FEC_LOAD_CMAX         75
/*!<default HMax threshold for FEC load parameter */
#define DEF_FEC_LOAD_HMAX         80
#endif

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#define ALS(x) card.interfaces[x].port.als
#define	INTERFACE(x) card.interfaces[x]
#define	PORT(x)	card.interfaces[x].port
#define	PORT_CAP(x) PORT(x).info.capabilities
#define	PORT_MONITOR(x)	card.interfaces[x].port.monitor
#define	PORT_CTRL(x) card.interfaces[x].port.control
#define	PORT_CTRL_LB(x) card.interfaces[x].port.control.loopback_mode
#define	TRANS(x) card.interfaces[x].transceiver
#define	TRANS_device(x) card.interfaces[x].transceiver.device
#define	TRANS_CTRL(x) card.interfaces[x].transceiver.control
#define	TRANS_CTRLTXDIS(x) card.interfaces[x].transceiver.control.tx_disable
#define	TRANS_DEVICETXDIS(x) card.interfaces[x].transceiver.device.tx_disable
#define	TRANS_MONITOR(x) card.interfaces[x].transceiver.monitor
#define	TRANS_MONITOR_INPWR(x)  TRANS_MONITOR(x).input_power
#define	TRANS_MONITOR_OUTPWR(x) TRANS_MONITOR(x).output_power
#define	TRANS_MONITORTEMP(x) card.interfaces[x].transceiver.monitor.temperature
#define IS_LINE(index) (PORT(index).info.operation == PORT_OPERATION_LINE)
#define IS_CLIENT(index) (PORT(index).info.operation == PORT_OPERATION_CLIENT)
#define CL_RATE(index) PORT_MONITOR(index).rate
#define IS_CLIENT_OTU1(index) PORT_MONITOR(index).rate == PORT_RATE_OTU1
#define ISNOTCLIENT_OTU1(index) (PORT_MONITOR(index).rate != PORT_RATE_OTU1)
#define ISNOTCLIENT_STM1(index) (PORT_MONITOR(index).rate != PORT_RATE_STM1)
#define ISNOTCLIENT_STM4(index) (PORT_MONITOR(index).rate != PORT_RATE_STM4)
#define ISCLIENT_STM16(index) (PORT_MONITOR(index).rate == PORT_RATE_STM16)
#define ISNOTCLIENT_STM16(index) (PORT_MONITOR(index).rate != PORT_RATE_STM16)
#define ISNOTCLIENT_GE(index) (PORT_MONITOR(index).rate != PORT_RATE_GE)
#define ISNOTCLIENT_FE(index) (PORT_MONITOR(index).rate != PORT_RATE_FE)
#define ISCLIENT_FC2(index) (PORT_MONITOR(index).rate == PORT_RATE_FC2)
#define ISALS_REMOTECTRL(index) (ALS(if_index)->control == ALS_CONTROL_REMOTE)
#define APS_EN		0
#define POUT_CMAX(x) pout[x].critical_max
#define POUT_CMIN(x) pout[x].critical_min
#define PIN_CMAX(x) pin[x].critical_max
#define PIN_CMIN(x) pin[x].critical_min
#define PIN_WMAX(x) pin[x].warning_max
#define PIN_WMIN(x) pin[x].warning_min
#define POUT_WMAX(x) pout[x].warning_max
#define POUT_WMIN(x) pout[x].warning_min
#define PIN_PMAX(x) pin[x].physical_max
#define PIN_PMIN(x) pin[x].physical_min
#define POUT_PMAX(x) pout[x].physical_max
#define POUT_PMIN(x) pout[x].physical_min
#define C_FECLOAD(x) PORT_MONITOR(x).fec_statistics.fec_load
#define C_RXOK(x) PORT_MONITOR(x).statistics.eth_sts.rx_ok
#define C_TXOK(x) PORT_MONITOR(x).statistics.eth_sts.tx_ok
#define C_RXOCTS(x) PORT_MONITOR(x).statistics.eth_sts.rx_octs
#define C_TXOCTS(x) PORT_MONITOR(x).statistics.eth_sts.tx_octs
#define C_RXCRC(x)  PORT_MONITOR(x).statistics.eth_sts.rx_crc
#define C_OTUXSTS(x) card.interfaces[x].port.monitor.statistics.otux_sts.alarms
#define C_STMXSTS(x) card.interfaces[x].port.monitor.statistics.stmx_sts.alarms
#define C_ETHALARMS(x)  card.interfaces[x].port.monitor.statistics.eth_sts.alarms

#define ALT_TICKS_5SEC	(5000) 	/*!< based on a 1000 ticks per second Hardware*/
#define ALT_TICKS_2SEC	(2000) 	/*!< based on a 1000 ticks per second Hardware*/
#define ALT_TICKS_1SEC	(1000) 	/*!< based on a 1000 ticks per second Hardware*/
#define ALT_TICKS_125MS	(125) 	/*!< based on a 1000 ticks per second Hardware*/
#define ALT_TICKS_100MS	(100) 	/*!< based on a 1000 ticks per second Hardware*/
#define ALT_TICKS_10MS	(10) 	/*!< based on a 1000 ticks per second Hardware*/

#define CL_SFP(x)	((sfp_device_t*)(TRANS(x).device.device))
#define LN_MMAP(x)	((line_mmap_t*)(PORT(x+16).mmap))
#define CL_MMAP(x)	((client_mmap_t*)(PORT(x).mmap))
#define FPGA1 (if_index < 8)
#define FPGA2 (if_index > 7 && if_index < 16 )
#define PORT_FPGA1 (_port_ptr->index < 8)
#define PORT_FPGA2 (_port_ptr->index > 7 && _port_ptr->index < 16)
/**
 * local variables
 */
static alt_alarm timer_systick; /*!< timer used for system ticks */
alt_u32 ticks_per_100ms = 0;
alt_u32 ticks_per_1ms = 0;
alt_u32 ticks_per_300ms = 0;
alt_u32	ticks_per_500ms = 0;
alt_u32 systicks = 0;
alt_u32 kurs_ticks = 0;
alt_u32 arm_ticks = 0;
alt_u32	dvt_ticks = 0;
alt_u8 nios_fw_rev[SW_NUM_LEN];
int system_uptime_s = 0; /*!< in seconds, how long has the system been up */
char enable_gmp_fifo_adjustment[CLIENTS_IF_COUNT];
alt_u8 flag_1ms_event = 0;
alt_u8 flag_5ms_event = 0;
alt_u8 flag_10ms_event = 0;
alt_u8 flag_50ms_event = 0;
alt_u8 flag_100ms_event = 0;
alt_u8 flag_125ms_event = 0;
alt_u8 flag_300ms_event = 0;
alt_u8 flag_500ms_event = 0;
alt_u8 flag_1000ms_event = 0;
alt_u8 flag_2000ms_event = 0;
alt_u8 flag_5000ms_event = 0;
alt_u8 selected_if = 0;
ipg_status_t om_voltage_fault;
uart_registers_t dvt_uart; /*!< instance of UART registers */
uart_registers_t arm_uart; /*!<ARM7 (KURS) instance of UART registers */
/*FPGA 1*/
i2c_bus_t xi2c_bus; /*!<instance of the I2C bus */
i2c_bus_t yi2c_bus; /*!<instance of the I2C bus */
i2c_bus_t ln0_i2c_bus2; /*!<instance of the I2C bus */
i2c_bus_t ln1_i2c_bus3; /*!<instance of the I2C bus */
i2c_bus_t i2c_bus4; /*!<instance of the I2C bus */

/*FPGA 2*/
i2c_bus_t rem_xi2c_bus; /*!<instance of the I2C bus */
i2c_bus_t rem_yi2c_bus; /*!<instance of the I2C bus */
i2c_bus_t rem_ln0_i2c_bus2; /*!<instance of the I2C bus */
i2c_bus_t rem_ln1_i2c_bus3; /*!<instance of the I2C bus */
i2c_bus_t rem_i2c_bus4; /*!<instance of the I2C bus */


si5375_device_t si5375[6]; /*!<array of the SI5338 devices */
si5338_device_t si5338[6]; /*!<array of the SI5338 devices */
mcp9808_device_t mcp9808[4]; /*!<instance of the  UART registers */
sfpp_device_t* _sfpp_ln;
pca9547_device_t i2c_mux; /*!<instance of a PCA9547 device */
pca9547_device_t rem_i2c_mux; /*!<instance of a remote PCA9547 device */
//pca9547_device_t i2c_mux2;		/*!<instance of a PCA9547 device */
alt_u32 steps_reg = 20 | (40 << 16);
alt_u8 cl_ppm_id = 0;
alt_u8 remote_override = 0;
alt_u8 link_partner_address = 0;
/*{which_silab, which_clk_output}, */
int client_to_silab[10][2] =
{
{ 0, 3 }, { 0, 2 }, { 0, 1 }, { 0, 0 }, { 1, 3 }, { 1, 2 }, { 1, 0 }, { 1, 1 },
{ 2, 0 }, { 2, 1 },
};

alt_u8 line_lb_detected = 0;
ipglb_card_t card;
rbd_t rb_data_to_kurs_uart;
uint16_t data_to_kurs_uart_buff[4096]; /* allocated memory for ring buffer */
rb_attr_t rbattr_data_to_kurs_uart;
KURS_processor_t* kurs_req_processor;
IPGLB_PROT2_processor_t* arm_processor;
IPGLB_PROT2_processor_t* dvt_processor;

alt_alarm heartbeat_alarm;
alt_alarm alarm_10ms;
alt_alarm alarm_100ms;
alt_alarm alarm_125ms;
alt_alarm alarm_1000ms;
alt_alarm alarm_2000ms;
extern char save;
char reboot_nios = 0;
#ifdef APS_EN
aps_module_t _aps;
#endif
//int brick_index = 0;
int if_index = 0;

#ifdef TEST
char slot_id = 3;
#else
#warning 'Slot ID should be read from ARM'
alt_u8 slot_id = 0;
#endif
alt_u32 temp_mem_address;
extern KURS_ProfileTypeDef profile;
diagnostics_t pin[INTERFACE_COUNT];
diagnostics_t pout[INTERFACE_COUNT];
board_leds_t board_leds;
alt_alarm kurs_handler_alarm;
alt_alarm dvt_handler_alarm;
alt_alarm arm_handler_alarm;
char restore = 0;
ipg_bool_t ppm_enabled[INTERFACE_COUNT] = {True, True, True, True, True, True, True, True,
										   True, True, True, True, True, True, True, True,
										   True, True};

char pt_cl_1_in_info[PORT_INFO_SIZE]; /*!<info string for port 1 in */
uint32_t pt_cl_1_in_cat = 0; /*!<cat index for   port 1 in */
char pt_cl_2_in_info[PORT_INFO_SIZE]; /*!<info string for port 2 in */
uint32_t pt_cl_2_in_cat = 0; /*!<cat index for   port 2 in */
char pt_cl_3_in_info[PORT_INFO_SIZE]; /*!<info string for port 3 in */
uint32_t pt_cl_3_in_cat = 0; /*!<cat index for   port 3 in */
char pt_cl_4_in_info[PORT_INFO_SIZE]; /*!<info string for port 4 in */
uint32_t pt_cl_4_in_cat = 0; /*!<cat index for   port 4 in */
char pt_cl_5_in_info[PORT_INFO_SIZE]; /*!<info string for port 5 in */
uint32_t pt_cl_5_in_cat = 0; /*!<cat index for   port 5 in */
char pt_cl_6_in_info[PORT_INFO_SIZE]; /*!<info string for port 6 in */
uint32_t pt_cl_6_in_cat = 0; /*!<cat index for   port 6 in */
char pt_cl_7_in_info[PORT_INFO_SIZE]; /*!<info string for port 7 in */
uint32_t pt_cl_7_in_cat = 0; /*!<cat index for   port 7 in */
char pt_cl_8_in_info[PORT_INFO_SIZE]; /*!<info string for port 8 in */
uint32_t pt_cl_8_in_cat = 0; /*!<cat index for   port 8 in */
char pt_cl_9_in_info[PORT_INFO_SIZE]; /*!<info string for port 9 in */
uint32_t pt_cl_9_in_cat = 0; /*!<cat index for   port 9 in */
char pt_cl_10_in_info[PORT_INFO_SIZE]; /*!<info string for port 10 in */
uint32_t pt_cl_10_in_cat = 0; /*!<cat index for   port 10 in */
char pt_cl_11_in_info[PORT_INFO_SIZE]; /*!<info string for port 11 in */
uint32_t pt_cl_11_in_cat = 0; /*!<cat index for   port 11 in */
char pt_cl_12_in_info[PORT_INFO_SIZE]; /*!<info string for port 12 in */
uint32_t pt_cl_12_in_cat = 0; /*!<cat index for   port 12 in */
char pt_cl_13_in_info[PORT_INFO_SIZE]; /*!<info string for port 13 in */
uint32_t pt_cl_13_in_cat = 0; /*!<cat index for   port 13 in */
char pt_cl_14_in_info[PORT_INFO_SIZE]; /*!<info string for port 14 in */
uint32_t pt_cl_14_in_cat = 0; /*!<cat index for   port 14 in */
char pt_cl_15_in_info[PORT_INFO_SIZE]; /*!<info string for port 15 in */
uint32_t pt_cl_15_in_cat = 0; /*!<cat index for   port 15 in */
char pt_cl_16_in_info[PORT_INFO_SIZE]; /*!<info string for port 16 in */
uint32_t pt_cl_16_in_cat = 0; /*!<cat index for   port 16 in */
char pt_ln_1_in_info[PORT_INFO_SIZE]; /*!<info string for line in */
uint32_t pt_ln_1_in_cat = 0; /*!<cat index for   line in */
char pt_ln_2_in_info[PORT_INFO_SIZE]; /*!<info string for line in */
uint32_t pt_ln_2_in_cat = 0; /*!<cat index for   line in */
/*/////////////////////////////////////////////////////////////////////////// */
/*///////////Section above added for handling PUSK profile 5///////////////// */
/*/////////////////////////////////////////////////////////////////////////// */
uint16_t request_to_arm = 0;
int flagTest1 = 0;
int flagTest2 = 0;
int i = 0;
int port_rate_change_flag[CLIENTS_IF_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0 };
logger_t _logger;
alt_u32 counter_ores_arm = 0;
alt_u32 counter_ores_dvt = 0;
alt_u32 counter_tmr0_run_bit = 0;
alt_u32 temp;
s_task_handle_t task_2000_hndl;
s_task_handle_t task_100_hndl;
s_task_handle_t task_55_hndl;
s_task_handle_t task_10_hndl;
/**
 * local prototypes
 */
void create_pfc_module(port_t* _port_ptr);
void destroy_pfc_module(port_t* _port_ptr);
void switch_to_ge(port_t* _port_ptr);
void switch_to_sonet(port_t* _port_ptr);
void switch_to_otu1(port_t* _port_ptr);
void switch_to_otu2(port_t* _port_ptr);
void switch_to_fc1(port_t* _port_ptr);
void switch_to_fc2(port_t* _port_ptr);
void configure_board_clocks(void);
void run_boot_sequence(void);
void register_mapping(void);
void task_ppm_adjustment(void);
#ifndef KURS_PROTOCOL
ret_code_t handle_if_port_pfc(alt_u8 COM, alt_u8 *request, alt_u8 *reply,
							  int *reply_ptr);
#endif
void set_build_number(int major, int minor, int build);
void read_fpga_number(void);
void handle_debug_packet(void);
#if ENABLE_ARM7_REQUESTS
void handle_kurs_packet(void);
#endif
void inituart(void);
void dump_statistics(void);
ret_code_t get_slot_id(alt_u8* _slot_id);
ret_code_t set_status_led(void);
ret_code_t get_fp_leds(void);
alt_u32 systick_isr(void* context);
alt_u32 heartbeat(void*);
#ifdef PPM_ADJUSTMENT_ENABLED
#ifdef PPM_ADJUSTMENT_SW
void do_gcc_task(void);
void monitor_ppm_clksts();
void handle_ppm (int which_brick , alt_u16 far_end_rx_frequency);
void monitor_gmp_aligner_of(void);
#endif
#endif
void enable_all_transceivers_tx(void);
void disable_all_transceivers_tx(void);
void unreset_i2c_mux(int i2c_mux_index);
/*FPGA 1*/
void select_channel_on_bus_x(uint32_t i2c_channel);
void select_channel_on_bus_y(uint32_t i2c_channel);
void select_channel_on_bus_ln0(uint32_t i2c_channel);
void select_channel_on_bus_ln1(uint32_t i2c_channel);
void select_channel_on_bus4(uint32_t i2c_channel, pca9547_device_t* i2c_mux_ptr, port_t* _port);

/*FPGA 2*/
void select_channel_on_rem_bus_x(uint32_t i2c_channel);
void select_channel_on_rem_bus_y(uint32_t i2c_channel);
void select_channel_on_rem_bus_ln0(uint32_t i2c_channel);
void select_channel_on_rem_bus_ln1(uint32_t i2c_channel);
void select_channel_on_rem_bus4(uint32_t i2c_channel, pca9547_device_t* i2c_mux_ptr, port_t* _port);

void assign_function_handlers(void);
void build_i2c_tree(void);
void update_if_dynamic_data(interface_t* _if_ptr);
void cancel_request(ipg_control32_t* req_ptr);
void dump_interface_monitor(interface_t* _if_ptr);
void put_flash_in_rst(void);
void adjust_ppm(void);
void reconfigure_ge_port(port_t* _port_ptr);
#ifdef KURS_PROTOCOL
void get_board_temperature();
KURS_port_rate_t map_rate_to_kurs_value(port_rate_t _ipglb_rate);
port_rate_t map_rate_from_kurs_value(KURS_port_rate_t _kurs_rate);
uint32_t map_als_from_kurs_value(als_mode_t als_mode);
#endif
void aps_task(aps_module_t* _aps_ptr);
void send_9bit_uart_packet(uart_registers_t* uart, alt_u8* packet, alt_u32 _len,
						   alt_u8 tag);
alt_u32 isr_10ms(void* context);
alt_u32 isr_100ms(void* context);
alt_u32 isr_125ms(void* context);
alt_u32 isr_1s(void* context);
alt_u32 isr_2s(void* context);
alt_u32 isr_5s(void* context);
void timer_kick(void);
void timer_start(void);
void timer_stop(void);

/*scheduler*/
void task_2000(s_task_handle_t me, s_task_msg_t** msg);
void task_100(s_task_handle_t me, s_task_msg_t** msg);
void task_55(s_task_handle_t me, s_task_msg_t** msg);
void task_10(s_task_handle_t me, s_task_msg_t** msg);

/*/////////////////////////////////////////////////////////////////////////// */
/*///////////Section below added for handling PUSK profile 5///////////////// */
/*/////////////////////////////////////////////////////////////////////////// */
extern void commit_user_settings();
extern ret_code_t read_user_settings(void);
extern int append_preference(void* data, size_t size);
void build_preferences_list(void);
extern void update_alarms(void);
extern void update_card_alarm1(void);
extern void update_card_alarm2(void);
extern uint32_t block_color;
void update_if_kurs_power(interface_t* _if_ptr);
/*char get_kurs_address(); */
alt_u32 kurs_handler(void*);
alt_u32 arm_handler(void*);
alt_u32 dvt_handler(void* context);
void load_new_image_if_valid();
void reset_nios();
void jump_to_boot_copier(void target(void));


/*void queue_data_from_arm(uint16_t d); */
void queue_data_to_arm(uint16_t d);
#if ENABLE_ALS_CODE
void init_als_module(interface_t* _if);
#endif
void report_local_los_to_fe(uint32_t br_idx, uint32_t enable);
/*rbd_t rbDataFromArmUart; */
/*uint16_t dataFromArmUart_buff[4096];// allocated memory for ring buffer */
/*rb_attr_t rbattr_dataFromArmUart; */

/**
 * Program's entry point
 */
int main()
{

	bool ret;
	printf("Starting HORIZON Multiprotocol dual otu2 firmware \n");
	memset(enable_gmp_fifo_adjustment, 0, sizeof(enable_gmp_fifo_adjustment));
	ticks_per_1ms = alt_ticks_per_second() / 1000;
	ticks_per_100ms = ticks_per_1ms * 100;
	ticks_per_300ms = ticks_per_100ms * 3;
	ticks_per_500ms = ticks_per_100ms * 5;
	kurs_ticks = ticks_per_1ms * 15;
	arm_ticks = ticks_per_1ms;
	dvt_ticks = 50 * ticks_per_1ms;

	ipglb_io_t cfi_reset = { 0x1000004, BIT24 };
	ipglb_io_t cfi_wpn = { 0x1000004, BIT25 };
	/* register mapping */
	printf("mapping registers\n");
	register_mapping();
	cfi_flash_init(cfi_reset, cfi_wpn);
	printf("starting heartbeat\n");
	alt_alarm_start(&alarm_10ms, ALT_TICKS_10MS, isr_10ms, NULL);
	alt_alarm_start(&alarm_100ms, ALT_TICKS_100MS, isr_100ms, NULL);
	alt_alarm_start(&alarm_125ms, ALT_TICKS_125MS, isr_125ms, NULL);
	alt_alarm_start(&alarm_1000ms, ALT_TICKS_1SEC, isr_1s, NULL);
	alt_alarm_start(&alarm_2000ms, ALT_TICKS_2SEC, isr_2s, NULL);

	/* initialize data structures; */
	printf("Initializing data structures\n");
	/*Brick APS*/
#if APS_EN
	_aps.index = 0; //brick_index;
	aps_init(&_aps);
#endif
	for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
	{
		INTERFACE(if_index).index = if_index;
		init_interface_data(&INTERFACE(if_index));
		printf("interface %d done...\n", if_index);
#if ENABLE_ALS_CODE
		if(PORT_CAP(if_index).ALS_CAPABLE)
		{
			init_als_module(&INTERFACE(if_index));
			printf("ALS for interface %d ready ...\n", if_index);
		}
#endif
	}
	/* choose which interval we want to use as system ticks interval
	 * (lowest interrupt time) */
	systicks = ticks_per_1ms;

	assign_function_handlers();
	/*printf("Function handles assigned\n"); */

	/*printf("registers mapped correctly \n"); */
	build_i2c_tree();

	/*restore user preferences*/
	build_preferences_list();
	/*printf("Preferences list built\n"); */

#ifdef KURS_PROTOCOL
	/*restore language file*/
	KURS_ReadLngFile(&profile);
#endif
	unselect_all_pca9547_channels(&i2c_mux);
	unselect_all_pca9547_channels(&rem_i2c_mux);
	printf("Starting Boot sequence ...\n");
	run_boot_sequence();
	printf("Boot sequence done\n");

	inituart();

#ifdef ENABLE_PERIODIC_TASKS
	alt_alarm_start(&timer_systick, ticks_per_1ms, systick_isr, NULL);
#endif

	/* allow debug UART to send requests */
#if ENABLE_DVT_CODE
	printf("Enabling DVT UART RRDY interrupts\n");
	struct IPGLB_PROT2_processor_attributes _dvt_attr = { &dvt_uart,
														  IPGLB_PROT2_SLAVE, };
	dvt_processor = ipglb_prot2_processor_init(_dvt_attr);
	ipg_uart_enable_IRRDY(&dvt_uart);
	ipg_uart_enable_IROE(&dvt_uart);
#endif
	/* wait */
	Delay_ms(1000);

	/*
	 * ARM UART peripheral uses a ring buffer
	 * to store data bytes to be transmitted.
	 * This buffer is initialized here.
	 */
	rbattr_data_to_kurs_uart.s_elem = sizeof(data_to_kurs_uart_buff[0]);
	rbattr_data_to_kurs_uart.n_elem = ARRAY_SIZE(data_to_kurs_uart_buff);
	rbattr_data_to_kurs_uart.buffer = data_to_kurs_uart_buff;
	if(ring_buffer_init(&rb_data_to_kurs_uart, &rbattr_data_to_kurs_uart) !=
		RET_SUCCESS)
	{
		/* TODO:handle repeat */
		int dummy = 0;
		dummy = dummy * 2;
	}

	/*
	 * Initialize the IPGLB PROT2 processor.
	 * It will handle IPGLB2 communication between
	 * NiOS and ARM.
	 */
#if ENABLE_ARM7_REQUESTS
	struct IPGLB_PROT2_processor_attributes _arm_attr = {&arm_uart,
														 IPGLB_PROT2_MASTER, };
	arm_processor = ipglb_prot2_processor_init(_arm_attr);
	printf("Enabling ARM UART RRDY interrupts\n");
	ipg_uart_enable_IRRDY(&arm_uart);
	ipg_uart_enable_IROE(&arm_uart);
#endif

	/*COMM between NiOS and ARM should be up let us read our KURS address*/
	while(get_slot_id(&slot_id) != RET_SUCCESS)
	{
		Delay_ms(1000); /*wait one second before trying again*/
	}
	slot_id |= DEF_KURS_ADDRESS;
	/*
	 * Initialize the KURS processor.
	 * It will handle KURS traffic between
	 * NiOS and CU bridged by the ARM.
	 */
	struct KURS_processor_attributes _kurs_attr = { &arm_uart, slot_id, };
	kurs_req_processor = kurs_processor_init(_kurs_attr);
	if(kurs_req_processor == NULL)
	{
		/*TODO handle KURS processor creation error*/
	}
#ifdef KURS_PROTOCOL
	profile.addr_bus = DEF_KURS_ADDRESS | slot_id;
	profile.Init(&profile);
	/*printf("KURS profile ready\n"); */
	/*Read latest user settings */
	if(read_user_settings() == RET_SUCCESS)
	{
		printf("User preferences found\n");
	}
#if 1
	/*Apply latest settings by the user */
	int ind = 0;
	printf("restoring user settings\n");
	for(ind = 0; ind < INTERFACE_COUNT; ind++)
	{
		/*restore port mode OOS, IS ...*/
		INTERFACE(ind).service_state.next =
		INTERFACE(ind).service_state.current;
		INTERFACE(ind).service_state.change = 1;
		/*restore port ALS mode*/
		ALS(ind)->set_mode(ALS(ind), PORT(ind).als->control);
		/*restore client port rate*/
		if(IS_CLIENT(ind))
		{
			/*clients are multirate*/
			PORT_CTRL(ind).rate.next = PORT_CTRL(ind).rate.current;
			/*#waning check this condition*/
			/*if(((ind == CL_1_IF_INDEX) || (ind == CL_3_IF_INDEX)) &&
			   ((PORT_CTRL(ind-1).rate.next == PORT_RATE_OTU1) ||
			   (PORT_CTRL(ind-1).rate.next  == PORT_RATE_STM16) ||
			   (PORT_CTRL(ind-1).rate.next  == PORT_RATE_FC2)))
			/**/
			if(((PORT_CTRL(ind).rate.next == PORT_RATE_OTU1)
				||(PORT_CTRL(ind).rate.next  == PORT_RATE_STM16)
				||(PORT_CTRL(ind).rate.next  == PORT_RATE_FC2)))
			{
				PORT_CTRL(ind).rate.change = 0;
			}
			else
			{
				PORT_CTRL(ind).rate.change = 1;
			}
			create_pfc_module(&PORT(ind));
		}
		if(IS_LINE(ind))
		{
			/*Lines are fixed rates OTU2 and support PFC*/
			create_pfc_module(&PORT(ind));
		}
		/*Only reset the counters on startup. */
		PORT_CTRL(ind).reset_counters.change = 1;
		/*Restore Loopback mode */
		PORT_CTRL(ind).loopback_mode.next = PORT_CTRL(ind).loopback_mode.current;
		PORT_CTRL(ind).loopback_mode.change = 1;
		/*Restore Tx Enable */
		if(INTERFACE(ind).service_state.next == IF_SERVICE_STATE_OOS)
		{
			/*force TX_DIS in csae port is in OOS*/
			TRANS_CTRL(ind).tx_disable.next =
			TRANS_CTRL(ind).tx_disable.current = 1;
			TRANS_CTRL(ind).tx_disable.change = 1;
		}
		else
		{
			TRANS_CTRL(ind).tx_disable.next = TRANS_CTRL(ind).tx_disable.current;
			TRANS_CTRL(ind).tx_disable.change = 1;
		}
	}
#if APS_EN
	/*restore APS mode*/
	_aps.aps_mode.change = 1;
	_aps.aps_protectch.change = 1;
	_aps.revertive_mode.change = 1;
	aps_set_wtr(&_aps);
	aps_set_ho(&_aps);
#endif
	read_fpga_number();
	printf("FPGA revision %s\n", card.info.fpga_sw_number);
	set_build_number(0, 0, 2);
	printf("Software revision %s\n", card.info.sw_number);

#endif
	/* now we are ready to receive ARM requests */

#ifdef KURS_PROTOCOL
	alt_alarm_start(&kurs_handler_alarm, kurs_ticks, kurs_handler, NULL);
	/*	alt_alarm_start(&dvt_handler_alarm, dvt_ticks, dvt_handler, NULL);
	 alt_alarm_start(&arm_handler_alarm, arm_ticks, arm_handler, NULL);*/
#endif
	/*Scheduler: */
	ret = scheduler_init(alt_nticks); /*Initialize scheduler */
	if(ret != 0)
	{
		ret &= s_task_create(1, S_TASK_HIGH_PRIORITY, 2000, task_2000,
							 &task_2000_hndl);
		ret &= s_task_create(1, S_TASK_HIGH_PRIORITY, 100, task_100,
							 &task_100_hndl);
		ret &= s_task_create(1, S_TASK_HIGH_PRIORITY, 55, task_55,
							 &task_55_hndl);
		ret &= s_task_create(1, S_TASK_HIGH_PRIORITY, 10, task_10,
							 &task_10_hndl);
		if(ret == 0)
		{
			printf("At least one task failed\n\r");
		}
		else
		{
			printf("sTasks running...\n\r");
		}

		/* endless loop */
		while (1)
		{
			timer_kick(); /*restart the timer in case it stopped unexpectedly*/

#if ENABLE_DVT_CODE
			dvt_handler(NULL);
#endif
			/*		HandleArm1Packet(); */
			if(restore == 1)
			{
				profile.SetDefault(0x3FFFFFF);
				int rest_ind = 0;
				for(rest_ind = 0; rest_ind < INTERFACE_COUNT; rest_ind++)
				{
					INTERFACE(rest_ind).service_state.change  = 1;
					PORT_CTRL(rest_ind).rate.change           = 1;
					TRANS_CTRL(rest_ind).tx_disable.change    = 1;
					PORT_CTRL(rest_ind).loopback_mode.change  = 1;
					PORT_CTRL(rest_ind).reset_counters.change = 1;
				}
				restore = 0;
			}
			if(reboot_nios == 1)
			{
				void *flash_image_ptr_2 = (void*) BOOT_IMAGE_2_ADDR;
				if(ValidateFlashImage(
						(alt_u32 *) (flash_image_ptr_2)) == CRCS_VALID)
				{
					/*Remote image is valid */
					/*Force FPGA reset */
					reset_nios();
				}
				reboot_nios = 0;
			}
			/*scheduler */
			scheduler();
		}
	}
	else
	{
		printf("Can't initialize cooperative scheduler\n\r");
	}
#endif
	return (0);
}
/**
 * This function kicks the timer in case it has unexpectidely stopped.
 * @note we have to delete this function once we find the true reason
 * for this anomaly
 * @return None
 */
void timer_kick(void)
{
	uint32_t temp = IORD_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE);
	if(BIT_IS_CLEAR(temp, ALTERA_AVALON_TIMER_STATUS_RUN_MSK))
	{
		/*If the timer is started and running, we should never end up here
		 * however this is not the case. In this application, the board was
		 * disappearing randomly from the EMS. After debugging, we found out
		 * the timer is no longer running (although its control bits are correct)
		 * So we had to implement this function in order to kick
		 * the timer when this happens*/
		counter_tmr0_run_bit++;
		/* First, stop the timer*/
		timer_stop();
		/* then restart the timer*/
		timer_start();
	}
}

/**
 * This function starts the timer used as systick source.
 * @return None
 */
void timer_start(void)
{
	uint32_t temp = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE);
	CLEAR_BIT(temp, ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
	SET_BIT(temp, ALTERA_AVALON_TIMER_CONTROL_START_MSK);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, temp);
}

/**
 * This function stops the timer used as systick source.
 * @return None
 */
void timer_stop(void)
{
	uint32_t temp = IORD_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE);
	SET_BIT(temp, ALTERA_AVALON_TIMER_CONTROL_STOP_MSK);
	CLEAR_BIT(temp, ALTERA_AVALON_TIMER_CONTROL_START_MSK);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, temp);
}

#if APS_EN
/**
 * This function run the aps task
 * @param[in] _aps_ptr pointer to aps object
 */
void aps_task(aps_module_t* _aps_ptr)
{
	uint32_t ln_1 = 0; //LN_IF_OFFSET;
	uint32_t ln_2 = 1;//LN_IF_OFFSET;
//	ln_1 += _aps->index * 2;
//	ln_2 += (_aps->index * 2) + 1;
	if(_aps_ptr->aps_protectch.current != APS_PROTECT_NO)
	{
		/*request to enable APS*/
		aps_enable(_aps_ptr);
		if(switch_working_line(_aps_ptr) == RET_SUCCESS)
		{
			_aps_ptr->assigned_aps_signal = _aps_ptr->aps_protectch.current;
			/*TX disable will be restored to the value updated in the case
			 * below*/
			set_transceiver_txdisable(&TRANS(ln_2));
			/*Disable loopbacks for proper APS functionality*/
			set_if_port_loopback(&PORT(ln_1), MODE_LB_NORMAL);
			set_if_port_loopback(&PORT(ln_2), MODE_LB_NORMAL);
			PORT_CTRL(ln_1).loopback_mode.next =
			PORT_CTRL(ln_1).loopback_mode.current = MODE_LB_NORMAL;
			PORT_CTRL(ln_2).loopback_mode.next =
			PORT_CTRL(ln_2).loopback_mode.current = MODE_LB_NORMAL;
			_aps_ptr->aps_protectch.change = 0;
		}
	}
	else
	{
		/*request to disable APS*/
		if(aps_disable(_aps_ptr) == RET_SUCCESS)
		{
			_aps_ptr->aps_protectch.current = APS_PROTECT_NO;
			_aps_ptr->assigned_aps_selected_line.current = APS_PROTECT_LINE_1;
			/*Disable loopbacks for proper APS functionality*/
			PORT_CTRL_LB(ln_1).next = PORT_CTRL_LB(ln_1).current =
									  MODE_LB_NORMAL;
			PORT_CTRL_LB(ln_2).next = PORT_CTRL_LB(ln_2).current =
									  MODE_LB_NORMAL;
			set_if_port_loopback(&PORT(ln_1),MODE_LB_NORMAL);
			set_if_port_loopback(&PORT(ln_2),MODE_LB_NORMAL);
			TRANS_CTRLTXDIS(ln_2).next = 1;
			set_transceiver_txdisable(&TRANS(ln_2));
			/* when APS is disabled, traffic is passed through LINE0
			 * and LINE1 TX is disabled.
			 * However, we need to remember the current state of LINE1
			 * Transmitter so we can restore it later when the APS feature is
			 * enabled once again.
			 * */
			/*in case the user disabled line2 before disabling the aps*/
			TRANS_CTRLTXDIS(ln_2).next = TRANS_CTRLTXDIS(ln_2).current;
			TRANS_CTRLTXDIS(ln_2).current = 1;
			_aps_ptr->aps_protectch.change = 0;
			_aps_ptr->assigned_aps_signal = _aps->aps_protectch.current;
		}
	}
}
#endif

/**
 * This function create the pfc module
 * @param[in] _port_ptr pointer to port object
 * @return None
 */
void create_pfc_module(port_t* _port_ptr)
{
	if(_port_ptr->pfc == NULL)
	{
		uint32_t _pfc_base = 0;
		if(IS_CLIENT(_port_ptr->index))
		{
			if PORT_FPGA1
			{
				_pfc_base = CLIENT_0_PFC_BASE + (0x4000 * _port_ptr->index);
			}
			else
			if PORT_FPGA2
			{
				_pfc_base = 0x10000000 + CLIENT_0_PFC_BASE + (0x4000 * _port_ptr->index);
			}
		}
		else
		{
			uint32_t _ln_pfc_base[LINES_IF_COUNT] = { LINE_0_PFC_BASE,
													  LINE_1_PFC_BASE };
			_pfc_base = _ln_pfc_base[_port_ptr->index - LN_INDEX_OFFSET];
		}
		_port_ptr->pfc = init_pfc_data(_port_ptr->pfc, _pfc_base);
	}
	set_pfc_index(PORT(_port_ptr->index).pfc, _port_ptr->index);
	start_pfc_monitoring(_port_ptr->pfc);
}
/**
 * This function destroy the pfc module
 * @param[in] _port_ptr pointer to port object
 */
void destroy_pfc_module(port_t* _port_ptr)
{
	if(_port_ptr->pfc != NULL)
	{
		stop_pfc_monitoring(PORT(_port_ptr->index).pfc);
		deinit_pfc(_port_ptr->pfc);
		_port_ptr->pfc = 0;
	}
}

/**
 * This function do the ppm adjustment
 * @return None
 */
/*warning check this fct with teddy to confirm the req and check if i have to add otu1 check*/
void task_ppm_adjustment(void)
{
	/*disable all adjustments that were made before this point*/
	ipg_si5338_disable_freq_inc_dec(&si5338[2], LN_0_IF_INDEX - 16);
	Delay_ms(2);
	ipg_si5338_enable_freq_inc_dec(&si5338[2], LN_0_IF_INDEX - 16);
	Delay_ms(20);

	ipg_si5338_disable_freq_inc_dec(&si5338[5], LN_1_IF_INDEX - 17);
	Delay_ms(2);
	ipg_si5338_enable_freq_inc_dec(&si5338[5], LN_1_IF_INDEX - 17);
	Delay_ms(20);

	//CL_MMAP(1)->misc->general_control->bits.PPM_EN = 0;

#ifdef aps
	LN_MMAP(0)->otu1_mapper->ln_cfg1_reg->bits.ENABLE_APS = 0;
	LN_MMAP(1)->otu1_mapper->ln_cfg1_reg->bits.ENABLE_APS = 0;
#endif
//	if(0 == flagTest2)
//	{
//		CL_MMAP(2)->misc->general_control->bits.PPM_EN = 0;
//	}
	adjust_ppm();

}
/* This function updates the board temperature by computing the average of the
 * temperatures read from the 4 available sensors.
 * @return None
 */
void get_board_temperature()
{
	/*Read the temperature from the 4 available temperature sensors */
	mcp9808_get_temperature(&mcp9808[0]);
	mcp9808_get_temperature(&mcp9808[1]);
	mcp9808_get_temperature(&mcp9808[2]);
	mcp9808_get_temperature(&mcp9808[3]);
	card.temperature.value = (mcp9808[0].temperature +
							  mcp9808[1].temperature +
						      mcp9808[2].temperature +
						      mcp9808[3].temperature) / 4;
}
/* This function is for the preferences list
 * @return None
 */
void build_preferences_list(void)
{
	int i = 0, offset = 0;
	/*card information settings*/
	offset += append_preference(card.info.pid, sizeof(card.info.pid));
	offset += append_preference(card.info.serial_number,
			                    sizeof(card.info.serial_number));
	offset += append_preference(&card.temperature.critical_min, sizeof(float));
	offset += append_preference(&card.temperature.warning_min, sizeof(float));
	offset += append_preference(&card.temperature.warning_max, sizeof(float));
	offset += append_preference(&card.temperature.critical_max, sizeof(float));

	/*section 103 settings*/
	offset += append_preference(pt_cl_1_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_2_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_3_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_4_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_5_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_6_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_7_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_8_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_9_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_10_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_11_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_12_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_13_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_14_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_15_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_cl_16_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_ln_1_in_info, PORT_INFO_SIZE);
	offset += append_preference(pt_ln_2_in_info, PORT_INFO_SIZE);
	offset += append_preference(&pt_cl_1_in_cat, sizeof(uint32_t));
	offset += append_preference(&pt_cl_2_in_cat, sizeof(uint32_t));
	offset += append_preference(&pt_cl_3_in_cat, sizeof(uint32_t));
	offset += append_preference(&pt_cl_4_in_cat, sizeof(uint32_t));
	offset += append_preference(&pt_cl_5_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_6_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_7_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_8_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_9_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_10_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_11_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_12_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_13_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_14_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_15_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_cl_16_in_info, sizeof(uint32_t));
	offset += append_preference(&pt_ln_1_in_cat, sizeof(uint32_t));
	offset += append_preference(&pt_ln_2_in_cat, sizeof(uint32_t));

	/*common interface settings*/
	for(i = 0; i < INTERFACE_COUNT; i++)
	{
		offset += append_preference(INTERFACE(i).destination,
									sizeof(INTERFACE(i).destination));
		/*port settings*/
		offset += append_preference(&PORT_CTRL(i).protocol_control,
									sizeof(PORT_CTRL(i).protocol_control));
		offset += append_preference(&PORT_CTRL(i).rate.current,
									sizeof(uint32_t));

		/*transceiver settings*/
		offset += append_preference(&TRANS_CTRL(i).threshold_mode,
									sizeof(threshold_mode_t));
		offset += append_preference(&pin[i].critical_min, sizeof(float));
		offset += append_preference(&pin[i].warning_min, sizeof(float));
		offset += append_preference(&pin[i].warning_max, sizeof(float));
		offset += append_preference(&pin[i].critical_max, sizeof(float));
		offset += append_preference(&pout[i].critical_min, sizeof(float));
		offset += append_preference(&pout[i].warning_min, sizeof(float));
		offset += append_preference(&pout[i].warning_max, sizeof(float));
		offset += append_preference(&pout[i].critical_max, sizeof(float));
		offset += append_preference(&TRANS_MONITOR(i).temperature.critical_min,
				  	  	  	  	  	 sizeof(float));
		offset += append_preference(&TRANS_MONITOR(i).temperature.warning_min,
									sizeof(float));
		offset += append_preference(&TRANS_MONITOR(i).temperature.warning_max,
									sizeof(float));
		offset += append_preference(&TRANS_MONITOR(i).temperature.critical_max,
									sizeof(float));
		offset += append_preference(&INTERFACE(i).service_state.current,
									sizeof(uint32_t));
		offset += append_preference(&TRANS_CTRL(i).tx_disable.current,
									sizeof(uint32_t));
		offset += append_preference(&PORT_CTRL(i).loopback_mode.current,
									sizeof(uint32_t));
		/*Add ALS settings to preference list*/
		offset += append_preference(&PORT(i).als->control, sizeof(als_mode_t));
	}
	offset += append_preference(&_aps.aps_mode.current, sizeof(uint32_t));
	offset += append_preference(&_aps.aps_protectch.current, sizeof(uint32_t));
	offset += append_preference(&_aps.revertive_mode.current, sizeof(uint32_t));
	offset += append_preference(&_aps.wtr_timer, sizeof(uint32_t));
	offset += append_preference(&_aps.holdoff_timer, sizeof(uint32_t));

	/*Line parameters*/
	for(i = LN_INDEX_START; i < INTERFACE_COUNT; i++)
	{
		offset += append_preference(&C_FECLOAD(i).critical_min, sizeof(float));
		offset += append_preference(&C_FECLOAD(i).critical_max, sizeof(float));
		offset += append_preference(&C_FECLOAD(i).warning_min, sizeof(float));
		offset += append_preference(&C_FECLOAD(i).warning_max, sizeof(float));
	}
}
/**
 * Callback function to read user prefs
 * @param[in] dst    destination address
 * @param[in] length length
 * @return -1: fail, 1: success
 */
ret_code_t IPGLB_ReadUserPrefsCallback(void* dst, size_t length)
{
	return (cfi_flash_read(PREFERENCES_OFFSET, dst, length));
}
/**
 * Callback function to read language file
 * @param[in] dst    destination address
 * @param[in] length length
 * @return -1: fail, 1: success
 */
ret_code_t IPGLB_ReadLngFileCallback(void* dst, size_t length)
{
	return (cfi_flash_read(LNG_FILE_OFFSET, dst, length));
}
/**
 * Callback function to write user prefs
 * @param[in] pSrc   source address
 * @param[in] dwSize size
 * @return none
 */
void IPGLB_WriteUserPrefsCallback(void * pSrc, size_t dwSize)
{
	cfi_flash_write(PREFERENCES_OFFSET, pSrc, dwSize);
	/*	lld_memcpy(FLASH_TRISTATE_CONTROLLER_0_BASE, PREFERENCES_OFFSET,
	 * dwSize, pSrc);*/
}
/**
 * Callback function to write language file
 * @param[in] pSrc   source address
 * @param[in] dwSize size
 * @return none
 */
void IPGLB_WriteLngFileCallback(void * pSrc, size_t dwSize)
{
	cfi_flash_write(LNG_FILE_OFFSET, pSrc, dwSize);
	/*	lld_memcpy(FLASH_TRISTATE_CONTROLLER_0_BASE, LNG_FILE_OFFSET, dwSize,
	 * (char*)pSrc);*/
}

/**
 * ISR function for 10 ms
 * @param[in] context
 * @return the number of ticks per ms
 */
alt_u32 isr_10ms(void* context)
{
	flag_10ms_event = 1;
	return (ALT_TICKS_10MS);
}
/**
 * ISR function for 100 ms
 * @param[in] context
 * @return the number of ticks per ms
 */
alt_u32 isr_100ms(void* context)
{
	flag_100ms_event = 1;
	//_if->port.monitor.statistics.otux_sts.bip_errors.accumulated
	return (ALT_TICKS_100MS);
}
/**
 * ISR function for 125 ms
 * @param[in] context
 * @return the number of ticks per ms
 */
alt_u32 isr_125ms(void* context)
{
	flag_125ms_event = 1;
	return (ALT_TICKS_125MS);
}
/**
 * ISR function for 1 s
 * @param[in] context
 * @return the number of ticks per s
 */
alt_u32 isr_1s(void* context)
{
	flag_1000ms_event = 1;
	card.sys_uptime++;
	return (ALT_TICKS_1SEC);
}
/**
 * ISR function for 2 s
 * @param[in] context
 * @return the number of ticks per s
 */
alt_u32 isr_2s(void* context)
{
	flag_2000ms_event = 1;
	return (ALT_TICKS_2SEC);
}
/**
 * ISR function for 5 s
 * @param[in] context
 * @return the number of ticks per s
 */
alt_u32 isr_5s(void* context)
{
	flag_5000ms_event = 1;
	return (ALT_TICKS_5SEC);
}
/*#warning check if we need this fuction for nw i commented out (sara)*/
/*alt_u32 heartbeat(void* context)
{
	static int freq = 500;
	static alt_u8 on = 0;

	if(on)
	{
		/*		HEARTBEAT_LED_ON();*/
/*		top_level_misc_reg.ctrl_reg->bits.LN_3_ALS_OVERRIDE = 1;
	}
	else
	{
		/*		HEARTBEAT_LED_OFF();*/
/*		top_level_misc_reg.ctrl_reg->bits.LN_3_ALS_OVERRIDE = 0;
	}
	(on) ? (on = 0) : (on = 1);
	return (freq);
}
*/
/**
 * Function that put flash in reset
 * @return none
 */
void put_flash_in_rst(void)
{
	alt_u32 temp = IORD_32DIRECT(0x1000000, 0x04);
	CLEAR_BIT(temp, BIT0 | BIT1);
	IOWR_32DIRECT(0x1000000, 0x04, temp);
	Delay_ms(500); /*To make sure flash is out of reset*/
}

/*#warning check if i will need this two fct (sara)*/

char* RATE_TEXT(port_t* _port)
{
	switch(_port->monitor.rate)
	{
	case PORT_RATE_XGE:
		return ("XGE");
	case PORT_RATE_FE:
		return ("FE");
	case PORT_RATE_GE:
		return ("GE");
	case PORT_RATE_FC1:
		return ("FC1");
	case PORT_RATE_FC2:
		return ("FC2");
	case PORT_RATE_OTU1:
		return ("OTU1");
	case PORT_RATE_OTU2:
		return ("OTU2");
	case PORT_RATE_OTU4:
		return ("OTU4");
	}
	return ("EMPTY");
}
/*
void dump_interface_monitor(interface_t* _if_ptr)
{
	if(PORT_RATE_XGE == _if_ptr->port.monitor.rate)
	{
		/*		printf("\t\t - LINK %s\n", (_if->port.monitor.statistics.eth_sts.alarms.LINK_DOWN)?"DOWN":"UP"); */
		/*		printf("\t\t - Remote LINK %s\n", (_if->port.monitor.statistics.eth_sts.alarms.REMOTE_LINK_DOWN)?"DOWN":"UP"); */
		/*		printf("\t\t - TX OK frames %d\n", _if->port.monitor.statistics.eth_sts.tx_ok.accumulated); */
		/*		printf("\t\t - RX OK frames %d\n", _if->port.monitor.statistics.eth_sts.rx_ok.accumulated); */
		/*		printf("\t\t - CRC frames %d\n", _if->port.monitor.statistics.eth_sts.rx_crc.accumulated); */
/*	}
	else
		if((PORT_RATE_OTU2 == _if_ptr->port.monitor.rate)
			|| (PORT_RATE_OTU4 == _if_ptr->port.monitor.rate))
		{
		/*		printf("\t\t - LOF %s\n", (_if->port.monitor.statistics.otux_sts.alarms.LOF)?"Error":"Normal"); */
		/*		printf("\t\t - BDI %s\n", (_if->port.monitor.statistics.otux_sts.alarms.BDI)?"Error":"Normal"); */
		/*		printf("\t\t - LOM %s\n", (_if->port.monitor.statistics.otux_sts.alarms.LOM)?"Error":"Normal"); */
		/*		printf("\t\t - BIP8 errors %d\n", _if->port.monitor.statistics.otux_sts.bip_errors.accumulated); */
/*		}
		else
		if(PORT_RATE_STM4 == _if_ptr->port.monitor.rate)
		{
		/*		printf("\t\t - MS_AIS %s\n", (_if->port.monitor.statistics.stmx_sts.alarms.MS_AIS)?"Error":"Normal"); */
		/*		printf("\t\t - BIP8 errors %d\n", _if->port.monitor.statistics.stmx_sts.bip_errors.accumulated); */
/*		}
}*/

/**
 * Function to cancel a request.
 * @param req_ptr pointer to the request object
 * Return none
 */
void cancel_request(ipg_control32_t* req_ptr)
{
	req_ptr->change = 0;
	req_ptr->next = req_ptr->current;
}

/**
 * Function to enable transmitters of all lines transceivers
 * @return None
 */
void enable_all_line_transceivers_tx(void)
{
	int if_index = 0;
	for(if_index = LN_INDEX_START; if_index < LN_INDEX_END; if_index++)
	{
		TRANS_CTRL(if_index).tx_disable.next = 0;
		set_transceiver_txdisable(&TRANS(if_index));
		TRANS_CTRL(if_index).tx_disable.current = 0;
	}
}
/**
 * Function to disable transmitters of all lines transceivers
 * @return None
 */
void disable_all_line_transceivers_tx(void)
{
	int if_index = 0;
	for(if_index = LN_INDEX_START; if_index < LN_INDEX_END; if_index++)
	{
		TRANS_CTRL(if_index).tx_disable.next = 1;
		set_transceiver_txdisable(&TRANS(if_index));
		TRANS_CTRL(if_index).tx_disable.current = 1;
	}
}

/**
 * Function to disable transmitters of all transceivers
 * @return None
 */
void disable_all_transceivers_tx(void)
{
	int if_index = 0;
	for(if_index = 0; if_index < CLIENTS_IF_COUNT; if_index++)
	{
		TRANS_CTRL(if_index).tx_disable.next = 1;
		set_transceiver_txdisable(&TRANS(if_index));
		TRANS_CTRL(if_index).tx_disable.current = 1;
	}
}

/**
 * Function to enable transmitters of all transceivers
 * @return None
 */
void enable_all_transceivers_tx(void)
{
	int if_index = 0;
	for(if_index = 0; if_index < CLIENTS_IF_COUNT; if_index++)
	{
		TRANS_CTRL(if_index).tx_disable.next = 0;
		set_transceiver_txdisable(&TRANS(if_index));
		TRANS_CTRL(if_index).tx_disable.current = 0;
	}
}
/*#warning check this function because nios stopped when running*/
/**
 * Function to configure the SILABs to generate the clocks needed for our
 * system.
 * @return None
 */
void configure_board_clocks(void)
{
	alt_u8 value = 0;

	ipg_si5338_config(&si5338[0]);
	printf("Si5338 0 initialized\n");
	Delay_ms(500);
	ipg_si5338_config(&si5338[1]);
	printf("Si5338 1 initialized\n");
	Delay_ms(500);
	ipg_si5338_config(&si5338[2]);
	printf("Si5338 2 initialized\n");
	Delay_ms(500);
	ipg_si5338_config(&si5338[3]);
	printf("Si5338 3 initialized\n");
	Delay_ms(500);
	ipg_si5338_config(&si5338[4]);
	printf("Si5338 4 initialized\n");
	Delay_ms(500);
	ipg_si5338_config(&si5338[5]);
	printf("Si5338 5 initialized\n");
	Delay_ms(500);

	/*FPGA 1*/
	/* tell the new SI-5375 to take its clock from the external clock input*/
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_CS_A = 0;
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_CS_B = 0;
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_CS_C = 0;
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_CS_D = 0;
	/* take the Si5375 channels out of reset*/
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_RST_A = 1;
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_RST_B = 1;
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_RST_C = 1;
	top_level_misc_reg.gen_ctrl_sts_reg_1->bits.SI5375_RST_D = 1;

	/*SI5375 BUS 0*/
	/* tell the new SI-5375 to take its clock from the external clock input*/
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_A_CS = 0;
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_B_CS = 0;
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_C_CS = 0;
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_D_CS = 0;
	/* take the Si5375 channels out of reset*/
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_A_RST = 1;
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_B_RST = 1;
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_C_RST = 1;
	top_level_misc_reg.sdh_rate_config_1->bits.xSI3575_D_RST = 1;

	/*SI5375 BUS 1*/
	/* tell the new SI-5375 to take its clock from the external clock input*/
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_A_CS = 0;
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_B_CS = 0;
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_C_CS = 0;
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_D_CS = 0;
	/* take the Si5375 channels out of reset*/
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_A_RST = 1;
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_B_RST = 1;
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_C_RST = 1;
	top_level_misc_reg.sdh_rate_config_1->bits.ySI3575_D_RST = 1;

	/*FPGA 2*/
	/* tell the new SI-5375 to take its clock from the external clock input*/
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_CS_A = 0;
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_CS_B = 0;
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_CS_C = 0;
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_CS_D = 0;
	/* take the Si5375 channels out of reset*/
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_RST_A = 1;
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_RST_B = 1;
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_RST_C = 1;
	top_level_misc_reg.gen_ctrl_sts_reg_2->bits.SI5375_RST_D = 1;

	/*SI5375 BUS 0*/
	/* tell the new SI-5375 to take its clock from the external clock input*/
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_A_CS = 0;
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_B_CS = 0;
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_C_CS = 0;
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_D_CS = 0;
	/* take the Si5375 channels out of reset*/
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_A_RST = 1;
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_B_RST = 1;
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_C_RST = 1;
	top_level_misc_reg.sdh_rate_config_2->bits.xSI3575_D_RST = 1;

	/*SI5375 BUS 1*/
	/* tell the new SI-5375 to take its clock from the external clock input*/
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_A_CS = 0;
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_B_CS = 0;
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_C_CS = 0;
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_D_CS = 0;
	/* take the Si5375 channels out of reset*/
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_A_RST = 1;
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_B_RST = 1;
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_C_RST = 1;
	top_level_misc_reg.sdh_rate_config_2->bits.ySI3575_D_RST = 1;

	Delay_ms(1);
	/*FPGA 1*/
	ipg_si5375_config(&si5375[0], PORT_A);
	Delay_ms(100);
	ipg_si5375_config(&si5375[0], PORT_B);
	Delay_ms(100);
	ipg_si5375_config(&si5375[0], PORT_C);
	Delay_ms(100);
	ipg_si5375_config(&si5375[0], PORT_D);
	Delay_ms(100);
	printf("Si5375 0 FPGA 1 initialized\n");
	ipg_si5375_config(&si5375[1], PORT_A);
	Delay_ms(100);
	ipg_si5375_config(&si5375[1], PORT_B);
	Delay_ms(100);
	ipg_si5375_config(&si5375[1], PORT_C);
	Delay_ms(100);
	ipg_si5375_config(&si5375[1], PORT_D);
	Delay_ms(500);
	printf("Si5375 1 FPGA 1 initialized\n");

	ipg_si5375_config(&si5375[2], PORT_A);
	Delay_ms(100);
	ipg_si5375_config(&si5375[2], PORT_C);
	Delay_ms(100);
	printf("Si5375 2 FPGA 1 initialized\n");


	/*FPGA 2*/
	ipg_si5375_config(&si5375[3], PORT_A);
	Delay_ms(100);
	ipg_si5375_config(&si5375[3], PORT_B);
	Delay_ms(100);
	ipg_si5375_config(&si5375[3], PORT_C);
	Delay_ms(100);
	ipg_si5375_config(&si5375[3], PORT_D);
	Delay_ms(100);
	printf("Si5375 0 FPGA 2 initialized\n");
	ipg_si5375_config(&si5375[4], PORT_A);
	Delay_ms(100);
	ipg_si5375_config(&si5375[4], PORT_B);
	Delay_ms(100);
	ipg_si5375_config(&si5375[4], PORT_C);
	Delay_ms(100);
	ipg_si5375_config(&si5375[4], PORT_D);
	Delay_ms(500);
	printf("Si5375 1 FPGA 2 initialized\n");
	ipg_si5375_config(&si5375[5], PORT_A);
	Delay_ms(100);
	ipg_si5375_config(&si5375[5], PORT_C);
	Delay_ms(100);
	printf("Si5375 2 FPGA 2 initialized\n");
}

/**Function that maps all the variable registers to their proper memory location.
 * @return None
 */
void register_mapping()
{
	/*int if_index = 0;*/
	/* map UART modules and registers*/
	map_uart_regs(&dvt_uart, DEBUG_UART_BASE);
	map_uart_regs(&arm_uart, UART_0_BASE);

	top_level_misc_reg.gen_ctrl_sts_reg_1 =
	(volatile union TOP_MISC_GENERAL_CTRL_STS_REG*)((ODUK_MPIF_BRICK_0_BASE +
													 0x00) | DE_CACHE_EN_BIT);
	/*AVALON_SPI_MASTER_BASE 0x10000000*/
	top_level_misc_reg.gen_ctrl_sts_reg_2 =
	(volatile union TOP_MISC_GENERAL_CTRL_STS_REG*)((0x10000000 + ODUK_MPIF_BRICK_0_BASE +
													 0x00) | DE_CACHE_EN_BIT);

	top_level_misc_reg.lcd_reg_1 =
	(volatile union TOP_MISC_LCD_REG*)((ODUK_MPIF_BRICK_0_BASE + 0x04) |
										DE_CACHE_EN_BIT);
	top_level_misc_reg.lcd_reg_2 =
	(volatile union TOP_MISC_LCD_REG*)((0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x04) |
										DE_CACHE_EN_BIT);
	top_level_misc_reg.lcd_reg_1->all = 0;
	top_level_misc_reg.lcd_reg_2->all = 0;

	top_level_misc_reg.config_1 =
	(volatile union TOP_MISC_CONFIG_REG*)((ODUK_MPIF_BRICK_0_BASE + 0x0C) |
										   DE_CACHE_EN_BIT);
	top_level_misc_reg.config_2 =
	(volatile union TOP_MISC_CONFIG_REG*)((0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x0C) |
										   DE_CACHE_EN_BIT);
	top_level_misc_reg.sdh_rate_config_1 =
	(volatile union TOP_MISC_SDH_RATE_REG*)((ODUK_MPIF_BRICK_0_BASE + 0x1C) |
											 DE_CACHE_EN_BIT);
	top_level_misc_reg.sdh_rate_config_2 =
	(volatile union TOP_MISC_SDH_RATE_REG*)((0x10000000 + ODUK_MPIF_BRICK_0_BASE + 0x1C) |
											 DE_CACHE_EN_BIT);

	//ODTUG2
	odtug2.regA_portM_rate_reg_1 = (volatile union ODTUG2_REGA_PORTM_RATE_REG*)((ODTUG2_BASE_1 + 0x24)|DE_CACHE_EN_BIT);
	odtug2.config_reg_1          = (volatile union ODTUG2_CONFIG_REG*)((ODTUG2_BASE_1 + 0x00)|DE_CACHE_EN_BIT);
	odtug2.regA_portM_rate_reg_2 = (volatile union ODTUG2_REGA_PORTM_RATE_REG*)((ODTUG2_BASE_2 + 0x24)|DE_CACHE_EN_BIT);
	odtug2.config_reg_2          = (volatile union ODTUG2_CONFIG_REG*)((ODTUG2_BASE_2 + 0x00)|DE_CACHE_EN_BIT);

	/*todo remove it from here and it to the brick*/
	/*map_aps_regs(&_aps, 0x1000020);*/
	//map_aps_regs(&_aps[1], 0x1000030);
}

/**Function for timer interrupt service routine.
 * Configured to trigger every ms.
 * @param[in] context pointer to data transfered between isr and application.
 * @return the time interval for the next trigger.
 */
alt_u32 systick_isr(void* context)
{
	systicks++;
	flag_1ms_event = 1;
	return (ticks_per_1ms);
}

/*
 * Function that sets the FW revision string.
 * @param[in] major major build number
 * @param[in] minor minor build number
 * @param[in] build compile build number
 * @return None
 */
void set_build_number(int major, int minor, int build)
{
	sprintf((char*) card.info.sw_number, "%d.%d.%d compiled on %s", major,
			minor, build, __DATE__);
}
/*
 * Function that read the fpga number.
 * @return None
 */
void read_fpga_number(void)
{
	uint32_t temp = IORD_32DIRECT(0x2000, 0);
	/* display the value as hex*/
	sprintf(card.info.fpga_sw_number, "%x", (unsigned int) temp);
}
/*
 * Function that initializes the UART drivers.
 * The debug interface is always initialized.
 * <br> The STM32F207 UART interface is initialized based on
 * the value of the ENABLE_ARM7_REQUESTS flag.
 * @return None
 */
void inituart()
{
	/*#if ENABLE_ARM7_REQUESTS */
	/*	ipg_uart_disable(&arm1_uart); */
	/*#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT */
	/*	init_arm1_uart(&arm1_uart, 1,FP_ARM1_UART_IRQ_INTERRUPT_CONTROLLER_ID,
	 * FP_ARM1_UART_IRQ); */
#if ENABLE_ARM7_REQUESTS
	ipg_uart_disable_IRRDY(&arm_uart);
	init_uart(&arm_uart, UART_0_IRQ_INTERRUPT_CONTROLLER_ID, UART_0_IRQ);
#endif

#if ENABLE_DVT_CODE
	ipg_uart_disable_IRRDY(&dvt_uart);
	init_uart(&dvt_uart, DEBUG_UART_IRQ_INTERRUPT_CONTROLLER_ID,
			  DEBUG_UART_IRQ);
#endif
}

/**
 * Function that handles any available DEBUG () requests.
 * @return None
 */
void handle_debug_packet()
{
#ifdef TEST
	if(get_dvt_request_ready())
	{
		alt_u8 request_buff[1024];
		alt_u8 tx_buff[2048];
		int tx_len = 0;
		memset(tx_buff, 0, sizeof(tx_buff));
		get__uart_request(request_buff);
		/* make sure the request addresses us */
		if(request_buff[0] != profile.addr_bus )
		{
			clear__request_ready();
			return;
		}
		/* printf("bp req %s\n", request_buff); */
		/* 1st byte is the address, we already checked for it */
		/* 2nd byte is the command */
		KURS_HandleRequests((char*)request_buff,(char*)tx_buff,&tx_len);

		/* send the reply */
		send__uart_reply((alt_u8*)tx_buff, tx_len);
		/* cleanup */
		clear__request_ready();
	}
#else
	ipglb_prot2_service(dvt_processor);
#endif
}

#if ENABLE_ARM7_REQUESTS
/*
 * Function that handles any available ARM7 requests.
 * @return None
 */
void handle_kurs_packet()
{
	kurs_service(kurs_req_processor);
}
#endif

/**This helper function handles users' requests to write to a NiOS memory
 *  register.
 * @param[in] base base address of the memory location
 * @param[in] offset offset register to write to
 * @param[in] value pointer where to store the read data
 * @return  RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2
 * 			protocol definitions.
 * \note 	The function, command and arguments used in this transaction is
 * 			defined in the
 * 			software design document of this application.
 */
ret_code_t handle_mem_wr(alt_u32 base, alt_u32 offset, alt_u32 value)
{
	switch(base)
	{
	/*#warning check if it must be removed*/
		case SDRAM_BASE:
			switch(offset)
			{
				case 0x00: /*Change Client 0 port Type */
					card.interfaces[0].port.control.rate.next = value & 0x0F;
					card.interfaces[0].port.control.rate.change = 1;
					break;
				case 0x04: /*Change Client 1 port Type */
					card.interfaces[1].port.control.rate.next = value & 0x0F;
					card.interfaces[1].port.control.rate.change = 1;
					break;
				case 0x20: /*Change Client 2 port Type */
					card.interfaces[2].port.control.rate.next = value & 0x0F;
					card.interfaces[2].port.control.rate.change = 1;
					break;
				case 0x24: /*Change Client 3 port Type */
					card.interfaces[3].port.control.rate.next = value & 0x0F;
					card.interfaces[3].port.control.rate.change = 1;
					break;
				default:
					break;
			}
		break;
	default:
		IOWR_32DIRECT(base, offset, value);
		break;
	}
	return (RET_SUCCESS);
}

/**This helper function handles users' requests to read from a NiOS memory
 * register.
 * @param[in] base base address of the memory location
 * @param[in] offset offset register to read from
 * @param[out] value pointer where to store the read data
 * @return  RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2
 * protocol definitions.
 * \note 	The function, command and arguments used in this transaction is
 *  defined in the
 * 	software design document of this application.
 */
ret_code_t handle_mem_rd(alt_u32 base, alt_u32 offset, alt_u32* value)
{
	switch(base)
	{
		case 0x8000000:
			switch(offset)
			{
				case 0x00: /*Systicks (ms) */
					*value = systicks;
					break;
				case 0x01: /*Card.sys_uptime (sec) */
					*value = card.sys_uptime;
					break;
				case 0x02: /*OREs_ARM */
					*value = counter_ores_arm;
					break;
				case 0x03: /*OREs_DVT */
					*value = counter_ores_dvt;
					break;
				case 0x04: /*Timer0 Run Bit Counter */
					*value = counter_tmr0_run_bit;
					break;
				default:
					break;
			}
			break;
	default:
		*value = IORD_32DIRECT(base, offset);
		break;
	}
	/* printf("mmap read base %x, offset %x, value returned %x\n", (int)base,
	 * (int)offset, (int)(*data32)); */
	return (RET_SUCCESS);
}

/**This helper function handles users' requests to read from a register of one
 * of the MDIO devices on board.
 * Depending on which MDIO device is passed as an argument, it dispatches the
 * corresponding handler.
 * @param[in] mdio_device_id ID of the I2C device that we need to configure
 *  @arg MDIO_DEVICE_ID_GENERIC
 * 	@arg MDIO_DEVICE_ID_PHY
 * 	@arg MDIO_DEVICE_ID_OM
 * @param[in] mdio_device_index index of the device (in case we have multiple
 * instances of the same MDIO device on board)
 * @param[in] offset offset register to read from
 * @param[out] value pointer where to store the read data
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2
 * 			protocol definitions.
 * \note 	The function, command and arguments used in this transaction
 * 			is defined in the
 * 			software design document of this application.
 */
ret_code_t handle_mdio_rd(alt_u8 mdio_device_id, alt_u8 mdio_device_index,
						  alt_u8 phy_address, alt_u8 device_address,
						  alt_u16 offset, alt_u16 *value)
{
	switch(mdio_device_id)
	{
		default:
			break;
	}
	return (RET_UNKNOWN_DEVICE);
}

/**This helper function handles users' requests to write to a register of one of
 *  the MDIO devices on board.
 * Depending on which MDIO device is passed as an argument, it dispatches the
 * corresponding handler.
 * @param[in] mdio_device_id ID of the I2C device that we need to configure
 *  @arg MDIO_DEVICE_ID_GENERIC
 * 	@arg MDIO_DEVICE_ID_PHY
 * 	@arg MDIO_DEVICE_ID_OM
 * @param[in] mdio_device_index index of the device (in case we have multiple
 * instances of the same MDIO device on board)
 * @param[in] offset offset register to write to
 * @param[in] value value to be written
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2
 *			protocol definitions.
 * \note 	The function, command and arguments used in this transaction is
 * 			defined in the
 * 			software design document of this application.
 */
ret_code_t handle_mdio_wr(alt_u8 mdio_device_id, alt_u8 mdio_device_index,
						  alt_u8 phy_address, alt_u8 device_address,
						  alt_u16 offset, alt_u16 value)
{
	switch(mdio_device_id)
	{
		default:
			break;
	}
	return (RET_UNKNOWN_DEVICE);
}

/**This helper function handles users' requests to write to a register of one of
 *  the I2C devices on board.
 * Depending on which I2C device is passed as an argument, it dispatches the
 * corresponding handler.
 * @param[in] i2c_device_id ID of the I2C device that we need to configure
 *  @arg I2C_GENERIC_DEVICE
 * 	@arg I2C_SILAB_DEVICE
 * 	@arg I2C_BOARD_TS_DEVICE
 * 	@arg I2C_DDR3_TS_DEVICE
 * @param[in] request pointer to the request that was received
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2
 *  protocol definitions.
 * \note 	The function, command and arguments used in this transaction is
 * defined in the
 * software design document of this application.
 */
ret_code_t handle_i2c_wr(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply,
						 int* reply_ptr)
{
#define I2C_DEVICE_INDEX	UART_PACKET_PARAMS_INDEX+1
	alt_u8 i2c_device_index = request[I2C_DEVICE_INDEX];
	memcpy(&reply[(*reply_ptr)], &request[I2C_DEVICE_INDEX], 1);
	(*reply_ptr)++;
	switch(i2c_device_id)
	{
		case I2C_GENERIC_DEVICE:
			break;
		case I2C_SILAB_DEVICE:
	#define I2C_ADDRESS_INDEX	UART_PACKET_PARAMS_INDEX+2
	#define I2C_VALUE_INDEX	 UART_PACKET_PARAMS_INDEX+3
		{
			alt_u8 i2c_address = request[I2C_ADDRESS_INDEX];
			alt_u8 i2c_value = request[I2C_VALUE_INDEX];
			memcpy(&reply[(*reply_ptr)], &request[I2C_ADDRESS_INDEX], 1);
			(*reply_ptr)++;
			/* we have 6 SI-5338 (indices 0:5) and 6 SI-5375 device with 4 ports
			 *  (index 6:30)*/
			if(i2c_device_index < 6)
			{
				return (handle_user_si5338_write(&si5338[i2c_device_index],
						i2c_address, i2c_value));
			}
			else if((i2c_device_index >= 6) && (i2c_device_index < 30) )
			{
				return (handle_user_si5375_write(&si5375[i2c_device_index - 6],
						i2c_device_index - 6,i2c_address, i2c_value));
			}
			return (RET_UNKNOWN_DEVICE);
		}
	#undef I2C_VALUE_INDEX
	#undef I2C_ADDRESS_INDEX
		case I2C_BOARD_TS_DEVICE:
			break;
		case I2C_SFPP_DEVICE:
#define I2C_MSA_INDEX	UART_PACKET_PARAMS_INDEX+2
#define I2C_ADDRESS_INDEX	UART_PACKET_PARAMS_INDEX+3
#define I2C_VALUE_INDEX	 UART_PACKET_PARAMS_INDEX+4
	{
		ret_code_t rc = RET_FAIL;
		alt_u8 i2c_msa = request[I2C_MSA_INDEX];
		alt_u8 i2c_address = request[I2C_ADDRESS_INDEX];
		alt_u8 i2c_value = request[I2C_VALUE_INDEX];
		memcpy(&reply[(*reply_ptr)], &request[I2C_MSA_INDEX], 2);
		(*reply_ptr) += 2;
		rc = handle_user_sfpp_write(
			(sfpp_device_t*)TRANS(i2c_device_index).device.device, i2c_msa,
			i2c_address, i2c_value);
		return (rc);
	}
	break;
#undef I2C_VALUE_INDEX
#undef I2C_ADDRESS_INDEX
#undef I2C_MSA_INDEX
			break;
		case I2C_SFP_DEVICE:
	#define I2C_MSA_INDEX	UART_PACKET_PARAMS_INDEX+2
	#define I2C_ADDRESS_INDEX	UART_PACKET_PARAMS_INDEX+3
	#define I2C_VALUE_INDEX	 UART_PACKET_PARAMS_INDEX+4
		{
			ret_code_t rc = RET_FAIL;
			alt_u8 i2c_msa = request[I2C_MSA_INDEX];
			alt_u8 i2c_address = request[I2C_ADDRESS_INDEX];
			alt_u8 i2c_value = request[I2C_VALUE_INDEX];
			memcpy(&reply[(*reply_ptr)], &request[I2C_MSA_INDEX], 2);
			(*reply_ptr) += 2;
			rc = handle_user_sfp_write(
				(sfp_device_t*)TRANS(i2c_device_index).device.device, i2c_msa,
				i2c_address, i2c_value);
			return (rc);
		}
		break;
#undef I2C_VALUE_INDEX
#undef I2C_ADDRESS_INDEX
#undef I2C_MSA_INDEX
		case I2C_XFP_DEVICE:
		break;
	case I2C_DDR3_EEPROM_DEVICE:
		break;
	case I2C_DDR3_TS_DEVICE:
		break;
	default:
		break;
	}
	return (RET_UNKNOWN_DEVICE);
#undef I2C_DEVICE_INDEX
}

/**This helper function handles users' requests to read a register from one of
 * the I2C devices on board.
 * Depending on which I2C device is passed as an argument, it dispatches the
 * corresponding handler.
 * @param[in] i2c_device_id ID of the I2C device that we need to configure
 *  @arg I2C_GENERIC_DEVICE
 * 	@arg I2C_SILAB_DEVICE
 * 	@arg I2C_BOARD_TS_DEVICE
 * 	@arg I2C_DDR3_TS_DEVICE
 * @param[in] request pointer to the request that was received
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @param[out] i2c_value pointer where to store the read data
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2
 * 			protocol definitions.
 * \note 	The function, command and arguments used in this transaction is
 * 			defined in the
 * 			software design document of this application.
 */
ret_code_t handle_i2c_rd(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply,
						int* reply_ptr, alt_u8 *i2c_value)
{
#define I2C_DEVICE_INDEX	UART_PACKET_PARAMS_INDEX+1
	alt_u8 i2c_device_index = request[I2C_DEVICE_INDEX];
	memcpy(&reply[(*reply_ptr)], &request[I2C_DEVICE_INDEX], 1);
	(*reply_ptr)++;
	switch(i2c_device_id)
	{
		case I2C_GENERIC_DEVICE:
			break;
		case I2C_SILAB_DEVICE:
	#define I2C_ADDRESS_INDEX	UART_PACKET_PARAMS_INDEX+2
		{
			alt_u8 i2c_address = request[I2C_ADDRESS_INDEX];
			memcpy(&reply[(*reply_ptr)], &request[I2C_ADDRESS_INDEX], 1);
			(*reply_ptr)++;
			/*we have 6 SI-5338 (indices 0:5) and 6 SI-5375 device with 4 ports
			 * (index 6:30)*/
			if(i2c_device_index < 6)
			{
				return (handle_user_si5338_read(&si5338[i2c_device_index],
						i2c_address, i2c_value));
			}
			else if((i2c_device_index >= 6) && (i2c_device_index < 30) )
			{
				return (handle_user_si5375_read(&si5375[i2c_device_index-6],
						i2c_device_index - 6, i2c_address, i2c_value));
			}

			return (RET_UNKNOWN_DEVICE);
		}
	#undef I2C_ADDRESS_INDEX
		case I2C_BOARD_TS_DEVICE:
			break;
		case I2C_SFPP_DEVICE:
#define I2C_MSA_INDEX		UART_PACKET_PARAMS_INDEX+2
#define I2C_ADDRESS_INDEX	UART_PACKET_PARAMS_INDEX+3
	{
		alt_u8 i2c_msa = request[I2C_MSA_INDEX];
		alt_u8 i2c_address = request[I2C_ADDRESS_INDEX];
		memcpy(&reply[(*reply_ptr)], &request[I2C_MSA_INDEX], 2);
		(*reply_ptr) += 2;
		printf("User SFP+ R from dev %d, msa %x, address %x\n",
				i2c_device_index, i2c_msa, i2c_address);
		return (handle_user_sfpp_read((sfpp_device_t*)card.interfaces
		 [i2c_device_index].transceiver.device.device, i2c_msa,
		  i2c_address, i2c_value));
	}
#undef I2C_ADDRESS_INDEX
#undef I2C_MSA_INDEX
		break;

			break;
		case I2C_SFP_DEVICE:
	#define I2C_MSA_INDEX	UART_PACKET_PARAMS_INDEX+2
	#define I2C_ADDRESS_INDEX	UART_PACKET_PARAMS_INDEX+3
		{
			alt_u8 i2c_msa = request[I2C_MSA_INDEX];
			alt_u8 i2c_address = request[I2C_ADDRESS_INDEX];
			memcpy(&reply[(*reply_ptr)], &request[I2C_MSA_INDEX], 2);
			(*reply_ptr) += 2;
			printf("User SFP+ R from dev %d, msa %x, address %x\n",
					i2c_device_index, i2c_msa, i2c_address);
			return (handle_user_sfp_read((sfp_device_t*)card.interfaces
			 [i2c_device_index].transceiver.device.device, i2c_msa,
			  i2c_address, i2c_value));
		}
	#undef I2C_ADDRESS_INDEX
	#undef I2C_MSA_INDEX
			break;
	case I2C_XFP_DEVICE:
			break;
		case I2C_DDR3_EEPROM_DEVICE:
			break;
		case I2C_DDR3_TS_DEVICE:
			break;
		default:
			break;
	}
	return (RET_UNKNOWN_DEVICE);
#undef I2C_DEVICE_INDEX
}

/**This helper function configures one of the I2C devices on board.
 * Depending on which I2C device is passed as an argument, it dispatches the
 * corresponding handler.
 * @param[in] i2c_device_id ID of the I2C device that we need to configure
 *  @arg I2C_GENERIC_DEVICE
 * 	@arg I2C_SILAB_DEVICE
 * 	@arg I2C_BOARD_TS_DEVICE
 * 	@arg I2C_DDR3_TS_DEVICE
 * 	@param[in] request pointer to the received request
 * @param[out] reply pointer to the reply that should be sent back
 * @param[in,out] reply_ptr pointer to the last char in the reply
 * @return \c RET_SUCCESS on success
 * \note 	The format of the request and reply packets follow the IPGLB2
 * 			protocol definitions.
 * \note 	The function, command and arguments used in this transaction is
 *          defined in the
 * 			software design document of this application.
 */
ret_code_t handle_user_i2c_config(alt_u8 i2c_device_id, alt_u8* request,
								  alt_u8* reply, int* reply_ptr)
{
#define I2C_DEVICE_INDEX	UART_PACKET_PARAMS_INDEX+1
	alt_u8 i2c_device_index = request[I2C_DEVICE_INDEX];
	memcpy(&reply[(*reply_ptr)], &request[I2C_DEVICE_INDEX], 1);
	(*reply_ptr)++;
	switch(i2c_device_id) {
	case I2C_SILAB_DEVICE:
#define I2C_CFG_INDEX	UART_PACKET_PARAMS_INDEX+2
	{
		memcpy(&reply[(*reply_ptr)], &request[I2C_CFG_INDEX], 1);
		(*reply_ptr)++;
		/*we have 6 SI-5338 (indices 0:5) and 6 SI-5375 device with 4 ports
		 * (index 6:30)*/
		if(i2c_device_index < 6)
		{
			return (handle_user_si5338_config(&si5338[i2c_device_index]));
		}
		else if((i2c_device_index >= 6) && (i2c_device_index < 30) )
		{
			return (handle_user_si5375_config(&si5375[i2c_device_index-6],
					i2c_device_index - 6));
		}
		return (RET_UNKNOWN_DEVICE);
	}
#undef I2C_CFG_INDEX
	case I2C_BOARD_TS_DEVICE:
		 break;
	case I2C_DDR3_TS_DEVICE:
		 break;
	default:
		break;
	}
	return (RET_UNKNOWN_DEVICE);
#undef I2C_DEVICE_INDEX
}

/**This function implements additional behavior and monitoring for this project
 * that has to run when dynamic updates occur.
 * It should not be called however since it will be called automatically by
 * update_if_dynamic_data.
 * @param[in,out] _if	pointer to the interface object
 * Return none
 */
void callback_update_if_dynamic_data(interface_t* _if)
{
	/*TODO implement callback_update_if_dynamic_data if needed*/
}

/**This function takes an I2C multiplexer (PCA9547) out of reset.
 * This is done by setting the RST pin that is connected to the FPGA.
 * In this application we only have two I2C multiplexers.
 * @param[in] i2c_mux_index index of the I2C multiplexer. The allowed values
 *  are 0 and 1.
 *  Return none
 */
void unreset_i2c_mux(int i2c_mux_index)
{
	/*TODO to check with Majdi the I2C MUX reset bit*/
}

/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus X FPGA1.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_bus_x(uint32_t i2c_channel)
{

}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus Y FPGA1.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_bus_y(uint32_t i2c_channel)
{
	/*do nothing*/
}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus 2 FPGA1.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_bus_ln0(uint32_t i2c_channel)
{
	/*do nothing*/
}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus 3 FPGA1.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_bus_ln1(uint32_t i2c_channel)
{
	/*do nothing*/
}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus X FPGA2.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_rem_bus_x(uint32_t i2c_channel)
{

}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus Y FPGA2.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_rem_bus_y(uint32_t i2c_channel)
{
	/*do nothing*/
}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus 2 FPGA1.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_rem_bus_ln0(uint32_t i2c_channel)
{
	/*do nothing*/
}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus 3 FPGA1.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_rem_bus_ln1(uint32_t i2c_channel)
{
	/*do nothing*/
}
/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus 4 FPGA1.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @param[in] i2c_mux_ptr pointer to the i2c_mux object
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_bus4(uint32_t i2c_channel, pca9547_device_t* i2c_mux_ptr, port_t* _port)
{
	int index = _port->index;
	static int current_selected_channel = CHANNEL_NONE;
	if(current_selected_channel == i2c_channel)
	{
		/* still talking to the same channel, nothing to do */
		return;
	}
	if(i2c_channel != CHANNEL_NONE)/* first make sure no channel is selected */
		select_channel_on_bus4(CHANNEL_NONE, i2c_mux_ptr, _port);

	/*printf("I2C channel %d is selected\n", i2c_channel); */
	switch(i2c_channel)
	{
		case CHANNEL_NONE:/* when not in need the I2C hub enable pins should
		 always be LOW (electrical problem on the board) */
		{
			//Unselect both SFPs
			LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_MOD_DES = 1;
			LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI2_MOD_DES = 1;
			unselect_all_pca9547_channels(i2c_mux_ptr);
		}
			break;
		case CHANNEL_SFP_SI5338_0:
		case CHANNEL_SFP_SI5338_1:
		case CHANNEL_SFP_SI5338_2:
		case CHANNEL_SFP_3:
		case CHANNEL_SFP_4:
		case CHANNEL_SFP_5:
		case CHANNEL_SFP_6:
		case CHANNEL_SFP_7:
			select_pca9547_channel(i2c_mux_ptr,
								  (i2c_channel - CHANNEL_SFP_SI5338_0));
			break;
//		case CHANNEL_SFPP_0:
//			otu2_brick.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_MOD_DES = 0; //We clear this bit to select XFP 0
//			break;
//		case CHANNEL_SFPP_1:
//			otu2_brick.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI2_MOD_DES = 0; //We clear this bit to select XFP 1
//			break;
//		case CHANNEL_SI5375_0:
//				break;
//		case CHANNEL_SI5375_1:
//				break;
		case CHANNEL_MCP9808_1:
		case CHANNEL_MCP9808_2:
			break;
	}
	current_selected_channel = i2c_channel;
}

/*
 * Function that makes the configurations needed to activate a certain I2C
 * channel on bus 4 FPGA2.
 * @param[in] i2c_channel The I2C channel that needs to be activated (selected)
 * @param[in] i2c_mux_ptr pointer to the i2c_mux object
 * @see I2C_CHANNELS
 * Return none
 */
void select_channel_on_rem_bus4(uint32_t i2c_channel, pca9547_device_t* i2c_mux_ptr, port_t* _port)
{
	int index = _port->index;
	static int current_selected_channel = CHANNEL_NONE;
	if(current_selected_channel == i2c_channel)
	{
		/* still talking to the same channel, nothing to do */
		return;
	}
	if(i2c_channel != CHANNEL_NONE)/* first make sure no channel is selected */
		select_channel_on_rem_bus4(CHANNEL_NONE, i2c_mux_ptr, _port);

	/*printf("I2C channel %d is selected\n", i2c_channel); */
	switch(i2c_channel)
	{
		case CHANNEL_NONE:/* when not in need the I2C hub enable pins should
		 always be LOW (electrical problem on the board) */
		{
			//Unselect both XFPs
			LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_MOD_DES = 1;
			LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI2_MOD_DES = 1;
			unselect_all_pca9547_channels(i2c_mux_ptr);
		}
			break;
		case CHANNEL_SFP_SI5338_0:
		case CHANNEL_SFP_SI5338_1:
		case CHANNEL_SFP_SI5338_2:
		case CHANNEL_SFP_3:
		case CHANNEL_SFP_4:
		case CHANNEL_SFP_5:
		case CHANNEL_SFP_6:
		case CHANNEL_SFP_7:
			select_pca9547_channel(i2c_mux_ptr,
								  (i2c_channel - CHANNEL_SFP_SI5338_0));
			break;
//		case CHANNEL_SFPP_0:
//			otu2_brick.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_MOD_DES = 0; //We clear this bit to select XFP 0
//			break;
//		case CHANNEL_SFPP_1:
//			otu2_brick.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI2_MOD_DES = 0; //We clear this bit to select XFP 1
//			break;
//		case CHANNEL_SI5375_0:
//				break;
//		case CHANNEL_SI5375_1:
//				break;
		case CHANNEL_MCP9808_1:
		case CHANNEL_MCP9808_2:
			break;
	}
	current_selected_channel = i2c_channel;
}

/*
 * Function that builds and configures the I2C devices to implement the I2C bus
 * tree. It will start by initializing the available I2C buses
 * (in this application we only have one).
 * And then for every I2C device we have, we assign to it the bus it is using,
 * its proper channel on the bus' virtual multiplexer.
 * Also the device is given its I2C address.
 * Return none
 */
void build_i2c_tree(void)
{
	int if_index = 0;
	/* configure I2C busses*/
	/* we have three busses*/
	/*The X-BUS*/
	xi2c_bus.name = SLS_AVALON_I2C_M_X_NAME;
	xi2c_bus.select_channel = select_channel_on_bus_x;
	INST_I2C(SLS_AVALON_I2C_M_X, xi2c_bus);
	xi2c_bus.speed = 0;
	set_i2c_bus_speed(&xi2c_bus);

	yi2c_bus.name = SLS_AVALON_I2C_M_Y_NAME;
	yi2c_bus.select_channel = select_channel_on_bus_y;
	INST_I2C(SLS_AVALON_I2C_M_Y, yi2c_bus);
	yi2c_bus.speed = 0;
	set_i2c_bus_speed(&yi2c_bus);

	ln0_i2c_bus2.name = SLS_AVALON_I2C_M_LI1_NAME;
	ln0_i2c_bus2.select_channel = select_channel_on_bus_ln0;
	INST_I2C(SLS_AVALON_I2C_M_LI1, ln0_i2c_bus2);
	ln0_i2c_bus2.speed = 0;
	set_i2c_bus_speed(&ln0_i2c_bus2);

	ln1_i2c_bus3.name = SLS_AVALON_I2C_M_LI2_NAME;
	ln1_i2c_bus3.select_channel = select_channel_on_bus_ln1;
	INST_I2C(SLS_AVALON_I2C_M_LI2, ln1_i2c_bus3);
	ln1_i2c_bus3.speed = 0;
	set_i2c_bus_speed(&ln1_i2c_bus3);

	i2c_bus4.name = SLS_AVALON_I2C_M_NAME;
	i2c_bus4.select_channel = select_channel_on_bus4;
	INST_I2C(SLS_AVALON_I2C_M, i2c_bus4);
	i2c_bus4.speed = 0;
	set_i2c_bus_speed(&i2c_bus4);


	/*The Y-BUS*/
	rem_xi2c_bus.name = SLS_AVALON_I2C_REM_M_X_NAME;
	rem_xi2c_bus.select_channel = select_channel_on_rem_bus_x;
	INST_I2C(SLS_AVALON_I2C_REM_M_X, rem_xi2c_bus);
	rem_xi2c_bus.speed = 0;
	set_i2c_bus_speed(&rem_xi2c_bus);

	rem_yi2c_bus.name = SLS_AVALON_I2C_REM_M_Y_NAME;
	rem_yi2c_bus.select_channel = select_channel_on_rem_bus_y;
	INST_I2C(SLS_AVALON_I2C_REM_M_Y, rem_yi2c_bus);
	rem_yi2c_bus.speed = 0;
	set_i2c_bus_speed(&rem_yi2c_bus);

	rem_ln0_i2c_bus2.name = SLS_AVALON_I2C_REM_M_LI1_NAME;
	rem_ln0_i2c_bus2.select_channel = select_channel_on_rem_bus_ln0;
	INST_I2C(SLS_AVALON_I2C_REM_M_LI1, rem_ln0_i2c_bus2);
	rem_ln0_i2c_bus2.speed = 0;
	set_i2c_bus_speed(&rem_ln0_i2c_bus2);

	rem_ln1_i2c_bus3.name = SLS_AVALON_I2C_REM_M_LI2_NAME;
	rem_ln1_i2c_bus3.select_channel = select_channel_on_rem_bus_ln1;
	INST_I2C(SLS_AVALON_I2C_REM_M_LI2, rem_ln1_i2c_bus3);
	rem_ln1_i2c_bus3.speed = 0;
	set_i2c_bus_speed(&rem_ln1_i2c_bus3);

	rem_i2c_bus4.name = SLS_AVALON_I2C_REM_M_NAME;
	rem_i2c_bus4.select_channel = select_channel_on_bus4;
	INST_I2C(SLS_AVALON_I2C_REM_M, rem_i2c_bus4);
	rem_i2c_bus4.speed = 0;
	set_i2c_bus_speed(&rem_i2c_bus4);
	/*FPGA 1*/
	open_i2c_bus(&xi2c_bus);
	open_i2c_bus(&yi2c_bus);
	open_i2c_bus(&ln0_i2c_bus2);
	open_i2c_bus(&ln1_i2c_bus3);
	open_i2c_bus(&i2c_bus4);
	/*FPGA 2*/
	open_i2c_bus(&rem_xi2c_bus);
	open_i2c_bus(&rem_yi2c_bus);
	open_i2c_bus(&rem_ln0_i2c_bus2);
	open_i2c_bus(&rem_ln1_i2c_bus3);
	open_i2c_bus(&rem_i2c_bus4);
	/*FPGA 1*/
	/*
	 * Connect devices on the X-bus.
	 * We have 1 si5375.
	 * */
	si5375[0].i2c_dev.bus = &xi2c_bus;
	/*	si5375[0].i2c_dev.address 		= SI5375_I2C_ADDRESS; */
	memcpy(&si5375[0].portA_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
	memcpy(&si5375[0].portB_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
	memcpy(&si5375[0].portC_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
	memcpy(&si5375[0].portD_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
	/*
	 * Connect devices on the Y-bus.
	 * We have 1 si5375.
	 * */
	si5375[1].i2c_dev.bus = &yi2c_bus;
	/*	si5375[1].i2c_dev.address 		= SI5375_I2C_ADDRESS; */
	memcpy(&si5375[1].portA_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
	memcpy(&si5375[1].portB_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
	memcpy(&si5375[1].portC_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
	memcpy(&si5375[1].portD_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
	/*
	 * Connect devices on the bus 4.
	 * We have 1 si5375.
	 * */
	si5375[2].i2c_dev.bus = &i2c_bus4;
	/*	si5375[1].i2c_dev.address 		= SI5375_I2C_ADDRESS; */
	memcpy(&si5375[2].portA_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));
	memcpy(&si5375[2].portB_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));
	memcpy(&si5375[2].portC_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));
	memcpy(&si5375[2].portD_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));


	/*FPGA 2*/
	/*todo check si5375 configuration*/
	si5375[3].i2c_dev.bus = &rem_xi2c_bus;
	memcpy(&si5375[3].portA_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
	memcpy(&si5375[3].portB_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
	memcpy(&si5375[3].portC_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
	memcpy(&si5375[3].portD_configuration[0][0], &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));

	si5375[4].i2c_dev.bus = &rem_yi2c_bus;
	/*	si5375[4].i2c_dev.address 		= SI5375_I2C_ADDRESS; */
	memcpy(&si5375[4].portA_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
	memcpy(&si5375[4].portB_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
	memcpy(&si5375[4].portC_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
	memcpy(&si5375[4].portD_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));

	si5375[5].i2c_dev.bus = &rem_i2c_bus4;
	/*	si5375[5].i2c_dev.address 		= SI5375_I2C_ADDRESS; */
	memcpy(&si5375[5].portA_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));
	memcpy(&si5375[5].portB_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));
	memcpy(&si5375[5].portC_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));
	memcpy(&si5375[5].portD_configuration[0][0], &si5375_cfg_167MHz_155_52MHz[0][0],
			sizeof(si5375_cfg_167MHz_155_52MHz));


	/*
	 * Connect devices on the main bus.
	 * We have:
	 *  one PCA9547 I2C MUX.
	 *  one MCP9808 temperature sensor.
	 *  three SI5338
	 *  8 SFPs
	 * */
	i2c_mux.i2c_dev.bus = &i2c_bus4;
	i2c_mux.i2c_dev.address = PCA9547_I2C_ADDRESS;

	rem_i2c_mux.i2c_dev.bus = &rem_i2c_bus4;
	rem_i2c_mux.i2c_dev.address = PCA9547_I2C_ADDRESS;

	mcp9808[0].i2c_dev.bus = &i2c_bus4;
	mcp9808[0].i2c_dev.channel = CHANNEL_MCP9808_1;
	mcp9808[0].i2c_dev.address = MCP_9808_I2C_ADDRESS;

	mcp9808[1].i2c_dev.bus = &i2c_bus4;
	mcp9808[1].i2c_dev.channel = CHANNEL_MCP9808_2;
	mcp9808[1].i2c_dev.address = MCP_9808_I2C_ADDRESS;

	mcp9808[2].i2c_dev.bus = &rem_i2c_bus4;
	mcp9808[2].i2c_dev.channel = CHANNEL_MCP9808_1;
	mcp9808[2].i2c_dev.address = MCP_9808_I2C_ADDRESS;

	mcp9808[3].i2c_dev.bus = &rem_i2c_bus4;
	mcp9808[3].i2c_dev.channel = CHANNEL_MCP9808_2;
	mcp9808[3].i2c_dev.address = MCP_9808_I2C_ADDRESS;

	/*--U156-- */
	si5338[0].i2c_dev.bus = &i2c_bus4;
	si5338[0].i2c_dev.channel = CHANNEL_SFP_SI5338_0;
	si5338[0].i2c_dev.address = SI5338_I2C_ADDRESS;
	memcpy(&si5338[0].configuration[0][0], &si_cfg_155MHz_4x155MHz_0_1ppm[0][0],
			sizeof(si_cfg_155MHz_4x155MHz_0_1ppm));

	/*--U157-- */
	si5338[1].i2c_dev.bus = &i2c_bus4;
	si5338[1].i2c_dev.channel = CHANNEL_SFP_SI5338_1;
	si5338[1].i2c_dev.address = SI5338_I2C_ADDRESS;
	memcpy(&si5338[1].configuration[0][0], &si_cfg_155MHz_4x155MHz_0_1ppm[0][0],
			sizeof(si_cfg_155MHz_4x155MHz_0_1ppm));

	/*--U155-- */
	si5338[2].i2c_dev.bus = &i2c_bus4;
	si5338[2].i2c_dev.channel = CHANNEL_SFP_SI5338_2;
	si5338[2].i2c_dev.address = SI5338_I2C_ADDRESS;
	memcpy(&si5338[2].configuration[0][0], &si_cfg_167Mhz_167Mhz[0][0],
			sizeof(si_cfg_167Mhz_167Mhz));

	/*todo check configuration*/
	si5338[3].i2c_dev.bus = &rem_i2c_bus4;
	si5338[3].i2c_dev.channel = CHANNEL_SFP_SI5338_0;
	si5338[3].i2c_dev.address = SI5338_I2C_ADDRESS;
	memcpy(&si5338[3].configuration[0][0], &si_cfg_155MHz_4x155MHz_0_1ppm[0][0],
			sizeof(si_cfg_155MHz_4x155MHz_0_1ppm));

	si5338[4].i2c_dev.bus = &rem_i2c_bus4;
	si5338[4].i2c_dev.channel = CHANNEL_SFP_SI5338_1;
	si5338[4].i2c_dev.address = SI5338_I2C_ADDRESS;
	memcpy(&si5338[4].configuration[0][0], &si_cfg_155MHz_4x155MHz_0_1ppm[0][0],
			sizeof(si_cfg_155MHz_4x155MHz_0_1ppm));

	si5338[5].i2c_dev.bus = &rem_i2c_bus4;
	si5338[5].i2c_dev.channel = CHANNEL_SFP_SI5338_2;
	si5338[5].i2c_dev.address = SI5338_I2C_ADDRESS;
	memcpy(&si5338[5].configuration[0][0], &si_cfg_167Mhz_167Mhz[0][0],
			sizeof(si_cfg_167Mhz_167Mhz));

	/* 8 SFPs on BUS4 as well */
	sfp_device_t* _sfp;
	for(if_index = 0; if_index < SFP_COUNT_BUS; if_index++)
	{
		_sfp = ((sfp_device_t*) TRANS_device(if_index).device);
		_sfp->i2c_dev.bus = &i2c_bus4;
		_sfp->i2c_dev.address = SFP_MSA_0_I2C_ADDRESS;
		_sfp->i2c_dev.channel = CHANNEL_SFP_SI5338_0 + if_index;
	}

	/* 8 SFPs on BUS9 as well */
	for(if_index = 0; if_index < SFP_COUNT_BUS; if_index++)
	{
		_sfp = ((sfp_device_t*) TRANS_device(if_index).device);
		_sfp->i2c_dev.bus = &rem_i2c_bus4;
		_sfp->i2c_dev.address = SFP_MSA_0_I2C_ADDRESS;
		_sfp->i2c_dev.channel = CHANNEL_SFP_SI5338_0 + if_index;
	}
	_sfpp_ln = ((sfpp_device_t*) TRANS_device(0).device);
	_sfpp_ln->i2c_dev.bus = &ln0_i2c_bus2;
	_sfpp_ln->i2c_dev.address = SFPP_MSA_0_I2C_ADDRESS;
	_sfpp_ln->i2c_dev.channel = CHANNEL_SFPP_0;

	_sfpp_ln = ((sfpp_device_t*) TRANS_device(1).device);
	_sfpp_ln->i2c_dev.bus = &ln1_i2c_bus3;
	_sfpp_ln->i2c_dev.address = SFPP_MSA_0_I2C_ADDRESS;
	_sfpp_ln->i2c_dev.channel = CHANNEL_SFPP_1;

	/*FPGA 2*/
	_sfpp_ln = ((sfpp_device_t*) TRANS_device(0).device);
	_sfpp_ln->i2c_dev.bus = &rem_ln0_i2c_bus2;
	_sfpp_ln->i2c_dev.address = SFPP_MSA_0_I2C_ADDRESS;
	_sfpp_ln->i2c_dev.channel = CHANNEL_SFPP_0;

	_sfpp_ln = ((sfpp_device_t*) TRANS_device(1).device);
	_sfpp_ln->i2c_dev.bus = &rem_ln1_i2c_bus3;
	_sfpp_ln->i2c_dev.address = SFPP_MSA_0_I2C_ADDRESS;
	_sfpp_ln->i2c_dev.channel = CHANNEL_SFPP_1;

}
/*
 * Function that runs the boot sequence needed to initialize the data path.
 * Return none
 */
void run_boot_sequence(void)
{
	disable_all_line_transceivers_tx();
	printf("Disabling all transceivers output\n");
	/*Delay */
	Delay_ms(500);

	/*Set OTN GXB source to reference */
	LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 1;
	LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 1;


	/* first initialize the clocks of our board */
	configure_board_clocks();
	/*printf("Clocks ready\n"); */
	/* then start configuring the different blocks */
	int if_index = 0;
	for(if_index = CL_INDEX_START; if_index < CLIENTS_IF_COUNT; if_index++)
	{
		if FPGA1
		{
			/*printf("Initializing CLIENT %d:\n", if_index); */
			/*initialize STM1/4 framer*/
			CL_MMAP(if_index)->stm_14_1->config->bits.SYS_RX_SCRAM_EN = 0;
			CL_MMAP(if_index)->stm_14_1->config->bits.SYS_TX_SCRAM_EN = 0;
			/*printf("\tSTM1/4 framer done.\n"); */
			/*initialize STM16 framer*/
			CL_MMAP(if_index)->stm_16_1->config->bits.SYS_RX_SCRAM_EN = 0;
			CL_MMAP(if_index)->stm_16_1->config->bits.SYS_TX_SCRAM_EN = 0;
			/*#warning check if i have a ppm enable bit*/
			//CL_MMAP(if_index)->misc->general_control->bits.PPM_EN = 0;
			/**/
			/*printf("\tSTM16 framer done.\n"); */
			/*initialize GE*/
			init_ge_pcs_ol(CL_MMAP(if_index)->gepcs_1);
			/*printf("\tGE-PCS done.\n"); */
			CL_MMAP(if_index)->gemac_1->mac_address = 0x22334455AA00
					+ (2 * if_index & 0xFF);
			init_ge_mac_ol(CL_MMAP(if_index)->gemac_1);
			CL_MMAP(if_index)->gemac_1->rx_section_full_reg->all = 0x5DC;
			/*printf("\tGE-MAC done.\n"); */
			/*configure GFP*/
			CL_MMAP(if_index)->ge->reset_ctrl_reg_1->bits.GFP_RST = 1;
			Delay_ms(10);
			/*printf("\tGFP done.\n"); */
			CL_MMAP(if_index)->ge->reset_ctrl_reg_1->bits.GFP_RST = 0;
			/*initialize GMP aligner*/
//			CL_MMAP(if_index)->gmp->general_cfg_reg->bits.SW_RESET = 1;
//			Delay_ms(1);
//			CL_MMAP(if_index)->gmp->general_cfg_reg->bits.SW_RESET = 0;
			/*printf("\tGMP done.\n"); */
			/*reset STM1/4 framer*/
			CL_MMAP(if_index)->stm_14_1->reset->bits.SOFT_RESET = 1;
			Delay_ms(1);
			CL_MMAP(if_index)->stm_14_1->reset->bits.SOFT_RESET = 0;
		}
		else
		{
			if FPGA2
			{
				/*printf("Initializing CLIENT %d:\n", if_index); */
				/*initialize STM1/4 framer*/
				CL_MMAP(if_index)->stm_14_2->config->bits.SYS_RX_SCRAM_EN = 0;
				CL_MMAP(if_index)->stm_14_2->config->bits.SYS_TX_SCRAM_EN = 0;
				/*printf("\tSTM1/4 framer done.\n"); */
				/*initialize STM16 framer*/
				CL_MMAP(if_index)->stm_16_2->config->bits.SYS_RX_SCRAM_EN = 0;
				CL_MMAP(if_index)->stm_16_2->config->bits.SYS_TX_SCRAM_EN = 0;
				/*#warning check if i have a ppm enable bit*/
				//CL_MMAP(if_index)->misc->general_control->bits.PPM_EN = 0;
				//*/
				/*printf("\tSTM16 framer done.\n"); */
				/*initialize GE*/
				init_ge_pcs_ol(CL_MMAP(if_index)->gepcs_2);
				/*printf("\tGE-PCS done.\n"); */
				CL_MMAP(if_index)->gemac_2->mac_address = 0x22334455AA00
						+ (2 * if_index & 0xFF);
				init_ge_mac_ol(CL_MMAP(if_index)->gemac_1);
				CL_MMAP(if_index)->gemac_2->rx_section_full_reg->all = 0x5DC;
				/*printf("\tGE-MAC done.\n"); */
				/*configure GFP*/
				CL_MMAP(if_index)->ge->reset_ctrl_reg_2->bits.GFP_RST = 1;
				Delay_ms(10);
				/*printf("\tGFP done.\n"); */
				CL_MMAP(if_index)->ge->reset_ctrl_reg_2->bits.GFP_RST = 0;
				/*initialize GMP aligner*/
	//			CL_MMAP(if_index)->gmp->general_cfg_reg->bits.SW_RESET = 1;
	//			Delay_ms(1);
	//			CL_MMAP(if_index)->gmp->general_cfg_reg->bits.SW_RESET = 0;
				/*printf("\tGMP done.\n"); */
				/*reset STM1/4 framer*/
				CL_MMAP(if_index)->stm_14_2->reset->bits.SOFT_RESET = 1;
				Delay_ms(1);
				CL_MMAP(if_index)->stm_14_2->reset->bits.SOFT_RESET = 0;
		}
	}
	/*Set OTN GXB source to automatic */
	LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 0;
	LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 0;
	/* all is ready, enable transmission on east and west GXBs */
	enable_all_line_transceivers_tx();
	/*printf("Re-enabling all transceivers output\n"); */
  }
}

/*
 * Function that maps port rate to kurs value
 * param[in] _ipglb_rate port rate value
 * Return the kurs data protocol rate
 * */
#ifdef KURS_PROTOCOL
KURS_port_rate_t map_rate_to_kurs_value(port_rate_t _ipglb_rate)
{
	KURS_port_rate_t _kurs_rate;
	switch(_ipglb_rate)
	{
		case PORT_RATE_STM1:
			_kurs_rate = KURS_PORT_RATE_STM1;
			break;
		case PORT_RATE_STM4:
			_kurs_rate = KURS_PORT_RATE_STM4;
			break;
		case PORT_RATE_STM16:
			_kurs_rate = KURS_PORT_RATE_STM16;
			break;
		case PORT_RATE_OTU1:
			_kurs_rate = KURS_PORT_RATE_OTU1;
			break;
		case PORT_RATE_GE:
			_kurs_rate = KURS_PORT_RATE_GE;
			break;
		case PORT_RATE_FE:
			_kurs_rate = KURS_PORT_RATE_FE;
			break;
		case PORT_RATE_FC1:
			_kurs_rate = KURS_PORT_RATE_FC1;
			break;
		case PORT_RATE_FC2:
			_kurs_rate = KURS_PORT_RATE_FC2;
			break;
		default:
			break;
	}
	return (_kurs_rate);
}
/*
 * Function that maps als mode from kurs value
 * param[in] als_mode als mode value
 * Return the als mode
 * */
uint32_t map_als_from_kurs_value(als_mode_t als_mode)
{
#define IPGLB_ALS_AUTO		0
#define IPGLB_ALS_LINE		1
#define IPGLB_ALS_REMOTE	2
	uint32_t ipglb_als_mode;
	switch(als_mode)
	{
		case ALS_CONTROL_LINE: /*Line ALS mode*/
			ipglb_als_mode = IPGLB_ALS_LINE;
			break;
		case ALS_CONTROL_CLIENT:
			ipglb_als_mode = IPGLB_ALS_AUTO;
			break;
		case ALS_CONTROL_REMOTE:
			ipglb_als_mode = IPGLB_ALS_REMOTE;
			break;
		case ALS_CONTROL_DISABLED: /*ALS disabled -
									should be handled elsewhere*/
		default:
			break;
	}
	return (ipglb_als_mode);
#undef IPGLB_ALS_AUTO
#undef IPGLB_ALS_LINE
#undef IPGLB_ALS_REMOTE
}
/*
 * Function that maps port rate from kurs value
 * param[in] _kurs_rate port rate value
 * Return the port rate value
 * */
port_rate_t map_rate_from_kurs_value(KURS_port_rate_t _kurs_rate)
{
	port_rate_t _ipglb_rate;
	switch(_kurs_rate)
	{
		case KURS_PORT_RATE_STM1:
			_ipglb_rate = PORT_RATE_STM1;
			break;
		case KURS_PORT_RATE_STM4:
			_ipglb_rate = PORT_RATE_STM4;
			break;
		case KURS_PORT_RATE_STM16:
			_ipglb_rate = PORT_RATE_STM16;
			break;
		case KURS_PORT_RATE_OTU1:
			_ipglb_rate = PORT_RATE_OTU1;
			break;
		case KURS_PORT_RATE_GE:
			_ipglb_rate = PORT_RATE_GE;
			break;
		case KURS_PORT_RATE_FE:
			_ipglb_rate = PORT_RATE_FE;
			break;
		case KURS_PORT_RATE_FC1:
			_ipglb_rate = PORT_RATE_FC1;
			break;
		case KURS_PORT_RATE_FC2:
			_ipglb_rate = PORT_RATE_FC2;
			break;
		default:
			break;
	}
	return (_ipglb_rate);
}
#endif

/*/////////////////////////////////////////////////////////////////////////// */
/*///////////Section below added for handling PUSK profile 5///////////////// */
/*/////////////////////////////////////////////////////////////////////////// */
/*!<flag which when set, indicates user has uploaded a new language file */
char new_lng_file = 0;

/* This function can not be used in my code because the BP UART address is not
 *  connected to the FPGA. So only the ARM knows the BP address and the NiOS has
 *  to query it. */
/*char get_kurs_address() */
/*{
/*	char pusk_address = 0; */
/*  Delay_ms(10000); //This delay is added because we have seen that the PUSK
 *  address needs some time before getting set */
/*	pusk_address = top.reg0_status->bits.BP_ADDRESS;//Dummy read */
/*	alt_u32 temp = IORD_32DIRECT(TOP_MPIF_0_BASE,0); */
/*	temp = IORD_32DIRECT(TOP_MPIF_0_BASE,0); */
/*	pusk_address = top.reg0_status->bits.BP_ADDRESS; */
/*	return (pusk_address & 0x0F); */
/*} */

/*
 * Function that gets a fresh copy of a parameter value.
 * @param pr	  pointer to the profile instance
 * @param section pointer to the parameter
 * @return 1 on success 0 on failure
 */
char KURS_userget_value(KURS_ProfileTypeDef *pr, profile_param_t *section)
{
	uint32_t rc = 1;
	if((0 == section->param_id) && (0 == section->user_get))
	{
		rc = 0;
	}
	if(1 == rc)
	{
		uint32_t GlID = section->param_id;
		int if_index = 0;
		switch(GlID)
		{
		case ID_2_CL1_SFP_TEMP:
		case ID_2_CL2_SFP_TEMP:
		case ID_2_CL3_SFP_TEMP:
		case ID_2_CL4_SFP_TEMP:
		case ID_2_CL5_SFP_TEMP:
		case ID_2_CL6_SFP_TEMP:
		case ID_2_CL7_SFP_TEMP:
		case ID_2_CL8_SFP_TEMP:
		case ID_2_CL9_SFP_TEMP:
		case ID_2_CL10_SFP_TEMP:
		case ID_2_CL11_SFP_TEMP:
		case ID_2_CL12_SFP_TEMP:
		case ID_2_CL13_SFP_TEMP:
		case ID_2_CL14_SFP_TEMP:
		case ID_2_CL15_SFP_TEMP:
		case ID_2_CL16_SFP_TEMP:
			if_index = (GlID - ID_2_CL1_SFP_TEMP);
			*(float*) section->data = TRANS_MONITOR(if_index).temperature.value;
			break;
#if APS_EN
			case ID_3_AGG_SET_APS_PROT_CHANNEL:
			{
				*(uint32_t*) section->data = (_aps.aps_protectch.current);
			}
			break;
			case ID_3_AGG_SET_APS_REVERSE_MODE:
			{
				*(uint32_t*)section->data = !(_aps.revertive_mode.current);
			}
			break;
			case ID_2_AGG_APS_SIGNAL:
			{
				if(_aps.enabled == 0)
				{
					*(uint32_t*)section->data = APS_PROTECT_NO;
				}
				else
				{
					*(uint32_t*)section->data =
					_aps.assigned_aps_selected_line.current;
				}
			}
			break;
#endif
		case ID_2_CL1_PIN:
		case ID_2_CL2_PIN:
		case ID_2_CL3_PIN:
		case ID_2_CL4_PIN:
		case ID_2_CL5_PIN:
		case ID_2_CL6_PIN:
		case ID_2_CL7_PIN:
		case ID_2_CL8_PIN:
		case ID_2_CL9_PIN:
		case ID_2_CL10_PIN:
		case ID_2_CL11_PIN:
		case ID_2_CL12_PIN:
		case ID_2_CL13_PIN:
		case ID_2_CL14_PIN:
		case ID_2_CL15_PIN:
		case ID_2_CL16_PIN:
		case ID_2_LN1_PIN:
		case ID_2_LN2_PIN:
		{
			if_index = (GlID - ID_2_CL1_PIN);
			*(float*) section->data = pin[if_index].value;
		}
			break;
		case ID_2_CL1_POUT:
		case ID_2_CL2_POUT:
		case ID_2_CL3_POUT:
		case ID_2_CL4_POUT:
		case ID_2_CL5_POUT:
		case ID_2_CL6_POUT:
		case ID_2_CL7_POUT:
		case ID_2_CL8_POUT:
		case ID_2_CL9_POUT:
		case ID_2_CL10_POUT:
		case ID_2_CL11_POUT:
		case ID_2_CL12_POUT:
		case ID_2_CL13_POUT:
		case ID_2_CL14_POUT:
		case ID_2_CL15_POUT:
		case ID_2_CL16_POUT:
		case ID_2_LN1_POUT:
		case ID_2_LN2_POUT:
		{
			if_index = (GlID - ID_2_CL1_POUT);
			*(float*) section->data = pout[if_index].value;
		}
			break;
		case ID_3_CL1_SET_DATA_TYPE:
		case ID_3_CL2_SET_DATA_TYPE:
		case ID_3_CL3_SET_DATA_TYPE:
		case ID_3_CL4_SET_DATA_TYPE:
		case ID_3_CL5_SET_DATA_TYPE:
		case ID_3_CL6_SET_DATA_TYPE:
		case ID_3_CL7_SET_DATA_TYPE:
		case ID_3_CL8_SET_DATA_TYPE:
		case ID_3_CL9_SET_DATA_TYPE:
		case ID_3_CL10_SET_DATA_TYPE:
		case ID_3_CL11_SET_DATA_TYPE:
		case ID_3_CL12_SET_DATA_TYPE:
		case ID_3_CL13_SET_DATA_TYPE:
		case ID_3_CL14_SET_DATA_TYPE:
		case ID_3_CL15_SET_DATA_TYPE:
		case ID_3_CL16_SET_DATA_TYPE:
		{
			if_index = (GlID - ID_3_CL1_SET_DATA_TYPE);
			*(uint32_t*) section->data = map_rate_to_kurs_value(
										 PORT_CTRL(if_index).rate.current);
		}
			break;
		case ID_2_CL1_ALS_STATE:
		case ID_2_CL2_ALS_STATE:
		case ID_2_CL3_ALS_STATE:
		case ID_2_CL4_ALS_STATE:
		case ID_2_CL5_ALS_STATE:
		case ID_2_CL6_ALS_STATE:
		case ID_2_CL7_ALS_STATE:
		case ID_2_CL8_ALS_STATE:
		case ID_2_CL9_ALS_STATE:
		case ID_2_CL10_ALS_STATE:
		case ID_2_CL11_ALS_STATE:
		case ID_2_CL12_ALS_STATE:
		case ID_2_CL13_ALS_STATE:
		case ID_2_CL14_ALS_STATE:
		case ID_2_CL15_ALS_STATE:
		case ID_2_CL16_ALS_STATE:
		case ID_2_LN1_ALS_STATE:
		case ID_2_LN2_ALS_STATE:
		{
			if_index = (GlID - ID_2_CL1_ALS_STATE);
			*(uint32_t*) section->data = als_get_state(ALS(if_index));
		}
			break;
		case ID_3_CL1_SET_ALS_MODE:
		case ID_3_CL2_SET_ALS_MODE:
		case ID_3_CL3_SET_ALS_MODE:
		case ID_3_CL4_SET_ALS_MODE:
		case ID_3_CL5_SET_ALS_MODE:
		case ID_3_CL6_SET_ALS_MODE:
		case ID_3_CL7_SET_ALS_MODE:
		case ID_3_CL8_SET_ALS_MODE:
		case ID_3_CL9_SET_ALS_MODE:
		case ID_3_CL10_SET_ALS_MODE:
		case ID_3_CL11_SET_ALS_MODE:
		case ID_3_CL12_SET_ALS_MODE:
		case ID_3_CL13_SET_ALS_MODE:
		case ID_3_CL14_SET_ALS_MODE:
		case ID_3_CL15_SET_ALS_MODE:
		case ID_3_CL16_SET_ALS_MODE:
		case ID_3_LN1_SET_ALS_MODE:
		case ID_3_LN2_SET_ALS_MODE:
		{
			if_index = (GlID - ID_3_CL1_SET_ALS_MODE);
			*(uint32_t*) section->data = ALS(if_index)->control;
		}
			break;
		case ID_8_CL_1_SDH_ES:
		case ID_8_CL_2_SDH_ES:
		case ID_8_CL_3_SDH_ES:
		case ID_8_CL_4_SDH_ES:
		case ID_8_CL_5_SDH_ES:
		case ID_8_CL_6_SDH_ES:
		case ID_8_CL_7_SDH_ES:
		case ID_8_CL_8_SDH_ES:
		case ID_8_CL_9_SDH_ES:
		case ID_8_CL_10_SDH_ES:
		case ID_8_CL_11_SDH_ES:
		case ID_8_CL_12_SDH_ES:
		case ID_8_CL_13_SDH_ES:
		case ID_8_CL_14_SDH_ES:
		case ID_8_CL_15_SDH_ES:
		case ID_8_CL_16_SDH_ES:
		{
			if_index = (GlID - ID_8_CL_1_SDH_ES);
			if(PORT_PROTOCOL_SDH == PORT_MONITOR(if_index).prot)
			{
				*(uint32_t*) section->data = get_pfc_es(PORT(if_index).pfc);
			}
			else
			{
				*(uint32_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_SDH_SES:
		case ID_8_CL_2_SDH_SES:
		case ID_8_CL_3_SDH_SES:
		case ID_8_CL_4_SDH_SES:
		case ID_8_CL_5_SDH_SES:
		case ID_8_CL_6_SDH_SES:
		case ID_8_CL_7_SDH_SES:
		case ID_8_CL_8_SDH_SES:
		case ID_8_CL_9_SDH_SES:
		case ID_8_CL_10_SDH_SES:
		case ID_8_CL_11_SDH_SES:
		case ID_8_CL_12_SDH_SES:
		case ID_8_CL_13_SDH_SES:
		case ID_8_CL_14_SDH_SES:
		case ID_8_CL_15_SDH_SES:
		case ID_8_CL_16_SDH_SES:
		{
			if_index = (GlID - ID_8_CL_1_SDH_SES);
			if(PORT_PROTOCOL_SDH == PORT_MONITOR(if_index).prot)
			{
				*(uint32_t*) section->data = get_pfc_ses(PORT(if_index).pfc);
			}
			else
			{
				*(uint32_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_SDH_UAS:
		case ID_8_CL_2_SDH_UAS:
		case ID_8_CL_3_SDH_UAS:
		case ID_8_CL_4_SDH_UAS:
		case ID_8_CL_5_SDH_UAS:
		case ID_8_CL_6_SDH_UAS:
		case ID_8_CL_7_SDH_UAS:
		case ID_8_CL_8_SDH_UAS:
		case ID_8_CL_9_SDH_UAS:
		case ID_8_CL_10_SDH_UAS:
		case ID_8_CL_11_SDH_UAS:
		case ID_8_CL_12_SDH_UAS:
		case ID_8_CL_13_SDH_UAS:
		case ID_8_CL_14_SDH_UAS:
		case ID_8_CL_15_SDH_UAS:
		case ID_8_CL_16_SDH_UAS:
		{
			if_index = (GlID - ID_8_CL_1_SDH_UAS);
			if(PORT_PROTOCOL_SDH == PORT_MONITOR(if_index).prot)
			{
				*(uint32_t*) section->data = get_pfc_uas(PORT(if_index).pfc);
			}
			else
			{
				*(uint32_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_SDH_BBE:
		case ID_8_CL_2_SDH_BBE:
		case ID_8_CL_3_SDH_BBE:
		case ID_8_CL_4_SDH_BBE:
		case ID_8_CL_5_SDH_BBE:
		case ID_8_CL_6_SDH_BBE:
		case ID_8_CL_7_SDH_BBE:
		case ID_8_CL_8_SDH_BBE:
		case ID_8_CL_9_SDH_BBE:
		case ID_8_CL_10_SDH_BBE:
		case ID_8_CL_11_SDH_BBE:
		case ID_8_CL_12_SDH_BBE:
		case ID_8_CL_13_SDH_BBE:
		case ID_8_CL_14_SDH_BBE:
		case ID_8_CL_15_SDH_BBE:
		case ID_8_CL_16_SDH_BBE:
		{
			if_index = (GlID - ID_8_CL_1_SDH_BBE);
			if(PORT_PROTOCOL_SDH == PORT_MONITOR(if_index).prot)
			{
				*(uint64_t*) section->data = get_pfc_bbe(PORT(if_index).pfc);
			}
			else
			{
				*(uint64_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_OTU_ES:
		case ID_8_CL_2_OTU_ES:
		case ID_8_CL_3_OTU_ES:
		case ID_8_CL_4_OTU_ES:
		case ID_8_CL_5_OTU_ES:
		case ID_8_CL_6_OTU_ES:
		case ID_8_CL_7_OTU_ES:
		case ID_8_CL_8_OTU_ES:
		case ID_8_CL_9_OTU_ES:
		case ID_8_CL_10_OTU_ES:
		case ID_8_CL_11_OTU_ES:
		case ID_8_CL_12_OTU_ES:
		case ID_8_CL_13_OTU_ES:
		case ID_8_CL_14_OTU_ES:
		case ID_8_CL_15_OTU_ES:
		case ID_8_CL_16_OTU_ES:
		case ID_8_LN_1_OTU_ES:
		case ID_8_LN_2_OTU_ES:
		{
			if_index = (GlID - ID_8_CL_1_OTU_ES);
			if(PORT_RATE_OTU1 == PORT_MONITOR(if_index).rate)
			{
				*(uint32_t*) section->data = get_pfc_es(PORT(if_index).pfc);
			}
			else
			{
				*(uint32_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_OTU_SES:
		case ID_8_CL_2_OTU_SES:
		case ID_8_CL_3_OTU_SES:
		case ID_8_CL_4_OTU_SES:
		case ID_8_CL_5_OTU_SES:
		case ID_8_CL_6_OTU_SES:
		case ID_8_CL_7_OTU_SES:
		case ID_8_CL_8_OTU_SES:
		case ID_8_CL_9_OTU_SES:
		case ID_8_CL_10_OTU_SES:
		case ID_8_CL_11_OTU_SES:
		case ID_8_CL_12_OTU_SES:
		case ID_8_CL_13_OTU_SES:
		case ID_8_CL_14_OTU_SES:
		case ID_8_CL_15_OTU_SES:
		case ID_8_CL_16_OTU_SES:
		case ID_8_LN_1_OTU_SES:
		case ID_8_LN_2_OTU_SES:
		{
			if_index = (GlID - ID_8_CL_1_OTU_SES);
			if(PORT_RATE_OTU1 == PORT_MONITOR(if_index).rate)
			{
				*(uint32_t*) section->data = get_pfc_ses(PORT(if_index).pfc);
			}
			else
			{
				*(uint32_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_OTU_UAS:
		case ID_8_CL_2_OTU_UAS:
		case ID_8_CL_3_OTU_UAS:
		case ID_8_CL_4_OTU_UAS:
		case ID_8_CL_5_OTU_UAS:
		case ID_8_CL_6_OTU_UAS:
		case ID_8_CL_7_OTU_UAS:
		case ID_8_CL_8_OTU_UAS:
		case ID_8_CL_9_OTU_UAS:
		case ID_8_CL_10_OTU_UAS:
		case ID_8_CL_11_OTU_UAS:
		case ID_8_CL_12_OTU_UAS:
		case ID_8_CL_13_OTU_UAS:
		case ID_8_CL_14_OTU_UAS:
		case ID_8_CL_15_OTU_UAS:
		case ID_8_CL_16_OTU_UAS:
		case ID_8_LN_1_OTU_UAS:
		case ID_8_LN_2_OTU_UAS:
		{
			if_index = (GlID - ID_8_CL_1_OTU_UAS);
			if(PORT_RATE_OTU1 == PORT_MONITOR(if_index).rate)
			{
				*(uint32_t*) section->data = get_pfc_uas(PORT(if_index).pfc);
			}
			else
			{
				*(uint32_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_OTU_BBE:
		case ID_8_CL_2_OTU_BBE:
		case ID_8_CL_3_OTU_BBE:
		case ID_8_CL_4_OTU_BBE:
		case ID_8_CL_5_OTU_BBE:
		case ID_8_CL_6_OTU_BBE:
		case ID_8_CL_7_OTU_BBE:
		case ID_8_CL_8_OTU_BBE:
		case ID_8_CL_9_OTU_BBE:
		case ID_8_CL_10_OTU_BBE:
		case ID_8_CL_11_OTU_BBE:
		case ID_8_CL_12_OTU_BBE:
		case ID_8_CL_13_OTU_BBE:
		case ID_8_CL_14_OTU_BBE:
		case ID_8_CL_15_OTU_BBE:
		case ID_8_CL_16_OTU_BBE:
		case ID_8_LN_1_OTU_BBE:
		case ID_8_LN_2_OTU_BBE:
		{
			if_index = (GlID - ID_8_CL_1_OTU_BBE);
			if(PORT_RATE_OTU1 == PORT_MONITOR(if_index).rate)
			{
				*(uint64_t*) section->data = get_pfc_bbe(PORT(if_index).pfc);
			}
			else
			{
				*(uint64_t*) section->data = 0;
			}
		}
			break;
		case ID_8_CL_1_ETH_RX_PKTS:
		case ID_8_CL_2_ETH_RX_PKTS:
		case ID_8_CL_3_ETH_RX_PKTS:
		case ID_8_CL_4_ETH_RX_PKTS:
		case ID_8_CL_5_ETH_RX_PKTS:
		case ID_8_CL_6_ETH_RX_PKTS:
		case ID_8_CL_7_ETH_RX_PKTS:
		case ID_8_CL_8_ETH_RX_PKTS:
		case ID_8_CL_9_ETH_RX_PKTS:
		case ID_8_CL_10_ETH_RX_PKTS:
		case ID_8_CL_11_ETH_RX_PKTS:
		case ID_8_CL_12_ETH_RX_PKTS:
		case ID_8_CL_13_ETH_RX_PKTS:
		case ID_8_CL_14_ETH_RX_PKTS:
		case ID_8_CL_15_ETH_RX_PKTS:
		case ID_8_CL_16_ETH_RX_PKTS:
			if_index = (GlID - ID_8_CL_1_ETH_RX_PKTS);
			if((PORT_RATE_GE == PORT_MONITOR(if_index).rate) ||
			   (PORT_RATE_FE == PORT_MONITOR(if_index).rate))
			{
				*(uint64_t*) section->data = C_RXOK(if_index).current;
			}
			else
			{
				*(uint64_t*) section->data = 0;
			}
			break;
		case ID_8_CL_1_ETH_TX_PKTS:
		case ID_8_CL_2_ETH_TX_PKTS:
		case ID_8_CL_3_ETH_TX_PKTS:
		case ID_8_CL_4_ETH_TX_PKTS:
		case ID_8_CL_5_ETH_TX_PKTS:
		case ID_8_CL_6_ETH_TX_PKTS:
		case ID_8_CL_7_ETH_TX_PKTS:
		case ID_8_CL_8_ETH_TX_PKTS:
		case ID_8_CL_9_ETH_TX_PKTS:
		case ID_8_CL_10_ETH_TX_PKTS:
		case ID_8_CL_11_ETH_TX_PKTS:
		case ID_8_CL_12_ETH_TX_PKTS:
		case ID_8_CL_13_ETH_TX_PKTS:
		case ID_8_CL_14_ETH_TX_PKTS:
		case ID_8_CL_15_ETH_TX_PKTS:
		case ID_8_CL_16_ETH_TX_PKTS:
			if_index = (GlID - ID_8_CL_1_ETH_TX_PKTS);
			if((PORT_RATE_GE == PORT_MONITOR(if_index).rate) ||
			   (PORT_RATE_FE == PORT_MONITOR(if_index).rate))
			{
				*(uint64_t*) section->data = C_TXOK(if_index).current;
			}
			else
			{
				*(uint64_t*) section->data = 0;
			}
			break;
		case ID_8_CL_1_ETH_RX_OCTS:
		case ID_8_CL_2_ETH_RX_OCTS:
		case ID_8_CL_3_ETH_RX_OCTS:
		case ID_8_CL_4_ETH_RX_OCTS:
		case ID_8_CL_5_ETH_RX_OCTS:
		case ID_8_CL_6_ETH_RX_OCTS:
		case ID_8_CL_7_ETH_RX_OCTS:
		case ID_8_CL_8_ETH_RX_OCTS:
		case ID_8_CL_9_ETH_RX_OCTS:
		case ID_8_CL_10_ETH_RX_OCTS:
		case ID_8_CL_11_ETH_RX_OCTS:
		case ID_8_CL_12_ETH_RX_OCTS:
		case ID_8_CL_13_ETH_RX_OCTS:
		case ID_8_CL_14_ETH_RX_OCTS:
		case ID_8_CL_15_ETH_RX_OCTS:
		case ID_8_CL_16_ETH_RX_OCTS:
			if_index = (GlID - ID_8_CL_1_ETH_RX_OCTS);
			if((PORT_RATE_GE == PORT_MONITOR(if_index).rate) ||
			   (PORT_RATE_FE == PORT_MONITOR(if_index).rate))
			{
				*(uint64_t*) section->data = C_RXOCTS(if_index).current;
			}
			else
			{
				*(uint64_t*) section->data = 0;
			}
			break;
		case ID_8_CL_1_ETH_TX_OCTS:
		case ID_8_CL_2_ETH_TX_OCTS:
		case ID_8_CL_3_ETH_TX_OCTS:
		case ID_8_CL_4_ETH_TX_OCTS:
		case ID_8_CL_5_ETH_TX_OCTS:
		case ID_8_CL_6_ETH_TX_OCTS:
		case ID_8_CL_7_ETH_TX_OCTS:
		case ID_8_CL_8_ETH_TX_OCTS:
		case ID_8_CL_9_ETH_TX_OCTS:
		case ID_8_CL_10_ETH_TX_OCTS:
		case ID_8_CL_11_ETH_TX_OCTS:
		case ID_8_CL_12_ETH_TX_OCTS:
		case ID_8_CL_13_ETH_TX_OCTS:
		case ID_8_CL_14_ETH_TX_OCTS:
		case ID_8_CL_15_ETH_TX_OCTS:
		case ID_8_CL_16_ETH_TX_OCTS:
			if_index = (GlID - ID_8_CL_1_ETH_TX_OCTS);
			if((PORT_RATE_GE == PORT_MONITOR(if_index).rate) ||
			   (PORT_RATE_FE == PORT_MONITOR(if_index).rate))
			{
				*(uint64_t*) section->data = C_TXOCTS(if_index).current;
			}
			else
			{
				*(uint64_t*) section->data = 0;
			}
			break;
		case ID_8_CL_1_ETH_RX_ERRS:
		case ID_8_CL_2_ETH_RX_ERRS:
		case ID_8_CL_3_ETH_RX_ERRS:
		case ID_8_CL_4_ETH_RX_ERRS:
		case ID_8_CL_5_ETH_RX_ERRS:
		case ID_8_CL_6_ETH_RX_ERRS:
		case ID_8_CL_7_ETH_RX_ERRS:
		case ID_8_CL_8_ETH_RX_ERRS:
		case ID_8_CL_9_ETH_RX_ERRS:
		case ID_8_CL_10_ETH_RX_ERRS:
		case ID_8_CL_11_ETH_RX_ERRS:
		case ID_8_CL_12_ETH_RX_ERRS:
		case ID_8_CL_13_ETH_RX_ERRS:
		case ID_8_CL_14_ETH_RX_ERRS:
		case ID_8_CL_15_ETH_RX_ERRS:
		case ID_8_CL_16_ETH_RX_ERRS:
			if_index = (GlID - ID_8_CL_1_ETH_RX_ERRS);
			if((PORT_RATE_GE == PORT_MONITOR(if_index).rate) ||
			   (PORT_RATE_FE == PORT_MONITOR(if_index).rate))
			{
				*(uint64_t*) section->data = C_RXCRC(if_index).current;
			}
			else
			{
				*(uint64_t*) section->data = 0;
			}
			break;
		case ID_3_CL1_ENABLE_TX:
		case ID_3_CL2_ENABLE_TX:
		case ID_3_CL3_ENABLE_TX:
		case ID_3_CL4_ENABLE_TX:
		case ID_3_CL5_ENABLE_TX:
		case ID_3_CL6_ENABLE_TX:
		case ID_3_CL7_ENABLE_TX:
		case ID_3_CL8_ENABLE_TX:
		case ID_3_CL9_ENABLE_TX:
		case ID_3_CL10_ENABLE_TX:
		case ID_3_CL11_ENABLE_TX:
		case ID_3_CL12_ENABLE_TX:
		case ID_3_CL13_ENABLE_TX:
		case ID_3_CL14_ENABLE_TX:
		case ID_3_CL15_ENABLE_TX:
		case ID_3_CL16_ENABLE_TX:
		case ID_3_LN1_ENABLE_TX:
		case ID_3_LN2_ENABLE_TX:
		{
			if_index = (GlID - ID_3_CL1_ENABLE_TX);
			if(get_bit_state(TRANS_DEVICETXDIS(if_index).address,
							 TRANS_DEVICETXDIS(if_index).bit) == 0)
			{
				*(uint32_t *) section->data = 1;
			}
			else
			{
				*(uint32_t *) section->data = 0;
			}
		}
			break;
		case ID_2_CL1_SFP_STATE:
		case ID_2_CL2_SFP_STATE:
		case ID_2_CL3_SFP_STATE:
		case ID_2_CL4_SFP_STATE:
		case ID_2_CL5_SFP_STATE:
		case ID_2_CL6_SFP_STATE:
		case ID_2_CL7_SFP_STATE:
		case ID_2_CL8_SFP_STATE:
		case ID_2_CL9_SFP_STATE:
		case ID_2_CL10_SFP_STATE:
		case ID_2_CL11_SFP_STATE:
		case ID_2_CL12_SFP_STATE:
		case ID_2_CL13_SFP_STATE:
		case ID_2_CL14_SFP_STATE:
		case ID_2_CL15_SFP_STATE:
		case ID_2_CL16_SFP_STATE:
		case ID_2_LN1_SFP_STATE:
		case ID_2_LN2_SFP_STATE:
		{
			if_index = (GlID - ID_2_CL1_SFP_STATE);
			if(TRANSCEIVER_STATE_REBOOTING ==
			   TRANS_MONITOR(if_index).state.current)
			{
				/*Display NOT READY in case the SFP was rebooting because
				 *  "REBOOTING" state is not available in the enum list*/
				*(uint32_t*) section->data = TRANSCEIVER_STATE_NOT_READY;
			}
			else
			{
				*(uint32_t*) section->data =
				TRANS_MONITOR(if_index).state.current;
			}
		}
			break;
		case ID_2_CL1_OTU_ALRMS:
		case ID_2_CL2_OTU_ALRMS:
		case ID_2_CL3_OTU_ALRMS:
		case ID_2_CL4_OTU_ALRMS:
		case ID_2_CL5_OTU_ALRMS:
		case ID_2_CL6_OTU_ALRMS:
		case ID_2_CL7_OTU_ALRMS:
		case ID_2_CL8_OTU_ALRMS:
		case ID_2_CL9_OTU_ALRMS:
		case ID_2_CL10_OTU_ALRMS:
		case ID_2_CL11_OTU_ALRMS:
		case ID_2_CL12_OTU_ALRMS:
		case ID_2_CL13_OTU_ALRMS:
		case ID_2_CL14_OTU_ALRMS:
		case ID_2_CL15_OTU_ALRMS:
		case ID_2_CL16_OTU_ALRMS:
		case ID_2_LN1_OTU_ALRMS:
		case ID_2_LN2_OTU_ALRMS:
		{
			if_index = (GlID - ID_2_CL1_OTU_ALRMS);
			if(PORT_MONITOR(if_index).rate != PORT_RATE_OTU1)
			{
			*(uint32_t*) section->data = 0; /*Requirement to show "OK"*/
			}
			else
			{
				*(uint32_t*) section->data = C_OTUXSTS(if_index).all & 0x7F;
			}
		}
			break;
		case ID_2_CL1_SDH_ALRMS:
		case ID_2_CL2_SDH_ALRMS:
		case ID_2_CL3_SDH_ALRMS:
		case ID_2_CL4_SDH_ALRMS:
		case ID_2_CL5_SDH_ALRMS:
		case ID_2_CL6_SDH_ALRMS:
		case ID_2_CL7_SDH_ALRMS:
		case ID_2_CL8_SDH_ALRMS:
		case ID_2_CL9_SDH_ALRMS:
		case ID_2_CL10_SDH_ALRMS:
		case ID_2_CL11_SDH_ALRMS:
		case ID_2_CL12_SDH_ALRMS:
		case ID_2_CL13_SDH_ALRMS:
		case ID_2_CL14_SDH_ALRMS:
		case ID_2_CL15_SDH_ALRMS:
		case ID_2_CL16_SDH_ALRMS:
		{
			if_index = (GlID - ID_2_CL1_SDH_ALRMS);
			if((ISNOTCLIENT_STM1(if_index)) &&
				(ISNOTCLIENT_STM4(if_index))&&
				(ISNOTCLIENT_STM16(if_index)))
			{
				*(uint32_t*) section->data = 0; /*Requirement to show "OK"*/
			}
			else
			{
				*(uint32_t*) section->data = C_STMXSTS(if_index).all & 0xFF;
			}
		}
			break;
		case ID_2_CL1_ETH_ALRMS:
		case ID_2_CL2_ETH_ALRMS:
		case ID_2_CL3_ETH_ALRMS:
		case ID_2_CL4_ETH_ALRMS:
		case ID_2_CL5_ETH_ALRMS:
		case ID_2_CL6_ETH_ALRMS:
		case ID_2_CL7_ETH_ALRMS:
		case ID_2_CL8_ETH_ALRMS:
		case ID_2_CL9_ETH_ALRMS:
		case ID_2_CL10_ETH_ALRMS:
		case ID_2_CL11_ETH_ALRMS:
		case ID_2_CL12_ETH_ALRMS:
		case ID_2_CL13_ETH_ALRMS:
		case ID_2_CL14_ETH_ALRMS:
		case ID_2_CL15_ETH_ALRMS:
		case ID_2_CL16_ETH_ALRMS:
		{
			if_index = (GlID - ID_2_CL1_ETH_ALRMS);
			if(ISNOTCLIENT_GE(if_index) && ISNOTCLIENT_FE(if_index))
			{
				*(uint32_t*) section->data = 0; /*Requirement to show "OK"*/
			}
			else
			{
				*(uint32_t*) section->data = C_ETHALARMS(if_index).all & 0x7;
			}
		}
			break;
		default:
			rc = 0;
		}
	}
	return (rc);
}

//TODO: Finish implementing this function KURS_userset_value
/*
 * Function that changes a parameter to a new value based on user selection.
 *
 * @param[in] PR	pointer to the profile instance
 * @param[in] t		pointer to the parameter
 * @param[in] value	pointer to the new value
 * @return 1 on success
 */
char KURS_userset_value(KURS_ProfileTypeDef *PR, profile_param_t *t,
						void *value)
{
#define ERR	0
#define OK	1
	uint32_t GlID = t->param_id;
	uint32_t rc = OK;
	if(t->param_id == 0)
		return (1);
	int if_index = 0;
	int br_index = 0;
	uint32_t mode = 0;
	switch(GlID)
	{
	/*#if KURS_DVT_CODE
	 case ID_3_NIOS_OPERATION:
	 {
	 uint32_t operation = *(uint32_t*)value;
	 if(operation == 0)
	 { /*READ NIOS address*/
	/*switch(nios_address)
	 {
	 case 0x80000000:
	 nios_data = systicks;
	 break;
	 case 0x80000001:
	 nios_data = card.sys_uptime;
	 break;
	 case 0x80000002:
	 nios_data = counter_ores_arm;
	 break;
	 case 0x80000003:
	 nios_data = counter_ores_dvt;
	 break;
	 case 0x80000004:
	 nios_data = counter_tmr0_run_bit;
	 break;
	 default:
	 nios_data = IORD_32DIRECT(nios_address, 0);
	 break;
	 }
	 }
	 else
	 if(operation == 1)
	 { /*WRITE NIOS address*/
	/*	IOWR_32DIRECT(nios_address, 0, nios_data);
	 } else
	 {
	 rc = ERR;
	 }
	 }
	 break;
	 #endif*/
#if APS_EN
	/*APS parameters*/
	case ID_3_AGG_SET_APS_MODE:
	{
		_aps.aps_mode.change = 1;
	}
		 break;
	case ID_3_AGG_SET_APS_PROT_CHANNEL:
	{
		_aps.aps_protectch.current = *((uint32_t*)value);
		_aps.aps_protectch.change = 1;
	}
		 break;
	case ID_3_AGG_SET_APS_REVERSE_MODE:
	{
		uint32_t _t = _aps.revertive_mode.current;
		_aps.revertive_mode.current = !(*((uint32_t*)value)& 0x01);
		if(aps_set_revert_mode(&_aps)!= RET_SUCCESS)
		{
			rc = ERR;
			/*restore the old value in case of failure*/
			_aps.revertive_mode.current = _t;
		}
	}
		 break;
	case ID_3_AGG_SET_APS_WTR:
	{
		_aps.wtr_timer = *((uint32_t*)value);
		aps_set_wtr(&_aps);
	}
		 break;
	case ID_3_AGG_SET_APS_HO:
	{
		_aps.holdoff_timer = *((uint32_t*)value);
		aps_set_ho(&_aps);
	}
	break;
#endif
	case ID_3_CL1_SET_STATE:
	case ID_3_CL2_SET_STATE:
	case ID_3_CL3_SET_STATE:
	case ID_3_CL4_SET_STATE:
	case ID_3_CL5_SET_STATE:
	case ID_3_CL6_SET_STATE:
	case ID_3_CL7_SET_STATE:
	case ID_3_CL8_SET_STATE:
	case ID_3_CL9_SET_STATE:
	case ID_3_CL10_SET_STATE:
	case ID_3_CL11_SET_STATE:
	case ID_3_CL12_SET_STATE:
	case ID_3_CL13_SET_STATE:
	case ID_3_CL14_SET_STATE:
	case ID_3_CL15_SET_STATE:
	case ID_3_CL16_SET_STATE:
	case ID_3_LN1_SET_STATE:
	case ID_3_LN2_SET_STATE:
	{
		if_index = (GlID - ID_3_CL1_SET_STATE);
		INTERFACE(if_index).service_state.next = *((uint32_t*) value) & 0x03;
		INTERFACE(if_index).service_state.change = 1;
	}
		break;
	case ID_3_CL1_ENABLE_TX:
	case ID_3_CL2_ENABLE_TX:
	case ID_3_CL3_ENABLE_TX:
	case ID_3_CL4_ENABLE_TX:
	case ID_3_CL5_ENABLE_TX:
	case ID_3_CL6_ENABLE_TX:
	case ID_3_CL7_ENABLE_TX:
	case ID_3_CL8_ENABLE_TX:
	case ID_3_CL9_ENABLE_TX:
	case ID_3_CL10_ENABLE_TX:
	case ID_3_CL11_ENABLE_TX:
	case ID_3_CL12_ENABLE_TX:
	case ID_3_CL13_ENABLE_TX:
	case ID_3_CL14_ENABLE_TX:
	case ID_3_CL15_ENABLE_TX:
	case ID_3_CL16_ENABLE_TX:
	case ID_3_LN1_ENABLE_TX:
	case ID_3_LN2_ENABLE_TX:
	{
		if_index = (GlID - ID_3_CL1_ENABLE_TX);
		if((TRANS_MONITOR(if_index).state.current != TRANSCEIVER_STATE_ABSENT)
			&&
			(INTERFACE(if_index).service_state.current != IF_SERVICE_STATE_OOS))
		{
			/*If the port is OOS, TX can not be turned ON, it is OFF*/
			TRANS_CTRLTXDIS(if_index).next = !(*((uint32_t*) value) & 0x01);
			TRANS_CTRLTXDIS(if_index).change = 1;
		}
		else
		{
			rc = ERR;
		}
	}
		break;
	case ID_3_CL1_SET_LOOPBACK:
	case ID_3_CL2_SET_LOOPBACK:
	case ID_3_CL3_SET_LOOPBACK:
	case ID_3_CL4_SET_LOOPBACK:
	case ID_3_CL5_SET_LOOPBACK:
	case ID_3_CL6_SET_LOOPBACK:
	case ID_3_CL7_SET_LOOPBACK:
	case ID_3_CL8_SET_LOOPBACK:
	case ID_3_CL9_SET_LOOPBACK:
	case ID_3_CL10_SET_LOOPBACK:
	case ID_3_CL11_SET_LOOPBACK:
	case ID_3_CL12_SET_LOOPBACK:
	case ID_3_CL13_SET_LOOPBACK:
	case ID_3_CL14_SET_LOOPBACK:
	case ID_3_CL15_SET_LOOPBACK:
	case ID_3_CL16_SET_LOOPBACK:
	{
		if_index = (GlID - ID_3_CL1_SET_LOOPBACK);
		if(TRANS_MONITOR(if_index).state.current != TRANSCEIVER_STATE_ABSENT)
		{
			PORT_CTRL_LB(if_index).next = (*((uint32_t*) value) & 0x03);
			PORT_CTRL_LB(if_index).change = 1;
		}
		else
		{
			rc = ERR;
		}
	}
		break;
	case ID_3_LN1_SET_LOOPBACK:
	{
		if_index = LN_INDEX_START;
		br_index = 0;
#if APS_EN
		/*user can only change loopback in case the APS is OFF or in LO and this
		 *  line is the working*/
		if((!_aps.enabled) ||
			((APS_MODE_LOP == _aps.aps_mode.current) &&
			(APS_PROTECT_LINE_2 == _aps.aps_protectch.current)))
		{
#endif
			if(TRANS_MONITOR(if_index).state.current !=
				TRANSCEIVER_STATE_ABSENT)
			{
				PORT_CTRL_LB(if_index).next = (*((uint32_t*) value) & 0x03);
				PORT_CTRL_LB(if_index).change = 1;
			}
			else
			{
				rc = ERR;
			}
#if APS_EN
	  }
	  else
	  {
		rc = ERR;
	  }
#endif
		break;
	}
	case ID_3_LN2_SET_LOOPBACK:
	{
		if_index = LN_INDEX_END;
		br_index = 0;
#if APS_EN
		/*user can only change loopback in case the APS is OFF or in LO and this line is the working*/
		if((!_aps.enabled) ||
			((APS_MODE_LOP == _aps.aps_mode.current) &&
			(APS_PROTECT_LINE_1 == _aps.aps_protectch.current)))
		{
#endif
			if(TRANS_MONITOR(if_index).state.current !=
				TRANSCEIVER_STATE_ABSENT)
			{
				PORT_CTRL_LB(if_index).next = (*((uint32_t*) value) & 0x03);
				PORT_CTRL_LB(if_index).change = 1;
			}
			else
			{
				rc = ERR;
			}
#if APS_EN
	    }
		else
		{
			rc = ERR;
		}
#endif
		break;
	}
	case ID_3_CL1_SET_DATA_TYPE:
	case ID_3_CL2_SET_DATA_TYPE:
	case ID_3_CL3_SET_DATA_TYPE:
	case ID_3_CL4_SET_DATA_TYPE:
	case ID_3_CL5_SET_DATA_TYPE:
	case ID_3_CL6_SET_DATA_TYPE:
	case ID_3_CL7_SET_DATA_TYPE:
	case ID_3_CL8_SET_DATA_TYPE:
	case ID_3_CL9_SET_DATA_TYPE:
	case ID_3_CL10_SET_DATA_TYPE:
	case ID_3_CL11_SET_DATA_TYPE:
	case ID_3_CL12_SET_DATA_TYPE:
	case ID_3_CL13_SET_DATA_TYPE:
	case ID_3_CL14_SET_DATA_TYPE:
	case ID_3_CL15_SET_DATA_TYPE:
	case ID_3_CL16_SET_DATA_TYPE:
	{
		if_index = (GlID - ID_3_CL1_SET_DATA_TYPE);
		if(PORT_CAP(if_index).MULTI_RATE)
		{
			/*TODO fix mapping between IPGLB values and IPM values*/
			KURS_port_rate_t _kurs_rate = (*((uint32_t*) value) & 0xFFF);
			PORT_CTRL(if_index).rate.next = map_rate_from_kurs_value(_kurs_rate);
/*warning check if i need a special case here*/			
/*			if( ((if_index == 1) || (if_index == 3)) &&
				(IS_CLIENT_OTU1(if_index-1) ||
				 ISCLIENT_STM16(if_index-1) ||
				 ISCLIENT_FC2(if_index-1)) )								 
			{
				PORT_CTRL(if_index).rate.change = 0;
			}
			else
			{*/
				PORT_CTRL(if_index).rate.change = 1;
		/*	}*/
		}
		else
		{
			rc = ERR;
		}
	}
		break;
	case ID_3_CL1_SET_ALS_MODE:
	case ID_3_CL2_SET_ALS_MODE:
	case ID_3_CL3_SET_ALS_MODE:
	case ID_3_CL4_SET_ALS_MODE:
	case ID_3_CL5_SET_ALS_MODE:
	case ID_3_CL6_SET_ALS_MODE:
	case ID_3_CL7_SET_ALS_MODE:
	case ID_3_CL8_SET_ALS_MODE:
	case ID_3_CL9_SET_ALS_MODE:
	case ID_3_CL10_SET_ALS_MODE:
	case ID_3_CL11_SET_ALS_MODE:
	case ID_3_CL12_SET_ALS_MODE:
	case ID_3_CL13_SET_ALS_MODE:
	case ID_3_CL14_SET_ALS_MODE:
	case ID_3_CL15_SET_ALS_MODE:
	case ID_3_CL16_SET_ALS_MODE:
	case ID_3_LN1_SET_ALS_MODE:
	case ID_3_LN2_SET_ALS_MODE:
	{
		if_index = (GlID - ID_3_CL1_SET_ALS_MODE);
		/*!<It can be a value of @ref kurs_als_control_t*/
		mode = *((uint32_t*) value) & 0x03;
		if(ALS(if_index)->set_mode(ALS(if_index), mode) == RET_SUCCESS)
		{
			ALS(if_index)->control = mode;
		}
		else
		{
			rc = ERR;
		}
	}
		break;
	case ID_3_CL1_SFP_THRS_RESET:
	case ID_3_CL2_SFP_THRS_RESET:
	case ID_3_CL3_SFP_THRS_RESET:
	case ID_3_CL4_SFP_THRS_RESET:
	case ID_3_CL5_SFP_THRS_RESET:
	case ID_3_CL6_SFP_THRS_RESET:
	case ID_3_CL7_SFP_THRS_RESET:
	case ID_3_CL8_SFP_THRS_RESET:
	case ID_3_CL9_SFP_THRS_RESET:
	case ID_3_CL10_SFP_THRS_RESET:
	case ID_3_CL11_SFP_THRS_RESET:
	case ID_3_CL12_SFP_THRS_RESET:
	case ID_3_CL13_SFP_THRS_RESET:
	case ID_3_CL14_SFP_THRS_RESET:
	case ID_3_CL15_SFP_THRS_RESET:
	case ID_3_CL16_SFP_THRS_RESET:
	case ID_3_LN1_SFP_THRS_RESET:
	case ID_3_LN2_SFP_THRS_RESET:
		if_index = (GlID - ID_3_CL1_SFP_THRS_RESET);
		if(*(uint32_t *) value == 0)
		{
			if(TRANS_MONITOR(if_index).state.current != TRANSCEIVER_STATE_ABSENT)
			{
				TRANS_CTRL(if_index).threshold_mode = THRESHOLD_TRANSCEIVER;
			}
			else
			{
				rc = ERR;
			}
		}
		break;
	case ID_3_RESET:
		if(*(uint32_t *) value == 0)
		{
			/* Inform the main loop that it has to update the HW to reflect the
			 * new settings*/
			restore = 1;
		}
		break;
	case ID_4_CL1_PIN_CMAX:
	case ID_4_CL2_PIN_CMAX:
	case ID_4_CL3_PIN_CMAX:
	case ID_4_CL4_PIN_CMAX:
	case ID_4_CL5_PIN_CMAX:
	case ID_4_CL6_PIN_CMAX:
	case ID_4_CL7_PIN_CMAX:
	case ID_4_CL8_PIN_CMAX:
	case ID_4_CL9_PIN_CMAX:
	case ID_4_CL10_PIN_CMAX:
	case ID_4_CL11_PIN_CMAX:
	case ID_4_CL12_PIN_CMAX:
	case ID_4_CL13_PIN_CMAX:
	case ID_4_CL14_PIN_CMAX:
	case ID_4_CL15_PIN_CMAX:
	case ID_4_CL16_PIN_CMAX:
	case ID_4_LN1_PIN_CMAX:
	case ID_4_LN2_PIN_CMAX:
		if_index = (GlID - ID_4_CL1_PIN_CMAX);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_4_CL1_PIN_WMAX:
	case ID_4_CL2_PIN_WMAX:
	case ID_4_CL3_PIN_WMAX:
	case ID_4_CL4_PIN_WMAX:
	case ID_4_CL5_PIN_WMAX:
	case ID_4_CL6_PIN_WMAX:
	case ID_4_CL7_PIN_WMAX:
	case ID_4_CL8_PIN_WMAX:
	case ID_4_CL9_PIN_WMAX:
	case ID_4_CL10_PIN_WMAX:
	case ID_4_CL11_PIN_WMAX:
	case ID_4_CL12_PIN_WMAX:
	case ID_4_CL13_PIN_WMAX:
	case ID_4_CL14_PIN_WMAX:
	case ID_4_CL15_PIN_WMAX:
	case ID_4_CL16_PIN_WMAX:
	case ID_4_LN1_PIN_WMAX:
	case ID_4_LN2_PIN_WMAX:
		if_index = (GlID - ID_4_CL1_PIN_WMAX);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_4_CL1_PIN_WMIN:
	case ID_4_CL2_PIN_WMIN:
	case ID_4_CL3_PIN_WMIN:
	case ID_4_CL4_PIN_WMIN:
	case ID_4_CL5_PIN_WMIN:
	case ID_4_CL6_PIN_WMIN:
	case ID_4_CL7_PIN_WMIN:
	case ID_4_CL8_PIN_WMIN:
	case ID_4_CL9_PIN_WMIN:
	case ID_4_CL10_PIN_WMIN:
	case ID_4_CL11_PIN_WMIN:
	case ID_4_CL12_PIN_WMIN:
	case ID_4_CL13_PIN_WMIN:
	case ID_4_CL14_PIN_WMIN:
	case ID_4_CL15_PIN_WMIN:
	case ID_4_CL16_PIN_WMIN:
	case ID_4_LN1_PIN_WMIN:
	case ID_4_LN2_PIN_WMIN:
		if_index = (GlID - ID_4_CL1_PIN_WMIN);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_4_CL1_PIN_CMIN:
	case ID_4_CL2_PIN_CMIN:
	case ID_4_CL3_PIN_CMIN:
	case ID_4_CL4_PIN_CMIN:
	case ID_4_CL5_PIN_CMIN:
	case ID_4_CL6_PIN_CMIN:
	case ID_4_CL7_PIN_CMIN:
	case ID_4_CL8_PIN_CMIN:
	case ID_4_CL9_PIN_CMIN:
	case ID_4_CL10_PIN_CMIN:
	case ID_4_CL11_PIN_CMIN:
	case ID_4_CL12_PIN_CMIN:
	case ID_4_CL13_PIN_CMIN:
	case ID_4_CL14_PIN_CMIN:
	case ID_4_CL15_PIN_CMIN:
	case ID_4_CL16_PIN_CMIN:
	case ID_4_LN1_PIN_CMIN:
	case ID_4_LN2_PIN_CMIN:
		if_index = (GlID - ID_4_CL1_PIN_CMIN);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_4_CL1_POUT_CMAX:
	case ID_4_CL2_POUT_CMAX:
	case ID_4_CL3_POUT_CMAX:
	case ID_4_CL4_POUT_CMAX:
	case ID_4_CL5_POUT_CMAX:
	case ID_4_CL6_POUT_CMAX:
	case ID_4_CL7_POUT_CMAX:
	case ID_4_CL8_POUT_CMAX:
	case ID_4_CL9_POUT_CMAX:
	case ID_4_CL10_POUT_CMAX:
	case ID_4_CL11_POUT_CMAX:
	case ID_4_CL12_POUT_CMAX:
	case ID_4_CL13_POUT_CMAX:
	case ID_4_CL14_POUT_CMAX:
	case ID_4_CL15_POUT_CMAX:
	case ID_4_CL16_POUT_CMAX:
	case ID_4_LN1_POUT_CMAX:
	case ID_4_LN2_POUT_CMAX:
		if_index = (GlID - ID_4_CL1_POUT_CMAX);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_4_CL1_POUT_WMAX:
	case ID_4_CL2_POUT_WMAX:
	case ID_4_CL3_POUT_WMAX:
	case ID_4_CL4_POUT_WMAX:
	case ID_4_CL5_POUT_WMAX:
	case ID_4_CL6_POUT_WMAX:
	case ID_4_CL7_POUT_WMAX:
	case ID_4_CL8_POUT_WMAX:
	case ID_4_CL9_POUT_WMAX:
	case ID_4_CL10_POUT_WMAX:
	case ID_4_CL11_POUT_WMAX:
	case ID_4_CL12_POUT_WMAX:
	case ID_4_CL13_POUT_WMAX:
	case ID_4_CL14_POUT_WMAX:
	case ID_4_CL15_POUT_WMAX:
	case ID_4_CL16_POUT_WMAX:
	case ID_4_LN1_POUT_WMAX:
	case ID_4_LN2_POUT_WMAX:
		if_index = (GlID - ID_4_CL1_POUT_WMAX);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_4_CL1_POUT_WMIN:
	case ID_4_CL2_POUT_WMIN:
	case ID_4_CL3_POUT_WMIN:
	case ID_4_CL4_POUT_WMIN:
	case ID_4_CL5_POUT_WMIN:
	case ID_4_CL6_POUT_WMIN:
	case ID_4_CL7_POUT_WMIN:
	case ID_4_CL8_POUT_WMIN:
	case ID_4_CL9_POUT_WMIN:
	case ID_4_CL10_POUT_WMIN:
	case ID_4_CL11_POUT_WMIN:
	case ID_4_CL12_POUT_WMIN:
	case ID_4_CL13_POUT_WMIN:
	case ID_4_CL14_POUT_WMIN:
	case ID_4_CL15_POUT_WMIN:
	case ID_4_CL16_POUT_WMIN:
	case ID_4_LN1_POUT_WMIN:
	case ID_4_LN2_POUT_WMIN:
		if_index = (GlID - ID_4_CL1_POUT_WMIN);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_4_CL1_POUT_CMIN:
	case ID_4_CL2_POUT_CMIN:
	case ID_4_CL3_POUT_CMIN:
	case ID_4_CL4_POUT_CMIN:
	case ID_4_CL5_POUT_CMIN:
	case ID_4_CL6_POUT_CMIN:
	case ID_4_CL7_POUT_CMIN:
	case ID_4_CL8_POUT_CMIN:
	case ID_4_CL9_POUT_CMIN:
	case ID_4_CL10_POUT_CMIN:
	case ID_4_CL11_POUT_CMIN:
	case ID_4_CL12_POUT_CMIN:
	case ID_4_CL13_POUT_CMIN:
	case ID_4_CL14_POUT_CMIN:
	case ID_4_CL15_POUT_CMIN:
	case ID_4_CL16_POUT_CMIN:
	case ID_4_LN1_POUT_CMIN:
	case ID_4_LN2_POUT_CMIN:
		if_index = (GlID - ID_4_CL1_POUT_CMIN);
		TRANS_CTRL(if_index).threshold_mode = THRESHOLD_USER;
		break;
	case ID_3_RESET_ALL_COUNTERS:
		for(i = 0; i < INTERFACE_COUNT; i++)
		{
			PORT_CTRL(i).reset_counters.change = 1;
		}
		break;
	case ID_3_CL1_RESET_COUNTERS:
	case ID_3_CL2_RESET_COUNTERS:
	case ID_3_CL3_RESET_COUNTERS:
	case ID_3_CL4_RESET_COUNTERS:
	case ID_3_CL5_RESET_COUNTERS:
	case ID_3_CL6_RESET_COUNTERS:
	case ID_3_CL7_RESET_COUNTERS:
	case ID_3_CL8_RESET_COUNTERS:
	case ID_3_CL9_RESET_COUNTERS:
	case ID_3_CL10_RESET_COUNTERS:
	case ID_3_CL11_RESET_COUNTERS:
	case ID_3_CL12_RESET_COUNTERS:
	case ID_3_CL13_RESET_COUNTERS:
	case ID_3_CL14_RESET_COUNTERS:
	case ID_3_CL15_RESET_COUNTERS:
	case ID_3_CL16_RESET_COUNTERS:
	case ID_3_LN1_RESET_COUNTERS:
	case ID_3_LN2_RESET_COUNTERS:
		if_index = (GlID - ID_3_CL1_RESET_COUNTERS);
		PORT_CTRL(if_index).reset_counters.change = 1;
		break;
	default:
		rc = ERR;
		break;
	}
#ifdef PROF_V5
	if(rc == OK)
	{
		/*
		 * In PROF 5.0, we can inform the EMS if a section has updated parameter
		 *  values so it can update its web contents.
		 */
		if(!profile.section_param_changed[TABLE_3_INDEX])
		{
			profile.section_param_changed[TABLE_3_INDEX] = 1;
		}
	}
#endif
	return (rc);
}

/*
 * Function that uses custom logic to calculate the color of a parameter.
 *
 * @param[in] t			pointer to the parameter to color
 * @param[in] T2_ADD	pointer to the additional data of a parameter
 * @return the color of the parameter
 */
char KURS_userget_color(profile_param_t *t, params_t2_add_t *T2_ADD)
{
	profile_param_t *T = t;
	uint32_t GlID = t->param_id;
	uint32_t u32_value = *(uint32_t*) T->data;
	float f_value = *(float*) T->data;
	/* if monitoring is OFF, do not color.*/
	if(*T2_ADD->monitor_on == 0)
	{
		T2_ADD->COLOR = COLOR_NO;
		return (T2_ADD->COLOR);
	}
	int if_index = 0;
	switch(GlID)
	{
	case ID_2_CL1_SFP_TEMP:
	case ID_2_CL2_SFP_TEMP:
	case ID_2_CL3_SFP_TEMP:
	case ID_2_CL4_SFP_TEMP:
	case ID_2_CL5_SFP_TEMP:
	case ID_2_CL6_SFP_TEMP:
	case ID_2_CL7_SFP_TEMP:
	case ID_2_CL8_SFP_TEMP:
	case ID_2_CL9_SFP_TEMP:
	case ID_2_CL10_SFP_TEMP:
	case ID_2_CL11_SFP_TEMP:
	case ID_2_CL12_SFP_TEMP:
	case ID_2_CL13_SFP_TEMP:
	case ID_2_CL14_SFP_TEMP:
	case ID_2_CL15_SFP_TEMP:
	case ID_2_CL16_SFP_TEMP:
	{
		if_index = (GlID - ID_2_CL1_SFP_TEMP);
		if(!TRANS(if_index).info.capabilities.TEMPERATURE_MONITOR)
		{
			T2_ADD->COLOR = COLOR_NO;
		}
		/*Do not color unless the port is IN SERVICE state*/
		else
			if(ACTIVITY_MONITOR_OFF == INTERFACE(if_index).monitor)
			{
				T2_ADD->COLOR = COLOR_NO;
			}
			else
			{
				if((float_compare_gt(f_value,
									 TRANS_MONITORTEMP(if_index).critical_max,
									 0.0000001)) ||
					(float_compare_lt(f_value,
									  TRANS_MONITORTEMP(if_index).critical_min,
									  0.0000001)))
				{
					T2_ADD->COLOR = T2_ADD->COLOR_Failure;
				}
				else
				if((float_compare_gt(f_value,
									  TRANS_MONITORTEMP(if_index).warning_max,
									  0.0000001)) ||
					(float_compare_lt(f_value,
									  TRANS_MONITORTEMP(if_index).warning_min,
									  0.0000001)))
				{
					T2_ADD->COLOR = T2_ADD->COLOR_Degrade;
				}
				else
				{
					T2_ADD->COLOR = COLOR_Normal;
				}
			}
	   }
		break;
	case ID_2_CL1_SFP_STATE:
	case ID_2_CL2_SFP_STATE:
	case ID_2_CL3_SFP_STATE:
	case ID_2_CL4_SFP_STATE:
	case ID_2_CL5_SFP_STATE:
	case ID_2_CL6_SFP_STATE:
	case ID_2_CL7_SFP_STATE:
	case ID_2_CL8_SFP_STATE:
	case ID_2_CL9_SFP_STATE:
	case ID_2_CL10_SFP_STATE:
	case ID_2_CL11_SFP_STATE:
	case ID_2_CL12_SFP_STATE:
	case ID_2_CL13_SFP_STATE:
	case ID_2_CL14_SFP_STATE:
	case ID_2_CL15_SFP_STATE:
	case ID_2_CL16_SFP_STATE:
	case ID_2_LN1_SFP_STATE:
	case ID_2_LN2_SFP_STATE:
		if(TRANSCEIVER_STATE_ABSENT == u32_value)
		{
			T2_ADD->COLOR = T2_ADD->COLOR_Failure;
		}
		else
		if(TRANSCEIVER_STATE_OK == u32_value)
		{
			T2_ADD->COLOR = COLOR_Normal;
		}
		else
		if((TRANSCEIVER_STATE_NOT_READY == u32_value) ||
		   (TRANSCEIVER_STATE_FAIL == u32_value))
		{
			T2_ADD->COLOR = COLOR_Minor;
		}
		break;
	case ID_2_CL1_OTU_ALRMS:
	case ID_2_CL2_OTU_ALRMS:
	case ID_2_CL3_OTU_ALRMS:
	case ID_2_CL4_OTU_ALRMS:
	case ID_2_CL5_OTU_ALRMS:
	case ID_2_CL6_OTU_ALRMS:
	case ID_2_CL7_OTU_ALRMS:
	case ID_2_CL8_OTU_ALRMS:
	case ID_2_CL9_OTU_ALRMS:
	case ID_2_CL10_OTU_ALRMS:
	case ID_2_CL11_OTU_ALRMS:
	case ID_2_CL12_OTU_ALRMS:
	case ID_2_CL13_OTU_ALRMS:
	case ID_2_CL14_OTU_ALRMS:
	case ID_2_CL15_OTU_ALRMS:
	case ID_2_CL16_OTU_ALRMS:
	case ID_2_LN1_OTU_ALRMS:
	case ID_2_LN2_OTU_ALRMS:
		if_index = (GlID - ID_2_CL1_OTU_ALRMS);
		/*Do not color unless the port is IN SERVICE state*/
		if((INTERFACE(if_index).monitor != ACTIVITY_MONITOR_OFF) &&
			(PORT_PROTOCOL_OTU == PORT_MONITOR(if_index).prot))
		{
			if(PORT_MONITOR(if_index).statistics.otux_sts.alarms.all)
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Failure;
			}
			else
			{
				T2_ADD->COLOR = COLOR_Normal;
			}
		}
		else
		{
			T2_ADD->COLOR = COLOR_NO;
		}
		break;
	case ID_2_LN1_ODU_ALRMS:
	case ID_2_LN2_ODU_ALRMS:
		if_index = (GlID - ID_2_LN1_ODU_ALRMS) + LN_0_IF_INDEX;
		/*Do not color unless the port is IN SERVICE state*/
		if((INTERFACE(if_index).monitor != ACTIVITY_MONITOR_OFF) &&
			(PORT_PROTOCOL_OTU == PORT_MONITOR(if_index).prot))
		{
			if(odux_alarms[if_index].all)
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Failure;
			}
			else
			{
				T2_ADD->COLOR = COLOR_Normal;
			}
		}
		else
		{
			T2_ADD->COLOR = COLOR_NO;
		}
		break;
	case ID_2_CL1_SDH_ALRMS:
	case ID_2_CL2_SDH_ALRMS:
	case ID_2_CL3_SDH_ALRMS:
	case ID_2_CL4_SDH_ALRMS:
	case ID_2_CL5_SDH_ALRMS:
	case ID_2_CL6_SDH_ALRMS:
	case ID_2_CL7_SDH_ALRMS:
	case ID_2_CL8_SDH_ALRMS:
	case ID_2_CL9_SDH_ALRMS:
	case ID_2_CL10_SDH_ALRMS:
	case ID_2_CL11_SDH_ALRMS:
	case ID_2_CL12_SDH_ALRMS:
	case ID_2_CL13_SDH_ALRMS:
	case ID_2_CL14_SDH_ALRMS:
	case ID_2_CL15_SDH_ALRMS:
	case ID_2_CL16_SDH_ALRMS:
	{
		if_index = (GlID - ID_2_CL1_SDH_ALRMS);
		/*Do not color unless the port is IN SERVICE state*/
		if((INTERFACE(if_index).monitor != ACTIVITY_MONITOR_OFF) &&
			(PORT_PROTOCOL_SDH == PORT_MONITOR(if_index).prot))
		{
			if(C_STMXSTS(if_index).all)
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Failure;
			}
			else
			{
				T2_ADD->COLOR = COLOR_Normal;
			}
		}
		else
		{
			T2_ADD->COLOR = COLOR_NO;
		}
	}
		break;
	case ID_2_CL1_ETH_ALRMS:
	case ID_2_CL2_ETH_ALRMS:
	case ID_2_CL3_ETH_ALRMS:
	case ID_2_CL4_ETH_ALRMS:
	case ID_2_CL5_ETH_ALRMS:
	case ID_2_CL6_ETH_ALRMS:
	case ID_2_CL7_ETH_ALRMS:
	case ID_2_CL8_ETH_ALRMS:
	case ID_2_CL9_ETH_ALRMS:
	case ID_2_CL10_ETH_ALRMS:
	case ID_2_CL11_ETH_ALRMS:
	case ID_2_CL12_ETH_ALRMS:
	case ID_2_CL13_ETH_ALRMS:
	case ID_2_CL14_ETH_ALRMS:
	case ID_2_CL15_ETH_ALRMS:
	case ID_2_CL16_ETH_ALRMS:
	{
		if_index = (GlID - ID_2_CL1_ETH_ALRMS);
		/*Do not color unless the port is IN SERVICE state*/
		if((INTERFACE(if_index).monitor != ACTIVITY_MONITOR_OFF) &&
			(PORT_PROTOCOL_ETHERNET == PORT_MONITOR(if_index).prot))
		{
			if(C_ETHALARMS(if_index).all)
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Failure;
			}
			else
			{
				T2_ADD->COLOR = COLOR_Normal;
			}
		}
		else
		{
			T2_ADD->COLOR = COLOR_NO;
		}
	}
		break;
	case ID_2_CL1_PIN:
	case ID_2_CL2_PIN:
	case ID_2_CL3_PIN:
	case ID_2_CL4_PIN:
	case ID_2_CL5_PIN:
	case ID_2_CL6_PIN:
	case ID_2_CL7_PIN:
	case ID_2_CL8_PIN:
	case ID_2_CL9_PIN:
	case ID_2_CL10_PIN:
	case ID_2_CL11_PIN:
	case ID_2_CL12_PIN:
	case ID_2_CL13_PIN:
	case ID_2_CL14_PIN:
	case ID_2_CL15_PIN:
	case ID_2_CL16_PIN:
	case ID_2_LN1_PIN:
	case ID_2_LN2_PIN:
	{
		if_index = (GlID - ID_2_CL1_PIN);
		if(!TRANS(if_index).info.capabilities.OPTICAL_POWER_MONITOR)
		{
			T2_ADD->COLOR = COLOR_NO;
		}
		/*Do not color unless the port is IN SERVICE state*/
		else
		if(INTERFACE(if_index).monitor == ACTIVITY_MONITOR_OFF)
		{
			T2_ADD->COLOR = COLOR_NO;
		}
		else
		{
			if((float_compare_gt(f_value, PIN_CMAX(if_index),0.0000001)) ||
			   (float_compare_lt(f_value, PIN_CMIN(if_index),0.0000001)))
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Failure;
			}
			else
			if((float_compare_gt(f_value, PIN_WMAX(if_index),0.0000001)) ||
			   (float_compare_lt(f_value, PIN_WMIN(if_index),0.0000001)))
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Degrade;
			}
			else
			{
				T2_ADD->COLOR = COLOR_Normal;
			}
		}
	}
			break;
	case ID_2_CL1_POUT:
	case ID_2_CL2_POUT:
	case ID_2_CL3_POUT:
	case ID_2_CL4_POUT:
	case ID_2_CL5_POUT:
	case ID_2_CL6_POUT:
	case ID_2_CL7_POUT:
	case ID_2_CL8_POUT:
	case ID_2_CL9_POUT:
	case ID_2_CL10_POUT:
	case ID_2_CL11_POUT:
	case ID_2_CL12_POUT:
	case ID_2_CL13_POUT:
	case ID_2_CL14_POUT:
	case ID_2_CL15_POUT:
	case ID_2_CL16_POUT:
	case ID_2_LN1_POUT:
	case ID_2_LN2_POUT:
		if_index = (GlID - ID_2_CL1_POUT);
		if(!TRANS(if_index).info.capabilities.OPTICAL_POWER_MONITOR)
		{
			T2_ADD->COLOR = COLOR_NO;
		}
		/*Do not color unless the port is IN SERVICE state*/
		else
		if(INTERFACE(if_index).monitor == ACTIVITY_MONITOR_OFF)
		{
			T2_ADD->COLOR = COLOR_NO;
		}
		else
		{

			if((float_compare_gt(f_value, POUT_CMAX(if_index), 0.0000001)) ||
			   (float_compare_lt(f_value, POUT_CMIN(if_index), 0.0000001)))
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Failure;
			}
			else
			if((float_compare_gt(f_value, POUT_WMAX(if_index),0.0000001)) ||
			   (float_compare_lt(f_value, POUT_WMIN(if_index),0.0000001)))
			{
				T2_ADD->COLOR = T2_ADD->COLOR_Degrade;
			}
			else
			{
				T2_ADD->COLOR = COLOR_Normal;
			}
		}
		 break;
	default:
		 break;
	}
	return (T2_ADD->COLOR);
}
/*
* Function that implements additional behavior and monitoring for this project
* that has to update the new lng pack.
* Return none
*/
void KURS_NewLngpackCallback(void)
{
	new_lng_file = 1;
}

/*
 * Function that reads the language file.
 * The language file location is defined
 * in the lang field of the profile_t structure
 * (normally sector 7 of the internal flash 0x8060000).
 * @return None
 */
void KURS_ReadLngFile(KURS_ProfileTypeDef* _profile)
{
	IPGLB_ReadLngFileCallback(&profile.lngpack, sizeof(profile.lngpack));
}

/*
 * Function that writes the language file to flash.
 * param[in] _profile pointer the KURS_ProfileTypeDef object
 * @return None
 */
void KURS_WriteLngFileToFlash(KURS_ProfileTypeDef* _profile)
{
	if(!new_lng_file)
		return;
	IPGLB_WriteLngFileCallback(&profile.lngpack, sizeof(profile.lngpack));
	new_lng_file = 0;
}
/*
 * Function that initialize the profile.
 * @return None
 */
void KURS_profile_init()
{
	/*The profile address will be set inside the main loop after being read
	 *  from the ARM */
	/*	profile.addr_bus = DEF_KURS_ADDRESS|slot_id; */
	memcpy(profile.ver_prof, PROF_VERSION, sizeof(profile.ver_prof));
	memcpy(profile.class_, DEV_CLASS, sizeof(profile.class_));
	memcpy(profile.prefix, DEV_PREFIX, sizeof(profile.prefix));
	profile.line_count = PROFILE_LINES_COUNT;
	memset(profile.lngpack, 0, LANG_BLOCK_SIZE * LANG_BLOCK_MAX_CNT);
	/*TODO: if we need to change the address of the language file */
	/*	profile.lngpack_location = LANG_PACK_OFFSET_IN_FLASH; */
	profile.num_lang = 2; /*English */
	profile.num_bitmap = 5;
	profile.set_param_enabled = 0;
	profile.text = (char**) Profile_Text;
	/*	profile.user_data = NULL; */
	/*	profile.user_data_size = 0; */

#if KURS_S1_EN
	profile.parameters[0] = section_1;
#endif
#if KURS_S2_EN
	profile.parameters[1] = section_2;
#endif
#if KURS_S3_EN
	profile.parameters[2] = section_3;
#endif
#if KURS_S4_EN
	profile.parameters[3] = section_4;
#endif
#if KURS_S5_EN
	profile.parameters[4] = section_5;
#endif
#if KURS_S6_EN
	profile.parameters[5] = NULL;
#endif
#if KURS_S7_EN
	profile.parameters[6] = NULL;
#endif
#ifdef PROF_V5
#if KURS_S8_EN
	profile.parameters[7] = section_8;
#endif
#if KURS_S9_EN
	profile.parameters[8] = NULL;
#endif
#if KURS_S10_EN
	profile.parameters[9] = NULL;
#endif
#if KURS_S11_EN
	profile.parameters[10] = NULL;
#endif
#if KURS_S103_EN
	profile.parameters[11] = section_103;
#endif
#else
	profile.parameters[7] = NULL;
	profile.parameters[8] = NULL;
	profile.parameters[9] = NULL;
	profile.parameters[10] = section_103;
#endif
#if KURS_S1_EN
	profile.section_param_count[TABLE_1_INDEX] = sizeof(section_1)/
												 sizeof(profile_param_t);
#endif
#if KURS_S2_EN
	profile.section_param_count[TABLE_2_INDEX] = sizeof(section_2)/
												 sizeof(profile_param_t);
	/*	profile.section_param_count[1] = 1; */
#endif
#if KURS_S3_EN
	profile.section_param_count[TABLE_3_INDEX] = sizeof(section_3)/
												 sizeof(profile_param_t);
	/*	profile.section_param_count[2] = 1; */
#endif
#if KURS_S4_EN
	profile.section_param_count[TABLE_4_INDEX] = sizeof(section_4)/
												 sizeof(profile_param_t);
#endif
#if KURS_S5_EN
	profile.section_param_count[TABLE_5_INDEX] = sizeof(section_5)/
												 sizeof(profile_param_t);
#endif
#if KURS_S6_EN
	profile.section_param_count[TABLE_6_INDEX] = 0;
#endif
#if KURS_S7_EN
	profile.section_param_count[TABLE_7_INDEX] = 0;
#endif
#ifdef PROF_V5
#if KURS_S8_EN
	profile.section_param_count[TABLE_8_INDEX] = sizeof(section_8)/
												 sizeof(profile_param_t);
#endif
#if KURS_S100_EN
	profile.section_param_count[TABLE_100_INDEX] = 0;
#endif
#if KURS_S101_EN
	profile.section_param_count[TABLE_101_INDEX] = 0;
#endif
#if KURS_S102_EN
	profile.section_param_count[TABLE_102_INDEX] = 0;
#endif
#if KURS_S103_EN
	profile.section_param_count[TABLE_103_INDEX] = sizeof(section_103)/
												   sizeof(profile_param_t);
#endif
#else
	profile.section_param_count[7] = 0;
	profile.section_param_count[8] = 0;
	profile.section_param_count[9] = 0;
	profile.section_param_count[10] = sizeof(section_103)/
									  sizeof(profile_param_t);
#endif

#if KURS_S1_EN
	profile.section_param_changed[TABLE_1_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_1_INDEX] = 0;
#endif
#if KURS_S2_EN
	profile.section_param_changed[TABLE_2_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_2_INDEX] = 0;
#endif
#if KURS_S3_EN
	profile.section_param_changed[TABLE_3_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_3_INDEX] = 0;
#endif
#if KURS_S4_EN
	profile.section_param_changed[TABLE_4_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_4_INDEX] = 0;
#endif
#if KURS_S5_EN
	profile.section_param_changed[TABLE_5_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_5_INDEX] = 0;
#endif
#if KURS_S6_EN
	profile.section_param_changed[TABLE_6_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_6_INDEX] = 0;
#endif
#if KURS_S7_EN
	profile.section_param_changed[TABLE_7_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_7_INDEX] = 0;
#endif
#if KURS_S8_EN
	profile.section_param_changed[TABLE_8_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_8_INDEX] = 0;
#endif
#if KURS_S100_EN
	profile.section_param_changed[TABLE_100_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_100_INDEX] = 0;
#endif
#if KURS_S101_EN
	profile.section_param_changed[TABLE_101_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_101_INDEX] = 0;
#endif
#if KURS_S102_EN
	profile.section_param_changed[TABLE_102_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_102_INDEX] = 0;
#endif
#if KURS_S103_EN
	profile.section_param_changed[TABLE_103_INDEX] = 1;
#else
	profile.section_param_changed[TABLE_103_INDEX] = 0;
#endif
	profile.SetDefault(0xFFFFFFF);

}
/*
 * Function parameters of one or more sections of the profile to their default
 * values.
 * @param[in] selected_sections profile sections which should be reset
 * @return None
 */
void KURS_profile_set_default(uint32_t selected_sections)
{
	int if_index = 0;
	if(selected_sections & 0x8000000)
	{
		memset((void *) card.info.hw_number, 0, sizeof(card.info.hw_number));
		memcpy((void *) card.info.hw_number, DEF_HW_NUM,
				(size_t) sizeof(card.info.hw_number));
		memset((void *) card.info.fpga_sw_number, 0,
				(size_t) sizeof(card.info.fpga_sw_number));
		memset((void *) card.info.sw_number, 0,
				(size_t) sizeof(card.info.sw_number));
		/*Get FPGA HW number */
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			memset(INTERFACE(if_index).destination, 0,
				   sizeof(INTERFACE(if_index).destination));
			memset(TRANS(if_index).info.VN, 0, VN_LENGTH);
			memset(TRANS(if_index).info.SN, 0, SN_LENGTH);
			memset(TRANS(if_index).info.HN, 0, HN_LENGTH);
			memset(TRANS(if_index).info.FN, 0, FN_LENGTH);
		}
	}
	/* section 1 */
	if(selected_sections & 0x01)
	{
		memset((void *) card.info.pid, 0, sizeof(card.info.pid));
		memcpy((void *) card.info.pid, DEF_PID, sizeof(card.info.pid));
		memset((void *) card.info.serial_number, 0,
				sizeof(card.info.serial_number));
		memcpy((void *) card.info.serial_number, DEF_SR_NUM,
				sizeof(card.info.serial_number));
	}
	/*section 4 */
	if(selected_sections & 0x08)
	{
		card.temperature.physical_max = DEF_TEMP_HMAX;
		card.temperature.critical_max = DEF_TEMP_CMAX;
		card.temperature.warning_max = DEF_TEMP_WMAX;
		card.temperature.warning_min = DEF_TEMP_WMIN;
		card.temperature.critical_min = DEF_TEMP_CMIN;
		card.temperature.physical_min = DEF_TEMP_HMIN;
		/*Common parameters */
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			TRANS_MONITORTEMP(if_index).physical_max = DEF_TEMP_HMAX;
			TRANS_MONITORTEMP(if_index).critical_max = DEF_TEMP_CMAX;
			TRANS_MONITORTEMP(if_index).warning_max = DEF_TEMP_WMAX;
			TRANS_MONITORTEMP(if_index).warning_min = DEF_TEMP_WMIN;
			TRANS_MONITORTEMP(if_index).critical_min = DEF_TEMP_CMIN;
			TRANS_MONITORTEMP(if_index).physical_min = DEF_TEMP_HMIN;
		}
		/*Client & Lines parameters */
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			TRANS_CTRL(if_index).threshold_mode = THRESHOLD_TRANSCEIVER;
			pin[if_index].physical_max = DEF_SFP_PIN_HMAX;
			pin[if_index].critical_max = DEF_SFP_PIN_CMAX;
			pin[if_index].warning_max = DEF_SFP_PIN_WMAX;
			pin[if_index].warning_min = DEF_SFP_PIN_WMIN;
			pin[if_index].critical_min = DEF_SFP_PIN_CMIN;
			pin[if_index].physical_min = DEF_SFP_PIN_HMIN;
			pout[if_index].physical_max = DEF_SFP_POUT_HMAX;
			pout[if_index].critical_max = DEF_SFP_POUT_CMAX;
			pout[if_index].warning_max = DEF_SFP_POUT_WMAX;
			pout[if_index].warning_min = DEF_SFP_POUT_WMIN;
			pout[if_index].critical_min = DEF_SFP_POUT_CMIN;
			pout[if_index].physical_min = DEF_SFP_POUT_HMIN;
		}
		/*Line parameters */
		for(if_index = LN_INDEX_START; if_index < INTERFACE_COUNT;if_index++)
		{
			C_FECLOAD(if_index).physical_min = DEF_FEC_LOAD_HMIN;
			C_FECLOAD(if_index).critical_min = DEF_FEC_LOAD_CMIN;
			C_FECLOAD(if_index).critical_max = DEF_FEC_LOAD_CMAX;
			C_FECLOAD(if_index).warning_max = DEF_FEC_LOAD_WMAX;
			C_FECLOAD(if_index).warning_min = DEF_FEC_LOAD_WMIN;
			C_FECLOAD(if_index).physical_max = DEF_FEC_LOAD_HMAX;
		}
	}
	if(selected_sections & 0x02) /*section 2 */
	{
		card.sys_uptime = 0;
		card.alarm1.current = card.alarm1.change = 1;
		card.alarm2.current = card.alarm2.change = 1;
		block_color = 0;
		card.block_state.current = BLOCK_STATE_OK;
		card.temperature.value = 0;
		/*Common parameters */
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			TRANS_MONITORTEMP(if_index).value = DEF_TEMP_HMIN;
			/*this will clear all alarms*/
			PORT_MONITOR(if_index).statistics.generic_sts.alarms.all = 0;
			odux_alarms[if_index].all = 0;
		}
		/*Clients & Lines parameters */
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			pin[if_index].value = DEF_SFP_PIN_HMIN;
			pout[if_index].value = DEF_SFP_POUT_HMIN;
		}
		/*Line parameters */
		for(if_index = LN_INDEX_START; if_index < LN_INDEX_END; if_index++)
		{
			C_FECLOAD(if_index).value = 0;
		}
#if APS_EN
		_aps.assigned_aps_request.current = APS_REQUEST_NR;
		_aps.assigned_aps_selected_line.current = APS_PROTECT_LINE_1;
#endif
	}
	if(selected_sections & 0x04) /* section 3 */
	{
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			INTERFACE(if_index).service_state.current =
			INTERFACE(if_index).service_state.next = IF_SERVICE_STATE_IS;
			INTERFACE(if_index).monitor = ACTIVITY_MONITOR_ON;
			TRANS_CTRL(if_index).tx_disable.current =
			TRANS_CTRL(if_index).tx_disable.next = 0;
			ALS(if_index)->control = ALS_CONTROL_DISABLED;
			PORT_CTRL_LB(if_index).current = PORT_CTRL_LB(if_index).next =
											 LB_CL_NORMAL;
			TRANS_CTRL(if_index).threshold_mode_reset.current = 0;
			PORT_CTRL(if_index).rate.current = PORT_CTRL(if_index).rate.next =
											   PORT_RATE_STM1;
#if APS_EN
			_aps.aps_protectch.current = _aps.aps_protectch.next =
										 APS_PROTECT_LINE_2;
			_aps.revertive_mode.current = APS_REVERTIVE;
			_aps.wtr_timer = 20;
			_aps.holdoff_timer = 0;
			_aps.aps_mode.current = APS_MODE_AUTO;
#endif
		}

		card.reset_settings.current = 0;
		/*    	board_reboot = 0; */
	}
	if(selected_sections & 0x10) /* section 5 */
	{
		board_leds.power_led_g.current = board_leds.status_led.current =
										 COLOR_OFF;
		board_leds.power_led_g.previous = board_leds.status_led.previous =
								 	  	  COLOR_OFF;
	}
	if(selected_sections & 0x20) /*section 103 */
	{
		strcpy(pt_ln_1_in_info, "");
		pt_ln_1_in_cat = 0;
		strcpy(pt_ln_2_in_info, "");
		pt_ln_2_in_cat = 0;
		strcpy(pt_cl_1_in_info, "");
		pt_cl_1_in_cat = 0;
		strcpy(pt_cl_2_in_info, "");
		pt_cl_2_in_cat = 0;
		strcpy(pt_cl_3_in_info, "");
		pt_cl_3_in_cat = 0;
		strcpy(pt_cl_4_in_info, "");
		pt_cl_4_in_cat = 0;
		strcpy(pt_cl_5_in_info, "");
		pt_cl_5_in_cat = 0;
		strcpy(pt_cl_6_in_info, "");
		pt_cl_6_in_cat = 0;
		strcpy(pt_cl_7_in_info, "");
		pt_cl_7_in_cat = 0;
		strcpy(pt_cl_8_in_info, "");
		pt_cl_8_in_cat = 0;
		strcpy(pt_cl_9_in_info, "");
		pt_cl_9_in_cat = 0;
		strcpy(pt_cl_10_in_info, "");
		pt_cl_10_in_cat = 0;
		strcpy(pt_cl_11_in_info, "");
		pt_cl_11_in_cat = 0;
		strcpy(pt_cl_12_in_info, "");
		pt_cl_12_in_cat = 0;
		strcpy(pt_cl_13_in_info, "");
		pt_cl_13_in_cat = 0;
		strcpy(pt_cl_14_in_info, "");
		pt_cl_14_in_cat = 0;
		strcpy(pt_cl_15_in_info, "");
		pt_cl_15_in_cat = 0;
		strcpy(pt_cl_16_in_info, "");
		pt_cl_16_in_cat = 0;
	}
	if(selected_sections & 0x40) /*section 8 */
	{
	}
}
/**
 * Function for kurs handler
 * @param[in] context
 * @return kurs_ticks: number of kurs ticks
 */
alt_u32 kurs_handler(void* context)
{
	handle_kurs_packet();
	return (kurs_ticks);
}
/**
 * Function for dvt handler
 * @param[in] context
 * @return dvt_ticks: number of dvt ticks
 */
alt_u32 dvt_handler(void* context)
{
	handle_debug_packet();
	return (dvt_ticks);
}
/**
 * Function for arm handler
 * @param[in] context
 * @return arm_ticks: number of arm ticks
 */
alt_u32 arm_handler(void* context)
{
	uint16_t d = 0;
	if(ring_buffer_get(rb_data_to_kurs_uart, &d) != RET_FAIL)
	{ /* no more characters*/
		arm_uart.uart_tx_reg->all = (d);
	}
	return (arm_ticks);
}
/**
 * Function to reset nios
 * @return none
 */
void reset_nios()
{
	/*TODO ask Ali to implement this bit*/
	/*top_level_misc_reg.reg1_config->bits.FORCE_RESET = 0;
	 top.reg1_config->bits.FORCE_RESET = 1;*/
	jump_to_boot_copier((void (*)(void)) (NIOS2_RESET_ADDR));
}
/**
 * Function to jump to boot copier
 * param[in] target a function to step to it as a new application
 * @return none
 */
void jump_to_boot_copier(void target(void))
{
	/*
	 * If you have any outstanding I/O or system resources that needed to be
	 * cleanly disabled before leaving the boot copier program, then this is
	 * the place to do that.
	 *
	 * In this example we only need to ensure the state of the Nios II cpu is
	 * equivalent to reset.  If we disable interrupts, and flush the caches,
	 * then the program we jump to should receive the cpu just as it would
	 * coming out of a hardware reset.
	 */
	alt_irq_disable_all();
	alt_dcache_flush_all();
	alt_icache_flush_all();

	/*
	 * The cpu state is as close to reset as we can get it, so we jump to the
	 *  new application.
	 */
	target();
	/*
	 * In the odd event that the program we jump to decides to return, we should
	 * probably just jump back to the reset vector. We pass in the reset address
	 * as a function pointer.
	 */

	/* Turn on an LED to indicate we are resetting */
	/*  IOWR_ALTERA_AVALON_PIO_DATA(PERIPHERAL_SUBSYSTEM_LED_PIO_BASE, 0x4); */
	/* Wait 5 seconds */
	usleep(5000000);

	/* Jump back to the reset address */
	jump_to_boot_copier((void (*)(void)) (NIOS2_RESET_ADDR));
}
/**
 * Function to load a new image if valid
 * @return none
 */
void load_new_image_if_valid()
{
	/*Calculate CRC for the remote program */
	alt_u16 flash_crc_calculated = 0;
	alt_u16 flash_crc_read = 0;
	flash_crc_calculated = calc_crc16((char*)(FLASH_TRISTATE_CONTROLLER_0_BASE +
					                          REMOTE_IMAGE_OFFSET_IN_FLASH),
			                                  REMOTE_IMAGE_SIZE_IN_FLASH - 4);
	flash_crc_read = IORD_8DIRECT(FLASH_TRISTATE_CONTROLLER_0_BASE,
							      REMOTE_IMAGE_OFFSET_IN_FLASH +
							      REMOTE_IMAGE_SIZE_IN_FLASH - 4);
	flash_crc_read |= (alt_u16)(IORD_8DIRECT(FLASH_TRISTATE_CONTROLLER_0_BASE,
			                                 REMOTE_IMAGE_OFFSET_IN_FLASH +
			                                 REMOTE_IMAGE_SIZE_IN_FLASH - 3))<<8;

	if(flash_crc_read != flash_crc_calculated)
	{
		/*Do nothing because there was an error in copying the new image to the
		 *  flash */
		return;
	}
	else /*there is a new image in the flash */
	{
		reset_nios();
	}
}
/**
 * Function to queue data to arm
 * param[in] d data value
 * @return none
 */
void queue_data_to_arm(uint16_t d)
{
	uint16_t _d = d & 0x1FF;
	alt_irq_context cntxt = alt_irq_disable_all();
	if(ring_buffer_put(rb_data_to_kurs_uart, &_d) == RET_SUCCESS)
	{
		/*enable TXE interrupts*/
		arm_uart.uart_cntrl_reg->bits.ITRDY = 1;
	}
	alt_irq_enable_all(cntxt);
}
/**
 * Function to queue many data to arm
 * param[in] data pointer to data value
 * param[in] count data lenght
 * @return none
 */
void queue_many_to_arm(uint16_t* data, uint32_t count)
{
	alt_irq_context cntxt = alt_irq_disable_all();
	if(ring_buffer_put_many(rb_data_to_kurs_uart, data, count) == RET_SUCCESS)
	{
		/*enable TXE interrupts*/
		arm_uart.uart_cntrl_reg->bits.ITRDY = 1;
	}
	alt_irq_enable_all(cntxt);
}
/*void queue_data_from_arm(uint16_t d){ */
/*	uint16_t _d = d; */
/*//	__disable_irq(); */
/*	if(ring_buffer_put(rbDataFromArmUart, &_d)==0){ */
/*//		USART_ITConfig(DBG_COM, USART_IT_TXE, ENABLE);
 *  // enable TX interrupts if we succeeded to add the byte */
/*	} */
/*//	__enable_irq(); */
/*} */

#if ENABLE_ALS_CODE
/**
 * Function that set port als mode
 * @param[in] _als
 * @param[in] als_mode als set state
 * @return RET_SUCCESS in case of success
 * */
/*#warning update lines after having the lines als mapping*/
ret_code_t set_port_als_mode(void* _als, als_mode_t als_mode)
{
	ret_code_t rc = RET_SUCCESS;
	als_t* als = _als;
	if(_als == NULL)
	{
		rc = RET_INVALID_PARAMS;
	}
	else
	{
		if(als_mode == ALS_CONTROL_DISABLED)
		{
			rc = als_disable(_als);
		}
		else
		{
			rc = als_enable(_als);
			uint32_t ipglb_als_mode = map_als_from_kurs_value(als_mode);
			/*write mode to HW*/
			switch(als->index)
			{
			    case CL_0_IF_INDEX:
				case CL_1_IF_INDEX:
				case CL_2_IF_INDEX:
				case CL_3_IF_INDEX:
				case CL_4_IF_INDEX:
				case CL_5_IF_INDEX:
				case CL_6_IF_INDEX:
				case CL_7_IF_INDEX:
					CL_MMAP(als->index)->odtu_brick->als_ctrl_reg_1->bits.ALS_MODE = ipglb_als_mode;
					break;
				case CL_8_IF_INDEX:
				case CL_9_IF_INDEX:
				case CL_10_IF_INDEX:
				case CL_11_IF_INDEX:
				case CL_12_IF_INDEX:
				case CL_13_IF_INDEX:
				case CL_14_IF_INDEX:
				case CL_15_IF_INDEX:
					CL_MMAP(als->index)->odtu_brick->als_ctrl_reg_2->bits.ALS_MODE = ipglb_als_mode;
					break;
				case LN_0_IF_INDEX:
				case LN_1_IF_INDEX:
					/*
					 * for lines, ALS can only be enabled in LINE mode
					 * (which is automatically done in HW once ALS is enabled)
					 */
					break;
				default:
					rc = RET_FAIL;
					break;
			}
		}
	}
	/* When Client ALS mode is in remote, hardware should report local LOS
	 * to the far end. Current HW uses Line GCC2 messages for that purpose
	 * and requires, when running in this mode (CL=OTU1 and ALS=remote)
	 * to LOAD GCC2 (on both Line1 and Line2 of each brick*/
	if(rc == RET_SUCCESS)
	{
		/*#warning check if i will need this check*/
		/* only check for clients that can be configured as OTU1
		 * if we do not do this check, then clients 2 and 4 will
		 * override this functionality*/
		if((als->index == CL_0_IF_INDEX) || (als->index == CL_2_IF_INDEX))
		{
			uint32_t br_idx = (als->index)/2;
			if((IS_CLIENT_OTU1((als->index))) &&
			   (als_mode == ALS_CONTROL_REMOTE))
			{
				report_local_los_to_fe(br_idx, 1);
			}
			else
			{
				report_local_los_to_fe(br_idx, 0);
			}
		}
	}
	return (rc);
}
/**
 * Function to report local los to fe
 * @param[in] br_idx brick index
 * @param[in] enable 1 if enabled 0 if disabled
 * @return none
 * */
/*warning check if i will need this fct*/
void report_local_los_to_fe(uint32_t br_idx, uint32_t enable)
{
	uint32_t ln1_if_idx = br_idx * 2;
	uint32_t ln2_if_idx = ln1_if_idx + 1;
	if(enable)
	{
//		LN_MMAP(ln1_if_idx)->otu1_mapper->ln_cfg1_reg->bits.LOAD_GCC2 = 1;
//		LN_MMAP(ln2_if_idx)->otu1_mapper->ln_cfg1_reg->bits.LOAD_GCC2 = 1;
	}
	else
	{
//		LN_MMAP(ln1_if_idx)->otu1_mapper->ln_cfg1_reg->bits.LOAD_GCC2 = 0;
//		LN_MMAP(ln2_if_idx)->otu1_mapper->ln_cfg1_reg->bits.LOAD_GCC2 = 0;
	}
}
/**
 * Function to initialize als module
 * @param[in] _if_ptr pointer to interface object
 * @return none
 */
/*#warning update lines having the lines als mapping*/
void init_als_module(interface_t* _if)
{
	int if_index = _if->port.index;
	card.interfaces[if_index].port.als = init_als_data(card.interfaces[if_index].port.als);
	set_als_index(_if->port.als, if_index);
	if(if_index < CLIENTS_IF_COUNT)
	{
		if FPGA1
		{
			_if->port.als->io_enable.address 			= (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_1);
			_if->port.als->io_enable.bit				= BIT11;
			_if->port.als->io_auto_enable.address 		= (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_1);
			_if->port.als->io_auto_enable.bit			= BIT10;
			_if->port.als->io_manual_restart.address 	= (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_1);
			_if->port.als->io_manual_restart.bit		= BIT9;
			_if->port.als->io_active.address 		    = (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_1);
			_if->port.als->io_active.bit			    = BIT31;
		}
		else
		{
			if FPGA2
			{
				_if->port.als->io_enable.address 			= (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_2);
				_if->port.als->io_enable.bit				= BIT11;
				_if->port.als->io_auto_enable.address 		= (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_2);
				_if->port.als->io_auto_enable.bit			= BIT10;
				_if->port.als->io_manual_restart.address 	= (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_2);
				_if->port.als->io_manual_restart.bit		= BIT9;
				_if->port.als->io_active.address 		    = (uint32_t)(CL_MMAP(if_index)->odtu_brick->als_ctrl_reg_2);
				_if->port.als->io_active.bit			    = BIT31;
			}
		}
	}
	else
	{
		/*#warning To be done after teddy give me the line als location
			_if->port.als->io_enable.address 			= (uint32_t)top_level_misc_reg.sdh_rate_config;
			_if->port.als->io_enable.bit				= BIT31;
			_if->port.als->io_auto_enable.address 		= (uint32_t)top_level_misc_reg.sdh_rate_config;
			_if->port.als->io_auto_enable.bit			= BIT15;
			_if->port.als->io_manual_restart.address 	= (uint32_t)top_level_misc_reg.sdh_rate_config;
			_if->port.als->io_manual_restart.bit		= BIT11;
			_if->port.als->io_active.address 			= (uint32_t)top_level_misc_reg.reg6_status;
			_if->port.als->io_active.bit				= BIT31;
			*/
	}
	/*set ALS mode callback function*/
	_if->port.als->set_mode = set_port_als_mode;
}
#endif

/*/////////////////////////////////////////////////////////////////////////// */
/*///////////Section above added for handling PUSK profile 5///////////////// */
/*/////////////////////////////////////////////////////////////////////////// */

/* <summary> */
/* Apply the SiLab changes needed to switch from GE to SONET */
/* </summary> */
/* <param name="client_index">index of the port that we want to change its
 * configuration</param> */
/*<returns></returns> 52, 63, 74, 85 */

/**
 * Function to switch to sonet
 * @param[in] _port_ptr pointer to port object
 * @return none
 */
void switch_to_sonet(port_t* _port_ptr)
{
	int _index = _port_ptr->index;
	//int _br_index = 0;
	int8_t err = 0;
	/*for more information on how the SILABs configurations should be modified,
	 * review the clocking diagram.*/
	switch(_index)
	{
		case CL_0_IF_INDEX:
			/*_br_index = 0;
			LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[0].portA_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_1_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[0].portB_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_2_IF_INDEX:
			/*_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_3_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[0].portD_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_4_IF_INDEX:
		/*	_br_index = 0;
			LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[1].portA_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_5_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[1].portB_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_6_IF_INDEX:
		/*	_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[1].portC_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_7_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[1].portD_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_8_IF_INDEX:
	/*		_br_index = 0;
			LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[2].portA_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_9_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[2].portB_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_10_IF_INDEX:
/*			_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[2].portC_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_11_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[2].portD_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_12_IF_INDEX:
				/*_br_index = 0;
				LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
				LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
				/*Modify the SI5375 PORT A output configuration.*/
				memcpy(&si5375[3].portA_configuration[0][0],
					   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
				break;
		case CL_13_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[3].portB_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_14_IF_INDEX:
			/*_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[3].portC_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
		case CL_15_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[3].portD_configuration[0][0],
				   &si5375_155_52_155_52[0][0], sizeof(si5375_155_52_155_52));
			break;
	default:
		err = 1;
		break;
	}
	if(err == 0)
	{
		/*disable all adjustments that were made before this point FPGA1*/
		ipg_si5338_disable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		/*disable all adjustments that were made before this point FPGA2*/
		ipg_si5338_disable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);
		/*Apply the new SI5375 configuration.*/
		ipg_si5375_config(&si5375[0], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[1], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[2], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[3], PORT_A + _index);
			Delay_ms(2);
	}
}

/* <summary> */
/* Apply the SiLab changes needed to switch from GE to SONET */
/* </summary> */
/* <param name="client_index">index of the port that we want to change its
 *  configuration</param> */
/*/ <returns></returns> 52, 63, 74, 85 */

/**
 * Function to switch to ge
 * @param[in] _port_ptr pointer to port object
 * @return none
 */
void switch_to_ge(port_t* _port_ptr)
{
	int _index = _port_ptr->index;
	int _br_index = 0;
	int8_t err = 0;
	/*for more information on how the SILABs configurations should be modified,
	 * review the clocking diagram.*/
	switch(_index)
	{
		case CL_0_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[0].portA_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			/*LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			break;
		case CL_1_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[0].portB_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_2_IF_INDEX:
			/*_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_3_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[0].portD_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_4_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[1].portA_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			/*LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			break;
		case CL_5_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[1].portB_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_6_IF_INDEX:
			/*_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[1].portC_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_7_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[1].portD_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_8_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[2].portA_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			/*LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			break;
		case CL_9_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[2].portB_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_10_IF_INDEX:
			/*_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[2].portC_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_11_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[2].portD_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_12_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[3].portA_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			/*LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			break;
		case CL_13_IF_INDEX:
			_br_index = 0;
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[3].portB_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_14_IF_INDEX:
			/*_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[3].portC_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		case CL_15_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[3].portD_configuration[0][0],
				   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
			break;
		default:
			err = 1;
			break;
	}
	if(err == 0)
	{
		/*disable all adjustments that were made before this point FPGA1*/
		ipg_si5338_disable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);

		/*disable all adjustments that were made before this point FPGA2*/
		ipg_si5338_disable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);

		/*Apply the new SI5375 configuration.*/
		ipg_si5375_config(&si5375[0], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[1], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[2], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[3], PORT_A + _index);
		Delay_ms(2);
	}
}
/// <summary>
/// Apply the SiLab changes needed to switch to OTU1
/// </summary>
/// <param name="client_index">index of the port that we want to change its configuration</param>
/// <returns></returns> 52, 63, 74, 85
void switch_to_otu1(port_t* _port)
{
	int _index = _port->index;
	/*int _br_index = 0;*/
	int8_t err = 0;
	/*for more information on how the SILABs configurations should be modified,
	 * review the clocking diagram.*/
	switch(_index)
	{
		case CL_0_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[0].portA_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			/*LN_MMAP(_br_index)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			break;
		case CL_1_IF_INDEX:
			/*_br_index = 0;*/
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[0].portB_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_2_IF_INDEX:
			/*_br_index = 1;
			LN_MMAP(_br_index+1)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;
			LN_MMAP(_br_index+2)->otu1_mapper->ln_cfg_reg->bits.FC2_MODE = 0;*/
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_3_IF_INDEX:
			/*_br_index = 1;*/
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_4_IF_INDEX:
				/*_br_index = 0;*/
				/*Modify the SI5375 PORT A output configuration.*/
				memcpy(&si5375[1].portA_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_5_IF_INDEX:
				/*_br_index = 0;*/
				/*Modify the SI5375 PORT B output configuration.*/
				memcpy(&si5375[1].portB_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_6_IF_INDEX:
				/*_br_index = 1;*/
				/*Modify the SI5375 PORT C output configuration.*/
				memcpy(&si5375[1].portC_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_7_IF_INDEX:
				/*_br_index = 1;*/
				/*Modify the SI5375 PORT D output configuration.*/
				memcpy(&si5375[1].portD_configuration[0][0],
					   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
				break;
			case CL_8_IF_INDEX:
				/*Modify the SI5375 PORT A output configuration.*/
				memcpy(&si5375[2].portA_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_9_IF_INDEX:
				/*Modify the SI5375 PORT B output configuration.*/
				memcpy(&si5375[2].portB_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_10_IF_INDEX:
				/*Modify the SI5375 PORT C output configuration.*/
				memcpy(&si5375[2].portC_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_11_IF_INDEX:
				/*Modify the SI5375 PORT D output configuration.*/
				memcpy(&si5375[2].portD_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_12_IF_INDEX:
				/*Modify the SI5375 PORT A output configuration.*/
				memcpy(&si5375[3].portA_configuration[0][0],
					   &si5375_155_52_125[0][0], sizeof(si5375_155_52_125));
				break;
			case CL_13_IF_INDEX:
				/*Modify the SI5375 PORT B output configuration.*/
				memcpy(&si5375[3].portB_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_14_IF_INDEX:
				/*Modify the SI5375 PORT C output configuration.*/
				memcpy(&si5375[3].portC_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
			case CL_15_IF_INDEX:
				/*Modify the SI5375 PORT D output configuration.*/
				memcpy(&si5375[3].portD_configuration[0][0], &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
				break;
		default:
			err = 1;
		break;
	}
	if (err == 0)
	{
		/*disable all adjustments that were made before this point FPGA1*/
		ipg_si5338_disable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		/*disable all adjustments that were made before this point FPGA2*/
		ipg_si5338_disable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);
		/*Apply the new SI5375 configuration.*/
		ipg_si5375_config(&si5375[0], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[1], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[2], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[3], PORT_A + _index);
		Delay_ms(2);
	}

}

/*/ <summary> */
/*/ Apply the SiLab changes needed to switch to OTU1 */
/*/ </summary> */
/*/ <param name="client_index">index of the port that we want to change its
 *  configuration</param> */
/*/ <returns></returns> 52, 63, 74, 85 */

/**
 * Function to switch to otu2
 * @param[in] _port_ptr pointer to port object
 * @return none
 */
void switch_to_otu2(port_t* _port_ptr)
{
	int _index = _port_ptr->index;
	int8_t err = 0;
	/*for more information on how the SILABs configurations should be modified,
	 * review the clocking diagram.*/
	switch(_index)
	{
		case CL_0_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[0].portA_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_1_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[0].portB_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_2_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_3_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_4_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[1].portA_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_5_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[1].portB_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_6_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[1].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_7_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[1].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_8_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[2].portA_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_9_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[2].portB_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_10_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[2].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_11_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[2].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;

		case CL_12_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[3].portA_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_13_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[3].portB_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_14_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[3].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
		case CL_15_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[3].portC_configuration[0][0],
				   &si5375_155_52_166_62[0][0], sizeof(si5375_155_52_166_62));
			break;
	default:
		err = 1;
		break;
	}
	if(err == 0)
	{
		/*disable all adjustments that were made before this point FPGA1*/
		ipg_si5338_disable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		/*disable all adjustments that were made before this point FPGA2*/
		ipg_si5338_disable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[3], _index);
		Delay_ms(2);
		/*Apply the new SI5375 configuration.*/
		ipg_si5375_config(&si5375[0], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[1], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[2], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[3], PORT_A + _index);
		Delay_ms(2);
	}

}

/*/ <summary> */
/*/ Apply the SiLab changes needed to switch to FC1 */
/*/ </summary> */
/*/ <param name="client_index">index of the port that we want to change its configuration</param> */
/*/ <returns></returns> 52, 63, 74, 85 */

/**
 * Function to switch to fc1
 * @param[in] _port_ptr pointer to port object
 * @return none
 */
void switch_to_fc1(port_t* _port_ptr)
{
	int _index = _port_ptr->index;
	int8_t err = 0;
	/*for more information on how the SILABs configurations should be modified,
	 * review the clocking diagram.*/
	switch(_index)
	{
		case CL_0_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[0].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_1_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[0].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_2_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_3_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[0].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_4_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[1].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_5_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[1].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_6_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[1].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_7_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[1].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_8_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[2].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_9_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[2].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_10_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[2].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_11_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[2].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_12_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[3].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_13_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[3].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_14_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[3].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_15_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[3].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		default:
			err = 1;
			break;
	}
	if(err == 0)
	{
		/*disable all adjustments that were made before this point*/
		ipg_si5338_disable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		/*Apply the new SI5375 configuration.*/
		ipg_si5375_config(&si5375[0], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[1], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[2], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[3], PORT_A + _index);
		Delay_ms(2);
	}
}

///*/ <summary> */
///*/ Apply the SiLab changes needed to switch to FC2 */
///*/ </summary> */
///*/ <param name="client_index">index of the port that we want to change its configuration</param> */
///*/ <returns></returns> 52, 63, 74, 85 */
//
/**
 * Function to switch to fc2
 * @param[in] _port_ptr pointer to port object
 * @return none
 */
void switch_to_fc2(port_t* _port_ptr)
{
	int _index = _port_ptr->index;
	int8_t err = 0;
	/*for more information on how the SILABs configurations should be modified,
	 * review the clocking diagram.*/
	switch(_index)
	{
		case CL_0_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[0].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_1_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[0].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_2_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[0].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_3_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[0].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_4_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[1].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_5_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[1].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_6_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[1].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_7_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[1].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_8_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[2].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_9_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[2].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_10_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[2].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_11_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[2].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_12_IF_INDEX:
			/*Modify the SI5375 PORT A output configuration.*/
			memcpy(&si5375[3].portA_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_13_IF_INDEX:
			/*Modify the SI5375 PORT B output configuration.*/
			memcpy(&si5375[3].portB_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_14_IF_INDEX:
			/*Modify the SI5375 PORT C output configuration.*/
			memcpy(&si5375[3].portC_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		case CL_15_IF_INDEX:
			/*Modify the SI5375 PORT D output configuration.*/
			memcpy(&si5375[3].portD_configuration[0][0],
				   &si5375_155_52_106_25[0][0], sizeof(si5375_155_52_106_25));
			break;
		default:
			err = 1;
			break;
	}
	if(err == 0)
	{
		/*disable all adjustments that were made before this point*/
		ipg_si5338_disable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		ipg_si5338_enable_freq_inc_dec(&si5338[0], _index);
		Delay_ms(2);
		/*Apply the new SI5375 configuration.*/
		ipg_si5375_config(&si5375[0], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[1], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[2], PORT_A + _index);
		Delay_ms(2);
		ipg_si5375_config(&si5375[3], PORT_A + _index);
		Delay_ms(2);
	}
}

/*/ <summary> */
/*/ Adjusts the ppm */
/*/ </summary> */
/*/ <returns></returns> */
/**
 * Function for ppm adjustment
 * @return none
 */
void adjust_ppm(void)
{
	int _index = 0;
	int silab_clock_channel = 0;

	for(_index = 0; _index < CLIENTS_IF_COUNT; _index++)
	{
		silab_clock_channel = _index;
		/*if(CL_MMAP(_index)->misc->general_status_live->bits.PPM_INC)
		{
			ipg_si5338_change_speed(&si5338[0], silab_clock_channel, SPEED_UP);
		}
		if(CL_MMAP(_index)->misc->general_status_live->bits.PPM_DEC)
		{
			ipg_si5338_change_speed(&si5338[0], silab_clock_channel, SLOW_DOWN);
		}*/
		if(CL_MMAP(_index)->odtu_brick->odtu_sts_reg_1->bits.PPM_INCR )
		{
			ipg_si5338_change_speed(&si5338[0], silab_clock_channel, SPEED_UP);
		}
		if(CL_MMAP(_index)->odtu_brick->odtu_sts_reg_1->bits.PPM_DECR)
		{
			ipg_si5338_change_speed(&si5338[0], silab_clock_channel, SLOW_DOWN);
		}

		if(CL_MMAP(_index)->odtu_brick->odtu_sts_reg_2->bits.PPM_INCR)
		{
			ipg_si5338_change_speed(&si5338[3], silab_clock_channel, SPEED_UP);
		}
		if(CL_MMAP(_index)->odtu_brick->odtu_sts_reg_2->bits.PPM_DECR)
		{
			ipg_si5338_change_speed(&si5338[3], silab_clock_channel, SLOW_DOWN);
		}
	}
}
/**
 * Function that configure GE MACs when we have a problem
 * param[in] _port_ptr pointer to port object
 * @return none
 */
void reconfigure_ge_port(port_t* _port_ptr)
{
	int index = _port_ptr->index;
	Delay_ms(500);
	if PORT_FPGA1
	{
		/*#warning ask teddy if you have to add this bit
		CL_MMAP(index)->odtu_brick.odtu_ctrl_reg_1->bits.GE_BRICK_RST = 1;
		CL_MMAP(index)->odtu_brick.odtu_ctrl_reg_1->bits.GE_BRICK_RST = 0;
		*/
		init_ge_pcs_ol(CL_MMAP(index)->gepcs_1);
		init_ge_mac_ol(CL_MMAP(index)->gemac_1);
		CL_MMAP(index)->gemac_1->rx_section_full_reg->all = 0x5DC;
		CL_MMAP(index)->gemac_1->command_config_reg->all = 0x0100015B;
		/*clear bit 12 (DISABLE OMIT CRC)*/
		CL_MMAP(index)->gemac_1->transmit_command_reg->all = 0;
		CL_MMAP(index)->ge->reset_ctrl_reg_1->bits.GFP_RST = 1;
		Delay_ms(10);
		CL_MMAP(index)->ge->reset_ctrl_reg_1->bits.GFP_RST = 0;
	}
	else
	{
		if PORT_FPGA2
		{
			/*#warning ask teddy if you have to add this bit
			CL_MMAP(index)->odtu_brick.odtu_ctrl_reg_2->bits.GE_BRICK_RST = 1;
			CL_MMAP(index)->odtu_brick.odtu_ctrl_reg_2->bits.GE_BRICK_RST = 0;
			*/
			init_ge_pcs_ol(CL_MMAP(index)->gepcs_2);
			init_ge_mac_ol(CL_MMAP(index)->gemac_2);
			CL_MMAP(index)->gemac_2->rx_section_full_reg->all = 0x5DC;
			CL_MMAP(index)->gemac_2->command_config_reg->all = 0x0100015B;
			/*clear bit 12 (DISABLE OMIT CRC)*/
			CL_MMAP(index)->gemac_2->transmit_command_reg->all = 0;
			CL_MMAP(index)->ge->reset_ctrl_reg_2->bits.GFP_RST = 1;
			Delay_ms(10);
			CL_MMAP(index)->ge->reset_ctrl_reg_2->bits.GFP_RST = 0;
		}
	}
}

char* ALS_STATE_TEXT[2] = { "PASSIVE", "ACTIVE" };
char* ALS_CONTROL_TEXT[4] = { "DISABLED", "LINE", "CLIENT", "REMOTE", };

char* PORT_OPERATION_TEXT[2] = { "CLIENT", "LINE", };

char* SERVICE_STATE_TEXT[3] = { "OOS", "MT", "IS", };
/*#warning check if we delete this fct*/
void dump_statistics(void)
{
	uint32_t i = 0;
	printf("Card temperature 	%f\n", card.temperature.value);
	printf("Up Time (SEC) 		%u\n", (unsigned int) card.sys_uptime);
	for(i = 0; i < INTERFACE_COUNT; i++)
	{
		printf("INTERFACE %u (%s):\n", (unsigned int) i,
				PORT_OPERATION_TEXT[PORT(i).info.operation - 1]);
		/*print interface data*/
		printf("\tService State	- 	%s\n",
				SERVICE_STATE_TEXT[(INTERFACE(i).service_state.current)]);
		printf("\tDestination	-	%s\n", INTERFACE(i).destination);
		/*print port data*/
		if(PORT(i).info.capabilities.ALS_CAPABLE)
		{
			printf("\tALS	State	-	%s\n",
					ALS_STATE_TEXT[als_get_state(PORT(i).als)]);
			printf("\tALS	Control	-	%s\n",
					ALS_CONTROL_TEXT[PORT(i).als->control]);
		}
		if(PORT(i).info.capabilities.PFC_CAPABLE)
		{
			printf("\tES	-	%u\n", (unsigned int) get_pfc_es(PORT(i).pfc));
			printf("\tSES	-	%u\n", (unsigned int) get_pfc_ses(PORT(i).pfc));
			printf("\tUAS	-	%u\n", (unsigned int) get_pfc_uas(PORT(i).pfc));
			printf("\tBBE	-	%u\n", (unsigned int) get_pfc_bbe(PORT(i).pfc));
		}
		if(PORT(i).info.capabilities.FEC_CAPABLE)
		{
			printf("\tCorr		-	%u\n",
					(unsigned int) PORT_MONITOR(i).fec_statistics.corrected_bits.accumulated);
			printf("\tUncorr	-	%u\n",
					(unsigned int) PORT_MONITOR(i).fec_statistics.uncorrected_cw.accumulated);
		}
		if(PORT_PROTOCOL_SDH == PORT_MONITOR(i).prot)
		{
			printf("\tAlarms	-	%x\n",(unsigned int) C_STMXSTS(i).all);
			printf("\t\tLOS		-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.LOS));
			printf("\t\tLOF		-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.LOF));
			printf("\t\tMS-AIS	-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.MS_AIS));
			printf("\t\tMS-BIP	-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.MS_BIP));
			printf("\t\tMS-RDI	-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.MS_RDI));
			printf("\t\tMS-REI	-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.MS_REI));
			printf("\t\tRS-BIP	-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.RS_BIP));
			printf("\t\tRS-TIM	-	%d\n",
					(PORT_MONITOR(i).statistics.stmx_sts.alarms.bits.RS_TIM));
		}
		if(PORT_PROTOCOL_ETHERNET == PORT_MONITOR(i).prot)
		{
			printf("\tAlarms	-	%x\n",
					(unsigned int) PORT_MONITOR(i).statistics.eth_sts.alarms.all);
			printf("\t\tLOS		-	%d\n",
					(PORT_MONITOR(i).statistics.eth_sts.alarms.bits.LOS));
			printf("\t\tLINK D	-	%d\n",
					(PORT_MONITOR(i).statistics.eth_sts.alarms.bits.LINK_DOWN));
			printf("\t\tFCS		-	%d\n",
					(PORT_MONITOR(i).statistics.eth_sts.alarms.bits.FCS));
			printf("\t\tTX OK	-	%u\n",
					(PORT_MONITOR(i).statistics.eth_sts.tx_ok.current));
			printf("\t\tRX OK	-	%u\n",
					(PORT_MONITOR(i).statistics.eth_sts.rx_ok.current));
			printf("\t\tRX FCS	-	%u\n",
					(PORT_MONITOR(i).statistics.eth_sts.rx_crc.current));
		}
		if(PORT_PROTOCOL_OTU == PORT_MONITOR(i).prot)
		{
			printf("\tAlarms	-	%x\n",
					(unsigned int) (PORT_MONITOR(i).statistics.otux_sts.alarms.all));
			printf("\t\tLOS		-	%d\n",
					PORT_MONITOR(i).statistics.otux_sts.alarms.bits.LOS);
			printf("\t\tLOF		-	%d\n",
					PORT_MONITOR(i).statistics.otux_sts.alarms.bits.LOF);
			printf("\t\tLOM		-	%d\n",
					PORT_MONITOR(i).statistics.otux_sts.alarms.bits.LOM);
		}
		if(TRANS(i).info.capabilities.TEMPERATURE_MONITOR)
		{
			printf("\tSFP temperature 	- %f\n",
					TRANS(i).monitor.temperature.value);
			printf("\tSFP IN 			- %f\n", TRANS(i).monitor.input_power->value);
			printf("\tSFP OUT 			- %f\n", TRANS(i).monitor.output_power->value);
		}
	}
}
/**
* Function that implements additional behavior and monitoring for this project
* that has to run when uart roe occur.
* @param[in] context
* Return none
*/
void callback_uart_roe(void* context)
{
	if(context != NULL)
	{
		uart_registers_t* _uart = (uart_registers_t*) context;
		if(_uart == &arm_uart)
		{
			ipg_uart_clear_ORE(_uart);
			counter_ores_arm++;
		}
		if(_uart == &dvt_uart)
		{
			ipglb_prot2_cleanup(dvt_processor);
			ipg_uart_clear_ORE(_uart);
			counter_ores_dvt++;
		}
	}
}
/**
* Function that implements additional behavior and monitoring for this project
* that has to run when uart trdy occur.
* @param[in] context
* Return none
*/
void callback_uart_trdy(void* context)
{
	if(context != NULL)
	{
		uart_registers_t* _uart = (uart_registers_t*) context;
		if(_uart == &arm_uart)
		{
			alt_u16 data = 0;
			alt_irq_context cntxt = alt_irq_disable_all();
			if(ring_buffer_get(rb_data_to_kurs_uart, &data) != RET_SUCCESS)
			{
				/*no more data in queue, turn off TRDY interrupts*/
				_uart->uart_cntrl_reg->bits.ITRDY = 0;
			}
			else
			{
				data &= 0x1FF;
				_uart->uart_tx_reg->all = data;
			}
			alt_irq_enable_all(cntxt);
		}
	}
}
/**
* Function that implements additional behavior and monitoring for this project
* that has to run when uart rrdy occur.
* @param[in] context
* @param[in] data
* Return none
*/
void callback_uart_rrdy(void* context, alt_u16 data)
{
	if(context != NULL)
	{
		uart_registers_t* _uart = (uart_registers_t*) context;
		if(_uart == &arm_uart)
		{
			if(BIT_IS_SET(data, BIT8))
			{
				ipglb_prot2_receive(arm_processor, (alt_u8) data);
			}
			else
			{
				kurs_receive(kurs_req_processor, (alt_u8) (data));
			}
		}
		if(_uart == &dvt_uart)
		{
			ipglb_prot2_receive(dvt_processor, (alt_u8) data);
		}
	}
}

/**
 * Function to send kurs packet. Project dependent implementation.
 * @param this pointer to kurs processor object
 * @param _tx data to send
 * @param _len data lenght
 * Return none
 */
void kurs_send_packet(KURS_processor_t *this, alt_u8* _tx, alt_u32 _len)
{
#if 0
	/*the queue used for ARM communications stores 16-bit elements
	 * we need to make the 8-bit array into a 16-bit array*/
	uint16_t _tx16[_len];
	uint32_t i = 0;
	for(i = 0; i < _len; i++)
	{
		_tx16[i] = _tx[i];
	}
	queue_many_to_arm(_tx16, _len);
#else
	uint32_t count = 0;
	while (count < _len)
	{
		queue_data_to_arm((alt_u16) _tx[count++]);
	}
#endif
}
/**
 * Function to send ipglb protocol 2 packet.
 * @param this pointer to ipglb protocol2 processor object
 * @param _tx data to send
 * @param _len data lenght
 * Return none
 */
void ipglb_prot2_send_packet(IPGLB_PROT2_processor_t *this, alt_u8* _tx,
							 alt_u32 _len)
{
	uart_registers_t* _uart =
	(uart_registers_t*) ipglb_prot2_processor_get_context(this);
	if(_uart == &dvt_uart)
	{
		ipg_uart_transmit(_uart, _tx, _len);
	}
#if 0
	if(_uart == &arm_uart)
	{
		/* this is a 9-bit UART interface, transform the data into 16-bit
		 * and we should also set b8 so the ARM knows whether it is addressed
		 * to it or for the KURS CU*/
		uint16_t _tx16[_len];
		uint32_t i = 0;
		for(i = 0; i < _len; i++)
		{
			_tx16[i] = _tx[i] | BIT8;
		}
		/*the buffer will be placed in the ARM queue*/
		queue_many_to_arm(_tx16, _len);
	}
#endif
}

#define GET_SLOT_ID_RQST_LEN		        4
#define GET_SLOT_ID_REPLY_LEN	        	5

#define GET_FP_LEDS_RQST_LEN		        4
#define GET_FP_LEDS_REPLY_LEN	        	6

#define SET_STATUS_LED_RQST_LEN		        6
#define SET_STATUS_LED_REPLY_LEN	        4

alt_u8 request[256]; /*!< array that will point to the request */
alt_u8* reply; /*!< array that will point to the reply */

/*
 * Function to get the slot id
 * param[in] _slot_id the slot id value
 * Return RET_SUCCESS in case of success
 * */
ret_code_t get_slot_id(alt_u8* _slot_id)
{
#define REQ_LEN			GET_SLOT_ID_RQST_LEN
#define REP_LEN			GET_SLOT_ID_REPLY_LEN
#define FUNC			FUNC_GET_SLOT_ID

	ipglb_prot2_cleanup(arm_processor);
	alt_u32 start_systicks = 0;
	memset((void*) request, 0, REQ_LEN);

	/* build the request header data */
	request[UART_PACKET_LENGTH_INDEX] = REQ_LEN;
	request[UART_PACKET_FUNCTION_INDEX] = FUNC;
	request[UART_PACKET_COMMAND_INDEX] = CMD_READ;
	/* calculate the CRC of this request */
	request[(REQ_LEN - 1)] = uart_calculate_crc(request);
	/* send the request */
#if 0
	ipglb_prot2_send_packet(arm_processor, request, REQ_LEN);
#else
	send_9bit_uart_packet(&arm_uart, request, REQ_LEN, 1);
#endif
	/* take a snapshot of when the request has been set */
	start_systicks = systicks;

	/* now wait for the reply */
	while (!ipglb_prot2_get_reply_ready(arm_processor))
	{
		/* did we timeout? */
		if((systicks - start_systicks) > 100)
		{
			ipglb_prot2_cleanup(arm_processor);
			return (RET_TIMEOUT);
		}
	}
	/* reply is ready, get it */
	reply = (alt_u8*) ipglb_prot2_get_packet(arm_processor);
	if(reply[UART_PACKET_LENGTH_INDEX] != REP_LEN)
	{
		/* incorrect length field */
		ipglb_prot2_cleanup(arm_processor);
		return (RET_ARG_MISMATCH);
	}
	if(!uart_crc_ok((alt_u8*) reply))
	{
		/* handle bad CRC */
		ipglb_prot2_cleanup(arm_processor);
		return (RET_CRC_MISMATCH);
	}
	/* make sure the reply is correct */
	if(reply[UART_PACKET_FUNCTION_INDEX] != FUNC)
	{
		ipglb_prot2_cleanup(arm_processor);
		return (RET_FUNC_MISMATCH);
	}
	if(request[UART_PACKET_COMMAND_INDEX]
			!= reply[UART_PACKET_COMMAND_INDEX])
	{
		/* only config command should remain the same */
		if(request[UART_PACKET_COMMAND_INDEX] == CMD_CONFIG)
		{
			ipglb_prot2_cleanup(arm_processor);
			return (RET_CMD_MISMATCH);
		}
	}

	/* Update the slot_id */
	*_slot_id = reply[UART_PACKET_PARAMS_INDEX];

	/* cleanup after we're done */
	ipglb_prot2_cleanup(arm_processor);
	return (RET_SUCCESS);
#undef REP_LEN
#undef REQ_LEN
#undef FUNC
}
/*
 * Function to get fp leds
 * Return RET_SUCCESS in case of success
 *
 * */
ret_code_t get_fp_leds(void)
{
#define REQ_LEN			GET_FP_LEDS_RQST_LEN
#define REP_LEN			GET_FP_LEDS_REPLY_LEN
#define FUNC			FUNC_FP_LEDS
	ipglb_prot2_cleanup(arm_processor);
	alt_u32 start_systicks = 0;
	memset((void*) request, 0, REQ_LEN);
	memset((void*) reply, 0, REP_LEN);

	/* build the request header data */
	request[UART_PACKET_LENGTH_INDEX] = REQ_LEN;
	request[UART_PACKET_FUNCTION_INDEX] = FUNC;
	request[UART_PACKET_COMMAND_INDEX] = CMD_READ;
	/* calculate the CRC of this request */
	request[(REQ_LEN - 1)] = uart_calculate_crc(request);
	/* send the request */
#if 0
	ipglb_prot2_send_packet(arm_processor, request, REQ_LEN);
#else
	send_9bit_uart_packet(&arm_uart, request, REQ_LEN, 1);
#endif

	/* take a snapshot of when the request has been set */
	start_systicks = systicks;

	/* now wait for the reply */
	while (!ipglb_prot2_get_reply_ready(arm_processor))
	{
		/* did we timeout? */
		if( (systicks - start_systicks) > 100 )
		{
			ipglb_prot2_cleanup(arm_processor);
			return (RET_TIMEOUT);
		}
	}
	/* reply is ready, get it */
	reply = (alt_u8*) ipglb_prot2_get_packet(arm_processor);
	if(reply[UART_PACKET_LENGTH_INDEX] != REP_LEN)
	{
		/* incorrect length field */
		ipglb_prot2_cleanup(arm_processor);
		return (RET_ARG_MISMATCH);
	}
	if(!uart_crc_ok((alt_u8*) reply))
	{
		/* handle bad CRC */
		ipglb_prot2_cleanup(arm_processor);
		return (RET_CRC_MISMATCH);
	}
	/* make sure the reply is correct */
	if(reply[UART_PACKET_FUNCTION_INDEX] != FUNC)
	{
		ipglb_prot2_cleanup(arm_processor);
		return (RET_FUNC_MISMATCH);
	}
	if(request[UART_PACKET_COMMAND_INDEX] != reply[UART_PACKET_COMMAND_INDEX])
	{
		/* only config command should remain the same */
		if(request[UART_PACKET_COMMAND_INDEX] == CMD_CONFIG)
		{
			ipglb_prot2_cleanup(arm_processor);
			return (RET_CMD_MISMATCH);
		}
	}

	/* Update the power led status */
	board_leds.power_led_g.current = reply[UART_PACKET_PARAMS_INDEX];
	board_leds.status_led.current = reply[UART_PACKET_PARAMS_INDEX + 1];

	/* cleanup after we're done */
	ipglb_prot2_cleanup(arm_processor);
	return (RET_SUCCESS);
#undef REP_LEN
#undef REQ_LEN
#undef FUNC
}
#if 1
/*
 * Function to set the status led
 * Return RET_SUCCESS in case of success
 * */
ret_code_t set_status_led()
{
#define REQ_LEN			SET_STATUS_LED_RQST_LEN
#define REP_LEN			SET_STATUS_LED_REPLY_LEN
#define FUNC			FUNC_FP_LEDS

	ipglb_prot2_cleanup(arm_processor);
	alt_u32 start_systicks = 0;
	memset((void*) request, 0, REQ_LEN);
	memset((void*) reply, 0, REP_LEN);

	/* build the request header data */
	request[UART_PACKET_LENGTH_INDEX] = REQ_LEN;
	request[UART_PACKET_FUNCTION_INDEX] = FUNC;
	request[UART_PACKET_COMMAND_INDEX] = CMD_WRITE;

	request[UART_PACKET_PARAMS_INDEX] = card.alarm2.current;
	request[UART_PACKET_PARAMS_INDEX + 1] = card.alarm1.current;

	/* calculate the CRC of this request */
	request[(REQ_LEN - 1)] = uart_calculate_crc(request);
	/* send the request */
#if 0
	ipglb_prot2_send_packet(arm_processor, request, REQ_LEN);
#else
	send_9bit_uart_packet(&arm_uart, request, REQ_LEN, 1);
#endif

	/* take a snapshot of when the request has been set */
	/* take a snapshot of when the request has been set */
	start_systicks = systicks;

	/* now wait for the reply */
	while ( !ipglb_prot2_get_reply_ready(arm_processor) )
	{
		/* did we timeout? */
		if( (systicks - start_systicks) > 100 )
		{
			ipglb_prot2_cleanup(arm_processor);
			return (RET_TIMEOUT);
		}
	}
	/* reply is ready, get it */
	reply = (alt_u8*) ipglb_prot2_get_packet(arm_processor);
	if(reply[UART_PACKET_LENGTH_INDEX] != REP_LEN)
	{
		/* incorrect length field */
		ipglb_prot2_cleanup(arm_processor);
		return (RET_ARG_MISMATCH);
	}
	if(!uart_crc_ok((alt_u8*) reply))
	{
		/* handle bad CRC */
		ipglb_prot2_cleanup(arm_processor);
		return (RET_CRC_MISMATCH);
	}
	/* make sure the reply is correct */
	if(reply[UART_PACKET_FUNCTION_INDEX] != FUNC)
	{
		ipglb_prot2_cleanup(arm_processor);
		return (RET_FUNC_MISMATCH);
	}
	if(request[UART_PACKET_COMMAND_INDEX]
			!= reply[UART_PACKET_COMMAND_INDEX])
	{
		/* only config command should remain the same */
		if(request[UART_PACKET_COMMAND_INDEX] == CMD_CONFIG)
		{
			ipglb_prot2_cleanup(arm_processor);
			return (RET_CMD_MISMATCH);
		}
	}
	/* cleanup after we're done */
	ipglb_prot2_cleanup(arm_processor);
	return (RET_SUCCESS);
#undef REP_LEN
#undef REQ_LEN
#undef FUNC
}
#endif

/*
 * Function to update kurs power
 * param[in] _if_ptr pointer to the interface object
 * Return none
 * */
void update_if_kurs_power(interface_t* _if_ptr)
{
#define HMIN(x) (TRANS_MONITOR(if_index).x[0].physical_min)
#define POWER(x) (TRANS_MONITOR(if_index).x[0].value)
#define RES		  (0.0000001f)
	int if_index = _if_ptr->index;
	if(_if_ptr->transceiver.monitor.input_power != NULL)
	{
		/*	trim_kurs_power(TRANS_MONITOR(if_index).input_power[0].value,
		 * 					pin[if_index].value); */
		/*	trim_kurs_power(TRANS_MONITOR(if_index).output_power[0].value,
		 * 					pout[if_index].value); */
		if(float_compare_lt(POWER(input_power), DEF_SFP_PIN_HMIN, RES))
		{
			pin[if_index].value = DEF_SFP_PIN_HMIN;
		}
		else
		{
			pin[if_index].value = POWER(input_power);
		}
	}
	if(_if_ptr->transceiver.monitor.output_power != NULL)
	{
		if(float_compare_lt(POWER(output_power), DEF_SFP_POUT_HMIN, RES))
		{
			pout[if_index].value = DEF_SFP_POUT_HMIN;
		}
		else
		{
			pout[if_index].value = POWER(output_power);
		}
	}
	if(TRANS_CTRL(if_index).threshold_mode == THRESHOLD_TRANSCEIVER)
	{
		if(!TRANS(if_index).monitor.absent.current)
		{
			PIN_CMAX(if_index) = TRANS_MONITOR_INPWR(if_index)[0].critical_max;
			POUT_CMAX(if_index)= TRANS_MONITOR_OUTPWR(if_index)[0].critical_max;
			PIN_CMIN(if_index) = TRANS_MONITOR_INPWR(if_index)[0].critical_min;
			POUT_CMIN(if_index)= TRANS_MONITOR_OUTPWR(if_index)[0].critical_min;
			PIN_WMAX(if_index) = TRANS_MONITOR_INPWR(if_index)[0].warning_max;
			POUT_WMAX(if_index)= TRANS_MONITOR_OUTPWR(if_index)[0].warning_max;
			PIN_WMIN(if_index) = TRANS_MONITOR_INPWR(if_index)[0].warning_min;
			POUT_WMIN(if_index)= TRANS_MONITOR_OUTPWR(if_index)[0].warning_min;
			PIN_PMAX(if_index) = TRANS_MONITOR_INPWR(if_index)[0].physical_max;
			POUT_PMAX(if_index)= TRANS_MONITOR_OUTPWR(if_index)[0].physical_max;
			PIN_PMIN(if_index) = TRANS_MONITOR_INPWR(if_index)[0].physical_min;
			POUT_PMIN(if_index)= TRANS_MONITOR_OUTPWR(if_index)[0].physical_min;
		}
	}
#undef HMIN
#undef POWER
#undef RES
}

/**
 * Function that sends a buffer of characters through a 9-bit UART interface.
 * @param[in,out] 	uart	pointer to the UART interface registers.
 * @param[in] 		reply	pointer to the characters to be transmitter
 * @param[in]		_len	length of the reply to be sent
 * @param[in]       tag
 * Return none
 */
void send_9bit_uart_packet( uart_registers_t* uart, alt_u8* packet,
							alt_u32 _len, alt_u8 tag )
{
	alt_u32 count = 0;
	alt_u16 data = 0;
	while (count < _len)
	{
#if 1
		data = (alt_u16) (packet[count] | 0x100); /*Set bit8*/
		queue_data_to_arm(data);
		count++;
#else
		if(!uart->uart_sts_reg->bits.TRDY)
		{
			continue;
		}
		data = (alt_u16) (packet[count]);
		if(tag)
		{
			/*Tag the 9-bit by 1 */
			data |= 0x100;
		}
		uart->uart_tx_reg->all = (data & 0x1FF);
		count++;
#endif
	}
}

/*
 * Function for scheduler that run every 2000 ms
 * @param[in] me handle to s_task
 * @param[in] msg s_task message
 * Return none
 * */
void task_2000(s_task_handle_t me, s_task_msg_t** msg)
{
	/* two seconds periodic tasks */
#ifdef KURS_PROTOCOL
	KURS_WriteLngFileToFlash(&profile);
#endif
	get_board_temperature();
	if(save)
	{
		commit_user_settings();
		save = 0;
	}
	s_task_flush_msgs(msg);
}
/*
 * Function for scheduler that run every 55 ms
 * @param[in] me handle to s_task
 * @param[in] msg s_task message
 * Return none
 * */
void task_55(s_task_handle_t me, s_task_msg_t** msg)
{
	update_if_dynamic_data( &card.interfaces[selected_if] );
#ifdef KURS_PROTOCOL
	/*Update power variables used in the profile */
	update_if_kurs_power( &card.interfaces[selected_if] );
#if ENABLE_ALS_CODE
	//update_kurs_als_state(&card.interfaces[selected_if]);
#endif
#endif
	/*To recover from LOF, we have to switch teh GXB to force LTR briefly and
	 * then remove it*/
	/*#warning check this test*/
	if (PORT_MONITOR(selected_if).rate == PORT_RATE_OTU1)
	{
		if (PORT_MONITOR(selected_if).statistics.otux_sts.alarms.bits.LOF &&
			!PORT_MONITOR(selected_if).statistics.otux_sts.alarms.bits.LOS)
		{
			if (PORT(selected_if).info.operation == PORT_OPERATION_CLIENT)
				{
					CL_MMAP(selected_if)->odtu_brick->odtu_ctrl_reg_1->bits.FORCE_LTR = 1;
					Delay_ms(1);
					CL_MMAP(selected_if)->odtu_brick->odtu_ctrl_reg_1->bits.FORCE_LTR = 0;
				}
		}
	}
	if(PORT_MONITOR(selected_if).rate == PORT_RATE_OTU2)
	{
		if(PORT_MONITOR(selected_if).statistics.otux_sts.alarms.bits.LOF
			&& !PORT_MONITOR(selected_if).statistics.otux_sts.alarms.bits.LOS)
		{
			if(PORT(selected_if).info.operation == PORT_OPERATION_LINE)
			{
				//uint32_t ln_index = selected_if - LN_IF_OFFSET;
				LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 1;
				Delay_ms(1);
				LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 0;

				LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 1;
				Delay_ms(1);
				LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LOCK_TO_REF = 0;
			}
		}
	}
	selected_if++;
	if(selected_if == INTERFACE_COUNT)
	{
		selected_if = 0;
		if(slot_id != -1)/*the slot_id was not updated from the ARM yet */
		{
			update_alarms();
			update_card_alarm1();
			update_card_alarm2();
			/*Update status LED according to Alarm 1 and Alarm 2 states */
			get_fp_leds();
			card.block_state.current =
			(board_leds.power_led_g.current == COLOR_GREEN) ?
			(BLOCK_STATE_OK) : (BLOCK_STATE_FAIL);
			if(card.alarm1.change || card.alarm2.change)
			{
				set_status_led();
			}
		}
	}
}
/*
 * Function for scheduler that run every 100 ms
 * @param[in] me handle to s_task
 * @param[in] msg s_task message
 * Return none
 * */
void task_100(s_task_handle_t me, s_task_msg_t** msg)
{
	ColorDynamicParameters();
#if APS_EN
	update_aps_data(&_aps);
#endif
	int if_index = 0;
	for(if_index = 0; if_index < CLIENTS_IF_COUNT; if_index++)
	{
		if(port_rate_change_flag[if_index])
		{
			port_rate_change_flag[if_index] = 0;
			if FPGA1
			{
//				CL_MMAP(i)->gmp->general_cfg_reg->bits.SW_RESET = 1;
//				Delay_ms(1);
//				CL_MMAP(i)->gmp->general_cfg_reg->bits.SW_RESET = 0;
				Delay_ms(1);
				CL_MMAP(if_index)->stm_14_1->reset->bits.SOFT_RESET = 1;
				Delay_ms(1);
				CL_MMAP(if_index)->stm_14_1->reset->bits.SOFT_RESET = 0;
			}
			else
			{
				if FPGA2
				{
				//CL_MMAP(i)->gmp->general_cfg_reg->bits.SW_RESET = 1;
				//Delay_ms(1);
				//CL_MMAP(i)->gmp->general_cfg_reg->bits.SW_RESET = 0;
					Delay_ms(1);
					CL_MMAP(if_index)->stm_14_2->reset->bits.SOFT_RESET = 1;
					Delay_ms(1);
					CL_MMAP(if_index)->stm_14_2->reset->bits.SOFT_RESET = 0;
				}
			}
		}
	}
}
/*
 * Function for scheduler that run every 10 ms
 * @param[in] me handle to s_task
 * @param[in] msg s_task message
 * Return none
 * */
void task_10(s_task_handle_t me, s_task_msg_t** msg)
{
	task_ppm_adjustment();
	/* check for changes in user controllable settings */
	for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
	{
		if(PORT_CTRL(if_index).reset_counters.change)
		{
			if(PORT(if_index).info.operation == PORT_OPERATION_CLIENT)
			{
				if FPGA1
				{
					clear_ge_counters_ol(CL_MMAP(if_index)->gemac_1);
				}
				else
				{
					if FPGA2
					{
						clear_ge_counters_ol(CL_MMAP(if_index)->gemac_2);
					}

				}
			}
			reset_port_counters(&PORT(if_index));
			PORT_CTRL(if_index).reset_counters.change = 0;
		}
		if(INTERFACE(if_index).service_state.change)
		{
			switch(INTERFACE(if_index).service_state.next)
			{
				/*We do not turn ON TX enable automatically for safety reasons*/
				/*We want the user to manually turn ON SFP TX */
				case IF_SERVICE_STATE_OOS:
					/*
					 * In this state, parameters should not be colored i.e.
					 *  monitor is OFF and TX cannot be turned ON or OFF
					 * EMS will not display parameters in this state.
					 * */
					INTERFACE(if_index).monitor = ACTIVITY_MONITOR_OFF;
					TRANS_CTRL(if_index).tx_disable.next = 1;
					TRANS_CTRL(if_index).tx_disable.change = 1;
					break;
				case IF_SERVICE_STATE_IS:
					/*
					 * In this state, parameters should be colored i.e. monitor
					 * is ON and TX can be turned ON or OFF
					 * */
					INTERFACE(if_index).monitor = ACTIVITY_MONITOR_ON;
					break;
				case IF_SERVICE_STATE_MT:
					/*
					 * In this state, parameters should not be colored i.e.
					 * monitor is OFF and TX can be turned ON or OFF.
					 * EMS will display parameters in this state.
					 * */
					INTERFACE(if_index).monitor = ACTIVITY_MONITOR_OFF;
					break;
				default:
					break;
			}
			INTERFACE(if_index).service_state.current =
			INTERFACE(if_index).service_state.next;
			INTERFACE(if_index).service_state.change = 0;
		}
		if(PORT_CTRL_LB(if_index).change)
		{
			if(set_if_port_loopback(&PORT(if_index),
									(loopback_mode_t)PORT_CTRL_LB(if_index).next)
									== RET_SUCCESS)
			{
				PORT_CTRL_LB(if_index).current = PORT_CTRL_LB(if_index).next;
				PORT_CTRL_LB(if_index).change = 0;
			}
		}
		if(PORT_CTRL(if_index).rate.change)
		{
			if(!PORT_CAP(if_index).MULTI_RATE)
			{
				cancel_request(&PORT_CTRL(if_index).rate);
			}
			else
			{
				if(set_if_port_rate(&PORT(if_index)) == RET_SUCCESS)
				{
					if(read_if_port_rate(&PORT(if_index)) == RET_SUCCESS)
					{
						if(PORT_MONITOR(if_index).rate ==
							PORT_CTRL(if_index).rate.next)
						{
							PORT_CTRL(if_index).rate.change = 0;
							/* rate modified successfully*/
							PORT_CTRL(if_index).rate.change = 0;
							/*keep the change flag set*/
							PORT_CTRL(if_index).rate.current =
							PORT_CTRL(if_index).rate.next;
							/* When Client ALS mode is in remote, hardware
							 * should report local LOS to the far end. Current
							 * HW uses Line GCC2 messages for that purpose and
							 * requires, when running in this mode
							 * (CL=OTU1 and ALS=remote) to LOAD GCC2
							 * (on both Line1 and Line2 of each brick only check
							 * for clients that can be configured as OTU1
							 * if we do not do this check, then clients 2 and
							 * 4 will override this functionality*/
							/*#warning check if i need this test */
//							if((if_index == CL_0_IF_INDEX) ||
//							   (if_index == CL_2_IF_INDEX))
//							{
								if(IS_CLIENT_OTU1(if_index) &&
								   ISALS_REMOTECTRL(if_index))
								{
									/*we should send the brick index*/
									report_local_los_to_fe(if_index / 2, 1);
								}
								else
								{
									report_local_los_to_fe(if_index / 2, 0);
								}
							//}
							/**/
							switch(PORT_MONITOR(if_index).rate)
							{
								case PORT_RATE_STM1:
								case PORT_RATE_STM4:
								case PORT_RATE_STM16:
									PORT_CAP(if_index).PFC_CAPABLE = 1;
									PORT_CAP(if_index).FEC_CAPABLE = 0;
									break;
								case PORT_RATE_OTU1:
									PORT_CAP(if_index).PFC_CAPABLE = 1;
									PORT_CAP(if_index).FEC_CAPABLE = 1;
									break;
								case PORT_RATE_GE:
									if FPGA1
									{
										clear_ge_counters_ol(CL_MMAP(if_index)->gemac_1);
									}
									else
									{
										if FPGA2
										{
											clear_ge_counters_ol(CL_MMAP(if_index)->gemac_2);
										}
									}
									PORT_CAP(if_index).PFC_CAPABLE = 0;
									PORT_CAP(if_index).FEC_CAPABLE = 0;
									break;
								case PORT_RATE_FE:
									PORT_CAP(if_index).PFC_CAPABLE = 0;
									PORT_CAP(if_index).FEC_CAPABLE = 0;
									/*TODO Nicolas clear FE counters*/
									break;
								default:
									PORT_CAP(if_index).PFC_CAPABLE = 0;
									PORT_CAP(if_index).FEC_CAPABLE = 0;
									break;
							}
						}
						if(PORT_CAP(if_index).PFC_CAPABLE)
						{
							create_pfc_module(&PORT(if_index));
						}
						else
						{
							destroy_pfc_module(&PORT(if_index));
						}
					}
					PORT_CTRL(if_index).reset_counters.change = 1;
				}
				PORT_CTRL(if_index).rate.change = 0;
			}
		}
		if(TRANS_CTRL(if_index).tx_disable.change)
		{
			set_transceiver_txdisable(&TRANS(if_index));
			TRANS_CTRLTXDIS(if_index).current =
			get_if_transceiver_txdisable(&TRANS(if_index));
			if(TRANS_CTRLTXDIS(if_index).current ==
			   TRANS_CTRLTXDIS(if_index).next)
			{
				/*change successful*/
				TRANS_CTRLTXDIS(if_index).change = 0;
			}
		}
		if(TRANS_CTRL(if_index).reboot.change)
		{
			if(boot_transceiver(&TRANS(if_index)) == RET_SUCCESS)
			{
				TRANS_CTRL(if_index).reboot.change = 0;
			}
		}
	}
#if APS_EN
	if( _aps.aps_mode.change)
	{
		if(aps_set_mode(&_aps) == RET_SUCCESS)
		{
			_aps.aps_mode.change = 0;
		}
	}
	if(_aps.aps_protectch.change)
	{
		aps_task(&_aps);
		_aps.aps_protectch.change = 0;
	}
	if(_aps.revertive_mode.change)
	{
		if(aps_set_revert_mode(&_aps) == RET_SUCCESS)
		{
			_aps.revertive_mode.change = 0;
		}
	}
#endif
}
