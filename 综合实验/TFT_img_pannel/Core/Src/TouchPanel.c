/***********************************************************************************
 TouchPanel 驱动  huo  2016-08-26  09:00
 IIC IO：IIC_SCL-PB6  IIC_SDA-PB7
 TouchPanel_ADDR  0x80
************************************************************************************/
#include "TouchPanel.h"
#include "GSL268x_fw.h"

uint8_t I2C_WriteByte(uint8_t SendByte, uint8_t WriteAddress)
{		
	uint16_t m;

	for (m = 0; m < 1000; m++)
	{
		/* 第1步：发起I2C总线启动信号 */
		i2c_Start();
		/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
		i2c_SendByte(I2C_DEVADD | I2C_WR);	/* 此处是写指令 */
		/* 第3步：发送一个时钟，判断器件是否正确应答 */
		if (i2c_WaitAck() == 0)
		{
			break;
		}
	}
	if (m  == 1000)
	{
		/* EEPROM器件写超时,发送I2C总线停止信号 */
		i2c_Stop();
		return 0;
	}
	/* 第4步：发送字节地址 */
	i2c_SendByte((uint8_t)WriteAddress);
	if (i2c_WaitAck() != 0)
	{
		/* EEPROM器件无应答,发送I2C总线停止信号 */
		i2c_Stop();
		return 0;
	}
	/* 第6步：开始写入数据 */
	i2c_SendByte(SendByte);
	/* 第7步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		/* EEPROM器件无应答,发送I2C总线停止信号 */
		i2c_Stop();
		return 0;
	}
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;
}

uint8_t I2C_BufferWrite(uint8_t* pBuffer, uint8_t length, uint8_t WriteAddress)
{
	uint16_t i,m;

	for (i = 0; i < length; i++)
	{
		for (m = 0; m < 1000; m++)
		{
			/* 第1步：发起I2C总线启动信号 */
			i2c_Start();
			/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
			i2c_SendByte(I2C_DEVADD | I2C_WR);	/* 此处是写指令 */
			/* 第3步：发送一个时钟，判断器件是否正确应答 */
			if (i2c_WaitAck() == 0)
			{
				break;
			}
		}
		if (m  == 1000)
		{
			/* EEPROM器件写超时,发送I2C总线停止信号 */
			i2c_Stop();
			return 0;
		}
		/* 第4步：发送字节地址 */
		i2c_SendByte((uint8_t)WriteAddress);
		if (i2c_WaitAck() != 0)
		{
			/* EEPROM器件无应答,发送I2C总线停止信号 */
			i2c_Stop();
			return 0;
		}
		/* 第6步：开始写入数据 */
		i2c_SendByte(pBuffer[i]);
		/* 第7步：发送ACK */
		if (i2c_WaitAck() != 0)
		{
			/* EEPROM器件无应答,发送I2C总线停止信号 */
			i2c_Stop();
			return 0;
		}
		WriteAddress++;
	}
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;
}

uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t length, uint8_t ReadAddress)
{		
	uint16_t i;

	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(I2C_DEVADD | I2C_WR);	/* 此处是写指令 */
	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
			/* EEPROM器件无应答,发送I2C总线停止信号 */
			i2c_Stop();
			return 0;
	}
	/* 第4步：发送字节地址 */
	i2c_SendByte((uint8_t)ReadAddress);
	if (i2c_WaitAck() != 0)
	{
		/* EEPROM器件无应答,发送I2C总线停止信号 */
		i2c_Stop();
		return 0;
	}
	/* 第5步：发送I2C总线停止信号 */
	i2c_Stop();
  HAL_Delay(10);		//huo 20160905

	/* 第6步：重新启动I2C总线。下面开始读取数据 */
	i2c_Start();
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(I2C_DEVADD | I2C_RD);	/* 此处是读指令 */
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		/* EEPROM器件无应答,发送I2C总线停止信号 */
		i2c_Stop();
		return 0;
	}
	/* 第9步：循环读取数据 */
	for (i = 0; i < length; i++)
	{
		pBuffer[i] = i2c_ReadByte();	/* 读1个字节 */

		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != length - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 1;	/* 执行成功 */
}

void gsl_start_core(void)
{
	uint8_t temp = 0;
	
	temp = 0x00;
	I2C_WriteByte(temp,0xe0);
	HAL_Delay(1);
}

void gsl_reset_core(void)
{
	uint8_t temp = 0;
	
	temp = 0x88;
	I2C_WriteByte(temp,0xE0);
	HAL_Delay(1);
	
	temp = 0x04;
	I2C_WriteByte(temp,0xE4);
	HAL_Delay(1);
	
	temp = 0x00;
	I2C_WriteByte(temp,0xbc);
	HAL_Delay(1);
}

void gsl_clear_reg(void)
{
	uint8_t temp = 0;
	
	temp = 0x88;
	I2C_WriteByte(temp,0xE0);
	HAL_Delay(1);
	
	temp = 0x01;
	I2C_WriteByte(temp,0x80);
	HAL_Delay(1);

	temp = 0x04;
	I2C_WriteByte(temp,0xE4);
	HAL_Delay(1);
	
	temp = 0x00;
	I2C_WriteByte(temp,0xE0);
	HAL_Delay(1);
}

uint8_t check_mem_data(void)
{
	uint8_t read_buf[4]  = {0};

	I2C_BufferRead(read_buf,4,0xb0);
	HAL_Delay(1);
	if (read_buf[3] != 0x5a || read_buf[2] != 0x5a || read_buf[1] != 0x5a || read_buf[0] != 0x5a)
	{
		return 1;
	}
	else	 
	{
		return 0;
	}
}

void gsl_fw_load()
{
	uint8_t buf[5] = {0};
	uint32_t source_line = 0;
	uint8_t *cur = buf + 1;
	
	for (source_line = 0; source_line < TOUCH_CODE_LEN; source_line++) 
	{
		buf[0]= GSL2681_FW[source_line].offset;
		buf[1] = (uint8_t)(GSL2681_FW[source_line].val&0x000000ff);
		buf[2] = (uint8_t)((GSL2681_FW[source_line].val&0x0000ff00) >> 8);
		buf[3] = (uint8_t)((GSL2681_FW[source_line].val&0x00ff0000) >> 16);
		buf[4] = (uint8_t)((GSL2681_FW[source_line].val&0xff000000) >> 24);

		if (buf[0] == 0xf0)
		{
			I2C_WriteByte(buf[1],buf[0]);
		}
		else
		{
			I2C_BufferWrite(cur, 4,buf[0]);
		}
	}
}

volatile uint16_t PENX,PENY;
void GSL2681_IRQ_Callback(void)
{
	uint8_t TPCodebuf[4];

	I2C_BufferRead(&TPCodebuf[0],4,0x84);
	PENX = ((0x0F&TPCodebuf[3])<<8)|TPCodebuf[2];
	PENY = TPCodebuf[1]<<8|TPCodebuf[0];	
}

#ifdef BSP_TP_ISR_EN
void GSL2681_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	//INT (IRQ)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
	//GPIOG.14 中断线以及中断初始化配置   上升沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG,GPIO_PinSource14);

	EXTI_InitStructure.EXTI_Line=EXTI_Line14;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}
/*
*********************************************************************************************************
*	函 数 名: BSP_TP_ISR
*	功能说明: 板载TouchPanel 中断服务功能函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BSP_TP_ISR(void)
{
	static u16 X,Y;
//	BSP_Delay_ms(1);//消抖
	if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14))	 	
	{				 
		GSL2681_IRQ_Callback();	
		if((X != PENX) || (Y != PENY))
		{
			printf("X: %d, Y: %d \r\n",PENX,PENY);
			X = PENX;
			Y = PENY;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line14); //清除LINE14上的中断标志位  
}
#else
void GSL2681_GPIO_Init()
{
	//GPIO_InitTypeDef GPIO_InitStructure;

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void BSP_TP_ISR(void)
{
}
#endif

//初始化IIC接口
uint8_t CWL_TP_Init(void)
{
	i2c_Init();
	while(i2c_CheckDevice(I2C_DEVADD)) {};
	//init gpio
	GSL2681_GPIO_Init();

	//set register
	gsl_clear_reg();
	gsl_reset_core();
	gsl_fw_load();
	gsl_start_core();
	HAL_Delay(20);
	return check_mem_data();
}

