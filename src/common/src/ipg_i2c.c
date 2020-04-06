/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file ipg_i2c.c
 * @author Danny Chamoun (dchamoun@ipgphotonics.com)
 * @date Oct 7, 2015
 * @version 0.0.2
 * @brief Contains procedures to access I2C devices on I2C bus.
 *
 */
#include "../inc/ipg_i2c.h"
#include "sys/ioctl.h"

/**This function opens a connection to an I2C bus.
 *
 * @param[in,out] bus	pointer to the I2C bus
 * @return the file descriptor to the bus
 */
int open_i2c_bus(i2c_bus_t* bus){
	int i2c_fd = 0;
	(bus->speed == 1)?(bus->speed = 1):(bus->speed = 0);/*handle the case where bus.speed is not initialized*/
	i2c_fd = sls_open(bus->name, bus->speed, ALT_CPU_FREQ);
	if(i2c_fd > 0){
		bus->fd = i2c_fd;
	}
	printf("I2C file handle is %d\n", i2c_fd);
	return 	(bus->fd);
}

void set_i2c_bus_speed(i2c_bus_t* bus){
	ioctl(bus->fd, bus->speed, NULL);
}
/**This function selects a channel on an I2C bus.
 * An I2C bus is considered as a virtual I2C multiplexer where every I2C device
 * is connected to a channel.
 * @param[in,out] 	bus		pointer to the I2C bus
 * @param[in] 		channel	channel to select
 */
void select_i2c_channel_on_bus(i2c_bus_t* bus, int channel){
	if(bus != NULL){
		if(bus->select_channel != NULL){
			bus->select_channel(channel);
		}
	}
	Delay_ms(1);
}


