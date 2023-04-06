/*
 * usart.c
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#include "usart.h"

extern bool b_getversion;


uint8_t cntbyteversion;
uint8_t ScanerVersion [71]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


char date_InputQueue [ DATE_LEN ];
static int date_iBegin = 0;
static int date_iEnd = 0;

char g_aInputQueue [ QUEUE_LEN ];
static int g_iBegin = 0;
static int g_iEnd = 0;

int flag_usart = 0;

//USART2 для передачи на компьютер
void Usart2Init(void) {
	GPIO_InitTypeDef		GPIO_InitStruct;
	USART_InitTypeDef		USART2_InitStruct;
	NVIC_InitTypeDef		NVIC_InitStruct;

	/* USART2 initialization: PA2 - USART2_TX, PA3 - USART2_RX */

	USART2_RCC_PERIPH_CLOCK_CMD ( USART2_RCC_PORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = USART2_TX_PIN;
	GPIO_Init ( USART2_GPIO_PORT, &GPIO_InitStruct );

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = USART2_RX_PIN;
	GPIO_Init ( USART2_GPIO_PORT, &GPIO_InitStruct );

	USART_RCC_PERIPH_CLOCK_CMD ( USART2_RCC_APBPORT, ENABLE );

	USART2_InitStruct.USART_BaudRate = 9600;//для работы с SPI2 только эта скорость подходит
	USART2_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART2_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART2_InitStruct.USART_Parity = USART_Parity_No;
	USART2_InitStruct.USART_StopBits = USART_StopBits_1;
	USART2_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init ( USART2_NUM, &USART2_InitStruct );

	USART_Cmd ( USART2_NUM, ENABLE );

	NVIC_PriorityGroupConfig ( NVIC_GROUP );
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init ( &NVIC_InitStruct );

	USART_ITConfig ( USART2_NUM, USART_IT_RXNE, ENABLE );

}
//******************************************************************


//******************************************************************
void Usart2_SendData (char* strcmd, int sz_cmd){
//Передача на копьютер до символа окончания строки (0x00).
	for(int ik=0;ik<sz_cmd;ik++){
	    //if(strcmd[ik]!=0x00){
 		  while(USART_GetFlagStatus(USART2_NUM, USART_FLAG_TXE) == RESET){}
		  USART_SendData(USART2_NUM, strcmd[ik]);
	    //}
	}

}

//******************************************************************
//******************************************************************
void SCAN_UsartInit(void) {
	GPIO_InitTypeDef		GPIO_InitStruct;
	USART_InitTypeDef		USART_InitStruct;
	NVIC_InitTypeDef		NVIC_InitStruct;

	/* USART1 initialization: PA9 - USART1_TX, PA10 - USART1_RX */

	USART_RCC_PERIPH_CLOCK_CMD ( USART_RCC_PORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = USART_TX_PIN;
	GPIO_Init ( USART_GPIO_PORT, &GPIO_InitStruct );

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = USART_RX_PIN;
	GPIO_Init ( USART_GPIO_PORT, &GPIO_InitStruct );

	USART_RCC_PERIPH_CLOCK_CMD ( USART_RCC_APBPORT, ENABLE );

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init ( USART_NUM, &USART_InitStruct );

	USART_Cmd ( USART_NUM, ENABLE );

	NVIC_PriorityGroupConfig ( NVIC_GROUP );
	NVIC_InitStruct.NVIC_IRQChannel = USART_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init ( &NVIC_InitStruct );

	USART_ITConfig ( USART_NUM, USART_IT_RXNE, ENABLE );
}



void SCAN_SendCMD (char* strcmd,int sz_cmd){

	for(int ik=0;ik<sz_cmd;ik++){

		 while (READ_BIT(USART_NUM->SR, USART_SR_TXE) != (USART_SR_TXE)) {}

		USART_SendData(USART_NUM, strcmd[ik]);
	}


}

void USART_NUM_IRQHandler ( void ) {
	if (USART_GetFlagStatus ( USART_NUM, USART_FLAG_RXNE ) == SET) {
		if(b_getversion){

			USART_ClearITPendingBit ( USART_NUM, USART_IT_RXNE );
			ScanerVersion [ cntbyteversion++ ] = USART_ReceiveData ( USART_NUM );
			if (cntbyteversion>71){
				b_getversion=DISABLE;
				cntbyteversion=0;
			}

		}
		else{

		USART_ClearITPendingBit ( USART_NUM, USART_IT_RXNE );
		g_aInputQueue [ g_iEnd++ ] = USART_ReceiveData ( USART_NUM );
		g_iEnd %= QUEUE_LEN;
		}
	}
}




ErrorStatus FIFO_GetNextData ( char * pResult ) {
	ErrorStatus res = ERROR;

	if ( g_iBegin != g_iEnd ) {
		*pResult = g_aInputQueue [ g_iBegin++ ];
		g_iBegin %= QUEUE_LEN;
		res = SUCCESS;
	}

	return res;
}



ErrorStatus Date_GetNextDate(char *dResult ){
	ErrorStatus res = ERROR;
	if(date_iBegin != date_iEnd){
		*dResult = date_InputQueue[date_iBegin++];
		date_iBegin %= DATE_LEN;
		res = SUCCESS;
	}
	return res;
}

void USART2_IRQHandler(void){
	if(USART_GetFlagStatus ( USART2_NUM, USART_FLAG_RXNE) == SET){
		flag_usart = 1;
		USART_ClearITPendingBit(USART2_NUM, USART_IT_RXNE);
		date_InputQueue [ date_iEnd++ ] = USART_ReceiveData(USART2_NUM);
		date_iEnd %= DATE_LEN;

	}
}
