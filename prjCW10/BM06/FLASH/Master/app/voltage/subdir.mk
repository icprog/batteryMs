################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/voltage/voltage.c" \

C_SRCS += \
$(MASTER)/app/voltage/voltage.c \

OBJS += \
./Master/app/voltage/voltage_c.obj \

OBJS_QUOTED += \
"./Master/app/voltage/voltage_c.obj" \

C_DEPS += \
./Master/app/voltage/voltage_c.d \

OBJS_OS_FORMAT += \
./Master/app/voltage/voltage_c.obj \

C_DEPS_QUOTED += \
"./Master/app/voltage/voltage_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/voltage/voltage_c.obj: $(MASTER)/app/voltage/voltage.c
	@echo 'Building file: $<'
	@echo 'Executing target #23 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/voltage/voltage.args" -o "Master/app/voltage/voltage_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/voltage/voltage_c.d: $(MASTER)/app/voltage/voltage.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


