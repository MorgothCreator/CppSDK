################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../UartToLoRaBridge.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
UartToLoRaBridge.o

OBJS_AS_ARGS +=  \
UartToLoRaBridge.o

C_DEPS +=  \
UartToLoRaBridge.d

C_DEPS_AS_ARGS +=  \
UartToLoRaBridge.d

OUTPUT_FILE_PATH +=UartToLoRaBridge.elf

OUTPUT_FILE_PATH_AS_ARGS +=UartToLoRaBridge.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 4.8.4
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -mthumb -D__SAMD21G18A__ -DDEBUG  -I"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\..\..\CMSIS_Atmel\Device\ATMEL\samd21\include" -I"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\..\..\CMSIS_Atmel" -I"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\..\..\CMSIS_Atmel\CMSIS\Include" -I"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\..\..\CMSIS_Atmel\Device\ATMEL" -I"C:\GitHub\CppSDK\SDK\platform\atsamd21" -I"C:\GitHub\CppSDK\SDK\lib" -I".."  -O1 -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mlong-calls -g3 -Wall -mcpu=cortex-m0plus -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker : 4.8.4
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="UartToLoRaBridge.map" --specs=nano.specs -Wl,--start-group -lm  -Wl,--end-group -L"../../../SDK/platform/atsamd21/sys"  -Wl,--gc-sections -mcpu=cortex-m0plus -Tsamd21g18a_flash.ld  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O binary "UartToLoRaBridge.elf" "UartToLoRaBridge.bin"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "UartToLoRaBridge.elf" "UartToLoRaBridge.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "UartToLoRaBridge.elf" "UartToLoRaBridge.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "UartToLoRaBridge.elf" > "UartToLoRaBridge.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "UartToLoRaBridge.elf" "UartToLoRaBridge.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\ARM GCC\Native\4.8.1443\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "UartToLoRaBridge.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "UartToLoRaBridge.elf" "UartToLoRaBridge.a" "UartToLoRaBridge.hex" "UartToLoRaBridge.bin" "UartToLoRaBridge.lss" "UartToLoRaBridge.eep" "UartToLoRaBridge.map" "UartToLoRaBridge.srec"
	