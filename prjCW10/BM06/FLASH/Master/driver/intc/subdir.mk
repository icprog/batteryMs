################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/intc/intc.c" \

C_SRCS += \
$(MASTER)/driver/intc/intc.c \

OBJS += \
./Master/driver/intc/intc_c.obj \

OBJS_QUOTED += \
"./Master/driver/intc/intc_c.obj" \

C_DEPS += \
./Master/driver/intc/intc_c.d \

OBJS_OS_FORMAT += \
./Master/driver/intc/intc_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/intc/intc_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/intc/intc_c.obj: $(MASTER)/driver/intc/intc.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/intc/intc.args" -o "Master/driver/intc/intc_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/intc/intc_c.d: $(MASTER)/driver/intc/intc.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


