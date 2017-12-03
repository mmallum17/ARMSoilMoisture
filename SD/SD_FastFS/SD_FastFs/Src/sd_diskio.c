/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/* 
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future. 
 * Kept to ensure backward compatibility with previous CubeMx versions when 
 * migrating projects. 
 * User code previously added there should be copied in the new user sections before 
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
extern SPI_HandleTypeDef hspi1;
/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS sd_initialize (BYTE pdrv);
DSTATUS sd_status (BYTE pdrv);
DRESULT sd_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT sd_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT sd_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  SD_Driver =
{
  sd_initialize,
  sd_status,
  sd_read,
#if  _USE_WRITE
  sd_write,
#endif  /* _USE_WRITE == 1 */  
#if  _USE_IOCTL == 1
  sd_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS sd_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
    Stat = STA_NOINIT;

    if(pdrv)
    {
    	return Stat;
    }
    uint8_t cmd0[6] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
	uint8_t cmd55[6] = {0x40 | 55, 0x00, 0x00, 0x00, 0x00, 0x65};
	uint8_t cmd41[6] = {0x40 | 41, 0x40, 0x00, 0x00, 0x00, 0x77};
	uint8_t cmd8[6] = {0x40 | 8, 0x00, 0x00, 0x01, 0xAA, 0x87};
	uint8_t cmd58[6] = {0x40 | 58, 0x00, 0x00, 0x01, 0xAA, 0x87};
	uint8_t data[10];
	uint8_t receive = 0xFF;
	uint8_t high = 0xFF;
	uint16_t n = 10;
	uint8_t i = 0;
	char display[15];
	for(i = 0; i < 10; i++)
	{
		data[i] = 0xFF;
	}
	/*Set CS High*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	/*Write with CS high for 74+ clock cycles*/
	HAL_SPI_Transmit(&hspi1, data, sizeof(data), 0x1000);


	/*Set CS low*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	/*Send CMD0*/
	HAL_SPI_Transmit(&hspi1, cmd0, sizeof(cmd0), 0x1000);
	/*Wait for Response*/
	do
	{
		HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
	}while((receive & 0x80) && --n);
	/*Dummy bytes sending... required*/
	for (i=0; i<8; i++)
		HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
	/*Set CS high*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	/*Debug Results on OLED Screen*/
	ssd1306_WriteString("HI", 1);
	sprintf(display, "%02X ", receive);
	ssd1306_WriteString(display, 1);
	updateScreen();



	uint8_t ocr[4] = {0,0,0,0};
	/*Set CS low*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	/*Send CMD0*/
	HAL_SPI_Transmit(&hspi1, cmd8, sizeof(cmd8), 0x1000);
	/*Wait for Response*/
	do
	{
		HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
	}while((receive & 0x80) && --n);
	for(i = 0; i < 4; i++)
	{
		HAL_SPI_TransmitReceive(&hspi1, &high, &ocr[i], sizeof(high), 0x1000);
	}
	/*Dummy bytes sending... required*/
	for (i=0; i<8; i++)
		HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
	/*Set CS high*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	/*Debug Results on OLED Screen*/
	clearScreen();
	ssd1306_WriteString("HI", 1);
	sprintf(display, "%02X %02X%02X%02X%02X", receive, ocr[0], ocr[1], ocr[2], ocr[3]);
	ssd1306_WriteString(display, 1);
	updateScreen();



	uint16_t tries = 1000;
	do
	{
		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD55*/
		HAL_SPI_Transmit(&hspi1, cmd55, sizeof(cmd55), 0x1000);
		n = 10;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);
		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
				HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("HA", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();

		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD41*/
		HAL_SPI_Transmit(&hspi1, cmd41, sizeof(cmd41), 0x1000);
		n = 10;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);
		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
				HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("Ho", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();
	}while(receive != 0 && --tries);



	ocr[0] = 0;
	ocr[1] = 0;
	ocr[2] = 0;
	ocr[3] = 0;
	/*Set CS low*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	/*Send CMD0*/
	HAL_SPI_Transmit(&hspi1, cmd58, sizeof(cmd58), 0x1000);
	/*Wait for Response*/
	do
	{
		HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
	}while((receive & 0x80) && --n);
	for(i = 0; i < 4; i++)
	{
		HAL_SPI_TransmitReceive(&hspi1, &high, &ocr[i], sizeof(high), 0x1000);
	}
	/*Dummy bytes sending... required*/
	for (i=0; i<8; i++)
		HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
	/*Set CS high*/
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	/*Debug Results on OLED Screen*/
	clearScreen();
	ssd1306_WriteString("HI", 1);
	sprintf(display, "%02X %02X%02X%02X%02X", receive, ocr[0], ocr[1], ocr[2], ocr[3]);
	ssd1306_WriteString(display, 1);
	updateScreen();

	if((ocr[0] & 0x40))
	{
		Stat = 0;
	}
    return Stat;
  /* USER CODE END INIT */
}
 
/**
  * @brief  Gets Disk Status 
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS sd_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
	if (pdrv)
	{
		return STA_NOINIT;		/* Supports only drive 0 */
	}

	return Stat;	/* Return disk status */
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s) 
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT sd_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
	uint8_t cmd17[6] = {0x40 | 17, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t cmd18[6] = {0x40 | 18, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t cmd12[6] = {0x40 | 12, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t receive = 0xFF;
	uint8_t high = 0xFF;
	uint16_t n = 10;
	uint16_t i = 0;
	char display[15];




	/*Check Parameters*/
	if(pdrv || !count)
	{
		return RES_PARERR;
	}

	/*Read Single block*/
	if(count == 1)
	{

		cmd17[1] = (BYTE)(sector >> 24);
		cmd17[2] = (BYTE)(sector >> 16);
		cmd17[3] = (BYTE)(sector >> 8);
		cmd17[4] = (BYTE)(sector);
		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD17*/
		HAL_SPI_Transmit(&hspi1, cmd17, sizeof(cmd17), 0x1000);
		n = 10;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);

		/*Get Data Token*/
		n = 20000;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);

		}while((receive == 0xFF) && --n);
		if(receive != 0xFE)
		{

			return 0;
		}

		/*Read Actual Data*/
		for (i=0; i<512; i++)
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &buff[i], sizeof(high), 0x1000);
		}
		/*Discard CRC*/
		HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);

		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
			HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("HA", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();
	}
	else	/*Multiple Block Read*/
	{
		cmd18[1] = (BYTE)(sector >> 24);
		cmd18[2] = (BYTE)(sector >> 16);
		cmd18[3] = (BYTE)(sector >> 8);
		cmd18[4] = (BYTE)(sector);
		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD18*/
		HAL_SPI_Transmit(&hspi1, cmd18, sizeof(cmd18), 0x1000);
		n = 10;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);

		/*Get Data*/
		do
		{
			/*Get Data Token*/
			n = 20000;
			receive = 0xFF;
			do
			{
				HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);

			}while((receive == 0xFF) && --n);
			if(receive != 0xFE)
			{
				return 0;
			}
			/*Read Actual Data*/
			for (i=0; i<512; i++)
			{
				HAL_SPI_TransmitReceive(&hspi1, &high, &buff[i], sizeof(high), 0x1000);
			}
			/*Discard CRC*/
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
			buff += 512;
		}while(--count);

		/*Send CMD12 to stop*/
		HAL_SPI_Transmit(&hspi1, cmd12, sizeof(cmd12), 0x1000);
		HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		n = 10;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);

		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
			HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("HA", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();
	}
    return RES_OK;
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)  
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT sd_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{ 
  /* USER CODE BEGIN WRITE */
	uint8_t cmd55[6] = {0x40 | 55, 0x00, 0x00, 0x00, 0x00, 0x65};
	uint8_t receive = 0xFF;
	uint8_t high = 0xFF;
	uint8_t token = 0xFE;
	uint16_t n = 10;
	uint16_t i = 0;
	char display[15];
	uint8_t cmd25[6] = {0x40 | 25, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t cmd24[6] = {0x40 | 24, 0x00, 0x00, 0x00, 0x00, 0x01};
	uint8_t cmd23[6] = {0x40 | 23, 0x00, 0x00, 0x00, 0x00, 0x01};
	/*char* test = (char*)buff;
	test[5] = 0;*/

	if(pdrv || !count)
	{
		return RES_PARERR;
	}

	if(count == 1)
	{
		/*clearScreen();
		ssd1306_WriteString(test, 1);
		updateScreen();
		HAL_Delay(1000);*/
		cmd24[1] = (BYTE)(sector >> 24);
		cmd24[2] = (BYTE)(sector >> 16);
		cmd24[3] = (BYTE)(sector >> 8);
		cmd24[4] = (BYTE)(sector);
		token = 0xFE;
		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD24*/
		HAL_SPI_Transmit(&hspi1, cmd24, sizeof(cmd24), 0x1000);
		n = 10;
		receive = 0xFF;
		/*Get Response*/
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);

		/*Check if busy*/
		n = 500;
		receive = 0x00;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while(receive != 0xFF && --n);
		if(receive != 0xFF)
		{
			return RES_ERROR;
		}

		/*Send Token*/
		HAL_SPI_Transmit(&hspi1, &token, sizeof(token), 0x1000);
		/*Send Data*/
		HAL_SPI_Transmit(&hspi1, buff, 512, 0x5000);
		/*Send dummy CRC*/
		HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Get Data Response*/
		HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		if((receive & 0x1F) != 0x05)
		{
			return RES_ERROR;
		}

		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
			HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("HA", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();
	}
	else
	{
		cmd23[1] = (BYTE)(((DWORD)count) >> 24);
		cmd23[2] = (BYTE)(((DWORD)count) >> 16);
		cmd23[3] = (BYTE)(((DWORD)count) >> 8);
		cmd23[4] = (BYTE)(((DWORD)count));
		token = 0xFC;
		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD55*/
		HAL_SPI_Transmit(&hspi1, cmd55, sizeof(cmd55), 0x1000);
		n = 10;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);
		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
				HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("HA", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();

		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD23*/
		HAL_SPI_Transmit(&hspi1, cmd23, sizeof(cmd23), 0x1000);
		n = 10;
		receive = 0xFF;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);
		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
				HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("Ho", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();

		cmd25[1] = (BYTE)(sector >> 24);
		cmd25[2] = (BYTE)(sector >> 16);
		cmd25[3] = (BYTE)(sector >> 8);
		cmd25[4] = (BYTE)(sector);
		token = 0xFC;
		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		/*Send CMD25*/
		HAL_SPI_Transmit(&hspi1, cmd25, sizeof(cmd25), 0x1000);
		n = 10;
		receive = 0xFF;
		/*Get Response*/
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while((receive & 0x80) && --n);

		/*Send Data*/
		do
		{
			/*Check if busy*/
			n = 500;
			receive = 0x00;
			do
			{
				HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
			}while(receive != 0xFF && --n);
			if(receive != 0xFF)
			{
				return RES_ERROR;
			}

			/*Send Token*/
			HAL_SPI_Transmit(&hspi1, &token, sizeof(token), 0x1000);
			/*Send Data*/
			HAL_SPI_Transmit(&hspi1, buff, 512, 0x5000);
			/*Send dummy CRC*/
			HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
			HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
			/*Get Data Response*/
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
			if((receive & 0x1F) != 0x05)
			{
				return RES_ERROR;
			}
			buff += 512;
		}while(--count);

		/*Check if busy*/
		n = 500;
		receive = 0x00;
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while(receive != 0xFF && --n);
		if(receive != 0xFF)
		{
			return RES_ERROR;
		}

		token = 0xFD;
		/*Send Token*/
		HAL_SPI_Transmit(&hspi1, &token, sizeof(token), 0x1000);

		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
				HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		/*Debug results on OLED Screen*/
		clearScreen();
		ssd1306_WriteString("Ho", 1);
		sprintf(display, "%02X ", receive);
		ssd1306_WriteString(display, 1);
		updateScreen();
	}
    return RES_OK;
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation  
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT sd_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
    DRESULT res = RES_ERROR;
    uint8_t high = 0xFF;
    uint8_t receive = 0;
    uint16_t n = 500;
    uint8_t i = 0;

    switch(cmd)
    {
    case CTRL_SYNC :
    	/*Check if busy*/
		n = 500;
		receive = 0x00;
		/*Set CS low*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		do
		{
			HAL_SPI_TransmitReceive(&hspi1, &high, &receive, sizeof(high), 0x1000);
		}while(receive != 0xFF && --n);
		if(receive != 0xFF)
		{
			return RES_ERROR;
		}
		/*Send dummy bytes... required*/
		for (i=0; i<8; i++)
				HAL_SPI_Transmit(&hspi1, &high, sizeof(high), 0x1000);
		/*Set CS high*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		res = RES_OK;
		break;
    }
    return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
