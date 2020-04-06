/****************************************************************************
 * Copyright © 2006 Altera Corporation, San Jose, California, USA.           *
 * All rights reserved. All use of this software and documentation is        *
 * subject to the License Agreement located at the end of this file below.   *
 ****************************************************************************/
/*****************************************************************************
*  File: custom_boot_copier.c
*
*  This is an example of a custom Nios II boot copier implemented in C.  
*  The boot copier can be built and run from Nios II IDE.  For instructions 
*  on how to build and run this boot copier, refer to the application note
*  that accompianies it.
*
*  Feel free to customize this boot copier, but do so at your own risk.  This 
*  boot copier can only be supported by Altera in its current, unmodified form
*  
*****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "advanced_boot_copier.h"
#include "system.h"
#include "alt_types.h"
#include "sys/alt_alarm.h"
#include "sys/alt_cache.h"
#include "sys/alt_dev.h"
#include "sys/alt_irq.h"
#include "sys/alt_sys_init.h"
#include "priv/alt_file.h"
//#include "altera_avalon_pio_regs.h"
#include "altera_avalon_jtag_uart_regs.h"
#include "altera_avalon_jtag_uart.h"

/*
 *  Edit this define to control the boot method.
 *  Options are:
 *        BOOT_FROM_CFI_FLASH
 *        BOOT_CFI_FROM_ONCHIP_ROM
 *        BOOT_EPCS_FROM_ONCHIP_ROM
 */
#define BOOT_METHOD BOOT_FROM_CFI_FLASH

/*
 * Edit this define to turn on or turn off the JTAG UART during boot
 */
#define USING_JTAG_UART 0

/*
 * These defines locate our two possible application images at specifically
 * these two offsets in the flash memory.  If you edit these defines, ensure
 * when programming the images to flash that you program them to the new 
 * locations you define here.
 */
#if BOOT_METHOD == BOOT_FROM_CFI_FLASH

#	define BOOT_IMAGE_1_OFFSET  ( 0x00240000 )
#	define BOOT_IMAGE_2_OFFSET  ( 0x00440000 )
#	define BOOT_IMAGE_1_ADDR  ( FLASH_TRISTATE_CONTROLLER_0_BASE + BOOT_IMAGE_1_OFFSET )
#	define BOOT_IMAGE_2_ADDR  ( FLASH_TRISTATE_CONTROLLER_0_BASE + BOOT_IMAGE_2_OFFSET )

#elif BOOT_METHOD == BOOT_CFI_FROM_ONCHIP_ROM

#	define BOOT_IMAGE_1_OFFSET  ( 0x00240000 )
#	define BOOT_IMAGE_2_OFFSET  ( 0x00440000 )
#	define BOOT_IMAGE_1_ADDR  ( EXT_FLASH_BASE + BOOT_IMAGE_1_OFFSET )
#	define BOOT_IMAGE_2_ADDR  ( EXT_FLASH_BASE + BOOT_IMAGE_2_OFFSET )

#elif BOOT_METHOD == BOOT_EPCS_FROM_ONCHIP_ROM

	// In an EPCS, the data is not present in the CPU's memory map, so we just
	// specify the image addresses as offsets within the EPCS.  The CopyFromFlash
	// routine will work out the details later.
	// It is important to carefully choose the offsets at which to place boot images
	// in EPCS devices, ensuring they do not overlap FPGA configuration data. These
	// offsets should be sufficient for a Cyclone II 2C35 development board.
#	define BOOT_IMAGE_1_ADDR  ( 0x00060000 )
#	define BOOT_IMAGE_2_ADDR  ( 0x00080000 )

#endif // BOOT_METHOD


/*
 * Don't edit these defines
 */
#if (BOOT_METHOD == BOOT_FROM_CFI_FLASH || BOOT_METHOD == BOOT_CFI_FROM_ONCHIP_ROM)
# define FLASH_TYPE CFI
#elif (BOOT_METHOD == BOOT_EPCS_FROM_ONCHIP_ROM)
# define FLASH_TYPE EPCS
#endif // BOOT_METHOD


/*
 * This example prints a lot of information, this macro is intended to make the
 * code a bit easier to read.  If you do not wish to print anything during boot,
 * change the define at the top of this file to "#define USING_JTAG_UART 0"
 */
#if USING_JTAG_UART 

# define JTAGPRINT(name) MyJtagWrite(name, sizeof(name)-1)
# define JTAG_UART_TIMEOUT   ( alt_ticks_per_second() * 2 ) // 2 seconds
# define JTAG_WAITING_BIT   (1 << 1)
# define JTAG_ABANDONED_BIT (1 << 2)

  // Stores the transmit state of the JTAG_UART
  volatile alt_u32 jtag_uart_state;

  // Prototypes for our JTAG UART printing functions
  int MyJtagWrite(const char *buf, int len);
  int MyJtagWrite8(const char *buf, int len);

#endif //USING_JTAG_UART


/*
 * Globally accessable variables.
 */
void *flash_image_ptr_1;
void *flash_image_ptr_2;

/*
 * The following statement defines "main()" so that when the Nios II SBT4E
 * debugger is set to break at "main()", it will break at the appropriate
 * place in this program, which does not contain a function called "main()".
*/
int main (void) __attribute__ ((weak, alias ("alt_main")));

/*****************************************************************************
*  Function: alt_main
*
*  Purpose: This is our boot copier's entry point. We are implementing 
*  this as an alt_main() instead of a main(), so that we can better control 
*  the drivers that load and the system resources that are enabled.  Since
*  code size may be a consideration, this method allows us to keep the 
*  memory requirements small.
*
*****************************************************************************/
int alt_main(void)
{
  /*
   * Define the local variables used by this function.
   */
  int boot_image;
  alt_u32 entry_point;
  
  flash_image_ptr_1 = (void*)BOOT_IMAGE_1_ADDR;
  flash_image_ptr_2 = (void*)BOOT_IMAGE_2_ADDR;
  /*
   * Define the strings used in this function for printing out STDOUT.  The 
   * purpose of this is to avoid using the potentially large printf C library.
   */
# if USING_JTAG_UART

    char stars_str[] = "****************************************\n";

#   if BOOT_METHOD == BOOT_FROM_CFI_FLASH

      char hello_str0[] = "  Example Custom Boot Copier Starting\n        Booting from Flash\n\n";
      char hello_str1[] = "\nThis copier expects application images\nto be located at offset 0x00240000 or \n0x00440000 in flash memory.\n";

#   elif BOOT_METHOD == BOOT_CFI_FROM_ONCHIP_ROM

      char hello_str0[] = "  Example Custom Boot Copier Starting\n     Booting CFI from On-Chip RAM\n\n";
      char hello_str1[] = "\nThis copier expects application images\nto be located at offset 0x00240000 or \n0x00440000 in flash memory.\n";

#   elif BOOT_METHOD == BOOT_EPCS_FROM_ONCHIP_ROM

      char hello_str0[] = "  Example Custom Boot Copier Starting\n     Booting EPCS from On-Chip RAM\n\n";
      char hello_str1[] = "\nThis copier expects application images\nto be located at offset 0x00000000 or \n0x00100000 in EPCS memory.\n";

#   endif //BOOT_FROM_FLASH

    char boot_proceed_str[] = "\nNow attempting to boot.\n";
    char reset_str[] = "\nNow jumping back to reset vector.\n";
    char no_image_str[] = "\nNo valid application image found in\nflash memory.\n";
    char image1_str[] = "\nPicked image in flash at location 1\n";
    char image2_str[] = "\nPicked image in flash at location 2\n";
    char jumping_str[] = "\nNow attempting to load and jump to the\napplication.\n\n";

    // Initialize the STDIO defaults as selected in the Nios II IDE.  
    alt_io_redirect (ALT_STDOUT, ALT_STDIN, ALT_STDERR);
    jtag_uart_state = 0x1;

# endif // USING_JTAG_UART

  /* 
   * Perform system initialization.  Since our entry point is alt_main instead 
   * of simply main, it's our responsibility to initialize any drivers we 
   * wish to use.  An example of how to do this can be found in the 
   * hello_world freestanding demo.
   */

  /*
   * In order to allow interrupts to occur while the boot copier executes we 
   * initialize the main irq handler.
   */
  alt_irq_init (ALT_IRQ_BASE);
  
  /*
   * Now we initialize the drivers that we require.
   */
  alt_sys_init();

  /*
   * If you intend to print anything during boot, you'll need to initialize 
   * the STDIO defaults as selected in the Nios II IDE.  If you dont need 
   * to print anything during boot, you can comment out this line.
   */
# if USING_JTAG_UART
    alt_io_redirect (ALT_STDOUT, ALT_STDIN, ALT_STDERR);
    jtag_uart_state = 0x1;
# endif  // USING_JTAG_UART
  
  /*
   * Now the system is initialized and ready to use.
   * 
   * At this point you could start doing whatever you feel necessary.  You could
   * perform diagnostics, validate the hardware environment, communicate with
   * other parts of the system to obtain instructions how to proceed, etc., etc
   * 
   * In this example, we'll just turn on an LED, print some basic information
   * to the JTAG UART, and then boot an application from flash.
   */
   
  // Turn on an LED to indicate we are alive
//  IOWR_ALTERA_AVALON_PIO_DATA(PERIPHERAL_SUBSYSTEM_LED_PIO_BASE, 0x1);

  
  
  /*
   * Print a startup message to let the user see we're alive.
   */
# if USING_JTAG_UART
    JTAGPRINT(stars_str);
    JTAGPRINT(stars_str);
    JTAGPRINT(hello_str0);
    JTAGPRINT(stars_str);
    JTAGPRINT(hello_str1);
    JTAGPRINT(stars_str);
    JTAGPRINT(stars_str);

# endif  //USING_JTAG_UART

  /*
   * Now we begin the boot process.
   */
 
  // Print a message to the JTAG UART that we're beginning to boot 
#if USING_JTAG_UART

  JTAGPRINT(boot_proceed_str);  

#endif  //USING_JTAG_UART

  /*
   * Pick a flash image to load.  The criteria for picking an image are 
   * discussed the text of the application note, and also in the code comments 
   * preceeding the function "PickFlashImage()" found in this file.
   */
  if( (boot_image = PickFlashImage()) ) 
  {
#   if USING_JTAG_UART
      // Print which image was picked.
      if(boot_image == 1)
        JTAGPRINT(image1_str);
      else
        JTAGPRINT(image2_str);

      JTAGPRINT(jumping_str);
#   endif  //USING_JTAG_UART
    
    /*
     * Now we're going to try to load the application into memory, this will
     * likely overwrite our current exception handler, so before we do that 
     * we'll disable interrupts and not turn them back on again. 
     * 
     * It's also important minimize your reliance on the ".rwdata", ".bss" and
     * stack sections.  Since all of these sections can exist in the exception 
     * memory they are all subject to being overwritten.  You can inspect how 
     * much of the ".rwdata" and ".bss" sections the bootcopier uses by 
     * looking at the disassembly for the bootcopier.  The disassembly can be 
     * generated by running "nios2-elf-objdump.exe" with the -d option from a 
     * Nios II Command Shell.
     */
    alt_irq_disable_all ();
   
    entry_point = LoadFlashImage( boot_image );
    if( entry_point >= 0 )  // load the image
    {
      // Turn on an LED to indicate we are jumping to a valid image.
//      IOWR_ALTERA_AVALON_PIO_DATA(PERIPHERAL_SUBSYSTEM_LED_PIO_BASE, 0x2);

      // Jump to the entry point of the application
      JumpFromBootCopier((void(*)(void))(entry_point));
    }
    else
    {
      // Turn on an LED to indicate we are resetting
//      IOWR_ALTERA_AVALON_PIO_DATA(PERIPHERAL_SUBSYSTEM_LED_PIO_BASE, 0x4);

      usleep(5000000);

      // If the entry point is not found, then we should jump back to the
      // reset vector.
      JumpFromBootCopier((void(*)(void))(NIOS2_RESET_ADDR));
    }
  }
  else
  {
    // If we didn't find an applicatoin to boot, jump back to the reset vector
#   if USING_JTAG_UART
      JTAGPRINT(no_image_str);
      JTAGPRINT(reset_str);
#   endif  //USING_JTAG_UART

    // Turn on an LED to indicate we are resetting
//    IOWR_ALTERA_AVALON_PIO_DATA(PERIPHERAL_SUBSYSTEM_LED_PIO_BASE, 0x4);
    
    // Wait 5 seconds
    usleep(5000000);
      
    // Jump back to the reset address
    JumpFromBootCopier((void(*)(void))(NIOS2_RESET_ADDR));
  }
  
  // We should never get here
  exit(0);
}


/*****************************************************************************
*  Function: JumpFromBootCopier
*
*  Purpose: This routine shuts down the boot copier and jumps somewhere else. 
*  The place to jump is passed in as a function pointer named "target".
*
*****************************************************************************/
void JumpFromBootCopier(void target(void))
{
  /*
   * If you have any outstanding I/O or system resources that needed to be 
   * cleanly disabled before leaving the boot copier program, then this is 
   * the place to do that.
   * 
   * In this example we only need to ensure the state of the Nios II cpu is 
   * equivalent to reset.  If we disable interrupts, and flush the caches, 
   * then the program we jump to should receive the cpu just as it would 
   * coming out of a hardware reset.
   */
  alt_irq_disable_all ();
  alt_dcache_flush_all ();
  alt_icache_flush_all ();
  
  /*
   * The cpu state is as close to reset as we can get it, so we jump to the new
   * application.
   */
  target();
  
  /*
   * In the odd event that the program we jump to decides to return, we should 
   * probably just jump back to the reset vector. We pass in the reset address
   * as a function pointer.
   */
   
  // Turn on an LED to indicate we are resetting
//  IOWR_ALTERA_AVALON_PIO_DATA(PERIPHERAL_SUBSYSTEM_LED_PIO_BASE, 0x4);

  // Wait 5 seconds
  usleep(5000000);

  // Jump back to the reset address
  JumpFromBootCopier((void(*)(void))(NIOS2_RESET_ADDR));
}

/*****************************************************************************
*  Function: CopyFromFlash
*
*  Purpose:  This subroutine copies data from a flash memory to a buffer
*  The function uses the appropriate copy routine for the flash that is
*  defined by FLASH_TYPE.  EPCS devices cant simply be read from using
*  memcpy().
*
*****************************************************************************/
void* CopyFromFlash( void * dest, const void * src, size_t num )
{
# if( FLASH_TYPE == CFI )

    memcpy( dest, src, num );


# elif( FLASH_TYPE == EPCS )

    // If we're dealing with EPCS, "src" has already been defined for us as 
    // an offset into the EPCS, not an absolute address.
    epcs_read_buffer( EPCS_CONTROLLER_BASE + EPCS_CONTROLLER_REGISTER_OFFSET,
                      (int)src,
                      (alt_u8*)dest,
                      (int)num );

# endif //FLASH_TYPE

  return (dest);
}

/*****************************************************************************
*  Function: LoadFlashImage
*
*  Purpose:  This subroutine loads an image from flash into the Nios II 
*  memory map.  It decodes boot records in the format produced from the 
*  elf2flash utility, and loads the image as directed by those records.  
*  The format of the boot record is described in the text of the application 
*  note.
* 
*  The input operand, "image" is expected to be the image selector indicating
*  which flash image, 1 or 2, should be loaded.
*
*****************************************************************************/
alt_u32 LoadFlashImage ( int image )
{
  alt_u8 *next_flash_byte;
  alt_u32 length;
  alt_u32 address;
  
  /*
   * Load the image pointer based on the value of "image"
   * The boot image header is 32 bytes long, so we add an offset of 32.
   */
  if( image == 1 )
  {
    next_flash_byte = (alt_u8 *)flash_image_ptr_1 + 32;
  }
  else
  {
    next_flash_byte = (alt_u8 *)flash_image_ptr_2 + 32;
  }

  /*
   * Flash images are not guaranteed to be word-aligned within the flash 
   * memory, so a word-by-word copy loop should not be used.
   * 
   * The "memcpy()" function works well to copy non-word-aligned data, and 
   * it is relativly small, so that's what we'll use.
   */
   
  // Get the first 4 bytes of the boot record, which should be a length record
  CopyFromFlash( (void*)(&length), (void*)(next_flash_byte), (size_t)(4) );
  next_flash_byte += 4;
  
  // Now loop until we get jump record, or a halt recotd
  while( (length != 0) && (length != 0xffffffff) )
  {
    // Get the next 4 bytes of the boot record, which should be an address 
    // record
    CopyFromFlash( (void*)(&address), (void*)(next_flash_byte), (size_t)(4) );
    next_flash_byte += 4;
    
    // Copy the next "length" bytes to "address"
    CopyFromFlash( (void*)(address), (void*)(next_flash_byte), (size_t)(length) );
    next_flash_byte += length;
    
    // Get the next 4 bytes of the boot record, which now should be another 
    // length record
    CopyFromFlash( (void*)(&length), (void*)(next_flash_byte), (size_t)(4) );
    next_flash_byte += 4;
  }
  
  // "length" was read as either 0x0 or 0xffffffff, which means we are done 
  // copying.
  if( length == 0xffffffff )
  {
    // We read a HALT record, so return a -1
    return -1;
  }
  else // length == 0x0
  {
    // We got a jump record, so read the next 4 bytes for the entry address
    CopyFromFlash( (void*)(&address), (void*)(next_flash_byte), (size_t)(4) );
    next_flash_byte += 4;
    
    // Return the entry point address
    return address;
  }
}


/*****************************************************************************
*  Function: PickFlashImage
*
*  Purpose:  This subroutine scans the two possible flash images and picks 
*  one to be loaded by the boot copier.  The selection criteria are pretty 
*  simple:
*    1 - if there's only one valid image, then we choose that one.
*    2 - if there are two valid images then
*          A - We choose the one with the latest revision
*          B - If both images have the same revision, the we choose the one
*              with the latest time stamp.
*          C - If both time stamp values are equal, the we choose image #2
*    3 - If neither image is valid, we return 0
*
*****************************************************************************/
int PickFlashImage(void)
{
  // Make sure our flash header buffers are word-aligned
  my_flash_header_type header_buffer_1 __attribute__((aligned(4)));
  my_flash_header_type header_buffer_2 __attribute__((aligned(4)));
  
  // Start by validating image 1
  if( ValidateFlashImage((alt_u32 *)(flash_image_ptr_1)) )
  {
    // Image 1 was not valid, so validate image 2
    if( ValidateFlashImage((alt_u32 *)(flash_image_ptr_2)) )
    {
      // Image 2 is not valid, return 0
      return 0;
    }
    else
    {
      // Image 2 is valid and image 1 is not, so choose 2
      return 2;
    }
  }
  else
  {
    // Image 1 is valid, check image 2
    if( ValidateFlashImage((alt_u32 *)(flash_image_ptr_2)) )
    {
      // Image 2 is not valid and image 1 is, so choose image 1
      return 1;
    }
    else
    {
      /*
       * Both images are valid, so we now must compare header information.
       * Just like the load_flash subroutine above, we don't assume that the
       * flash images are word aligned, so we memcpy the boot image headers 
       * from flash, to our word aligned temporary header buffers.  
       */

        CopyFromFlash(&header_buffer_1, flash_image_ptr_1, 32);
        CopyFromFlash(&header_buffer_2, flash_image_ptr_2, 32);
      
      // First compare the versions
      if( header_buffer_1.version == header_buffer_2.version )
      {
        // The versions are equal, so compare timestamps
        if( header_buffer_1.timestamp > header_buffer_2.timestamp )
        {
          // Image 1 has a later timestamp
          return 1;
        }
        else
        {
          // Image 2 has a later timestamp, or the timestamps were equal
          return 2;
        }
      }
      else
      {
        // The versions were not equal
        if( header_buffer_1.version > header_buffer_2.version )
        {
          // Image 1 has a later version
          return 1;
        }
        else
        {
          // Image 2 has a later version
          return 2;
        }
      }
    }
  }
}


/*****************************************************************************
*  Function: ValidateFlashImage
*
*  Purpose:  This routine validates a flash image based upon three critera:
*            1.) It contains the correct flash image signature
*            2.) A CRC check of the image header 
*            3.) A CRC check of the image data (payload)
*  
*  Since it's inefficient to read individual bytes from EPCS, and since
*  we dont really want to expend RAM to buffer the entire image, we comprimise
*  in the case of EPCS, and create a medium-size buffer, who's size is 
*  adjustable by the user.
*
*****************************************************************************/
int ValidateFlashImage(void *image_ptr)
{
  my_flash_header_type temp_header  __attribute__((aligned(4)));
  
  /*
   * Again, we don't assume the image is word aligned, so we copy the header
   * from flash to a word aligned buffer.
   */
    CopyFromFlash(&temp_header, image_ptr, 32);
  
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
  
/*****************************************************************************
*  Function: FlashCalcCRC32
*
*  Purpose:  This subroutine calcuates a reflected CRC32 on data located
*  flash.  The routine buffers flash contents locally in order
*  to support EPCS flash as well as CFI
*
*****************************************************************************/
alt_u32 FlashCalcCRC32(alt_u8 *flash_addr, int bytes)
{
  alt_u32 crcval = 0xffffffff;
  int i, buf_index, copy_length;
  alt_u8 cval;
  char flash_buffer[FLASH_BUFFER_LENGTH];
  
  while(bytes != 0)
  {
    copy_length = (FLASH_BUFFER_LENGTH < bytes) ? FLASH_BUFFER_LENGTH : bytes;
    CopyFromFlash(flash_buffer, flash_addr, copy_length);
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


#if USING_JTAG_UART
/*****************************************************************************
*  Function: MyJtagWrite
*
*  Purpose:  This subroutine is provided so that we can write characters out 
*  the JTAG UART without getting into a situation where we are permanently 
*  blocked, because there is no host reading from the JTAG UART.  So this 
*  routine takes the message that we want to write, and breaks it up into 
*  8 byte or less chunks to send to MyJtagWrite8().  MyJtagWrite8() 
*  makes sure that we can write to the jtag uart before attempting the write.
*
*****************************************************************************/
int MyJtagWrite(const char *buf, int len)
{
  int ret_val;
  int orig_len = len;
  
  if(len > 8)
  {
    // Our write message is greater than 8 bytes long, so break it up into
    // 8-byte or less chunks.
    do
    {
      ret_val = MyJtagWrite8( buf, (len > 8)?(8):(len));
      if(ret_val < 0)
      {
        // If an error returns then we're done
        break;
      }
      else
      {
        // No error, decrement our length, and increment the buffer
        len -= ret_val;
        buf += ret_val;
      }
    } while(len > 0);
    
    // Return the whole length of the buffer transmitted
    ret_val = orig_len;    
  }
  else
  {
    // If the write message is less than 8 bytes, just write it.
    ret_val = MyJtagWrite8( buf, len);
  }
  
  return ret_val;
}


/*****************************************************************************
*  Function: MyJtagWrite8
*
*  Purpose:  This subroutine is called by MyJtagWrite, which feeds it 8-byte 
*  or less chunks of data to write out the JTAG UART.  Basically, this routine 
*  queries the write fifo in the jtag uart to make sure there is room for our 
*  data first.  If there's not room, then it waits JTAG_UART_TIMEOUT seconds 
*  to see if a host will clear some of the pending data to allow us to write 
*  new data to the fifo.  If no host connects within JTAG_UART_TIMEOUT, then 
*  the jtag uart is marked abandoned, and the write data is discarded.  Once 
*  abandoned, every time a new write call is made to this routine, it checks 
*  to see if a host may have emptied the write fifo, if that ever happens, 
*  then the jtag uart is reclaimed and the original algorithm prevails.
*
*****************************************************************************/
int MyJtagWrite8(const char *buf, int len)
{
  alt_fd *the_fd;
  altera_avalon_jtag_uart_dev *the_dev;
  altera_avalon_jtag_uart_state *the_state;
  unsigned int the_base;
  alt_u32 control;
  alt_u32 wr_fifo_space;
  int ret_val;
  alt_alarm my_jtag_uart_alarm;
  volatile alt_u32 my_jtag_uart_context;
  
  /*
   * Look thru the device table to find our device block and extract the base
   * address to this STDOUT peripheral.
   * 
   * A big assumption here is that the STDOUT peripheral is a JTAG UART.
   */
  the_fd = &alt_fd_list[STDOUT_FILENO];
  the_dev = (altera_avalon_jtag_uart_dev *)the_fd->dev;
  the_state = (altera_avalon_jtag_uart_state *)&(the_dev->state);
  the_base = the_state->base;

  // Read the jtag uart control register and grab the write fifo space
  control = IORD_ALTERA_AVALON_JTAG_UART_CONTROL(the_base);
  wr_fifo_space = (control & ALTERA_AVALON_JTAG_UART_CONTROL_WSPACE_MSK)
                  >> ALTERA_AVALON_JTAG_UART_CONTROL_WSPACE_OFST;

  if(wr_fifo_space >= len)
  {
    // The write fifo has room for our write, so clear the abandoned flag
    // and write the message.
    ret_val = write(STDOUT_FILENO, buf, len);
  } 
  else
  {
    // The write fifo does not have room for us. have we previously abandoned
    // the jtag uart?
    if( jtag_uart_state & JTAG_ABANDONED_BIT )
    {
      // We have previously abandoned the jtag uart
      if(control & ALTERA_AVALON_JTAG_UART_CONTROL_AC_MSK)
      {
        // There has been activity from a host so let's clear the abandoned
        // flag, and see if the host will clear the fifo for us.
        jtag_uart_state &= ~JTAG_ABANDONED_BIT;
      }
    }
    
    // At this point we check to see if the jtag uart is abandoned
    if( jtag_uart_state & JTAG_ABANDONED_BIT )
    {
      // The jtag uart has been abandoned, so just dump the data and return -1
      ret_val = -1;
    }
    else
    {
      // The jtag uart has not been abandoned
      ret_val = -1;
      
      // Clear the activity bit in the jtag uart control register
      control |= ALTERA_AVALON_JTAG_UART_CONTROL_AC_MSK;
      IOWR_ALTERA_AVALON_JTAG_UART_CONTROL(the_base, control);
      
      // Set a timeout alarm
      my_jtag_uart_context = 0;
      alt_alarm_start ( 
                        &my_jtag_uart_alarm,          // alt_alarm* alarm,
                        JTAG_UART_TIMEOUT,            // alt_u32 nticks,
                        GenericTimeoutCallback,  // alt_u32 (*callback) (void* context),
                        (void *)&my_jtag_uart_context // void* context
                      );
      
      // Now wait until the timeout occurs and abandon the uart, or the host
      // clears the fifo for us.
      while( my_jtag_uart_context == 0 )
      {
        // Get the current control register value
        control = IORD_ALTERA_AVALON_JTAG_UART_CONTROL(the_base);
        
        if( control & ALTERA_AVALON_JTAG_UART_CONTROL_AC_MSK )
        {
          // We see activity, so stop the timeour alarm
          alt_alarm_stop ( &my_jtag_uart_alarm );
          my_jtag_uart_context = 0;
          
          // Extract the write fifo space
          wr_fifo_space = (control & ALTERA_AVALON_JTAG_UART_CONTROL_WSPACE_MSK)
                          >> ALTERA_AVALON_JTAG_UART_CONTROL_WSPACE_OFST;
          
          if(wr_fifo_space >= len)
          {
            // We now have room to perform our write, so do it and get outa here
            ret_val = write(STDOUT_FILENO, buf, len);
            break;
          } 
          // There's still not enough room so clear the activity bit
          control |= ALTERA_AVALON_JTAG_UART_CONTROL_AC_MSK;
          IOWR_ALTERA_AVALON_JTAG_UART_CONTROL(the_base, control);
          
          // And set the timeout alarm again
          alt_alarm_start ( 
                            &my_jtag_uart_alarm,          // alt_alarm* alarm,
                            JTAG_UART_TIMEOUT,            // alt_u32 nticks,
                            GenericTimeoutCallback,  // alt_u32 (*callback) (void* context),
                            (void *)&my_jtag_uart_context // void* context
                          );
          
        }
      }
      
      // If we get here, then we're done waiting, so clear the waiting flag
      jtag_uart_state &= ~JTAG_WAITING_BIT;
  
      if( my_jtag_uart_context == 1 )
      {
        // We got here because the timeout alarm fired, so set the abandoned flag
        jtag_uart_state |= JTAG_ABANDONED_BIT;
      }
    }
  }
  return ret_val;  
}


/*****************************************************************************
*  Function: GenericTimeoutCallback
*
*  Purpose:  This subroutine is a generic timeout callback routine for 
*  timeout alarms that get set.  This routine simply increments the alt_u32 
*  pointed to by the context pointer and returns 0, which requests no 
*  additional alarm time.
*
*****************************************************************************/
alt_u32 GenericTimeoutCallback (void* context)
{
  *((volatile alt_u32 *)(context)) += 0x1;
  return(0);
}

#endif  //USING_JTAG_UART

/******************************************************************************
 *                                                                             *
 * License Agreement                                                           *
 *                                                                             *
 * Copyright (c) 2006 Altera Corporation, San Jose, California, USA.           *
 * All rights reserved.                                                        *
 *                                                                             *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * and/or sell copies of the Software, and to permit persons to whom the       *
 * Software is furnished to do so, subject to the following conditions:        *
 *                                                                             *
 * The above copyright notice and this permission notice shall be included in  *
 * all copies or substantial portions of the Software.                         *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
 * DEALINGS IN THE SOFTWARE.                                                   *
 *                                                                             *
 * This agreement shall be governed in all respects by the laws of the State   *
 * of California and by the laws of the United States of America.              *
 * Altera does not recommend, suggest or require that this reference design    *
 * file be used in conjunction or combination with any other product.          *
 ******************************************************************************/
