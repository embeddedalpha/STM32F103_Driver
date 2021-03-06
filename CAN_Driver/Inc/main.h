/**
 ******************************************************************************
 * @file           : main.h
 * @author         : Kunal Salvi
 * @brief          : Header for Important Functions
 ******************************************************************************
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "inttypes.h"


uint32_t APB1CLK_SPEED;
uint32_t APB2CLK_SPEED;

#define Use_External_Crystal 1
#define Use_Internal_Crystal 2

__STATIC_INLINE int32_t SystemAPB1_Clock_Speed(void)
{
	return (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1)>> RCC_CFGR_PPRE1_Pos]);
}

__STATIC_INLINE int32_t SystemAPB2_Clock_Speed(void)
{
	return (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2)>> RCC_CFGR_PPRE2_Pos]);
}

__STATIC_INLINE void MCU_Clock_Init(int crystal)
{
	SystemInit();
	switch (crystal) {
		case Use_External_Crystal:
		{
			RCC->CR |= RCC_CR_HSEON ;
			while(!(RCC->CR & RCC_CR_HSERDY));
			FLASH->ACR |= FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_2;
			RCC->CFGR |= RCC_CFGR_PLLSRC;
			RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE ;
			RCC->CFGR |= RCC_CFGR_PLLMULL9;
			RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
			RCC->CR |= RCC_CR_PLLON;
			while(!(RCC->CR & RCC_CR_PLLRDY));
			RCC->CFGR |= RCC_CFGR_SW_PLL;
			while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
			SystemCoreClockUpdate();
			APB1CLK_SPEED = SystemAPB1_Clock_Speed();
			APB2CLK_SPEED = SystemAPB2_Clock_Speed();
		}
			break;

		case Use_Internal_Crystal:
		{
			SystemInit();
			FLASH->ACR |= FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_2;
			RCC->CFGR &= ~RCC_CFGR_PLLSRC;
			RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE ;
			RCC->CFGR |= RCC_CFGR_PLLMULL16;
			RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
			RCC->CR |= RCC_CR_PLLON;
			while(!(RCC->CR & RCC_CR_PLLRDY));
			RCC->CFGR |= RCC_CFGR_SW_PLL;
			while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
			SystemCoreClockUpdate();
			APB1CLK_SPEED = SystemAPB1_Clock_Speed();
			APB2CLK_SPEED = SystemAPB2_Clock_Speed();
		}
			break;
	}
}

__STATIC_INLINE uint32_t Delay_Config(void)
{

	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL |= 5;
	return (0UL);                                                     /* Function successful */
}

__STATIC_INLINE uint32_t Delay_ns500(void)
{

	SysTick->LOAD = 192;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
	return (0UL);                                                     /* Function successful */
}


__STATIC_INLINE uint32_t Delay_us(float us)
{

	SysTick->LOAD = (SystemCoreClock / 1000000) * us;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
	return (0UL);                                                     /* Function successful */
}

__STATIC_INLINE uint32_t Delay_ms(unsigned long ms)
{
	unsigned long x = (SystemCoreClock / 1000) * (ms);
	SysTick->LOAD = x ;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
	return (0UL);                                                     /* Function successful */
}


__STATIC_INLINE uint32_t Delay_s(unsigned long s)
{
	s = s * 1000;
	for (; s>0; s--)
	{
		Delay_ms(1);
	}
	return (0UL);
}



__STATIC_INLINE void convert_float_to_char(float number,uint8_t buffer[])
{
	unsigned char *ptr, i;
	ptr  = (unsigned char *)&number;
	for(i = 0; i < sizeof(float); i++)
	{
		buffer[i] = *(ptr + i);
	}
}

__STATIC_INLINE void convert_float_to_int(float number,uint8_t buffer[])
{
	unsigned int *ptr, i;
	ptr  = (unsigned int *)&number;
	for(i = 0; i < sizeof(float); i++)
	{
		buffer[i] = *(ptr + i);
	}
}




#endif /* MAIN_H_ */
