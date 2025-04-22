#include "esp32.h"

uint8_t rBuff[1000] = {0};
/**
* @description: 初始化ESP32
*/
void ESP32_Init(void)
{
  /* 1. 初始化串口2(通过串口2与ESP32进行通讯) */
  MX_USART2_UART_Init();

  /* 2. 重启 ESP32 */
  uint8_t *atCmd = "AT+RST=0\r\n";
  ESP32_Send_CMD(atCmd, strlen((char *)atCmd));
  HAL_Delay(3000);
}

/**
* @description: 发送AT指令
* @param {uint8_t} *cmd 要发送的AT指令(指令必须以 \r\n结束)
* @param {uint16_t} cmdLength 指令长度
*/
void ESP32_Send_CMD(uint8_t *cmd, uint16_t cmdLength)
{
  UART_Transmit(USART2, cmd, cmdLength, 1000);
  do
  {
    ESP32_ReadResponse(rBuff, 1000);
    printf("%s", rBuff);
  }while(strstr((char *)rBuff, "OK") == NULL); // 一直等到读到OK
  printf("\r\n=====================\r\n");
}

/**
* @description: 发送AT指令后，用来接收响应
*     要考虑到接收的响应为非定长数据。
*
* @param {uint8_t} responeBuff[] 存储接收的的响应的缓冲区。
* @param {unin16_t} Size 缓冲区的长度
*/

void ESP32_ReadResponse(uint8_t responeBuff[], uint16_t size)
{
  /* 清空缓冲区 */
  memset(responeBuff, 0, size);
  uint16_t rxLen = 0;
//   HAL_UARTEx_ReceiveToIdle(&huart2, responeBuff, size, &rxLen, 10000);
}