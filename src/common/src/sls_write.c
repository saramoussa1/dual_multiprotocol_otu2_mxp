
/************************************************************************************
* 																					*
* This program is same as the standard write system call and helps in write data to *
* the slave device                                                                  *
*																					*
************************************************************************************/

#include "sys/alt_llist.h"
#include "alt_types.h"
#include "../inc/i2c_dev.h"
#include "stddef.h"

//ALT_LLIST_HEAD(alt_sls_device_llist);

/*
 * The sls_read() system call is used to read a block of data from a file or device.
 * This function simply vectors the request to the device driver associated
 * with the input file descriptor "file".
 */


int sls_write(int handle,alt_u8 addr,const char* ptr,alt_u8 offset,int len)
{
	int index;

	i2c_dev_list* next = (i2c_dev_list*) alt_sls_device_llist.next;

	index=1;

	while(index!=handle && next != NULL)
	{

		next = (i2c_dev_list*)next->llist.next;
		index++;
	}

	if(next==NULL)
	{
		return 		(-1);
	}

	return 	(next->dev.write(next,addr,(alt_u8*)ptr,offset,len));

}
