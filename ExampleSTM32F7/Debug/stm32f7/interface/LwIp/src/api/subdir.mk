################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/api_lib.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/api_msg.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/err.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/netbuf.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/netdb.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/netifapi.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/pppapi.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/sockets.c \
C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/tcpip.c 

OBJS += \
./stm32f7/interface/LwIp/src/api/api_lib.o \
./stm32f7/interface/LwIp/src/api/api_msg.o \
./stm32f7/interface/LwIp/src/api/err.o \
./stm32f7/interface/LwIp/src/api/netbuf.o \
./stm32f7/interface/LwIp/src/api/netdb.o \
./stm32f7/interface/LwIp/src/api/netifapi.o \
./stm32f7/interface/LwIp/src/api/pppapi.o \
./stm32f7/interface/LwIp/src/api/sockets.o \
./stm32f7/interface/LwIp/src/api/tcpip.o 

C_DEPS += \
./stm32f7/interface/LwIp/src/api/api_lib.d \
./stm32f7/interface/LwIp/src/api/api_msg.d \
./stm32f7/interface/LwIp/src/api/err.d \
./stm32f7/interface/LwIp/src/api/netbuf.d \
./stm32f7/interface/LwIp/src/api/netdb.d \
./stm32f7/interface/LwIp/src/api/netifapi.d \
./stm32f7/interface/LwIp/src/api/pppapi.d \
./stm32f7/interface/LwIp/src/api/sockets.d \
./stm32f7/interface/LwIp/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f7/interface/LwIp/src/api/api_lib.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/api_lib.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/api_msg.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/api_msg.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/err.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/err.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/netbuf.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/netbuf.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/netdb.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/netdb.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/netifapi.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/netifapi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/pppapi.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/pppapi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/sockets.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/sockets.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f7/interface/LwIp/src/api/tcpip.o: C:/GitHub/CppSDK/SDK/platform/stm32f7/interface/LwIp/src/api/tcpip.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32F769NIHx -DSTM32F769I_DISCO -DSTM32F7 -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/ExampleSTM32F7" -I"C:/GitHub/CppSDK/SDK/platform/stm32f7" -I"C:/GitHub/CppSDK/SDK/lib" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


