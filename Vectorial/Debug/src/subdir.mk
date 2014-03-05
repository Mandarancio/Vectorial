################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Entity.cpp \
../src/GameScene.cpp \
../src/GraphicalEntity.cpp \
../src/SVGGraphicalEntity.cpp \
../src/graphical_primitives.cpp \
../src/main.cpp 

OBJS += \
./src/Entity.o \
./src/GameScene.o \
./src/GraphicalEntity.o \
./src/SVGGraphicalEntity.o \
./src/graphical_primitives.o \
./src/main.o 

CPP_DEPS += \
./src/Entity.d \
./src/GameScene.d \
./src/GraphicalEntity.d \
./src/SVGGraphicalEntity.d \
./src/graphical_primitives.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


