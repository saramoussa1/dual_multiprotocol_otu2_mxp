/*
 * linker.h - Linker script mapping information
 *
 * Machine generated for CPU 'cpu_0' in SOPC Builder design 'oduk_muxponder_dual_qsys'
 * SOPC Builder design path: ../sopc/oduk_muxponder_dual_qsys.sopcinfo
 *
 * Generated: Mon Feb 10 11:37:11 EET 2020
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

#ifndef __LINKER_H_
#define __LINKER_H_


/*
 * BSP controls alt_load() behavior in crt0.
 *
 */

#define ALT_LOAD_EXPLICITLY_CONTROLLED


/*
 * Base address and span (size in bytes) of each linker region
 *
 */

#define APP_SDRAM_REGION_BASE 0x2000000
#define APP_SDRAM_REGION_SPAN 16777216
#define BOOT_CFI_REGION_BASE 0x3000020
#define BOOT_CFI_REGION_SPAN 2359264
#define BOOT_OCRAM_REGION_BASE 0x400400
#define BOOT_OCRAM_REGION_SPAN 100000
#define CONSTANTS_REGION_BASE 0x2000
#define CONSTANTS_REGION_SPAN 32
#define IMG1_CFI_REGION_BASE 0x3240000
#define IMG1_CFI_REGION_SPAN 2097152
#define IMG2_CFI_REGION_BASE 0x3440000
#define IMG2_CFI_REGION_SPAN 2097152
#define LNG_CFI_REGION_BASE 0x3640000
#define LNG_CFI_REGION_SPAN 2097152
#define ONCHIP_MEMORY2_0_BEFORE_EXCEPTION_REGION_BASE 0x400000
#define ONCHIP_MEMORY2_0_BEFORE_EXCEPTION_REGION_SPAN 32
#define ONCHIP_MEMORY2_0_REGION_BASE 0x400020
#define ONCHIP_MEMORY2_0_REGION_SPAN 992
#define RESET_REGION_BASE 0x3000000
#define RESET_REGION_SPAN 32
#define USER_CFI_REGION_BASE 0x3840000
#define USER_CFI_REGION_SPAN 2097152


/*
 * Devices associated with code sections
 *
 */

#define ALT_EXCEPTIONS_DEVICE ONCHIP_MEMORY2_0
#define ALT_RESET_DEVICE ONCHIP_MEMORY2_0
#define ALT_RODATA_DEVICE SDRAM
#define ALT_RWDATA_DEVICE SDRAM
#define ALT_TEXT_DEVICE SDRAM


/*
 * Initialization code at the reset address is allowed (e.g. no external bootloader).
 *
 */

#define ALT_ALLOW_CODE_AT_RESET


/*
 * The alt_load() facility is called from crt0 to copy sections into RAM.
 *
 */

#define ALT_LOAD_COPY_EXCEPTIONS
#define ALT_LOAD_COPY_RODATA
#define ALT_LOAD_COPY_RWDATA

#endif /* __LINKER_H_ */
