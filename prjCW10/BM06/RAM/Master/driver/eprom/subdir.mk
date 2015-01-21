################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/eprom/eprom.c" \

C_SRCS += \
$(MASTER)/driver/eprom/eprom.c \

OBJS += \
./Master/driver/eprom/eprom_c.obj \

OBJS_QUOTED += \
"./Master/driver/eprom/eprom_c.obj" \

C_DEPS += \
./Master/driver/eprom/eprom_c.d \

OBJS_OS_FORMAT += \
./Master/driver/eprom/eprom_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/eprom/eprom_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/eprom/eprom_c.obj: $(MASTER)/driver/eprom/eprom.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/eprom/eprom.args" -o "Master/driver/eprom/eprom_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/eprom/eprom_c.d: $(MASTER)/driver/eprom/eprom.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


