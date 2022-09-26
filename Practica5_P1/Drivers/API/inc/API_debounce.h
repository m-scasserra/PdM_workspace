/*
 * API_debounce.h
 *
 *  Created on: Sep 13, 2022
 *      Author: Marco Scasserra
 *      Brief : Implementation of modularized non blocking debouncing routine
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "API_delay.h"

/* Exported defines ----------------------------------------------------------*/
#define DEBOUNCE_DELAY 40
#define ESTADO_INICIAL BUTTON_UP

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  debounceFSM_init, initializes the debounce FSM
 * @param  None
 * @retval None
 */
void debounceFSM_Init();

/**
 * @brief  debounceFSM_update, updates the debounce FSM
 * @param  None
 * @retval None
 */
void debounceFSM_Update();

/**
 * @brief  readKey, returns the value of the variable keyPressed. keyPressed is
 * 		   true if debouncing FSM detects a valid button input
 * @param  None
 * @retval true if the FSM detects a valid button input, false otherwise
 */
bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
