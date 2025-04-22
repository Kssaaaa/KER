#include "esp32.h"

uint8_t rBuff[1000] = {0};
/**
* @description: ��ʼ��ESP32
*/
void ESP32_Init(void)
{
  /* 1. ��ʼ������2(ͨ������2��ESP32����ͨѶ) */
  MX_USART2_UART_Init();

  /* 2. ���� ESP32 */
  uint8_t *atCmd = "AT+RST=0\r\n";
  ESP32_Send_CMD(atCmd, strlen((char *)atCmd));
  HAL_Delay(3000);
}

/**
* @description: ����ATָ��
* @param {uint8_t} *cmd Ҫ���͵�ATָ��(ָ������� \r\n����)
* @param {uint16_t} cmdLength ָ���
*/
void ESP32_Send_CMD(uint8_t *cmd, uint16_t cmdLength)
{
  UART_Transmit(USART2, cmd, cmdLength, 1000);
  do
  {
    ESP32_ReadResponse(rBuff, 1000);
    printf("%s", rBuff);
  }while(strstr((char *)rBuff, "OK") == NULL); // һֱ�ȵ�����OK
  printf("\r\n=====================\r\n");
}

/**
* @description: ����ATָ�������������Ӧ
*     Ҫ���ǵ����յ���ӦΪ�Ƕ������ݡ�
*
* @param {uint8_t} responeBuff[] �洢���յĵ���Ӧ�Ļ�������
* @param {unin16_t} Size �������ĳ���
*/

void ESP32_ReadResponse(uint8_t responeBuff[], uint16_t size)
{
  /* ��ջ����� */
  memset(responeBuff, 0, size);
  uint16_t rxLen = 0;
//   HAL_UARTEx_ReceiveToIdle(&huart2, responeBuff, size, &rxLen, 10000);
}