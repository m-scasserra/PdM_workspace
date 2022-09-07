/*
 * API_delay.c
 *
 *  Created on: Sep 6, 2022
 *      Author: Marco Scasserra
 */

#include <stddef.h>
#include "API_delay.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"


/* Public functions ---------------------------------------------------------*/
/**
 * @brief  Initializes a delay of miliseconds
 * @param  *delay pointer to the delay structure
 * @param  duration duration of the delay, must be a positive integer
 * @retval None
 */
void delayInit(delay_t *delay, tick_t duration){
	if(delay != NULL && duration > 0 && duration < MAX_DELAY){
		delay->duration = duration;
		delay->running = false;
		delay->startTime = 0;
	}else{
		while(1){

		}
	}
}

/**
 * @brief  Reads the current time of the delay, if the delay is not running it initializes it
 * @param  *delay pointer to the delay strucure
 * @retval True if its running, otherwise returns false
 */
bool_t delayRead(delay_t *delay){
	bool timerFinish = false;

	if(delay != NULL){
		if(delay->running == false){
			delay->startTime = HAL_GetTick();
			delay->running = true;
		}else{
			if(HAL_GetTick() - delay->startTime >= delay->duration){
				delay->running = false;
				delay->startTime = 0;
				timerFinish = true;
			}
		}
		return timerFinish;

	}else{
		while(1){
		}
	}
}

/**
 * @brief  Rewrites the duration of a delay in miliseconds
 * @param  *delay pointer to the delay structure
 * @param  duration new duration for the delay, must be a positive integer
 * @retval None
 */
void delayWrite(delay_t *delay, tick_t duration){
	if(delay != NULL && duration > 0 && duration < MAX_DELAY){
		delay->duration = duration;
	}else{
		while(1){
		}
	}
}
