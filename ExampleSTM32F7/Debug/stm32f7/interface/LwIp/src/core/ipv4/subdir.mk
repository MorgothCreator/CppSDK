################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/autoip.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/dhcp.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/icmp.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/igmp.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/ip4.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/ip4_addr.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/ip_frag.c 

OBJS += \
./stm32f7/interface/LwIp/src/core/ipv4/autoip.o \
./stm32f7/interface/LwIp/src/core/ipv4/dhcp.o \
./stm32f7/interface/LwIp/src/core/ipv4/icmp.o \
./stm32f7/interface/LwIp/src/core/ipv4/igmp.o \
./stm32f7/interface/LwIp/src/core/ipv4/ip4.o \
./stm32f7/interface/LwIp/src/core/ipv4/ip4_addr.o \
./stm32f7/interface/LwIp/src/core/ipv4/ip_frag.o 

C_DEPS += \
./stm32f7/interface/LwIp/src/core/ipv4/autoip.d \
./stm32f7/interface/LwIp/src/core/ipv4/dhcp.d \
./stm32f7/interface/LwIp/src/core/ipv4/icmp.d \
./stm32f7/interface/LwIp/src/core/ipv4/igmp.d \
./stm32f7/interface/LwIp/src/core/ipv4/ip4.d \
./stm32f7/interface/LwIp/src/core/ipv4/ip4_addr.d \
./stm32f7/interface/LwIp/src/core/ipv4/ip_frag.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f7/interface/LwIp/src/core/ipv4/autoip.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/autoip.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/ipv4/dhcp.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/dhcp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/ipv4/icmp.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/icmp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/ipv4/igmp.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/igmp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/ipv4/ip4.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/ip4.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/ipv4/ip4_addr.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/ip4_addr.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/ipv4/ip_frag.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/ipv4/ip_frag.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


