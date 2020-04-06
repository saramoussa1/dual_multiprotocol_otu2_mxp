/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipglb_protocol_2.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Contains prototypes of functions used in ipglb_protocol.c as well as type and enum definitions.
 *
 */
#ifndef __IPGLB_PROTOCOL_H__
#define __IPGLB_PROTOCOL_H__

#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "ipg_types.h"

/**List of supported I2C devices IDs.
 *
 */
enum I2C_DEVICE_ID{
	I2C_GENERIC_DEVICE = 0,     //!< I2C_GENERIC_DEVICE			ID of a generic I2C device.
	I2C_SILAB_DEVICE,           //!< I2C_SILAB_DEVICE			ID of a SILAB.
	I2C_BOARD_TS_DEVICE,        //!< I2C_BOARD_TS_DEVICE		ID of a temperature sensor.
	I2C_SFP_DEVICE,             //!< I2C_SFP_DEVICE				ID of an SFP.
	I2C_SFPP_DEVICE,            //!< I2C_SFPP_DEVICE			ID of an SFP+.
	I2C_XFP_DEVICE,             //!< I2C_XFP_DEVICE				ID of an XFP.
	I2C_PORT_EXPANDER,          //!< I2C_PORT_EXPANDER			ID of an I2C port expander.
	I2C_BOARD_EEPROM_DEVICE,    //!< I2C_BOARD_EEPROM_DEVICE	ID of an I2C EEPROM.
	I2C_OPTICAL_MODULE_DEVICE,  //!< I2C_OPTICAL_MODULE_DEVICE	ID of a 100G optical module.
	I2C_DDR3_TS_DEVICE,         //!< I2C_DDR3_TS_DEVICE			ID of a DD3 temperature sensor.
	I2C_DDR3_EEPROM_DEVICE      //!< I2C_DDR3_EEPROM_DEVICE		ID of a DD3 EEPROM.
};

/**List of supported MDIO devices IDs.
 *
 */
enum MDIO_DEVICE_ID{
	MDIO_DEVICE_ID_GENERIC_22 = 0,//!< MDIO_GENERIC_DEVICE	ID of a generic MDIO device.
	MDIO_DEVICE_ID_PHY,        //!< PHY_DEVICE				ID of a PHY.
	MDIO_DEVICE_ID_OM,         //!< OM_DEVICE				ID of an optical module.
	MDIO_DEVICE_ID_GENERIC_45 = 0x80//!< MDIO_GENERIC_DEVICE	ID of a generic MDIO device.
};

/**Enumeration of the indices of the different fields in an IPGLB II  packet.
 *
 */
enum UART_DATA_LOCATIONS{
	UART_PACKET_LENGTH_INDEX = 0,//!<UART_PACKET_LENGTH_INDEX	index of the \c LENGTH field inside the packet.
	UART_PACKET_FUNCTION_INDEX,  //!<UART_PACKET_FUNCTION_INDEX index of the \c FUNCTION field inside the packet.
	UART_PACKET_COMMAND_INDEX,   //!<UART_PACKET_COMMAND_INDEX index of the \c COMMAND field inside the packet.
	UART_PACKET_PARAMS_INDEX     //!<UART_PACKET_PARAMS_INDEX	index of the first parameter of \c PARAMS inside the packet.
};

/**Enumeration of the \c FUNCTION field values.
 *
 */
enum FUNCTIONS{
	/* function definitions common to all boards*/
	FUNC_MMAP = 0x00,  //!<FUNC_MMAP 	value of memory map function.
	FUNC_I2C,          //!<FUNC_I2C 	value of I2C function.
	FUNC_SPI,          //!<FUNC_SPI	value of the SPI function.
	FUNC_MDIO,         //!<FUNC_MDIO	value of the MDIO function.
	FUNC_UART,         //!<FUNC_UART	value of the UART function.
	FUNC_ETH,          //!<FUNC_ETH	value of the ETHERNET function.
	FUNC_FW_REV = 0x0E,//!<FUNC_FW_REV	value of the FW REV function.
	FUNC_PING = 0x0F,  //!<FUNC_PING	value of the PING function.
	/*space for board specific custom commands*/
#ifdef KURS_PROTOCOL
	FUNC_GET_SLOT_ID = 0x50,
	FUNC_FP_LEDS = 0x51,
#endif
};

/**Enumeration of the \c COMMAND field values.
 *
 */
enum COMMANDS{
	CMD_READ = 0,//!<CMD_READ		value of the READ command.
	CMD_WRITE,   //!<CMD_WRITE		value of the WRITE command.
	CMD_CONFIG   //!<CMD_CONFIG		value of the CONFIG command.
};

/**Enumeration of the \c OPCODE values.
 *
 */
enum OPCODE{
	OPCODE_ACK = 0,   //!<OPCODE_ACK	value for ACK.
	OPCODE_NACK = 0x80//!<OPCODE_NACK	value for NACK.
};

typedef ret_code_t (*func_handler)(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);//!<Callback function type for IPGLB II communication handlers*/
void assign_function_handlers(void);
void custom_assign_function_handlers(void);
ret_code_t handle_common(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr);
func_handler handlers[256];	//!<List of possible handlers
alt_u8 uart_calculate_crc( alt_u8 *buff );
alt_u8 uart_crc_ok( alt_u8 *buff );

/*handlers prototypes*/
ret_code_t handle_mmap_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_ping_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_fw_rev_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_i2c_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_mdio_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_mem_rd(alt_u32 base, alt_u32 offset, alt_u32* value);
ret_code_t handle_mem_wr(alt_u32 base, alt_u32 offset, alt_u32 value) ;
ret_code_t handle_i2c_wr(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply, int* reply_ptr);
ret_code_t handle_i2c_rd(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply, int* reply_ptr, alt_u8 *i2c_value);
ret_code_t handle_user_i2c_config(alt_u8 i2c_device_id, alt_u8* request, alt_u8* reply, int* reply_ptr);
ret_code_t handle_mdio_wr(alt_u8 mdio_device_id, alt_u8 mdio_device_index, alt_u8 phy_address, alt_u8 device_address, alt_u16 offset, alt_u16 value);
ret_code_t handle_mdio_rd(alt_u8 mdio_device_id, alt_u8 mdio_device_index, alt_u8 phy_address, alt_u8 device_address, alt_u16 offset, alt_u16 *value);
#ifndef KURS_PROTOCOL
ret_code_t handle_brd_vitals_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_user_led_func(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr);
/*port handlers prototypes*/
ret_code_t handle_if_port_dynamic_data_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_port_fectype_func(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr);
ret_code_t handle_if_port_lb_func(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr);
ret_code_t handle_if_port_cntrs(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_port_rate_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_port_pfc(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_port_info(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
#if ENABLE_ETH_CODE
ret_code_t handle_if_port_eth_control(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
#endif
/*transceivers handlers prototypes*/
ret_code_t handle_if_transceiver_txdis_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_transceiver_freq_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_transceiver_reboot_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_transceiver_tx_power_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_transceiver_dynamic_data_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr);
ret_code_t handle_if_transceiver_opt_thresholds(alt_u8 COM, alt_u8* request, alt_u8* reply, int *reply_ptr);
ret_code_t handle_if_transceiver_info_func(alt_u8 COM, alt_u8 *request, alt_u8 *reply, int *reply_ptr) ;
#endif
#endif /*__IPGLB_PROTOCOL_H__*/
