################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/soc/soc.c" \

C_SRCS += \
$(MASTER)/app/soc/soc.c \

OBJS += \
./Master/app/soc/soc_c.obj \

OBJS_QUOTED += \
"./Master/app/soc/soc_c.obj" \

C_DEPS += \
./Master/app/soc/soc_c.d \

OBJS_OS_FORMAT += \
./Master/app/soc/soc_c.obj \

C_DEPS_QUOTED += \
"./Master/app/soc/soc_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/soc/soc_c.obj: $(MASTER)/app/soc/soc.c
	@echo 'Building file: $<'
	@echo 'Executing target #30 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soc/soc.args" -o "Master/app/soc/soc_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soc/soc_c.d: $(MASTER)/app/soc/soc.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


