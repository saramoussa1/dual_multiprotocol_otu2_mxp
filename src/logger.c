/*
 * logger.c
 *
 *  Created on: Jun 27, 2019
 *      Author: dcham
 */
#include "logger.h"
#include "stdio.h"
#include "stdarg.h"

#define LOGGERUART _logger_ptr->_uart
/**
 * Function to initialize logger
 * @param[in] _logger_ptr pointer to logger object
 * @param[in] _uart_ptr pointer to uart object
 */
void logger_init(logger_t* _logger_ptr, uart_registers_t* _uart_ptr)
{
	_logger_ptr->_ptr.rd = 0;
	_logger_ptr->_ptr.wr = 0;
	LOGGERUART = _uart_ptr;
}

__attribute((format(printf, 2, 3)))
/**
 * Function to log logger
 * @param[in] _logger_ptr pointer to logger object
 * @param[in] format_ptr string of character
 */
void logger_log(logger_t* _logger_ptr, const char* format_ptr, ...)
{
	va_list va;
	va_start(va, format_ptr);
	_logger_ptr->_ptr.wr +=
	vsprintf(&_logger_ptr->_msg[_logger_ptr->_ptr.wr], format_ptr, va);
	va_end(va);
	/*re-enable TX interrupts*/
	LOGGERUART->uart_cntrl_reg->bits.ITRDY = 1;
}
/**
 * Function to print one logger
 * @param[in] _logger_ptr pointer to logger object
 */
void logger_print_one(logger_t* _logger_ptr)
{
	if(_logger_ptr->_ptr.rd == _logger_ptr->_ptr.wr)
	{
		_logger_ptr->_uart->uart_cntrl_reg->bits.ITRDY = 0;
		return;
	}
	LOGGERUART->uart_tx_reg->all = _logger_ptr->_msg[_logger_ptr->_ptr.rd++];
}
