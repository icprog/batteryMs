################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 54
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Master/driver/uart/subdir.mk
-include Master/driver/timer/subdir.mk
-include Master/driver/spi/subdir.mk
-include Master/driver/rtc/subdir.mk
-include Master/driver/pll/subdir.mk
-include Master/driver/intc/subdir.mk
-include Master/driver/eprom/subdir.mk
-include Master/driver/adc/subdir.mk
-include Master/driver/LTC68042/subdir.mk
-include Master/driver/Gpio/subdir.mk
-include Master/driver/CAN/subdir.mk
-include Master/app/voltage/subdir.mk
-include Master/app/temp/subdir.mk
-include Master/app/soh/subdir.mk
-include Master/app/soc/subdir.mk
-include Master/app/soc/soc/subdir.mk
-include Master/app/selftest/subdir.mk
-include Master/app/public/subdir.mk
-include Master/app/protocol/subdir.mk
-include Master/app/middleLayer/subdir.mk
-include Master/app/mgclib/subdir.mk
-include Master/app/insulation_resistance/subdir.mk
-include Master/app/current/subdir.mk
-include Master/app/ctl/subdir.mk
-include Master/app/banlance_battery/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
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
STATICLIB += \

STATICLIB_QUOTED += \

STATICLIB_OS_FORMAT += \

EXECUTABLES += \

EXECUTABLES_QUOTED += \

EXECUTABLES_OS_FORMAT += \


# All Target
all: warning-messages explicit-dependencies BM06.elf

# Tool invocations
BM06.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/MPC5634M_RAM.lcf
	@echo 'Building target: $@'
	@echo 'Executing target #55 $@'
	@echo 'Invoking: PowerPC Linker'
	"$(PAToolsDirEnv)/mwldeppc" -o "BM06.elf" @@"BM06.args"  $(USER_OBJS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) $(STATICLIB_QUOTED) "./*/*/*/*.obj" "./*/*/*/*/*.obj" "./*/*/*.obj" "./*/*.obj"  $(CPP_DEPS_QUOTED) $(CC_DEPS_QUOTED) $(S_UPPER_DEPS_QUOTED) $(CXX_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(C_UPPER_DEPS_QUOTED) $(C++_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*/*.d" "./*/*/*/*/*.d" "./*/*/*.d" "./*/*.d"  $(ProjDirPath)/RAM/BM06.elf
	-@echo ' '

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/MPC5634M_RAM.lcf 

warning-messages: 

