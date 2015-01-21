################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/temp/temperature.c" \

C_SRCS += \
$(MASTER)/app/temp/temperature.c \

OBJS += \
./Master/app/temp/temperature_c.obj \

OBJS_QUOTED += \
"./Master/app/temp/temperature_c.obj" \

C_DEPS += \
./Master/app/temp/temperature_c.d \

OBJS_OS_FORMAT += \
./Master/app/temp/temperature_c.obj \

C_DEPS_QUOTED += \
"./Master/app/temp/temperature_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/temp/temperature_c.obj: $(MASTER)/app/temp/temperature.c
	@echo 'Building file: $<'
	@echo 'Executing target #24 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/temp/temperature.args" -o "Master/app/temp/temperature_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/temp/temperature_c.d: $(MASTER)/app/temp/temperature.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


