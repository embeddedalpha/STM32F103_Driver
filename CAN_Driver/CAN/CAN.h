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


/*
 * @func 			:	CAN_Init
 * @rev				:	1
 * @Comment			:	None
 * @input param		:	mailbox
 * @output param	:	none
 * @operation		:	Configures the CAN controller as per the CAN_Config struct.
 *                      Take a look at the examples and the CAN_Config structure for
 *                      more information
 */
void CAN_Init(CAN_Config mailbox);

/*
 * @func 			:	CAN_Flush_TX_Buffers
 * @rev				:	1
 * @Comment			:	None
 * @input param		:	buffer_id
 * @output param	:	none
 * @operation		:	Clears the TX buffers of CAN controller. The user can either
 * 						clear a single buffer or all buffers at the same time.
 */
void CAN_Flush_TX_Buffers(int buffer_id);

/*
 * @func 			:	CAN_Get_Operating_Mode
 * @rev				:	1
 * @Comment			:	None
 * @input param		:	None
 * @output param	:	temp
 * @operation		:	Returns the current operating mode of the CAN Controller.
 * 						Possible return values are ->
 * 						CAN_Initialization_Mode
 * 						CAN_Normal_Mode
 * 						CAN_Sleep_Mode
 */
int CAN_Get_Operating_Mode(void);



void CAN_Send_Payload(CAN_Config mailbox);


void CAN_TX_Payload(CAN_Config mailbox);





#endif /* CAN_H_ */
