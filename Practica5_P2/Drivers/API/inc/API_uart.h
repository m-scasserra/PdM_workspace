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

/* Defines the default baud rate for the UART communication*/
#define BAUD_RATE 9600
/* Defines the default timeout delay for the communication in mS*/
#define TIMEOUT_DELAY 100

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  uartinit, initializes the UART Module and if it is successful sends
 * 		   trough the same module the configurations. Returns true if the
 * 		   initialization is successful, otherwise return false
 * @param  UartHandle pointer to the UART_Handle structure which holds the
 * 		   parameters
 * @retval Boolean, true if the module initializes properly, otherwise it
 * 	       returns false
 */
bool_t uartInit(UART_HandleTypeDef *UartHandle);

/**
 * @brief  uartSendString, sends a full string trough the UART module selected
 * @param  UartHandle, pointer to theUart structure that sets
 * 		   which uart module to use
 * @param  txString, pointer to the string to send fully
 * @retval None
 */
void uartSendString(UART_HandleTypeDef *UartHandle, char *txString);

/**
 * @brief  uartSendStringSize, sends a string trough the UART module selected
 * 		   up to the byte specified
 * @param  UartHandle, pointer to theUart structure that sets
 * 		   which uart module to use
 * @param  txString, pointer to the string to send
 * @param  size, positive integer of bytes of the string to send
 * @retval None
 */
void uartSendStringSize(UART_HandleTypeDef *UartHandle, char *txString, uint16_t size);

/**
 * @brief  uartReceiveStringSize, receives a string trough the UART module selected up
 * 		   to the byte specified
 * @param  UartHandle, pointer to theUart structure that sets
 * 		   which uart module to use
 * @param  rxString, pointer to the string where to store the received information
 * @param  size, positive integer of bytes to receive
 * @retval None
 */
void uartReceiveStringSize(UART_HandleTypeDef *UartHandle, char *rxString, uint16_t size);


#endif /* API_INC_API_UART_H_ */
