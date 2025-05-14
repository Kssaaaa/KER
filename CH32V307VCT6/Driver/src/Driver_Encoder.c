#include "Driver_Encoder.h"

// 电机数量定义
#define MOTOR_NUM 4

// 编码器引脚定义
typedef struct {
    GPIO_TypeDef* gpio;
    uint16_t pin_a;
    uint16_t pin_b;
} Encoder_Pins;

// 电机控制结构体
typedef struct {
    int32_t encoder_count;    // 编码器计数
    int32_t last_count;       // 上次计数
    int16_t speed_rpm;        // 计算出的转速(RPM)
    Encoder_Pins encoder;     // 编码器引脚
} Motor_Control;

Motor_Control motors[MOTOR_NUM];

// 初始化编码器引脚
void Encoder_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // 配置编码器引脚为输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 初始化电机编码器引脚配置
    motors[0].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_0, GPIO_Pin_1};
    motors[1].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_2, GPIO_Pin_3};
    motors[2].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_4, GPIO_Pin_5};
    motors[3].encoder = (Encoder_Pins){GPIOC, GPIO_Pin_6, GPIO_Pin_7};
}

// 初始化定时器用于速度测量
void TIM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // 定时器基础配置
    // 假设系统时钟144MHz，预分频144-1，计数频率1MHz
    // 定时周期1000-1，即1ms中断一次
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 144 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // 使能定时器中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM2, ENABLE);
}

// 编码器状态读取和计数
void Read_Encoders(void)
 {
    static uint8_t last_state[MOTOR_NUM] = {0};
    
    for (int i = 0; i < MOTOR_NUM; i++) {
        uint8_t a = GPIO_ReadInputDataBit(motors[i].encoder.gpio, motors[i].encoder.pin_a);
        uint8_t b = GPIO_ReadInputDataBit(motors[i].encoder.gpio, motors[i].encoder.pin_b);
        uint8_t current_state = (a << 1) | b;
        uint8_t transition = (last_state[i] << 2) | current_state;
        
        // 状态机判断方向并计数
        switch (transition) {
            case 0b0001: case 0b0111: case 0b1110: case 0b1000:
                motors[i].encoder_count++; // 正转
                break;
            case 0b0010: case 0b1011: case 0b1101: case 0b0100:
                motors[i].encoder_count--; // 反转
                break;
            // 其他情况不计数或处理错误
        }
        
        last_state[i] = current_state;
    }
}

// 定时器中断服务函数
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        static uint16_t count = 0;
        count++;
        
        // 每10ms读取一次编码器
        Read_Encoders();
        
        // 每100ms计算一次速度
        if (count >= 100) {
            count = 0;
            for (int i = 0; i < MOTOR_NUM; i++) {
                // 计算转速 (RPM)
                // MG310P20编码器分辨率: 7PPR (每转7个脉冲)
                // 100ms内计数变化量 * 600 / (7 * 0.1) = RPM
                int32_t delta = motors[i].encoder_count - motors[i].last_count;
                motors[i].speed_rpm = delta * 6000 / 7;
                motors[i].last_count = motors[i].encoder_count;
                
                // 输出速度信息 (调试用)
                printf("Motor %d Speed: %d RPM\n", i+1, motors[i].speed_rpm);
            }
        }
    }
}
