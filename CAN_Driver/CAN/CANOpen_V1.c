/*
 * CANOpen_V1.c
 *
 *  Created on: 17-May-2022
 *      Author: Kunal
 */


#include "CANOpen_V1.h"



//
//void CANOpen_Transmit(uint8_t mailbox,uint8_t Function_Code, uint8_t Node_ID, bool Frame_Type,int data_length, int *data)
//{
//	switch (mailbox) {
//		case 1:
//		{
//			CAN_Mailbox1.Standard_ID = Function_Code << 8 | Node_ID << 0;
//			CAN_Mailbox1.Frame_Type = Frame_Type;
//			CAN_Mailbox1.ID_Type = CAN_ID_Standard;
//			CAN_Mailbox1.DLC = data_length;
//			CAN_Mailbox1.data[0] = data[0];
//			CAN_Mailbox1.data[1] = data[1];
//			CAN_Mailbox1.data[2] = data[2];
//			CAN_Mailbox1.data[3] = data[3];
//			CAN_Mailbox1.data[4] = data[4];
//			CAN_Mailbox1.data[5] = data[5];
//			CAN_Mailbox1.data[6] = data[6];
//			CAN_Mailbox1.data[7] = data[7];
//			CAN_TX_Mailbox1();
//		}
//			break;
//
//		case 2:
//		{
//			CAN_Mailbox2.Standard_ID = Function_Code << 8 | Node_ID << 0;
//			CAN_Mailbox2.Frame_Type = Frame_Type;
//			CAN_Mailbox2.ID_Type = CAN_ID_Standard;
//			CAN_Mailbox2.DLC = data_length;
//			CAN_Mailbox2.data[0] = data[0];
//			CAN_Mailbox2.data[1] = data[1];
//			CAN_Mailbox2.data[2] = data[2];
//			CAN_Mailbox2.data[3] = data[3];
//			CAN_Mailbox2.data[4] = data[4];
//			CAN_Mailbox2.data[5] = data[5];
//			CAN_Mailbox2.data[6] = data[6];
//			CAN_Mailbox2.data[7] = data[7];
//			CAN_TX_Mailbox2();
//		}
//			break;
//
//		case 3:
//		{
//			CAN_Mailbox3.Standard_ID = Function_Code << 8 | Node_ID << 0;
//			CAN_Mailbox3.Frame_Type = Frame_Type;
//			CAN_Mailbox3.ID_Type = CAN_ID_Standard;
//			CAN_Mailbox3.DLC = data_length;
//			CAN_Mailbox3.data[0] = data[0];
//			CAN_Mailbox3.data[1] = data[1];
//			CAN_Mailbox3.data[2] = data[2];
//			CAN_Mailbox3.data[3] = data[3];
//			CAN_Mailbox3.data[4] = data[4];
//			CAN_Mailbox3.data[5] = data[5];
//			CAN_Mailbox3.data[6] = data[6];
//			CAN_Mailbox3.data[7] = data[7];
//			CAN_TX_Mailbox3();
//		}
//			break;
//
//	}
//}
