/*
 * spi.c
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */


#include "spi.h"



static uint8_t *pBufTx, *pBufRx;
static uint16_t usBufPosRx, usBufPosTx;
static uint16_t usBufCnt;


volatile uint8_t SpiActive;					// флаг активной передачи



void PN532_SPIInit(void){

	SpiActive = 1;

//	GPIO_InitTypeDef  		GPIO_InitStruct;
	SPI_InitTypeDef			SPI_PN532_InitStruct;

/*	SPI_RCC_PORT_CLOCK_CMD ( SPI_RCC_PORT, ENABLE);
	SPI_RCC_PERIPH_CLOCK_CMD ( SPI_RCC_APBPORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN | SPI_SCK_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SPI_NSS_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );


	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_AF_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );
*/




//------------------------------------------------------------------------------
	//SPI_StructInit( &SPI_PN532_InitStruct);
	SPI_Cmd(SPI2, DISABLE);
	NVIC_DisableIRQ(SPI2_IRQn);

	/* Initialize the SPI_Direction member */
	SPI_PN532_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    /* initialize the SPI_Mode member */
	SPI_PN532_InitStruct.SPI_Mode = SPI_Mode_Master;
	/* initialize the SPI_DataSize member */
	SPI_PN532_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	/* Initialize the SPI_CPOL member */
	SPI_PN532_InitStruct.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_High;SPI_CPOL_Low;
	/* Initialize the SPI_CPHA member */
	SPI_PN532_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	/* Initialize the SPI_NSS member */
	SPI_PN532_InitStruct.SPI_NSS = SPI_NSS_Hard;//SPI_NSS_Hard;//SPI_NSS_Soft;//SPI_NSS_Hard;
	/* Initialize the SPI_BaudRatePrescaler member */
	SPI_PN532_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//SPI_BaudRatePrescaler_256;//SPI_BaudRatePrescaler_4;
	/* Initialize the SPI_FirstBit member */
	SPI_PN532_InitStruct.SPI_FirstBit = SPI_FirstBit_LSB;//SPI_FirstBit_MSB;//SPI_FirstBit_LSB;
	/* Initialize the SPI_CRCPolynomial member */
	SPI_PN532_InitStruct.SPI_CRCPolynomial = 7;

	SPI_Init (SPI2,  &SPI_PN532_InitStruct);
	//SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE|SPI_I2S_IT_TXE, ENABLE);
	SPI_I2S_ITConfig( SPI2_NUM, SPI_I2S_IT_RXNE, DISABLE );

	SPI_CalculateCRC ( SPI2, DISABLE);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_NSS_PIN,1);

	SPI_SSOutputCmd ( SPI2, ENABLE);


/*
	RCC_PLLCmd(DISABLE);
	uint32_t tmpreg = 0;
	tmpreg=RCC->CFGR;
	RCC->CFGR=tmpreg|0x00000500;
	RCC_PLLCmd(ENABLE);
*/

	SPI_Cmd(SPI2, ENABLE);

	SpiActive = 0;

}



void SPI_SetDataSize(SPI_TypeDef * SPIx, SPI_Bit_t bitnum)
{
	SPIx->CR1 &= ~(SPI_CR1_SPE);
	if(bitnum == SPI_8BIT){
		SPIx->CR1 &= ~(SPI_CR1_DFF);
	}
	else if(bitnum == SPI_16BIT){
		SPIx->CR1 |= SPI_CR1_DFF;
	}
	SPIx->CR1 |= SPI_CR1_SPE;
}



void SPI_Set_SPI_RCC(SPI_TypeDef *SPIx)
{
	if(SPIx == SPI1){
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}
	else if(SPIx == SPI2){
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	}
}

void SPI_Set_GPIO_RCC(GPIO_TypeDef *GPIOx)
{
	if(GPIOx == GPIOA){
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	}
	else if(GPIOx == GPIOB){
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	}
	else if(GPIOx == GPIOC){
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	}
	else if(GPIOx == GPIOD){
		RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	}
	else if(GPIOx == GPIOE){
		RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	}
}


void SPI_Config(SPI_TypeDef *SPIx, uint8_t CS_Pin, GPIO_TypeDef *CS_Port)
{

	SPI_Set_GPIO_RCC(CS_Port);

	CS_Port->CRL &= ~(GPIO_CRL_MODE0 << (CS_Pin << 2));
	CS_Port->CRL |= (GPIO_CRL_MODE0 << (CS_Pin << 2));
	CS_Port->CRL &= ~(GPIO_CRL_CNF0 << (CS_Pin << 2));



	SPI_Set_SPI_RCC(SPIx);

	SPIx->CR1 = 0;
	SPIx->CR2 = 0;

	SPIx->CR1 |= (SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPHA | SPI_CR1_CPOL);
	SPIx->CR1 |= SPI_CR1_SPE;



}

void SPI_Set_Speed(SPI_TypeDef *SPIx, uint32_t clk)
{
	SPIx->CR1 &= ~(SPI_CR1_BR);
	if(clk >= 50000000)
		SPIx->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2);
	else if(clk >= 24000000 && clk < 50000000)
		SPIx->CR1 |= (SPI_CR1_BR_1 | SPI_CR1_BR_2);
	else if(clk >= 12000000 && clk < 24000000)
		SPIx->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_2);
	else if(clk >= 6000000 && clk < 12000000)
		SPIx->CR1 |= (SPI_CR1_BR_2);
	else
		SPIx->CR1 |= (SPI_CR1_BR_0 | SPI_CR1_BR_1);
}


void SPI_CS_Set(GPIO_TypeDef *port, uint8_t pin)
{
	port->BSRR = 1 << pin;
}


void SPI_CS_Reset(GPIO_TypeDef *port, uint8_t pin)
{
	port->BRR = 1 << pin;
}


SPI_Status_t SPI_Send8Data(SPI_TypeDef * SPIx, uint8_t * data, uint32_t len)
{
	SPI_SetDataSize(SPIx, SPI_8BIT);
	for(uint32_t i = 0; i < len; ++i )
	{
		while(!(SPIx->SR & SPI_SR_TXE));
		  *(__IO uint8_t*)&SPIx->DR = data[i];
		while(!(SPIx->SR & SPI_SR_RXNE));
		 *(__IO uint8_t*)&SPIx->DR;
	}
	while(SPIx->SR & SPI_SR_BSY){};
	return SPI_OK;
}

SPI_Status_t SPI_Receive8Data(SPI_TypeDef * SPIx, uint8_t * data, uint32_t len)
{
	SPI_SetDataSize(SPIx, SPI_8BIT);
	for(uint32_t i = 0; i < len; ++i )
	{
		while(!(SPIx->SR & SPI_SR_TXE));
		*(__IO uint8_t*)&SPIx->DR = 0xFF;
		while(!(SPIx->SR & SPI_SR_RXNE));
		data[i] = *(__IO uint8_t*)&SPIx->DR;
	}
	while(SPIx->SR & SPI_SR_BSY);
	return SPI_OK;
}


SPI_Status_t SPI_Send16Data(SPI_TypeDef * SPIx, uint16_t * data, uint32_t len)
{
	SPI_SetDataSize(SPIx, SPI_16BIT);
	for(uint32_t i = 0; i < len; ++i)
	{
		while(!(SPIx->SR & SPI_SR_TXE));
		uint16_t tmp = (data[i] << 8) | (data[i] >> 8);
		SPIx->DR = tmp;
		while(!(SPIx->SR & SPI_SR_RXNE));
		SPIx->DR;
	}
	while(SPIx->SR & SPI_SR_BSY);
	return SPI_OK;
}

SPI_Status_t SPI_Receive16Data(SPI_TypeDef * SPIx, uint16_t * data, uint32_t len)
{
	SPI_SetDataSize(SPIx, SPI_16BIT);
	for(uint32_t i = 0; i < len; ++i)
	{
		while(!(SPIx->SR & SPI_SR_TXE));
		SPIx->DR = 0xFFFF;
		while(!(SPIx->SR & SPI_SR_RXNE));
		uint16_t tmp = SPIx->DR;
		data[i] = (tmp >> 8) | (tmp << 8);
	}
	while((SPIx->SR & SPI_SR_BSY));
	return SPI_OK;
}


void SPI_Send_Recv(uint8_t *buf_tx, uint8_t *buf_rx, uint16_t len)
{
	SpiActive = 1;

	pBufTx = buf_tx;
	pBufRx = buf_rx;
	usBufPosRx = 0;
	usBufPosTx = 1;
	usBufCnt = len;

	while (!(SPI2->SR & SPI_SR_TXE)); //ожидание установки 1 в TXE(окончание передачи)
	if (pBufTx)
		*(uint8_t*)&SPI2->DR = *pBufTx;
	//_SPI->DR = 5;
	else
		*(uint8_t*)&SPI2->DR = SPI_EMPTY_BYTE;
//	while(SpiActive);
}


void SpiSendRecvFlash(uint8_t **a_buf_tx, uint8_t **a_buf_rx, uint16_t *a_buf_len, uint8_t cnt)
{
			for (uint8_t i = 0; i < cnt; i++)
			{

				SPI_Send_Recv(a_buf_tx[i], a_buf_rx[i], a_buf_len[i]);

				while(SpiActive);
			}
}

void SPI_GPIO_Init(void)
{
    /*Configure SPI GPIO************************************************************************/
    /*You should configure MISO, MOSI and SCK pins here!*/
    RCC->APB2ENR     |=  RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
   // GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
 //   GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
  //  GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7);

    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5);	//sck
    GPIOA->CRL |= (GPIO_CRL_CNF6_0);	//miso
    GPIOA->CRL |= (GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7);	//mosi

    /*
    GPIOC->MODER    &=  ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER11 | GPIO_MODER_MODER12);
    GPIOC->MODER    |=  (GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1);
    GPIOC->OTYPER   &=  ~(GPIO_OTYPER_OT_10 | GPIO_OTYPER_OT_11 | GPIO_OTYPER_OT_12);
    GPIOC->OSPEEDR  |=  (GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12);
    GPIOC->PUPDR    &=  ~(GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR11 | GPIO_PUPDR_PUPDR12);
    GPIOC->PUPDR    |=  (GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0 | GPIO_PUPDR_PUPDR12_0);
*/

    /*********************************************************************************************/
}


static void spi1_gpio_init()
{
	GPIO_InitTypeDef  		GPIO_InitStruct;


	SPI1_RCC_PORT_CLOCK_CMD ( SPI1_RCC_PORT, ENABLE);
	SPI1_RCC_PERIPH_CLOCK_CMD ( SPI1_RCC_APBPORT, ENABLE );



	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SPI1_MOSI_PIN | SPI1_SCK_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SPI1_NSS_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );


	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_AF_PP;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = SPI1_MISO_PIN;
	GPIO_Init ( SPI_GPIO_PORT, &GPIO_InitStruct );


}


void SPI_FLASH_INIT()
{

	SPI_InitTypeDef			SPI_FLASH_InitStruct;

	spi1_gpio_init();

//	SPI1->CR1 &= ~(SPI_CR1_CRCEN);

	/* Initialize the SPI_Direction member */
	SPI_FLASH_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    /* initialize the SPI_Mode member */
	SPI_FLASH_InitStruct.SPI_Mode = SPI_Mode_Master;
	/* initialize the SPI_DataSize member */
	SPI_FLASH_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	/* Initialize the SPI_CPOL member */
	SPI_FLASH_InitStruct.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_High;SPI_CPOL_Low;
	/* Initialize the SPI_CPHA member */
	SPI_FLASH_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	/* Initialize the SPI_NSS member */
	SPI_FLASH_InitStruct.SPI_NSS = SPI_NSS_Hard;//SPI_NSS_Hard;//SPI_NSS_Soft;//SPI_NSS_Hard;
	/* Initialize the SPI_BaudRatePrescaler member */
	SPI_FLASH_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//SPI_BaudRatePrescaler_256;//SPI_BaudRatePrescaler_4;
	/* Initialize the SPI_FirstBit member */
	SPI_FLASH_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//SPI_FirstBit_MSB;//SPI_FirstBit_LSB;

	/* Initialize the SPI_CRCPolynomial member */
	SPI_FLASH_InitStruct.SPI_CRCPolynomial = 7;

	SPI_Init (SPI1,  &SPI_FLASH_InitStruct);
	SPI_CalculateCRC ( SPI1, DISABLE);
	GPIO_WriteBit(SPI1_GPIO_PORT, SPI1_NSS_PIN,1);
	GPIO_WriteBit(SPI1_GPIO_PORT, SPI1_NSS_PIN,0);

	SPI_SSOutputCmd ( SPI1, ENABLE);

	//SPI1->CR1 |= SPI_CR1_SPE;
	SPI1->CR2 |= SPI_CR2_RXNEIE;

	SPI_Cmd(SPI1, ENABLE);

	NVIC_SetPriority(SPI1_IRQn, 0);
	NVIC_EnableIRQ(SPI1_IRQn);

}


void SPI2_IRQHandler(void)
{
	{
		volatile uint8_t ucByte = *(uint8_t*)&SPI2->DR;

		if (pBufRx)
			pBufRx[usBufPosRx] = ucByte;
	}

	{
		if (usBufPosTx < usBufCnt)
		{
			if (pBufTx)
				*(uint8_t*)&SPI2->DR = pBufTx[usBufPosTx];
			else
				*(uint8_t*)&SPI2->DR = SPI_EMPTY_BYTE;

			usBufPosTx++;
		}
	/*	else{
			GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);
		}*/
	}

	if (++usBufPosRx >= usBufCnt)
	{
		SpiActive = 0;
	//	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);
		//SPI_ON_READY_ISR_CB(0);
	}

}



void SPI_FLASH_CONFIG(void)
{
//	NVIC_InitTypeDef		NVIC_InitStruct;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_NSS_PIN,1);
//	SPI2->CR1 &= ~(SPI_CR1_SPE);
//	SPI2->CR1 &= ~(SPI_CR1_LSBFIRST);
//	SPI2->CR2 |= SPI_CR2_RXNEIE;
//	SPI2->CR1 |= SPI_CR1_SPE;


	SPI_InitTypeDef			SPI_FLASH_Initstruct;

	SPI_Cmd(SPI2, DISABLE);
//	NVIC_DisableIRQ(SPI2_IRQn);

	/* Initialize the SPI_Direction member */
	SPI_FLASH_Initstruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
    /* initialize the SPI_Mode member */
	SPI_FLASH_Initstruct.SPI_Mode = SPI_Mode_Master;
	/* initialize the SPI_DataSize member */
	SPI_FLASH_Initstruct.SPI_DataSize = SPI_DataSize_8b;
	/* Initialize the SPI_CPOL member */
	SPI_FLASH_Initstruct.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_High;SPI_CPOL_Low;
	/* Initialize the SPI_CPHA member */
	SPI_FLASH_Initstruct.SPI_CPHA = SPI_CPHA_1Edge;
	/* Initialize the SPI_NSS member */
	SPI_FLASH_Initstruct.SPI_NSS = SPI_NSS_Soft;//SPI_NSS_Hard;//SPI_NSS_Soft;//SPI_NSS_Hard;
	/* Initialize the SPI_BaudRatePrescaler member */
	SPI_FLASH_Initstruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//SPI_BaudRatePrescaler_256;//SPI_BaudRatePrescaler_4;
	/* Initialize the SPI_FirstBit member */
	SPI_FLASH_Initstruct.SPI_FirstBit = SPI_FirstBit_MSB;//SPI_FirstBit_MSB;//SPI_FirstBit_LSB;
	/* Initialize the SPI_CRCPolynomial member */
	SPI_FLASH_Initstruct.SPI_CRCPolynomial = 7;

	SPI_Init (SPI2,  &SPI_FLASH_Initstruct);
	//SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE|SPI_I2S_IT_TXE, ENABLE);

	SPI_CalculateCRC ( SPI2, DISABLE);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_NSS_PIN,1);

	SPI_SSOutputCmd ( SPI2, ENABLE);


/*
	RCC_PLLCmd(DISABLE);
	uint32_t tmpreg = 0;
	tmpreg=RCC->CFGR;
	RCC->CFGR=tmpreg|0x00000500;
	RCC_PLLCmd(ENABLE);
*/
	SPI_I2S_ITConfig( SPI2_NUM, SPI_I2S_IT_RXNE, ENABLE );
	NVIC_SetPriority(SPI2_IRQn, 3);
	NVIC_EnableIRQ(SPI2_IRQn);



	SPI_Cmd(SPI2, ENABLE);

	SpiActive = 0;



/*
	NVIC_PriorityGroupConfig ( NVIC_GROUP );
	NVIC_InitStruct.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init ( &NVIC_InitStruct );
*/


}

void SPI_FLASH_DECONFIG(void){
	SPI2->CR1 &= ~(SPI_CR1_SPE);
	SPI2->CR1 |= SPI_CR1_LSBFIRST;
	SPI2->CR2 &= ~(SPI_CR2_RXNEIE);
	SPI2->CR1 |= SPI_CR1_SPE;
}



