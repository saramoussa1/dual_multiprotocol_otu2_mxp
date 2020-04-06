/*
 * cfi_flash.h
 *
 *  Created on: Feb 27, 2018
 *      Author: IPG-CLARA
 */

#ifndef CFI_FLASH_H_
#define CFI_FLASH_H_

#include "sys/alt_flash.h"
#include "common/inc/ipg_types.h"

void cfi_flash_init(ipglb_io_t reset_bit, ipglb_io_t wp_bit);
int cfi_flash_write(int offset,alt_u8* src_addr, int length);
int cfi_flash_read(int offset, alt_u8* dest_addr,int length);
alt_u8 cfi_flash_read_byte(int offset);
int cfi_flash_write_byte(int offset, alt_u8 data);

#endif /* CFI_FLASH_H_ */
