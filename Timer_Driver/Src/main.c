
#include <stdint.h>
#include "main.h"
#include "GPIO.h"
#include "Console.h"
#include "Timer.h"

//-u_printf_float


int main(void)
{
	MCU_Clock_Init(Use_External_Crystal);
	Console_Init(115200);

	Timer_Input_PWM(TIM1);

	float freq;
	float period;

    /* Loop forever */
	for(;;)
	{
		Get_Input_PWM(TIM1, &period, &freq);
	}
}
