################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(MASTER)/app/protocol/accp_protocol.c" \
"$(MASTER)/app/protocol/cpcu_protocol.c" \
"$(MASTER)/app/protocol/uart_protocol.c" \
"$(MASTER)/app/protocol/vcu_protocol.c" \

C_SRCS += \
$(MASTER)/app/protocol/accp_protocol.c \
$(MASTER)/app/protocol/cpcu_protocol.c \
$(MASTER)/app/protocol/uart_protocol.c \
$(MASTER)/app/protocol/vcu_protocol.c \

OBJS += \
./Master/app/protocol/accp_protocol_c.obj \
./Master/app/protocol/cpcu_protocol_c.obj \
./Master/app/protocol/uart_protocol_c.obj \
./Master/app/protocol/vcu_protocol_c.obj \

OBJS_QUOTED += \
"./Master/app/protocol/accp_protocol_c.obj" \
"./Master/app/protocol/cpcu_protocol_c.obj" \
"./Master/app/protocol/uart_protocol_c.obj" \
"./Master/app/protocol/vcu_protocol_c.obj" \

C_DEPS += \
./Master/app/protocol/accp_protocol_c.d \
./Master/app/protocol/cpcu_protocol_c.d \
./Master/app/protocol/uart_protocol_c.d \
./Master/app/protocol/vcu_protocol_c.d \

OBJS_OS_FORMAT += \
./Master/app/protocol/accp_protocol_c.obj \
./Master/app/protocol/cpcu_protocol_c.obj \
./Master/app/protocol/uart_protocol_c.obj \
./Master/app/protocol/vcu_protocol_c.obj \

C_DEPS_QUOTED += \
"./Master/app/protocol/accp_protocol_c.d" \
"./Master/app/protocol/cpcu_protocol_c.d" \
"./Master/app/protocol/uart_protocol_c.d" \
"./Master/app/protocol/vcu_protocol_c.d" \


# Each subdirectory must supply rules for building sources it contributes
Master/app/protocol/accp_protocol_c.obj: $(MASTER)/app/protocol/accp_protocol.c
	@echo 'Building file: $<'
	@echo 'Executing target #38 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/protocol/accp_protocol.args" -o "Master/app/protocol/accp_protocol_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/protocol/accp_protocol_c.d: $(MASTER)/app/protocol/accp_protocol.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/protocol/cpcu_protocol_c.obj: $(MASTER)/app/protocol/cpcu_protocol.c
	@echo 'Building file: $<'
	@echo 'Executing target #39 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/protocol/cpcu_protocol.args" -o "Master/app/protocol/cpcu_protocol_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/protocol/cpcu_protocol_c.d: $(MASTER)/app/protocol/cpcu_protocol.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/protocol/uart_protocol_c.obj: $(MASTER)/app/protocol/uart_protocol.c
	@echo 'Building file: $<'
	@echo 'Executing target #40 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/protocol/uart_protocol.args" -o "Master/app/protocol/uart_protocol_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/protocol/uart_protocol_c.d: $(MASTER)/app/protocol/uart_protocol.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Master/app/protocol/vcu_protocol_c.obj: $(MASTER)/app/protocol/vcu_protocol.c
	@echo 'Building file: $<'
	@echo 'Executing target #41 $<'
	@echo 'Invoking: PowerPC Compiler'
	"$(PAToolsDirEnv)/mwcceppc" @@"Master/app/protocol/vcu_protocol.args" -o "Master/app/protocol/vcu_protocol_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Master/app/protocol/vcu_protocol_c.d: $(MASTER)/app/protocol/vcu_protocol.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


