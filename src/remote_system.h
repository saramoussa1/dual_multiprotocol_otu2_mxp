/****************************************************************************
 * * IPG Photonics Corporation
 * COPYRIGHT (C) 2015 IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *
 * IPG Photonics Corporation
 * 50 Old Webster Road
 * Oxford, MA 01540, USA
 ****************************************************************************/

/**
 * @file main.h
 * @brief Header file containing definitions needed in main.c.
 *
 *
 */

#ifndef REMOTE_SYSTEM_H
#define REMOTE_SYSTEM_H

/*
 * remote sls_avalon_i2c_m configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m sls_avalon_i2c_rem_m
#define SLS_AVALON_I2C_REM_M_BASE 0x10c00000
#define SLS_AVALON_I2C_REM_M_IRQ 2
#define SLS_AVALON_I2C_REM_M_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_REM_M_NAME "/dev/sls_avalon_i2c_rem_m"
#define SLS_AVALON_I2C_REM_M_SPAN 64
#define SLS_AVALON_I2C_REM_M_TYPE "sls_avalon_i2c_m"


/*
 * remote sls_avalon_i2c_m_li1 configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_li1 sls_avalon_i2c_rem_m
#define SLS_AVALON_I2C_REM_M_LI1_BASE 0x10f00000
#define SLS_AVALON_I2C_REM_M_LI1_IRQ 5
#define SLS_AVALON_I2C_REM_M_LI1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_REM_M_LI1_NAME "/dev/sls_avalon_i2c_rem_m_li1"
#define SLS_AVALON_I2C_REM_M_LI1_SPAN 64
#define SLS_AVALON_I2C_REM_M_LI1_TYPE "sls_avalon_i2c_m"


/*
 * remote sls_avalon_i2c_m_li2 configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_li2 sls_avalon_i2c_rem_m
#define SLS_AVALON_I2C_REM_M_LI2_BASE 0x10f10000
#define SLS_AVALON_I2C_REM_M_LI2_IRQ 6
#define SLS_AVALON_I2C_REM_M_LI2_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_REM_M_LI2_NAME "/dev/sls_avalon_i2c_rem_m_li2"
#define SLS_AVALON_I2C_REM_M_LI2_SPAN 64
#define SLS_AVALON_I2C_REM_M_LI2_TYPE "sls_avalon_i2c_m"


/*
 * remote sls_avalon_i2c_m_x configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_x sls_avalon_i2c_rem_m
#define SLS_AVALON_I2C_REM_M_X_BASE 0x10d00000
#define SLS_AVALON_I2C_REM_M_X_IRQ 1
#define SLS_AVALON_I2C_REM_M_X_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_REM_M_X_NAME "/dev/sls_avalon_i2c_rem_m_x"
#define SLS_AVALON_I2C_REM_M_X_SPAN 64
#define SLS_AVALON_I2C_REM_M_X_TYPE "sls_avalon_i2c_m"


/*
 * remote sls_avalon_i2c_m_y configuration
 *
 */

#define ALT_MODULE_CLASS_sls_avalon_i2c_m_y sls_avalon_i2c_rem_m
#define SLS_AVALON_I2C_REM_M_Y_BASE 0x10e00000
#define SLS_AVALON_I2C_REM_M_Y_IRQ 3
#define SLS_AVALON_I2C_REM_M_Y_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SLS_AVALON_I2C_REM_M_Y_NAME "/dev/sls_avalon_i2c_m_y"
#define SLS_AVALON_I2C_REM_M_Y_SPAN 64
#define SLS_AVALON_I2C_REM_M_Y_TYPE "sls_avalon_i2c_m"

#endif
