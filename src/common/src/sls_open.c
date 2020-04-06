
/*******************************************************************************
*                                                                              *
*  This program is same as standard open function and used to open the device  *
*  registered with the same name as the name given and to select the speed of  *
*  the device accordingly.													   *
*                                                                              *
*******************************************************************************/

#include "alt_types.h"
#include "sys/alt_llist.h"
#include "../inc/i2c_dev.h"
#include "string.h"
#include "stddef.h"
#include "../inc/sls_avalon_i2c_m.h"


ALT_LLIST_HEAD(alt_sls_device_llist);


/*
 * sls_open() is called in order to get a file descriptor that reference the file
 * or device named "name". This descriptor can then be used to manipulate the
 * file/device using the standard system calls, e.g. sls_write(), sls_read(),
 * etc.
 *
 * This is equivalent to the standard open() system call
 */

int sls_open(char *name,int speed,unsigned int sys_clock){
	int index;
	alt_32 len;
    i2c_dev_list *next  = (i2c_dev_list *)alt_sls_device_llist.next;
	len  = strlen(name) + 1;
	index=1;

	/*
	 * Check each list entry in turn, until a match is found, or we reach the
	 * end of the list (i.e. next winds up pointing back to the list head).
	 */

	while (next !=NULL){

		/*
	     * memcmp() is used here rather than strcmp() in order to reduce the size
	     * of the executable.
	     */

	     if (!memcmp (next->dev.name, name, len))
	     {
			 /* match found */

             next->dev.init(next,speed,sys_clock);
			 return 			(index);
		 }


		 next = (i2c_dev_list*)next->llist.next;
		 index++;
         if(index == 32)
           return 		(-1);

	 }


	 /* No match found */

	 return 	(-1);
}
