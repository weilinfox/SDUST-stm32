/***********************************************************************************
 Graphics 基本图形驱动  huo  2016-12-23  09:45
 各类基本图形功能实现
 TFTLCD IO：FSMC  Bank1_SRAM4_ADDR ((u32)0x6C000000)
			LCD_COMM_ADD	*((vu16 *)0X6C000084)
			LCD_DATA_ADD	*((vu16 *)0X6C000080)
			INT-PG7  WAIT-PG11
************************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include "TFTLCD.h"
#include "Graphics.h"

u16 fontbuf[256];
static __IO uint8_t s_ucRA8875Busy = 0;			/* 忙标志, 其他模块通过 RA8875_IsBusy()函数判断忙 */

/**************************************************************************************
函数功能：画点
输入：    u16  x,y    	        点的坐标
	  	  u16  color		颜色
输出：	  无
**************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
	XY_Coordinate(x,y);
	WriteCommand(0x02);//MRWC
	WriteData(color);
}

/**************************************************************************************
函数功能：	画水平直线
输入	：  WORD  x    		起点的横坐标
	  		WORD  y  	    起点的纵坐标
	  		WORD  width		直线长度
	  		WORD  color		颜色
输出	：	无
**************************************************************************************/
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x,x+width-1,y,y); // 设置水平线起始点
	WriteCommand(0x90);//写0x90寄存器
	WriteData(0x80);   //向0x90寄存器写数据
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：画喷油器零件号框
**************************************************************************************/
void LcdPrintHorzZuHe(u16 x, u16 y, u16 width, u16 color)
{
	int i;
	for(i =y;i<=475;i+=42)
	{
		LcdPrintHorz(x, i, width, color);
		LcdPrintHorz(x, i+1, width, color);
	}
}
/**************************************************************************************
函数功能：  画垂直直线
输入	：  u16  x    	    起点的横坐标
	  		u16  y  	    起点的纵坐标
	  		u16  height		直线高度
	  		u16  color		颜色
输出	：	无
**************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
	Text_color(color); //设定颜色
	Geometric_Coordinate(x,x,y,y+height-1); // 设置起始点坐标
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：画一条斜线
输入：    u16  x1   起点的横坐标
	  u16  y1  	    起点的纵坐标
	  u16  x2		结束的横坐标
	  u16  y2		结束的纵坐标
	  u16  color		颜色
输出：	  无
**************************************************************************************/
void LcdPrintBIAS(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x1,x2,y1,y2); // 设置起始点坐标
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：画矩形
输入：    矩形的左上角和右下角坐标，和目标颜色
		  x1,y1  左上角坐标
		  x2,y2  右上角坐标
		  fill	 0  画矩形框
		  		 1  矩形填充
输出：	  无
**************************************************************************************/
void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x1,x2,y1,y2); // 设定矩形坐标
	WriteCommand(0x90);
	if (fill)
	{
		WriteData(0xB0);
	}
	else
	{
		WriteData(0x90);
	}
	RA8875_WAITSTATUS();
}

/**************************************************************************************
函数功能：填充一个矩形区域
输入：    矩形的左上角和右下角坐标，和目标颜色
输出：	  无
**************************************************************************************/
/*void Lcd_cursorFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)//LcdFillRec
{
	RA8875_WAITSTATUS();
	Text_color(color); // 设定颜色
	Geometric_Coordinate(x1,x2,y1,y2-1); // 设定矩形坐标
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}*/
/**************************************************************************************
函数功能：清除整个LCD屏幕
输入：    目标颜色
输出：	  无
**************************************************************************************/
void LcdClear(u16 color)
{
	Text_color(color); // 设定颜色
	Geometric_Coordinate(0,799,0,479); // 设定矩形坐标
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
函数功能：在LCD上打印一个汉字
输入：    code       字符内码
          x,y	     目标显示位置坐标
	  	  color	     字体颜色
	  	  bcolor     背景颜色
输出：	  无
**************************************************************************************/
//void LcdPrintHz(u16 code,u16 x,u16 y,u16 color,u16 bcolor)
//{
//	u16 i,j,ip,fcode;
//	
//	ip = 0;
//	do
//	{
//		if(sysHzzk[ip].code == code)
//		{
//			for(i=0;i<32;i=i+2)
//			{
//				fcode = sysHzzk[ip].HZCODE[i]<<8|sysHzzk[ip].HZCODE[i+1];
//				XY_Coordinate(x,y+i/2);
//				WriteCommand(0x02);
//				for(j=0;j<16;j++)
//				{
//					if(fcode&(0x8000>>j))
//					{
//						WriteData(color);
//					}
//					else
//					{
//						WriteData(bcolor);
//					}
//				}
//			}
//			return;
//		}
//		ip++;
//	}while(sysHzzk[ip].code);
//}

/**************************************************************************************
功能描述: 	在屏幕显示一个字符串
输    入: 	char* str       字符串指针
                u16 x,y		目标显示位置坐标
		u16 color	字体颜色
		u16 bcolor	背景颜色
		u8  mode    模式，0-透明1-非透明
输    出: 	无
**************************************************************************************/
void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor,u8 mode)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//是汉字
//		{
//			LcdPrintHz((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 16;
//		}
//		else//非汉字
		{
			Font_with_BackgroundTransparency();
			//Text_Background_Color(bcolor);
			Text_Foreground_Color(color);
			Font_Coordinate(px,y);//坐标
			Text_Mode();
			WriteCommand(0x02);
			WriteData(str[i]);
			i++;
			px += 8;
			Graphic_Mode();//切换回图形模式
		}
		RA8875_WAITSTATUS();
	}
}

/**************************************************************************************
功能描述: 在屏幕按格式显示一个字符串
输    入: 

输    出: 无
**************************************************************************************/
void LcdPrintf(u16 x,u16 y,u16 color,u16 bcolor,char *fmt,...)
{
    va_list ap;
    char str[64];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    LcdPrintStr(str,x,y,color,bcolor,0);
    va_end(ap);
}

/**************************************************************************************
功能描述: 在屏幕显示一 16bit bmp图形
输    入: u16* image    	图形数据数组地址
          u16 x,y			左上角坐标
	  	  u16 width,height	图形的宽度和高度
输    出: 无
**************************************************************************************/
void LcdPrint16bitBmp(u16* image,u16 x,u16 y,u16 widht,u16 height)
{
	u16 w,h;
	
	for(h=0;h<height;h++)
	{
		XY_Coordinate(x,y+h);
		WriteCommand(0x02);		//MRWC  REG[02h] -- Memory Read/Write Command
		for(w = 0; w< widht; w++)
		{
			WriteData(*image++);
		}
	}
}

void LcdPrint8bitBmp(const u8* image,u16 x,u16 y,u16 widht,u16 height)
{
	//u8  color8;
	u16 w,h;
	//u16 r,g,b;
    
	for(h=0;h<height;h++)
	{
		XY_Coordinate(x,y+h);
		WriteCommand(0x02);		//MRWC  REG[02h] -- Memory Read/Write Command
		for(w = 0; w< widht; w++)
		{
			WriteData(*image++);
			
			//color8 = *image++;
			/*
			0123456700000000 
			67000 345000 01200
            0011 1000 1110 0011
	
			*/
			//r = (color8 & 0xE0)<<13; 
			//g = (color8 & 0x1C)<<6;
    		//b = (color8 & 0x03)<<3;
            
            //r = ((color8 & 0xE0)<<13)|0x1800; 
    		//g = ((color8 & 0x1C)<<6) |0xE0;
    		//b = ((color8 & 0x03)<<3) |0x07;
    		
            //r = (color8 & 0x03)<<14;
            //g = (color8 & 0x1C)<<6;
    		//b = (color8 & 0xE0)>>11; 
			
            //WriteData(r|g|b|0x38E3);
		}
	}
}


/* 以下用于emWin的接口函数 */
/*
*********************************************************************************************************
*	函 数 名: RA8875_PutPixelGUI
*	功能说明: 画1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  :像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_PutPixelGUI(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{	
	s_ucRA8875Busy = 1;
	
	RA8875_REG = 0x46; RA8875_RAM = _usX;
	RA8875_REG = 0x47; RA8875_RAM = _usX >> 8;
	RA8875_REG = 0x48; RA8875_RAM = _usY;
	RA8875_REG = 0x49; RA8875_RAM = _usY >> 8;	

	RA8875_REG = 0x02;
	RA8875_RAM = _usColor;

	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_GetPixelGUI
*	功能说明: 读取1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  :像素颜色
*	返 回 值: RGB颜色值
*********************************************************************************************************
*/
uint16_t RA8875_GetPixelGUI(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;
	
	s_ucRA8875Busy = 1;

//	RA8875_WriteReg(0x40, (1 << 0));	/* 设置为绘图模式，读取光标不自动加1 */
//  RA8875_SetReadCursor(_usX, _usY);	/* 设置读取光标位置 */
	
	RA8875_REG = 0x4A; RA8875_RAM = _usX;
	RA8875_REG = 0x4B; RA8875_RAM = _usX >> 8;
	RA8875_REG = 0x4C; RA8875_RAM = _usY;
	RA8875_REG = 0x4D; RA8875_RAM = _usY >> 8;
	RA8875_REG = 0x02;
	
	usRGB = RA8875_RAM;
	usRGB = RA8875_RAM;
	usRGB = RA8875_RAM;

	s_ucRA8875Busy = 0;

	return usRGB;
}

/*
*********************************************************************************************************
*	函 数 名: GUI_FillRect
*	功能说明: 用于emWin的接口函数
*	形    参:  _bright 亮度，0是灭，255是最亮
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_RTERect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t _usColor) 
{
	s_ucRA8875Busy = 1;	
	
//	BTE_SetTarBlock(x0, y0, y1-y0+1, x1-x0+1, 0);	/* 设置BTE位置和宽度高度 */
//	BTE_SetOperateCode(0x0C);	        			/* 设定BTE 操作码和光栅运算码  REG[51h] Bit[3:0] = 0Ch */
//	RA8875_SetFrontColor(_usColor);			        /* 设置BTE前景色 */
//	BTE_Start();				        			/* 开启BTE 功能 */
//	BTE_Wait();	

	LcdFillRec(x0,y0,x1,y1,1,_usColor);
	
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DrawHLine
*	功能说明: 绘制一条固定颜色的水平线 （主要用于UCGUI的接口函数）
*	形    参：_usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usX2    ：结束点X坐标
*			  _usColor : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor)
{
	uint16_t i;

	s_ucRA8875Busy = 1;
	RA8875_REG = 0x46; RA8875_RAM = _usX1;
	RA8875_REG = 0x47; RA8875_RAM = _usX1 >> 8;
	RA8875_REG = 0x48; RA8875_RAM = _usY1;
	RA8875_REG = 0x49; RA8875_RAM = _usY1 >> 8;

	RA8875_REG = 0x02; 		/* 用于设定RA8875 进入内存(DDRAM或CGRAM)读取/写入模式 */
	
	for (i = 0; i < _usX2 - _usX1 + 1; i++)
	{
		
		//RA8875_WriteData16(_usColor);
		RA8875_RAM = _usColor;
	}
	
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DrawHColorLine
*	功能说明: 绘制一条彩色水平线 （主要用于UCGUI的接口函数）
*	形    参：_usX1    ：起始点X坐标
*			  _usY1    ：水平线的Y坐标
*			  _usWidth ：直线的宽度
*			  _pColor : 颜色缓冲区
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor)
{
	uint16_t i;

	s_ucRA8875Busy = 1;
	
	RA8875_REG = 0x46; RA8875_RAM = _usX1;
	RA8875_REG = 0x47; RA8875_RAM = _usX1 >> 8;
	RA8875_REG = 0x48; RA8875_RAM = _usY1;
	RA8875_REG = 0x49; RA8875_RAM = _usY1 >> 8;

	RA8875_REG = 0x02; 		/* 用于设定RA8875 进入内存(DDRAM或CGRAM)读取/写入模式 */
	
	for (i = 0; i < _usWidth; i++)
	{
		RA8875_RAM = *_pColor++;
		//RA8875_WriteData16(*_pColor++);
	}
	
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DrawVLine
*	功能说明: 绘制一条垂直线条 （主要用于UCGUI的接口函数）
*	形    参： _usX1    : 垂直线的X坐标
*			  _usY1    : 起始点Y坐标
*			  _usY2    : 结束点Y坐标
*			  _usColor : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor)
{
	uint16_t i;

	s_ucRA8875Busy = 1;

	RA8875_REG = 0x46; RA8875_RAM = _usX1;
	RA8875_REG = 0x47; RA8875_RAM = _usX1 >> 8;
	RA8875_REG = 0x48; RA8875_RAM = _usY1;
	RA8875_REG = 0x49; RA8875_RAM = _usY1 >> 8;
	
	for (i = 1; i <= _usY2 - _usY1 + 1; i++)
	{	
		RA8875_REG = 0x02; 		/* 用于设定RA8875 进入内存(DDRAM或CGRAM)读取/写入模式 */
		RA8875_RAM = _usColor;
		
		RA8875_REG = 0x46; RA8875_RAM = _usX1;
		RA8875_REG = 0x47; RA8875_RAM = _usX1 >> 8;
		RA8875_REG = 0x48; RA8875_RAM = _usY1 + i;
		RA8875_REG = 0x49; RA8875_RAM = ( _usY1 + i) >> 8;
	}
	
	s_ucRA8875Busy = 0;
}

