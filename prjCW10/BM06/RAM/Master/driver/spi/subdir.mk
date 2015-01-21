################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/spi/spi.c" \
"$(MASTER)/driver/spi/spi_simulator.c" \

C_SRCS += \
$(MASTER)/driver/spi/spi.c \
$(MASTER)/driver/spi/spi_simulator.c \

OBJS += \
./Master/driver/spi/spi_c.obj \
./Master/driver/spi/spi_simulator_c.obj \

OBJS_QUOTED += \
"./Master/driver/spi/spi_c.obj" \
"./Master/driver/spi/spi_simulator_c.obj" \

C_DEPS += \
./Master/driver/spi/spi_c.d \
./Master/driver/spi/spi_simulator_c.d \

OBJS_OS_FORMAT += \
./Master/driver/spi/spi_c.obj \
./Master/driver/spi/spi_simulator_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/spi/spi_c.d" \
"./Master/driver/spi/spi_simulator_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/spi/spi_c.obj: $(MASTER)/driver/spi/spi.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/spi/spi.args" -o "Master/driver/spi/spi_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/spi/spi_c.d: $(MASTER)/driver/spi/spi.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/driver/spi/spi_simulator_c.obj: $(MASTER)/driver/spi/spi_simulator.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/spi/spi_simulator.args" -o "Master/driver/spi/spi_simulator_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/spi/spi_simulator_c.d: $(MASTER)/driver/spi/spi_simulator.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


