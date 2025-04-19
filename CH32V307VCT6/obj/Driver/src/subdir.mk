################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/src/Driver_Key.c \
../Driver/src/Driver_LED.c \
../Driver/src/Driver_TIM1.c \
../Driver/src/Driver_TIM4.c \
../Driver/src/Driver_TIM5.c \
../Driver/src/Driver_TIM6.c \
../Driver/src/Driver_USART.c \
../Driver/src/dome_oled.c \
../Driver/src/dome_word.c 

C_DEPS += \
./Driver/src/Driver_Key.d \
./Driver/src/Driver_LED.d \
./Driver/src/Driver_TIM1.d \
./Driver/src/Driver_TIM4.d \
./Driver/src/Driver_TIM5.d \
./Driver/src/Driver_TIM6.d \
./Driver/src/Driver_USART.d \
./Driver/src/dome_oled.d \
./Driver/src/dome_word.d 

OBJS += \
./Driver/src/Driver_Key.o \
./Driver/src/Driver_LED.o \
./Driver/src/Driver_TIM1.o \
./Driver/src/Driver_TIM4.o \
./Driver/src/Driver_TIM5.o \
./Driver/src/Driver_TIM6.o \
./Driver/src/Driver_USART.o \
./Driver/src/dome_oled.o \
./Driver/src/dome_word.o 



# Each subdirectory must supply rules for building sources it contributes
Driver/src/%.o: ../Driver/src/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"f:/Gtip/KER/CH32V307VCT6/Debug" -I"f:/Gtip/KER/CH32V307VCT6/Core" -I"f:/Gtip/KER/CH32V307VCT6/User" -I"f:/Gtip/KER/CH32V307VCT6/Peripheral/inc" -I"f:/Gtip/KER/CH32V307VCT6/Driver/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
