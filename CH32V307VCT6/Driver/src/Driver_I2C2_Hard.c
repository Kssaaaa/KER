#include "Driver_I2C2_Hard.h"

#define I2C_DELAY Delay_Us(10)

/**
 * @description: ��ʼ��
 * @return {*}
 */
void Driver_I2C2_Init(void)
{

    /* 1. ����ʱ�� */
    /* 1.1 i2cӲ����ʱ�� */
    RCC->APB1PCENR |= RCC_I2C2EN;
    /* 1.2 GPIOʱ�� */
    RCC->APB2PCENR |= RCC_IOPBEN;

    /* 2. ����gpio�����ŵĹ���ģʽ */
    /*
        PB6->SCL
        PB7->SDA
            ���ÿ�©���: �ȿ����������Ҳ��������. ���Ҫ����������.
                    ���������ʱ��,��������һ��1,���ߵĿ���Ȩ�������.

            MODE=11 CNF=11

     */
    GPIOB->CFGLR |= (GPIO_CFGLR_MODE6 | GPIO_CFGLR_MODE7 | GPIO_CFGLR_CNF6 | GPIO_CFGLR_CNF7);

    /* 3. ����I2C2 */
    /* 3.1 ����Ӳ���Ĺ���ģʽ  I2C  */
    I2C2->CTLR1 &= ~I2C_CTLR1_SMBUS;

    /* 3.2 ���ø�I2C�豸�ṩ��ʱ�ӵ�Ƶ�� 36MHz*/
    I2C2->CTLR2 |= 36 << 0;

    /* 3.3 ���ñ�׼ģʽ=0 or ����ģʽ=1 */
    I2C2->CKCFGR &= ~I2C_CKCFGR_FS;

    /* 3.3 ����I2C����ʱ�ӵ�Ƶ�� 100K or 400K
        Thigh=CKCFGR * Tcplk1

        CKCFGR = Thigh/=Tcplk1 = 5us / (1/36)us = 180

    */
    I2C2->CKCFGR |= 180 << 0;

    /* 3.4 ʱ���źŵ�������
          100KHz��ʱ��Ҫ����������ز�����1us(�ֲ�)��
            ʱ��Ƶ����36MHz�� д�룺1 /��1/36�� + 1 = 37
           ��ʵ���Ǽ���� ���������ʱ��/ʱ������ + 1

    */
    I2C2->RTR |= 37;

    /* 3.4 ʹ��I2C */
    I2C2->CTLR1 |= I2C_CTLR1_PE;
}

/**
 * @description: ��ʼ�ź�
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
 * @description: ֹͣ�ź�
 * @return {*}
 */
void Driver_I2C2_Stop(void)
{
    /* ������ֹ���� */
    I2C2->CTLR1 |= I2C_CTLR1_STOP;
}

/**
 * @description: ���շ�����Ӧ���ź�
 */
void Driver_I2C2_Ack(void)
{
    /* ����Ӧ���ź� */
    I2C2->CTLR1 |= I2C_CTLR1_ACK;
}

/**
 * @description: ���շ�������Ӧ���ź�
 */
void Driver_I2C2_NAck(void)
{
    I2C2->CTLR1 &= ~I2C_CTLR1_ACK;
}

/**
 * @description: ����һ���豸��ַ
 * @param {uint8_t} byte
 */
uint8_t Driver_I2C_SendAddr(uint8_t addr)
{
    uint16_t timeout = 0xffff;
    while (((I2C2->STAR1 & I2C_STAR1_TXE) == 0) && timeout)
    {
        timeout--;
    }
    // ��Ҫ���͵�����д�뵽���ݼĴ���
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
 * @description: ����һ���ֽڵ�����
 * @param {uint8_t} byte Ҫ���͵��ֽ�
 */
uint8_t Driver_I2C_SendByte(uint8_t byte)
{
    uint16_t timeout = 0xffff;
    while (((I2C2->STAR1 & I2C_STAR1_TXE) == 0) && timeout)
    {
        timeout--;
    }
    // ��Ҫ���͵�����д�뵽���ݼĴ���
    I2C2->DATAR = byte;

    timeout = 0xffff;

    while (((I2C2->STAR1 & I2C_STAR1_BTF) == 0) && timeout)
    {
        timeout--;
    }
    return timeout ? OK : FAIL;
}

/**
 * @description: ��һ���ֽڵ�����
 * @param {uint8_t} byte Ҫ���͵��ֽ�
 */
uint8_t Driver_I2C_ReadByte(void)
{
    uint16_t timeout = 0xffff;

    while (((I2C2->STAR1 & I2C_STAR1_RXNE) == 0) && timeout)
    {
        timeout--;
    }
    // �����ݼĴ�����ֵ����
    uint8_t data = timeout ? I2C2->DATAR : 0;
    return data;
}
