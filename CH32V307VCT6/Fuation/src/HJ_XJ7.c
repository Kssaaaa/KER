#include "HJ_XJ7.h"
#include "Driver_ADC.h"
#include "control.h"
// 灰度传感器定义
#define GRAY_SENSOR_COUNT 7
#define GRAY_THRESHOLD    1500  // 灰度阈值(根据实际调整)
// 读取所有灰度传感器值
// 灰度传感器ADC通道引脚定义
const uint32_t GraySensorPins[GRAY_SENSOR_COUNT] = {
    GPIO_Pin_0,  // 传感器1 - ADC1通道0
    GPIO_Pin_1,  // 传感器2 - ADC1通道1
    GPIO_Pin_2,  // 传感器3 - ADC1通道2
    GPIO_Pin_3,  // 传感器4 - ADC1通道3
    GPIO_Pin_4,  // 传感器5 - ADC1通道4
    GPIO_Pin_5,  // 传感器6 - ADC1通道5
    GPIO_Pin_6   // 传感器7 - ADC1通道6
};

// 灰度传感器ADC值
uint16_t grayValues[GRAY_SENSOR_COUNT] = {0};
// 读取单个灰度传感器值
uint16_t ReadGraySensor(uint8_t sensorIndex)
{
    if(sensorIndex >= GRAY_SENSOR_COUNT) return 0;
    
    // 设置ADC通道
    ADC_RegularChannelConfig(ADC1, sensorIndex + 1, 1, ADC_SampleTime_239Cycles5);
    
    // 启动ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换完成
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    return ADC_GetConversionValue(ADC1);
}


void ReadAllGraySensors(void)
{
    for(uint8_t i = 0; i < GRAY_SENSOR_COUNT; i++) {
        grayValues[i] = ReadGraySensor(i);
    }
}

// 循迹控制算法
void TrackControl(void)
{
    // 读取所有传感器值
    ReadAllGraySensors();
    
    // 检测传感器状态(0:未检测到线, 1:检测到线)
    uint8_t sensorState[GRAY_SENSOR_COUNT] = {0};
    for(uint8_t i = 0; i < GRAY_SENSOR_COUNT; i++) {
        sensorState[i] = (grayValues[i] > GRAY_THRESHOLD) ? 1 : 0;
    }
    
    // 计算偏差(权重法)
    int8_t error = 0;
    int8_t weights[GRAY_SENSOR_COUNT] = {-3, -2, -1, 0, 1, 2, 3}; // 传感器权重
    
    uint8_t activeSensors = 0;
    for(uint8_t i = 0; i < GRAY_SENSOR_COUNT; i++) {
        if(sensorState[i]) {
            error += weights[i];
            activeSensors++;
        }
    }
    
    // 如果没有传感器检测到线，保持上次动作或停止
    if(activeSensors == 0) {
        // 可以根据需要选择停止或保持前进
        SetAllMotors(0, 0, 0, 0);
        return;
    }
    
    // 计算平均偏差
    error = error / activeSensors;
    
    // PID控制(简化版)
    static int8_t lastError = 0;
    static int8_t integral = 0;
    
    int8_t Kp = 20;  // 比例系数
    int8_t Ki = 1;   // 积分系数
    int8_t Kd = 10;  // 微分系数
    
    integral += error;
    if(integral > 50) integral = 50;
    if(integral < -50) integral = -50;
    
    int8_t derivative = error - lastError;
    lastError = error;
    
    int8_t control = (Kp * error + Ki * integral + Kd * derivative) / 10;
    
    // 基础速度和转向控制
    int8_t baseSpeed = 60;
    int8_t leftSpeed = baseSpeed - control;
    int8_t rightSpeed = baseSpeed + control;
    
    // 限制速度范围
    if(leftSpeed > 100) leftSpeed = 100;
    if(leftSpeed < -100) leftSpeed = -100;
    if(rightSpeed > 100) rightSpeed = 100;
    if(rightSpeed < -100) rightSpeed = -100;
    
    // 设置电机速度
    SetAllMotors(leftSpeed, rightSpeed, leftSpeed, rightSpeed);
}
