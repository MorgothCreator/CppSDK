################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Class/MSC/ff_gen_drv.c \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Class/MSC/usbh_conf.c 

OBJS += \
./stm32f4/driver/USBH/Class/MSC/ff_gen_drv.o \
./stm32f4/driver/USBH/Class/MSC/usbh_conf.o 

C_DEPS += \
./stm32f4/driver/USBH/Class/MSC/ff_gen_drv.d \
./stm32f4/driver/USBH/Class/MSC/usbh_conf.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f4/driver/USBH/Class/MSC/ff_gen_drv.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Class/MSC/ff_gen_drv.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4/driver/USBH/Class/MSC/usbh_conf.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBH/Class/MSC/usbh_conf.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


