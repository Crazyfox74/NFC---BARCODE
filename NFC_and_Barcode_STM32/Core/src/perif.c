/*
 * perif.c
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#include "perif.h"


extern uint16_t cnttimer1;
extern uint16_t timer1data;


extern bool b_1secflg;
extern uint16_t cnttimer30sec;
extern uint16_t cnttimer1sec;
extern uint16_t cnttimer1;
extern uint8_t previousState;
extern uint8_t u8_btnstate;
extern uint16_t timer1data;

extern uint8_t u8_wrnfcstatus;
extern bool b_wrstr1;//флаг выдачи первой строки в режиме записи


extern uint8_t uidnfcwrdara[];	// массив для записи номера метки nfc
extern uint8_t u8_cntchar;
extern uint8_t u8_cntindex;

extern uint8_t pn_chip;
extern uint8_t pn_fw1chip;
extern uint8_t pn_fw2chip;
extern uint8_t pn_suppchip;



extern char s_pn_nchip[2];
extern char s_pn_fw1nchip[3];
extern char s_pn_fw2nchip[3];
extern char s_pn_suchip[2];

extern uint32_t tick;


extern char b_bounce_value[];
extern uint16_t testtimer1;


extern uint8_t outwrdata[];


uint32_t bcode_to_flash[4];
uint64_t bcode;
char barcode[64];

uint32_t nfc_to_flash[2];

uint8_t data_to_flash[16];
uint8_t addr_to_flash[4];

uint8_t wr_test_buff[256];
uint8_t rd_test_buff[256];
uint8_t rd_flash_buff[16];
uint32_t wr_test_addr;
uint8_t addr_to_wr[4];
char fl_bar_buff[16];


NVIC_InitTypeDef        NVIC_InitTmr1Structure;
EXTI_InitTypeDef		EXTI_InitExtiStruct;
NVIC_InitTypeDef		NVIC_InitExtiStruct;

void LED_Init(void) {
	GPIO_InitTypeDef		GPIO_InitStruct;
//	EXTI_InitTypeDef		EXTI_InitStruct;
//	NVIC_InitTypeDef		NVIC_InitStruct;
/*
 /*
 /* конфигурация вывода PB13 на активный выход */
//GPIO_InitStruct.Pin = GPIO_PIN_13; // номер вывода
//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // режим выход
//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM; // средняя скорость выхода
//HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	/*
	 * EXTI pin configure
	 */
	LED_RCC_PERIPH_CLOCK_CMD ( LED_RCC_GPIO_PORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_Init ( LED_GPIO_PORT, &GPIO_InitStruct );
//#define LED_GPIO_PORT					GPIOA
//#define LED_PIN							GPIO_Pin_12


	GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 0);
//	/* Enable AFIO clock */
//	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO, ENABLE );
//
//	/* Connect EXTI0 Line to PB.0 pin */
//	GPIO_EXTILineConfig ( BTN_EXTI_PORTSOURCE, BTN_EXTI_PINSOURCE );
//
//	EXTI_InitStruct.EXTI_Line = BTN_EXTI_LINE;
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_Init ( &EXTI_InitStruct );
//
//	NVIC_PriorityGroupConfig ( NVIC_GROUP );
//	NVIC_InitStruct.NVIC_IRQChannel = BTN_IRQ;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init ( &NVIC_InitStruct );
}





void BTN_Init(void) {
	GPIO_InitTypeDef		GPIO_InitStruct;
//	EXTI_InitTypeDef		EXTI_InitStruct;
//	NVIC_InitTypeDef		NVIC_InitStruct;

	/*
	 * EXTI pin configure
	 */
	BTN_RCC_PERIPH_CLOCK_CMD ( BTN_RCC_GPIO_PORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = BTN_PIN;
	GPIO_Init ( BTN_GPIO_PORT, &GPIO_InitStruct );

//	/* Enable AFIO clock */				//комментирование блока инициализации для кнопки с прерыванием
//	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO, ENABLE );
////
////	/* Connect EXTI0 Line to PB.0 pin */
//	GPIO_EXTILineConfig ( BTN_EXTI_PORTSOURCE, BTN_EXTI_PINSOURCE );
////
//	EXTI_InitExtiStruct.EXTI_Line = BTN_EXTI_LINE;
//	EXTI_InitExtiStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitExtiStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitExtiStruct.EXTI_Trigger = EXTI_Trigger_Falling;//EXTI_Trigger_Rising_Falling;
//	EXTI_Init ( &EXTI_InitExtiStruct );
//
//	NVIC_PriorityGroupConfig ( NVIC_GROUP );
//	NVIC_InitExtiStruct.NVIC_IRQChannel = BTN_IRQ;
//	NVIC_InitExtiStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitExtiStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitExtiStruct.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init ( &NVIC_InitExtiStruct );
}



void BTN_IRQHandler ( void ) {
	if (EXTI_GetFlagStatus ( BTN_EXTI_LINE ) == SET) {
		if(u8_btnstate==BTN_UNPRESSED){

		  NVIC_InitExtiStruct.NVIC_IRQChannelCmd = DISABLE;
		  EXTI_InitExtiStruct.EXTI_Trigger = EXTI_Trigger_Rising;
		  NVIC_Init ( &NVIC_InitExtiStruct );
		  cnttimer1=0;
		  u8_btnstate=BTN_PRESSED;
		  EnableTimer1Interrupt();
		  GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 1);
		}
		else{
			//DisableTimer1Interrupt
			  NVIC_InitExtiStruct.NVIC_IRQChannelCmd = DISABLE;
			  EXTI_InitExtiStruct.EXTI_Trigger = EXTI_Trigger_Falling;
			  NVIC_Init ( &NVIC_InitExtiStruct );
			  timer1data=cnttimer1;
			  u8_btnstate=BTN_UNPRESSED;
			  GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 0);


		}
//		if(b_ledbtnstate==FALSE){
//
//		GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 1);
//		b_ledbtnstate=TRUE;
//		}
//		else{
//			GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 0);
//			b_ledbtnstate=FALSE;
//		}
		EXTI_ClearITPendingBit ( BTN_EXTI_LINE );


	}
}


void Timer1Init(void)	{		//	(функция инициализации таймера)

//GPIO_InitTypeDef  		GPIO_InitStruct;
TIM_TimeBaseInitTypeDef		TIMER1_InitStruct;


//	LED_RCC_PERIPH_CLOCK_CMD ( LED_RCC_GPIO_PORT, ENABLE );

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
/*
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_Init ( LED_GPIO_PORT, &GPIO_InitStruct );

*/
	TIMER1_InitStruct.TIM_Prescaler=TIMER1_PRESCALER-1;	//предделитель 720-1
	TIMER1_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;	//режим прямого счета
	TIMER1_InitStruct.TIM_Period=TIMER1_RELOAD_PERIOD;		//множитель периодов?
	TIMER1_InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;		//делитель самого таймера(можно не ставить)
	TIMER1_InitStruct.TIM_RepetitionCounter=0x0000;		//счетчик повторов?
	TIM_TimeBaseInit(TIM1,&TIMER1_InitStruct);


	//timer1data=1;
    NVIC_InitTmr1Structure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitTmr1Structure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitTmr1Structure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitTmr1Structure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitTmr1Structure);

	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM1,DISABLE);


	//NVIC_EnableIRQ(TIM1_UP_IRQn);


}



void EnableTimer1Interrupt()
{
    //NVIC_InitTmr1Structure.NVIC_IRQChannelCmd = ENABLE;
    //NVIC_Init(&NVIC_InitTmr1Structure);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM1,ENABLE);

    NVIC_EnableIRQ(TIM1_UP_IRQn);

}

void DisableTimer1Interrupt()
{

    //NVIC_InitTmr1Structure.NVIC_IRQChannelCmd = ENABLE;
    //NVIC_Init(&NVIC_InitTmr1Structure);
	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM1,DISABLE);

    NVIC_EnableIRQ(TIM1_UP_IRQn);

}




void TIM1_IRQHandler()
{
/*

 //---------------------------------------------------------------------------------------------
uint8_t uidnfcwrdara[] = { '0', '0', '0', '0', '0', '0', '0', '0'};	// массив для записи номера метки nfc
uint8_t u8_cntchar;
uint8_t u8_cntindex;
//--------------------------------------------------------------------------------------------

 */

	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){


		if (GPIO_ReadInputDataBit ( BTN_GPIO_PORT, BTN_PIN ) == Bit_RESET) {//если кнопка нажата

			   if (previousState==1){
				   u8_btnstate=0;
				   cnttimer1=0;

				}
			   else{
				   cnttimer1++;
				   if (cnttimer1==60 && u8_wrnfcstatus==0) {

					   u8_wrnfcstatus=1; b_wrstr1=TRUE;//флаг выдачи первой строки в режиме записи
					   u8_cntchar=0; u8_cntindex=0;
					   b_1secflg=FALSE;
					   cnttimer1sec=0;


				   }
			   }

		}
		else{//если кнопка не нажата

			 if(previousState==0){
				 u8_btnstate=1;
				 cnttimer1=0;
				 cnttimer30sec=0;
				 switch (u8_wrnfcstatus){
				 case 1:
				 case 2:
				 case 3:
				 case 4:
				 case 5:
				 case 6:
				 case 7:
				 case 8:
					    u8_cntchar++;
					    if (u8_cntchar>15) u8_cntchar=0;
					    if (u8_cntchar<10){
					       uidnfcwrdara[u8_cntindex]= 48 + u8_cntchar;
					    }
					    else{
					    	uidnfcwrdara[u8_cntindex]= 55 + u8_cntchar;
					    }
					 break;


				 }

				}

			   else{
				   cnttimer30sec++;
				   if(cnttimer30sec==600){
					   u8_wrnfcstatus=0;
					   cnttimer30sec=0;
				   }


				   cnttimer1sec++;
				   if(cnttimer1sec==10){
					   cnttimer1sec=0;
					   b_1secflg=~b_1secflg;
				   }

				   cnttimer1++;
				   if (cnttimer1==100 ) {
					   switch (u8_wrnfcstatus){
					   case 1:
						   u8_wrnfcstatus=2;
						   u8_cntchar=0; u8_cntindex=1;
						   break;
					   case 2:
						   u8_wrnfcstatus=3;
						   u8_cntchar=0; u8_cntindex=2;
						   break;
					   case 3:
						   u8_wrnfcstatus=4;
						   u8_cntchar=0; u8_cntindex=3;
						   break;
					   case 4:
						   u8_wrnfcstatus=5;
						   u8_cntchar=0; u8_cntindex=4;
						   break;
					   case 5:
						   u8_wrnfcstatus=6;
						   u8_cntchar=0; u8_cntindex=5;
						   break;
					   case 6:
						   u8_wrnfcstatus=7;
						   u8_cntchar=0; u8_cntindex=6;
						   break;
					   case 7:
						   u8_wrnfcstatus=8;
						   u8_cntchar=0; u8_cntindex=7;
						   break;
					   case 8:
					   		u8_wrnfcstatus=9;

					   		break;
					   }

				   }
			   }

		}

		previousState=u8_btnstate;

//		if(u8_btnstate==BTN_PRESSED){
//
//			cnttimer1++;
//		}
//		if(u8_btnstate==BTN_UNPRESSED){
//
//		}

		////////////////////////////////////////////////////////
	//	cnttimer1++;
//
//		if(u8_btnstate==BTN_PRESSED){
//		 if(cnttimer1==10){
//
//				NVIC_InitExtiStruct.NVIC_IRQChannelCmd = ENABLE;
//				NVIC_Init ( &NVIC_InitExtiStruct );
//
//		 }
//		}
//
//		if(u8_btnstate==BTN_UNPRESSED)
//		{
//			if(cnttimer1==timer1data+10){
//
//				NVIC_InitExtiStruct.NVIC_IRQChannelCmd = ENABLE;
//				NVIC_Init ( &NVIC_InitExtiStruct );
//				DisableTimer1Interrupt();
//				previousState=1;
//				timer1data=0;
//			}
//
//		}
//
////	GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 1);
//
//	TIM1->ARR=TIMER1_RELOAD_PERIOD;
//
//	if(cnttimer1==timer1data){
//		cnttimer1=0;
//
//    // Если на выходе был 0
//    if (previousState == 0)
//    {
//        // Выставляем единицу на выходе
//        previousState = 1;
//    //    GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 1);
//        timer1data=3;
//        // Период 50 тиков таймера, то есть 0.5 мс
//        //TIM1->ARR = 50;
//    }
//    else
//    {
//        // Выставляем ноль на выходе
//        previousState = 0;
//  //      GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 0);
//        timer1data=1;
//        // А период теперь будет 250 тиков – 2.5 мс
//       // TIM1->ARR = 250;
//    }
//
//	}
//



    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

	}
}

uint32_t GetTickCount() {
	return tick;
}


void PN532_FirmWareVersion2String(uint32_t version){

		 pn_chip=version>>24;
		 pn_fw1chip=version>>16;
		 pn_fw2chip=version>>8;
		 pn_suppchip=version&0xFF;

		 s_pn_nchip[0] = 0x30 + (pn_chip >> 4);
		 s_pn_nchip[1] = 0x30 + (pn_chip & 0x0F);

		 s_pn_fw1nchip[0] = 0x30+(pn_fw1chip/100);
		 s_pn_fw1nchip[1] = 0x30+(pn_fw1chip/10%10);
		 s_pn_fw1nchip[2] = 0x30+(pn_fw1chip%10);

		 s_pn_fw2nchip[0] = 0x30+(pn_fw2chip/100);
		 s_pn_fw2nchip[1] = 0x30+(pn_fw2chip/10%10);
		 s_pn_fw2nchip[2] = 0x30+(pn_fw2chip%10);

		 s_pn_suchip[0] = 0x30 + (pn_suppchip >> 4);
		 s_pn_suchip[1] = 0x30 + (pn_suppchip & 0x0F);

}



void ValueInterrupToStr(){
	b_bounce_value[0] = 0x30+(testtimer1/10000%10);
	b_bounce_value[1] = 0x30+(testtimer1/1000%10);
	b_bounce_value[2] = 0x30+(testtimer1/100%10);
	b_bounce_value[3] = 0x30+(testtimer1/10%10);
	b_bounce_value[4] = 0x30+(testtimer1%10);

}



bool fourbytes2strhex(uint8_t *indatarray, uint8_t *outstrhex){

	uint8_t i=0;
	uint8_t b=0;
	uint8_t th=0;
	uint8_t tl=0;

		for ( b=0; b<4; b++){


			th = indatarray[b] >> 4;
			if ((th>=0) && (th <= 9))   { th = th + 0x30; }
			else{
			     if ((th>=0x0A) && (th<=0x0F) ) { th = th + 0x37;}
			}

			tl = indatarray[b]& 0x0F;

			if ((tl>=0) && (tl <= 9) )   { tl = tl + 0x30; }
			else{
				if ((tl>=0x0A) && (tl<=0x0F) ) { tl = tl + 0x37;}

			}

			outstrhex[i] = '0';	i++;
			outstrhex[i] = 'x';	i++;
			outstrhex[i] = th;  i++;
			outstrhex[i] = tl;	i++;
			//outstrhex[i] = ' '; i++;
			if(i<18){ outstrhex[i] = ' '; i++; }


		}
		return 1;
	//}
	//else{
	//	return 0;
	//}
}



uint32_t UIDResponse(uint8_t *uid,  uint8_t uidLength){
	uint32_t result;
	result=0;

	if(uidLength==4){

		result = uid[0];
		result <<= 8;
		result |= uid[1];
		result <<= 8;
		result |= uid[2];
		result <<= 8;
		result |= uid[3];
	}
return result;

}


bool BytesHex2Str(uint8_t *indatarray, uint8_t size_array, uint8_t *outstrhex){

	uint8_t i=0;
	uint8_t b=0;
	uint8_t th=0;
	uint8_t tl=0;

		for ( b=0; b<size_array; b++){


			th = indatarray[b] >> 4;
			if ((th>=0) && (th <= 9))   { th = th + 0x30; }
			else{
			     if ((th>=0x0A) && (th<=0x0F) ) { th = th + 0x37;}
			}

			tl = indatarray[b]& 0x0F;

			if ((tl>=0) && (tl <= 9) )   { tl = tl + 0x30; }
			else{
				if ((tl>=0x0A) && (tl<=0x0F) ) { tl = tl + 0x37;}

			}

		//	outstrhex[i] = '0';	i++;
		//	outstrhex[i] = 'x';	i++;
			outstrhex[i] = th;  i++;
			outstrhex[i] = tl;	i++;
			//outstrhex[i] = ' '; i++;

			 outstrhex[i] = ' '; i++;




		}
		return 1;
	//}
	//else{
	//	return 0;
	//}
}

void PN532_WriteTagtoHex(){

	/*

	 //---------------------------------------------------------------------------------------------
	uint8_t uidnfcwrdara[] = { '0', '0', '0', '0', '0', '0', '0', '0'};	// массив для записи номера метки nfc
	uint8_t u8_cntchar;
	uint8_t u8_cntindex;
	//--------------------------------------------------------------------------------------------

	 */

	uint8_t k=0;
	uint8_t tx=0;
	uint8_t x=0;

	for ( x=0; x<4 ; x++){


				//tx = uidnfcwrdara[k];

				if ((uidnfcwrdara[k]>='0') && (uidnfcwrdara[k] <= '9'))   { tx = uidnfcwrdara[k] - 0x30; }
				else{
				     if ((uidnfcwrdara[k]>='A') && (uidnfcwrdara[k]<='F') ) { tx = uidnfcwrdara[k] - 0x37;}
				}

				outwrdata[x]=tx<<4;
				k++;

				if ((uidnfcwrdara[k]>='0') && (uidnfcwrdara[k] <= '9'))   { tx = uidnfcwrdara[k] - 0x30; }
				else{
				     if ((uidnfcwrdara[k]>='A') && (uidnfcwrdara[k]<='F') ) { tx = uidnfcwrdara[k] - 0x37;}
				}
				outwrdata[x]|=tx;
				k++;

			}


}



void spi_gpio_init(void){

	GPIO_InitTypeDef  		GPIO_InitStruct;

	SPI_RCC_PORT_CLOCK_CMD ( SPI_RCC_PORT, ENABLE);
	SPI_RCC_PERIPH_CLOCK_CMD ( SPI_RCC_APBPORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN | SPI_SCK_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_NSS_PIN | SPI_FLASH_NSS;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );


	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_AF_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );






}



uint64_t bcode_conv_to_flash(uint32_t rtc_time, char *bar_buff){

	uint8_t s;
	char *pBar = bar_buff;
	if(bar_buff[0] =='\n'){
		*pBar++;
	}
	for(int i = 0; i < 13; i++){
		barcode[i] = *pBar++;
	}

	for(int a=0;a<7;a++){
		if(a==0){
			s=barcode[a]-0x30;
			s<<=4;
			s|=barcode[a+1]-0x30;
		}
		if(a>0 && a<=5){
			s=barcode[a*2]-0x30;
			s<<=4;
			s|=barcode[a*2+1]-0x30;
		}
		if(a==6){
			s=barcode[a*2]-0x30;
			s<<=4;
		}

		bcode<<=8;
		bcode|=(uint64_t)s;

	}


	return bcode;

}


void nfc_conv_to_flash(uint32_t rtc_time, uint32_t nfc_tag ){
	nfc_to_flash[0]=rtc_time;
	nfc_to_flash[1]=nfc_tag;
}


void data_conv2Flash(uint32_t cnt_timer, uint64_t data ){


	data_to_flash[0]=(cnt_timer >> 24) & 0xFF;
	data_to_flash[1]=(cnt_timer >> 16) & 0xFF;
	data_to_flash[2]=(cnt_timer >> 8) & 0xFF;
	data_to_flash[3]=cnt_timer & 0xFF;

	data_to_flash[4]=(data >> 48) & 0xFF;
	data_to_flash[5]=(data >> 40) & 0xFF;
	data_to_flash[6]=(data >> 32) & 0xFF;
	data_to_flash[7]=(data >> 24) & 0xFF;
	data_to_flash[8]=(data >> 16) & 0xFF;
	data_to_flash[9]=(data >> 8) & 0xFF;
	data_to_flash[10]=data & 0xFF;


}

void nfc_conv2Flash(uint32_t rtc_time, uint32_t data ){


	data_to_flash[0]=(rtc_time >> 24) & 0xFF;
	data_to_flash[1]=(rtc_time >> 16) & 0xFF;
	data_to_flash[2]=(rtc_time >> 8) & 0xFF;
	data_to_flash[3]=rtc_time & 0xFF;

	data_to_flash[4]=(data >> 24) & 0xFF;
	data_to_flash[5]=(data >> 16) & 0xFF;
	data_to_flash[6]=(data >> 8) & 0xFF;
	data_to_flash[7]=data & 0xFF;


}

void addr_conv2Flash(uint32_t addr_flash){

	addr_to_flash[0]=(addr_flash >> 24) & 0xFF;
	addr_to_flash[1]=(addr_flash >> 16) & 0xFF;
	addr_to_flash[2]=(addr_flash >> 8) & 0xFF;
	addr_to_flash[3]=addr_flash & 0xFF;

}

uint32_t flash_conv2_addr(uint8_t *padBuf){

	uint32_t addr2_wr = (padBuf[0] << 24) | (padBuf[1] << 16) | (padBuf[2] << 8) | padBuf[3];
	return addr2_wr;

}

uint32_t flash_conv2_timer(uint8_t *pdBuf){

	uint32_t timer_flash = (pdBuf[0] << 24) | (pdBuf[1] << 16) | (pdBuf[2] << 8) | pdBuf[3];
	return timer_flash;

}

void flash_conv2_bcode(uint8_t *pdBuf){

/*	char *pbar_buff = fl_bar_buff;
	pbar_buff = (pdBuf[4] >> 4) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[4] && 0x0F) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[5] >> 4) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[5] && 0x0F) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[6] >> 4) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[6] && 0x0F) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[7] >> 4) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[7] && 0x0F) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[8] >> 4) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[8] && 0x0F) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[9] >> 4) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[9] && 0x0F) + 0x30;
	pbar_buff++;
	pbar_buff = (pdBuf[10] >> 4) + 0x30;
	pbar_buff++;
/*	pbar_buff = (pdBuf[9] && 0x0F) + 0x30;
	pbar_buff++;*/

	fl_bar_buff[0] = (pdBuf[4] >> 4) + 0x30;
	fl_bar_buff[1] = (pdBuf[4] && 0x0F) + 0x30;
	fl_bar_buff[2] = (pdBuf[5] >> 4) + 0x30;
	fl_bar_buff[3] = (pdBuf[5] && 0x0F) + 0x30;
	fl_bar_buff[4] = (pdBuf[6] >> 4) + 0x30;
	fl_bar_buff[5] = (pdBuf[6] && 0x0F) + 0x30;
	fl_bar_buff[6] = (pdBuf[7] >> 4) + 0x30;
	fl_bar_buff[7] = (pdBuf[7] && 0x0F) + 0x30;
	fl_bar_buff[8] = (pdBuf[8] >> 4) + 0x30;
	fl_bar_buff[9] = (pdBuf[8] && 0x0F) + 0x30;
	fl_bar_buff[10] = (pdBuf[9] >> 4) + 0x30;
	fl_bar_buff[11] = (pdBuf[9] && 0x0F) + 0x30;
	fl_bar_buff[12] = (pdBuf[10] >> 4) + 0x30;

}



void wr_test_prepare(uint32_t addr_wr, uint16_t cnt){

	wr_test_addr = addr_wr;

	for(int i = 0; i < cnt ;i++){
		wr_test_buff[i] = wr_test_addr;
		wr_test_addr++;
	}
}
