################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/pll/pll.c" \

C_SRCS += \
$(MASTER)/driver/pll/pll.c \

OBJS += \
./Master/driver/pll/pll_c.obj \

OBJS_QUOTED += \
"./Master/driver/pll/pll_c.obj" \

C_DEPS += \
./Master/driver/pll/pll_c.d \

OBJS_OS_FORMAT += \
./Master/driver/pll/pll_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/pll/pll_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/pll/pll_c.obj: $(MASTER)/driver/pll/pll.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/pll/pll.args" -o "Master/driver/pll/pll_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/pll/pll_c.d: $(MASTER)/driver/pll/pll.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


