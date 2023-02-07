#ifndef _BSP_TP_H_
#define _BSP_TP_H_
#include "stm32f1xx_hal.h"
#include "MyIIC.h"

#define I2C_DEVADD	0x80
#define GSL_PAGE_REG 0xf0
//#define I2C_PageSize  64      
#define TOUCH_CODE_LEN	5049	

struct fw_data
{
    uint8_t offset;
    uint32_t val;
};

extern volatile uint16_t PENX,PENY;

uint8_t CWL_TP_Init(void);
void GSL2681_IRQ_Callback(void);
void BSP_TP_ISR(void);

#endif
