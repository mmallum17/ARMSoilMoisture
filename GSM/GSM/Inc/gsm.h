/*
 * gsm.h
 *
 *  Created on: Dec 6, 2017
 *      Author: Marcus
 */

#ifndef GSM_H_
#define GSM_H_
/*Public Function Prototypes*/
void executeCommand(char* command, char* rcvBuffer, char* search, uint32_t timeout);
void connectNetwork();
void connectServer();
void serverComm(char* send, char* receive);
void switchCmdMode();
void closeServer();


#endif /* GSM_H_ */
