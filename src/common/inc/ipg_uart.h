#ifndef _IPG_UART_H_
#define _IPG_UART_H_

#include <system.h>
#include "ipg_bits.h"
#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "io.h"
#include "altera_avalon_uart_regs.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/alt_alarm.h>

/*
 * UART registers and bits
 */
union UART_RXDATA_REG
{
	alt_u32 all;// used to write or read from the whole register
};

union UART_TXDATA_REG
{
	alt_u32 all;// used to write or read from the whole register
};

struct UART_STATUS_BITS
{
	volatile unsigned int  PE:1;		// bit 0
	volatile unsigned int  FE:1;		// bit 1
	volatile unsigned int  BRK:1;		// bit 2
	volatile unsigned int  ROE:1;		// bit 3
	volatile unsigned int  TOE:1;		// bit 4
	volatile unsigned int  TMT:1;		// bit 5
	volatile unsigned int  TRDY:1;		// bit 6
	volatile unsigned int  RRDY:1;		// bit 7
	volatile unsigned int  E:1;			// bit 8
	volatile unsigned int  :1;			// bit 9
	volatile unsigned int  DCTS:1;		// bit 10
	volatile unsigned int  CTS:1;		// bit 11
	volatile unsigned int  EOP:1;		// bit 12
	volatile unsigned int  :19;			// bit 31 to bit 13
};
union UART_STATUS_REG
{
	alt_u32 all;// used to write or read from the whole register
	struct UART_STATUS_BITS bits;
};
struct UART_CONTROL_BITS
{
	volatile unsigned int  IPE:1;
	volatile unsigned int  IFE:1;
	volatile unsigned int  IBRK:1;
	volatile unsigned int  IROE:1;
	volatile unsigned int  ITOE:1;
	volatile unsigned int  ITMT:1;
	volatile unsigned int  ITRDY:1;
	volatile unsigned int  IRRDY:1;
	volatile unsigned int  IE:1;
	volatile unsigned int  TRBK:1;
	volatile unsigned int  IDCTS:1;
	volatile unsigned int  RTS:1;
	volatile unsigned int  IEOP:1;
	volatile unsigned int  :19;
};
union UART_CONTROL_REG
{
	alt_u32 all;// used to write or read from the whole register
	struct UART_CONTROL_BITS bits;
};
typedef struct
{
     volatile union UART_RXDATA_REG* uart_rx_reg;
     volatile union UART_TXDATA_REG* uart_tx_reg;
     volatile union UART_STATUS_REG* uart_sts_reg;
     volatile union UART_CONTROL_REG* uart_cntrl_reg;
}uart_registers_t;

void init_uart(uart_registers_t* uart, alt_u8 _interrupt_controller, alt_u8 _irq);
void map_uart_regs(uart_registers_t* uart, alt_u32 uart_base);
void send_reply(uart_registers_t* uart, alt_u8* reply, int _len);
void ipg_uart_enable(uart_registers_t* uart);
void ipg_uart_disable(uart_registers_t* uart);
void ipg_uart_enable_IROE(uart_registers_t* uart);
void ipg_uart_disable_IROE(uart_registers_t* uart);
void ipg_uart_enable_IRRDY(uart_registers_t* uart);
void ipg_uart_disable_IRRDY(uart_registers_t* uart);
void ipg_uart_enable_ITRDY(uart_registers_t* uart);
void ipg_uart_disable_ITRDY(uart_registers_t* uart);
void ipg_uart_clear_ORE(void* context);
void callback_uart_roe(void* context);
void callback_uart_trdy(void* context);
void callback_uart_rrdy(void* context, alt_u16 data);
void ipg_uart_transmit(uart_registers_t* _uart, alt_u8* _tx, alt_u32 _len);
#endif /*_IPG_UART_H_*/
