/*
 * spiFlash.h
 *
 *  Created on: 2 апр. 2023 г.
 *      Author: crazy
 */

#ifndef INC_SPIFLASH_H_
#define INC_SPIFLASH_H_

#include "stm32f10x.h"
#include "stddef.h"

#define FLASH_RES_OK				0
#define FLASH_RES_ERROR_BUSY		1
#define FLASH_RES_ERROR_WRITE_ENABLE	2
#define FLASH_RES_ERROR_AGAIN		10

#define CMD_WRITE_ENABLE					0x06
#define CMD_PAGE_PROGRAMM					0x02
#define CMD_RELEASE_POWER_DOWN				0xAB
#define CMD_READ_STATUS_REG1				0x05
#define CMD_READ_STATUS_REG2				0x35
#define CMD_READ_DATA						0x03
#define CMD_ERASE_4KB						0x20
#define CMD_ERASE_64KB						0xd8


void spiFlash_Init(void);
void spiFlash_Deinit(void);
uint8_t spiFlash_readStatus(uint8_t cmd);
void spiFlash_Read(uint32_t dwAddrRead, uint16_t bCnt, uint8_t *pBuf);
uint8_t spiFlash_write( uint32_t dwAddrWrite, uint16_t bCnt, uint8_t* pBuf );
uint8_t spiFlash_eraseSector( uint32_t dwSectAddr );
uint8_t spiFlash_wrtEnbl();

uint32_t spiFlash_readJEDECDesc( void );

#endif /* INC_SPIFLASH_H_ */
