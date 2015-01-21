################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/uart/uart.c" \

C_SRCS += \
$(MASTER)/driver/uart/uart.c \

OBJS += \
./Master/driver/uart/uart_c.obj \

OBJS_QUOTED += \
"./Master/driver/uart/uart_c.obj" \

C_DEPS += \
./Master/driver/uart/uart_c.d \

OBJS_OS_FORMAT += \
./Master/driver/uart/uart_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/uart/uart_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/uart/uart_c.obj: $(MASTER)/driver/uart/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/uart/uart.args" -o "Master/driver/uart/uart_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/uart/uart_c.d: $(MASTER)/driver/uart/uart.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


