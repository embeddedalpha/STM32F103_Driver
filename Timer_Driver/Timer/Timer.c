/*
 * Timer.c
 *
 *  Created on: 28-May-2022
 *      Author: Kunal
 */


#include "Timer.h"



void Timer_Input_PWM(TIM_TypeDef *Timer)
{
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_IOPAEN;
	Timer -> PSC = 72-1;
	Timer -> ARR = 0xFFFF;
	Timer -> CCMR1 |= TIM_CCMR1_CC1S_0; //IC1 mapped to T1
	Timer -> CCMR1 |= TIM_CCMR1_CC2S_1; //IC2 mapped to T1
	Timer -> CCMR1 |= (TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1);
	Timer -> CCMR1 |= (TIM_CCMR1_IC2F_0 | TIM_CCMR1_IC2F_1);
	Timer -> CCER &= ~TIM_CCER_CC1P; //non inverted
	Timer -> CCER |= TIM_CCER_CC2P; //inverted
	Timer -> SMCR |= TIM_SMCR_TS_2 | TIM_SMCR_TS_0; //trigger select
	Timer -> SMCR |= TIM_SMCR_SMS_2;
	Timer -> CCMR1 &= ~TIM_CCMR1_IC1PSC;
	Timer -> CCMR1 &= ~TIM_CCMR1_IC2PSC;
	Timer -> CCER |= TIM_CCER_CC1E;
	Timer -> CCER |= TIM_CCER_CC2E;
	Timer -> CR1 |= TIM_CR1_CEN;
}


void Get_Input_PWM(TIM_TypeDef *Timer, float *period, float *frequency)
{
	float ms = 1.0/1000000.0;

	*frequency = ms * (float)(Timer->CCR1);
	*period =    ms * (float)(Timer->CCR2);

}
