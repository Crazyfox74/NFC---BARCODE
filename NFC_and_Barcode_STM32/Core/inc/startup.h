/*
 * startup.h
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#ifndef INC_STARTUP_H_
#define INC_STARTUP_H_

#include "stm32f10x.h"


void TimingDelay_Decrement ( void );

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds
 * @retval None
 */
void Delay ( uint32_t nTime ) ;



#endif /* INC_STARTUP_H_ */
