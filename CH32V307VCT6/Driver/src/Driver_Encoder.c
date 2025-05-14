#include "Driver_Encoder.h"

// �����������
#define MOTOR_NUM 4

// ���������Ŷ���
typedef struct {
    GPIO_TypeDef* gpio;
    uint16_t pin_a;
    uint16_t pin_b;
} Encoder_Pins;

// ������ƽṹ��
typedef struct {
    int32_t encoder_count;    // ����������
    int32_t last_count;       // �ϴμ���
    int16_t speed_rpm;        // �������ת��(RPM)
    Encoder_Pins encoder;     // ����������
} Motor_Control;

Motor_Control motors[MOTOR_NUM];

// ��ʼ������������
void Encoder_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // ���ñ���������Ϊ����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // ��ʼ�������������������
    motors[0].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_0, GPIO_Pin_1};
    motors[1].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_2, GPIO_Pin_3};
    motors[2].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_4, GPIO_Pin_5};
    motors[3].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_6, GPIO_Pin_7};
}

// ��ʼ����ʱ�������ٶȲ���
void TIM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // ��ʱ����������
    // ����ϵͳʱ��144MHz��Ԥ��Ƶ144-1������Ƶ��1MHz
    // ��ʱ����1000-1����1ms�ж�һ��
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 144 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // ʹ�ܶ�ʱ���ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM2, ENABLE);
}

// ������״̬��ȡ�ͼ���
void Read_Encoders(void)
 {
    static uint8_t last_state[MOTOR_NUM] = {0};
    
    for (int i = 0; i < MOTOR_NUM; i++) {
        uint8_t a = GPIO_ReadInputDataBit(motors[i].encoder.gpio, motors[i].encoder.pin_a);
        uint8_t b = GPIO_ReadInputDataBit(motors[i].encoder.gpio, motors[i].encoder.pin_b);
        uint8_t current_state = (a << 1) | b;
        uint8_t transition = (last_state[i] << 2) | current_state;
        
        // ״̬���жϷ��򲢼���
        switch (transition) {
            case 0b0001: case 0b0111: case 0b1110: case 0b1000:
                motors[i].encoder_count++; // ��ת
                break;
            case 0b0010: case 0b1011: case 0b1101: case 0b0100:
                motors[i].encoder_count--; // ��ת
                break;
            // ��������������������
        }
        
        last_state[i] = current_state;
    }
}

// ��ʱ���жϷ�����
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        static uint16_t count = 0;
        count++;
        
        // ÿ10ms��ȡһ�α�����
        Read_Encoders();
        
        // ÿ100ms����һ���ٶ�
        if (count >= 100) {
            count = 0;
            for (int i = 0; i < MOTOR_NUM; i++) {
                // ����ת�� (RPM)
                // MG310P20�������ֱ���: 7PPR (ÿת7������)
                // 100ms�ڼ����仯�� * 600 / (7 * 0.1) = RPM
                int32_t delta = motors[i].encoder_count - motors[i].last_count;
                motors[i].speed_rpm = delta * 6000 / 7;
                motors[i].last_count = motors[i].encoder_count;
                
                // ����ٶ���Ϣ (������)
                printf("Motor %d Speed: %d RPM\n", i+1, motors[i].speed_rpm);
            }
        }
    }
}
