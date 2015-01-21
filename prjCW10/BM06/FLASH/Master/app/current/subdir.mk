################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/current/current.c" \
"$(MASTER)/app/current/currentAbstract.c" \

C_SRCS += \
$(MASTER)/app/current/current.c \
$(MASTER)/app/current/currentAbstract.c \

OBJS += \
./Master/app/current/current_c.obj \
./Master/app/current/currentAbstract_c.obj \

OBJS_QUOTED += \
"./Master/app/current/current_c.obj" \
"./Master/app/current/currentAbstract_c.obj" \

C_DEPS += \
./Master/app/current/current_c.d \
./Master/app/current/currentAbstract_c.d \

OBJS_OS_FORMAT += \
./Master/app/current/current_c.obj \
./Master/app/current/currentAbstract_c.obj \

C_DEPS_QUOTED += \
"./Master/app/current/current_c.d" \
"./Master/app/current/currentAbstract_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/current/current_c.obj: $(MASTER)/app/current/current.c
	@echo 'Building file: $<'
	@echo 'Executing target #48 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/current/current.args" -o "Master/app/current/current_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/current/current_c.d: $(MASTER)/app/current/current.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/current/currentAbstract_c.obj: $(MASTER)/app/current/currentAbstract.c
	@echo 'Building file: $<'
	@echo 'Executing target #49 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/current/currentAbstract.args" -o "Master/app/current/currentAbstract_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/current/currentAbstract_c.d: $(MASTER)/app/current/currentAbstract.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


