#include "mpu6050.h"

void MPU6050_Write_Byte(uint8_t reg, uint8_t data)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(I2C1, MPU6050_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    I2C_SendData(I2C1, reg);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
    
    I2C_SendData(I2C1, data);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t MPU6050_Read_Byte(uint8_t reg)
{
    uint8_t data;
    
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(I2C1, MPU6050_ADDR, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    I2C_SendData(I2C1, reg);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(I2C1, MPU6050_ADDR, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
    data = I2C_ReceiveData(I2C1);
    
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    return data;
}

void MPU6050_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    I2C_SendData(I2C1, reg);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    if(len > 1) {
        I2C_AcknowledgeConfig(I2C1, ENABLE);
    }
    
    for(uint8_t i=0; i<len; i++) {
        if(i == len-1) {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        buf[i] = I2C_ReceiveData(I2C1);
    }
}

uint8_t MPU6050_Init(void)
{
    uint8_t res;
    
    I2C_GPIO_Init();
    
    // 复位设备
    MPU6050_Write_Byte(MPU6050_RA_PWR_MGMT_1, 0x80);
    Delay_Ms(100);
    
    // 唤醒设备，选择时钟源
    MPU6050_Write_Byte(MPU6050_RA_PWR_MGMT_1, 0x01);
    Delay_Ms(5);
    
    // 设置陀螺仪量程 ±2000°/s
    MPU6050_Write_Byte(MPU6050_RA_GYRO_CONFIG, 0x18);
    
    // 设置加速度计量程 ±8g
    MPU6050_Write_Byte(MPU6050_RA_ACCEL_CONFIG, 0x10);
    
    // 设置低通滤波器带宽 5Hz
    MPU6050_Write_Byte(MPU6050_RA_CONFIG, 0x06);
    
    // 设置采样率 1kHz/(1+7)=125Hz
    MPU6050_Write_Byte(MPU6050_RA_SMPLRT_DIV, 0x07);
    
    // 关闭所有中断
    MPU6050_Write_Byte(MPU6050_RA_INT_ENABLE, 0x00);
    
    // 自检
    res = MPU6050_Read_Byte(MPU6050_RA_WHO_AM_I);
    if(res != 0x68) {
        return 1; // 初始化失败
    }
    
    return 0; // 初始化成功
}

void MPU6050_GetData(MPU6050_Data *mpu_data)
{
    uint8_t buf[14];
    
    // 读取14个字节的数据(0x3B-0x48)
    MPU6050_Read_Len(MPU6050_ADDR, MPU6050_RA_ACCEL_XOUT_H, 14, buf);
    
    // 加速度数据
    mpu_data->Accel_X = (int16_t)((buf[0] << 8) | buf[1]);
    mpu_data->Accel_Y = (int16_t)((buf[2] << 8) | buf[3]);
    mpu_data->Accel_Z = (int16_t)((buf[4] << 8) | buf[5]);
    
    // 温度数据
    mpu_data->Temp = (int16_t)((buf[6] << 8) | buf[7]);
    
    // 陀螺仪数据
    mpu_data->Gyro_X = (int16_t)((buf[8] << 8) | buf[9]);
    mpu_data->Gyro_Y = (int16_t)((buf[10] << 8) | buf[11]);
    mpu_data->Gyro_Z = (int16_t)((buf[12] << 8) | buf[13]);
    
    // 转换为实际物理量
    mpu_data->Ax = mpu_data->Accel_X / 4096.0f; // ±8g对应4096 LSB/g
    mpu_data->Ay = mpu_data->Accel_Y / 4096.0f;
    mpu_data->Az = mpu_data->Accel_Z / 4096.0f;
    
    mpu_data->Gx = mpu_data->Gyro_X / 16.4f; // ±2000°/s对应16.4 LSB/°/s
    mpu_data->Gy = mpu_data->Gyro_Y / 16.4f;
    mpu_data->Gz = mpu_data->Gyro_Z / 16.4f;
    
    mpu_data->Temperature = mpu_data->Temp / 340.0f + 36.53f; // 温度转换公式
}