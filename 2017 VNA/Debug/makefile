################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./adc14vna.o" \
"./DynamicCommandParser.o" \
"./main.o" \
"./quadrature.o" \
"./si5351.o" \
"./vna_2017.o" \
$(GEN_CMDS__FLAG) \
-Wl,--start-group \
-Wl,-T"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432/linker.cmd" \
-lMSP-EXP432P401R_core \
-l:msp432p4xx_driverlib.a \
-lstdc++ \
-lgcc \
-lc \
-lm \
-lnosys \
-Wl,--end-group \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
2017\ VNA.out \

EXE_OUTPUTS__QUOTED += \
"2017 VNA.out" \

BIN_OUTPUTS += \
2017\ VNA.hex \

BIN_OUTPUTS__QUOTED += \
"2017 VNA.hex" \


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "2017 VNA.out"

dependents:
	-cd /home/frohro/workspace_v7/MSP-EXP432P401R_core/Debug && $(MAKE) clean all

# Tool invocations
2017\ VNA.out: $(OBJS) $(GEN_CMDS) /home/frohro/workspace_v7/MSP-EXP432P401R_core/Debug/libMSP-EXP432P401R_core.a
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/bin/arm-none-eabi-gcc" -fno-exceptions -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_target_name__=M4F -Dxdc_cfg__xheader__=\"configPkg/package/cfg/energia_pm4fg.h\" -DTARGET_IS_MSP432P4XX -DBOARD_MSP_EXP432P401R -Dxdc__nolocalstring=1 -D__MSP432P401R__ -DCORE_VERSION=380 -DF_CPU=48000000L -DENERGIA_MSP_EXP432P401R -DENERGIA_ARCH_MSP432 -DENERGIA=18 -DARDUINO=10610 -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mabi=aapcs -fno-threadsafe-statics -mabi=aapcs -fno-threadsafe-statics -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,-Map,"2017 VNA.map" -nostartfiles -Wl,--gc-sections -L"/home/frohro/workspace_v7/MSP-EXP432P401R_core/Debug" -L"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432" -L"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432" -L"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432/variants/MSP_EXP432P401R" -L"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/gnu/targets/arm/libs/install-native/arm-none-eabi/lib/armv7e-m/fpu" -L"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib/MSP432P4xx/gcc" -Wl,--undefined,main -Os -Wl,--check-sections -o"2017 VNA.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

2017\ VNA.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: GNU Objcopy Utility'
	"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/bin/arm-none-eabi-objcopy"  $(EXE_OUTPUTS__QUOTED) "2017 VNA.hex"
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "DynamicCommandParser.o" "adc14vna.o" "quadrature.o" "si5351.o" "main.o" "vna_2017.o" 
	-$(RM) "adc14vna.d" 
	-$(RM) "DynamicCommandParser.d" "quadrature.d" "si5351.d" "main.d" "vna_2017.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/frohro/workspace_v7/MSP-EXP432P401R_core/Debug/libMSP-EXP432P401R_core.a:

-include ../makefile.targets

