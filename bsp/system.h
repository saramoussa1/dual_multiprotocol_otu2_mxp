/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'cpu_0' in SOPC Builder design 'oduk_muxponder_dual_qsys'
 * SOPC Builder design path: ../sopc/oduk_muxponder_dual_qsys.sopcinfo
 *
 * Generated: Mon Mar 30 11:36:06 EEST 2020
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_qsys"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x00020020
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1d
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x00400020
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 1
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x1a
#define ALT_CPU_NAME "cpu_0"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_RESET_ADDR 0x00400000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x00020020
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1d
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x00400020
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 1
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x1a
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_RESET_ADDR 0x00400000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_SPI
#define __ALTERA_AVALON_TIMER
#define __ALTERA_AVALON_UART
#define __ALTERA_GENERIC_TRISTATE_CONTROLLER
#define __ALTERA_NIOS2_QSYS
#define __AVALON_SPI_MASTER
#define __ODUK_MPIF_BRICK
#define __SLS_AVALON_I2C_M


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Stratix IV"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/null"
#define ALT_STDERR_BASE 0x0
#define ALT_STDERR_DEV null
#define ALT_STDERR_TYPE ""
#define ALT_STDIN "/dev/null"
#define ALT_STDIN_BASE 0x0
#define ALT_STDIN_DEV null
#define ALT_STDIN_TYPE ""
#define ALT_STDOUT "/dev/null"
#define ALT_STDOUT_BASE 0x0
#define ALT_STDOUT_DEV null
#define ALT_STDOUT_TYPE ""
#define ALT_SYSTEM_NAME "oduk_muxponder_dual_qsys"


/*
 * avalon_spi_master configuration
 *
 */

#define ALT_MODULE_CLASS_avalon_spi_master avalon_spi_master
#define AVALON_SPI_MASTER_BASE 0x10000000
#define AVALON_SPI_MASTER_IRQ -1
#define AVALON_SPI_MASTER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AVALON_SPI_MASTER_NAME "/dev/avalon_spi_master"
#define AVALON_SPI_MASTER_SPAN 268435456
#define AVALON_SPI_MASTER_TYPE "avalon_spi_master"


/*
 * constants configuration
 *
 */

#define ALT_MODULE_CLASS_constants altera_avalon_onchip_memory2
#define CONSTANTS_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define CONSTANTS_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define CONSTANTS_BASE 0x2000
#define CONSTANTS_CONTENTS_INFO ""
#define CONSTANTS_DUAL_PORT 0
#define CONSTANTS_GUI_RAM_BLOCK_TYPE "AUTO"
#define CONSTANTS_INIT_CONTENTS_FILE "oduk_muxponder_dual_constants"
#define CONSTANTS_INIT_MEM_CONTENT 1
#define CONSTANTS_INSTANCE_ID "NONE"
#define CONSTANTS_IRQ -1
#define CONSTANTS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CONSTANTS_NAME "/dev/constants"
#define CONSTANTS_NON_DEFAULT_INIT_FILE_ENABLED 1
#define CONSTANTS_RAM_BLOCK_TYPE "AUTO"
#define CONSTANTS_READ_DURING_WRITE_MODE "DONT_CARE"
#define CONSTANTS_SINGLE_CLOCK_OP 0
#define CONSTANTS_SIZE_MULTIPLE 1
#define CONSTANTS_SIZE_VALUE 32
#define CONSTANTS_SPAN 32
#define CONSTANTS_TYPE "altera_avalon_onchip_memory2"
#define CONSTANTS_WRITABLE 0


/*
 * debug_uart configuration
 *
 */

#define ALT_MODULE_CLASS_debug_uart altera_avalon_uart
#define DEBUG_UART_BASE 0x0
#define DEBUG_UART_BAUD 115200
#define DEBUG_UART_DATA_BITS 8
#define DEBUG_UART_FIXED_BAUD 1
#define DEBUG_UART_FREQ 50000000
#define DEBUG_UART_IRQ 14
#define DEBUG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DEBUG_UART_NAME "/dev/debug_uart"
#define DEBUG_UART_PARITY 'N'
#define DEBUG_UART_SIM_CHAR_STREAM ""
#define DEBUG_UART_SIM_TRUE_BAUD 0
#define DEBUG_UART_SPAN 32
#define DEBUG_UART_STOP_BITS 1
#define DEBUG_UART_SYNC_REG_DEPTH 2
#define DEBUG_UART_TYPE "altera_avalon_uart"
#define DEBUG_UART_USE_CTS_RTS 0
#define DEBUG_UART_USE_EOP_REGISTER 0


/*
 * flash_tristate_controller_0 configuration
 *
 */

#define ALT_MODULE_CLASS_flash_tristate_controller_0 altera_generic_tristate_controller
#define FLASH_TRISTATE_CONTROLLER_0_BASE 0x3000000
#define FLASH_TRISTATE_CONTROLLER_0_IRQ -1
#define FLASH_TRISTATE_CONTROLLER_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FLASH_TRISTATE_CONTROLLER_0_NAME "/dev/flash_tristate_controller_0"
#define FLASH_TRISTATE_CONTROLLER_0_SIZE 16777216
#define FLASH_TRISTATE_CONTROLLER_0_SPAN 16777216
#define FLASH_TRISTATE_CONTROLLER_0_SUPPORTS_FLASH_FILE_SYSTEM 1
#define FLASH_TRISTATE_CONTROLLER_0_TYPE "altera_generic_tristate_controller"


/*
 * hal configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK TIMER_0
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_0 altera_avalon_jtag_uart
#define JTAG_UART_0_BASE 0x60
#define JTAG_UART_0_IRQ 4
#define JTAG_UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_0_NAME "/dev/jtag_uart_0"
#define JTAG_UART_0_READ_DEPTH 64
#define JTAG_UART_0_READ_THRESHOLD 8
#define JTAG_UART_0_SPAN 8
#define JTAG_UART_0_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_0_WRITE_DEPTH 64
#define JTAG_UART_0_WRITE_THRESHOLD 8


/*
 * oduk_mpif_brick_0 configuration
 *
 */

#define ALT_MODULE_CLASS_oduk_mpif_brick_0 oduk_mpif_brick
#define ODUK_MPIF_BRICK_0_BASE 0x1000000
#define ODUK_MPIF_BRICK_0_IRQ -1
#define ODUK_MPIF_BRICK_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ODUK_MPIF_BRICK_0_NAME "/dev/oduk_mpif_brick_0"
#define ODUK_MPIF_BRICK_0_SPAN 524288
#define ODUK_MPIF_BRICK_0_TYPE "oduk_mpif_brick"


/*
 * onchip_memory2_0 configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory2_0 altera_avalon_onchip_memory2
#define ONCHIP_MEMORY2_0_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY2_0_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY2_0_BASE 0x400000
#define ONCHIP_MEMORY2_0_CONTENTS_INFO ""
#define ONCHIP_MEMORY2_0_DUAL_PORT 0
#define ONCHIP_MEMORY2_0_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_INIT_CONTENTS_FILE "oduk_muxponder_dual"
#define ONCHIP_MEMORY2_0_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY2_0_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY2_0_IRQ -1
#define ONCHIP_MEMORY2_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY2_0_NAME "/dev/onchip_memory2_0"
#define ONCHIP_MEMORY2_0_NON_DEFAULT_INIT_FILE_ENABLED 1
#define ONCHIP_MEMORY2_0_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY2_0_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY2_0_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY2_0_SIZE_VALUE 1048576
#define ONCHIP_MEMORY2_0_SPAN 1048576
#define ONCHIP_MEMORY2_0_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY2_0_WRITABLE 1


/*
 * sdram configuration
 *
 */

#define ALT_MODULE_CLASS_sdram altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x2000000
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/sdram"
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x16
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 8
#define SDRAM_SDRAM_DATA_WIDTH 32
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 12
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 0
#define SDRAM_SPAN 16777216
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0


/*
 * sls_avalon_i2c_m configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m sls_avalon_i2c_m
#define SLS_AVALON_I2C_M_BASE 0xc00000
#define SLS_AVALON_I2C_M_IRQ 2
#define SLS_AVALON_I2C_M_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_M_NAME "/dev/sls_avalon_i2c_m"
#define SLS_AVALON_I2C_M_SPAN 64
#define SLS_AVALON_I2C_M_TYPE "sls_avalon_i2c_m"


/*
 * sls_avalon_i2c_m_li1 configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_li1 sls_avalon_i2c_m
#define SLS_AVALON_I2C_M_LI1_BASE 0xf00000
#define SLS_AVALON_I2C_M_LI1_IRQ 5
#define SLS_AVALON_I2C_M_LI1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_M_LI1_NAME "/dev/sls_avalon_i2c_m_li1"
#define SLS_AVALON_I2C_M_LI1_SPAN 64
#define SLS_AVALON_I2C_M_LI1_TYPE "sls_avalon_i2c_m"


/*
 * sls_avalon_i2c_m_li2 configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_li2 sls_avalon_i2c_m
#define SLS_AVALON_I2C_M_LI2_BASE 0xf10000
#define SLS_AVALON_I2C_M_LI2_IRQ 6
#define SLS_AVALON_I2C_M_LI2_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_M_LI2_NAME "/dev/sls_avalon_i2c_m_li2"
#define SLS_AVALON_I2C_M_LI2_SPAN 64
#define SLS_AVALON_I2C_M_LI2_TYPE "sls_avalon_i2c_m"


/*
 * sls_avalon_i2c_m_x configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_x sls_avalon_i2c_m
#define SLS_AVALON_I2C_M_X_BASE 0xd00000
#define SLS_AVALON_I2C_M_X_IRQ 1
#define SLS_AVALON_I2C_M_X_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_M_X_NAME "/dev/sls_avalon_i2c_m_x"
#define SLS_AVALON_I2C_M_X_SPAN 64
#define SLS_AVALON_I2C_M_X_TYPE "sls_avalon_i2c_m"


/*
 * sls_avalon_i2c_m_y configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_y sls_avalon_i2c_m
#define SLS_AVALON_I2C_M_Y_BASE 0xe00000
#define SLS_AVALON_I2C_M_Y_IRQ 3
#define SLS_AVALON_I2C_M_Y_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_M_Y_NAME "/dev/sls_avalon_i2c_m_y"
#define SLS_AVALON_I2C_M_Y_SPAN 64
#define SLS_AVALON_I2C_M_Y_TYPE "sls_avalon_i2c_m"


/*
 * spi_0 configuration
 *
 */

#define ALT_MODULE_CLASS_spi_0 altera_avalon_spi
#define SPI_0_BASE 0x80
#define SPI_0_CLOCKMULT 1
#define SPI_0_CLOCKPHASE 0
#define SPI_0_CLOCKPOLARITY 0
#define SPI_0_CLOCKUNITS "Hz"
#define SPI_0_DATABITS 8
#define SPI_0_DATAWIDTH 16
#define SPI_0_DELAYMULT "1.0E-9"
#define SPI_0_DELAYUNITS "ns"
#define SPI_0_EXTRADELAY 0
#define SPI_0_INSERT_SYNC 0
#define SPI_0_IRQ 11
#define SPI_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SPI_0_ISMASTER 0
#define SPI_0_LSBFIRST 0
#define SPI_0_NAME "/dev/spi_0"
#define SPI_0_NUMSLAVES 1
#define SPI_0_PREFIX "spi_"
#define SPI_0_SPAN 32
#define SPI_0_SYNC_REG_DEPTH 2
#define SPI_0_TARGETCLOCK 128000u
#define SPI_0_TARGETSSDELAY "0.0"
#define SPI_0_TYPE "altera_avalon_spi"


/*
 * timer_0 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_0 altera_avalon_timer
#define TIMER_0_ALWAYS_RUN 0
#define TIMER_0_BASE 0x40
#define TIMER_0_COUNTER_SIZE 32
#define TIMER_0_FIXED_PERIOD 0
#define TIMER_0_FREQ 50000000
#define TIMER_0_IRQ 12
#define TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_0_LOAD_VALUE 49999
#define TIMER_0_MULT 0.001
#define TIMER_0_NAME "/dev/timer_0"
#define TIMER_0_PERIOD 1
#define TIMER_0_PERIOD_UNITS "ms"
#define TIMER_0_RESET_OUTPUT 0
#define TIMER_0_SNAPSHOT 1
#define TIMER_0_SPAN 32
#define TIMER_0_TICKS_PER_SEC 1000
#define TIMER_0_TIMEOUT_PULSE_OUTPUT 1
#define TIMER_0_TYPE "altera_avalon_timer"


/*
 * uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_uart_0 altera_avalon_uart
#define UART_0_BASE 0x20
#define UART_0_BAUD 115200
#define UART_0_DATA_BITS 8
#define UART_0_FIXED_BAUD 1
#define UART_0_FREQ 50000000
#define UART_0_IRQ 13
#define UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define UART_0_NAME "/dev/uart_0"
#define UART_0_PARITY 'N'
#define UART_0_SIM_CHAR_STREAM ""
#define UART_0_SIM_TRUE_BAUD 0
#define UART_0_SPAN 32
#define UART_0_STOP_BITS 1
#define UART_0_SYNC_REG_DEPTH 2
#define UART_0_TYPE "altera_avalon_uart"
#define UART_0_USE_CTS_RTS 0
#define UART_0_USE_EOP_REGISTER 0

#endif /* __SYSTEM_H_ */
