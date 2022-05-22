/*
 * CAN.h
 *
 *  Created on: May 16, 2022
 *      Author: Kunal
 */

#ifndef CAN_H_
#define CAN_H_


#include "main.h"
#include "GPIO.h"
#include "Console.h"
#include "CAN_Defines.h"


#define _CAN_DEBUG_ 1



typedef struct CAN_Mailbox{
	int mailbox_id;
	int timestamp;
	int32_t baudrate;
	int ID_Type;
	int Frame_Type;
	int32_t ID;
	int data_length;
	int data[8];
	int filter_type;
	int32_t mask[13];
	int32_t list[13];
}CAN_Mailbox;


void CAN_Init(CAN_Mailbox mailbox);
void CAN_Send_Payload(CAN_Mailbox mailbox);
int CAN_Get_Operating_Mode(void);
void CAN_Enter_Sleep_Mode(void);
void CAN_ID_Filter(int filter_number, int filter_type, int32_t reg1, int32_t reg2 );


#endif /* CAN_H_ */
