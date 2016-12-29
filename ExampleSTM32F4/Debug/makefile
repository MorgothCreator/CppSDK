################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include stm32f4/sys/subdir.mk
-include stm32f4/interface/LwIp/src/netif/ppp/polarssl/subdir.mk
-include stm32f4/interface/LwIp/src/netif/ppp/subdir.mk
-include stm32f4/interface/LwIp/src/netif/subdir.mk
-include stm32f4/interface/LwIp/src/core/ipv4/subdir.mk
-include stm32f4/interface/LwIp/src/core/subdir.mk
-include stm32f4/interface/LwIp/src/api/subdir.mk
-include stm32f4/interface/LwIp/subdir.mk
-include stm32f4/interface/subdir.mk
-include stm32f4/driver/USBH/Core/Src/subdir.mk
-include stm32f4/driver/USBH/Class/MSC/Src/subdir.mk
-include stm32f4/driver/USBH/Class/MSC/subdir.mk
-include stm32f4/driver/USBD/Core/Src/subdir.mk
-include stm32f4/driver/USBD/Class/MSC/Src/subdir.mk
-include stm32f4/driver/USBD/Class/MSC/subdir.mk
-include stm32f4/driver/USBD/Class/CDC/Src/subdir.mk
-include stm32f4/driver/USBD/Class/CDC/subdir.mk
-include stm32f4/driver/USBD/Class/subdir.mk
-include stm32f4/driver/subdir.mk
-include stm32f4/board/STM32F469I-DISCO/subdir.mk
-include lib/lib/util/subdir.mk
-include lib/lib/gfx/resource/subdir.mk
-include lib/lib/gfx/libdrawtext/subdir.mk
-include lib/lib/gfx/files/subdir.mk
-include lib/lib/gfx/subdir.mk
-include lib/lib/fs/fat/src/option/subdir.mk
-include lib/lib/fs/fat/src/subdir.mk
-include lib/lib/buffers/subdir.mk
-include lib/lib/subdir.mk
-include lib/include/subdir.mk
-include lib/device/subdir.mk
-include lib/app/lwip/tftpserver/subdir.mk
-include lib/app/lwip/sntp/subdir.mk
-include lib/app/lwip/httpd/subdir.mk
-include lib/app/lwip/http_client/subdir.mk
-include lib/app/lwip/ftpserver/subdir.mk
-include lib/api/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ExampleSTM32F4.elf

# Tool invocations
ExampleSTM32F4.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU G++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -T"C:\GitHub\CppSDK\ExampleSTM32F4\LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -fno-exceptions -fno-rtti -lm -o "ExampleSTM32F4.elf" @"objects.list" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "ExampleSTM32F4.elf" "ExampleSTM32F4.bin"
	arm-none-eabi-size "ExampleSTM32F4.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
