################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Fuation/src/oled.c \
../Fuation/src/word.c 

C_DEPS += \
./Fuation/src/oled.d \
./Fuation/src/word.d 

OBJS += \
./Fuation/src/oled.o \
./Fuation/src/word.o 



# Each subdirectory must supply rules for building sources it contributes
Fuation/src/%.o: ../Fuation/src/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"f:/Gtip/KER/CH32V307VCT6/Debug" -I"f:/Gtip/KER/CH32V307VCT6/Core" -I"f:/Gtip/KER/CH32V307VCT6/User" -I"f:/Gtip/KER/CH32V307VCT6/Peripheral/inc" -I"f:/Gtip/KER/CH32V307VCT6/Driver/inc" -I"f:/Gtip/KER/CH32V307VCT6/Fuation/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
