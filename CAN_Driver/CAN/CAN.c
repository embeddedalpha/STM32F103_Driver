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
}


void CAN_Enter_Sleep_Mode(bool enable_disable)
{

}


int CAN_Get_Operating_Mode(void)
{
	int temp;
	int reg = CAN1 -> MSR;

	if( ((reg & CAN_MSR_SLAK) != CAN_MSR_SLAK) && ((reg & CAN_MSR_INAK) == CAN_MSR_INAK) )
	{
		temp = CAN_Initialization_Mode;
#if _CAN_DEBUG_
	printConsole("CAN controller is in Initialization Mode \r\n");
#endif
	}

	if( ((reg & CAN_MSR_SLAK) != CAN_MSR_SLAK) && ((reg & CAN_MSR_INAK) != CAN_MSR_INAK) )
	{
		temp = CAN_Normal_Mode;
#if _CAN_DEBUG_
	printConsole("CAN controller is in Normal Mode \r\n");
#endif
	}

	if( ((reg & CAN_MSR_SLAK) == CAN_MSR_SLAK) && ((reg & CAN_MSR_INAK) != CAN_MSR_INAK) )
	{
		temp = CAN_Sleep_Mode;
#if _CAN_DEBUG_
	printConsole("CAN controller is in Sleep Mode \r\n");
#endif
	}

	return temp;
}



void CAN_ID_Filter(int filter_number, int filter_type, int32_t reg1, int32_t reg2 )
{
	CAN1 -> FMR |= CAN_FMR_FINIT;
	CAN1 -> FM1R |= filter_type << filter_number;
	CAN1 -> FS1R |= 1 << filter_number;
	CAN1 -> sFilterRegister[filter_number].FR1 = reg1;
	CAN1 -> sFilterRegister[filter_number].FR2 = reg2;
	CAN1 -> FMR &= ~CAN_FMR_FINIT;
}



void CAN_Send_Payload(CAN_Mailbox mailbox)
{
	switch (mailbox.mailbox_id)
	{
//=================================================================================================//
		case CAN_Mailbox_1:{
			switch (mailbox.ID_Type)
			{
				case CAN_ID_Standard:
				{
					CAN1 -> sTxMailBox[0].TIR  = mailbox.ID << 21;
					CAN1 -> sTxMailBox[0].TIR  &= ~CAN_TI0R_EXID;
				}
					break;
				case CAN_ID_Extended:
				{
					CAN1 -> sTxMailBox[0].TIR  = mailbox.ID << 2;
					CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_EXID;
				}
					break;
			}

			switch (mailbox.Frame_Type)
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
			CAN1 -> sTxMailBox[0].TDTR = mailbox.data_length;
			CAN1 -> sTxMailBox[0].TDHR = mailbox.data[7] << 24 | mailbox.data[6] << 16 | mailbox.data[5] << 8 | mailbox.data[4] << 0;
			CAN1 -> sTxMailBox[0].TDLR = mailbox.data[3] << 24 | mailbox.data[2] << 16 | mailbox.data[1] << 8 | mailbox.data[0] << 0;
			CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_TXRQ;
		}
			break;

//=================================================================================================//
		case CAN_Mailbox_2:{
			switch (mailbox.ID_Type)
			{
				case CAN_ID_Standard:
				{
					CAN1 -> sTxMailBox[1].TIR  = mailbox.ID << 21;
					CAN1 -> sTxMailBox[1].TIR  &= ~CAN_TI1R_EXID;
				}
					break;
				case CAN_ID_Extended:
				{
					CAN1 -> sTxMailBox[1].TIR  = mailbox.ID << 2;
					CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_EXID;
				}
					break;
			}

			switch (mailbox.Frame_Type)
			{
				case CAN_Frame_Data:
				{
					CAN1 -> sTxMailBox[1].TIR  &= ~CAN_TI1R_RTR;
				}
					break;
				case CAN_Frame_Remote:
				{
					CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_RTR;
				}
					break;
			}
			CAN1 -> sTxMailBox[1].TDTR = mailbox.data_length;
			CAN1 -> sTxMailBox[1].TDHR = mailbox.data[7] << 24 | mailbox.data[6] << 16 | mailbox.data[5] << 8 | mailbox.data[4] << 0;
			CAN1 -> sTxMailBox[1].TDLR = mailbox.data[3] << 24 | mailbox.data[2] << 16 | mailbox.data[1] << 8 | mailbox.data[0] << 0;
			CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_TXRQ;
		}
		break;

//=================================================================================================//
				case CAN_Mailbox_3:{
					switch (mailbox.ID_Type)
					{
						case CAN_ID_Standard:
						{
							CAN1 -> sTxMailBox[2].TIR  = mailbox.ID << 21;
							CAN1 -> sTxMailBox[2].TIR  &= ~CAN_TI2R_EXID;
						}
							break;
						case CAN_ID_Extended:
						{
							CAN1 -> sTxMailBox[2].TIR  = mailbox.ID << 2;
							CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_EXID;
						}
							break;
					}

					switch (mailbox.Frame_Type)
					{
						case CAN_Frame_Data:
						{
							CAN1 -> sTxMailBox[2].TIR  &= ~CAN_TI2R_RTR;
						}
							break;
						case CAN_Frame_Remote:
						{
							CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_RTR;
						}
							break;
					}
					CAN1 -> sTxMailBox[2].TDTR = mailbox.data_length;
					CAN1 -> sTxMailBox[2].TDHR = mailbox.data[7] << 24 | mailbox.data[6] << 16 | mailbox.data[5] << 8 | mailbox.data[4] << 0;
					CAN1 -> sTxMailBox[2].TDLR = mailbox.data[3] << 24 | mailbox.data[2] << 16 | mailbox.data[1] << 8 | mailbox.data[0] << 0;
					CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_TXRQ;
				}
				break;
	}
}


