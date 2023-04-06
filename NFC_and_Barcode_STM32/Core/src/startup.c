/*
 * startup.c
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#include "startup.h"

extern int main(void);

extern __IO uint32_t TimingDelay;


uint32_t tick = 0;

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement ( void ) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
	tick++;
}

/**
 * @brief  Inserts a delay time.
 * @param  nTime: specifies the delay time length, in milliseconds
 * @retval None
 */
void Delay ( uint32_t nTime ) {
	TimingDelay = nTime;
	while (TimingDelay != 0);
}




