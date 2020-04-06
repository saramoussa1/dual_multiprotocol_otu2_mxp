/*
 * logger.h
 *
 *  Created on: Jun 27, 2019
 *      Author: dcham
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "stdint.h"
#include "common/inc/ipg_types.h"
#include "common/inc/ipg_uart.h"

#define LOG_SIZE_1K	1024
#define LOG_SIZE_2K	2048
#define LOG_SIZE_4K	4096
#define LOG_SIZE	LOG_SIZE_4K
#if LOG_SIZE == LOG_SIZE_4K
#define CNTR_SIZE	12
#define RSRVD_SIZE	8
#endif
#if LOG_SIZE == LOG_SIZE_1K
#define CNTR_SIZE	10
#define RSRVD_SIZE	12
#endif

/*
#define LOG(x, format, args)	\
				(x._ptr.wr += sprintf(&x._msg[x._ptr.wr], format, args));	\
				USART_ITConfig(x._uart, USART_IT_TXE, ENABLE);\

*/
struct RW_Pointer{
	alt_u32 wr:CNTR_SIZE;
	alt_u32 rd:CNTR_SIZE;
	alt_u32 _reserved:RSRVD_SIZE;
};
typedef struct{
	struct 	RW_Pointer _ptr;
	char	_msg[LOG_SIZE];
	uart_registers_t* _uart;
}logger_t;


void logger_init(logger_t* _logger_ptr, uart_registers_t* _uart_ptr);
void logger_log(logger_t* _logger_ptr, const char* format_ptr, ...);
void logger_print_one(logger_t* _logger_ptr);
/*#define LOG(x, format, args)(logger_log(x, format, args))*/
#endif /* LOGGER_H_ */
