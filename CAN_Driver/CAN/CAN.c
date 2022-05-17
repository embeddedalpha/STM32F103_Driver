/*
 * CAN.c
 *
 *  Created on: May 16, 2022
 *      Author: Kunal
 */


#include "CAN.h"


void CAN_Init(int32_t baudrate)
{
	RCC -> APB1ENR &= ~RCC_APB1ENR_CAN1EN;


	GPIO_Pin_Setup(GPIOA, 12, ALT_PUSH_PULL_OUTPUT);
	GPIO_Pin_Setup(GPIOA, 11, FLOATING_INPUT);
	RCC -> APB1ENR |= RCC_APB1ENR_CAN1EN;
	CAN1 -> MCR |= CAN_MCR_DBF | CAN_MCR_RESET | CAN_MCR_SLEEP;
	while(!(CAN1 -> MSR & CAN_MSR_SLAK)){}
	CAN1 -> MCR &= ~CAN_MCR_SLEEP;
	CAN1 -> MCR |= CAN_MCR_INRQ;
	while((CAN1 -> MSR & CAN_MSR_SLAK)){}
	while(!(CAN1-> MSR & CAN_MSR_INAK)){}
	CAN1 -> BTR = baudrate;
	CAN1->BTR |= CAN_BTR_LBKM;  //SET LBKM BIT
    CAN1->MCR &= ~CAN_MCR_INRQ;
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while((CAN1->MSR & CAN_MSR_INAK));
//    CAN1->MCR &= ~CAN_MCR_SLEEP;
//    while((CAN1->MSR & CAN_MSR_SLAK));
}


void CAN_TX_Mailbox1(void)
{
	switch (CAN_Mailbox1.ID_Type)
	{
		case CAN_ID_Standard:
		{
			CAN1 -> sTxMailBox[0].TIR  = CAN_Mailbox1.Standard_ID << 21;
			CAN1 -> sTxMailBox[0].TIR  &= ~CAN_TI0R_EXID;
		}
			break;
		case CAN_ID_Extended:
		{
			CAN1 -> sTxMailBox[0].TIR  = CAN_Mailbox1.Extended_ID << 2;
			CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_EXID;
		}
			break;
	}

	switch (CAN_Mailbox1.Frame_Type)
	{
		case CAN_Frame_Data:
		{
			CAN1 -> sTxMailBox[0].TIR  &= ~CAN_TI0R_RTR;
		}
			break;
		case CAN_Frame_Remote:
		{
			CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_RTR;
		}
			break;

	}


	CAN1 -> sTxMailBox[0].TDTR = CAN_Mailbox1.DLC;
	CAN1 -> sTxMailBox[0].TDHR = CAN_Mailbox1.data[7] << 24 | CAN_Mailbox1.data[6] << 16 | CAN_Mailbox1.data[5] << 8 | CAN_Mailbox1.data[4] << 0;
	CAN1 -> sTxMailBox[0].TDLR = CAN_Mailbox1.data[3] << 24 | CAN_Mailbox1.data[2] << 16 | CAN_Mailbox1.data[1] << 8 | CAN_Mailbox1.data[0] << 0;
	CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_TXRQ;

//    while(!(CAN1 -> TSR & CAN_TSR_TXOK0)){}

}


void CAN_TX_Mailbox2(void)
{

	switch (CAN_Mailbox2.ID_Type)
	{
		case CAN_ID_Standard:
		{
			CAN1 -> sTxMailBox[1].TIR  |= CAN_Mailbox2.Standard_ID << 21;
			CAN1 -> sTxMailBox[1].TIR  &= ~CAN_TI0R_EXID;
		}
			break;
		case CAN_ID_Extended:
		{
			CAN1 -> sTxMailBox[1].TIR  |= CAN_Mailbox2.Extended_ID << 3;
			CAN1 -> sTxMailBox[1].TIR  |= CAN_TI0R_EXID;
		}
			break;
	}

	switch (CAN_Mailbox2.Frame_Type)
	{
		case CAN_Frame_Data:
		{
			CAN1 -> sTxMailBox[1].TIR  &= ~CAN_TI0R_RTR;
		}
			break;
		case CAN_Frame_Remote:
		{
			CAN1 -> sTxMailBox[1].TIR  |= CAN_TI0R_RTR;
		}
			break;

	}

	CAN1 -> sTxMailBox[1].TDTR |= CAN_Mailbox2.DLC;
	CAN1 -> sTxMailBox[1].TDHR |= CAN_Mailbox2.data[7] << 24 | CAN_Mailbox2.data[6] << 16 | CAN_Mailbox2.data[5] << 8 | CAN_Mailbox2.data[4] << 0;
	CAN1 -> sTxMailBox[1].TDLR |= CAN_Mailbox2.data[3] << 24 | CAN_Mailbox2.data[2] << 16 | CAN_Mailbox2.data[1] << 8 | CAN_Mailbox2.data[0] << 0;
	CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_TXRQ;

}


void CAN_TX_Mailbox3(void)
{

	switch (CAN_Mailbox3.ID_Type)
	{
		case CAN_ID_Standard:
		{
			CAN1 -> sTxMailBox[2].TIR  |= CAN_Mailbox3.Standard_ID << 21;
			CAN1 -> sTxMailBox[2].TIR  &= ~CAN_TI0R_EXID;
		}
			break;
		case CAN_ID_Extended:
		{
			CAN1 -> sTxMailBox[2].TIR  |= CAN_Mailbox3.Extended_ID << 3;
			CAN1 -> sTxMailBox[2].TIR  |= CAN_TI0R_EXID;
		}
			break;
	}

	switch (CAN_Mailbox3.Frame_Type)
	{
		case CAN_Frame_Data:
		{
			CAN1 -> sTxMailBox[2].TIR  &= ~CAN_TI0R_RTR;
		}
			break;
		case CAN_Frame_Remote:
		{
			CAN1 -> sTxMailBox[2].TIR  |= CAN_TI0R_RTR;
		}
			break;

	}

	CAN1 -> sTxMailBox[2].TDTR |= CAN_Mailbox3.DLC;
	CAN1 -> sTxMailBox[2].TDHR |= CAN_Mailbox3.data[7] << 24 | CAN_Mailbox3.data[6] << 16 | CAN_Mailbox3.data[5] << 8 | CAN_Mailbox3.data[4] << 0;
	CAN1 -> sTxMailBox[2].TDLR |= CAN_Mailbox3.data[3] << 24 | CAN_Mailbox3.data[2] << 16 | CAN_Mailbox3.data[1] << 8 | CAN_Mailbox3.data[0] << 0;
	CAN1 -> sTxMailBox[2].TIR  |= CAN_TI1R_TXRQ;

}