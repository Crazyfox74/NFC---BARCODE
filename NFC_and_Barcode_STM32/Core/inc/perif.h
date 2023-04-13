/*
 * perif.h
 *
 *  Created on: 1 апр. 2023 г.
 *      Author: crazy
 */

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_spi.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifndef INC_PERIF_H_
#define INC_PERIF_H_





#define ST_DEV_ALL_ERR 0x00
#define ST_DEV_NFC_NOERR 0x01
#define ST_DEV_BAR_NOERR 0x10
#define ST_DEV_ALL_NOERR  0x11

#define TIMEOUT_MAX				1000

#define LCD_I2C_ADDRESS			0x4E

#define NVIC_GROUP		NVIC_PriorityGroup_0

#define LCD_I2C_SCL_RCC_PERIPH_CLOCK_CMD RCC_APB2PeriphClockCmd
#define LCD_I2C_SCL_RCC_PORT			RCC_APB2Periph_GPIOB
#define LCD_I2C_SCL_GPIO_PORT			GPIOB
#define LCD_I2C_SCL_PIN					GPIO_Pin_6

#define LCD_I2C_SDA_RCC_PERIPH_CLOCK_CMD RCC_APB2PeriphClockCmd
#define LCD_I2C_SDA_RCC_PORT			RCC_APB2Periph_GPIOB
#define LCD_I2C_SDA_GPIO_PORT			GPIOB
#define LCD_I2C_SDA_PIN					GPIO_Pin_7

#define LCD_I2C_RCC_PERIPH_CLOCK_CMD	RCC_APB1PeriphClockCmd
#define LCD_I2C_RCC_APBPORT				RCC_APB1Periph_I2C1
#define LCD_I2C_NUM						I2C1

#define LCD_RS							0x04
#define LCD_RW							0x01
#define LCD_E							0x02

#define LCD_STR_LEN						8

#define  LCD_CURSOR_1STR				0x80
#define  LCD_CURSOR_2STR				0xC0

//*************************************************************
#define BTN_RCC_PERIPH_CLOCK_CMD		RCC_APB2PeriphClockCmd
#define BTN_RCC_GPIO_PORT				RCC_APB2Periph_GPIOB
#define BTN_GPIO_PORT					GPIOB
#define BTN_PIN							GPIO_Pin_0

#define BTN_EXTI_PORTSOURCE				GPIO_PortSourceGPIOB
#define BTN_EXTI_PINSOURCE				GPIO_PinSource0
#define BTN_EXTI_LINE					EXTI_Line0
#define BTN_IRQ							EXTI0_IRQn

#define BTN_TIMEOUT						1000//500
//*********************************************************
#define LED_RCC_PERIPH_CLOCK_CMD		RCC_APB2PeriphClockCmd
#define LED_RCC_GPIO_PORT				RCC_APB2Periph_GPIOA
#define LED_GPIO_PORT					GPIOA
#define LED_PIN							GPIO_Pin_12

#define LED_EXTI_PORTSOURCE				GPIO_PortSourceGPIOA
#define LED_EXTI_PINSOURCE				GPIO_PinSource12
#define LED_EXTI_LINE					EXTI_Line12
#define LED_IRQ							EXTI0_IRQn

#define LED_FLASH1						50000
#define LED_FLASH2						100000
//**********************************************************
#define SUM_MAX							10000000
#define SUM_STR_LEN						(LCD_STR_LEN - 1)

#define RUN_STR_DELAY_TST				200
#define RUN_STR_DELAY					800
#define STR_CMD_LENGTH					12

#define CNT_LOOP_PN532	50000

#define TIMER1_PRESCALER 7200
//	500=50мс
/// 1000=100мс
//	10000=1мс

#define	TIMER1_RELOAD_PERIOD 500

#define BTN_PRESSED 1
#define BTN_UNPRESSED 2



/********/

#define PA7_RCC_PERIPH_CLOCK_CMD		RCC_APB2PeriphClockCmd
#define PA7_RCC_GPIO_PORT				RCC_APB2Periph_GPIOA
#define PA7_GPIO_PORT					GPIOA
#define PA7_PIN							GPIO_Pin_7



 /**************/

#define FIRMWARE_PAGE_OFFSET 	0xC800

#define flash_addr1 0x0800C800


#define SIZE_CMD 3



#define QUEUE_LEN		64
#define BUFFER_LEN		128

#define DATE_LEN 64

#define USB_STATE_LEN	26



//------------------------------------------------------------------------------
// USART1 Scaner
#define USART_RCC_PERIPH_CLOCK_CMD	RCC_APB2PeriphClockCmd
#define USART_RCC_APBPORT		RCC_APB2Periph_USART1
#define USART_RCC_PORT			RCC_APB2Periph_GPIOA
#define USART_GPIO_PORT			GPIOA
#define USART_TX_PIN			GPIO_Pin_9
#define USART_RX_PIN			GPIO_Pin_10
//#define USART_GPIO_TX_PINSOURCE	GPIO_PinSource9
//#define USART_GPIO_RX_PINSOURCE	GPIO_PinSource10
//#define USART_GPIOAF			GPIO_AF_USART1
#define USART_IRQ				USART1_IRQn
#define USART_NUM				USART1
//------------------------------------------------------------------------------
// USART2 Computer
#define USART2_RCC_PERIPH_CLOCK_CMD	 RCC_APB1PeriphClockCmd
#define USART2_RCC_APBPORT			 RCC_APB1Periph_USART2
#define USART2_RCC_PORT				 RCC_APB1Periph_USART2//RCC_APB2Periph_GPIOA
#define USART2_GPIO_PORT			 GPIOA
#define USART2_TX_PIN				 GPIO_Pin_2
#define USART2_RX_PIN				 GPIO_Pin_3
#define USART2_IRQ					 USART2_IRQn
#define USART2_NUM					 USART2
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// SPI2 NFC
#define SPI_RCC_PERIPH_CLOCK_CMD	RCC_APB1PeriphClockCmd
#define SPI_RCC_PORT_CLOCK_CMD	    RCC_APB2PeriphClockCmd
#define SPI_RCC_AFIO_CLOCK_CMD      RCC_APB2PeriphClockCmd
#define RCC_AFIO 				    RCC_APB2Periph_AFIO

#define SPI_RCC_APBPORT		        RCC_APB1Periph_SPI2
#define SPI_RCC_PORT			    RCC_APB2Periph_GPIOB //RCC_APB1Periph_SPI2//RCC_APB2Periph_GPIOB
#define SPI_GPIO_PORT			    GPIOB
#define SPI_NSS_PIN				    GPIO_Pin_12
#define SPI_SCK_PIN				    GPIO_Pin_13
#define SPI_MISO_PIN			    GPIO_Pin_14
#define SPI_MOSI_PIN			    GPIO_Pin_15

#define SPI_FLASH_NSS				GPIO_Pin_8
#define SPI2_NUM					SPI2



//SPI1 FLASH

#define SPI1_RCC_PERIPH_CLOCK_CMD	RCC_APB2PeriphClockCmd
#define SPI1_RCC_PORT_CLOCK_CMD	    RCC_APB2PeriphClockCmd
#define SPI1_RCC_AFIO_CLOCK_CMD      RCC_APB2PeriphClockCmd

#define SPI1_RCC_APBPORT		        RCC_APB2Periph_SPI1
#define SPI1_RCC_PORT			    RCC_APB2Periph_GPIOA
#define SPI1_GPIO_PORT			    GPIOA
#define SPI1_NSS_PIN				    GPIO_Pin_4
#define SPI1_SCK_PIN				    GPIO_Pin_5
#define SPI1_MISO_PIN			    GPIO_Pin_6
#define SPI1_MOSI_PIN			    GPIO_Pin_7






void LED_Init(void);
void BTN_Init(void);
void BTN_IRQHandler ( void );


void Timer1Init(void);
void EnableTimer1Interrupt();
void DisableTimer1Interrupt();


void PN532_FirmWareVersion2String(uint32_t version);

uint32_t GetTickCount();

void ValueInterrupToStr();

bool fourbytes2strhex(uint8_t *indatarray, uint8_t *outstrhex);


uint32_t UIDResponse(uint8_t *uid,  uint8_t uidLength);

bool BytesHex2Str(uint8_t *indatarray, uint8_t size_array, uint8_t *outstrhex);

void PN532_WriteTagtoHex();

void spi_gpio_init(void);

uint64_t bcode_conv_to_flash(uint32_t rtc_time, char *bar_buff);
void nfc_conv_to_flash(uint32_t rtc_time, uint32_t nfc_tag );
void data_conv2Flash(uint32_t rtc_time, uint64_t data );
void nfc_conv2Flash(uint32_t rtc_time, uint32_t data );
void addr_conv2Flash(uint32_t addr_flash);
void wr_test_prepare(uint32_t addr_wr, uint16_t cnt);
uint32_t flash_conv2_addr(uint8_t *padBuf);

#endif /* INC_PERIF_H_ */
