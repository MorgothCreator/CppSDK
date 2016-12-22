################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/lib/lib/buffers/ring_buff.c 

OBJS += \
./lib/lib/buffers/ring_buff.o 

C_DEPS += \
./lib/lib/buffers/ring_buff.d 


# Each subdirectory must supply rules for building sources it contributes
lib/lib/buffers/ring_buff.o: C:/GitHub/CppSDK/SDK/lib/lib/buffers/ring_buff.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


