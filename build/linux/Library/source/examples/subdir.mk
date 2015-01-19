################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/nikola/prog/workspace/TraceDog/source/examples/basic.c 

OBJS += \
./source/examples/basic.o 

C_DEPS += \
./source/examples/basic.d 


# Each subdirectory must supply rules for building sources it contributes
source/examples/basic.o: /home/nikola/prog/workspace/TraceDog/source/examples/basic.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/nikola/prog/workspace/TraceDog/build/linux/Library/../../../include" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


