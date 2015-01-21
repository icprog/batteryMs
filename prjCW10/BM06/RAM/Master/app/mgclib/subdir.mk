################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/mgclib/mgcfifo.c" \
"$(MASTER)/app/mgclib/mgcprintf.c" \
"$(MASTER)/app/mgclib/mgcstring.c" \

C_SRCS += \
$(MASTER)/app/mgclib/mgcfifo.c \
$(MASTER)/app/mgclib/mgcprintf.c \
$(MASTER)/app/mgclib/mgcstring.c \

OBJS += \
./Master/app/mgclib/mgcfifo_c.obj \
./Master/app/mgclib/mgcprintf_c.obj \
./Master/app/mgclib/mgcstring_c.obj \

OBJS_QUOTED += \
"./Master/app/mgclib/mgcfifo_c.obj" \
"./Master/app/mgclib/mgcprintf_c.obj" \
"./Master/app/mgclib/mgcstring_c.obj" \

C_DEPS += \
./Master/app/mgclib/mgcfifo_c.d \
./Master/app/mgclib/mgcprintf_c.d \
./Master/app/mgclib/mgcstring_c.d \

OBJS_OS_FORMAT += \
./Master/app/mgclib/mgcfifo_c.obj \
./Master/app/mgclib/mgcprintf_c.obj \
./Master/app/mgclib/mgcstring_c.obj \

C_DEPS_QUOTED += \
"./Master/app/mgclib/mgcfifo_c.d" \
"./Master/app/mgclib/mgcprintf_c.d" \
"./Master/app/mgclib/mgcstring_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/mgclib/mgcfifo_c.obj: $(MASTER)/app/mgclib/mgcfifo.c
	@echo 'Building file: $<'
	@echo 'Executing target #44 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/mgclib/mgcfifo.args" -o "Master/app/mgclib/mgcfifo_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/mgclib/mgcfifo_c.d: $(MASTER)/app/mgclib/mgcfifo.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/mgclib/mgcprintf_c.obj: $(MASTER)/app/mgclib/mgcprintf.c
	@echo 'Building file: $<'
	@echo 'Executing target #45 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/mgclib/mgcprintf.args" -o "Master/app/mgclib/mgcprintf_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/mgclib/mgcprintf_c.d: $(MASTER)/app/mgclib/mgcprintf.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/mgclib/mgcstring_c.obj: $(MASTER)/app/mgclib/mgcstring.c
	@echo 'Building file: $<'
	@echo 'Executing target #46 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/mgclib/mgcstring.args" -o "Master/app/mgclib/mgcstring_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/mgclib/mgcstring_c.d: $(MASTER)/app/mgclib/mgcstring.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


