#include "Driver_LED.h"

/**
?* @description: ��LED���г�ʼ��
?*/
void Driver_LED_Init(void)
{
      /* 1. ��GPIOA��ʱ�� */
      RCC->APB2PCENR |= APB2PERIPH_BASE;

      /* 2. ���õ��Ķ˿ڵ����� PIN (PA0 PA1 PA8) ���ù���ģʽ: ͨ��������� MODE:11 CNF:00 */
      GPIOA->CFGLR |= (GPIO_CFGLR_MODE0 | GPIO_CFGLR_MODE1);
      GPIOA->CFGLR &= ~(GPIO_CFGLR_CNF0 | GPIO_CFGLR_CNF1);

      GPIOA->CFGHR |= GPIO_CFGHR_MODE8;
      GPIOA->CFGHR &= ~GPIO_CFGHR_CNF8;

      /* 3. �ر����е� */
      Drviver_LED_Off(LED_1);
      Drviver_LED_Off(LED_2);
      Drviver_LED_Off(LED_3);
}

/**
?* @description: ����ָ����LED
?* @param {uint32_t} led Ҫ������LED
?*/
void Drviver_LED_On(uint32_t led)
{
      GPIOA->OUTDR &= ~led;
      }

/**
?* @description: �ر�ָ����LED
?* @param {uint32_t} led Ҫ�رյ�LED
?*/
void Drviver_LED_Off(uint32_t led)
{
      GPIOA->OUTDR |= led;
}

/**
?* @description: ��תLED��״̬
?* @param {uint32_t} led Ҫ��ת��LED
?*/
void Drviver_LED_Toggle(uint32_t led)
{
      /* 1. ��ȡ���ŵĵ�ƽ,�����1(Ŀǰ�ǹر�), ��, ����͹ر� */
      if ((GPIOA->INDR & led) == 0)
      {
        Drviver_LED_Off(led);
      }
      else
      {
           Drviver_LED_On(led);
      }
}

/**
?* @description: �����������еĵ�
?* @param {uint32_t} leds ���е�
?* @param {uint8_t} size �Ƶĸ���
?*/
void Drviver_LED_OnAll(uint32_t leds[], uint8_t size)
{

      for (uint8_t i = 0; i < size; i++)
      {
      Drviver_LED_On(leds[i]);
      }
}

/**
?* @description: �ر����������еĵ�
?* @param {uint32_t} leds ���е�
?* @param {uint8_t} size �Ƶĸ���
?*/
void Drviver_LED_OffAll(uint32_t leds[], uint8_t size)
{
      for (uint8_t i = 0; i < size; i++)
      {
           Drviver_LED_Off(leds[i]);
      }
}