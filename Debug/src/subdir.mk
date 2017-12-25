################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MXString.cpp \
../src/MXStringBase.cpp \
../src/MXStringT.cpp \
../src/MXStringTranscoder.cpp \
../src/main.cpp 

OBJS += \
./src/MXString.o \
./src/MXStringBase.o \
./src/MXStringT.o \
./src/MXStringTranscoder.o \
./src/main.o 

CPP_DEPS += \
./src/MXString.d \
./src/MXStringBase.d \
./src/MXStringT.d \
./src/MXStringTranscoder.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


