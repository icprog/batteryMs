################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/adc/adc.c" \

C_SRCS += \
$(MASTER)/driver/adc/adc.c \

OBJS += \
./Master/driver/adc/adc_c.obj \

OBJS_QUOTED += \
"./Master/driver/adc/adc_c.obj" \

C_DEPS += \
./Master/driver/adc/adc_c.d \

OBJS_OS_FORMAT += \
./Master/driver/adc/adc_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/adc/adc_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/adc/adc_c.obj: $(MASTER)/driver/adc/adc.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/adc/adc.args" -o "Master/driver/adc/adc_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/adc/adc_c.d: $(MASTER)/driver/adc/adc.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


