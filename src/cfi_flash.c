/*
 * cfi_flash.c
 *
 *  Created on: Feb 27, 2018
 *      Author: IPG-CLARA
 */
#include "altera_avalon_cfi_flash.h"
#include "cfi_flash.h"
#include "system.h"

#ifndef FLASH_TRISTATE_CONTROLLER_0_NAME
#warning "define FLASH_TRISTATE_CONTROLLER_0_NAME"
#endif

ipglb_io_t reset_bit;
ipglb_io_t wp_bit;

ALTERA_AVALON_CFI_FLASH_INSTANCE(FLASH_TRISTATE_CONTROLLER_0, cfi_0);
/**
 * Function to initialize cfi flash
 * @param[in] reset_bit   reset bit
 * @param[in] wp_bit      write protect bit
 * Return none
 *
 */
void cfi_flash_init(ipglb_io_t reset_bit, ipglb_io_t wp_bit)
{
	set_bit_state(reset_bit.address, reset_bit.bit, 1);
	set_bit_state(wp_bit.address, wp_bit.bit, 1);
	ALTERA_AVALON_CFI_FLASH_INIT(FLASH_TRISTATE_CONTROLLER_0, cfi_0);
}
/**
 * Function to write cfi flash
 * @param[in] offset    offset value
 * @param[in] src_addr  source address
 * @param[in] length    length
 * @return -1: fail, 1: success
 */
int cfi_flash_write( int offset,alt_u8* src_addr,int length )
{
	int ret_code = -1;
	alt_flash_fd* fd;
	fd = alt_flash_open_dev(FLASH_TRISTATE_CONTROLLER_0_NAME);
	if(fd != NULL)
	{
		ret_code = alt_write_flash(fd,offset,src_addr,length);
	}
	alt_flash_close_dev(fd);
	return (ret_code);
}
/**
 * Function to read cfi flash
 * @param[in] offset     offset value
 * @param[in] dest_addr  destination address
 * @param[in] length     length
 * @return -1: fail, 1: success
 */
int cfi_flash_read( int offset,alt_u8* dest_addr,int length )
{
	int ret_code = -1;
	alt_flash_fd* fd;
	fd = alt_flash_open_dev(FLASH_TRISTATE_CONTROLLER_0_NAME);
	if(fd != NULL)
	{
		ret_code = alt_read_flash(fd,offset,dest_addr,length);
	}
	alt_flash_close_dev(fd);
	return (ret_code);
}
/**
 * Function to erase cfi flash
 * @param[in] offset   offset value
 * @param[in] length   length
 * @return -1: fail, 1: success
 */
int cfi_flash_erase( int offset,int length )
{
	int ret_code = -1;
	alt_flash_fd* fd;
	fd = alt_flash_open_dev(FLASH_TRISTATE_CONTROLLER_0_NAME);
	if(fd != NULL)
	{
		ret_code = alt_erase_flash_block(fd,offset,length);
	}
	alt_flash_close_dev(fd);
	return (ret_code);
}
/**
 * Function to read cfi flash byte
 * @param[in] offset     offset value
 * @return 0xFF: fail, destination address: success
 */
alt_u8 cfi_flash_read_byte(int offset)
{
	 alt_u8 dest_addr[1];
	 alt_u8 return_val = 0;
	 int ret_code = -1;

	 ret_code = cfi_flash_read(offset,dest_addr,1 );
	 if (0 == ret_code)/*Success*/
	 {
		 return_val = dest_addr[0];
	 }
	 else
	 {
		 return_val = 0xFF;
	 }
	 return (return_val);
}
/**
 * Function to write cfi flash byte
 * @param[in] offset  offset value
 * @param[in] data    value to write
 * @return -1: fail, 1: success
 */
int cfi_flash_write_byte(int offset, alt_u8 data)
{
	alt_u8 src[1];
	src[0] = data;
	return (cfi_flash_write(offset,src,1));
}
