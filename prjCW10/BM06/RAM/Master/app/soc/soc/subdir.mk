################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/soc/soc/Closed_Loop_NCM_29Ah_change.c" \
"$(MASTER)/app/soc/soc/const_params.c" \
"$(MASTER)/app/soc/soc/look1_iflf_binlg.c" \

C_SRCS += \
$(MASTER)/app/soc/soc/Closed_Loop_NCM_29Ah_change.c \
$(MASTER)/app/soc/soc/const_params.c \
$(MASTER)/app/soc/soc/look1_iflf_binlg.c \

OBJS += \
./Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.obj \
./Master/app/soc/soc/const_params_c.obj \
./Master/app/soc/soc/look1_iflf_binlg_c.obj \

OBJS_QUOTED += \
"./Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.obj" \
"./Master/app/soc/soc/const_params_c.obj" \
"./Master/app/soc/soc/look1_iflf_binlg_c.obj" \

C_DEPS += \
./Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.d \
./Master/app/soc/soc/const_params_c.d \
./Master/app/soc/soc/look1_iflf_binlg_c.d \

OBJS_OS_FORMAT += \
./Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.obj \
./Master/app/soc/soc/const_params_c.obj \
./Master/app/soc/soc/look1_iflf_binlg_c.obj \

C_DEPS_QUOTED += \
"./Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.d" \
"./Master/app/soc/soc/const_params_c.d" \
"./Master/app/soc/soc/look1_iflf_binlg_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.obj: $(MASTER)/app/soc/soc/Closed_Loop_NCM_29Ah_change.c
	@echo 'Building file: $<'
	@echo 'Executing target #31 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soc/soc/Closed_Loop_NCM_29Ah_change.args" -o "Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soc/soc/Closed_Loop_NCM_29Ah_change_c.d: $(MASTER)/app/soc/soc/Closed_Loop_NCM_29Ah_change.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/soc/soc/const_params_c.obj: $(MASTER)/app/soc/soc/const_params.c
	@echo 'Building file: $<'
	@echo 'Executing target #32 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soc/soc/const_params.args" -o "Master/app/soc/soc/const_params_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soc/soc/const_params_c.d: $(MASTER)/app/soc/soc/const_params.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/soc/soc/look1_iflf_binlg_c.obj: $(MASTER)/app/soc/soc/look1_iflf_binlg.c
	@echo 'Building file: $<'
	@echo 'Executing target #33 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/soc/soc/look1_iflf_binlg.args" -o "Master/app/soc/soc/look1_iflf_binlg_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/soc/soc/look1_iflf_binlg_c.d: $(MASTER)/app/soc/soc/look1_iflf_binlg.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


