/*
 * API_uart.c
 *
 *  Created on: Sep 21, 2022
 *      Author: marco
 */

/* Includes ------------------------------------------------------------------*/
#include "API_uart.h"

/* Private functions ---------------------------------------------------------*/
static void okInitMessage(UART_HandleTypeDef *UartHandle){
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
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
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
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
void uartSendString(UART_HandleTypeDef *UartHandle, char *txString){
	if(UartHandle != NULL){
		uint16_t messageLength = strlen((const char*)txString);
		HAL_UART_Transmit(UartHandle, (uint8_t *)txString, messageLength, TIMEOUT_DELAY);
	}else{
		while(1){}
	}
}

/**
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
void uartSendStringSize(UART_HandleTypeDef *UartHandle, char *txString, uint16_t size){
	if(size > 0 && UartHandle != NULL){
		HAL_UART_Transmit(UartHandle, (uint8_t *)txString, size, TIMEOUT_DELAY);
	}else{
		while(1){}
	}
}

/**
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
void uartReceiveStringSize(UART_HandleTypeDef *UartHandle, char *rxString, uint16_t size){
	if(size > 0 && UartHandle != NULL){
		HAL_UART_Receive(UartHandle, (uint8_t *)rxString, size, TIMEOUT_DELAY);
	}else{
		while(1){}
	}
}
