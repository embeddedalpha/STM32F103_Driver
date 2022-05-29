################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Console/Console.c 

OBJS += \
./Console/Console.o 

C_DEPS += \
./Console/Console.d 


# Each subdirectory must supply rules for building sources it contributes
Console/%.o: ../Console/%.c Console/subdir.mk
	arm-none-eabi-gcc -u_printf_float "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"C:/Users/Kunal/STM32CubeIDE/Kunal/STM32F103_DRIVER/Timer_Driver/Console" -I"C:/Users/Kunal/STM32CubeIDE/Kunal/STM32F103_DRIVER/Timer_Driver/GPIO" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Console

clean-Console:
	-$(RM) ./Console/Console.d ./Console/Console.o

.PHONY: clean-Console

