/***********************************************************************************
 Graphics ����ͼ������  huo  2016-12-23  09:45
 �������ͼ�ι���ʵ��
 TFTLCD IO��FSMC  Bank1_SRAM4_ADDR ((u32)0x6C000000)
			LCD_COMM_ADD	*((vu16 *)0X6C000084)
			LCD_DATA_ADD	*((vu16 *)0X6C000080)
			INT-PG7  WAIT-PG11
************************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include "TFTLCD.h"
#include "Graphics.h"

u16 fontbuf[256];
static __IO uint8_t s_ucRA8875Busy = 0;			/* æ��־, ����ģ��ͨ�� RA8875_IsBusy()�����ж�æ */

/**************************************************************************************
�������ܣ�����
���룺    u16  x,y    	        �������
	  	  u16  color		��ɫ
�����	  ��
**************************************************************************************/
void LcdPrintDot(u16 x, u16 y, u16 color)
{
	XY_Coordinate(x,y);
	WriteCommand(0x02);//MRWC
	WriteData(color);
}

/**************************************************************************************
�������ܣ�	��ˮƽֱ��
����	��  WORD  x    		���ĺ�����
	  		WORD  y  	    ����������
	  		WORD  width		ֱ�߳���
	  		WORD  color		��ɫ
���	��	��
**************************************************************************************/
void LcdPrintHorz(u16 x, u16 y, u16 width, u16 color)
{
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x,x+width-1,y,y); // ����ˮƽ����ʼ��
	WriteCommand(0x90);//д0x90�Ĵ���
	WriteData(0x80);   //��0x90�Ĵ���д����
	RA8875_WAITSTATUS();
}
/**************************************************************************************
�������ܣ�������������ſ�
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
�������ܣ�  ����ֱֱ��
����	��  u16  x    	    ���ĺ�����
	  		u16  y  	    ����������
	  		u16  height		ֱ�߸߶�
	  		u16  color		��ɫ
���	��	��
**************************************************************************************/
void LcdPrintVert(u16 x, u16 y, u16 height, u16 color)
{
	Text_color(color); //�趨��ɫ
	Geometric_Coordinate(x,x,y,y+height-1); // ������ʼ������
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
�������ܣ���һ��б��
���룺    u16  x1   ���ĺ�����
	  u16  y1  	    ����������
	  u16  x2		�����ĺ�����
	  u16  y2		������������
	  u16  color		��ɫ
�����	  ��
**************************************************************************************/
void LcdPrintBIAS(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x1,x2,y1,y2); // ������ʼ������
	WriteCommand(0x90);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
�������ܣ�������
���룺    ���ε����ϽǺ����½����꣬��Ŀ����ɫ
		  x1,y1  ���Ͻ�����
		  x2,y2  ���Ͻ�����
		  fill	 0  �����ο�
		  		 1  �������
�����	  ��
**************************************************************************************/
void LcdFillRec(u16 x1, u16 y1, u16 x2, u16 y2, u8 fill, u16 color)
{
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x1,x2,y1,y2); // �趨��������
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
�������ܣ����һ����������
���룺    ���ε����ϽǺ����½����꣬��Ŀ����ɫ
�����	  ��
**************************************************************************************/
/*void Lcd_cursorFillRec(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)//LcdFillRec
{
	RA8875_WAITSTATUS();
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(x1,x2,y1,y2-1); // �趨��������
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}*/
/**************************************************************************************
�������ܣ��������LCD��Ļ
���룺    Ŀ����ɫ
�����	  ��
**************************************************************************************/
void LcdClear(u16 color)
{
	Text_color(color); // �趨��ɫ
	Geometric_Coordinate(0,799,0,479); // �趨��������
	WriteCommand(0x90);
	WriteData(0xB0);
	RA8875_WAITSTATUS();
}
/**************************************************************************************
�������ܣ���LCD�ϴ�ӡһ������
���룺    code       �ַ�����
          x,y	     Ŀ����ʾλ������
	  	  color	     ������ɫ
	  	  bcolor     ������ɫ
�����	  ��
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
��������: 	����Ļ��ʾһ���ַ���
��    ��: 	char* str       �ַ���ָ��
                u16 x,y		Ŀ����ʾλ������
		u16 color	������ɫ
		u16 bcolor	������ɫ
		u8  mode    ģʽ��0-͸��1-��͸��
��    ��: 	��
**************************************************************************************/
void LcdPrintStr(char *str,u16 x,u16 y,u16 color,u16 bcolor,u8 mode)
{
	u8 i;
	u16 px;
	
	i = 0;
	px = x;
	while(str[i])
	{
//		if(str[i] > 0x7F)//�Ǻ���
//		{
//			LcdPrintHz((str[i]<<8)|str[i+1],px,y,color,bcolor);
//			i += 2;
//			px += 16;
//		}
//		else//�Ǻ���
		{
			Font_with_BackgroundTransparency();
			//Text_Background_Color(bcolor);
			Text_Foreground_Color(color);
			Font_Coordinate(px,y);//����
			Text_Mode();
			WriteCommand(0x02);
			WriteData(str[i]);
			i++;
			px += 8;
			Graphic_Mode();//�л���ͼ��ģʽ
		}
		RA8875_WAITSTATUS();
	}
}

/**************************************************************************************
��������: ����Ļ����ʽ��ʾһ���ַ���
��    ��: 

��    ��: ��
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
��������: ����Ļ��ʾһ 16bit bmpͼ��
��    ��: u16* image    	ͼ�����������ַ
          u16 x,y			���Ͻ�����
	  	  u16 width,height	ͼ�εĿ�Ⱥ͸߶�
��    ��: ��
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


/* ��������emWin�Ľӿں��� */
/*
*********************************************************************************************************
*	�� �� ��: RA8875_PutPixelGUI
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  :������ɫ
*	�� �� ֵ: ��
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
*	�� �� ��: RA8875_GetPixelGUI
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  :������ɫ
*	�� �� ֵ: RGB��ɫֵ
*********************************************************************************************************
*/
uint16_t RA8875_GetPixelGUI(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;
	
	s_ucRA8875Busy = 1;

//	RA8875_WriteReg(0x40, (1 << 0));	/* ����Ϊ��ͼģʽ����ȡ��겻�Զ���1 */
//  RA8875_SetReadCursor(_usX, _usY);	/* ���ö�ȡ���λ�� */
	
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
*	�� �� ��: GUI_FillRect
*	����˵��: ����emWin�Ľӿں���
*	��    ��:  _bright ���ȣ�0����255������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_RTERect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t _usColor) 
{
	s_ucRA8875Busy = 1;	
	
//	BTE_SetTarBlock(x0, y0, y1-y0+1, x1-x0+1, 0);	/* ����BTEλ�úͿ�ȸ߶� */
//	BTE_SetOperateCode(0x0C);	        			/* �趨BTE ������͹�դ������  REG[51h] Bit[3:0] = 0Ch */
//	RA8875_SetFrontColor(_usColor);			        /* ����BTEǰ��ɫ */
//	BTE_Start();				        			/* ����BTE ���� */
//	BTE_Wait();	

	LcdFillRec(x0,y0,x1,y1,1,_usColor);
	
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawHLine
*	����˵��: ����һ���̶���ɫ��ˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
*	��    �Σ�_usX1    ����ʼ��X����
*			  _usY1    ��ˮƽ�ߵ�Y����
*			  _usX2    ��������X����
*			  _usColor : ��ɫ
*	�� �� ֵ: ��
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

	RA8875_REG = 0x02; 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
	
	for (i = 0; i < _usX2 - _usX1 + 1; i++)
	{
		
		//RA8875_WriteData16(_usColor);
		RA8875_RAM = _usColor;
	}
	
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawHColorLine
*	����˵��: ����һ����ɫˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
*	��    �Σ�_usX1    ����ʼ��X����
*			  _usY1    ��ˮƽ�ߵ�Y����
*			  _usWidth ��ֱ�ߵĿ��
*			  _pColor : ��ɫ������
*	�� �� ֵ: ��
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

	RA8875_REG = 0x02; 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
	
	for (i = 0; i < _usWidth; i++)
	{
		RA8875_RAM = *_pColor++;
		//RA8875_WriteData16(*_pColor++);
	}
	
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawVLine
*	����˵��: ����һ����ֱ���� ����Ҫ����UCGUI�Ľӿں�����
*	��    �Σ� _usX1    : ��ֱ�ߵ�X����
*			  _usY1    : ��ʼ��Y����
*			  _usY2    : ������Y����
*			  _usColor : ��ɫ
*	�� �� ֵ: ��
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
		RA8875_REG = 0x02; 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
		RA8875_RAM = _usColor;
		
		RA8875_REG = 0x46; RA8875_RAM = _usX1;
		RA8875_REG = 0x47; RA8875_RAM = _usX1 >> 8;
		RA8875_REG = 0x48; RA8875_RAM = _usY1 + i;
		RA8875_REG = 0x49; RA8875_RAM = ( _usY1 + i) >> 8;
	}
	
	s_ucRA8875Busy = 0;
}

