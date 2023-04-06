/*
 * main.c
 *
 *  Created on: Apr 1, 2023
 *      Author: crazy
 */

#include "main.h"
#include "nfcpn532.h"
#include "perif.h"
#include "LCD.h"
#include "usart.h"
#include "spi.h"
#include "startup.h"

typedef enum {
	UNPRESSED = 0,
	PRESSED,
} ButtonState_t;

char g_sSum[SUM_STR_LEN + 2];

__IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

ButtonState_t buttonState = UNPRESSED;
char cmd_Version [STR_CMD_LENGTH]      = {0x0A, 0x04, 0x31, 0x00, 0x00, 0x59, 0x52, 0x56, 0x4b, 0xFF, 0xFD, 0x76};
char cmd_TriggerScann [STR_CMD_LENGTH] = {0x0A, 0x04, 0x31, 0x00, 0x00, 0x59, 0x4c, 0x54, 0x4b, 0xFF, 0xFD, 0x7E};
char g_sBuffer [ BUFFER_LEN + 1 ];
int g_iBufferLength = 0;

static char g_sBarCode [ USB_STATE_LEN + 1 ];


static char g_sNfcCode [ USB_STATE_LEN + 1 ];

uint8_t pn532_cmd[PN532_PACKBUFFSIZ];
uint8_t pn532_cmdlen;

uint8_t btnpress;
uint8_t pn532_rddata[PN532_PACKBUFFSIZ];
uint8_t pn532_rdlen;


bool b_getversion=DISABLE;
extern uint8_t ScanerVersion [];
extern uint8_t cntbyteversion;
bool b_firstrddata=DISABLE;

//****bluetooth uart
static char date_sCode [ USB_STATE_LEN + 1 ];
char date_sBuffer [ BUFFER_LEN + 1];
int date_iBufferLength = 0;

char test_buffer [BUFFER_LEN + 1];


extern int flag_usart;


uint8_t nfc_command;
uint8_t nfc_cntcomand;
bool b_wakeup;
bool b_samconfig;
bool b_rertyes;
bool b_discannfc;
bool success;
//uint8_t uidt[] = { 0, 0, 0x0F, 0x1F, 0, 0, 0, 0};	// Buffer to store the returned UID
//uint8_t uidt_2[] = { 0, 0, 0, 0, 0, 0, 0, 0};	// Buffer to store the returned UID
bool b_hextostr;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };	// Buffer to store the returned UID
uint8_t uidhex2pc4b[20] = { '0', 'x', '0', '0', ' ','0', 'x', '0', '0', ' ','0', 'x', '0', '0', ' ','0', 'x', '0', '0', 0x00 };	// Buffer to store the returned UID
//---------------------------------------------------------------------------------------------
uint8_t uidnfcwrdara[] = { '0', '0', '0', '0', '0', '0', '0', '0'};	// массив для записи номера метки nfc
uint8_t u8_cntchar;
uint8_t u8_cntindex;

//--------------------------------------------------
uint8_t outwrdata[]={0,0,0,0};
//--------------------------------------------------------------------------------------------
uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)

uint32_t decuid;
uint32_t cntlooppn532=0;

uint8_t cntbutpress=0;

bool b_1secflg;
uint16_t cnttimer30sec=0;
uint16_t cnttimer1sec=0;
uint16_t cnttimer1=0;
uint8_t previousState=0;
uint8_t u8_btnstate=0;
uint16_t timer1data=0;

uint16_t testtimer1=0;
bool timer1flag;

uint8_t u8_wrnfcstatus=0;
bool b_wrstr1;//флаг выдачи первой строки в режиме записи

uint8_t pn532_rd_stat=0;

uint8_t pn532_wr_stat;






static char s_lcd_hello[]    = " Hello! ";
static char s_lcd_scanner_rdy[]  = "Scanner ";
static char s_lcd_scanner[]  = "Scanner NFC & Barcode";
static char s_lcd_test[]     = "Test... ";
static char s_lcd_err[]      = "error...";
static char s_lcd_ok[]       = "  OK!   ";
static char s_lcd_nfc[]      = "  NFC   ";
static char s_lcd_barcode[]  = "Barcode ";
static char s_lcd_ready[]    = " ready  ";
static char s_lcd_nfc_err[]  = "NFC module:  test error...";
static char s_lcd_bar_err[]  = "Barcode reader module: test error...";
static char s_lcd_clrstr[]   = "                   " ;
static char s_lcd_wrnfc[]      = "Write NFC";

static char s_lcd_barcode_read[]  = "Barcode: ";
static char s_lcd_nfc_read[]      = "NFC: ";

static char s_pc_hello[]    = "Hello!\r\n";
static char s_pc_scanner[]  = "Scanner NFC & Barcode...\r\n";
static char s_pc_scanner_ready[]  = "Scanner NFC & Barcode ready...\r\n";

static char s_pc_test[]     = "Test device wait...\r\n";
static char s_pc_barcode[]     = "Barcode device read code: ";
static char s_pc_err[]      = "test error...";

//static char s_point[1]    = ".";
static char s_newline[2]  = {"\r\n"};
static char s_pn_samconfig_ok[]   = "NFC configuration set\r\n";
static char s_pn_samconfig_err[]  = "NFC configuration error\r\n";

static char s_pn_notfoundcard[]   = "NFC card not found\r\n";
static char s_pn_foundcard[]      = "NFC card found!\r\n";
static char s_pn_uidlength[]      = "UID Length (bytes): ";
static char s_pn_uidvalue[]       = "UID Value (hex): ";
static char s_pn_uidlengthdata[]  = "000";
static char s_pn_uiddecdata[]     = "0000000000";
static char s_pn_uiddecvalue[]    = "UID Value (dec): ";

//***********************************************************global NFC var

static char s_space[1]       = " ";
static char s_pn_point[1]    = ".";
static char s_pn_newline[2]  = {0x0D,0x0A};

char s_pn_nchip[2]     = "  ";
char s_pn_fw1nchip[3]  = "   ";
char s_pn_fw2nchip[3]  = "   ";
char s_pn_suchip[2]    = "  ";

static char s_pn_nfchip[]   = "NFC module: test error...\r\n";
static char s_pn_fchip[]    = "NFC chip PN5";
static char s_pn_wfchip[]   = "Firmware ver. ";
static char s_pn_suppchip[] = "Supports ";

uint8_t pn_chip;
uint8_t pn_fw1chip;
uint8_t pn_fw2chip;
uint8_t pn_suppchip;

char s_pn_info[128] = "";
char s_bar_info[128] = "";

char s_bar_ver_info[42]="";
char s_bar_site_info[42]="";

uint8_t statusDevice;

char b_bounce_value[]="00000";

uint8_t rdDataBlock[16];


//*xxx*yyyy-MM-dd HH:mm:ss

static char s_cal_data[21]="0000-00-00 00:00:00\r\n";
static char s_scan_d_c[64]="";
static char s_nfc_tag_date[64]="";

static char s_pc_PN532_info1[]="NFC read operation correct";
static char s_pc_PN532_info2[]="NFC write operation correct";
static char s_pc_PN532_err1[]="Authentication failed for sector";
static char s_pc_PN532_err2[]="Unable to write to sector";
static char s_pc_PN532_err3[]="Unable to write trailer block of sector";
static char s_pc_PN532_err4[]="Unable to read data block";

uint8_t DataHex2pc[47] = { '0', '0',' ', '0', '0',' ', '0','0',' ', '0', '0',' ', '0', '0',' ','0', '0',' ', '0', '0',' ', '0','0',' ', '0', '0',' ', '0', '0',' ', '0', '0',' ', '0', '0',' ', '0','0',' ', '0', '0',' ', '0', '0',' ','0', '0' };
static char s_pn_data_block0[]    = "Data from block 1: ";



static char sd_error_con[]="Can't connect to SD\r\n";
static char sd_error_wr[]="Can't write to SD\r\n";
static char sd_error_rd[]="Can't read from SD\r\n";

uint32_t flash_test;


uint32_t timer = 138450400+14400;
uint32_t tim;
static char s_cal_sec[2]="00";
static char s_cal_min[2]="00";
static char s_cal_hour[2]="00";

extern char date_InputQueue [ DATE_LEN ];

static char flash_wrbcode[]="*101*\r\n";
static char flash_rdbcode[]="*102*\r\n";
static char flash_wrntag[]="*103*\r\n";
static char flash_rdntag[]="*104*\r\n";
static char b_op_cor[]="Operation correct\r\n";
static char b_op_incor[]="Operation incorrect\r\n";
static char android_no_cmd_error[]="No cmd in message";
static char android_no_such_cmd[]="No such cmd";
static char android_wron_cmd_form[]="Wrong cmd format";

static char incorrect_rtc_year[]="Incorrect year";
static char incorrect_rtc_mon[]="Incorrect month";
static char incorrect_rtc_day[]="Incorrect day";

static char android_set_time[]="Setting time...";
static char android_read_time[]="Reading time...";
static char android_read_settings[]="Reading settings..";

static char android_last_barcode[]="Reading the last barcode...";
static char android_last_nfc[]="Reading the last nfc-tag...";
static char android_last_5records[]="Reading the last 5 records...";

char error_cmd[]="*~*";

char CMD ='*';


int NUM_CMD = 0;

uint32_t jedec_test = 0;

extern volatile uint8_t SpiActive;

void LCD_PN532_CreateRunInfoString(){

	strcpy( s_pn_info, s_pn_fchip);	//копирую первую заготовку в пустую строку
	strcat(s_pn_info, s_pn_nchip);	//копирую в заготовку пробельную строку
	strcat(s_pn_info,s_space );
	strcat(s_pn_info,s_pn_wfchip );
	strcat(s_pn_info, s_pn_fw1nchip);
	strcat(s_pn_info,s_pn_fw2nchip );
	strcat(s_pn_info,s_space );
	strcat(s_pn_info,s_pn_suppchip);
	strcat(s_pn_info,s_pn_suchip);

}


//Сборка сборка бегущей строки с версией ПО сканера штрих-кодов модуля для LCD
void LCD_BAR_CreateRunInfoString(){

//	while (ScanerVersion != NULL) {
//		if (strstr(ScanerVersion, "\r\n") != NULL) {
			/* Выделение лексемы,
			 *  ограниченной справа одним из символов множества "\r\n"
	//		 */
//			strtok(ScanerVersion, "\r\n");
//
	//		char *s_bar_info = ScanerVersion;
//			while (strpbrk(s_bar_info, "\n\xFF") != NULL) {
//				s_bar_info++;
//			}
//		}
//	}

	char *ptr_bar_ver_info=strtok(ScanerVersion,"\r\n");
	strcpy(s_bar_info,ptr_bar_ver_info);
	strcat(s_bar_info,s_space);
	ptr_bar_ver_info=strtok(NULL,"\r\n");
	strcat(s_bar_info,ptr_bar_ver_info);

//	char *s_bar_info=strcat(s_bar_ver_info,s_bar_site_info);
//	strcpy(s_bar_info,s_bar_ver_info);


}


uint8_t PN532_Write_Data(uint8_t *uid_card, uint8_t uid_card_len, uint8_t blockNumber, uint8_t *data){
//	uint8_t success=0;	// Flag to check if there was an error with the PN532
	uint8_t KEY_DEFAULT_KEYAB[6]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	// The default Mifare Classic key

	uint8_t res_excep;

	bool authenticated = FALSE;
	uint8_t blockBuffer[16];                  // Buffer to store block contents
	uint8_t blankAccessBits[3] = { 0xff, 0x07, 0x80 };
	//uint8_t idx = 0;
	uint8_t numOfSector = 16;                 // Assume Mifare Classic 1K for now (16 4-block sectors)
    uint8_t i=0;

    uint8_t RdWrDataBlock[16];


	        if (!(PN532_mifareclassic_AuthenticateBlock (uid_card, uid_card_len, 0, 1, (uint8_t *)KEY_DEFAULT_KEYAB)))
	        {
	          return res_excep=1;	//выход из-за ошибки аутентификации блока
	        }

	        res_excep=PN532_mifareclassic_ReadDataBlock(1, RdWrDataBlock);
	        if (!(PN532_mifareclassic_ReadDataBlock(1, RdWrDataBlock)))
	        {
	          return res_excep=2;	//выход из-за ошибки чтения блока
	        }
	        else{

	    		  for(i=0;i<4;i++){
	    			  RdWrDataBlock[i]=data[i];
	    		}



	    		  if(!(PN532_mifareclassic_WriteDataBlock(1,RdWrDataBlock))){
	    			  return res_excep=3;	//выход из-за ошибки запииси блока
	    		  }

	        // Step 3: Reset both keys to 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
	        memcpy(blockBuffer, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));
	        memcpy(blockBuffer + 6, blankAccessBits, sizeof(blankAccessBits));
	        blockBuffer[9] = 0x69;
	        memcpy(blockBuffer + 10, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));

	         //Step 4: Write the trailer block
	        if (!(PN532_mifareclassic_WriteDataBlock(3, blockBuffer)))
	        {
	          return res_excep=4;	//выход из-за ошибки записи ключа блока
	        }else return res_excep=0;

	        }

}






int main(void){

	g_sSum[SUM_STR_LEN + 1] = '\0';


	rtc_cal rtc_time;
	RTC_INIT();

	RCC_PLLCmd(DISABLE);
		RCC_HSEConfig(RCC_HSE_ON);
		RCC_ClockSecuritySystemCmd(ENABLE);
	    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		uint32_t tmpreg = 0;
		tmpreg=RCC->CFGR;
		RCC->CFGR=tmpreg & 0xFFFFF0FF;
		//RCC->CFGR=tmpreg|0x00000400;//HCLK / 2 - значение по умолчанию? (36МГц)
		RCC->CFGR=tmpreg|0x00000500;//HCLK / 4

		RCC->CFGR|=RCC_CFGR_USBPRE;	//предделитель для usb

		//RCC->CFGR=tmpreg | 0x00000600;//HCLK / 8

	    RCC_PLLCmd(ENABLE);

		/* SysTick end of count event each 1 ms */
		RCC_GetClocksFreq ( &RCC_Clocks );
		SysTick_Config ( RCC_Clocks.HCLK_Frequency / 1000 );

		btnpress=0;

		RTC_SET_COUNTER(timer);


		 LCD_Init();
		BTN_Init();
		u8_btnstate=GPIO_ReadInputDataBit ( BTN_GPIO_PORT, BTN_PIN );
		previousState=u8_btnstate;
		 LED_Init();

		 spi_gpio_init();

		   Usart2Init();
			SCAN_UsartInit();
			PN532_SPIInit();

		//flash test

	//		SPI_FLASH_INIT();
	//		jedec_test = spiFlash_readJEDECDesc();



			nfc_cntcomand=0;


			statusDevice=ST_DEV_ALL_ERR;

				uint32_t buttonTick = GetTickCount();
				uint32_t ledTick1 = GetTickCount();
				uint32_t ledTick2 = ledTick1;

				uint32_t versiondata;
				uint32_t st;


				Usart2_SendData(s_pc_hello,strlen(s_pc_hello));
				Usart2_SendData(s_pc_scanner,strlen(s_pc_scanner));
				Usart2_SendData(s_pc_test,strlen(s_pc_test));


			LCD_RunStringDevInfo ( s_lcd_hello,s_lcd_scanner,RUN_STR_DELAY_TST);


			while(SpiActive !=0 && SPI_I2S_GetFlagStatus(SPI2,SPI_SR_BSY) != RESET){}
				SpiActive = 1;
			b_wakeup=PN532_WakeUp();
			b_samconfig=PN532_SAMConfig();

			versiondata = PN532_getFirmwareVersion();//Получениe версии ПО PN532

				if(versiondata){
					statusDevice=statusDevice|ST_DEV_NFC_NOERR;//установка флага успешного теста NFC
					PN532_FirmWareVersion2String (versiondata);
					b_rertyes=PN532_setPassiveActivationRetries(0xFF);
					b_discannfc=0;
				}
				 while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET) {};
				SpiActive = 0;




				while(SpiActive !=0 && SPI_I2S_GetFlagStatus(SPI2,SPI_SR_BSY) != RESET){}
				SPI_FLASH_CONFIG();
				jedec_test = spiFlash_readJEDECDesc();



				SpiActive = 0;








			while(SpiActive !=0 && SPI_I2S_GetFlagStatus(SPI2,SPI_SR_BSY) != RESET){}
				PN532_SPIInit();


		/*		versiondata = 0;
				versiondata = PN532_getFirmwareVersion();//Получениe версии ПО PN532

						if(versiondata){
							statusDevice=statusDevice|ST_DEV_NFC_NOERR;//установка флага успешного теста NFC
							PN532_FirmWareVersion2String (versiondata);
							b_rertyes=PN532_setPassiveActivationRetries(0xFF);
							b_discannfc=0;
						}
*/

				 b_getversion=ENABLE;
				 cntbyteversion=0;

				 SCAN_SendCMD(cmd_Version,STR_CMD_LENGTH);//Запрос версии  ПО сканера штрих-кодов

				uint32_t tick1 = 0;
				uint32_t tick2 = 0;

					tick1=GetTickCount();
					tick2=tick1;
				while(b_getversion || (tick2 - tick1 > 1000)){
						// b_getversion=ENABLE;
						tick2=GetTickCount();
					}

					if(!b_getversion){//Получена версия сканера штрих-кодов
						statusDevice=statusDevice|ST_DEV_BAR_NOERR;//установка флага успешного теста сканера штрих-кодов
					}



switch(statusDevice){
	case ST_DEV_ALL_ERR://Оба устройства не ответили

		Usart2_SendData(s_lcd_bar_err,strlen(s_pc_barcode));
		Usart2_SendData(s_newline,strlen(s_newline));

		Usart2_SendData(s_lcd_nfc_err,sizeof(s_pn_nfchip));
		Usart2_SendData(s_newline,strlen(s_newline));

		LCD_RunStringDevInfo ( s_lcd_bar_err,s_lcd_nfc_err,RUN_STR_DELAY_TST);

	break;

	case ST_DEV_NFC_NOERR://Ответил только NFC

		Usart2_SendData(s_lcd_bar_err,strlen(s_pc_barcode));
		Usart2_SendData(s_newline,strlen(s_newline));

	    Usart2_SendData(s_pn_fchip,strlen(s_pn_fchip));
	    Usart2_SendData(s_pn_nchip,sizeof(s_pn_nchip));
		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));

		Usart2_SendData(s_pn_wfchip,strlen(s_pn_wfchip));
		Usart2_SendData(s_pn_fw1nchip,sizeof(s_pn_fw1nchip));
		Usart2_SendData(s_pn_point,strlen(s_pn_point));
		Usart2_SendData(s_pn_fw2nchip,sizeof(s_pn_fw2nchip));
		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));

		Usart2_SendData(s_pn_suppchip,sizeof(s_pn_suppchip));
		Usart2_SendData(s_pn_suchip,sizeof(s_pn_suchip));
		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));


		LCD_PN532_CreateRunInfoString();
		LCD_RunStringDevInfo ( s_lcd_bar_err,s_pn_info,RUN_STR_DELAY_TST);

	break;

	case ST_DEV_BAR_NOERR://Ответил только сканер штрих-кодов

		Usart2_SendData(s_pc_barcode,strlen(s_pc_barcode));
		Usart2_SendData(ScanerVersion,strlen(ScanerVersion));
		Usart2_SendData(s_newline,strlen(s_newline));

		Usart2_SendData(s_lcd_nfc_err,sizeof(s_pn_nfchip));
		Usart2_SendData(s_newline,strlen(s_newline));

		LCD_BAR_CreateRunInfoString();
		LCD_RunStringDevInfo ( s_bar_info,s_lcd_nfc_err,RUN_STR_DELAY_TST);

//		Usart2_SendData(s_bar_info,strlen(s_bar_info));
//		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));

	break;

	case ST_DEV_ALL_NOERR://Оба устройства ответили

		Usart2_SendData(s_pc_barcode,strlen(s_pc_barcode));
		Usart2_SendData(ScanerVersion,strlen(ScanerVersion));
		Usart2_SendData(s_newline,strlen(s_newline));

	    Usart2_SendData(s_pn_fchip,strlen(s_pn_fchip));
	    Usart2_SendData(s_pn_nchip,sizeof(s_pn_nchip));
		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));

		Usart2_SendData(s_pn_wfchip,strlen(s_pn_wfchip));
		Usart2_SendData(s_pn_fw1nchip,sizeof(s_pn_fw1nchip));
		Usart2_SendData(s_pn_point,strlen(s_pn_point));
		Usart2_SendData(s_pn_fw2nchip,sizeof(s_pn_fw2nchip));
		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));

		Usart2_SendData(s_pn_suppchip,sizeof(s_pn_suppchip));
		Usart2_SendData(s_pn_suchip,sizeof(s_pn_suchip));
		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));


		LCD_BAR_CreateRunInfoString();
		LCD_PN532_CreateRunInfoString();
		LCD_RunStringDevInfo ( s_bar_info,s_pn_info,RUN_STR_DELAY_TST);

//		Usart2_SendData(s_bar_info,strlen(s_bar_info));
//		Usart2_SendData(s_pn_newline,sizeof(s_pn_newline));

		LCD_SetCursor ( LCD_CURSOR_1STR );
		LCD_WriteString (s_lcd_scanner_rdy );

		LCD_SetCursor ( LCD_CURSOR_2STR );
		LCD_WriteString (s_lcd_ready );

		Usart2_SendData(s_pc_scanner_ready,sizeof(s_pc_scanner_ready));


	break;

}

Usart2_SendData(s_pc_scanner_ready,sizeof(s_pc_scanner_ready));


Timer1Init();
EnableTimer1Interrupt();


	while(1){


	/*
		GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 0);
		GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 1);
*/

		char cSymbol;

		char date_Symbol;

		if(timer1flag==1){
			ValueInterrupToStr();
			Usart2_SendData(b_bounce_value,sizeof(b_bounce_value));
			Usart2_SendData(s_newline,strlen(s_newline));
			timer1flag=0;
		}



		//если не режим записи метки NFC
		if(u8_wrnfcstatus==0){

				while (FIFO_GetNextData ( &cSymbol ) == SUCCESS) {
					g_sBuffer [ g_iBufferLength++ ] = cSymbol;
					g_sBuffer [ g_iBufferLength ] = '\0';
					if ( g_iBufferLength >= BUFFER_LEN ) {
						memmove ( g_sBuffer, g_sBuffer + 1, BUFFER_LEN );
						g_iBufferLength--;
					}
				}
		//	прием данных с телефона
				while (Date_GetNextDate(&date_Symbol) == SUCCESS){
				//	GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 1);

				//	Usart2_SendData(s_pc_hello,strlen(s_pc_hello));
				//	Usart2_SendData(s_newline,strlen(s_newline));


					date_sBuffer [ date_iBufferLength++ ] = date_Symbol;
					date_sBuffer [ date_iBufferLength] = '\0';
					if( date_iBufferLength >= BUFFER_LEN){
						memmove(date_sBuffer, date_sBuffer + 1, BUFFER_LEN);
						date_iBufferLength--;
					}
						Usart2_SendData(date_sBuffer,strlen(date_sBuffer));
						Usart2_SendData(s_newline,strlen(s_newline));

				}
		/*
		if(date_InputQueue [ DATE_LEN ] !=NULL){
			Usart2_SendData(s_pc_hello,strlen(s_pc_hello));
			Usart2_SendData(s_newline,strlen(s_newline));
		}
		*/



		//	декодирование данных с телефона
				if(strstr(date_sBuffer, "\r\n") != NULL){

				//	Usart2_SendData(date_sBuffer,strlen(date_sBuffer));
				//	Usart2_SendData(s_newline,strlen(s_newline));

					char *pstr = date_sBuffer;

					if( date_sBuffer[0] == CMD /*&& date_sBuffer[4] == CMD*//**pstr == CMD*/ /*&& *pstr+4 == CMD*/){
						pstr++;

						for(int i = 0; i < SIZE_CMD ; i++){
							test_buffer[i] = *pstr++;

						}
						NUM_CMD = atoi(test_buffer);
						if(NUM_CMD == 0){
							Usart2_SendData(android_wron_cmd_form,strlen(android_wron_cmd_form));
							Usart2_SendData(s_newline,strlen(s_newline));
						}
						switch (NUM_CMD) {
							case 101:

								Usart2_SendData(test_buffer,strlen(test_buffer));
								Usart2_SendData(s_newline,strlen(s_newline));

								uint8_t date_result;

								date_result = convert_time_from_android(&rtc_time, date_sBuffer);
								switch(date_result){
								case 0:
									tim = cal_to_timer(&rtc_time);
									RTC_SET_COUNTER(tim);
									Usart2_SendData(s_lcd_ok,strlen(s_lcd_ok));
									Usart2_SendData(s_newline,strlen(s_newline));
									break;
								case 1:
									Usart2_SendData(incorrect_rtc_year,strlen(incorrect_rtc_year));
									Usart2_SendData(s_newline,strlen(s_newline));
									break;
								case 2:
									Usart2_SendData(incorrect_rtc_mon,strlen(incorrect_rtc_mon));
									Usart2_SendData(s_newline,strlen(s_newline));
									break;
								case 3:
									Usart2_SendData(incorrect_rtc_day,strlen(incorrect_rtc_day));
									Usart2_SendData(s_newline,strlen(s_newline));
									break;

								}
								break;
							case 102:

								Usart2_SendData(test_buffer,strlen(test_buffer));
								Usart2_SendData(s_newline,strlen(s_newline));

								timer = RTC_GET_COUNTER();
								timer_to_cal(timer, &rtc_time);
								tim = cal_to_timer(&rtc_time);


								//*xxx*yyyy-MM-dd HH:mm:ss

								s_cal_data[0] = 0x30 + (rtc_time.year/1000%10);
								s_cal_data[1] = 0x30 + (rtc_time.year/100%10);
								s_cal_data[2] = 0x30 + (rtc_time.year/10%10);
								s_cal_data[3] = 0x30 + (rtc_time.year%10);

								s_cal_data[5] = 0x30 + (rtc_time.mon/10);
								s_cal_data[6] = 0x30 + (rtc_time.mon%10);

								s_cal_data[8] = 0x30 + (rtc_time.mday/10);
								s_cal_data[9] = 0x30 + (rtc_time.mday%10);

								s_cal_data[11] = 0x30 + (rtc_time.hour/10);
								s_cal_data[12] = 0x30 + (rtc_time.hour%10);

								s_cal_data[14] = 0x30 + (rtc_time.min/10);
								s_cal_data[15] = 0x30 + (rtc_time.min%10);

								s_cal_data[17] = 0x30 + (rtc_time.sec/10);
								s_cal_data[18] = 0x30 + (rtc_time.sec%10);

								Usart2_SendData(s_cal_data,strlen(s_cal_data));
								Usart2_SendData(s_newline,strlen(s_newline));


								Usart2_SendData(s_lcd_ok,strlen(s_lcd_ok));
								Usart2_SendData(s_newline,strlen(s_newline));
								break;
							case 103://отправка настроек устройства

								break;
							case 201://чтение последнего штрих-кода
								Usart2_SendData(android_last_barcode,strlen(android_last_barcode));
								Usart2_SendData(s_newline,strlen(s_newline));
								break;
							case 202://чтение последней NFC-метки
								Usart2_SendData(android_last_nfc,strlen(android_last_nfc));
								Usart2_SendData(s_newline,strlen(s_newline));
								break;
							case 203://чтение последних пяти записей
								Usart2_SendData(android_last_5records,strlen(android_last_5records));
								Usart2_SendData(s_newline,strlen(s_newline));
								break;
							default:
								Usart2_SendData(android_no_such_cmd,strlen(android_no_such_cmd));
								Usart2_SendData(s_newline,strlen(s_newline));
								break;
						}
					} else {
						Usart2_SendData(android_no_cmd_error,strlen(android_no_cmd_error));
						Usart2_SendData(s_newline,strlen(s_newline));

					}

				//	strtok(date_sBuffer, "\r\n");

				//	char *dBuffer = date_sBuffer;

				//	strtok(date_sBuffer, "\r\n");

				/*	while (strpbrk ( dBuffer, "\n\xFF" ) != NULL) {
						dBuffer++;
					}
*/
			/*		if(strcmp(flash_bcode, date_sBuffer) == 0){

								} else {

								}
		*/
					//memset(date_sCode,0,strlen(date_sCode));
					//strlcpy ( date_sCode, (const char *)s_lcd_barcode_read, USB_STATE_LEN );
					//strcat(date_sCode,dBuffer );

				//	Usart2_SendData(s_pc_barcode,strlen(s_pc_barcode));
				//	Usart2_SendData(s_newline,strlen(s_newline));

					Usart2_SendData(date_sBuffer,strlen(date_sBuffer));
					Usart2_SendData(s_newline,strlen(s_newline));



					date_iBufferLength=0;
				//	int strDateLength = strlen(date_sBuffer) + 1;
				//	date_iBufferLength -= strDateLength;
				/*	memmove ( date_sBuffer, date_sBuffer + strDateLength,

							BUFFER_LEN +1 - strDateLength );
*/
					memset(date_sBuffer, 0, strlen(date_sBuffer));
				}

				//GPIO_WriteBit(LED_GPIO_PORT, LED_PIN, 0);

				/*
				 * Декодирование данных сканера
				 */
				if (strstr(g_sBuffer, "\r\n") != NULL) {
					/* Выделение лексемы,
					 *  ограниченной справа одним из символов множества "\r\n"
					 */
					strtok ( g_sBuffer, "\r\n" );

					char * pBuffer = g_sBuffer;
					while (strpbrk ( pBuffer, "\n\xFF" ) != NULL) {
						pBuffer++;
					}

					memset(g_sBarCode,0,strlen(g_sBarCode));
					  strlcpy ( g_sBarCode, (const char *)s_lcd_barcode_read, USB_STATE_LEN );
					  strcat(g_sBarCode,pBuffer );
					//strlcpy ( g_sBarCode, (const char *)pBuffer, USB_STATE_LEN );
		/*
					Usart2_SendData(s_pc_barcode,strlen(s_pc_barcode));
					Usart2_SendData(s_newline,strlen(s_newline));
					Usart2_SendData(g_sBarCode,strlen(g_sBarCode));
					Usart2_SendData(s_newline,strlen(s_newline));
		*/
					//LCD_SetCursor ( LCD_CURSOR_1STR );
					//LCD_WriteString ( s_lcd_barcode );

					//if(b_firstrddata==DISABLE){
						LCD_SetCursor ( LCD_CURSOR_1STR );
						LCD_WriteString ( s_lcd_clrstr );
						LCD_SetCursor ( LCD_CURSOR_2STR );
						LCD_WriteString ( s_lcd_clrstr );
						//b_firstrddata=ENABLE;

					//}

					LCD_RunStringBARCode ( g_sBarCode, RUN_STR_DELAY, 0 );

					int strLength = strlen(g_sBuffer) + 1;
					g_iBufferLength -= strLength;
					memmove ( g_sBuffer, g_sBuffer + strLength,

							BUFFER_LEN + 1 - strLength );



					timer = RTC_GET_COUNTER();
					timer_to_cal(timer, &rtc_time);
					tim = cal_to_timer(&rtc_time);


		/*
					s_cal_sec[0] = 0x30 + (unix_time.sec/10);
					s_cal_sec[1] = 0x30 + (unix_time.sec%10);
					s_cal_min[0] = 0x30 + (unix_time.min/10);
					s_cal_min[1] = 0x30 + (unix_time.min%10);
					s_cal_hour[0] = 0x30 + (unix_time.hour/10);
					s_cal_hour[1] = 0x30 + (unix_time.hour%10);
		*/

					//*xxx*yyyy-MM-dd HH:mm:ss

					s_cal_data[0] = 0x30 + (rtc_time.year/1000%10);
					s_cal_data[1] = 0x30 + (rtc_time.year/100%10);
					s_cal_data[2] = 0x30 + (rtc_time.year/10%10);
					s_cal_data[3] = 0x30 + (rtc_time.year%10);

					s_cal_data[5] = 0x30 + (rtc_time.mon/10);
					s_cal_data[6] = 0x30 + (rtc_time.mon%10);

					s_cal_data[8] = 0x30 + (rtc_time.mday/10);
					s_cal_data[9] = 0x30 + (rtc_time.mday%10);

					s_cal_data[11] = 0x30 + (rtc_time.hour/10);
					s_cal_data[12] = 0x30 + (rtc_time.hour%10);

					s_cal_data[14] = 0x30 + (rtc_time.min/10);
					s_cal_data[15] = 0x30 + (rtc_time.min%10);

					s_cal_data[17] = 0x30 + (rtc_time.sec/10);
					s_cal_data[18] = 0x30 + (rtc_time.sec%10);


					strncat(s_scan_d_c, s_cal_data, 19);
					strncat(s_scan_d_c, s_space, strlen(s_space));
					strncat(s_scan_d_c, g_sBarCode, strlen(g_sBarCode));
					strncat(s_scan_d_c, s_newline, strlen(s_newline));



				//	Usart2_SendData(s_cal_data,strlen(s_cal_data));
					Usart2_SendData(s_scan_d_c,strlen(s_scan_d_c));
					Usart2_SendData(s_newline,strlen(s_newline));

					memset(s_scan_d_c, 0, strlen(s_scan_d_c));
				}



				//*****************************************************************************
				//Если тест NFC пройден успешно,то сканируем ID
						if(statusDevice==ST_DEV_NFC_NOERR || statusDevice==ST_DEV_ALL_NOERR ){
							if(cntlooppn532==CNT_LOOP_PN532){

								success=PN532_readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength,1500);

								if(success && !b_discannfc){

									b_discannfc=1;

									s_pn_uidlengthdata[0] = 0x30+(uidLength/100);
									s_pn_uidlengthdata[1] = 0x30+(uidLength/10%10);
									s_pn_uidlengthdata[2] = 0x30+(uidLength%10);

									Usart2_SendData(s_pn_foundcard,strlen(s_pn_foundcard));

									Usart2_SendData(s_pn_uidlength,strlen(s_pn_uidlength));
									Usart2_SendData(s_pn_uidlengthdata,strlen(s_pn_uidlengthdata));
									Usart2_SendData(s_newline,strlen(s_newline));

									b_hextostr = fourbytes2strhex (&uid[0], &uidhex2pc4b[0]);

									Usart2_SendData(s_pn_uidvalue,strlen(s_pn_uidvalue));
									Usart2_SendData(uidhex2pc4b,strlen(uidhex2pc4b));
									Usart2_SendData(s_newline,strlen(s_newline));

									decuid = UIDResponse(&uid[0], uidLength);

									s_pn_uiddecdata[0] = 0x30+(decuid/1000000000);
									s_pn_uiddecdata[1] = 0x30+(decuid/100000000%10);
									s_pn_uiddecdata[2] = 0x30+(decuid/10000000%10);
									s_pn_uiddecdata[3] = 0x30+(decuid/1000000%10);
									s_pn_uiddecdata[4] = 0x30+(decuid/100000%10);
									s_pn_uiddecdata[5] = 0x30+(decuid/10000%10);
									s_pn_uiddecdata[6] = 0x30+(decuid/1000%10);
									s_pn_uiddecdata[7] = 0x30+(decuid/100%10);
									s_pn_uiddecdata[8] = 0x30+(decuid/10%10);
									s_pn_uiddecdata[9] = 0x30+(decuid%10);

									Usart2_SendData(s_pn_uiddecvalue,strlen(s_pn_uiddecvalue));
									Usart2_SendData(s_pn_uiddecdata,strlen(s_pn_uiddecdata));
									Usart2_SendData(s_newline,strlen(s_newline));

									//memset(g_sBarCode,0,strlen(g_sBarCode));
									//strlcpy ( g_sBarCode, (const char *)s_pn_uiddecdata,strlen(s_pn_uiddecdata)+1 );


									//if(b_firstrddata==DISABLE){
										LCD_SetCursor ( LCD_CURSOR_1STR );
										LCD_WriteString ( s_lcd_clrstr );
										LCD_SetCursor ( LCD_CURSOR_2STR );
										LCD_WriteString ( s_lcd_clrstr );
										//b_firstrddata=ENABLE;

									//}

									memset(g_sNfcCode,0,strlen(g_sNfcCode));

									  strlcpy ( g_sNfcCode, (const char *)s_lcd_nfc_read, USB_STATE_LEN );
									  strcat(g_sNfcCode, s_pn_uiddecdata );

									//strlcpy ( g_sNfcCode, (const char *)s_pn_uiddecdata,strlen(s_pn_uiddecdata)+1 );

									//LCD_SetCursor ( LCD_CURSOR_1STR );
									//LCD_WriteString ( s_lcd_nfc );
									LCD_RunStringNFCCode ( g_sNfcCode, RUN_STR_DELAY, 0 );


								//	PN532_WriteID();
									pn532_rd_stat=PN532_Read_Data(uid,uidLength,1,  rdDataBlock);

									switch(pn532_rd_stat){
									case 0:
										Usart2_SendData(s_pc_PN532_info1,strlen(s_pc_PN532_info1));
										Usart2_SendData(s_newline,strlen(s_newline));
										break;
									case 1:
										Usart2_SendData(s_pc_PN532_err1,strlen(s_pc_PN532_err1));
										Usart2_SendData(s_newline,strlen(s_newline));
										break;
									case 2:
										Usart2_SendData(s_pc_PN532_err4,strlen(s_pc_PN532_err4));
										Usart2_SendData(s_newline,strlen(s_newline));
										break;
									}


									/*
									if(pn532_rd_stat==1){
										Usart2_SendData(s_pc_PN532_err1,strlen(s_pc_PN532_err1));
										Usart2_SendData(s_newline,strlen(s_newline));
									}
								*/

									Usart2_SendData(s_pn_data_block0,strlen(s_pn_data_block0));


									//if(pn532_rd_stat==0){
									BytesHex2Str(rdDataBlock, sizeof(rdDataBlock), DataHex2pc);
										Usart2_SendData(DataHex2pc,sizeof(DataHex2pc));
										Usart2_SendData(s_newline,strlen(s_newline));

									//}
										timer = RTC_GET_COUNTER();
										timer_to_cal(timer, &rtc_time);
										tim = cal_to_timer(&rtc_time);

										//*xxx*yyyy-MM-dd HH:mm:ss

										s_cal_data[0] = 0x30 + (rtc_time.year/1000%10);
										s_cal_data[1] = 0x30 + (rtc_time.year/100%10);
										s_cal_data[2] = 0x30 + (rtc_time.year/10%10);
										s_cal_data[3] = 0x30 + (rtc_time.year%10);

										s_cal_data[5] = 0x30 + (rtc_time.mon/10);
										s_cal_data[6] = 0x30 + (rtc_time.mon%10);

										s_cal_data[8] = 0x30 + (rtc_time.mday/10);
										s_cal_data[9] = 0x30 + (rtc_time.mday%10);

										s_cal_data[11] = 0x30 + (rtc_time.hour/10);
										s_cal_data[12] = 0x30 + (rtc_time.hour%10);

										s_cal_data[14] = 0x30 + (rtc_time.min/10);
										s_cal_data[15] = 0x30 + (rtc_time.min%10);

										s_cal_data[17] = 0x30 + (rtc_time.sec/10);
										s_cal_data[18] = 0x30 + (rtc_time.sec%10);

										strncat(s_nfc_tag_date, s_cal_data, 19);
										strncat(s_nfc_tag_date, s_space, strlen(s_space));
										strncat(s_nfc_tag_date, g_sNfcCode, strlen(g_sNfcCode));
										strncat(s_nfc_tag_date, s_newline, strlen(s_newline));



									//	Usart2_SendData(s_cal_data,strlen(s_cal_data));
										Usart2_SendData(s_nfc_tag_date,strlen(s_nfc_tag_date));
										Usart2_SendData(s_newline,strlen(s_newline));

										memset(s_nfc_tag_date, 0, strlen(s_nfc_tag_date));

								}
								if(!success)
								{

									//Usart2_SendData(s_pn_notfoundcard,sizeof(s_pn_notfoundcard));
									b_discannfc=0;

								}

								cntlooppn532=0;
							}//конец if(cntlooppn532==CNT_LOOP_PN532)

							cntlooppn532++;

							LCD_RunStringNFCCode ( g_sNfcCode, RUN_STR_DELAY, 1 );

						}//конец if(statusDevice==ST_DEV_NFC_NOERR || statusDevice==ST_DEV_ALL_NOERR )
				//******************************************************************************




		}//конец если не режим записи метки NFC
		else{ //если режим записи метки NFC


			if(!b_1secflg){
				LCD_SetCursor ( LCD_CURSOR_2STR );
				LCD_WriteString (uidnfcwrdara );

			}
			else{
				LCD_SetCursor ( LCD_CURSOR_2STR+u8_cntindex );
				LCD_WriteString (s_space);

			}
		}


		if(u8_wrnfcstatus!=0 && b_wrstr1==TRUE){
			   memset(g_sBarCode,0,strlen(g_sBarCode));
			  strlcpy ( g_sBarCode, (const char *)s_lcd_wrnfc, USB_STATE_LEN );
			  LCD_RunStringBARCode ( g_sBarCode, RUN_STR_DELAY, 0 );
			  b_wrstr1=FALSE;
		}

		LCD_RunStringBARCode ( g_sBarCode, RUN_STR_DELAY, 1 );

		if(u8_wrnfcstatus==9){
			PN532_WriteTagtoHex();

			//b_hextostr = fourbytes2strhex (&outwrdata[0], &uidhex2pc4b[0]);

			Usart2_SendData(s_pn_uidvalue,strlen(s_pn_uidvalue));
			Usart2_SendData(outwrdata,sizeof(outwrdata));
			Usart2_SendData(s_newline,strlen(s_newline));

			pn532_wr_stat=PN532_Write_Data(uid,uidLength, 1, outwrdata);

			switch(pn532_wr_stat){
			case 0:
				Usart2_SendData(s_pc_PN532_info2,strlen(s_pc_PN532_info2));
				Usart2_SendData(s_newline,strlen(s_newline));
				break;
			case 1:
				Usart2_SendData(s_pc_PN532_err1,strlen(s_pc_PN532_err1));
				Usart2_SendData(s_newline,strlen(s_newline));
				break;
			case 2:
				Usart2_SendData(s_pc_PN532_err4,strlen(s_pc_PN532_err4));
				Usart2_SendData(s_newline,strlen(s_newline));
				break;
			case 3:
				Usart2_SendData(s_pc_PN532_err2,strlen(s_pc_PN532_err2));
				Usart2_SendData(s_newline,strlen(s_newline));
				break;
			case 4:
				Usart2_SendData(s_pc_PN532_err3,strlen(s_pc_PN532_err3));
				Usart2_SendData(s_newline,strlen(s_newline));
				break;


			}

			//PN532_WriteID();

			u8_wrnfcstatus=0;

			   memset(g_sBarCode,0,strlen(g_sBarCode));
			  strlcpy ( g_sBarCode, (const char *)s_lcd_scanner_rdy, USB_STATE_LEN );
			  LCD_RunStringBARCode ( g_sBarCode, RUN_STR_DELAY, 0 );


			//LCD_SetCursor ( LCD_CURSOR_1STR );
		//	LCD_WriteString (s_lcd_scanner_rdy );

			LCD_SetCursor ( LCD_CURSOR_2STR );
			LCD_WriteString (s_lcd_ready );
		}




	}
}
