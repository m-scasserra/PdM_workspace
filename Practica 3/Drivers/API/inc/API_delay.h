/*
 * API_delay.h
 *
 *  Created on: Sep 6, 2022
 *      Author: Marco Scasserra
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported defines ------------------------------------------------------------*/
#define MAX_DELAY 30000

/* Exported types ------------------------------------------------------------*/
typedef uint32_t tick_t;
typedef bool bool_t;

/* Structure of delay_t with 3 parameters
 * @param startTime positive int
 * @param duration positive int
 * @param running boolean */
typedef struct{
	tick_t startTime;
	tick_t duration;
	bool_t running;
} delay_t;

/* Exported functions ------------------------------------------------------- */
void delayInit(delay_t *delay, tick_t duration);
bool_t delayRead(delay_t *delay);
void delayWrite(delay_t *delay, tick_t duration);

#endif /* API_INC_API_DELAY_H_ */
