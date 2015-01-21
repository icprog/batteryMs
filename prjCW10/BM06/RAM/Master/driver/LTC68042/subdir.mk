################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/LTC68042/ltc6804.c" \

C_SRCS += \
$(MASTER)/driver/LTC68042/ltc6804.c \

OBJS += \
./Master/driver/LTC68042/ltc6804_c.obj \

OBJS_QUOTED += \
"./Master/driver/LTC68042/ltc6804_c.obj" \

C_DEPS += \
./Master/driver/LTC68042/ltc6804_c.d \

OBJS_OS_FORMAT += \
./Master/driver/LTC68042/ltc6804_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/LTC68042/ltc6804_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/LTC68042/ltc6804_c.obj: $(MASTER)/driver/LTC68042/ltc6804.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/LTC68042/ltc6804.args" -o "Master/driver/LTC68042/ltc6804_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/LTC68042/ltc6804_c.d: $(MASTER)/driver/LTC68042/ltc6804.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


