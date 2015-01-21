################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/driver/rtc/rtc.c" \

C_SRCS += \
$(MASTER)/driver/rtc/rtc.c \

OBJS += \
./Master/driver/rtc/rtc_c.obj \

OBJS_QUOTED += \
"./Master/driver/rtc/rtc_c.obj" \

C_DEPS += \
./Master/driver/rtc/rtc_c.d \

OBJS_OS_FORMAT += \
./Master/driver/rtc/rtc_c.obj \

C_DEPS_QUOTED += \
"./Master/driver/rtc/rtc_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/driver/rtc/rtc_c.obj: $(MASTER)/driver/rtc/rtc.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/driver/rtc/rtc.args" -o "Master/driver/rtc/rtc_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/driver/rtc/rtc_c.d: $(MASTER)/driver/rtc/rtc.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


