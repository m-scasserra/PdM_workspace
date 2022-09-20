/*
 * API_debounce.c
 *
 *  Created on: Sep 13, 2022
 *      Author: Marco Scasserra
 *      Brief : Implementation of modularized non blocking debouncing routine
 */

/* Includes ------------------------------------------------------------------*/
#include "API_debounce.h"
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"

/* Private types --------------------------------------------------------------*/

/**
 * @brief Enum for the implementation of a FSM for the switch
 */
typedef enum
{
    BUTTON_UP, /**< State for the switch when is resting*/
    BUTTON_FALLING, /**< State for the switch when it is on a transitory state
                         from resting to pressed*/
    BUTTON_DOWN, /**< State for the switch is being pressed*/
    BUTTON_RAISING /**< State for the switch when it is on a transitory state
                        from pressed to resting*/
} debounceState_t;


/* Global private variables --------------------------------------------------------*/

/* debounceState_t variable that tracks the current state of the button*/
static debounceState_t debounceState = ESTADO_INICIAL;

/* Boolean variable that tracks if the button is pressed. True if it is pressed,
 * otherwise is false.*/
static bool_t keyPressed = false;

/* delay_t variable for the timing of the debounce*/
static delay_t debounceTimer;

/* Public functions ---------------------------------------------------------*/

/**
 * @brief  Reads the variable keyPressed and returns it. If the variable
 * is true, it will reset it.
 * @param  None
 * @retval true if the variable is true, false otherwise.
 */
bool_t readKey(){
	bool_t returnValue = false;
	if(keyPressed == true){
		returnValue = true;
		keyPressed = false;
	}
	return returnValue;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  debounceFSM_Init, initializes the debounce FSM with a inital state
 * and initializes the timer for the debounce routine
 * @param  None
 * @retval None
 */
void debounceFSM_Init(){
	debounceState = ESTADO_INICIAL;
	/* Initialize the timer struct for the debouncing*/
	delayInit(&debounceTimer, DEBOUNCE_DELAY);
}

/**
 * @brief  debounceFSM_Update, updates the debounce FSM
 * @param  None
 * @retval None
 */
void debounceFSM_Update(){
	switch(debounceState){
		case BUTTON_UP:
			if(BSP_PB_GetState(BUTTON_USER)){
				debounceState = BUTTON_FALLING;
				delayRead(&debounceTimer);
			}
		break;
		case BUTTON_FALLING:
			if(delayRead(&debounceTimer)){
				if(BSP_PB_GetState(BUTTON_USER)){
					keyPressed = true;
					debounceState = BUTTON_DOWN;
				}else{
					debounceState = BUTTON_UP;
				}
			}
		break;
		case BUTTON_DOWN:
			if(!BSP_PB_GetState(BUTTON_USER)){
				debounceState = BUTTON_RAISING;
				delayRead(&debounceTimer);
			}
		break;
		case BUTTON_RAISING:
			if(delayRead(&debounceTimer)){
				if(!BSP_PB_GetState(BUTTON_USER)){
					debounceState = BUTTON_UP;
				}else{
					debounceState = BUTTON_DOWN;
				}
			}
		break;
		default:
			debounceFSM_Init();
	}
}
