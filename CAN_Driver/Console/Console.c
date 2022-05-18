/*
 * Console.c
 *
 *  Created on: 17-May-2022
 *      Author: Kunal
 */


#include "Console.h"


void Console_Init(int32_t baudrate)
{
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
	GPIO_Pin_Setup(GPIOA, 9, 0b1011);
	GPIO_Pin_Setup(GPIOA, 10, 0b1011);
	USART1 -> CR1 |= USART_CR1_UE;
	USART1 -> BRR = (int)(SystemCoreClock / (16 * baudrate)) << 4;
	USART1 -> CR1 |= USART_CR1_TE ;
	USART1 -> CR1 |=  USART_CR1_RE  ;
}


void printConsole(char *msg, ...)
{
	char buff[10000];
	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);

	for(int i = 0; i<= strlen(buff); i++)
	{
		USART1 -> DR = buff[i];
		while (!(USART1->SR & USART_SR_TXE));
	}

}
