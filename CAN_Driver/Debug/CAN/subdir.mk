################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CAN/CAN.c \
../CAN/CANOpen_V1.c 

OBJS += \
./CAN/CAN.o \
./CAN/CANOpen_V1.o 

C_DEPS += \
./CAN/CAN.d \
./CAN/CANOpen_V1.d 


# Each subdirectory must supply rules for building sources it contributes
CAN/%.o: ../CAN/%.c CAN/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"C:/Users/Kunal/STM32CubeIDE/Kunal/STM32F103_DRIVER/CAN_Driver/GPIO" -I"C:/Users/Kunal/STM32CubeIDE/Kunal/STM32F103_DRIVER/CAN_Driver/CAN" -I"C:/Users/Kunal/STM32CubeIDE/Kunal/STM32F103_DRIVER/CAN_Driver/Console" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-CAN

clean-CAN:
	-$(RM) ./CAN/CAN.d ./CAN/CAN.o ./CAN/CANOpen_V1.d ./CAN/CANOpen_V1.o

.PHONY: clean-CAN

