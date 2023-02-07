#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
#include "stm32f1xx_hal.h"

typedef uint32_t	u32;
typedef uint16_t	u16;
typedef uint8_t		u8;

#define LCDBUFSIZE	800*480

#define RA8875_BASE		((uint32_t)(0x6C000000 | 0x00000000))
#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE +  0x0084)	/* FSMC 16??????,FSMC_A0????????A1 */
#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE +  0x0080)
#define RA8875_RAM_ADDR		RA8875_BASE

#define   BLUE0     0xEE56      //???
#define   BLACK    0x0000      	//??
#define   BLUE     0xF800      	//??
#define   RED      0x001F      	//??
#define   GRAY0    0xCE9A   	//??0 3165 00110 001011 00101
#define   GRAY1    0x8410      	//??1      00000 000000 00000
#define   GRAY2    0x4208      	//??2  1111111111011111
#define   WHITE    0xFFFF		//??   110010 110100 110100
#define   GREEN    0x07E0		//??   110010 110100 110100
#define   YELLOW   0xF800      	//??07FF

//extern void LcdSetArea(u16 x1, u16 y1, u16 x2, u16 y2);
extern void LcdPrintDot(u16 x, u16 y, u16 color);
extern void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color);
extern void LcdPrintVert(u16 x, u16 y, u16 height, u16 color);
//extern void LcdPrintRec(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
extern void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color);
extern void LcdClear(u16 color);
//extern void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor);
extern void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor,u8 mode);
extern void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,char *fmt,...);
extern void LcdPrint16bitBmp(u16* image,u16 x,u16 y,u16 widht,u16 height);
extern void LcdPrint8bitBmp(const u8* image,u16 x,u16 y,u16 widht,u16 height);
//extern void LcdPrintVertZuHe(u16 x, u16 y, u16 height, u16 y1);
extern void LcdPrintHorzZuHe(u16 x, u16 y, u16 width, u16 color);
void LcdPrintBIAS(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
//void Lcd_cursorFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);

/* ??emWin????? */
void RA8875_PutPixelGUI(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixelGUI(uint16_t _usX, uint16_t _usY);
void RA8875_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void RA8875_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor);
void RA8875_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_RTERect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t _usColor); 

#endif

