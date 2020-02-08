################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifndef ANDESIGHT_ROOT
ANDESIGHT_ROOT=/cygdrive/C/Andestech/AndeSight211MCU
endif

ifndef CROSS_COMPILE
CROSS_COMPILE=nds32le-elf-
endif

ifndef SECONDARY_OUTPUT_PATH
SECONDARY_OUTPUT_PATH=output
endif

$(shell mkdir -p $(SECONDARY_OUTPUT_PATH))

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
BUILD_ARTIFACT_FILE_BASE_NAME = 7segDip

LINKER_OUTPUTS = 7segDip.adx

GCOV_OUT += \
$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)) \

GPROF_OUT += \
../gmon.sum ../gmon.out \

SYMBOL_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/symbol.txt \

READELF_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/readelf.txt \

OBJDUMP_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/objdump.txt \

OBJCOPY_OUTPUTS += \
$(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin \

TOOL_NDS_SIZE_OUTPUT_OUTPUTS += \
.PHONY.size \


# All Target
all: 7segDip.adx secondary-outputs

# Tool invocations
7segDip.adx: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Andes C Linker'
	$(CROSS_COMPILE)gcc -O0 -nostartfiles -static -mcmodel=medium -T"../nds32-c-ext.ld" -o "7segDip.adx" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(foreach subdir,$(SUBDIRS),$(wildcard $(subdir)/*.gcda) $(wildcard $(subdir)/*.gcno)): 7segDip.adx $(OBJS) $(USER_OBJS)
../gmon.sum ../gmon.out: 7segDip.adx $(OBJS) $(USER_OBJS)

$(SECONDARY_OUTPUT_PATH)/symbol.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: NM (symbol listing)'
	$(CROSS_COMPILE)nm -n -l -C "7segDip.adx" > $(SECONDARY_OUTPUT_PATH)/symbol.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/readelf.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: Readelf (ELF info listing)'
	$(CROSS_COMPILE)readelf -a "7segDip.adx" > $(SECONDARY_OUTPUT_PATH)/readelf.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/objdump.txt: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objdump (disassembly)'
	$(CROSS_COMPILE)objdump -x -d -C "7segDip.adx" > $(SECONDARY_OUTPUT_PATH)/objdump.txt
	@echo 'Finished building: $@'
	@echo ' '

$(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin: $(LINKER_OUTPUTS)
	@echo 'Invoking: Objcopy (object content copy)'
	$(CROSS_COMPILE)objcopy -S -O binary "7segDip.adx" $(SECONDARY_OUTPUT_PATH)/$(BUILD_ARTIFACT_FILE_BASE_NAME).bin
	@echo 'Finished building: $@'
	@echo ' '

.PHONY.size: $(LINKER_OUTPUTS)
	@echo 'Invoking: Size (section size listing)'
	$(CROSS_COMPILE)size  "7segDip.adx"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TOOL_NDS_SIZE_OUTPUT_OUTPUTS)$(OBJCOPY_OUTPUTS)$(OBJS)$(C_DEPS)$(GCOV_OUT)$(SYMBOL_OUTPUTS)$(OBJDUMP_OUTPUTS)$(READELF_OUTPUTS)$(GPROF_OUT)$(EXECUTABLES)$(S_UPPER_DEPS) 7segDip.adx
	-@echo ' '

secondary-outputs: $(SYMBOL_OUTPUTS) $(READELF_OUTPUTS) $(OBJDUMP_OUTPUTS) $(OBJCOPY_OUTPUTS) $(TOOL_NDS_SIZE_OUTPUT_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
