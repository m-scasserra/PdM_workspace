/*
 * API_delay.h
 *
 *  Created on: Sep 6, 2022
 *      Author: Marco Scasserra
 *      Brief : Implementation of modularized non blocking Timer routine
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported defines ----------------------------------------------------------*/
#define MAX_DELAY 30000

/* Exported types ------------------------------------------------------------*/
typedef uint32_t tick_t;
typedef bool bool_t;

/**
 * @brief Basic struct for the creation of non blocking delays
 */
typedef struct{
	tick_t startTime; /**< Positive integer used to store the starting time of
                      	   the delay.*/
	tick_t duration; /**< Positive integer used to store the duration of the
                          delay.*/
	bool_t running; /**< Boolean variable used for storing if the timer is running
                        or not. True if it is, otherwise is false. */
} delay_t;

/* Exported functions ------------------------------------------------------- */
/**
 * @brief  Initializes a delay of milliseconds
 * @param  *delay pointer to the delay structure
 * @param  duration duration of the delay, must be a positive integer
 * @retval None
 */
void delayInit(delay_t *delay, tick_t duration);

/**
 * @brief  Reads the current time of the delay, if the delay is not running it initializes it
 * @param  *delay pointer to the delay structure
 * @retval True if its running, otherwise returns false
 */
bool_t delayRead(delay_t *delay);

/**
 * @brief  Rewrites the duration of a delay in milliseconds
 * @param  *delay pointer to the delay structure
 * @param  duration new duration for the delay, must be a positive integer
 * @retval None
 */
void delayWrite(delay_t *delay, tick_t duration);

#endif /* API_INC_API_DELAY_H_ */
