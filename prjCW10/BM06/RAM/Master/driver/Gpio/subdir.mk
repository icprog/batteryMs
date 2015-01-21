################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/Gpio/gpio.c" \
"$(MASTER)/driver/Gpio/gpioFuncs.c" \
"$(MASTER)/driver/Gpio/gpio_events.c" \

C_SRCS += \
$(MASTER)/driver/Gpio/gpio.c \
$(MASTER)/driver/Gpio/gpioFuncs.c \
$(MASTER)/driver/Gpio/gpio_events.c \

OBJS += \
./Master/driver/Gpio/gpio_c.obj \
./Master/driver/Gpio/gpioFuncs_c.obj \
./Master/driver/Gpio/gpio_events_c.obj \

OBJS_QUOTED += \
"./Master/driver/Gpio/gpio_c.obj" \
"./Master/driver/Gpio/gpioFuncs_c.obj" \
"./Master/driver/Gpio/gpio_events_c.obj" \

C_DEPS += \
./Master/driver/Gpio/gpio_c.d \
./Master/driver/Gpio/gpioFuncs_c.d \
./Master/driver/Gpio/gpio_events_c.d \

OBJS_OS_FORMAT += \
./Master/driver/Gpio/gpio_c.obj \
./Master/driver/Gpio/gpioFuncs_c.obj \
./Master/driver/Gpio/gpio_events_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/Gpio/gpio_c.d" \
"./Master/driver/Gpio/gpioFuncs_c.d" \
"./Master/driver/Gpio/gpio_events_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/Gpio/gpio_c.obj: $(MASTER)/driver/Gpio/gpio.c
	@echo 'Building file: $<'
	@echo 'Executing target #19 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/Gpio/gpio.args" -o "Master/driver/Gpio/gpio_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/Gpio/gpio_c.d: $(MASTER)/driver/Gpio/gpio.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/driver/Gpio/gpioFuncs_c.obj: $(MASTER)/driver/Gpio/gpioFuncs.c
	@echo 'Building file: $<'
	@echo 'Executing target #20 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/Gpio/gpioFuncs.args" -o "Master/driver/Gpio/gpioFuncs_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/Gpio/gpioFuncs_c.d: $(MASTER)/driver/Gpio/gpioFuncs.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/driver/Gpio/gpio_events_c.obj: $(MASTER)/driver/Gpio/gpio_events.c
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/Gpio/gpio_events.args" -o "Master/driver/Gpio/gpio_events_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/Gpio/gpio_events_c.d: $(MASTER)/driver/Gpio/gpio_events.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


