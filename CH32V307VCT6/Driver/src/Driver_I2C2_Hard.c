#include "Driver_I2C2_Hard.h"

#define I2C_DELAY Delay_Us(10)

/**
 * @description: 初始化
 * @return {*}
 */
void Driver_I2C2_Init(void)
{

    /* 1. 开启时钟 */
    /* 1.1 i2c硬件的时钟 */
    RCC->APB1PCENR |= RCC_I2C2EN;
    /* 1.2 GPIO时钟 */
    RCC->APB2PCENR |= RCC_IOPBEN;

    /* 2. 设置gpio的引脚的工作模式 */
    /*
        PB6->SCL
        PB7->SDA
            复用开漏输出: 既可以用于输出也可以输入. 外界要有上拉电阻.
                    用于输入的时候,最好先输出一个1,把线的控制权交给外界.

            MODE=11 CNF=11

     */
    GPIOB->CFGLR |= (GPIO_CFGLR_MODE6 | GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF6 | GPIO_CFGLR_CNF7);

    /* 3. 设置I2C2 */
    /* 3.1 配置硬件的工作模式  I2C  */
    I2C2->CTLR1 &= ~I2C_CTLR1_SMBUS;

    /* 3.2 配置给I2C设备提供的时钟的频率 36MHz*/
    I2C2->CTLR2 |= 36 << 0;

    /* 3.3 设置标准模式=0 or 快速模式=1 */
    I2C2->CKCFGR &= ~I2C_CKCFGR_FS;

    /* 3.3 配置I2C产生时钟的频率 100K or 400K
        Thigh=CKCFGR * Tcplk1

        CKCFGR = Thigh/=Tcplk1 = 5us / (1/36)us = 180

    */
    I2C2->CKCFGR |= 180 << 0;

    /* 3.4 时钟信号的上升沿
          100KHz的时候要求最大上升沿不超过1us(手册)。
            时钟频率是36MHz则 写入：1 /（1/36） + 1 = 37
           其实就是计算的 最大上升沿时间/时钟周期 + 1

    */
    I2C2->RTR |= 37;

    /* 3.4 使能I2C */
    I2C2->CTLR1 |= I2C_CTLR1_PE;
}

/**
 * @description: 起始信号
 * @return {*}
 */
uint8_t Driver_I2C2_Start(void)
{
    I2C2->CTLR1 |= I2C_CTLR1_START;

    uint16_t timeout = 0xffff;
    while (((I2C2->STAR1 & I2C_STAR1_SB) == 0) && timeout)
    {
        timeout--;
    }

    return timeout ? OK : FAIL;
}

/**
 * @description: 停止信号
 * @return {*}
 */
void Driver_I2C2_Stop(void)
{
    /* 产生终止条件 */
    I2C2->CTLR1 |= I2C_CTLR1_STOP;
}

/**
 * @description: 接收方产生应答信号
 */
void Driver_I2C2_Ack(void)
{
    /* 产生应答信号 */
    I2C2->CTLR1 |= I2C_CTLR1_ACK;
}

/**
 * @description: 接收方产生非应答信号
 */
void Driver_I2C2_NAck(void)
{
    I2C2->CTLR1 &= ~I2C_CTLR1_ACK;
}

/**
 * @description: 发送一个设备地址
 * @param {uint8_t} byte
 */
uint8_t Driver_I2C_SendAddr(uint8_t addr)
{
    uint16_t timeout = 0xffff;
    while (((I2C2->STAR1 & I2C_STAR1_TXE) == 0) && timeout)
    {
        timeout--;
    }
    // 把要发送的数据写入到数据寄存器
    I2C2->DATAR = addr;

    timeout = 0xffff;

    while (((I2C2->STAR1 & I2C_STAR1_ADDR) == 0) && timeout)
    {
        timeout--;
    }

    if (timeout)
    {
        I2C2->STAR2;
    }

    return timeout ? OK : FAIL;
}

/**
 * @description: 发送一个字节的数据
 * @param {uint8_t} byte 要发送的字节
 */
uint8_t Driver_I2C_SendByte(uint8_t byte)
{
    uint16_t timeout = 0xffff;
    while (((I2C2->STAR1 & I2C_STAR1_TXE) == 0) && timeout)
    {
        timeout--;
    }
    // 把要发送的数据写入到数据寄存器
    I2C2->DATAR = byte;

    timeout = 0xffff;

    while (((I2C2->STAR1 & I2C_STAR1_BTF) == 0) && timeout)
    {
        timeout--;
    }
    return timeout ? OK : FAIL;
}

/**
 * @description: 读一个字节的数据
 * @param {uint8_t} byte 要发送的字节
 */
uint8_t Driver_I2C_ReadByte(void)
{
    uint16_t timeout = 0xffff;

    while (((I2C2->STAR1 & I2C_STAR1_RXNE) == 0) && timeout)
    {
        timeout--;
    }
    // 把数据寄存器的值返回
    uint8_t data = timeout ? I2C2->DATAR : 0;
    return data;
}
