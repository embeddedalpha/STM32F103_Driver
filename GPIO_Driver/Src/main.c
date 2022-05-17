/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Kunal Salvi
 * @brief          : Main program body
 ******************************************************************************
 *
 ******************************************************************************
 */

#include "main.h"
#include "GPIO.h"


int main(void)
{
	MCU_Clock_Init();
	GPIO_Pin_Setup(GPIOA, 0, GEN_PURPOSE_OUTPUT_PUSH_PULL);

	for(;;);
}
