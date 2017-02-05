################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include nxp_kinetis_e/sys/subdir.mk
-include nxp_kinetis_e/interface/subdir.mk
-include nxp_kinetis_e/driver/subdir.mk
-include nxp_kinetis_e/board/FRDM-KEAZ128/subdir.mk
-include lib/lib/util/subdir.mk
-include lib/lib/gfx/resource/subdir.mk
-include lib/lib/gfx/libdrawtext/subdir.mk
-include lib/lib/gfx/files/subdir.mk
-include lib/lib/gfx/subdir.mk
-include lib/lib/fs/fat/src/option/subdir.mk
-include lib/lib/fs/fat/src/subdir.mk
-include lib/lib/fs/subdir.mk
-include lib/lib/crypto/subdir.mk
-include lib/lib/buffers/subdir.mk
-include lib/lib/subdir.mk
-include lib/include/subdir.mk
-include lib/device/subdir.mk
-include lib/app/sys/subdir.mk
-include lib/api/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ExampleKinetisKeaz.hex \

SECONDARY_SIZE += \
ExampleKinetisKeaz.siz \


# All Target
all: ExampleKinetisKeaz.elf secondary-outputs

# Tool invocations
ExampleKinetisKeaz.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -T "C:\GitHub\CppSDK\ExampleKinetisKeaz\LinkerScript.ld" -Xlinker --gc-sections -Wl,-Map,"ExampleKinetisKeaz.map" --specs=nano.specs -specs=nosys.specs -o "ExampleKinetisKeaz.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

ExampleKinetisKeaz.hex: ExampleKinetisKeaz.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "ExampleKinetisKeaz.elf"  "ExampleKinetisKeaz.hex"
	@echo 'Finished building: $@'
	@echo ' '

ExampleKinetisKeaz.siz: ExampleKinetisKeaz.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "ExampleKinetisKeaz.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(SECONDARY_FLASH)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) ExampleKinetisKeaz.elf
	-@echo ' '

post-build:
	-arm-none-eabi-size "ExampleKinetisKeaz.elf"; arm-none-eabi-objcopy -O binary "ExampleKinetisKeaz.elf" "ExampleKinetisKeaz.bin" ;  arm-none-eabi-objcopy -O srec "ExampleKinetisKeaz.elf" "ExampleKinetisKeaz.hex"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
