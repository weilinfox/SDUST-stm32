#ifndef _MYIIC_H_
#define _MYIIC_H_
#include "stm32f1xx_hal.h"

/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */

#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */

#define PORT_I2C_SCL	GPIOB			/* GPIO�˿� */
#define PIN_I2C_SCL		GPIO_PIN_6		/* GPIO���� */

#define PORT_I2C_SDA	GPIOB			/* GPIO�˿� */
#define PIN_I2C_SDA		GPIO_PIN_7		/* GPIO���� */

#define I2C_SCL_PIN		GPIO_PIN_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_PIN_7			/* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ� */
#define I2C_SCL_1()  PORT_I2C_SCL->BSRR = I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  PORT_I2C_SCL->BRR = I2C_SCL_PIN				/* SCL = 0 */

#define I2C_SDA_1()  PORT_I2C_SDA->BSRR = I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  PORT_I2C_SDA->BRR = I2C_SDA_PIN				/* SDA = 0 */

#define I2C_SDA_READ()  ((PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#define I2C_SCL_READ()  ((PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* ��SCL����״̬ */

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

void i2c_Init(void);
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif
