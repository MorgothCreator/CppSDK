################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/GitHub/CppSDK/SDK/lib/app/lwip/ftpserver/ftpd.cpp \
C:/GitHub/CppSDK/SDK/lib/app/lwip/ftpserver/vfs.cpp 

OBJS += \
./lib/app/lwip/ftpserver/ftpd.o \
./lib/app/lwip/ftpserver/vfs.o 

CPP_DEPS += \
./lib/app/lwip/ftpserver/ftpd.d \
./lib/app/lwip/ftpserver/vfs.d 


# Each subdirectory must supply rules for building sources it contributes
lib/app/lwip/ftpserver/ftpd.o: C:/GitHub/CppSDK/SDK/lib/app/lwip/ftpserver/ftpd.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/app/lwip/ftpserver/vfs.o: C:/GitHub/CppSDK/SDK/lib/app/lwip/ftpserver/vfs.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


