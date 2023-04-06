/*
 * LCD.c
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#include "LCD.h"
//#include "startup.c"

/* Display initialization commands */
static uint8_t g_arInitDisplayCommands[] = {
		0b0011, 0b0011, 0b0011,
		0b0010, 0b0010, 0b1000,
		//4 bits interface, confirm 4 bits interface
		//N == 1 (2 active lines), F == 0 (matrix 5 x 7 dots)
		0b0000, 0b1000,
		//D == 0, C == 0, B == 0
		//Display OFF
		0b0000, 0b0110,
		//I/D == 1, S == 0
		//shift cursor to right
		0b0000, 0b1100,
		//D == 1, C == 0, B == 0
		//Display ON
};

static int g_iSum = 0;
extern char g_sSum[SUM_STR_LEN + 2];




//******************************************************************
void SCAN_SumToString ( int sum, char * sumStr ) {
	int div = SUM_MAX / 10;
	uint8_t i = 0;

	sum = (sum >= SUM_MAX) ? (SUM_MAX - 1) : sum;
	sum = (sum < 0) ? 0 : sum;

	while (div) {
		sumStr[i++] = sum / div + '0';
		sum %= div;
		div /= 10;
	}
}




void LCD_CtrlLinesConfig(void) {
	GPIO_InitTypeDef		GPIO_InitStruct;
	I2C_InitTypeDef			I2C_InitStruct;

	/*
	 * I2C pins configure
	 */
	LCD_I2C_SCL_RCC_PERIPH_CLOCK_CMD ( LCD_I2C_SCL_RCC_PORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = LCD_I2C_SCL_PIN;
	GPIO_Init ( LCD_I2C_SCL_GPIO_PORT, &GPIO_InitStruct );

	LCD_I2C_SDA_RCC_PERIPH_CLOCK_CMD ( LCD_I2C_SDA_RCC_PORT, ENABLE );

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Pin = LCD_I2C_SDA_PIN;
	GPIO_Init ( LCD_I2C_SDA_GPIO_PORT, &GPIO_InitStruct );

	LCD_I2C_RCC_PERIPH_CLOCK_CMD ( LCD_I2C_RCC_APBPORT, ENABLE );

	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 50000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = 1;
	I2C_Init ( LCD_I2C_NUM, &I2C_InitStruct );

	I2C_Cmd ( LCD_I2C_NUM, ENABLE );
}
//******************************************************************
/**
  * @brief  Writes a byte at a specific LCD register
  * @param  Device: device address
  * @param  Addr: register address
  * @param  Data: data to be written to the specific register
  * @retval 0x00 if write operation is OK
  *         0xFF if timeout condition occured (device not connected or bus error).
  */
uint8_t I2C_SingleRandomWrite ( I2C_TypeDef* I2Cx, uint8_t Device, uint8_t Data ) {
	uint32_t timeout = TIMEOUT_MAX;

	/* Generate the Start Condition */
	I2C_GenerateSTART ( I2Cx, ENABLE );

	/* Test on I2Cx EV5 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_MODE_SELECT )) {
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send TS selected device slave Address for write */
	I2C_Send7bitAddress ( I2Cx, Device, I2C_Direction_Transmitter );

	/* Test on I2Cx EV6 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING )) {
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send Data */
	I2C_SendData ( I2Cx, Data );

	/* Test on I2Cx EV8 and clear it */
	timeout = TIMEOUT_MAX; /* Initialize timeout value */
	while (!I2C_CheckEvent ( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED )) {
		/* If the timeout delay is exceeded, exit with error code */
		if ((timeout--) == 0)
			return 0xFF;
	}

	/* Send I2Cx STOP Condition */
	I2C_GenerateSTOP ( I2Cx, ENABLE );

	/* If operation is OK, return 0 */
	return 0;
}
//******************************************************************
void LCD_SetCursor ( uint8_t index ) {
	uint8_t index_hi = index & 0xF0;
	uint8_t index_lo = (index & 0x0F) << 4;

	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			(index_hi & ~LCD_E) & ~LCD_RS & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			(index_hi | LCD_E) & ~LCD_RS & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			(index_hi & ~LCD_E) & ~LCD_RS & ~LCD_RW );

	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			(index_lo & ~LCD_E) & ~LCD_RS & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			(index_lo | LCD_E) & ~LCD_RS & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			(index_lo & ~LCD_E) & ~LCD_RS & ~LCD_RW );
}
//******************************************************************
void LCD_WriteData ( uint8_t data ) {
	uint8_t data_hi = data & 0xF0;
	uint8_t data_lo = (data & 0x0F) << 4;

	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			((data_hi & ~LCD_E) | LCD_RS) & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			((data_hi | LCD_E) | LCD_RS) & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			((data_hi & ~LCD_E) | LCD_RS) & ~LCD_RW );

	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			((data_lo & ~LCD_E) | LCD_RS) & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			((data_lo | LCD_E)  | LCD_RS) & ~LCD_RW );
	I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
			((data_lo & ~LCD_E) | LCD_RS) & ~LCD_RW );
}
//******************************************************************
void LCD_WriteString ( char * str ) {
	uint8_t i;

	for (i = 0; i < strlen(str); i++) {
		LCD_WriteData ( str[i] );
	}

}
//******************************************************************
//******************************************************************
void LCD_RunStringDevInfo ( char * str1line,char * str2line, uint32_t delay) {
	static uint8_t strFrameIndex = 0;
	static uint32_t tick = 0;
	static char runStr1[100 + 2 * LCD_STR_LEN + 1];
	static char runStr2[100 + 2 * LCD_STR_LEN + 1];
	static uint8_t runStr1Len, runStr2Len;
	char str1Frame[LCD_STR_LEN + 1];
	char str2Frame[LCD_STR_LEN + 1];
	uint8_t i;
	uint8_t cnt;

		tick = GetTickCount();
		strFrameIndex = 0;
		cnt=0;

		/*
		 * Копирование исходной строки в буфер
		 *  с учётом добавления с двух сторон пробелов
		 */
		*runStr1 = '\0';
		*runStr2 = '\0';

		for (i = 0; i < (LCD_STR_LEN - 1); i++) {
			strcat ( runStr1, " " );
			strcat ( runStr2, " " );
		}
		strcat ( runStr1, str1line );
		strcat ( runStr2, str2line );

		for (i = 0; i < (LCD_STR_LEN - 1); i++) {
			strcat ( runStr1, " " );
			strcat ( runStr2, " " );
		}

		runStr1Len = strlen ( runStr1 );
		runStr2Len = strlen ( runStr2 );

	while(cnt<(runStr1Len-1)||cnt<(runStr2Len-1))
		if (GetTickCount() - tick > delay) {
			tick = GetTickCount();
			strFrameIndex++;

			if (cnt<(runStr1Len-1)) {
			for (i = 0; i < LCD_STR_LEN; i++)
				str1Frame[i] = runStr1[(strFrameIndex + i) % runStr1Len];

			LCD_SetCursor ( LCD_CURSOR_1STR );//вывод в первую строку строку
			LCD_WriteString ( str1Frame );
			}

			if (cnt<(runStr2Len-1)){
			for (i = 0; i < LCD_STR_LEN; i++)
				str2Frame[i] = runStr2[(strFrameIndex + i) % runStr2Len];

			LCD_SetCursor ( LCD_CURSOR_2STR );//вывод во вторую строку
			LCD_WriteString ( str2Frame );
			}

			cnt++;
		}

}
//******************************************************************
//******************************************************************
//Отображение кода сканера штрих-кодов бегущей строкой в первой строчке LCD
void LCD_RunStringBARCode ( char * str, uint32_t delay, uint8_t start ) {
	static uint8_t strFrameIndex = 0;
	static uint32_t tick = 0;
	static char runStr[USB_STATE_LEN + 2 * LCD_STR_LEN + 1];
	static uint8_t runStrLen;
	char strFrame[LCD_STR_LEN + 1];
	uint8_t i;

	if (start == 0) {
		tick = GetTickCount();
		strFrameIndex = 0;

		/*
		 * Копирование исходной строки в буфер
		 *  с учётом добавления с двух сторон пробелов
		 */
		*runStr = '\0';

		for (i = 0; i < (LCD_STR_LEN - 1); i++) {
			strcat ( runStr, " " );
		}
		strcat ( runStr, str );
		for (i = 0; i < (LCD_STR_LEN - 1); i++) {
			strcat ( runStr, " " );
		}
		runStrLen = strlen ( runStr );
	}

	if (strlen(str)) {
		if (GetTickCount() - tick > delay) {
			tick = GetTickCount();
			strFrameIndex++;
			for (i = 0; i < LCD_STR_LEN; i++) {
				strFrame[i] = runStr[(strFrameIndex + i) % runStrLen];
			}
			LCD_SetCursor ( LCD_CURSOR_1STR );//вывод в первую строку
			LCD_WriteString ( strFrame );
		}
	}
}
//******************************************************************
//******************************************************************
//Отображение кода сканера NFC бегущей строкой во второй строчке LCD
void LCD_RunStringNFCCode ( char * str, uint32_t delay, uint8_t start ) {
	static uint8_t strFrameIndex = 0;
	static uint32_t tick = 0;
	static char runStr[USB_STATE_LEN + 2 * LCD_STR_LEN + 1];
	static uint8_t runStrLen;
	char strFrame[LCD_STR_LEN + 1];
	uint8_t i;

	if (start == 0) {
		tick = GetTickCount();
		strFrameIndex = 0;

		/*
		 * Копирование исходной строки в буфер
		 *  с учётом добавления с двух сторон пробелов
		 */
		*runStr = '\0';

		for (i = 0; i < (LCD_STR_LEN - 1); i++) {
			strcat ( runStr, " " );
		}
		strcat ( runStr, str );
		for (i = 0; i < (LCD_STR_LEN - 1); i++) {
			strcat ( runStr, " " );
		}
		runStrLen = strlen ( runStr );
	}

	if (strlen(str)) {
		if (GetTickCount() - tick > delay) {
			tick = GetTickCount();
			strFrameIndex++;
			for (i = 0; i < LCD_STR_LEN; i++) {
				strFrame[i] = runStr[(strFrameIndex + i) % runStrLen];
			}
			LCD_SetCursor ( LCD_CURSOR_2STR );//вывод во вторую строку
			LCD_WriteString ( strFrame );
		}
	}
}
//******************************************************************

void LCD_Init(void) {
	uint8_t i;

	LCD_CtrlLinesConfig();

	for (i = 0; i < sizeof(g_arInitDisplayCommands) / sizeof(uint8_t); i++) {
		I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
				((g_arInitDisplayCommands[i] << 4) & ~LCD_E) & ~LCD_RS & ~LCD_RW );
		Delay(10);
		I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
				((g_arInitDisplayCommands[i] << 4) | LCD_E) & ~LCD_RS & ~LCD_RW );
		Delay(10);
		I2C_SingleRandomWrite ( LCD_I2C_NUM, LCD_I2C_ADDRESS,
				((g_arInitDisplayCommands[i] << 4) & ~LCD_E) & ~LCD_RS & ~LCD_RW );
		Delay(10);
	}
	/*
	 * Очистка дисплея
	 */
	LCD_SetCursor ( LCD_CURSOR_1STR );
	LCD_WriteString ( "                   " );
	LCD_SetCursor ( LCD_CURSOR_2STR );
	LCD_WriteString ( "                   " );
	//LCD_WriteString ( "SUM:               " );
	/*
	 * Отображение на дисплее текущей суммы
	 */
	//LCD_DisplaySum();
}
//******************************************************************
void LCD_DisplaySum ( void ) {
	uint8_t i = SUM_STR_LEN - 3;

	SCAN_SumToString ( g_iSum, g_sSum );
	memmove ( g_sSum + SUM_STR_LEN - 1, g_sSum + SUM_STR_LEN - 2, 2 );
	g_sSum[SUM_STR_LEN - 2] = '.';

	do {
		if (*g_sSum == '0') {
			memmove ( g_sSum, g_sSum + 1, SUM_STR_LEN + 1 );
		} else break;
	} while (--i);

	//LCD_SetCursor ( 0xC0 );
	//LCD_WriteString ("*");

	LCD_SetCursor ( 0xC8 - strlen(g_sSum) );
	LCD_WriteString ( g_sSum );
}



