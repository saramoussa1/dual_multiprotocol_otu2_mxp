#ifndef __I2C_DEV_H__
#define __I2C_DEV_H__


/********************************************************************************************
*                                                                                           *
*                  THIS IS A LIBRARY READ-ONLY SOURCE FILE. DO NOT EDIT.                    *
*                                                                                           *
********************************************************************************************/


#include "sys/alt_llist.h"
#include "alt_types.h"

extern alt_llist alt_sls_device_llist;


#define i2c_dev_reg(dev) alt_llist_insert(&alt_sls_device_llist,&dev.sls_dev_list.llist)

int sls_devices_init();
int sls_open(char*, int, unsigned int);
int sls_read(int handle,alt_u8 addr,alt_u8* ptr,alt_u8 offset,int len,int type);
int sls_read_16_bit(int handle,alt_u8 addr,alt_u8* ptr,alt_u8 offset);
int sls_write(int handle,alt_u8 addr,const char* ptr,alt_u8 offset,int len);

/*
 * The device structure definition.
 */

typedef struct {
	const char*  name;
    int (*init)  ();
    int (*read)  (void* next,alt_u8 addr,alt_u8* ptr,alt_u8 offset,int len,int type);
    int (*write) (void* next,alt_u8 addr,const alt_u8* ptr,alt_u8 offset,int len);
    int (*read_16_bit)(void* next, alt_u8 addr,alt_u8* buff,alt_u8 offset);
}i2c_dev;

typedef struct{
	alt_llist llist;
    i2c_dev   dev;
}i2c_dev_list;
#endif
