/*
 * usart.h
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#ifndef INC_USART_H_
#define INC_USART_H_


#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#include "perif.h"

void Usart2_SendData (char* strcmd, int sz_cmd);
void Usart2Init(void);
void SCAN_UsartInit(void);
void SCAN_SendCMD (char* strcmd,int sz_cmd);

void USART_NUM_IRQHandler ( void );
ErrorStatus FIFO_GetNextData ( char * pResult );


ErrorStatus Date_GetNextDate(char *dResult );
void USART2_IRQHandler(void);


#endif /* INC_USART_H_ */
