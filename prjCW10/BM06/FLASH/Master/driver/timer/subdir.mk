################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/timer/timer.c" \

C_SRCS += \
$(MASTER)/driver/timer/timer.c \

OBJS += \
./Master/driver/timer/timer_c.obj \

OBJS_QUOTED += \
"./Master/driver/timer/timer_c.obj" \

C_DEPS += \
./Master/driver/timer/timer_c.d \

OBJS_OS_FORMAT += \
./Master/driver/timer/timer_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/timer/timer_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/timer/timer_c.obj: $(MASTER)/driver/timer/timer.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/timer/timer.args" -o "Master/driver/timer/timer_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/timer/timer_c.d: $(MASTER)/driver/timer/timer.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


