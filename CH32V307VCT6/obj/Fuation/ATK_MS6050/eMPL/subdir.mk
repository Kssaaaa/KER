################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Fuation/ATK_MS6050/eMPL/inv_mpu.c \
../Fuation/ATK_MS6050/eMPL/inv_mpu_dmp_motion_driver.c 

C_DEPS += \
./Fuation/ATK_MS6050/eMPL/inv_mpu.d \
./Fuation/ATK_MS6050/eMPL/inv_mpu_dmp_motion_driver.d 

OBJS += \
./Fuation/ATK_MS6050/eMPL/inv_mpu.o \
./Fuation/ATK_MS6050/eMPL/inv_mpu_dmp_motion_driver.o 



# Each subdirectory must supply rules for building sources it contributes
Fuation/ATK_MS6050/eMPL/%.o: ../Fuation/ATK_MS6050/eMPL/%.c
	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"f:/Gtip/KER/CH32V307VCT6/Debug" -I"f:/Gtip/KER/CH32V307VCT6/Core" -I"f:/Gtip/KER/CH32V307VCT6/User" -I"f:/Gtip/KER/CH32V307VCT6/Peripheral/inc" -I"f:/Gtip/KER/CH32V307VCT6/Driver/inc" -I"f:/Gtip/KER/CH32V307VCT6/Fuation/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
