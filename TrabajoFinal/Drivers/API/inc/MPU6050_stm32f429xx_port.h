/**
 ******************************************************************************
 * @file    TrabajoFinal/API/inc/MPU6050_stm32f429xx_port.h
 * @author  Marco Scasserra
 * @brief   Implementation of the modularization of the I2C routine for the
 * 			basic communication with the MPU 6050 gyroscope, accelerometer and
 * 			temperature sensor. This file contains the specific code for the
 * 			STM32F429ZI NUCLEO 144 board implementation of the API_MPU6050
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#ifndef API_INC_MPU6050_STM32F429XX_PORT_H_
#define API_INC_MPU6050_STM32F429XX_PORT_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "API_MPU6050.h"

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  I2C1ModuleInitPort, Initializes the I2C1 module of the board with
 * 		   the default parameters.
 * @param  none
 * @retval true if the initialization is successful, otherwise returns false
 */
bool_t I2C1ModuleInitPort();

/**
 * @brief  GPIOPinInitPort, Configures the GPIO module of the board so the I2C1
 * 		   module can work. Pin 6 and Pin 9 are the SCL and SDA respectively.
 * @param  none
 * @retval none
 */
void GPIOPinInitPort();

/**
 * @brief  MPUWritePort, transmits trough the I2C1 module 1 byte of data. This
 * 		   data will be written on the address given on the slave module.
 * @param  uint8_t REG_ADDR, Address where the I2C slave will write the given
 * 		   data
 * 		   uin8_t data, Data to be written
 * @retval none
 */
void MPUWritePort(uint8_t REG_ADDR, uint8_t data);

/**
 * @brief  MPUReadPort, receives trough the I2C1 module 1 byte of data. This
 * 		   data will come from the address given to the I2C slave and it will
 * 		   be written on the pointer parameter given.
 * @param  uint8_t REG_ADDR, Address that the I2C slave will transmit
 * 		   in8_t *data, pointer to where the data will be written.
 * @retval none
 */
void MPUReadPort(uint8_t REG_ADDR, int8_t *value);

#endif /* API_INC_MPU6050_STM32F429XX_PORT_H_ */
