/*
 * API_uart.c
 *
 *  Created on: Sep 21, 2022
 *      Author: Marco Scasserra
 *      Brief : Implementation of modularized blocking UART communication
 *      routine. It implements 4 functions.
 *      uartInit Initializes the UART module
 *      uartSendString sends the full string
 *      uartSendStringSize sends the string up to the defined size
 *      uartReceiveStringSize receives the string up to the defined size
 */

/* Includes ------------------------------------------------------------------*/
#include "API_uart.h"

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  okInitMessage, if the UART module initalization is successful it will
 * 		   send the full characteristics of the communication via the UART
 * @param  UartHandle pointer to the UART_Handle structure which holds the
 * 		   parameters and makes the communication
 * @retval None
 */
static void okInitMessage(UART_HandleTypeDef *UartHandle){
	uartCLRScreen(UartHandle);
	char okInitString[] = "USART initialized correctly with the following parameters:\n\r";
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "USART instance: 3 \n\r");
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "Baud Rate: 9600 \n\r");
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "Word Length: 8 \n\r");
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "Stop bits: 1 \n\r");
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "Parity: Odd \n\r");
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "Hardware Control: None \n\r");
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "Mode: Tx and Rx \n\r");
	uartSendString(UartHandle, okInitString);
	strcpy(okInitString, "Over Sampling: 16 \n\r");
	uartSendString(UartHandle, okInitString);
}

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
bool_t uartInit(UART_HandleTypeDef *UartHandle){

	bool_t returnValue = true;
	if(BAUD_RATE < 0){
		while(1){}
	}

	UartHandle->Instance        = USART3;
	UartHandle->Init.BaudRate   = BAUD_RATE;
	UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle->Init.StopBits   = UART_STOPBITS_1;
	UartHandle->Init.Parity     = UART_PARITY_ODD;
	UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle->Init.Mode       = UART_MODE_TX_RX;
	UartHandle->Init.OverSampling = UART_OVERSAMPLING_16;

	if(HAL_UART_Init(UartHandle) != HAL_OK)
	{
		/* Initialization Error */
		returnValue = false;
	}else{
		okInitMessage(UartHandle);
	}
	return returnValue;
}


/**
 * @brief  uartCLRScreen, sends two commands to the serial terminal to clear the
 * 		   screen and send the cursor come to the UART module selected
 * @param  UartHandle, pointer to theUart structure that sets which uart module
 * 		   to use
 * @retval None
 */
void uartCLRScreen(UART_HandleTypeDef *UartHandle){
	if(UartHandle != NULL){
		char str[10];
		sprintf(str, "%c[2J", ESC_CMD);
		HAL_UART_Transmit(UartHandle, (uint8_t *)str, 4, TIMEOUT_DELAY);
		sprintf(str, "%c[H", ESC_CMD);
		HAL_UART_Transmit(UartHandle, (uint8_t *)str, 3, TIMEOUT_DELAY);
	}
}

/**
 * @brief  uartSendString, sends a full string trough the UART module selected
 * @param  UartHandle, pointer to theUart structure that sets
 * 		   which uart module to use
 * @param  txString, pointer to the string to send fully
 * @retval None
 */
void uartSendString(UART_HandleTypeDef *UartHandle, char *txString){
	if(UartHandle != NULL && txString != NULL){
		// You have to add 1 to the length so it sends the \0 char at the end of the string
		uint16_t messageLength = strlen((const char*)txString) + 1;
		HAL_UART_Transmit(UartHandle, (uint8_t *)txString, messageLength, TIMEOUT_DELAY);
	}else{
		while(1){}
	}
}

/**
 * @brief  uartSendStringSize, sends a string trough the UART module selected
 * 		   up to the byte specified
 * @param  UartHandle, pointer to theUart structure that sets
 * 		   which uart module to use
 * @param  txString, pointer to the string to send
 * @param  size, positive integer of bytes of the string to send
 * @retval None
 */
void uartSendStringSize(UART_HandleTypeDef *UartHandle, char *txString, uint16_t size){
	if(size > 0 && UartHandle != NULL && txString != NULL){
		HAL_UART_Transmit(UartHandle, (uint8_t *)txString, size, TIMEOUT_DELAY);
	}else{
		while(1){}
	}
}

/**
 * @brief  uartReceiveStringSize, receives a string trough the UART module selected up
 * 		   to the byte specified
 * @param  UartHandle, pointer to theUart structure that sets
 * 		   which uart module to use
 * @param  rxString, pointer to the string where to store the received information
 * @param  size, positive integer of bytes to receive
 * @retval None
 */
void uartReceiveStringSize(UART_HandleTypeDef *UartHandle, char *rxString, uint16_t size){
	if(size > 0 && UartHandle != NULL && rxString != NULL){
		HAL_UART_Receive(UartHandle, (uint8_t *)rxString, size, TIMEOUT_DELAY);
	}else{
		while(1){}
	}
}
