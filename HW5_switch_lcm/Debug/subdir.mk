################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../init-default.c \
../init-soc.c \
../interrupt-c-ext.c \
../main.c 

OBJS += \
./init-default.o \
./init-soc.o \
./interrupt-c-ext.o \
./main.o 

C_DEPS += \
./init-default.d \
./init-soc.d \
./interrupt-c-ext.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -DREMAP -DUSE_C_EXT -O0 -mcmodel=medium -g3 -Wall -mcpu=n801 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


