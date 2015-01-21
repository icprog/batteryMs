################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/CAN/can.c" \

C_SRCS += \
$(MASTER)/driver/CAN/can.c \

OBJS += \
./Master/driver/CAN/can_c.obj \

OBJS_QUOTED += \
"./Master/driver/CAN/can_c.obj" \

C_DEPS += \
./Master/driver/CAN/can_c.d \

OBJS_OS_FORMAT += \
./Master/driver/CAN/can_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/CAN/can_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/CAN/can_c.obj: $(MASTER)/driver/CAN/can.c
	@echo 'Building file: $<'
	@echo 'Executing target #22 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/CAN/can.args" -o "Master/driver/CAN/can_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/CAN/can_c.d: $(MASTER)/driver/CAN/can.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


