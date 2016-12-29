################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/3d.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/PasswordWindowNumeric.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/bitmap.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/button.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/checkbox.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/controls_definition.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/gfx_gui_paint.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/gfx_util.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/graphic_string.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/item.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/keyboard.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/listbox.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/picturebox.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/progressbar.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/scrollbar.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/string.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/textbox.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/util.cpp \
C:/GitHub/CppSDK/SDK/lib/lib/gfx/window.cpp 

OBJS += \
./lib/lib/gfx/3d.o \
./lib/lib/gfx/PasswordWindowNumeric.o \
./lib/lib/gfx/bitmap.o \
./lib/lib/gfx/button.o \
./lib/lib/gfx/checkbox.o \
./lib/lib/gfx/controls_definition.o \
./lib/lib/gfx/gfx_gui_paint.o \
./lib/lib/gfx/gfx_util.o \
./lib/lib/gfx/graphic_string.o \
./lib/lib/gfx/item.o \
./lib/lib/gfx/keyboard.o \
./lib/lib/gfx/listbox.o \
./lib/lib/gfx/picturebox.o \
./lib/lib/gfx/progressbar.o \
./lib/lib/gfx/scrollbar.o \
./lib/lib/gfx/string.o \
./lib/lib/gfx/textbox.o \
./lib/lib/gfx/util.o \
./lib/lib/gfx/window.o 

CPP_DEPS += \
./lib/lib/gfx/3d.d \
./lib/lib/gfx/PasswordWindowNumeric.d \
./lib/lib/gfx/bitmap.d \
./lib/lib/gfx/button.d \
./lib/lib/gfx/checkbox.d \
./lib/lib/gfx/controls_definition.d \
./lib/lib/gfx/gfx_gui_paint.d \
./lib/lib/gfx/gfx_util.d \
./lib/lib/gfx/graphic_string.d \
./lib/lib/gfx/item.d \
./lib/lib/gfx/keyboard.d \
./lib/lib/gfx/listbox.d \
./lib/lib/gfx/picturebox.d \
./lib/lib/gfx/progressbar.d \
./lib/lib/gfx/scrollbar.d \
./lib/lib/gfx/string.d \
./lib/lib/gfx/textbox.d \
./lib/lib/gfx/util.d \
./lib/lib/gfx/window.d 


# Each subdirectory must supply rules for building sources it contributes
lib/lib/gfx/3d.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/3d.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/PasswordWindowNumeric.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/PasswordWindowNumeric.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/bitmap.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/bitmap.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/button.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/button.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/checkbox.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/checkbox.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/controls_definition.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/controls_definition.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/gfx_gui_paint.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/gfx_gui_paint.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/gfx_util.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/gfx_util.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/graphic_string.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/graphic_string.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/item.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/item.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/keyboard.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/keyboard.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/listbox.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/listbox.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/picturebox.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/picturebox.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/progressbar.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/progressbar.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/scrollbar.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/scrollbar.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/string.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/string.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/textbox.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/textbox.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/util.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/util.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

lib/lib/gfx/window.o: C:/GitHub/CppSDK/SDK/lib/lib/gfx/window.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


