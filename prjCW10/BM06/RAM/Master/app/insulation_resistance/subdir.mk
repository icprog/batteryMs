################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/insulation_resistance/insulation_resistance.c" \

C_SRCS += \
$(MASTER)/app/insulation_resistance/insulation_resistance.c \

OBJS += \
./Master/app/insulation_resistance/insulation_resistance_c.obj \

OBJS_QUOTED += \
"./Master/app/insulation_resistance/insulation_resistance_c.obj" \

C_DEPS += \
./Master/app/insulation_resistance/insulation_resistance_c.d \

OBJS_OS_FORMAT += \
./Master/app/insulation_resistance/insulation_resistance_c.obj \

C_DEPS_QUOTED += \
"./Master/app/insulation_resistance/insulation_resistance_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/insulation_resistance/insulation_resistance_c.obj: $(MASTER)/app/insulation_resistance/insulation_resistance.c
	@echo 'Building file: $<'
	@echo 'Executing target #47 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/insulation_resistance/insulation_resistance.args" -o "Master/app/insulation_resistance/insulation_resistance_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/insulation_resistance/insulation_resistance_c.d: $(MASTER)/app/insulation_resistance/insulation_resistance.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


