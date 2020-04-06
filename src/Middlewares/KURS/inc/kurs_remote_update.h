/*
 * kurs_remote_update.h
 *
 *  Created on: June 26, 2018
 *      Author: Clara Zaiter
 */

#ifndef KURS_REMOTE_UPDATE_H_
#define KURS_REMOTE_UPDATE_H_

#include "../inc/kurs.h"
#include "io.h"
#include "system.h"

#define INITIAL_IMAGE_OFFSET_IN_FLASH				0x240000//+Bootloader
#define REMOTE_IMAGE_OFFSET_IN_FLASH				0x440000
#define REMOTE_IMAGE_SIZE_IN_FLASH					(REMOTE_IMAGE_OFFSET_IN_FLASH - INITIAL_IMAGE_OFFSET_IN_FLASH)
#ifndef FLASH_BASE_ADDR
#define FLASH_BASE_ADDR								FLASH_TRISTATE_CONTROLLER_0_BASE
#endif
/*following values used for remote update*/
#define FLASH_PROG_START 							REMOTE_IMAGE_OFFSET_IN_FLASH//INITIAL_IMAGE_OFFSET_IN_FLASH
#define FLASH_PROG_SIZE 							REMOTE_IMAGE_SIZE_IN_FLASH
#define FLASH_PROG_END 								( FLASH_PROG_START + FLASH_PROG_SIZE )
#define FLASH_COPY_START 							REMOTE_IMAGE_OFFSET_IN_FLASH
#define FLASH_COPY_END 								REMOTE_IMAGE_OFFSET_IN_FLASH + REMOTE_IMAGE_SIZE_IN_FLASH

//#define DEVICE_SIGNATURE_SIZE 	0							//!<size of the signature

#define FWHEX_PACKET_SIZE 							64			//!<size of the packet used to send binary file from EMS to device

#define FLASH_LANG_START	LANG_PACK_OFFSET_IN_FLASH			//!< start address of language file
#define FLASH_LANG_SIZE		LANG_BLOCK_MAX_CNT*LANG_BLOCK_SIZE

void HandleKursSafeReprogramming( alt_u8 *str_rx, alt_u8 *str_tx, int *reply_len );
uint32_t MakeAnswerHEXCRC( char * szParserBuf, const char * szAnswer );
uint32_t MakeBLCfgAnswerHEXCRC( char * szParserBuf );
int PrepareEraseProgCopy( void );
alt_u8 WriteToProgCopy( alt_u32 dwCopyAddr, alt_u8 * pWrBuf, alt_u32 dwWrBufSize, alt_u8 bFinishLoad, alt_u8 bTestIdent  );
uint32_t MakeAddrAnswerHEXCRC( char * szParserBuf, uint32_t dwAddr );

/* Section related to the validation of the remote image */
#	define BOOT_IMAGE_2_OFFSET  ( 0x00440000 )
#	define BOOT_IMAGE_2_ADDR  ( FLASH_TRISTATE_CONTROLLER_0_BASE + BOOT_IMAGE_2_OFFSET )
/*
 * Some CRC error codes for readability.
 */
#define CRCS_VALID 0
#define SIGNATURE_INVALID 1
#define HEADER_CRC_INVALID 2
#define DATA_CRC_INVALID 3

/*
 * Size of buffer for processing flash contents
 */
#define FLASH_BUFFER_LENGTH 256

/*
 * The boot images stored in flash memory, have a specific header
 * attached to them.  This is the structure of that header.  The
 * perl script "make_header.pl", included with this example is
 *  used to add this header to your boot image.
 */
typedef struct {
  alt_u32 signature;
  alt_u32 version;
  alt_u32 timestamp;
  alt_u32 data_length;
  alt_u32 data_crc;
  alt_u32 res1;
  alt_u32 res2;
  alt_u32 header_crc;
} my_flash_header_type;

int ValidateFlashImage(void *image_ptr);
alt_u32 FlashCalcCRC32(alt_u8 *flash_addr, int bytes);
#endif /* KURS_REMOTE_UPDATE_H_ */
