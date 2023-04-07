/*
 * rtc_time.h
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: crazy
 */

#ifndef INC_RTC_TIME_H_
#define INC_RTC_TIME_H_


#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define SEC_A_DAY 86400

#define DATE_OK			0
#define INCORRECT_YEAR	1
#define INCORRECT_MONTH	2
#define INCORRECT_DAY	3
#define INCORRECT_HOUR 	4
#define INCORRECT_MIN	5
#define INCORRECT_SEC	6
#define INCORRECR_DATE_S	7



typedef struct
	{
	int year;
	char mon;
	char mday;
	char hour;
	char min;
	char sec;
	char wday;
	} rtc_cal;

void timer_to_cal (unsigned long timer, rtc_cal * rtc_time);
unsigned long cal_to_timer (rtc_cal * rtc_time);
uint8_t convert_time_from_android(rtc_cal *rtc_time, char *date);


#endif /* INC_RTC_TIME_H_ */
