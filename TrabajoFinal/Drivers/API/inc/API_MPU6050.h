/**
 ******************************************************************************
 * @file    TrabajoFinal/API/inc/API_MPU6050.h
 * @author  Marco Scasserra
 * @brief   Implementation of the modularization of the I2C routine for the
 * 			basic communication with the MPU 6050 gyroscope, accelerometer and
 * 			temperature sensor. This program takes the raw data from the
 * 			sensors and converts it to a given unit.
 * 			This file contains the public defines and the public functions.
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

#ifndef API_INC_API_MPU6050_H_
#define API_INC_API_MPU6050_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

/* Exported defines ----------------------------------------------------------*/

/* Max time to wait for the I2C communication*/
#define MAX_TIMEOUT_MS 	1000

/* Address for the MPU6050 Module. If the AD0 pin is not connected the in built
 * pulldown resistor makes it so the address is 0x68, if its connected to VCC
 * the address will be set to 0x69*/
#define MPU6050_ADDR 	0x68

/* Addr for the internal registers that the driver will read from or write to*/
#define SMPRT_DIV		0x19
#define GYRO_CONFIG		0X1B
#define ACCEL_CONFIG	0X1C

#define ACCEL_XOUTH 	0x3B
#define ACCEL_XOUTL 	0x3C
#define ACCEL_YOUTH 	0x3D
#define ACCEL_YOUTL 	0x3E
#define ACCEL_ZOUTH 	0x3F
#define ACCEL_ZOUTL 	0x40
#define TEMP_OUTH 		0x41
#define TEMP_OUTL 		0x42
#define GYRO_XOUTH 		0x43
#define GYRO_XOUTL 		0x44
#define GYRO_YOUTH 		0x45
#define GYRO_YOUTL 		0x46
#define GYRO_ZOUTH 		0x47
#define GYRO_ZOUTL 		0x48
#define PWR_MGMT_1		0x6B
#define WHO_AM_I		0x75

/* Configs for the internal register of the MPU 6050*/
#define GYRO_RANGE		0 //Range for the gyroscope, it must be an int from 0 to 3
#define ACCEL_RANGE		0 //Range for the accelerometer, it must be an int from o to 3
#define PWR_CONFIG		0 //Config for the power settings, it selects the internal 8MHz oscillator
						  //enables the temperature sensor and disables the sleep mode
#define SMPRT_CONFIG	7 //Sample rate config for the sensors
#define GYRO_SMPRT		0 //Sample rate config for the gyroscope
#define ACCEL_SMPRT		0 //Sample rate config for the accelerometer

/* Exported structures -------------------------------------------------------*/

/* Struct for storing the gyroscope values converted to °/s*/
typedef struct {
	float gyroX;
	float gyroY;
	float gyroZ;
} gyroValues_t;

/* Struct for storing the accelerometer values converted to g/s*/
typedef struct {
	float accelX;
	float accelY;
	float accelZ;
} accelValues_t;

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  MPU6050Init, Initializes the I2C module and configures the pins from
 * 		   the board. If the initialization is successful then it will transmit
 * 		   with the MPU6050 and set the necessary registers with the values from
 * 		   the defines so it will work.
 * @param  None
 * @retval True if all the initializations are successful, otherwise returns false
 */
bool_t MPU6050Init();

/**
 * @brief  readGyro, will transmit the instructions for the MPU to send one
 * 		   measure from all the gyroscope sensors
 * @param  gyroMeasure, pointer to the gyroValues_t structure that holds one
 * 		   read from the gyroscope sensor at the time.
 * @retval None
 */
void readGyro(gyroValues_t *gyroMeasure);

/**
 * @brief  readAccel, will transmit the instructions for the MPU to send one
 * 		   measure from all the accelerometer sensors
 * @param  accelMeasure, pointer to the accelValues_t structure that holds one
 * 		   read from the accelerometer sensor at the time.
 * @retval None
 */
void readAccel(accelValues_t *accelMeasure);

/**
 * @brief  readTemp, will transmit the instructions for the MPU to send one
 * 		   measure from all the temperature sensors
 * @param  tempMeasure, pointer to the int16_t variable that holds one
 * 		   read from the temperature sensor at the time.
 * @retval None
 */
void readTemp(float *tempMeasure);

#endif /* API_INC_API_MPU6050_H_ */
