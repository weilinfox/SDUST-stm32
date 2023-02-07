#ifndef _MYIIC_H_
#define _MYIIC_H_
#include "stm32f1xx_hal.h"

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */

#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */

#define PORT_I2C_SCL	GPIOB			/* GPIO端口 */
#define PIN_I2C_SCL		GPIO_PIN_6		/* GPIO引脚 */

#define PORT_I2C_SDA	GPIOB			/* GPIO端口 */
#define PIN_I2C_SDA		GPIO_PIN_7		/* GPIO引脚 */

#define I2C_SCL_PIN		GPIO_PIN_6			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_PIN_7			/* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_1()  PORT_I2C_SCL->BSRR = I2C_SCL_PIN				/* SCL = 1 */
#define I2C_SCL_0()  PORT_I2C_SCL->BRR = I2C_SCL_PIN				/* SCL = 0 */

#define I2C_SDA_1()  PORT_I2C_SDA->BSRR = I2C_SDA_PIN				/* SDA = 1 */
#define I2C_SDA_0()  PORT_I2C_SDA->BRR = I2C_SDA_PIN				/* SDA = 0 */

#define I2C_SDA_READ()  ((PORT_I2C_SDA->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#define I2C_SCL_READ()  ((PORT_I2C_SCL->IDR & I2C_SCL_PIN) != 0)	/* 读SCL口线状态 */

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

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
