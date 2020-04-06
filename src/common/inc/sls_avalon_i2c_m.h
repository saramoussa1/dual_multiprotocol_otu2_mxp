

#ifndef SLS_AVALON_I2C_M_H__
#define SLS_AVALON_I2C_M_H__

#include<stddef.h>

#include "sys/alt_llist.h"
#include "alt_types.h"
#include "sys/alt_dev.h"
#include "sys/alt_alarm.h"
#include "sys/alt_warning.h"

#include "i2c_dev.h"
#include "os/alt_sem.h"
#include "os/alt_flag.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef struct
{
  i2c_dev_list sls_dev_list;
  unsigned int base_addr;
}sls_avalon_i2c_dev;

int sls_avalon_i2c_write(void* next,alt_u8 addr,const char *ptr,alt_u8 offset,int len);
int sls_avalon_i2c_read(void* next,alt_u8 addr,alt_u8 *ptr,alt_u8 offset,int len,int type);
int sls_avalon_i2c_init(void* next,int speed, unsigned int sys_clock);
int sls_avalon_i2c_ioctl(void *next,int req, void *arg);
int sls_avalon_i2c_read_16_bit(void* next, alt_u8 addr, alt_u8 *buff, alt_u8 offset);

#define SLS_AVALON_I2C_M_INSTANCE(name, dev)               \
  static sls_avalon_i2c_dev dev =                        \
  {                                                      \
    {                                                    \
      ALT_LLIST_ENTRY,                                   \
      {                                                  \
        name##_NAME,                                     \
        sls_avalon_i2c_init,                             \
        sls_avalon_i2c_read,                             \
        sls_avalon_i2c_write,                            \
        sls_avalon_i2c_read_16_bit,						 \
      },                                                 \
    },                                                   \
      name##_BASE,                                       \
  }

#define SLS_AVALON_I2C_M_INIT(dev)                   \
  {                                                      \
                                                         \
      i2c_dev_reg(dev);                                  \
  }

#ifdef __cplusplus
}

#endif

#endif
