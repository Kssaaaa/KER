################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Driver/i2c_Hard/Driver_I2C2.c 

C_DEPS += \
./Driver/i2c_Hard/Driver_I2C2.d 

OBJS += \
./Driver/i2c_Hard/Driver_I2C2.o 



# Each subdirectory must supply rules for building sources it contributes
Driver/i2c_Hard/%.o: ../Driver/i2c_Hard/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"f:/Gtip/KER/CH32V307VCT6/Debug" -I"f:/Gtip/KER/CH32V307VCT6/Core" -I"f:/Gtip/KER/CH32V307VCT6/User" -I"f:/Gtip/KER/CH32V307VCT6/Peripheral/inc" -I"f:/Gtip/KER/CH32V307VCT6/Driver/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
