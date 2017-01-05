################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/GitHub/CppSDK/SDK/lib/lib/fs/fat/src/diskio.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/fs/fat/src/fattime.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/fs/fat/src/ff.cpp 

OBJS += \
./lib/lib/fs/fat/src/diskio.o \
./lib/lib/fs/fat/src/fattime.o \
./lib/lib/fs/fat/src/ff.o 

CPP_DEPS += \
./lib/lib/fs/fat/src/diskio.d \
./lib/lib/fs/fat/src/fattime.d \
./lib/lib/fs/fat/src/ff.d 


# Each subdirectory must supply rules for building sources it contributes
lib/lib/fs/fat/src/diskio.o: C:/GitHub/CppSDK/SDK/lib/lib/fs/fat/src/diskio.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/fs/fat/src/fattime.o: C:/GitHub/CppSDK/SDK/lib/lib/fs/fat/src/fattime.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/fs/fat/src/ff.o: C:/GitHub/CppSDK/SDK/lib/lib/fs/fat/src/ff.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


