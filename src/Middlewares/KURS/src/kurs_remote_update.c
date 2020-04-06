/*
 * kurs_remote_update.c
 *
 *  Created on: June 26, 2018
 *      Author: Clara Zaiter
 */
#include <stddef.h>
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <inttypes.h>
#include "../inc/kurs_remote_update.h"
#include "../inc/kurs_helper.h"
#include "../inc/kurs.h"
#include "alt_types.h"
#include "../../HAL/inc/sys/alt_alarm.h"
#include "../inc/S29GLxxxP.h"

extern char reboot_nios;

/*
* @Brief: Function that writes the program copy block by block into the external flash memory
* @dwCopyAddr: address in the external memory where writing will take place
* @pWrBuf: data of the block to be written
* @dwWrBufSize: size of the block to be written
* @bFinishLoad: flag that indicates termination of writing the program copy
* @bTestIdent: flag of signature check request of the over-written program (in case of unsuccessful check, writing is not executed)
* @retval: code of the error or success
*/
alt_u8 WriteToProgCopy( alt_u32 dwCopyAddr, alt_u8 * pWrBuf, alt_u32 dwWrBufSize, alt_u8 bFinishLoad, alt_u8 bTestIdent  )
{
	alt_u8 byError = 0, by;
	alt_u32 offsetAddr = dwCopyAddr - FLASH_COPY_START;
	//address range check
	if( dwCopyAddr < FLASH_COPY_START || dwCopyAddr >= FLASH_COPY_START + FLASH_PROG_SIZE ||
		dwCopyAddr + dwWrBufSize > FLASH_COPY_START + FLASH_PROG_SIZE ) byError = 1;
	if( dwWrBufSize && !byError )
	{
		// if there are data for writing - write it
		if( dwCopyAddr + dwWrBufSize > FLASH_COPY_START + FLASH_PROG_SIZE - 4 )
		{
			dwWrBufSize = FLASH_COPY_START + FLASH_PROG_SIZE - 4 - dwWrBufSize;
		}
		//Copy the remote image to the CFI flash
		lld_memcpy(FLASH_BASE_ADDR, dwCopyAddr, 64, pWrBuf);
		dwCopyAddr += dwWrBufSize;
	}
	if( bFinishLoad && !byError ) //End of program. Writing of the last blocks and writing of the checksum.
	{
		alt_u32 dwCRC = 0;
		dwCRC = calc_crc16( (char*)(FLASH_BASE_ADDR + REMOTE_IMAGE_OFFSET_IN_FLASH), REMOTE_IMAGE_SIZE_IN_FLASH - 4 );
		lld_memcpy(FLASH_BASE_ADDR, REMOTE_IMAGE_OFFSET_IN_FLASH + REMOTE_IMAGE_SIZE_IN_FLASH - 4, 4, dwCRC);

//		dwCRC = Calc_CRC16( (alt_u8*)(SDRAM_BASE+0x100000), 0x100000 - 4 );
//		TEMP_MEM_WR_byte((FLASH_COPY_START + FLASH_PROG_SIZE - 4), (dwCRC&0xFF));//LSB
//		TEMP_MEM_WR_byte((FLASH_COPY_START + FLASH_PROG_SIZE - 3), (dwCRC>>8)&0xFF);//MSB

	}
	if( byError ) // reprogramming error, recover the old one from the main program space of the MC external memory
	{
		//TODO: handling error case
		// erasing
//		cfi_flash_erase( FLASH_COPY_START, FLASH_PROG_SIZE );
//		SPI_FLASH_ResetToNewBlock();
//		WDT_Reset();
//		alt_u32 dwProg = FLASH_PROG_START;
//		alt_u32 dwCopy = FLASH_COPY_START;
//		for( dwProg = FLASH_PROG_START, dwCopy = FLASH_COPY_START;
//			dwProg < FLASH_PROG_START + FLASH_PROG_SIZE; dwProg += 256, dwCopy += 256 )// write page by page
//		{
//			cfi_flash_write( dwCopy, (void*)dwProg, 256 );
////			WDT_Reset();
//		}
//		SPI_FLASH_ResetToNewBlock();
	}
	return byError;
}


/*
* @Brief: Function that erases the external flash memory reserved for the program copy
* @retval: operation success result
*/
int cfi_flash_erase( int offset,int length );
int PrepareEraseProgCopy( void )
{
#if 0
	WDT_Reset();
	alt_u8 bOk = SPI_FLASH_Erase( FLASH_COPY_START, FLASH_PROG_SIZE );
	WDT_Reset();
	SPI_FLASH_ResetToNewBlock();
	return bOk;
#else
#define CFI_SECTOR_SIZE 0x20000
	//We have allocated 10 sectors for each image and thus we need to erase these 10 sectors
	//cfi_flash_erase only erases 1 sector at a time
	int ind = 0;
	for(ind = 0; ind < 10; ind++){
		cfi_flash_erase(REMOTE_IMAGE_OFFSET_IN_FLASH + (CFI_SECTOR_SIZE*ind), CFI_SECTOR_SIZE);
	}
	return 0;
#endif
}

//============= Additional functions of processing the loader protocol ==========
/**This function builds a reply to a loader configuration request for reprogramming
* @param[out] 	szParserBuf the resulting string used as part of the reply
* @return length of the resulting string
* @since 0.0.7
*/
uint32_t MakeBLCfgAnswerHEXCRC(char* szParserBuf){
	char * str;
	str = szParserBuf + 2;
	strcpy( str, "FWCONFIG;HEXFWLOADER;FWSTARTADDR:" );//base header
	str += strlen( "FWCONFIG;HEXFWLOADER;FWSTARTADDR:" );
	sprintf(str, "%.8X", FLASH_PROG_START );// program (firmware) start address
	str += 8;// 8 digits for each address
	strcpy( str, ";FWRESSIZE:" );
	str += strlen( ";FWRESSIZE:" );
	sprintf(str, "%.8X",FLASH_PROG_SIZE );// size of the reserved space for the program
	str += 8;// 8 digits for each address
	strcpy( str, ";LANGRESSIZE:" );
	str += strlen(  ";LANGRESSIZE:"  );
	sprintf(str,"%.8X", FLASH_LANG_SIZE );// size of the reserved space for the language pack
	str += 8;// 8 digits for each address
	*str++ = ';';
	str += sprintf(str, "%.4X\r", get_crc16_ascii(szParserBuf, str - szParserBuf));//Check sum and OD
	*str = 0;
	return (uint32_t)( str - szParserBuf );
}

/**This function appends a CRC string to another string.
* @param[out] 	szParserBuf the resulting string used as part of the reply
* @param[in] 	szAnswer 	the original string
* @return	length of the resulting string
* @since 0.0.7
*/
uint32_t MakeAnswerHEXCRC( char * szParserBuf, const char * szAnswer ){
	char * str;
	str = szParserBuf + 2;
	strcpy( str, szAnswer );
	str += strlen( str );
	str = WordToHexStr( str, get_crc16_ascii( szParserBuf, str - szParserBuf ) );//Check sum and OD
	*str++ = '\r';
	*str = 0;
	return (uint32_t)( str - szParserBuf );
}

/**This function adds the address of the next block to be loaded to the reply string.
* @param[out] 	szParserBuf the resulting string used as part of the reply
* @param[in]	dwAddr		address of the next block to be loaded
* @return length of the resulting string
*/
uint32_t MakeAddrAnswerHEXCRC( char * szParserBuf, uint32_t dwAddr ){
	char szAddrBuf[ 12 ];
	UintToHexStr( szAddrBuf, dwAddr );
	return MakeAnswerHEXCRC( szParserBuf, szAddrBuf );
}

/*
* @Brief: Handle the "Launch the processor safe reprogramming" command.
* @str_rx: the request string
* @str_tx: the reply string that needs to be sent, populated inside this function
* @reply_len: the length of the reply string, populated inside this function
* @retval None
*/
void HandleKursSafeReprogramming( alt_u8 *str_rx, alt_u8 *str_tx, int *reply_len )
{
	alt_u8 *str = str_tx;

	alt_u8 data_error = 0;
	static alt_u32 dwCurFwAddr;
	static alt_u32 dwCurCopyAddr;
	static alt_u8 bVerifyIdent = true;//false;//true;
	static alt_u8 bFwUpdateStarted = false;
	static alt_u32 dwLastActionTime = 0;
	static alt_u32 dwLastTimeout = 0;
	alt_u32 dwLen = 0;

	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	//str += 2;

	//Find the first occurrence of 0x0D which is the indication of the end of the request
	alt_u8 *end_request = (alt_u8*)strchr((char*)str_rx, '\r');
	if(end_request != 0 && end_request != NULL)
	{
		dwLen = (end_request - str_rx); //length of the received request EXcluding <0x0D>
	}
	if(dwLen < 6) //Too short request
	{
		data_error = 1;
	}

	else //Str4HexToU16
	{
		// check for CRC mismatch NOTE: CRC uses strictly 4 symbols
		alt_u16 crc_read = 0, crc_match = 0;
		alt_u8 bHex = Str4HexToU16( (char*)str_rx + dwLen - 4, &crc_read ); // CRC received
		*(alt_u8*)( str_rx + dwLen - 4 ) = 0;
		crc_match = get_crc16_ascii((char *)str_rx, dwLen - 4); // CRC that we calculated
		if (!bHex || (crc_read != crc_match)) // they should match
		{
			data_error = 1;
			/**
			 * When doing remote update, ARM UART was getting ORE
			 * this is probably due to the fact that in OTU1 multiponder
			 * NiOS runs at 50 MHz. And with a mere 10uS to process incoming data
			 * we are missing data bytes resulting in CRC mismatch.
			 * Normally in this case, function will exit with a reply packet
			 * set to NA.
			 * As a trial, DC decided to ignore wrong packets so the CU will
			 * retry instead of existing with an error.
			 */
			if(bFwUpdateStarted){
				*reply_len = 0;
				return;
			}
		}
	}

	if(data_error)
	{
		*reply_len = MakeAnswerHEXCRC((char*)str, "NA" );
		return; // If there is a control sum mismatch - error
	}

	// No data Error
	// If the control sum is valid, process the request by commands
	if( !strncmp( (char*)str_rx + 2, "FWCONFIG", 8 ) )// Request reprogramming parameters
	{
		*reply_len = MakeBLCfgAnswerHEXCRC((char*)str);
	}
	else if( !strncmp( (char*)str_rx + 2, "DISABLESIGNATURE", 16  ) )// Turning OFF the signature check (at the manufacturing firm)
	{
		bVerifyIdent = false;
		*reply_len = MakeAnswerHEXCRC((char*)str, "DISABLESIGNATURE;OK" );
	}
	else if( !strncmp( (char*)str_rx + 2, "SOFTRESET", 9 ) )// MC program reset
	{
//		WDT_MCUReset();//NVIC_SystemReset();// reset the MCU -- No reply to this command
		*reply_len = MakeAnswerHEXCRC( (char*)str, "OK" );
	}
//	else if( !strncmp( str_rx + 2, "HEXLOAD", 7) && SPI_FLASH_WaitReady( 50000 ) )// Launch the program reprogramming procedure
	else if( !strncmp( (char*)str_rx + 2, "HEXLOAD", 7) )// Launch the program reprogramming procedure
	{
		// Start of reprogramming, initialize the parameters with start values
		bFwUpdateStarted = true; //Launched reprogramming flag
		dwLastActionTime = alt_nticks();// time of last request - for timeout control
		dwLastTimeout = 30*alt_ticks_per_second();// timeout for next request in micro seconds i.e. 30 seconds
		dwCurFwAddr = FLASH_PROG_START;// program start address
		dwCurCopyAddr = FLASH_COPY_START;// The start address of the program copy in the external FLASH

		PrepareEraseProgCopy();// Erase the external flash space reserved for the program copy

		*reply_len = MakeAddrAnswerHEXCRC( (char*)str, dwCurFwAddr );// Form reply containing the address of the next block to be written
	}
	else if( alt_nticks() - dwLastActionTime > dwLastTimeout )// The above listed commands are executed without timeouts control, but the reprogramming process is done by timeouts
	{
		// controlling the timeouts during the reprogramming process; on exit abort reprogramming
		bFwUpdateStarted = 0;
//		SPI_FLASH_ResetToNewBlock();
		*reply_len = MakeAnswerHEXCRC( (char*)str, "EFINISH" );
	}
//	else if( bFwUpdateStarted && !strncmp( str_rx + 2, "FINISH", 6 ) && SPI_FLASH_WaitReady( 50000 ) )// planned reprogramming termination command
	else if( bFwUpdateStarted && !strncmp((char*) str_rx + 2, "FINISH", 6 ) )// planned reprogramming termination command
	{
		//---------- End of loading --------------
		bFwUpdateStarted = false;
		alt_u8 byUpdateFwErr = WriteToProgCopy( dwCurCopyAddr, NULL, 0, true, bVerifyIdent );// end reprogramming, see signature status
		bVerifyIdent = true;
		switch( byUpdateFwErr )// different replies, depending on results of reprogramming termination
		{
			case 1: *reply_len = MakeAnswerHEXCRC((char*)str, "EFINISH" ); break;// termination error
			case 2: *reply_len = MakeAnswerHEXCRC((char*)str, "EFINISHSIGN" ); break;// Invalid signature
			default:
			{
				*reply_len = MakeAnswerHEXCRC((char*)str, "COMPLETE" );
//				RCC_APB2PeriphResetCmd(NIOS_COM_CLK, DISABLE);//Disable NiOS UART communication
				reboot_nios = 1; //Signal the ARM to boot again
			};
			break;
		}
	}
//	else if( bFwUpdateStarted && SPI_FLASH_WaitReady( 50000 ) )// Reception and copying of blocks into the Flash
	else if( bFwUpdateStarted )// Reception and copying of blocks into the Flash
	{
		dwLastActionTime = alt_nticks();// For controlling the timeout of the next block
		dwLastTimeout = 3*alt_ticks_per_second();
		// process the request according to the format
		alt_u8 bWriteErr = false;
		alt_u32 dwAddrRcv;
		alt_u8 bHex = Str8HexToU32((char*)str_rx + 2, &dwAddrRcv );// block address

//		if(dwAddrRcv == 0x10000)
//		{
//			int dummy = 1;
//		}
		if( bHex && dwAddrRcv == dwCurFwAddr )// the block address must correspond to the next block in queue for reprogramming
		{
			// For the block with a correct address, process the contents and transform the block's data to binary
			alt_u8 byFwBuf[ FWHEX_PACKET_SIZE ];
			alt_u8 * str = str_rx + ( 2 + 8 );
			alt_u32 i = 0;
			for( i = 0; i < FWHEX_PACKET_SIZE; str += 2, i++ )
			{
				if( !Str2HexToU8((char*)str, &byFwBuf[ i ] ) )
				{
					bHex = 0;
					break;
				}
			}
			if( bHex )// Upon a successful result of processing the received block, write its binary representation into the external flash
			{
				bWriteErr = WriteToProgCopy( dwCurCopyAddr, byFwBuf, FWHEX_PACKET_SIZE, false, false ) != 0;
				if( !bWriteErr )
				{
					// mark the written
					dwCurFwAddr += FWHEX_PACKET_SIZE;
					dwCurCopyAddr += FWHEX_PACKET_SIZE;
				}
			}
		}

		if( bWriteErr )// in case of an error, abort reprogramming
		{
			bFwUpdateStarted = false;
			*reply_len = MakeAnswerHEXCRC( (char*)str, "EFINISH" );
		}
		else *reply_len = MakeAddrAnswerHEXCRC( (char*)str, dwCurFwAddr ); // reply about success in case of correctly writing the received block
	}

	if( *reply_len == 0 )// no reply - therefore report about an error
	{
		//<0x80+SlotAddr><0x0F>NA<CRC><0x0D>
		*reply_len = MakeAnswerHEXCRC( (char*)str, "NA" );
	}
}

alt_u32 FlashCalcCRC32(alt_u8 *flash_addr, int bytes)
{
  alt_u32 crcval = 0xffffffff;
  int i, buf_index, copy_length;
  alt_u8 cval;
  char flash_buffer[FLASH_BUFFER_LENGTH];

  while(bytes != 0)
  {
    copy_length = (FLASH_BUFFER_LENGTH < bytes) ? FLASH_BUFFER_LENGTH : bytes;
    memcpy( flash_buffer, flash_addr, copy_length );
//    CopyFromFlash(flash_buffer, flash_addr, copy_length);
    for(buf_index = 0; buf_index < copy_length; buf_index++ )
    {
      cval = flash_buffer[buf_index];
      crcval ^= cval;
      for (i = 8; i > 0; i-- )
      {
        crcval = (crcval & 0x00000001) ? ((crcval >> 1) ^ 0xEDB88320) : (crcval >> 1);
      }
      bytes--;
    }
    flash_addr += FLASH_BUFFER_LENGTH;
  }
  return crcval;
}

int ValidateFlashImage(void *image_ptr)
{
   my_flash_header_type temp_header  __attribute__((aligned(4)));

   /*
   * Again, we don't assume the image is word aligned, so we copy the header
   * from flash to a word aligned buffer.
   */
   memcpy( &temp_header, image_ptr, 32 );
//   CopyFromFlash(&temp_header, image_ptr, 32);

  // Check the signature first
  if( temp_header.signature == 0xa5a5a5a5 )
  {
    // Signature is good, validate the header crc
    if( temp_header.header_crc != FlashCalcCRC32( (alt_u8*)image_ptr, 28) )
    {
      // Header crc is not valid
      return HEADER_CRC_INVALID;
     }
    else
    {
      // header crc is valid, now validate the data crc
      if ( temp_header.data_crc == FlashCalcCRC32( image_ptr + 32, temp_header.data_length) )
      {
        // data crc validates, the image is good
        return CRCS_VALID;
      }
      else
      {
        // data crc is not valid
        return DATA_CRC_INVALID;
      }
    }
  }
  else
  {
    // bad signature, return 1
    return SIGNATURE_INVALID;
  }
}
