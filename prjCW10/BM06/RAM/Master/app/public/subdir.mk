################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/public/ParameterManager.c" \
"$(MASTER)/app/public/public.c" \
"$(MASTER)/app/public/systemState.c" \

C_SRCS += \
$(MASTER)/app/public/ParameterManager.c \
$(MASTER)/app/public/public.c \
$(MASTER)/app/public/systemState.c \

OBJS += \
./Master/app/public/ParameterManager_c.obj \
./Master/app/public/public_c.obj \
./Master/app/public/systemState_c.obj \

OBJS_QUOTED += \
"./Master/app/public/ParameterManager_c.obj" \
"./Master/app/public/public_c.obj" \
"./Master/app/public/systemState_c.obj" \

C_DEPS += \
./Master/app/public/ParameterManager_c.d \
./Master/app/public/public_c.d \
./Master/app/public/systemState_c.d \

OBJS_OS_FORMAT += \
./Master/app/public/ParameterManager_c.obj \
./Master/app/public/public_c.obj \
./Master/app/public/systemState_c.obj \

C_DEPS_QUOTED += \
"./Master/app/public/ParameterManager_c.d" \
"./Master/app/public/public_c.d" \
"./Master/app/public/systemState_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/public/ParameterManager_c.obj: $(MASTER)/app/public/ParameterManager.c
	@echo 'Building file: $<'
	@echo 'Executing target #35 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/public/ParameterManager.args" -o "Master/app/public/ParameterManager_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/public/ParameterManager_c.d: $(MASTER)/app/public/ParameterManager.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/public/public_c.obj: $(MASTER)/app/public/public.c
	@echo 'Building file: $<'
	@echo 'Executing target #36 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/public/public.args" -o "Master/app/public/public_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/public/public_c.d: $(MASTER)/app/public/public.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/public/systemState_c.obj: $(MASTER)/app/public/systemState.c
	@echo 'Building file: $<'
	@echo 'Executing target #37 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/public/systemState.args" -o "Master/app/public/systemState_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/public/systemState_c.d: $(MASTER)/app/public/systemState.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


