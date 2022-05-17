#ifndef _GPIO_H_
#define _GPIO_H_

#include "main.h"


#define GEN_PURPOSE_OUTPUT_PUSH_PULL	0x03
#define GEN_PURPOSE_OUTPUT_OPEN_DRAIN	0x07
#define ALT_FUNCTION_OUTPUT_PUSH_PULL	0x0B
#define ALT_FUNCTION_OUTPUT_OPEN_DRAIN	0x0F
#define INPUT_ANALOG                    0x00
#define INPUT_FLOATING                  0x04
#define INPUT_PULL_UP_DOWN              0x80


void GPIO_Pin_Setup(GPIO_TypeDef *Port, uint8_t pin, uint8_t function);

void GPIO_Interrupt_Setup(int pin, int edge_select);

#endif
