/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_uart.c
 * @author d_chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 6, 2015
 * @brief Here goes a brief description.
 * @revision 0.0.1
 * Here goes a more detailed description.
 */
#include "../inc/ipg_uart.h"
#include "sys/alt_irq.h"

#ifndef __weak
#define __weak __attribute__((weak))
#endif
static void uart_isr(void* context);

/**This function runs the default initialization of a UART interface.
 * @param[in,out] uart pointer to the UART interface registers
 */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void init_uart(uart_registers_t* uart, alt_u8 _interrupt_controller, alt_u8 _irq){
	if(uart != NULL){
		void* context = 0;
		void* flags = 0;
		context = uart;
		alt_ic_isr_register(_interrupt_controller, _irq, uart_isr, context, flags);
	}
}
#else
void init_uart(uart_registers_t* uart, alt_u8 _irq){
	if(uart != NULL){
		void* context = 0;
		void* flags = 0;
		context = uart;
		alt_irq_register(irq, context, arm_isr);
	}
}
#endif

/**
 * ISR handler for a UART COMM.
 */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void uart_isr(void* context){
#else
void uart_isr(void* context, alt_u8 id){
#endif
	if(context != NULL){
		uart_registers_t* _uart = (uart_registers_t*)context;
		struct UART_STATUS_BITS 	status 	= _uart->uart_sts_reg->bits;
		struct UART_CONTROL_BITS 	control	= _uart->uart_cntrl_reg->bits;
		alt_u32 data = 0;
		if(control.IROE && status.ROE){
			callback_uart_roe(context);
		}
		if(control.IRRDY && status.RRDY){
			/*receiver mode*/
			data = _uart->uart_rx_reg->all;
			callback_uart_rrdy(context, data);

		}
		if(control.ITRDY && status.TRDY){
			callback_uart_trdy(context);
		}
	}
}

/**
 * Callback function when a character has been successfully
 * transmitted.
 * @param[in,out] 	context		pointer to the UART object.
 */
__weak void callback_uart_trdy(void* context){
	/*
	 * Do not modify this function, implement it in application.
	 */
}

/**
 * This function is called by the UART RX ISR whenever a
 * character is received successfully.
 * @param[in, out]	context	pointer to the UART object.
 * @param[in]		data	received data
 *
 */
__weak void callback_uart_rrdy(void* context, alt_u16 data){
	/*
	 * Do not modify this function, implement it in application.
	 */
}


#ifdef DEBUG
static uint32_t roe_cntr = 0; /*!<this is a debug counter and should be removed*/
#endif
__weak void callback_uart_roe(void* context){
	if(context != NULL){
		uart_registers_t* _uart = (uart_registers_t*)context;
		_uart->uart_sts_reg->bits.ROE = 0;
#ifdef DEBUG
	roe_cntr++;
#endif
	}
}

void ipg_uart_clear_ORE(void* context){
	if(context != NULL){
		uart_registers_t* _uart = (uart_registers_t*)context;
		_uart->uart_sts_reg->bits.ROE = 0;
	}
}
/**This function maps UART interface registers to our application variables.
 *
 * @param[in,out] uart pointer to the application UART variables
 * @param uart_base	base address of the UART interface in memory
 * @return None
 */
void map_uart_regs(uart_registers_t* uart, alt_u32 uart_base){
	uart->uart_rx_reg = (volatile union UART_RXDATA_REG*)((uart_base + 0)| BIT31);
	uart->uart_tx_reg = (volatile union UART_TXDATA_REG*)((uart_base + 0x4) | BIT31);
	uart->uart_sts_reg = (volatile union UART_STATUS_REG*)((uart_base + 0x8) | BIT31);
	uart->uart_cntrl_reg = (volatile union UART_CONTROL_REG*)((uart_base + 0xC) | BIT31);
}

/**This function enables interrupts when an ORE error occurs.
 * @param[in,out] uart	pointer to the UART interface registers
 */
void ipg_uart_enable_IROE(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.IROE = 1;
}

/**This function disables interrupts when an ORE error occurs.
 * @param[in,out] uart	pointer to the UART interface registers
 */
void ipg_uart_disable_IROE(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.IROE = 0;
}

/**This function enables interrupts upon receiving data on a UART interface.
 * @param[in,out] uart	pointer to the UART interface registers
 */
void ipg_uart_enable_IRRDY(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.IRRDY = 1;
}

/**This function disables interrupts upon receiving data on a UART interface.
 * @param[in,out] uart	pointer to the UART interface registers
 */
void ipg_uart_disable_IRRDY(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.IRRDY = 0;
}
/**This function enables interrupts upon transmitting data on a UART interface.
 * @param[in,out] uart	pointer to the UART interface registers
 */
void ipg_uart_enable_ITRDY(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.ITRDY = 1;
}

/**This function disables interrupts upon transmitting data on a UART interface.
 * @param[in,out] uart	pointer to the UART interface registers
 */
void ipg_uart_disable_ITRDY(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.ITRDY = 0;
}

/**This function enables interrupts upon receiving data on a UART interface.
 * @param[in,out] uart	pointer to the UART interface registers
 * @deprecated
 */
void ipg_uart_enable(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.IRRDY = 1;
}

/**This function disables interrupts upon receiving data on a UART interface.
 * @param[in,out] uart	pointer to the UART interface registers
 * @deprecated
 */
void ipg_uart_disable(uart_registers_t* uart){
	uart->uart_cntrl_reg->bits.IRRDY = 0;
}


/**This function sends a buffer of characters through a UART interface.
 * @param[in,out] 	uart	pointer to the UART interface registers.
 * @param[in] 		reply	pointer to the characters to be transmitter
 * @param[in]		_len	length of the reply to be sent
 * @deprecated
 */
void send_reply(uart_registers_t* uart, alt_u8* reply, int _len){
	ipg_uart_transmit(uart, reply, _len);
}

/**This function transmits a buffer of characters via a UART interface.
 * @param[in,out] 	uart	pointer to the UART interface registers.
 * @param[in] 		reply	pointer to the characters to be transmitter
 * @param[in]		_len	length of the reply to be sent
 */
void ipg_uart_transmit(uart_registers_t* _uart, alt_u8* _tx, alt_u32 _len){
	int count = 0;
	while (count < _len){
		if (!_uart->uart_sts_reg->bits.TRDY){
			continue;
		}
		_uart->uart_tx_reg->all = _tx[count];
		count++;
	}
}
