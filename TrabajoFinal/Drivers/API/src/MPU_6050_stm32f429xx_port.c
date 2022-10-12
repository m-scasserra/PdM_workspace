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
#include "MPU6050_stm32f429xx_port.h"

/* Private defines------------------------------------------------------------*/
/* Address of the MPU 6050 module. We need to shift it by 1 so the write or
 * read bit can be sent.*/
#define I2C_ADDR (MPU6050_ADDR << 1)


/* Private variables ---------------------------------------------------------*/

/* I2C handler for the I2C module*/
static I2C_HandleTypeDef I2C1Handler;


/* Exported functions --------------------------------------------------------*/
/**
 * @brief  I2C1ModuleInitPort, Initializes the I2C1 module of the board with
 * 		   the default parameters.
 * @param  none
 * @retval true if the initialization is successful, otherwise returns false
 */
bool_t I2C1ModuleInitPort(){
	I2C1Handler.Instance = I2C1;
	I2C1Handler.Init.ClockSpeed = 100000;
	I2C1Handler.Init.DutyCycle = I2C_DUTYCYCLE_2;
	I2C1Handler.Init.OwnAddress1 = 0;
	I2C1Handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	I2C1Handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	I2C1Handler.Init.OwnAddress2 = 0;
	I2C1Handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2C1Handler.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	__I2C1_CLK_ENABLE();
	if (HAL_I2C_Init(&I2C1Handler) != HAL_OK)
	{
		return false;
	}

	/* Configure Analogue filter*/
	if (HAL_I2CEx_ConfigAnalogFilter(&I2C1Handler, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return false;
	}

	/* Configure Digital filter*/
	if (HAL_I2CEx_ConfigDigitalFilter(&I2C1Handler, 0) != HAL_OK)
	{
		return false;
	}
	return true;
}

/**
 * @brief  GPIOPinInitPort, Configures the GPIO module of the board so the I2C1
 * 		   module can work. Pin 6 and Pin 9 are the SCL and SDA respectively.
 * @param  none
 * @retval none
 */
void GPIOPinInitPort(){

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

	__GPIOB_CLK_ENABLE();
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief  MPUWritePort, transmits trough the I2C1 module 1 byte of data. This
 * 		   data will be written on the address given on the slave module.
 * @param  uint8_t REG_ADDR, Address where the I2C slave will write the given
 * 		   data
 * 		   uin8_t data, Data to be written
 * @retval none
 */
void MPUWritePort(uint8_t REG_ADDR, uint8_t data){
	HAL_I2C_Mem_Write(&I2C1Handler, I2C_ADDR, REG_ADDR, 1, &data, 1, MAX_TIMEOUT_MS);
}

/**
 * @brief  MPUReadPort, receives trough the I2C1 module 1 byte of data. This
 * 		   data will come from the address given to the I2C slave and it will
 * 		   be written on the pointer parameter given.
 * @param  uint8_t REG_ADDR, Address that the I2C slave will transmit
 * 		   in8_t *data, pointer to where the data will be written.
 * @retval none
 */
void MPUReadPort(uint8_t REG_ADDR, int8_t *value){
	HAL_I2C_Mem_Read(&I2C1Handler, I2C_ADDR, REG_ADDR, 1, value, 1, MAX_TIMEOUT_MS);
}
