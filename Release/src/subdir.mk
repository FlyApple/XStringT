################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/XStringBaseT.cpp \
../src/XStringSwapperT.cpp \
../src/XStringT.cpp \
../src/XStringTranscoderT.cpp \
../src/main.cpp 

OBJS += \
./src/XStringBaseT.o \
./src/XStringSwapperT.o \
./src/XStringT.o \
./src/XStringTranscoderT.o \
./src/main.o 

CPP_DEPS += \
./src/XStringBaseT.d \
./src/XStringSwapperT.d \
./src/XStringT.d \
./src/XStringTranscoderT.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../include -O3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


