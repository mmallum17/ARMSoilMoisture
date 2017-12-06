/*
 * gsm.c
 *
 *  Created on: Dec 6, 2017
 *      Author: Marcus
 */
/*Includes*/
#include "stm32l1xx_hal.h"
#include "gsm.h"




/*Global Variables*/
extern UART_HandleTypeDef huart4;

/*Functions*/
void connectNetwork()
{
	char rcvBuffer[80] = "";
	HAL_Delay(2000);
	executeCommand("AT+CREG?\r", rcvBuffer, "+CREG: 0,1", 500);
	executeCommand("AT+CGATT?\r", rcvBuffer, "+CGATT: 1", 500);
}

void connectServer()
{
	char rcvBuffer[80] = "";
	connectNetwork();
	executeCommand("AT+CIPMODE=1\r", rcvBuffer, "OK", 500);
	executeCommand("AT+CSTT=\"wholesale\"\r", rcvBuffer, "OK", 500);
	executeCommand("AT+CIICR\r", rcvBuffer, "OK", 2000);
	executeCommand("AT+CIFSR\r", rcvBuffer, "", 500);
	executeCommand("AT+CIPSTART=\"TCP\",\"18.221.30.192\",\"4000\"\r", rcvBuffer, "CONNECT", 2000);
}

void serverComm(char* send, char* receive)
{
	executeCommand(send, receive, "", 2000);
}

void switchCmdMode()
{
	char rcvBuffer[80] = "";
	HAL_Delay(1000);
	executeCommand("+++", rcvBuffer, "", 500);
	HAL_Delay(1000);
}

void closeServer()
{
	char rcvBuffer[80] = "";
	executeCommand("AT+CIPCLOSE\r", rcvBuffer, "CLOSE OK", 1000);
}

void executeCommand(char* command, char* rcvBuffer, char* search, uint32_t timeout)
{
	uint8_t length = strlen(command);
	do
	{
		for(uint8_t i = 0; i < 50; i++)
		{
			rcvBuffer[i] = 0;
		}

		HAL_UART_Transmit(&huart4, (uint8_t*)command, length, 50);
		HAL_UART_Receive(&huart4, (uint8_t*)rcvBuffer, 80, timeout);
		/*clearScreen();
		ssd1306_WriteString(rcvBuffer, 1);
		updateScreen();*/
	}while(strstr(rcvBuffer, search) == NULL);
}
