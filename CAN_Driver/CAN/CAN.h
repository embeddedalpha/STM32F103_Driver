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


#define _CAN_DEBUG_ 0

typedef struct CAN_RX_Mailbox{

	int message_timestamp;
	int data_length;
	int data[8];
	int filter_index;
	int frame_type;
	int id_type;
	int32_t ID;

}CAN_RX_Mailbox;




typedef struct CAN_Config{
	int32_t baudrate;
	int timestamp_enable;

	int filter_index;
	int filter_type;
	int filter_scale;
	int filter_bank_index;
	uint32_t filter_bank1;
	uint32_t filter_bank2;

	int TX_ID_Type;
	int TX_Frame_Type;
	int TX_Timestamp;
	int TX_Mailbox_ID;
	int32_t TX_ID;
	int TX_data_length;
	int TX_data[8];

	CAN_RX_Mailbox RX_Mailbox_1;
	CAN_RX_Mailbox RX_Mailbox_2;
	CAN_RX_Mailbox RX_Mailbox_3;

}CAN_Config;





typedef struct CAN_Mailbox{


//FILER
	int filter_type;
	int32_t mask[13];
	int32_t list[13];


// Configuration
	int32_t baudrate;
	int timestamp_enable;

// TX
	int TX_ID_Type;
	int TX_Frame_Type;
	int TX_Mailbox_ID;
	int32_t TX_ID;
	int TX_data_length;
	int TX_data[8];

}CAN_Mailbox;


void CAN_Init(CAN_Config mailbox);
void CAN_Send_Payload(CAN_Config mailbox);
int CAN_Get_Operating_Mode(void);
void CAN_Enter_Sleep_Mode(void);
void CAN_TX_Payload(CAN_Config mailbox);
void CAN_Flush_TX_Buffers(int buffer_id);

#endif /* CAN_H_ */
