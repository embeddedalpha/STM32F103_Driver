/*
 * CAN.h
 *
 *  Created on: May 26, 2022
 *      Author: Kunal
 */

#ifndef CAN_H_
#define CAN_H_

#include "main.h"
#include "Console.h"
#include "GPIO.h"
#include "CAN_Defines.h"



typedef struct CAN_RX_Mailbox{

	int message_timestamp;
	int data_length;
	int data[8];
	int filter_index;
	int frame_type;
	int id_type;
	int32_t ID;

}CAN_RX_Mailbox;

CAN_RX_Mailbox RX_Mailbox_1;			//Structure stores the data from RX Mailbox 1
CAN_RX_Mailbox RX_Mailbox_2;            //Structure stores the data from RX Mailbox 2

typedef struct CAN_Filter_TypeDef{
	int enable;
	int type;
	int scale;
	int bank_id;
	uint32_t Mask_Register;
	uint32_t ID_Register;

}CAN_Filter_TypeDef;



typedef struct CAN_Config{
	int32_t baudrate;
	int timestamp_enable;
	CAN_Filter_TypeDef Filter0;
	CAN_Filter_TypeDef Filter1;
	CAN_Filter_TypeDef Filter2;
	CAN_Filter_TypeDef Filter3;
	CAN_Filter_TypeDef Filter4;
	CAN_Filter_TypeDef Filter5;
	CAN_Filter_TypeDef Filter6;
	CAN_Filter_TypeDef Filter7;
	CAN_Filter_TypeDef Filter8;
	CAN_Filter_TypeDef Filter9;
	CAN_Filter_TypeDef Filter10;
	CAN_Filter_TypeDef Filter11;
	CAN_Filter_TypeDef Filter12;
	CAN_Filter_TypeDef Filter13;
//	int filter_index;
//	int filter_type;
//	int filter_scale;
//	int filter_bank_index;
//	uint32_t filter_Mask;
//	uint32_t filter_ID;
//
//	uint32_t filter_bank1;
//	uint32_t filter_bank2;
	int TX_ID_Type;
	int TX_Frame_Type;
	int TX_Timestamp;
//	int TX_Mailbox_ID;
	int32_t TX_ID;							//ID of the data buffer
	int TX_data_length;						//DLC : Data length of the buffer to be sent
	int TX_data[8];							//Data buffer to be sent to the CAN bus

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
 * @func 			:	CAN_Send_Payload
 * @rev				:	2
 * @Comment			:	Fixed Transmission Error
 * @input param		:	mailbox
 * @output param	:	none
 * @operation		:	Sends a payload on the CAN bus by filling up the next available mailbox.
 */
int CAN_Send_Payload(CAN_Config mailbox);

/*
 * @func 			:	CAN_Get_Payload
 * @rev				:	2
 * @Comment			:	Fixed frame drops in reception and filling time-stamp
 * @input param		:	mailbox
 * @output param	:	none
 * @operation		:	Sends a payload on the CAN bus by filling up the next available mailbox.
 */
int CAN_Get_Payload(CAN_Config mailbox);


#endif /* CAN_H_ */
