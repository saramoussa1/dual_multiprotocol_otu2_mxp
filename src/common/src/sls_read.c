
/************************************************************************************
* 																					*
* This program is same as the standard read system call and helps in get the data   *
* from the slave device                                                             *
*																					*
************************************************************************************/

#include "sys/alt_llist.h"
#include "alt_types.h"
#include "../inc/i2c_dev.h"
#include "stddef.h"


/*
 * The sls_read() system call is used to read a block of data from a file or device.
 * This function simply vectors the request to the device driver associated
 * with the input file descriptor "file".
 */


int sls_read(int handle,alt_u8 addr,alt_u8* ptr,alt_u8 offset,int len,int type)
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

	return 	(next->dev.read(next,addr,ptr,offset,len,type));

}

/*
 * The sls_read_16_bit() system call is used to read 16 bit value from a file or device.
 * This function simply vectors the request to the device driver associated
 * with the input file descriptor "file".
 */
int sls_read_16_bit(int handle,alt_u8 addr,alt_u8* ptr,alt_u8 offset)
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

	return 	(next->dev.read_16_bit(next,addr,ptr,offset));
}

