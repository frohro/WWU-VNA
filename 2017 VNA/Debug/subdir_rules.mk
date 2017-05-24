################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_target_name__=M4F -Dxdc_cfg__xheader__=\"configPkg/package/cfg/energia_pm4fg.h\" -DTARGET_IS_MSP432P4XX -DBOARD_MSP_EXP432P401R -Dxdc__nolocalstring=1 -D__MSP432P401R__ -DCORE_VERSION=380 -DF_CPU=48000000L -DENERGIA_MSP_EXP432P401R -DENERGIA_ARCH_MSP432 -DENERGIA=18 -DARDUINO=10610 -I"/home/frohro/ti/simplelink_msp432_sdk_1_30_00_40/source" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib/MSP432P4xx" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc/CMSIS/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/variants/MSP_EXP432P401R" -I"/home/frohro/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" -I"/home/frohro/Projects/2017_VNA/2017 VNA" -I"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/arm-none-eabi/include" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mabi=aapcs -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -fno-rtti -o"$@" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

MultiTaskSerial.null: ../MultiTaskSerial.ino
	@echo 'Building file: $<'
	@echo 'Invoking: Resource Custom Build Step'
	
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_target_name__=M4F -Dxdc_cfg__xheader__=\"configPkg/package/cfg/energia_pm4fg.h\" -DTARGET_IS_MSP432P4XX -DBOARD_MSP_EXP432P401R -Dxdc__nolocalstring=1 -D__MSP432P401R__ -DCORE_VERSION=380 -DF_CPU=48000000L -DENERGIA_MSP_EXP432P401R -DENERGIA_ARCH_MSP432 -DENERGIA=18 -DARDUINO=10610 -I"/home/frohro/ti/simplelink_msp432_sdk_1_30_00_40/source" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib/MSP432P4xx" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc/CMSIS/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/variants/MSP_EXP432P401R" -I"/home/frohro/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" -I"/home/frohro/Projects/2017_VNA/2017 VNA" -I"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/arm-none-eabi/include" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mabi=aapcs -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

build-591496191:
	@$(MAKE) -Onone -f subdir_rules.mk build-591496191-inproc

build-591496191-inproc: ../vna_2017.ino
	@echo 'Building file: $<'
	@echo 'Invoking: Resource Custom Build Step'
	
	@echo 'Finished building: $<'
	@echo ' '

vna_2017: build-591496191 ../vna_2017.ino
main.cpp: build-591496191
vna_2017.cpp: build-591496191

%.o: ./%.cpp $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -Dxdc_target_types__=gnu/targets/arm/std.h -Dxdc_target_name__=M4F -Dxdc_cfg__xheader__=\"configPkg/package/cfg/energia_pm4fg.h\" -DTARGET_IS_MSP432P4XX -DBOARD_MSP_EXP432P401R -Dxdc__nolocalstring=1 -D__MSP432P401R__ -DCORE_VERSION=380 -DF_CPU=48000000L -DENERGIA_MSP_EXP432P401R -DENERGIA_ARCH_MSP432 -DENERGIA=18 -DARDUINO=10610 -I"/home/frohro/ti/simplelink_msp432_sdk_1_30_00_40/source" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432/ti/runtime/wiring/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib/MSP432P4xx" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/driverlib" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system/inc/CMSIS/" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/system" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/cores/msp432" -I"/home/frohro/.energia15/packages/energia/hardware/msp432/3.8.0/variants/MSP_EXP432P401R" -I"/home/frohro/ti/simplelink_msp432_sdk_1_30_00_40/source/third_party/CMSIS/Include" -I"/home/frohro/Projects/2017_VNA/2017 VNA" -I"/home/frohro/.energia15/packages/energia/tools/arm-none-eabi-gcc/4.8.4-20140725/arm-none-eabi/include" -Os -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -w -Wall -mabi=aapcs -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -fno-rtti -o"$@" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


