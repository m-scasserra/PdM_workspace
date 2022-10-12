/**
 ******************************************************************************
 * @file    TrabajoFinal/API/src/API_MPU6050.c
 * @author  Marco Scasserra
 * @brief   Implementation of the modularization of the I2C routine for the
 * 			basic communication with the MPU 6050 gyroscope, accelerometer and
 * 			temperature sensor. This program takes the raw data from the
 * 			sensors and converts it to a given unit.
 * 			This file contains the implementation of the public and private
 * 			functions.
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
#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "API_MPU6050.h"
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "MPU6050_stm32f429xx_port.h"

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  readGyroX, will transmit the instructions for the MPU to send one
 * 		   measure from the gyroscope X axis sensor. The data is stored in two
 * 		   separate registers so we need to bitbang the full read from two
 * 		   separate values. To convert the data we need to divide it by the
 * 		   sensitivity.
 * @param  none
 * @retval float value of the gyroscope X axis sensor converted to °/s
 */
static float readGyroX();

/**
 * @brief  readGyroY, will transmit the instructions for the MPU to send one
 * 		   measure from the gyroscope Y axis sensor. The data is stored in two
 * 		   separate registers so we need to bitbang the full read from two
 * 		   separate values. To convert the data we need to divide it by the
 * 		   sensitivity.
 * @param  none
 * @retval float value of the gyroscope Y axis sensor converted to °/s
 */
static float readGyroY();

/**
 * @brief  readGyroZ, will transmit the instructions for the MPU to send one
 * 		   measure from the gyroscope Z axis sensor. The data is stored in two
 * 		   separate registers so we need to bitbang the full read from two
 * 		   separate values. To convert the data we need to divide it by the
 * 		   sensitivity.
 * @param  none
 * @retval float value of the gyroscope Z axis sensor converted to °/s
 */
static float readGyroZ();

/**
 * @brief  readAccelX, will transmit the instructions for the MPU to send one
 * 		   measure from the accelerometer X axis sensor. The data is stored
 * 		   in two separate registers so we need to bitbang the full read from
 * 		   two separate values. To convert the data we need to divide it by
 * 		   the sensitivity.
 * @param  none
 * @retval float value of the accelerometer X axis sensor converted to g/s
 */
static float readAccelX();

/**
 * @brief  readAccelY, will transmit the instructions for the MPU to send one
 * 		   measure from the accelerometer Y axis sensor. The data is stored
 * 		   in two separate registers so we need to bitbang the full read from
 * 		   two separate values. To convert the data we need to divide it by
 * 		   the sensitivity.
 * @param  none
 * @retval float value of the accelerometer Y axis sensor converted to g/s
 */
static float readAccelY();

/**
 * @brief  readAccelZ, will transmit the instructions for the MPU to send one
 * 		   measure from the accelerometer Z axis sensor. The data is stored
 * 		   in two separate registers so we need to bitbang the full read from
 * 		   two separate values. To convert the data we need to divide it by
 * 		   the sensitivity.
 * @param  none
 * @retval float value of the accelerometer Z axis sensor converted to g/s
 */
static float readAccelZ();

/* Vector that stores the sensitivity for the accelerometer conversions*/
const float ACCEL_SENSITIVITY[4] = {
		16384.0,
		8192.0,
		4096.0,
		2048.0};

/* Vector that stores the sensitivity for the gyroscope conversions*/
const float GYRO_SENSITIVITY[4] = {
		131.0,
		65.5,
		32.8,
		16.4};

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  MPU6050Init, Initializes the I2C module and configures the pins from
 * 		   the board. If the initialization is successful then it will transmit
 * 		   with the MPU6050 and set the necessary registers with the values from
 * 		   the defines so it will work.
 * @param  None
 * @retval True if all the initializations are successful, otherwise returns false
 */
bool_t MPU6050Init(){
	int8_t who_value = 0;
	bool_t returnValue = false;
	returnValue = I2C1ModuleInitPort();
	GPIOPinInitPort();
	if(returnValue == true){
		MPUWritePort(PWR_MGMT_1, PWR_CONFIG);
		MPUWritePort(SMPRT_DIV, SMPRT_CONFIG);
		MPUWritePort(GYRO_CONFIG, GYRO_SMPRT << 3);
		MPUWritePort(ACCEL_CONFIG, ACCEL_SMPRT << 3);
		MPUReadPort(WHO_AM_I, &who_value);
		if(who_value != MPU6050_ADDR){
			returnValue = false;
		}
	}
	return returnValue;
}

/**
 * @brief  readGyro, will transmit the instructions for the MPU to send one
 * 		   measure from all the gyroscope sensors
 * @param  gyroMeasure, pointer to the gyroValues_t structure that holds one
 * 		   read from the gyroscope sensor at the time.
 * @retval None
 */
void readGyro(gyroValues_t *gyroMeasure){
	gyroMeasure->gyroX = readGyroX();
	gyroMeasure->gyroY = readGyroY();
	gyroMeasure->gyroZ = readGyroZ();
}

/**
 * @brief  readAccel, will transmit the instructions for the MPU to send one
 * 		   measure from all the accelerometer sensors
 * @param  accelMeasure, pointer to the accelValues_t structure that holds one
 * 		   read from the accelerometer sensor at the time.
 * @retval None
 */
void readAccel(accelValues_t *accelMeasure){
	accelMeasure->accelX = readAccelX();
	accelMeasure->accelY = readAccelY();
	accelMeasure->accelZ = readAccelZ();
}

/**
 * @brief  readTemp, will transmit the instructions for the MPU to send one
 * 		   measure from all the temperature sensors.
 * @param  tempMeasure, pointer to the int16_t variable that holds one
 * 		   read from the temperature sensor at the time.
 * @retval None
 */
void readTemp(float *tempMeasure){
	int8_t dataH_RAW = 0, dataL_RAW = 0;
	int16_t returnValue_RAW = 0;
	MPUReadPort(TEMP_OUTH, &dataH_RAW);
	MPUReadPort(TEMP_OUTL, &dataL_RAW);
	returnValue_RAW = (int16_t)(dataH_RAW << 8) | dataL_RAW;

	*tempMeasure = (returnValue_RAW / 340.0) + 36.53;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  readGyroX, will transmit the instructions for the MPU to send one
 * 		   measure from the gyroscope X axis sensor. The data is stored in two
 * 		   separate registers so we need to bitbang the full read from two
 * 		   separate values. To convert the data we need to divide it by the
 * 		   sensitivity.
 * @param  none
 * @retval float value of the gyroscope X axis sensor converted to °/s
 */
static float readGyroX(){
	int8_t dataH_RAW = 0, dataL_RAW = 0;
	int16_t returnValue_RAW = 0;
	float returnValue = 0;
	MPUReadPort(GYRO_XOUTH, &dataH_RAW);
	MPUReadPort(GYRO_XOUTL, &dataL_RAW);
	returnValue_RAW = (int16_t)(dataH_RAW << 8 ) | dataL_RAW;
	returnValue = returnValue_RAW / GYRO_SENSITIVITY[GYRO_RANGE];
	return returnValue;
}

/**
 * @brief  readGyroY, will transmit the instructions for the MPU to send one
 * 		   measure from the gyroscope Y axis sensor. The data is stored in two
 * 		   separate registers so we need to bitbang the full read from two
 * 		   separate values. To convert the data we need to divide it by the
 * 		   sensitivity.
 * @param  none
 * @retval float value of the gyroscope Y axis sensor converted to °/s
 */
static float readGyroY(){
	int8_t dataH_RAW = 0, dataL_RAW = 0;
	int16_t returnValue_RAW = 0;
	float returnValue = 0;
	MPUReadPort(GYRO_YOUTH, &dataH_RAW);
	MPUReadPort(GYRO_YOUTL, &dataL_RAW);
	returnValue_RAW = (int16_t)(dataH_RAW << 8 ) | dataL_RAW;
	returnValue = returnValue_RAW / GYRO_SENSITIVITY[GYRO_RANGE];
	return returnValue;
}

/**
 * @brief  readGyroZ, will transmit the instructions for the MPU to send one
 * 		   measure from the gyroscope ZX axis sensor. The data is stored in two
 * 		   separate registers so we need to bitbang the full read from two
 * 		   separate values. To convert the data we need to divide it by the
 * 		   sensitivity.
 * @param  none
 * @retval float value of the gyroscope Z axis sensor converted to °/s
 */
static float readGyroZ(){
	int8_t dataH_RAW = 0, dataL_RAW = 0;
	int16_t returnValue_RAW = 0;
	float returnValue = 0;
	MPUReadPort(GYRO_ZOUTH, &dataH_RAW);
	MPUReadPort(GYRO_ZOUTL, &dataL_RAW);
	returnValue_RAW = (int16_t)(dataH_RAW << 8 ) | dataL_RAW;
	returnValue = returnValue_RAW / GYRO_SENSITIVITY[GYRO_RANGE];
	return returnValue;
}

/**
 * @brief  readAccelX, will transmit the instructions for the MPU to send one
 * 		   measure from the accelerometer X axis sensor. The data is stored
 * 		   in two separate registers so we need to bitbang the full read from
 * 		   two separate values. To convert the data we need to divide it by
 * 		   the sensitivity.
 * @param  none
 * @retval float value of the accelerometer X axis sensor converted to g/s
 */
static float readAccelX(){
	int8_t dataH_RAW = 0, dataL_RAW = 0;
	int16_t returnValue_RAW = 0;
	float returnValue = 0;
	MPUReadPort(ACCEL_XOUTH, &dataH_RAW);
	MPUReadPort(ACCEL_XOUTL, &dataL_RAW);
	returnValue_RAW = (int16_t)(dataH_RAW << 8 ) | dataL_RAW;
	returnValue = returnValue_RAW / ACCEL_SENSITIVITY[ACCEL_RANGE];
	return returnValue;
}

/**
 * @brief  readAccelY, will transmit the instructions for the MPU to send one
 * 		   measure from the accelerometer Y axis sensor. The data is stored
 * 		   in two separate registers so we need to bitbang the full read from
 * 		   two separate values. To convert the data we need to divide it by
 * 		   the sensitivity.
 * @param  none
 * @retval float value of the accelerometer Y axis sensor converted to g/s
 */
static float readAccelY(){
	int8_t dataH_RAW = 0, dataL_RAW = 0;
	int16_t returnValue_RAW = 0;
	float returnValue = 0;
	MPUReadPort(ACCEL_YOUTH, &dataH_RAW);
	MPUReadPort(ACCEL_YOUTL, &dataL_RAW);
	returnValue_RAW = (int16_t)(dataH_RAW << 8 ) | dataL_RAW;
	returnValue = returnValue_RAW / ACCEL_SENSITIVITY[ACCEL_RANGE];
	return returnValue;
}

/**
 * @brief  readAccelZ, will transmit the instructions for the MPU to send one
 * 		   measure from the accelerometer Z axis sensor. The data is stored
 * 		   in two separate registers so we need to bitbang the full read from
 * 		   two separate values. To convert the data we need to divide it by
 * 		   the sensitivity.
 * @param  none
 * @retval float value of the accelerometer Z axis sensor converted to g/s
 */
static float readAccelZ(){
	int8_t dataH_RAW = 0, dataL_RAW = 0;
	int16_t returnValue_RAW = 0;
	float returnValue = 0;
	MPUReadPort(ACCEL_ZOUTH, &dataH_RAW);
	MPUReadPort(ACCEL_ZOUTL, &dataL_RAW);
	returnValue_RAW = (int16_t)(dataH_RAW << 8 ) | dataL_RAW;
	returnValue = returnValue_RAW / ACCEL_SENSITIVITY[ACCEL_RANGE];
	return returnValue;
}
#endif /* API_INC_API_UART_H_ */
