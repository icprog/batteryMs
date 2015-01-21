################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/soh/SOH.c" \
"$(MASTER)/app/soh/div_s32_sat.c" \
"$(MASTER)/app/soh/mul_s32_s32_u32_sat.c" \
"$(MASTER)/app/soh/mul_wide_su32.c" \
"$(MASTER)/app/soh/sohProcess.c" \

C_SRCS += \
$(MASTER)/app/soh/SOH.c \
$(MASTER)/app/soh/div_s32_sat.c \
$(MASTER)/app/soh/mul_s32_s32_u32_sat.c \
$(MASTER)/app/soh/mul_wide_su32.c \
$(MASTER)/app/soh/sohProcess.c \

OBJS += \
./Master/app/soh/SOH_c.obj \
./Master/app/soh/div_s32_sat_c.obj \
./Master/app/soh/mul_s32_s32_u32_sat_c.obj \
./Master/app/soh/mul_wide_su32_c.obj \
./Master/app/soh/sohProcess_c.obj \

OBJS_QUOTED += \
"./Master/app/soh/SOH_c.obj" \
"./Master/app/soh/div_s32_sat_c.obj" \
"./Master/app/soh/mul_s32_s32_u32_sat_c.obj" \
"./Master/app/soh/mul_wide_su32_c.obj" \
"./Master/app/soh/sohProcess_c.obj" \

C_DEPS += \
./Master/app/soh/SOH_c.d \
./Master/app/soh/div_s32_sat_c.d \
./Master/app/soh/mul_s32_s32_u32_sat_c.d \
./Master/app/soh/mul_wide_su32_c.d \
./Master/app/soh/sohProcess_c.d \

OBJS_OS_FORMAT += \
./Master/app/soh/SOH_c.obj \
./Master/app/soh/div_s32_sat_c.obj \
./Master/app/soh/mul_s32_s32_u32_sat_c.obj \
./Master/app/soh/mul_wide_su32_c.obj \
./Master/app/soh/sohProcess_c.obj \

C_DEPS_QUOTED += \
"./Master/app/soh/SOH_c.d" \
"./Master/app/soh/div_s32_sat_c.d" \
"./Master/app/soh/mul_s32_s32_u32_sat_c.d" \
"./Master/app/soh/mul_wide_su32_c.d" \
"./Master/app/soh/sohProcess_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/soh/SOH_c.obj: $(MASTER)/app/soh/SOH.c
	@echo 'Building file: $<'
	@echo 'Executing target #25 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soh/SOH.args" -o "Master/app/soh/SOH_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soh/SOH_c.d: $(MASTER)/app/soh/SOH.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/soh/div_s32_sat_c.obj: $(MASTER)/app/soh/div_s32_sat.c
	@echo 'Building file: $<'
	@echo 'Executing target #26 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soh/div_s32_sat.args" -o "Master/app/soh/div_s32_sat_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soh/div_s32_sat_c.d: $(MASTER)/app/soh/div_s32_sat.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/soh/mul_s32_s32_u32_sat_c.obj: $(MASTER)/app/soh/mul_s32_s32_u32_sat.c
	@echo 'Building file: $<'
	@echo 'Executing target #27 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soh/mul_s32_s32_u32_sat.args" -o "Master/app/soh/mul_s32_s32_u32_sat_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soh/mul_s32_s32_u32_sat_c.d: $(MASTER)/app/soh/mul_s32_s32_u32_sat.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/soh/mul_wide_su32_c.obj: $(MASTER)/app/soh/mul_wide_su32.c
	@echo 'Building file: $<'
	@echo 'Executing target #28 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soh/mul_wide_su32.args" -o "Master/app/soh/mul_wide_su32_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soh/mul_wide_su32_c.d: $(MASTER)/app/soh/mul_wide_su32.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/soh/sohProcess_c.obj: $(MASTER)/app/soh/sohProcess.c
	@echo 'Building file: $<'
	@echo 'Executing target #29 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soh/sohProcess.args" -o "Master/app/soh/sohProcess_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soh/sohProcess_c.d: $(MASTER)/app/soh/sohProcess.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


