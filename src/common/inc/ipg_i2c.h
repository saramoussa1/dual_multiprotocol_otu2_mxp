/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_i2c.h
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 7, 2015
 * @brief Contains prototypes of functions defined in ipg_i2c.c as well as type definitions.
 *
 */
#ifndef IPG_I2C_H_
#define IPG_I2C_H_

#include <system.h>
#include "io.h"
#include "alt_types.h"
#include "ipg_types.h"
#include "ipg_bits.h"
#include <stdarg.h>
#include <unistd.h>
#include "ipg_delay.h"
#include "sls_avalon_i2c_m.h"
#include "i2c_dev.h"

typedef void (*selectChannel)(uint32_t channel);	//!<funtion type to select a channel
/**Representation of an I2C bus.
 *
 */
typedef struct{
	int fd;								//!<file descriptor, used to communicate with any device on this bus
	char* name;							//!< name of the bus (SLS I2C master)
	alt_u8 speed;						//!< speed of the bus clock, 0 for slow (100KHz) and 1 for high (400KHz)
	selectChannel	select_channel;		//!< pointer to the callback function that implements the channel selection on this bus
}i2c_bus_t;

/**Representation of an I2C device.
 *
 */
typedef struct {
	i2c_bus_t *bus;						//!< pointer to the bus to which the device is connected
	alt_u8 address;						//!< I2C address of this device
	int channel;						//!< I2C channel to which the device on the bus is connected to (used when multiplexers and hubs are present on the bus)
}i2c_device_t;
/** * Creating an instance of the I2c Master that will be used in I2c read and write commands.
 * The NiOs is the master whereas the other devices are the slaves such as the SILAB
 */
#define INST_I2C(name, dev)					\
{											\
	SLS_AVALON_I2C_M_INSTANCE(name, dev);	\
	SLS_AVALON_I2C_M_INIT(dev);\
}

/*
 * exported functions
 */
int open_i2c_bus(i2c_bus_t* bus);
void select_i2c_channel_on_bus(i2c_bus_t* bus, int i2c_channel);
void set_i2c_bus_speed(i2c_bus_t* bus);
#endif /* IPG_I2C_H_ */
