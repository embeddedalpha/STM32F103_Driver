/*
 * CAN_Defines.h
 *
 *  Created on: 18-May-2022
 *      Author: Kunal
 */

#ifndef CAN_DEFINES_H_
#define CAN_DEFINES_H_

#include "main.h"

#define CAN_BAUDRATE_1000_KBPS	0x001e0001
#define CAN_BAUDRATE_750_KBPS   0x001c0002
#define CAN_BAUDRATE_500_KBPS   0x001e0003
#define CAN_BAUDRATE_250_KBPS   0x001e0007
#define CAN_BAUDRATE_200_KBPS   0x001e0009
#define CAN_BAUDRATE_150_KBPS   0x001c000e
#define CAN_BAUDRATE_125_KBPS   0x001e000f
#define CAN_BAUDRATE_100_KBPS   0x001e0013
#define CAN_BAUDRATE_75_KBPS    0x001c001d
#define CAN_BAUDRATE_50_KBPS    0x001e0027
#define CAN_BAUDRATE_25_KBPS    0x001e004f
#define CAN_BAUDRATE_10_KBPS    0x001e00c7
#define CAN_BAUDRATE_5_KBPS     0x001e018f

#define CAN_Frame_Data 0
#define CAN_Frame_Remote 10
#define CAN_ID_Standard 0
#define CAN_ID_Extended 10

#define CAN_FILTER_MASK_MODE 0
#define CAN_FILTER_LIST_MODE 10


#define CAN_Initialization_Mode	    0x10
#define CAN_Sleep_Mode			    0xEF
#define CAN_Normal_Mode			    0x45

#define CAN_Mailbox_1 0x00
#define CAN_Mailbox_2 0xAA
#define CAN_Mailbox_3 0xFF


#endif /* CAN_DEFINES_H_ */