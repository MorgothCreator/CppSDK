################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f7/driver/USBD/Class/usbd_conf.c 

OBJS += \
./stm32f7/driver/USBD/Class/usbd_conf.o 

C_DEPS += \
./stm32f7/driver/USBD/Class/usbd_conf.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f7/driver/USBD/Class/usbd_conf.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/driver/USBD/Class/usbd_conf.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


