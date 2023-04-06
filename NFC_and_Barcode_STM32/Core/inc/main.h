/*
 * main.h
 *
 *  Created on: Apr 1, 2023
 *      Author: crazy
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stdlib.h"
#include "rtc.h"
#include "rtc_time.h"
#include "spi.h"
#include "spiFlash.h"





void LCD_PN532_CreateRunInfoString();
//Сборка сборка бегущей строки с версией ПО сканера штрих-кодов модудя для LCD
void LCD_BAR_CreateRunInfoString();


uint8_t PN532_Write_Data(uint8_t *uid_card, uint8_t uid_card_len, uint8_t blockNumber, uint8_t *data);


#endif /* INC_MAIN_H_ */
