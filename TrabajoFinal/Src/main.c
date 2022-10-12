/**
 ******************************************************************************
 * @file    TrabajoFinal/Src/main.c
 * @author  Marco Scasserra
 * @brief   this program configures the MPU 6050 and takes a measure of a given
 * 			sensor every X time (the times between measures can be changed with
 * 			the given defines at the top of this file) and transmit the data
 * 			trough UART to the computer. The user can change between the
 * 			sensors by pressing the builtin button on the STM32F429ZI NUCLEO144
 * 			board.
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

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdbool.h>
#include "main.h"
#include "API_delay.h"
#include "API_debounce.h"
#include "API_uart.h"
#include "API_MPU6050.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	START_STATE,
	GYRO_STATE,
	ACCEL_STATE,
	TEMP_STATE
} programState_t;

/* Private define ------------------------------------------------------------*/
#define TIMEBETWEENGYRO 100
#define TIMEBETWEENACCEL 100
#define TIMEBETWEENTEMP 100

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);
static void measureFSM_Init();
static void measureFSM_Update();
static void sendMenu();

/* Global variables ----------------------------------------------------------*/

/* UART structure for the communication*/
UART_HandleTypeDef UartHandle;

// programState is the variable that tracks the current state of the main FSM
static programState_t programState = START_STATE;

// Global variables where the last measure of the sensors will be stored
gyroValues_t gyroMeasure;
accelValues_t accelMeasure;
float tempMeasure;

// Delays for time between measures
static delay_t gyroDelay;
static delay_t accelDelay;
static delay_t tempDelay;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	/* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the system clock to 180 MHz */
	SystemClock_Config();

	/* Private variables -----------------------------------------------------*/
	/* Initialize BSP Led for LED1, LED2 and LED3*/
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);

	/* Initialize BSP Button for the BuiltIn Button*/
	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Initialize the UART transmission*/
	if(!uartInit(&UartHandle)){
		Error_Handler();
	}
	/* Initialize the debounce FSM */
	debounceFSM_Init();

	/* Initialize the measuring FSM */
	measureFSM_Init();

	/* Initialize the delays for the reading of the sensors.*/
	delayInit(&gyroDelay, TIMEBETWEENGYRO);
	delayInit(&accelDelay, TIMEBETWEENACCEL);
	delayInit(&tempDelay, TIMEBETWEENTEMP);

	/* Infinite loop */
	while (1)
	{
		debounceFSM_Update();
		measureFSM_Update();
	}
}


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	if(HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}

/**
 * @brief  measureFSM_Init(), Initializes the FSM for the measuring and transmitting of the
 * 		   MPU6050 sensor. First it initializes the sensor and if that returns true sends
 * 		   a message trough the UART and sends a menu, if it senses an error on that routine
 * 		   it will send a message trought the UART and then it will run the Error_Handler
 * 		   function
 * @param  None
 * @retval None
 */
static void measureFSM_Init(){
	bool_t status;
	char str[100];
	status = MPU6050Init();
	if(status == true){
		strcpy(str, "Inicializacion correcta del modulo I2C y conectado al MPU6050.\n\r");
		uartSendString(&UartHandle, str);
		sendMenu();
	}else{
		strcpy(str, "Error al inicializar modulo I2C.\n\r");
		uartSendString(&UartHandle, str);
		Error_Handler();
	}
}

/**
 * @brief  measureFSM_Update(), updates the current state of the measuring program.
 * 		   The input to change the current state is the builtin button on the NUCLEO144
 * 		   board. It will loop trough the three different sensors
 * 		   TODO: Implement the reading of the UART to change states
 * @param  None
 * @retval None
 */
static void measureFSM_Update(){
	char str[100];
	switch(programState){
	case START_STATE:
		if(readKeyFE()){
			programState = GYRO_STATE;
			BSP_LED_On(LED1);
			BSP_LED_Off(LED2);
			BSP_LED_Off(LED3);
		}
		break;

	case GYRO_STATE:
		if(delayRead(&gyroDelay)){
			readGyro(&gyroMeasure);
			sprintf(str, "Gyro  X: %.3f d/s	Y: %.3f d/s	Z: %.3f d/s\n\r", gyroMeasure.gyroX, gyroMeasure.gyroY, gyroMeasure.gyroZ);
			uartCLRScreen(&UartHandle);
			uartSendString(&UartHandle, str);
		}
		if(readKeyFE()){
			programState = ACCEL_STATE;
			BSP_LED_Off(LED1);
			BSP_LED_On(LED2);
			BSP_LED_Off(LED3);
		}
		break;

	case ACCEL_STATE:
		if(delayRead(&accelDelay)){
			readAccel(&accelMeasure);
			sprintf(str, "Accel X: %.3f g/s	Y: %.3f g/s	Z: %.3f g/s\n\r", accelMeasure.accelX, accelMeasure.accelY, accelMeasure.accelZ);
			uartCLRScreen(&UartHandle);
			uartSendString(&UartHandle, str);
		}
		if(readKeyFE()){
			programState = TEMP_STATE;
			BSP_LED_Off(LED1);
			BSP_LED_Off(LED2);
			BSP_LED_On(LED3);
		}
		break;

	case TEMP_STATE:
		if(delayRead(&tempDelay)){
			readTemp(&tempMeasure);
			sprintf(str, "Temp: %.3f C\n\r", tempMeasure);
			uartCLRScreen(&UartHandle);
			uartSendString(&UartHandle, str);
		}
		if(readKeyFE()){
			programState = GYRO_STATE;
			BSP_LED_On(LED1);
			BSP_LED_Off(LED2);
			BSP_LED_Off(LED3);
		}
		break;
	default:
		Error_Handler();
		break;
	}
}

/**
 * @brief  sendMenu, sends trough the UART a menu for the user to see
 * @param  None
 * @retval None
 */
static void sendMenu(){
	char str[100];
	strcpy(str, "1 - Mediciones gyroscopio.\n\r");
	uartSendString(&UartHandle, str);
	strcpy(str, "2 - Mediciones acelerometro.\n\r");
	uartSendString(&UartHandle, str);
	strcpy(str, "3 - Mediciones temperatura.\n\r");
	uartSendString(&UartHandle, str);
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
