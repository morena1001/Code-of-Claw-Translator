################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Code of Claw Black Pill
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Code\ of\ Claw\ Black\ Pill.elf \

MAP_FILES += \
Code\ of\ Claw\ Black\ Pill.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Code\ of\ Claw\ Black\ Pill.list \

OBJCOPY_HEX += \
Code\ of\ Claw\ Black\ Pill.hex \

OBJCOPY_BIN += \
Code\ of\ Claw\ Black\ Pill.bin \


# All Target
all: main-build

# Main-build Target
main-build: Code\ of\ Claw\ Black\ Pill.elf secondary-outputs

# Tool invocations
Code\ of\ Claw\ Black\ Pill.elf Code\ of\ Claw\ Black\ Pill.map: $(OBJS) $(USER_OBJS) C:\Users\josue\Code-of-Claw-Translator\Breadboard\STM32F411CEUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Code of Claw Black Pill.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\josue\Code-of-Claw-Translator\Breadboard\STM32F411CEUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Code of Claw Black Pill.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Code\ of\ Claw\ Black\ Pill.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Code of Claw Black Pill.list"
	@echo 'Finished building: $@'
	@echo ' '

Code\ of\ Claw\ Black\ Pill.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Code of Claw Black Pill.hex"
	@echo 'Finished building: $@'
	@echo ' '

Code\ of\ Claw\ Black\ Pill.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Code of Claw Black Pill.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Code\ of\ Claw\ Black\ Pill.bin Code\ of\ Claw\ Black\ Pill.elf Code\ of\ Claw\ Black\ Pill.hex Code\ of\ Claw\ Black\ Pill.list Code\ of\ Claw\ Black\ Pill.map Code\\ of\\ Claw\\ Black\\ Pill.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
