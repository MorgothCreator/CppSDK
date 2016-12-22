################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_core.c \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_ctlreq.c \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_ioreq.c \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_pipes.c 

OBJS += \
./stm32f4/driver/USBH/Core/Src/usbh_core.o \
./stm32f4/driver/USBH/Core/Src/usbh_ctlreq.o \
./stm32f4/driver/USBH/Core/Src/usbh_ioreq.o \
./stm32f4/driver/USBH/Core/Src/usbh_pipes.o 

C_DEPS += \
./stm32f4/driver/USBH/Core/Src/usbh_core.d \
./stm32f4/driver/USBH/Core/Src/usbh_ctlreq.d \
./stm32f4/driver/USBH/Core/Src/usbh_ioreq.d \
./stm32f4/driver/USBH/Core/Src/usbh_pipes.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f4/driver/USBH/Core/Src/usbh_core.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_core.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4/driver/USBH/Core/Src/usbh_ctlreq.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_ctlreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4/driver/USBH/Core/Src/usbh_ioreq.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_ioreq.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4/driver/USBH/Core/Src/usbh_pipes.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Core/Src/usbh_pipes.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


