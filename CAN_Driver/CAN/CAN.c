/*
 * CAN.c
 *
 *  Created on: May 26, 2022
 *      Author: Kunal
 */



#include "CAN.h"


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
void CAN_Init(CAN_Config mailbox)
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
	CAN1 -> BTR = mailbox.baudrate;
//	CAN1->BTR |= CAN_BTR_LBKM;  //SET LBKM BIT

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
     CAN1->MCR &= ~CAN_MCR_INRQ;
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while((CAN1->MSR & CAN_MSR_INAK));
}



/*
 * @func 			:	CAN_Flush_TX_Buffers
 * @rev				:	1
 * @Comment			:	None
 * @input param		:	buffer_id
 * @output param	:	none
 * @operation		:	Clears the TX buffers of CAN controller. The user can either
 * 						clear a single buffer or all buffers at the same time.
 */
void CAN_Flush_TX_Buffers(int buffer_id)
{
	switch (buffer_id) {
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


/*
 * @func 			:	CAN_TX_Payload
 * @rev				:	1
 * @Comment			:	None
 * @input param		:	mailbox_id
 * @output param	:	temp
 * @operation		:	Transmits the data in the mailbox
 */
int CAN_Send_Payload(CAN_Config mailbox)
{
	int mailbox_id;
//	int time_out = 0;
//******************************** Check empty mailbox ******************************
//====================================================================================
	if((CAN1 -> TSR & CAN_TSR_TME_Msk) == CAN_TSR_TME)
	{
		mailbox_id = CAN_TX_Mailbox_0;
	}
//====================================================================================
	if( ((CAN1 -> TSR & CAN_TSR_TME_Msk) == CAN_TSR_TME0) &&
		((CAN1 -> TSR & CAN_TSR_RQCP2_Msk) == CAN_TSR_RQCP2) &&
		((CAN1 -> TSR & CAN_TSR_TXOK2_Msk) == CAN_TSR_TXOK2)
	  )
	{
		CAN1->TSR |= CAN_TSR_RQCP2;
		CAN1->TSR |= CAN_TSR_TXOK2;
		mailbox_id = CAN_TX_Mailbox_0;
	}
//	if( (((CAN1 -> TSR & CAN_TSR_CODE) >> CAN_TSR_CODE_Msk) == 0))
//	{
//		CAN1->TSR |= CAN_TSR_RQCP2;
//		CAN1->TSR |= CAN_TSR_TXOK2;
//		mailbox_id = CAN_TX_Mailbox_0;
//	}
//====================================================================================
	if( ((CAN1 -> TSR & CAN_TSR_TME_Msk) == CAN_TSR_TME1) &&
		((CAN1 -> TSR & CAN_TSR_RQCP0_Msk) == CAN_TSR_RQCP0) &&
		((CAN1 -> TSR & CAN_TSR_TXOK0_Msk) == CAN_TSR_TXOK0)
	  )
	{
		CAN1->TSR |= CAN_TSR_RQCP0;
		CAN1->TSR |= CAN_TSR_TXOK0;
		mailbox_id = CAN_TX_Mailbox_1;
	}
//	if( (((CAN1 -> TSR & CAN_TSR_CODE) >> CAN_TSR_CODE_Msk) == 1))
//	{
//		CAN1->TSR |= CAN_TSR_RQCP2;
//		CAN1->TSR |= CAN_TSR_TXOK2;
//		mailbox_id = CAN_TX_Mailbox_1;
//	}
//====================================================================================
	if( ((CAN1 -> TSR & CAN_TSR_TME_Msk) == CAN_TSR_TME2) &&
		((CAN1 -> TSR & CAN_TSR_RQCP1_Msk) == CAN_TSR_RQCP1) &&
		((CAN1 -> TSR & CAN_TSR_TXOK1_Msk) == CAN_TSR_TXOK1)
	  )
	{
		CAN1->TSR |= CAN_TSR_TXOK1;
		CAN1->TSR |= CAN_TSR_RQCP1;
		mailbox_id = CAN_TX_Mailbox_2;
	}
//	if( (((CAN1 -> TSR & CAN_TSR_CODE) >> CAN_TSR_CODE_Msk) == 2))
//	{
//		CAN1->TSR |= CAN_TSR_RQCP2;
//		CAN1->TSR |= CAN_TSR_TXOK2;
//		mailbox_id = CAN_TX_Mailbox_2;
//	}
//====================================================================================
	if((CAN1 -> TSR & CAN_TSR_TME_Msk) != CAN_TSR_TME)
	{
		return -1;
	}


//******************************** Clear mailboxes ******************************
		CAN1 -> sTxMailBox[mailbox_id].TIR  &= ~0xFFFFFFFF;
		CAN1 -> sTxMailBox[mailbox_id].TDHR &= ~0xFFFFFFFF;
		CAN1 -> sTxMailBox[mailbox_id].TDLR &= ~0xFFFFFFFF;
		CAN1 -> sTxMailBox[mailbox_id].TDTR &= ~0xFFFFFFFF;
//******************************** Fill up the mailboxes ******************************

//================================	Fill-up ID Type	===============================
	switch (mailbox.TX_ID_Type)
	{
		case CAN_ID_Standard:
		{
			CAN1 -> sTxMailBox[mailbox_id].TIR  = mailbox.TX_ID << 21;
			CAN1 -> sTxMailBox[mailbox_id].TIR  &= ~1 << 2;
#if _CAN_DEBUG_
			printConsole("Standard ID : %d\r\n",mailbox.TX_ID);
#endif
		}
			break;
		case CAN_ID_Extended:
		{
			CAN1 -> sTxMailBox[mailbox_id].TIR  = mailbox.TX_ID << 3;
			CAN1 -> sTxMailBox[mailbox_id].TIR  |= 1 << 2;
#if _CAN_DEBUG_
			printConsole("Extended ID : %d\r\n",mailbox.TX_ID);
#endif
		}
			break;
	}

//================================ Fill-up Frame Type ===============================
	switch (mailbox.TX_Frame_Type)
	{
		case CAN_Frame_Data:
		{
			CAN1 -> sTxMailBox[mailbox_id].TIR  &= ~(1 << 1);
#if _CAN_DEBUG_
			printConsole("Frame Type : Data\r\n");
#endif
		}
			break;
		case CAN_Frame_Remote:
		{
			CAN1 -> sTxMailBox[mailbox_id].TIR  |= (1 << 1);
#if _CAN_DEBUG_
			printConsole("Frame Type : Remote\r\n");
#endif
		}
			break;
	}

//================================ Fill-up Actual Data ===============================
	if(mailbox.timestamp_enable == CAN_Timestamp_Enable)
	{
		CAN1 -> sTxMailBox[mailbox_id].TDTR = mailbox.TX_data_length | (1 << 8);
		CAN1 -> sTxMailBox[mailbox_id].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
		CAN1 -> sTxMailBox[mailbox_id].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 ;
		CAN1 -> sTxMailBox[mailbox_id].TIR  |= (1 << 0);
		while(CAN1 -> sTxMailBox[mailbox_id].TIR & (1 << 0)){}
#if _CAN_DEBUG_
			printConsole("Data  : %x, %x, %x, %x, %x\r\n",mailbox.TX_data[0],mailbox.TX_data[1],mailbox.TX_data[2],mailbox.TX_data[3],mailbox.TX_data[4],mailbox.TX_data[5]);
			printConsole("Time_Stamp : %"PRIx32"\r\n",((CAN1 -> sTxMailBox[0].TDTR) >> 16));
#endif

	}
	else
	{
		CAN1 -> sTxMailBox[mailbox_id].TDTR = mailbox.TX_data_length;
		CAN1 -> sTxMailBox[mailbox_id].TDTR &= ~CAN_TDT0R_TGT;
		CAN1 -> sTxMailBox[mailbox_id].TDHR = mailbox.TX_data[7] << 24 | mailbox.TX_data[6] << 16 | mailbox.TX_data[5] << 8 | mailbox.TX_data[4] << 0;
		CAN1 -> sTxMailBox[mailbox_id].TDLR = mailbox.TX_data[3] << 24 | mailbox.TX_data[2] << 16 | mailbox.TX_data[1] << 8 | mailbox.TX_data[0] << 0;
		CAN1 -> sTxMailBox[mailbox_id].TIR  |= (1 << 0);
//		volatile int attempt = 0;
//		while(CAN1 -> sTxMailBox[mailbox_id].TIR & (1 << 0));
#if _CAN_DEBUG_
		printConsole("Data  : %d, %d, %d, %d, %d, %d, %d\r\n",mailbox.TX_data[0],mailbox.TX_data[1],mailbox.TX_data[2],mailbox.TX_data[3],mailbox.TX_data[4],mailbox.TX_data[5],mailbox.TX_data[6],mailbox.TX_data[7]);
#endif
	}


//	while(time_out < 10000){ time_out++; }


	if(mailbox_id == CAN_TX_Mailbox_0 && (CAN1->TSR & CAN_TSR_TXOK0) && (CAN1->TSR & CAN_TSR_TME0) && (CAN1->TSR & CAN_TSR_RQCP0)){
		return (mailbox_id = CAN_TX_Mailbox_0);
	}
	if(mailbox_id == CAN_TX_Mailbox_1 && (CAN1->TSR & CAN_TSR_TXOK1) && (CAN1->TSR & CAN_TSR_TME1) && (CAN1->TSR & CAN_TSR_RQCP1)){
		return (mailbox_id = CAN_TX_Mailbox_1);
	}
	if(mailbox_id == CAN_TX_Mailbox_2 && (CAN1->TSR & CAN_TSR_TXOK2) && (CAN1->TSR & CAN_TSR_TME2) && (CAN1->TSR & CAN_TSR_RQCP2)){
		return (mailbox_id = CAN_TX_Mailbox_2);
	}

	return 10;
}


/*
 * @func 			:	CAN_TX_Payload
 * @rev				:	1
 * @Comment			:	None
 * @input param		:	mailbox_id
 * @output param	:	temp
 * @operation		:	Transmits the data in the mailbox
 */
int CAN_Get_Payload(CAN_Config mailbox)
{
	int fifo1_full = 0;
	int fifo2_full = 0;

	int frame_type = 0;
	int id_type = 0;

	fifo1_full = (CAN1 -> RF0R & CAN_RF0R_FMP0_Msk) >> CAN_RF0R_FMP0_Pos;

	fifo2_full = (CAN1 -> RF1R & CAN_RF1R_FMP1_Msk) >> CAN_RF1R_FMP1_Pos;

	if(fifo1_full)
	{
		RX_Mailbox_1.ID = 0;
		RX_Mailbox_1.id_type = 0;
		RX_Mailbox_1.data[0] = 0;
		RX_Mailbox_1.data[1] = 0;
		RX_Mailbox_1.data[2] = 0;
		RX_Mailbox_1.data[3] = 0;
		RX_Mailbox_1.data[4] = 0;
		RX_Mailbox_1.data[5] = 0;
		RX_Mailbox_1.data[6] = 0;
		RX_Mailbox_1.data[7] = 0;
		RX_Mailbox_1.data_length = 0;

		GPIOA -> BSRR |= GPIO_BSRR_BS0;
		GPIOC -> BSRR |= GPIO_BSRR_BR13;
		id_type =  (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_IDE_Msk) >> CAN_RI0R_IDE_Pos ;
		frame_type = (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_RTR_Msk) >> CAN_RI0R_RTR_Pos ;

		if(id_type)
		{
			//Extended ID
			RX_Mailbox_1.id_type = CAN_ID_Extended;
			RX_Mailbox_1.ID = (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_EXID_Msk) >> CAN_RI0R_EXID_Pos;
		}
		else
		{
			//Standard ID
			RX_Mailbox_1.id_type = CAN_ID_Standard;
			RX_Mailbox_1.ID = (CAN1 -> sFIFOMailBox[0].RIR & CAN_RI0R_STID_Msk) >> CAN_RI0R_STID_Pos;
//			printConsole("ID = %d \r\n",RX_Mailbox_1.ID);
		}

		if(mailbox.timestamp_enable)
		{
			RX_Mailbox_1.message_timestamp = (CAN1 -> sFIFOMailBox[0].RDTR & CAN_RDT0R_TIME_Msk) >> CAN_RDT0R_TIME_Pos;
		}

		if(frame_type)
		{
			//RTR Frame
			RX_Mailbox_1.frame_type = CAN_Frame_Remote;
			RX_Mailbox_1.data_length = (CAN1 -> sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;
			CAN1 -> RF0R |= CAN_RF0R_RFOM0;
			while((CAN1 -> RF0R & CAN_RF0R_RFOM0)){}
		}
		else
		{
			//Data Frame
			RX_Mailbox_1.frame_type = CAN_Frame_Data;
			RX_Mailbox_1.data_length = (CAN1 -> sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos;
			for(int i = 0; i < RX_Mailbox_1.data_length; i++)
			{
				if(i < 4)
				{
					RX_Mailbox_1.data[i] =  (CAN1 -> sFIFOMailBox[0].RDLR & ( 0xFF << (8*i))) >> (8*i);
				}
				else
				{
					RX_Mailbox_1.data[i] =  (CAN1 -> sFIFOMailBox[0].RDHR & ( 0xFF << (8*(i-4)))) >> (8*(i-4));
				}
			}

			CAN1 -> RF0R |= CAN_RF0R_RFOM0;
			while((CAN1 -> RF0R & CAN_RF0R_RFOM0)){}
		}
		return CAN_RX_Buffer_1;
	}

	if(fifo2_full)
	{
		id_type =  (CAN1 -> sFIFOMailBox[1].RIR & CAN_RI0R_IDE_Msk) >> CAN_RI0R_IDE_Pos ;
		frame_type = (CAN1 -> sFIFOMailBox[1].RIR & CAN_RI0R_RTR_Msk) >> CAN_RI0R_RTR_Pos ;

		if(id_type)
		{
			//Extended ID
			RX_Mailbox_2.id_type = CAN_ID_Extended;
			RX_Mailbox_2.ID = (CAN1 -> sFIFOMailBox[1].RIR & CAN_RI1R_EXID_Msk) >> CAN_RI1R_EXID_Pos;
		}
		else
		{
			//Standard ID
			RX_Mailbox_2.id_type = CAN_ID_Standard;
			RX_Mailbox_2.ID = (CAN1 -> sFIFOMailBox[1].RIR & CAN_RI1R_STID_Msk) >> CAN_RI1R_STID_Pos;
		}

		if(mailbox.timestamp_enable)
		{
			RX_Mailbox_2.message_timestamp = (CAN1 -> sFIFOMailBox[1].RDTR & CAN_RDT1R_TIME_Msk) >> CAN_RDT1R_TIME_Pos;
		}

		if(frame_type)
		{
			//RTR Frame
			RX_Mailbox_2.frame_type = CAN_Frame_Remote;
			RX_Mailbox_2.data_length = (CAN1 -> sFIFOMailBox[1].RDTR & CAN_RDT1R_DLC_Msk) >> CAN_RDT1R_DLC_Pos;
			CAN1 -> RF1R |= CAN_RF1R_RFOM1;
			while((CAN1 -> RF1R & CAN_RF1R_RFOM1)){}
		}
		else
		{
			//Data Frame
			RX_Mailbox_2.frame_type = CAN_Frame_Data;
			RX_Mailbox_2.data_length = (CAN1 -> sFIFOMailBox[1].RDTR & CAN_RDT1R_DLC_Msk) >> CAN_RDT1R_DLC_Pos;
			for(int i = 0; i < RX_Mailbox_2.data_length; i++)
			{
				if(i < 4)
				{
					RX_Mailbox_2.data[i] =  (CAN1 -> sFIFOMailBox[1].RDLR & ( 0xFF << (8*i))) >> (8*i);
				}
				else
				{
					RX_Mailbox_2.data[i] =  (CAN1 -> sFIFOMailBox[1].RDHR & ( 0xFF << (8*i))) >> (8*i);
				}

				CAN1 -> RF1R |= CAN_RF1R_RFOM1;
				while((CAN1 -> RF1R & CAN_RF1R_RFOM1)){}
			}
		}

		return CAN_RX_Buffer_2;
	}

	return -1;
}
