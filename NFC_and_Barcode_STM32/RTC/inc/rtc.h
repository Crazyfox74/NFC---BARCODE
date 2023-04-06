/*
 * rtc.h
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: crazy
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f10x.h"
#include "stdio.h"



void RTC_INIT(void);
uint32_t RTC_GET_COUNTER(void);
void RTC_SET_COUNTER(uint32_t count);


#endif /* INC_RTC_H_ */
