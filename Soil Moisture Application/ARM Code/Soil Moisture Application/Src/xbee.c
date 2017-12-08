/*
 * XBEE.c
 *
 *  Created on: Dec 5, 2017
 *      Author: Marcus
 */
/*Includes*/
#include<stdint.h>
#include <math.h>
#include "xbee.h"
#include "stm32l1xx_hal.h"

/*Function Prototypes*/
/*void initXbee();
void sensorTransmit();
void sensorReceive(APIResponseFrame* samplesFrame);
float toCelsius(uint16_t tempSample);
float toFahrenheit(float celsius);
void floatToString(float value, char* floatString, int afterpoint);*/

/*Private Variables*/
extern UART_HandleTypeDef huart2;

/*Functions*/
void initXbee()
{
	/*Flush initial buffer*/
	APIResponseFrame temp;
	sensorReceive(&temp);
}
void sensorTransmit(uint8_t sensor)
{
	uint8_t bufferZero[19] = {0x7E, 0x00, 0x0F, 0x17, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x00, 0x02, 0x49, 0x53, 0x49};
	uint8_t bufferOne[19] = {0x7E, 0x00, 0x0F, 0x17, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
					 0x00, 0x00, 0x01, 0x02, 0x49, 0x53, 0x48};
	uint8_t bufferTwo[19] = {0x7E, 0x00, 0x0F, 0x17, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						 0x00, 0x00, 0x02, 0x02, 0x49, 0x53, 0x47};
	switch(sensor)
	{
		case 0:
			HAL_UART_Transmit(&huart2, bufferZero, 19, 50);
			break;
		case 1:
			HAL_UART_Transmit(&huart2, bufferOne, 19, 50);
			break;
		case 2:
			HAL_UART_Transmit(&huart2, bufferTwo, 19, 50);
			break;
	}
}

void sensorReceive(APIResponseFrame* samplesFrame)
{
	uint8_t buffer[28];
	uint8_t i;

	for(i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = 0;
	}
	HAL_UART_Receive(&huart2, buffer, sizeof(buffer), 1000);

	samplesFrame->startDelimiter = buffer[0];
	samplesFrame->length = (buffer[1] << 8) | buffer[2];
	samplesFrame->frameType = buffer[3];
	samplesFrame->frameId = buffer[4];
	samplesFrame->sourceAddress64 = ((uint64_t)buffer[5] << 56) | ((uint64_t)buffer[6] << 48) | ((uint64_t)buffer[7] << 40) | ((uint64_t)buffer[8] << 32) | ((uint64_t)buffer[9] << 24) | ((uint64_t)buffer[10] << 16) | ((uint64_t)buffer[11] << 8) | buffer[12];
	samplesFrame->sourceAddress16 = (buffer[13] << 8) | buffer[14];
	samplesFrame->remoteCommand = (buffer[15] << 8) | buffer[16];
	samplesFrame->status = buffer[17];
	samplesFrame->samples = buffer[18];
	samplesFrame->analogMask = buffer[19];
	samplesFrame->digitalMask = buffer[20];
	samplesFrame->digitalSample = (buffer[21] << 8) | buffer[22];
	samplesFrame->moistureSample = (buffer[23] << 8) | buffer[24];
	samplesFrame->temperatureSample = (buffer[25] << 8) | buffer[26];
	samplesFrame->checksum = buffer[27];
}

float toCelsius(uint16_t tempSample)
{
	return ((float)tempSample / 1023 * 3.3 - 1.8663) * 1000 / -11.69;
}

float toFahrenheit(float celsius)
{
	return celsius * 9 / 5 + 32;
}

void floatToString(float value, char* floatString, int afterpoint)
{
	uint32_t intValue = value;
	float tmpFrac = value - intValue;
	uint32_t intFrac = trunc(tmpFrac * pow(10, afterpoint));

	sprintf(floatString, "%lu.%lu", intValue, intFrac);
}
