#include "GPIO.h"



void GPIO_Pin_Setup(GPIO_TypeDef *Port, uint8_t pin, uint8_t function)
{
	if(pin < 8)
	{
		Port -> CRL |= function << pin;
	}
	else
	{
		Port -> CRH |= function << pin;
	}
}

void GPIO_Interrupt_Setup(int pin, int edge_select)
{
	EXTI ->IMR |= 1 << pin;
	switch (edge_select) {
		case 0:
			EXTI ->RTSR |= 1 << pin;
			break;
		case 1:
			EXTI ->FTSR |= 1 << pin;
			break;
		case 2:
			EXTI ->RTSR |= 1 << pin;
			EXTI ->FTSR |= 1 << pin;
			break;
	}

	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_EnableIRQ(EXTI0_IRQn);
}
