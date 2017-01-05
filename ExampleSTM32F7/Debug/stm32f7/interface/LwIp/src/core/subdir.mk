################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/def.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/dns.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/inet_chksum.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/init.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/lwip_timers.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/mem.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/memp.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/netif.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/pbuf.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/raw.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/stats.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/sys.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/tcp.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/tcp_in.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/tcp_out.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/udp.c 

OBJS += \
./stm32f7/interface/LwIp/src/core/def.o \
./stm32f7/interface/LwIp/src/core/dns.o \
./stm32f7/interface/LwIp/src/core/inet_chksum.o \
./stm32f7/interface/LwIp/src/core/init.o \
./stm32f7/interface/LwIp/src/core/lwip_timers.o \
./stm32f7/interface/LwIp/src/core/mem.o \
./stm32f7/interface/LwIp/src/core/memp.o \
./stm32f7/interface/LwIp/src/core/netif.o \
./stm32f7/interface/LwIp/src/core/pbuf.o \
./stm32f7/interface/LwIp/src/core/raw.o \
./stm32f7/interface/LwIp/src/core/stats.o \
./stm32f7/interface/LwIp/src/core/sys.o \
./stm32f7/interface/LwIp/src/core/tcp.o \
./stm32f7/interface/LwIp/src/core/tcp_in.o \
./stm32f7/interface/LwIp/src/core/tcp_out.o \
./stm32f7/interface/LwIp/src/core/udp.o 

C_DEPS += \
./stm32f7/interface/LwIp/src/core/def.d \
./stm32f7/interface/LwIp/src/core/dns.d \
./stm32f7/interface/LwIp/src/core/inet_chksum.d \
./stm32f7/interface/LwIp/src/core/init.d \
./stm32f7/interface/LwIp/src/core/lwip_timers.d \
./stm32f7/interface/LwIp/src/core/mem.d \
./stm32f7/interface/LwIp/src/core/memp.d \
./stm32f7/interface/LwIp/src/core/netif.d \
./stm32f7/interface/LwIp/src/core/pbuf.d \
./stm32f7/interface/LwIp/src/core/raw.d \
./stm32f7/interface/LwIp/src/core/stats.d \
./stm32f7/interface/LwIp/src/core/sys.d \
./stm32f7/interface/LwIp/src/core/tcp.d \
./stm32f7/interface/LwIp/src/core/tcp_in.d \
./stm32f7/interface/LwIp/src/core/tcp_out.d \
./stm32f7/interface/LwIp/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f7/interface/LwIp/src/core/def.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/def.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/dns.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/dns.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/inet_chksum.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/inet_chksum.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/init.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/init.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/lwip_timers.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/lwip_timers.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/mem.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/mem.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/memp.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/memp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/netif.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/netif.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/pbuf.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/pbuf.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/raw.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/raw.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/stats.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/stats.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/sys.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/sys.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/tcp.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/tcp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/tcp_in.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/tcp_in.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/tcp_out.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/tcp_out.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/core/udp.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/core/udp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


