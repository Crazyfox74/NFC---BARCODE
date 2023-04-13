/*
 * FLASH.c
 *
 *  Created on: 5 апр. 2023 г.
 *      Author: crazy
 */

#include "flashUtils.h"

uint32_t flash_addr;
uint32_t addr_start;
uint8_t circle;
uint8_t k;
uint16_t test_flash[128];
uint8_t *p_flash;
uint16_t cnt_flash;

uint16_t stat_reg1;
uint16_t stat_reg2;
uint8_t wr_en;
uint8_t wr_stat_en;



uint8_t CheckMemoryFlash(uint32_t addr_check){

	uint8_t er_check = ERASE_FLAG_ERROR;
	addr_start = addr_check;
	flash_addr = addr_check;
	circle = 0;
	k = 16;
	cnt_flash = 256;
	p_flash = test_flash;

	memset(test_flash, 0x00, sizeof(test_flash));

	for(circle = 0; circle < k; circle++){
		spiFlash_Read(flash_addr, cnt_flash, p_flash);
		for(int i=0; sizeof(test_flash); i++){
			if(test_flash[i] != 0xffff){
				er_check = ERASE_FLAG_OK;
				circle = 16;
				return er_check;
				break;

			} else{
				flash_addr = flash_addr+256;
			}
		}
	}

}


void Set_Started_Address(uint32_t addr, uint16_t cnt_flash, uint8_t* pBuf){

	wr_en = spiFlash_wrtEnbl();
	stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
	stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
	if(stat_reg1!=2){
		wr_stat_en = spiFlash_wrtStatReg();
		stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
		stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
		wr_en = spiFlash_wrtEnbl();
		stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
		stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
					}
	spiFlash_write(addr, cnt_flash, pBuf);
	do {
		stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
	} while (stat_reg1 & 0x01 );


}


void Write_data_2Flash(uint32_t addr, uint16_t cnt_flash, uint8_t* pBuf){



}


void Write_Stat_Reg(){
	wr_en = spiFlash_wrtEnbl();
	stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
	stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
	wr_stat_en = spiFlash_wrtStatReg();
	stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
	stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
}

void Set_Cnt_to_Flash(uint32_t addr, uint16_t cnt_flash, uint8_t* pBuf){

	wr_en = spiFlash_wrtEnbl();
	stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
	stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
		if(stat_reg1!=2){
			wr_stat_en = spiFlash_wrtStatReg();
			stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
			stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
			wr_en = spiFlash_wrtEnbl();
			stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
			stat_reg2 = spiFlash_readStatus(CMD_READ_STATUS_REG2);
						}
	spiFlash_write(addr, cnt_flash, pBuf);
	do {
			stat_reg1 = spiFlash_readStatus(CMD_READ_STATUS_REG1);
	} while (stat_reg1 & 0x01 );
}



