#include "HJ_XJ7.h"
#include "Driver_ADC.h"
#include "control.h"
// �Ҷȴ���������
#define GRAY_SENSOR_COUNT 7
#define GRAY_THRESHOLD    1500  // �Ҷ���ֵ(����ʵ�ʵ���)
// ��ȡ���лҶȴ�����ֵ
// �Ҷȴ�����ADCͨ�����Ŷ���
const uint32_t GraySensorPins[GRAY_SENSOR_COUNT] = {
    GPIO_Pin_0,  // ������1 - ADC1ͨ��0
    GPIO_Pin_1,  // ������2 - ADC1ͨ��1
    GPIO_Pin_2,  // ������3 - ADC1ͨ��2
    GPIO_Pin_3,  // ������4 - ADC1ͨ��3
    GPIO_Pin_4,  // ������5 - ADC1ͨ��4
    GPIO_Pin_5,  // ������6 - ADC1ͨ��5
    GPIO_Pin_6   // ������7 - ADC1ͨ��6
};

// �Ҷȴ�����ADCֵ
uint16_t grayValues[GRAY_SENSOR_COUNT] = {0};
// ��ȡ�����Ҷȴ�����ֵ
uint16_t ReadGraySensor(uint8_t sensorIndex)
{
    if(sensorIndex >= GRAY_SENSOR_COUNT) return 0;
    
    // ����ADCͨ��
    ADC_RegularChannelConfig(ADC1, sensorIndex + 1, 1, ADC_SampleTime_239Cycles5);
    
    // ����ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // �ȴ�ת�����
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    return ADC_GetConversionValue(ADC1);
}


void ReadAllGraySensors(void)
{
    for(uint8_t i = 0; i < GRAY_SENSOR_COUNT; i++) {
        grayValues[i] = ReadGraySensor(i);
    }
}

// ѭ�������㷨
void TrackControl(void)
{
    // ��ȡ���д�����ֵ
    ReadAllGraySensors();
    
    // ��⴫����״̬(0:δ��⵽��, 1:��⵽��)
    uint8_t sensorState[GRAY_SENSOR_COUNT] = {0};
    for(uint8_t i = 0; i < GRAY_SENSOR_COUNT; i++) {
        sensorState[i] = (grayValues[i] > GRAY_THRESHOLD) ? 1 : 0;
    }
    
    // ����ƫ��(Ȩ�ط�)
    int8_t error = 0;
    int8_t weights[GRAY_SENSOR_COUNT] = {-3, -2, -1, 0, 1, 2, 3}; // ������Ȩ��
    
    uint8_t activeSensors = 0;
    for(uint8_t i = 0; i < GRAY_SENSOR_COUNT; i++) {
        if(sensorState[i]) {
            error += weights[i];
            activeSensors++;
        }
    }
    
    // ���û�д�������⵽�ߣ������ϴζ�����ֹͣ
    if(activeSensors == 0) {
        // ���Ը�����Ҫѡ��ֹͣ�򱣳�ǰ��
        SetAllMotors(0, 0, 0, 0);
        return;
    }
    
    // ����ƽ��ƫ��
    error = error / activeSensors;
    
    // PID����(�򻯰�)
    static int8_t lastError = 0;
    static int8_t integral = 0;
    
    int8_t Kp = 20;  // ����ϵ��
    int8_t Ki = 1;   // ����ϵ��
    int8_t Kd = 10;  // ΢��ϵ��
    
    integral += error;
    if(integral > 50) integral = 50;
    if(integral < -50) integral = -50;
    
    int8_t derivative = error - lastError;
    lastError = error;
    
    int8_t control = (Kp * error + Ki * integral + Kd * derivative) / 10;
    
    // �����ٶȺ�ת�����
    int8_t baseSpeed = 60;
    int8_t leftSpeed = baseSpeed - control;
    int8_t rightSpeed = baseSpeed + control;
    
    // �����ٶȷ�Χ
    if(leftSpeed > 100) leftSpeed = 100;
    if(leftSpeed < -100) leftSpeed = -100;
    if(rightSpeed > 100) rightSpeed = 100;
    if(rightSpeed < -100) rightSpeed = -100;
    
    // ���õ���ٶ�
    SetAllMotors(leftSpeed, rightSpeed, leftSpeed, rightSpeed);
}
