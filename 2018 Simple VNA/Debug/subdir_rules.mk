################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/frohro/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_target_name__=M4F -Dxdc_cfg__xheader__=\"configPkg/package/cfg/energia_pm4fg.h\" -DTARGET_IS_MSP432P4XX -DBOARD_MSP_EXP432P401R -Dxdc__nolocalstring=1 -D__MSP432P401R__ -DCORE_VERSION=380 -DF_CPU=48000000L -DENERGIA_MSP_EXP432P401R -DENERGIA_ARCH_MSP432 -DENERGIA=18 -DARDUINO=10610 -I"/home/frohro/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib/MSP432P4xx" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc/CMSIS/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/variants/MSP_EXP432P401R" -I"/home/frohro/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"/home/frohro/Projects/2017_VNA/2018 Simple VNA" -I"/home/frohro/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mabi=aapcs -fno-threadsafe-statics -mabi=aapcs -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -fno-rtti $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

MultiTaskSerial.null: ../MultiTaskSerial.ino
	@echo 'Building file: "$<"'
	@echo 'Invoking: Resource Custom Build Step'
	
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/frohro/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_target_name__=M4F -Dxdc_cfg__xheader__=\"configPkg/package/cfg/energia_pm4fg.h\" -DTARGET_IS_MSP432P4XX -DBOARD_MSP_EXP432P401R -Dxdc__nolocalstring=1 -D__MSP432P401R__ -DCORE_VERSION=380 -DF_CPU=48000000L -DENERGIA_MSP_EXP432P401R -DENERGIA_ARCH_MSP432 -DENERGIA=18 -DARDUINO=10610 -I"/home/frohro/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib/MSP432P4xx" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc/CMSIS/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/variants/MSP_EXP432P401R" -I"/home/frohro/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"/home/frohro/Projects/2017_VNA/2018 Simple VNA" -I"/home/frohro/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mabi=aapcs -fno-threadsafe-statics -mabi=aapcs -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1799934161:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1799934161-inproc

build-1799934161-inproc: ../vna_2017.ino
	@echo 'Building file: "$<"'
	@echo 'Invoking: Resource Custom Build Step'
	
	@echo 'Finished building: "$<"'
	@echo ' '

vna_2017: build-1799934161 ../vna_2017.ino
main.cpp: build-1799934161
vna_2017.cpp: build-1799934161

%.o: ./%.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/frohro/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_target_name__=M4F -Dxdc_cfg__xheader__=\"configPkg/package/cfg/energia_pm4fg.h\" -DTARGET_IS_MSP432P4XX -DBOARD_MSP_EXP432P401R -Dxdc__nolocalstring=1 -D__MSP432P401R__ -DCORE_VERSION=380 -DF_CPU=48000000L -DENERGIA_MSP_EXP432P401R -DENERGIA_ARCH_MSP432 -DENERGIA=18 -DARDUINO=10610 -I"/home/frohro/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib/MSP432P4xx" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc/CMSIS/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/variants/MSP_EXP432P401R" -I"/home/frohro/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"/home/frohro/Projects/2017_VNA/2018 Simple VNA" -I"/home/frohro/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mabi=aapcs -fno-threadsafe-statics -mabi=aapcs -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -fno-rtti $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


