################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Package/CommonPackage.cpp \
../Package/Encoder.cpp \
../Package/Header.cpp 

OBJS += \
./Package/CommonPackage.o \
./Package/Encoder.o \
./Package/Header.o 

CPP_DEPS += \
./Package/CommonPackage.d \
./Package/Encoder.d \
./Package/Header.d 


# Each subdirectory must supply rules for building sources it contributes
Package/%.o: ../Package/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../muduo -I../../Include -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


