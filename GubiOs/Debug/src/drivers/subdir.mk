################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/drivers/Driver.cpp \
../src/drivers/amd_am79C973.cpp \
../src/drivers/keyboard.cpp \
../src/drivers/mouse.cpp \
../src/drivers/vga.cpp 

OBJS += \
./src/drivers/Driver.o \
./src/drivers/amd_am79C973.o \
./src/drivers/keyboard.o \
./src/drivers/mouse.o \
./src/drivers/vga.o 

CPP_DEPS += \
./src/drivers/Driver.d \
./src/drivers/amd_am79C973.d \
./src/drivers/keyboard.d \
./src/drivers/mouse.d \
./src/drivers/vga.d 


# Each subdirectory must supply rules for building sources it contributes
src/drivers/%.o: ../src/drivers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


