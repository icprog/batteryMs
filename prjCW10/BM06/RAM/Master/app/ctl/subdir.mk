################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/ctl/ColdHeatManager.c" \
"$(MASTER)/app/ctl/charge.c" \
"$(MASTER)/app/ctl/control.c" \
"$(MASTER)/app/ctl/relay.c" \

C_SRCS += \
$(MASTER)/app/ctl/ColdHeatManager.c \
$(MASTER)/app/ctl/charge.c \
$(MASTER)/app/ctl/control.c \
$(MASTER)/app/ctl/relay.c \

OBJS += \
./Master/app/ctl/ColdHeatManager_c.obj \
./Master/app/ctl/charge_c.obj \
./Master/app/ctl/control_c.obj \
./Master/app/ctl/relay_c.obj \

OBJS_QUOTED += \
"./Master/app/ctl/ColdHeatManager_c.obj" \
"./Master/app/ctl/charge_c.obj" \
"./Master/app/ctl/control_c.obj" \
"./Master/app/ctl/relay_c.obj" \

C_DEPS += \
./Master/app/ctl/ColdHeatManager_c.d \
./Master/app/ctl/charge_c.d \
./Master/app/ctl/control_c.d \
./Master/app/ctl/relay_c.d \

OBJS_OS_FORMAT += \
./Master/app/ctl/ColdHeatManager_c.obj \
./Master/app/ctl/charge_c.obj \
./Master/app/ctl/control_c.obj \
./Master/app/ctl/relay_c.obj \

C_DEPS_QUOTED += \
"./Master/app/ctl/ColdHeatManager_c.d" \
"./Master/app/ctl/charge_c.d" \
"./Master/app/ctl/control_c.d" \
"./Master/app/ctl/relay_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/ctl/ColdHeatManager_c.obj: $(MASTER)/app/ctl/ColdHeatManager.c
	@echo 'Building file: $<'
	@echo 'Executing target #50 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/ctl/ColdHeatManager.args" -o "Master/app/ctl/ColdHeatManager_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/ctl/ColdHeatManager_c.d: $(MASTER)/app/ctl/ColdHeatManager.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/ctl/charge_c.obj: $(MASTER)/app/ctl/charge.c
	@echo 'Building file: $<'
	@echo 'Executing target #51 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/ctl/charge.args" -o "Master/app/ctl/charge_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/ctl/charge_c.d: $(MASTER)/app/ctl/charge.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/ctl/control_c.obj: $(MASTER)/app/ctl/control.c
	@echo 'Building file: $<'
	@echo 'Executing target #52 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/ctl/control.args" -o "Master/app/ctl/control_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/ctl/control_c.d: $(MASTER)/app/ctl/control.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/ctl/relay_c.obj: $(MASTER)/app/ctl/relay.c
	@echo 'Building file: $<'
	@echo 'Executing target #53 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/ctl/relay.args" -o "Master/app/ctl/relay_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/ctl/relay_c.d: $(MASTER)/app/ctl/relay.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


