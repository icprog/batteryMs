################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/banlance_battery/banlancebattery.c" \

C_SRCS += \
$(MASTER)/app/banlance_battery/banlancebattery.c \

OBJS += \
./Master/app/banlance_battery/banlancebattery_c.obj \

OBJS_QUOTED += \
"./Master/app/banlance_battery/banlancebattery_c.obj" \

C_DEPS += \
./Master/app/banlance_battery/banlancebattery_c.d \

OBJS_OS_FORMAT += \
./Master/app/banlance_battery/banlancebattery_c.obj \

C_DEPS_QUOTED += \
"./Master/app/banlance_battery/banlancebattery_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/banlance_battery/banlancebattery_c.obj: $(MASTER)/app/banlance_battery/banlancebattery.c
	@echo 'Building file: $<'
	@echo 'Executing target #54 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/banlance_battery/banlancebattery.args" -o "Master/app/banlance_battery/banlancebattery_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/banlance_battery/banlancebattery_c.d: $(MASTER)/app/banlance_battery/banlancebattery.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


