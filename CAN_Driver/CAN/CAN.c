/*
 * CAN.c
 *
 *  Created on: May 16, 2022
 *      Author: Kunal
 */


#include "CAN.h"

int next_mailbox = 0;

void CAN_Init(CAN_Config mailbox)
{
	RCC -> APB1ENR |= RCC_APB1ENR_CAN1EN;
	GPIO_Pin_Setup(GPIOA, 12, ALT_PUSH_PULL_OUTPUT);
	GPIO_Pin_Setup(GPIOA, 11, PULL_DOWN_INPUT);

	CAN1 -> MCR |= CAN_MCR_INRQ;
	while(!(CAN1 -> MSR & CAN_MSR_INAK)){}
	CAN1 -> MCR |= CAN_MCR_ABOM;
	CAN1 -> MCR &= ~CAN_MCR_DBF;
	CAN1 -> BTR |= CAN_BTR_LBKM;
	CAN1 -> BTR = mailbox.baudrate;
	CAN1 -> FMR |= CAN_FMR_FINIT;
	CAN1 -> FMR &= 0xFFFFC0FF;
	CAN1 -> FMR |= 0x1C << 8;

	//Deactivate Filter:
	CAN1 -> FA1R &= ~(1 << mailbox.filter_index);
	//Set Filter Scale
	if(mailbox.filter_scale == CAN_Filter_Scale_16bit) CAN1 -> FS1R &= ~(1 << mailbox.filter_index);
	if(mailbox.filter_scale == CAN_Filter_Scale_32bit) CAN1 -> FS1R |=  (1 << mailbox.filter_index);
	//Set Filter Type
	if(mailbox.filter_type == CAN_FILTER_LIST_MODE) CAN1 -> FM1R |=  (1 << mailbox.filter_index);
	if(mailbox.filter_type == CAN_FILTER_MASK_MODE) CAN1 -> FM1R &= ~(1 << mailbox.filter_index);
	//Set FIFO for Filter Bank
	if(mailbox.filter_type == CAN_Filter_Bank_FIFO1) CAN1 -> FFA1R |=  (1 << mailbox.filter_index);
	if(mailbox.filter_type == CAN_Filter_Bank_FIFO0) CAN1 -> FFA1R &= ~(1 << mailbox.filter_index);
	//Set FIFO Bank
	CAN1 -> sFilterRegister[mailbox.filter_index].FR1 = mailbox.filter_bank1;
	CAN1 -> sFilterRegister[mailbox.filter_index].FR2 = mailbox.filter_bank2;
	CAN1 -> FA1R |= 1 << mailbox.filter_index;

	CAN1 -> FMR &= ~CAN_FMR_FINIT;
	CAN1 -> MCR &= ~CAN_MCR_INRQ;
	CAN1 -> MCR &= ~CAN_MCR_SLEEP;
	while((CAN1->MSR & CAN_MSR_INAK));
	while((CAN1->MSR & CAN_MSR_SLAK));


}

/*
 * Figure out which mode is the controller is in right now.
 * Print that out in debug mode
 * Then make a decision to flip the necessary bits to go into Sleep mode from that mode.
 */
void CAN_Enter_Sleep_Mode(void)
{
	switch (CAN_Get_Operating_Mode())
	{
//=================================================================================================//
		case CAN_Initialization_Mode:
		{
#if _CAN_DEBUG_
			printConsole("Controller is going from Initialization Mode to Sleep Mode \r\n");
#endif
			CAN1 -> MCR |= CAN_MCR_SLEEP;
			CAN1 -> MCR &= ~CAN_MCR_INRQ;
			while(!(CAN1 -> MSR & CAN_MSR_SLAK)){} //Wait till SLAK does not go to 1
			while((CAN1 -> MSR & CAN_MSR_INAK)){}  //Wait till INAK does not go to 0
#if _CAN_DEBUG_
			printConsole("Controller is in Sleep Mode \r\n");
#endif
		}
			break;
//=================================================================================================//
		case CAN_Normal_Mode:
		{
#if _CAN_DEBUG_
			printConsole("Controller is going from Normal Mode to Sleep Mode \r\n");
#endif
			CAN1 -> MCR |= CAN_MCR_SLEEP;
			while(!(CAN1 -> MSR & CAN_MSR_SLAK)){} //Wait till SLAK does not go to 1
#if _CAN_DEBUG_
			printConsole("Controller is in Sleep Mode \r\n");
#endif
		}
			break;
//=================================================================================================//
		case CAN_Sleep_Mode:
		{
#if _CAN_DEBUG_
			printConsole("Controller is in Sleep Mode \r\n");
#endif
		}
			break;
	}

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



void CAN_Send_Payload(CAN_Config mailbox)
{


	switch (mailbox.TX_Mailbox_ID)
	{
//=================================================================================================//
		case CAN_TX_Mailbox_1:{
			switch (mailbox.TX_ID_Type)
			{
				case CAN_ID_Standard:
				{
					CAN1 -> sTxMailBox[0].TIR  = mailbox.TX_ID << 21;
					CAN1 -> sTxMailBox[0].TIR  &= ~CAN_TI0R_EXID;
#if _CAN_DEBUG_
					printConsole("Standard ID : %d\r\n",mailbox.TX_ID);
#endif
				}
					break;
				case CAN_ID_Extended:
				{
					CAN1 -> sTxMailBox[0].TIR  = mailbox.TX_ID << 3;
					CAN1 -> sTxMailBox[0].TIR  |= 1 << 2;
#if _CAN_DEBUG_
					printConsole("Extended ID : %d\r\n",mailbox.TX_ID);
#endif
				}
					break;
			}

			switch (mailbox.TX_Frame_Type)
			{
				case CAN_Frame_Data:
				{
					CAN1 -> sTxMailBox[0].TIR  &= ~CAN_TI0R_RTR;
#if _CAN_DEBUG_
					printConsole("Frame Type : Data\r\n");
#endif
				}
					break;
				case CAN_Frame_Remote:
				{
					CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_RTR;
#if _CAN_DEBUG_
					printConsole("Frame Type : Remote\r\n");
#endif
				}
					break;
			}

			if(mailbox.timestamp_enable == CAN_Timestamp_Enable)
			{
				CAN1 -> sTxMailBox[0].TDTR = mailbox.TX_data_length | CAN_TDT0R_TGT;
				CAN1 -> sTxMailBox[0].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[0].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 ;
				CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_TXRQ;
				while(CAN1 -> sTxMailBox[0].TIR & CAN_TI0R_TXRQ){}
#if _CAN_DEBUG_
					printConsole("Data  : %x, %x, %x, %x, %x\r\n",mailbox.TX_data[0],mailbox.TX_data[1],mailbox.TX_data[2],mailbox.TX_data[3],mailbox.TX_data[4],mailbox.TX_data[5]);
					printConsole("Time_Stamp : %"PRIx32"\r\n",((CAN1 -> sTxMailBox[0].TDTR) >> 16));
#endif

			}
			else
			{
				CAN1 -> sTxMailBox[0].TDTR = mailbox.TX_data_length;
				CAN1 -> sTxMailBox[0].TDTR &= ~CAN_TDT0R_TGT;
				CAN1 -> sTxMailBox[0].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[0].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 | mailbox.TX_data[1] << 8 | mailbox.TX_data[0] << 0;
				CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_TXRQ;
				volatile int attempt = 0;
				while(CAN1->sTxMailBox[0].TIR & 0x1UL && attempt++ < 1000000);
#if _CAN_DEBUG_
				printConsole("Data  : %d, %d, %d, %d, %d, %d, %d\r\n",mailbox.TX_data[0],mailbox.TX_data[1],mailbox.TX_data[2],mailbox.TX_data[3],mailbox.TX_data[4],mailbox.TX_data[5],mailbox.TX_data[6],mailbox.TX_data[7]);
#endif
			}

		}
			break;

//=================================================================================================//
		case CAN_TX_Mailbox_2:{
			switch (mailbox.TX_ID_Type)
			{
				case CAN_ID_Standard:
				{
					CAN1 -> sTxMailBox[1].TIR  = mailbox.TX_ID << 21;
					CAN1 -> sTxMailBox[1].TIR  &= ~CAN_TI1R_EXID;
				}
					break;
				case CAN_ID_Extended:
				{
					CAN1 -> sTxMailBox[1].TIR  = mailbox.TX_ID << 3;
					CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_EXID;
				}
					break;
			}

			switch (mailbox.TX_Frame_Type)
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
			if(mailbox.timestamp_enable == CAN_Timestamp_Enable)
			{
				CAN1 -> sTxMailBox[1].TDTR = mailbox.TX_data_length | CAN_TDT1R_TGT;
				CAN1 -> sTxMailBox[1].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[1].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 ;
				CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_TXRQ;
				while(CAN1 -> sTxMailBox[1].TIR & CAN_TI1R_TXRQ){}
			}
			else
			{
				CAN1 -> sTxMailBox[1].TDTR = mailbox.TX_data_length;
				CAN1 -> sTxMailBox[1].TDTR &= ~CAN_TDT1R_TGT;
				CAN1 -> sTxMailBox[1].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[1].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 | mailbox.TX_data[1] << 8 | mailbox.TX_data[0] << 0;
				CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_TXRQ;
				while(CAN1 -> sTxMailBox[1].TIR & CAN_TI1R_TXRQ){}
			}

		}
		break;

//=================================================================================================//
				case CAN_TX_Mailbox_3:{
					switch (mailbox.TX_ID_Type)
					{
						case CAN_ID_Standard:
						{
							CAN1 -> sTxMailBox[2].TIR  = mailbox.TX_ID << 21;
							CAN1 -> sTxMailBox[2].TIR  &= ~CAN_TI2R_EXID;
						}
							break;
						case CAN_ID_Extended:
						{
							CAN1 -> sTxMailBox[2].TIR  = mailbox.TX_ID << 3;
							CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_EXID;
						}
							break;
					}
					switch (mailbox.TX_Frame_Type)
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
					if(mailbox.timestamp_enable == CAN_Timestamp_Enable)
					{
						CAN1 -> sTxMailBox[2].TDTR = mailbox.TX_data_length | CAN_TDT2R_TGT;
						CAN1 -> sTxMailBox[2].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
						CAN1 -> sTxMailBox[2].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 ;
						CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_TXRQ;
						while(CAN1 -> sTxMailBox[2].TIR & CAN_TI2R_TXRQ){}
					}
					else
					{
						CAN1 -> sTxMailBox[2].TDTR = mailbox.TX_data_length;
						CAN1 -> sTxMailBox[2].TDTR &= ~CAN_TDT2R_TGT;
						CAN1 -> sTxMailBox[2].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
						CAN1 -> sTxMailBox[2].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 | mailbox.TX_data[1] << 8 | mailbox.TX_data[0] << 0;
						CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_TXRQ;
						while(CAN1 -> sTxMailBox[2].TIR & CAN_TI2R_TXRQ){}
					}

				}
				break;
	}
}




void CAN_TX_Payload(CAN_Config mailbox)
{

	int code;
//	int code = (0x3000000 && (CAN1 -> TSR & CAN_TSR_CODE)) >> CAN_TSR_CODE_Pos;

	code = next_mailbox;

	switch (code) {
//===================================================================================
		case 0:
		{
			printConsole("Mailbox 1 \r\n");
//===================================== ID Type	=====================================
			if(mailbox.TX_ID_Type == CAN_ID_Standard)
			{
				CAN1 -> sTxMailBox[0].TIR  = mailbox.TX_ID << 21;
				CAN1 -> sTxMailBox[0].TIR  &= ~CAN_TI0R_EXID;
			}

			if(mailbox.TX_ID_Type == CAN_ID_Extended)
			{
				CAN1 -> sTxMailBox[0].TIR  = mailbox.TX_ID << 3;
				CAN1 -> sTxMailBox[0].TIR  |= 1 << 2;
			}
//===================================== Frame Type	=====================================
			if(mailbox.TX_Frame_Type == CAN_Frame_Data)
			{
				CAN1 -> sTxMailBox[0].TIR  &= ~CAN_TI0R_RTR;
			}

			if(mailbox.TX_Frame_Type == CAN_Frame_Remote)
			{
				CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_RTR;
			}
//===================================== Frame Type	=====================================
			if(mailbox.timestamp_enable == CAN_Timestamp_Enable)
			{
				CAN1 -> sTxMailBox[0].TDTR = mailbox.TX_data_length | CAN_TDT0R_TGT;
				CAN1 -> sTxMailBox[0].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[0].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 ;
				CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_TXRQ;
				volatile int attempt = 0;
				while(CAN1->sTxMailBox[0].TIR & 0x1UL && attempt++ < 1000000);
			}
			else
			{
				CAN1 -> sTxMailBox[0].TDTR = mailbox.TX_data_length;
				CAN1 -> sTxMailBox[0].TDTR &= ~CAN_TDT0R_TGT;
				CAN1 -> sTxMailBox[0].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[0].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 | mailbox.TX_data[1] << 8 | mailbox.TX_data[0] << 0;
				CAN1 -> sTxMailBox[0].TIR  |= CAN_TI0R_TXRQ;
				volatile int attempt = 0;
				while(CAN1->sTxMailBox[0].TIR & 0x1UL && attempt++ < 1000000);
			}

			next_mailbox = 1;
		}
			break;

//=================================================================================
//===================================================================================
		case 1:
		{
			printConsole("Mailbox 2 \r\n");
			//===================================== ID Type	=====================================
			if(mailbox.TX_ID_Type == CAN_ID_Standard)
			{
				CAN1 -> sTxMailBox[1].TIR  = mailbox.TX_ID << 21;
				CAN1 -> sTxMailBox[1].TIR  &= ~CAN_TI1R_EXID;
			}

			if(mailbox.TX_ID_Type == CAN_ID_Extended)
			{
				CAN1 -> sTxMailBox[1].TIR  = mailbox.TX_ID << 3;
				CAN1 -> sTxMailBox[1].TIR  |= 1 << 2;
			}
			//===================================== Frame Type	=====================================
			if(mailbox.TX_Frame_Type == CAN_Frame_Data)
			{
				CAN1 -> sTxMailBox[1].TIR  &= ~CAN_TI1R_RTR;
			}

			if(mailbox.TX_Frame_Type == CAN_Frame_Remote)
			{
				CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_RTR;
			}
			//===================================== Frame Type	=====================================
			if(mailbox.timestamp_enable == CAN_Timestamp_Enable)
			{
				CAN1 -> sTxMailBox[1].TDTR = mailbox.TX_data_length | CAN_TDT1R_TGT;
				CAN1 -> sTxMailBox[1].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[1].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 ;
				CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_TXRQ;
				volatile int attempt = 0;
				while(CAN1->sTxMailBox[0].TIR & 0x1UL && attempt++ < 1000000);
			}
			else
			{
				CAN1 -> sTxMailBox[1].TDTR = mailbox.TX_data_length;
				CAN1 -> sTxMailBox[1].TDTR &= ~CAN_TDT1R_TGT;
				CAN1 -> sTxMailBox[1].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[1].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 | mailbox.TX_data[1] << 8 | mailbox.TX_data[0] << 0;
				CAN1 -> sTxMailBox[1].TIR  |= CAN_TI1R_TXRQ;
				volatile int attempt = 0;
				while(CAN1->sTxMailBox[1].TIR & 0x1UL && attempt++ < 1000000);
			}
			next_mailbox = 2;
		}
		break;
//=================================================================================
//===================================================================================
		case 2:
		{
			//===================================== ID Type	=====================================
			if(mailbox.TX_ID_Type == CAN_ID_Standard)
			{
				CAN1 -> sTxMailBox[2].TIR  = mailbox.TX_ID << 21;
				CAN1 -> sTxMailBox[2].TIR  &= ~CAN_TI2R_EXID;
			}

			if(mailbox.TX_ID_Type == CAN_ID_Extended)
			{
				CAN1 -> sTxMailBox[2].TIR  = mailbox.TX_ID << 3;
				CAN1 -> sTxMailBox[2].TIR  |= 1 << 2;
			}
			//===================================== Frame Type	=====================================
			if(mailbox.TX_Frame_Type == CAN_Frame_Data)
			{
				CAN1 -> sTxMailBox[2].TIR  &= ~CAN_TI2R_RTR;
			}

			if(mailbox.TX_Frame_Type == CAN_Frame_Remote)
			{
				CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_RTR;
			}
			//===================================== Frame Type	=====================================
			if(mailbox.timestamp_enable == CAN_Timestamp_Enable)
			{
				CAN1 -> sTxMailBox[2].TDTR = mailbox.TX_data_length | CAN_TDT2R_TGT;
				CAN1 -> sTxMailBox[2].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[2].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 ;
				CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_TXRQ;
				volatile int attempt = 0;
				while(CAN1->sTxMailBox[2].TIR & 0x1UL && attempt++ < 1000000);
			}
			else
			{
				CAN1 -> sTxMailBox[2].TDTR = mailbox.TX_data_length;
				CAN1 -> sTxMailBox[2].TDTR &= ~CAN_TDT1R_TGT;
				CAN1 -> sTxMailBox[2].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
				CAN1 -> sTxMailBox[2].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 | mailbox.TX_data[1] << 8 | mailbox.TX_data[0] << 0;
				CAN1 -> sTxMailBox[2].TIR  |= CAN_TI2R_TXRQ;
				volatile int attempt = 0;
				while(CAN1->sTxMailBox[2].TIR & 0x1UL && attempt++ < 1000000);
			}
			next_mailbox = 0;
		}
		break;
	}
}





void CAN_Flush_TX_Buffers(int buffer_id)
{
	switch (tx_or_rx_buffer) {
//////////////////////////////////////////////////////////////////////////////////////
		case CAN_TX_All_Buffers:
		{
#if _CAN_DEBUG_
			printConsole("Clearing All TX Buffers \r\n");
#endif
			for(int i = 0 ; i < 3; i++)
			{
				CAN1 -> sTxMailBox[i].TDHR &= ~0xFFFFFFFF;
				CAN1 -> sTxMailBox[i].TDLR &= ~0xFFFFFFFF;
				CAN1 -> sTxMailBox[i].TDTR &= ~0xFFFFFFFF;
				CAN1 -> sTxMailBox[i].TIR &= ~0xFFFFFFFF;
#if _CAN_DEBUG_
			printConsole("Buffer %d Cleared\r\n",i);
#endif
			}
#if _CAN_DEBUG_
			printConsole("All TX Buffers are cleared \r\n");
#endif
		}
			break;
//////////////////////////////////////////////////////////////////////////////////////
		case CAN_TX_Buffer_0:
		{
#if _CAN_DEBUG_
			printConsole("Clearing TX Buffer 0 \r\n");
#endif
			CAN1 -> sTxMailBox[0].TDHR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[0].TDLR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[0].TDTR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[0].TIR  &= ~0xFFFFFFFF;

#if _CAN_DEBUG_
			printConsole("TX Buffer 0 Cleared \r\n");
#endif
		}
			break;
//////////////////////////////////////////////////////////////////////////////////////
		case CAN_TX_Buffer_1:
		{
#if _CAN_DEBUG_
			printConsole("Clearing TX Buffer 1 \r\n");
#endif
			CAN1 -> sTxMailBox[1].TDHR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[1].TDLR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[1].TDTR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[1].TIR  &= ~0xFFFFFFFF;

#if _CAN_DEBUG_
			printConsole("TX Buffer 1 Cleared \r\n");
#endif
		}
			break;
		case CAN_TX_Buffer_2:
		{
#if _CAN_DEBUG_
			printConsole("Clearing TX Buffer 2 \r\n");
#endif
			CAN1 -> sTxMailBox[2].TDHR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[2].TDLR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[2].TDTR &= ~0xFFFFFFFF;
			CAN1 -> sTxMailBox[2].TIR  &= ~0xFFFFFFFF;

#if _CAN_DEBUG_
			printConsole("TX Buffer 2 Cleared \r\n");
#endif
		}
			break;
	}
}
