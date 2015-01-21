################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/middleLayer/appToDriver.c" \
"$(MASTER)/app/middleLayer/simulateDrivers.c" \

C_SRCS += \
$(MASTER)/app/middleLayer/appToDriver.c \
$(MASTER)/app/middleLayer/simulateDrivers.c \

OBJS += \
./Master/app/middleLayer/appToDriver_c.obj \
./Master/app/middleLayer/simulateDrivers_c.obj \

OBJS_QUOTED += \
"./Master/app/middleLayer/appToDriver_c.obj" \
"./Master/app/middleLayer/simulateDrivers_c.obj" \

C_DEPS += \
./Master/app/middleLayer/appToDriver_c.d \
./Master/app/middleLayer/simulateDrivers_c.d \

OBJS_OS_FORMAT += \
./Master/app/middleLayer/appToDriver_c.obj \
./Master/app/middleLayer/simulateDrivers_c.obj \

C_DEPS_QUOTED += \
"./Master/app/middleLayer/appToDriver_c.d" \
"./Master/app/middleLayer/simulateDrivers_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/middleLayer/appToDriver_c.obj: $(MASTER)/app/middleLayer/appToDriver.c
	@echo 'Building file: $<'
	@echo 'Executing target #42 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/middleLayer/appToDriver.args" -o "Master/app/middleLayer/appToDriver_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/middleLayer/appToDriver_c.d: $(MASTER)/app/middleLayer/appToDriver.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/middleLayer/simulateDrivers_c.obj: $(MASTER)/app/middleLayer/simulateDrivers.c
	@echo 'Building file: $<'
	@echo 'Executing target #43 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/middleLayer/simulateDrivers.args" -o "Master/app/middleLayer/simulateDrivers_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/middleLayer/simulateDrivers_c.d: $(MASTER)/app/middleLayer/simulateDrivers.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


