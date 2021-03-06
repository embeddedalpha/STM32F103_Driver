/**
 ******************************************************************************
 * @file           : main.h
 * @author         : Kunal Salvi
 * @brief          : Header for Important Functions
 ******************************************************************************
 *
 ******************************************************************************
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include "stm32f103xb.h"
#include "stm32f1xx.h"

uint32_t APB1CLK_SPEED;
uint32_t APB2CLK_SPEED;

__STATIC_INLINE int32_t SystemAPB1_Clock_Speed(void)
{
	return (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1)>> RCC_CFGR_PPRE1_Pos]);
}

__STATIC_INLINE int32_t SystemAPB2_Clock_Speed(void)
{
	return (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2)>> RCC_CFGR_PPRE2_Pos]);
}

__STATIC_INLINE void MCU_Clock_Init(void)
{
	SystemInit();
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



#endif /* MAIN_H_ */
