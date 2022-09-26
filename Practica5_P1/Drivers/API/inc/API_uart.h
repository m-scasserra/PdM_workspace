/*
 * API_uart.h
 *
 *  Created on: Sep 21, 2022
 *      Author: Marco Scasserra
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

#define BAUD_RATE 9600
#define TIMEOUT_DELAY 100

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
bool_t uartInit(UART_HandleTypeDef *UartHandle);

/**
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
void uartSendString(UART_HandleTypeDef *UartHandle, char *txString);

/**
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
void uartSendStringSize(UART_HandleTypeDef *UartHandle, char *txString, uint16_t size);

/**
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
void uartReceiveStringSize(UART_HandleTypeDef *UartHandle, char *rxString, uint16_t size);


#endif /* API_INC_API_UART_H_ */
