################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/src/Driver_ADC.c \
../Driver/src/Driver_Encoder.c \
../Driver/src/Driver_GPIO.c \
../Driver/src/Driver_TIM1.c \
../Driver/src/Driver_USART.c 

C_DEPS += \
./Driver/src/Driver_ADC.d \
./Driver/src/Driver_Encoder.d \
./Driver/src/Driver_GPIO.d \
./Driver/src/Driver_TIM1.d \
./Driver/src/Driver_USART.d 

OBJS += \
./Driver/src/Driver_ADC.o \
./Driver/src/Driver_Encoder.o \
./Driver/src/Driver_GPIO.o \
./Driver/src/Driver_TIM1.o \
./Driver/src/Driver_USART.o 



# Each subdirectory must supply rules for building sources it contributes
Driver/src/%.o: ../Driver/src/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"f:/Gtip/KER/CH32V307VCT6/Debug" -I"f:/Gtip/KER/CH32V307VCT6/Core" -I"f:/Gtip/KER/CH32V307VCT6/User" -I"f:/Gtip/KER/CH32V307VCT6/Peripheral/inc" -I"f:/Gtip/KER/CH32V307VCT6/Driver/inc" -I"f:/Gtip/KER/CH32V307VCT6/Fuation/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
