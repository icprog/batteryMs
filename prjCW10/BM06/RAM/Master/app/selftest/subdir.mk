################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/selftest/selftest.c" \

C_SRCS += \
$(MASTER)/app/selftest/selftest.c \

OBJS += \
./Master/app/selftest/selftest_c.obj \

OBJS_QUOTED += \
"./Master/app/selftest/selftest_c.obj" \

C_DEPS += \
./Master/app/selftest/selftest_c.d \

OBJS_OS_FORMAT += \
./Master/app/selftest/selftest_c.obj \

C_DEPS_QUOTED += \
"./Master/app/selftest/selftest_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/selftest/selftest_c.obj: $(MASTER)/app/selftest/selftest.c
	@echo 'Building file: $<'
	@echo 'Executing target #34 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/selftest/selftest.args" -o "Master/app/selftest/selftest_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/selftest/selftest_c.d: $(MASTER)/app/selftest/selftest.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


