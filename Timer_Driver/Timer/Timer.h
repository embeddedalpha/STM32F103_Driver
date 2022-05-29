/*
 * Timer.h
 *
 *  Created on: 28-May-2022
 *      Author: Kunal
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "main.h"
#include "GPIO.h"



void Timer_Input_PWM(TIM_TypeDef *Timer);

void Get_Input_PWM(TIM_TypeDef *Timer, float *period, float *frequency);

#endif /* TIMER_H_ */
