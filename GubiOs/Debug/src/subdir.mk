################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/loader.asm 

CPP_SRCS += \
../src/MemoryManager.cpp \
../src/gdt.cpp \
../src/kernel.cpp \
../src/multitasking.cpp \
../src/syscall.cpp 

OBJS += \
./src/MemoryManager.o \
./src/gdt.o \
./src/kernel.o \
./src/loader.o \
./src/multitasking.o \
./src/syscall.o 

CPP_DEPS += \
./src/MemoryManager.d \
./src/gdt.d \
./src/kernel.d \
./src/multitasking.d \
./src/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


