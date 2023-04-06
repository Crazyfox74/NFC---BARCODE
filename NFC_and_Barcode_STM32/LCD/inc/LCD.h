/*
 * LCD.h
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "perif.h"
#include "string.h"
#include "startup.h"


void SCAN_SumToString ( int sum, char * sumStr );

uint8_t I2C_SingleRandomWrite ( I2C_TypeDef* I2Cx, uint8_t Device, uint8_t Data );

void LCD_SetCursor ( uint8_t index );
void LCD_WriteData ( uint8_t data );
void LCD_WriteString ( char * str );
//Отображение кода сканера штрих-кодов бегущей строкой в первой строчке LCD
void LCD_RunStringBARCode ( char * str, uint32_t delay, uint8_t start );
//Отображение кода сканера NFC бегущей строкой во второй строчке LCD
void LCD_RunStringNFCCode ( char * str, uint32_t delay, uint8_t start );
void LCD_DisplaySum ( void );
//Сборка сборка бегущей строки с версией ПО NFC модудя для LCD


void LCD_RunStringDevInfo ( char * str1line,char * str2line, uint32_t delay);
void LCD_Init(void);


uint32_t GetTickCount();

#endif /* INC_LCD_H_ */
