################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/hardwarecommunication/interrupts.cpp \
../src/hardwarecommunication/pci.cpp \
../src/hardwarecommunication/pci_org.cpp \
../src/hardwarecommunication/port.cpp 

OBJS += \
./src/hardwarecommunication/interrupts.o \
./src/hardwarecommunication/pci.o \
./src/hardwarecommunication/pci_org.o \
./src/hardwarecommunication/port.o 

CPP_DEPS += \
./src/hardwarecommunication/interrupts.d \
./src/hardwarecommunication/pci.d \
./src/hardwarecommunication/pci_org.d \
./src/hardwarecommunication/port.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardwarecommunication/%.o: ../src/hardwarecommunication/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


