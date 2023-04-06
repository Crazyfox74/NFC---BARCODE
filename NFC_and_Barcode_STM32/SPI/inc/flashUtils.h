/*
 * FLASH.h
 *
 *  Created on: 5 апр. 2023 г.
 *      Author: crazy
 */

#ifndef INC_FLASHUTILS_H_
#define INC_FLASHUTILS_H_

#include "spiFlash.h"
#include <stdio.h>
#include <string.h>


#define ERASE_FLAG_ERROR 	10
#define ERASE_FLAG_OK 		0

uint8_t CheckMemoryFlash(uint32_t addr_check);

void Set_Started_Address(uint32_t addr, uint16_t cnt_flash, uint8_t* pBuf);
void Write_data_2Flash(void);

#endif /* INC_FLASHUTILS_H_ */
