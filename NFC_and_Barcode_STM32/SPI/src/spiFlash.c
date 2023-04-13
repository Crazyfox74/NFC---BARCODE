/*
 * spiFlash.c
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: crazy
 */

#include "spi.h"
#include "spiFlash.h"

#define CMD_WRITE_ENABLE					0x06
#define CMD_PAGE_PROGRAMM					0x02
#define CMD_RELEASE_POWER_DOWN				0xAB
#define CMD_READ_STATUS_REG					0x05
#define CMD_READ_DATA						0x03
#define CMD_ERASE_4KB						0x20
#define CMD_ERASE_64KB						0xd8
#define CMD_WRITE_STATUS_REGISTER			0x01

#define MEM_BUF_CHAIN_LEN					2

static uint8_t *a_buf_tx[MEM_BUF_CHAIN_LEN];
static uint8_t *a_buf_rx[MEM_BUF_CHAIN_LEN];
static uint16_t a_buf_len[MEM_BUF_CHAIN_LEN];
static uint8_t buf_txrx[6];

static uint8_t spi_buf_tx[32];		//буфер для передачи
static uint8_t spi_buf_rx[32];		//буфер для чтения

extern volatile uint8_t SpiActive;


void spiFlash_Read( uint32_t dwAddrRead, uint16_t bCnt, uint8_t* pBuf )
{
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);
	buf_txrx[0] = CMD_READ_DATA;
	buf_txrx[1] = (uint8_t)(dwAddrRead >> 16);
	buf_txrx[2] = (uint8_t)(dwAddrRead >> 8);
	buf_txrx[3] = (uint8_t)(dwAddrRead >> 0);
	a_buf_tx[0] = buf_txrx;
	a_buf_rx[0] = NULL;
	a_buf_len[0] = 4;

	a_buf_tx[1] = NULL;
	a_buf_rx[1] = pBuf;
	a_buf_len[1] = bCnt;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,0);

	SpiSendRecvFlash(a_buf_tx, a_buf_rx, a_buf_len, 2);
	while(SpiActive);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

}

uint8_t spiFlash_write( uint32_t dwAddrWrite, uint16_t bCnt, uint8_t* pBuf )
{
	uint8_t res = FLASH_RES_ERROR_AGAIN;
	uint16_t i;
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);
	buf_txrx[0] = CMD_PAGE_PROGRAMM;
	buf_txrx[1] = (uint8_t)(dwAddrWrite >> 16);
	buf_txrx[2] = (uint8_t)(dwAddrWrite >> 8);
	buf_txrx[3] = (uint8_t)(dwAddrWrite >> 0);
	a_buf_tx[0] = buf_txrx;
	a_buf_rx[0] = NULL;
	a_buf_len[0] = 4;

	a_buf_tx[1] = pBuf;
	a_buf_rx[1] = NULL;
	a_buf_len[1] = bCnt;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,0);

	SpiSendRecvFlash(a_buf_tx, a_buf_rx, a_buf_len, 2);
	while(SpiActive);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	res = FLASH_RES_OK;

	return res;
}

uint8_t spiFlash_readStatus( uint8_t cmd )
{
	uint8_t bRegStatus;
//	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);
	spi_buf_tx[0] = cmd;
	spi_buf_tx[1] = 0xff;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,0);

	SPI_Send_Recv(spi_buf_tx, spi_buf_rx, 2);
	while(SpiActive);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	bRegStatus = spi_buf_rx[1];


	return bRegStatus;
}

uint8_t spiFlash_wrtEnbl()
{
	uint8_t res = FLASH_RES_ERROR_AGAIN;
	//GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	buf_txrx[0] = CMD_WRITE_ENABLE;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,0);
	SPI_Send_Recv(buf_txrx, NULL, 1);
	while(SpiActive);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	res = FLASH_RES_OK;

	return res;
}


uint8_t spiFlash_wrtStatReg()
{
	uint8_t res = FLASH_RES_ERROR_AGAIN;
	//GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	buf_txrx[0] = CMD_WRITE_STATUS_REGISTER;
	buf_txrx[1] = NULL;
	//buf_txrx[2] = 0xff;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,0);
	SPI_Send_Recv(buf_txrx, NULL, 2);
	while(SpiActive);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	res = FLASH_RES_OK;

	return res;
}

uint32_t spiFlash_readJEDECDesc( void )
{
	uint32_t result = 0;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

//	SPI_CS_Set(GPIOA, 4);

	buf_txrx[0] = 0x9F;
	a_buf_tx[0] = buf_txrx;
	a_buf_rx[0] = NULL;
	a_buf_len[0] = 1;

	a_buf_tx[1] = NULL;
	a_buf_rx[1] = (uint8_t*)&result;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,0);
	SpiSendRecvFlash(a_buf_tx, a_buf_rx, a_buf_len, 2);
//	while(SpiActive);
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	return result;
}


uint8_t spiFlash_eraseSector( uint32_t dwSectAddr )
{
	uint8_t res = FLASH_RES_ERROR_AGAIN;

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	buf_txrx[0] = CMD_ERASE_4KB;
	buf_txrx[1] = (uint8_t)(dwSectAddr >> 16);
	buf_txrx[2] = (uint8_t)(dwSectAddr >> 8);
	buf_txrx[3] = (uint8_t)(dwSectAddr >> 0);

	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,0);

	SPI_Send_Recv(buf_txrx, NULL, 4);
	while(SpiActive){};
	GPIO_WriteBit(SPI_GPIO_PORT, SPI_FLASH_NSS,1);

	res = FLASH_RES_OK;

	return res;
}
