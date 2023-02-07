/***********************************************************************************
 TFTLCD 驱动  huo  2016-12-23  09:45
 TFTLCD 寄存器配置
 TFTLCD IO：FSMC  Bank1_SRAM4_ADDR ((u32)0x6C000000)
			LCD_COMM_ADD	*((vu16 *)0X6C000084)
			LCD_DATA_ADD	*((vu16 *)0X6C000080)
			INT-PG7  WAIT-PG11
************************************************************************************/
#include "TFTLCD.h"
#include "Graphics.h"

void DelayMs(u32 nMs)
{
	u32 i;
	
	for(; nMs !=0; nMs--)
	{
		i = 10301;
		while(i--);
	}
}

u8 ReadData(void)
{   
	return LCD_DATA_ADD;
}
u8 ReadCOMM(void)
{
	return LCD_COMM_ADD;
}

/**************************************************************************************
函数功能：初始化TFTLCD
输入：    无0x20
输出：	  无 78m 80 
注意：    无 
**************************************************************************************/
void HAL_CWL_TFTLCD_Init(void)
{
	// BSP_TFTLCD_GPIO_Init();
	
	/* SYS_CLK = FIN*(PLLDIVN[4:0] +1)/((PLLDIVM+1)*(2^PLLDIVK[2:0]))=25*(15+1)/((0+1)*(2^2))=100 */
	WriteCommand(0x88);	
	WriteData(0x0F);	//PLLDIVN[4:0] = 15   PLLDIVM =0
	DelayMs(1);
	WriteCommand(0x89);
	WriteData(0x02);	//PLLDIVK[2:0] =2
	DelayMs(10);
	
	WriteCommand(0x10);  //SYSR  [3:2]=1xb 65k color display  [1:0]=1xb  16bit MCU interface
	WriteData(0x0F);   
	DelayMs(1);
	
	WriteCommand(0x04);  //PCLK inverse
	WriteData(0x81);
	DelayMs(1);
	
	//Horizontal set
	WriteCommand(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]                      
	WriteData(0x63);//Horizontal display width(pixels) = (HDWR + 1)*8                      
	WriteCommand(0x15);//Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)   
	WriteData(0x00);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]               
	WriteCommand(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]                         
	WriteData(0x03);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8                
	WriteCommand(0x17); //HSTR//HSYNC Start Position[4:0]                                      
	WriteData(0x03);//HSYNC Start Position(PCLK) = (HSTR + 1)*8                            
	WriteCommand(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.                     
	WriteData(0x0B);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8    
	DelayMs(1);
	//Vertical set                             
	WriteCommand(0x19); //VDHR0 //Vertical Display Height Bit [7:0]                            
	WriteData(0xdf);//Vertical pixels = VDHR + 1                                           
	WriteCommand(0x1a); //VDHR1 //Vertical Display Height Bit [8]                              
	WriteData(0x01);//Vertical pixels = VDHR + 1                                           
	WriteCommand(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]                        
	WriteData(0x1f);//Vertical Non-Display area = (VNDR + 1)                               
	WriteCommand(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]                          
	WriteData(0x00);//Vertical Non-Display area = (VNDR + 1)                               
	WriteCommand(0x1d); //VSTR0 //VSYNC Start Position[7:0]                                    
	WriteData(0x16);//VSYNC Start Position(PCLK) = (VSTR + 1)                              
	WriteCommand(0x1e); //VSTR1 //VSYNC Start Position[8]                                      
	WriteData(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)                              
	WriteCommand(0x1f); //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]                        
	WriteData(0x01);//VSYNC Pulse Width(PCLK) = (VPWR + 1)                                 
	DelayMs(1);
	
	//Active window  set 
	//setting active window X
	WriteCommand(0x30); //Horizontal Start Point 0 of Active Window (HSAW0)
	WriteData(0x00); //Horizontal Start Point of Active Window [7:0]
	WriteCommand(0x31); //Horizontal Start Point 1 of Active Window (HSAW1)	   
	WriteData(0x00); //Horizontal Start Point of Active Window [9:8]
	WriteCommand(0x34); //Horizontal End Point 0 of Active Window (HEAW0)
	WriteData(0x1F); //Horizontal End Point of Active Window [7:0]
	WriteCommand(0x35); //Horizontal End Point 1 of Active Window (HEAW1)	   
	WriteData(0x03); //Horizontal End Point of Active Window [9:8]
	DelayMs(1);
	
	//setting active window Y
	WriteCommand(0x32); //Vertical Start Point 0 of Active Window (VSAW0)
	WriteData(0x00); //Vertical Start Point of Active Window [7:0]
	WriteCommand(0x33); //Vertical Start Point 1 of Active Window (VSAW1)	   
	WriteData(0x00); //Vertical Start Point of Active Window [8]
	WriteCommand(0x36); //Vertical End Point of Active Window 0 (VEAW0)
	WriteData(0xdf); //Vertical End Point of Active Window [7:0]
	WriteCommand(0x37); //Vertical End Point of Active Window 1 (VEAW1)	   
	WriteData(0x01); //Vertical End Point of Active Window [8]
	DelayMs(1);
	
//	WriteCommand(0x70);		//set TP sample time  & ADC clock 
//	WriteData(0xB3);//0xb2
//	RA8875_WAITSTATUS();
//	TP_auto_mode();			//enable auto mode
//	Touch_Panel_Interrupt_Enable();	// demo code using Auto mode + polling 8Fh to confirm touch
//	clear_TP_interrupt();
	Active_Window(0,799,0,479);	//设定面板分辨率为800*480 pixel
//	HDIR_SEGn_SEG0();		//让屏幕显示颠倒90度
	Clear_Active_Window();
	Text_Foreground_Color(GRAY0);
	Text_Background_Color(GRAY0);
	
	//Font_with_BackgroundTransparency();
	Memory_Clear();
	Display_ON();
	
	//设置背光
//	PWM1_enable();
//	PWM1_fnuction_sel();
//	PWM1_clock_ratio(0x03);//0x03
//	PWM1_duty_cycle(0x00);
	
	//设置数字光标
	Text_Cursor_Horizontal_Size(24);
	Text_Cursor_Vertical_Size(4);
	Text_Blink_Time(0x35);
	Text_Cursor_Blink_Enable();
	//Text_Cursor_Enable();
	Text_Cursor_Disable();
	MemoryWrite_Cursor_autoIncrease();//写光标加1，才和数据显示有关
	//MemoryWrite_Cursor_NoautoIncrease();
	
	//设置图形鼠标箭头
	/*Graphic_Cursor_Color_0(0xFF);
	Graphic_Cursor_Color_1(0x00);
	Graphic_Cursor_Coordinate(100,100);
	Graphic_Cursor();	//No_Graphic_Cursor();
	Write_To_GraphicCursor();
	Graphic_Cursor_Set2();
	Graphic_Mode();
	
	//往寄存器里写入鼠标图形
	WriteCommand(0x02);//MRWC
	u8 ii,jj;
	for(ii=0;ii<32;ii++)
	{
		jj=8;
		do
		{
			jj--;
			WriteData(gImage_wait_l[ii][jj]);
			RA8875_WAITSTATUS();
		}while(jj);
	}
	DelayMs(3);
	Write_To_Bank1and2();
	Write_To_Bank1();
	Graphic_Cursor_Set2();
	DelayMs(1);*/
}

/*
static void BSP_TFTLCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PENIRQ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//8875WAIT
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}
*/


void Text_color(u16 color)//0xF800 == RED
{
	WriteCommand(0x63);//放R
	WriteData((color&0xf800)>>11);//5  R
	
	WriteCommand(0x64);
	WriteData((color&0x07e0)>>5);//6  G
	
	WriteCommand(0x65);
	WriteData((color&0x001f));//5    B
	
	RA8875_WAITSTATUS();
}
//RA8875等待空闲
void RA8875_WAITSTATUS(void)
{
	while(!(GPIOG->IDR & GPIO_PIN_11 ));
}
u8 LCD_StatusRead(void)
{
	u16 Data;
	Data = ReadCOMM();
	return Data;
}
				
//------------------------//
//STATUS 
void Chk_Busy(void)
{
	u8 temp;
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x80)==0x80);
}
void Chk_Busy_BTE(void)
{
	u8 temp;
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x40)==0x40);
}
void Chk_Busy_DIR_Access(void)
{
	u8  temp;
	do
	{
		temp=LCD_StatusRead();
	}while((temp&0x01)==0x01);
}

//REG[BFh]
void Chk_DMA_Busy(void)
{
	u8 temp; 	
	do
	{
		WriteCommand(0xBF);
		temp = ReadData();
	}while((temp&0x01)==0x01);
}
//REG[01h]		 
void Display_ON(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x80);
	RA8875_WAITSTATUS();
}
void Display_OFF(void)
{
	WriteCommand(0x01);//PWRR
	WriteData(0x00);
}

void Normal_Mode_exitSleep(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x00);
}
void Sleep_Mode(void)
{
	WriteCommand(0x01);//PWRR
	WriteData(0x02);
}
void Software_Reset(void)
{	
	WriteCommand(0x01);//PWRR
	WriteData(0x01);
	WriteData(0x00);
	DelayMs(1);// 
}							   
//-------------------------------//
//REG[04h] for test 
void PCLK_inversion(void)
{
	u8 temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp |= cSetD7;
	WriteData(temp);
}
void PCLK_non_inversion(void)
{
	u8 temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp &= cClrD7;
	WriteData(temp);
}

void PCLK_width(u8 setx) //u8[1:0]
{
	u8 temp;
  	WriteCommand(0x04);//PWRR
	temp = ReadData();
	temp &=0x80;
	temp |= setx;
	WriteData(temp);
}

//-------------------------------//
//REG[05h]
void Serial_ROM_select0(void)
{
	u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=cClrD7;
	WriteData(temp);
}

void Serial_ROM_select1(void)
{
	u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp |=cSetD7;
	WriteData(temp);
}

void Serial_ROM_Address_set_24u8(void)
{
	u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=cClrD6;
	WriteData(temp);
}

void Serial_ROM_Address_set_32u8(void)
{
	u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp |=cSetD6;
	WriteData(temp);
}

void Select_Serial_Waveform_mode0(void)
{	u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=cClrD5;
	WriteData(temp);
}

void Select_Serial_Waveform_mode3(void)
{	u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp |=cSetD5;
	WriteData(temp);
}

void SERIAL_ROM_Read_Cycle_4bus(void)
{   u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=0xE7;
	WriteData(temp);
}

void SERIAL_ROM_Read_Cycle_5bus(void)
{
	u8 temp;
	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=0xE7;
	temp |=cSetD3;
	WriteData(temp);
}

void SERIAL_ROM_Read_Cycle_6bus(void)
{
	u8 temp;
	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=0xE7;
	temp |=cSetD4;
	WriteData(temp);
}

void SERIAL_ROM_Font_mode(void)
{
	u8 temp;
  	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=cClrD2;
	WriteData(temp);
}

void SERIAL_ROM_DMA_mode(void)
{
	u8 temp;
	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp |=cSetD2;
	WriteData(temp);
}

void SERIAL_ROM_Signal_mode(void)
{
	u8 temp;
	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=0xFC;
	WriteData(temp);
}

void SERIAL_ROM_Dual_mode0(void)
{
	u8 temp;
	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=0xFC;
	temp |=cSetD1;
	WriteData(temp);
}

void SERIAL_ROM_Dual_mode1(void)
{
	u8 temp;
	WriteCommand(0x05);//PWRR
	temp = ReadData();
	temp &=0xFC;
	temp |=cSetD1;
	temp |=cSetD0;
	WriteData(temp);
}

void SROM_CLK_DIV(u8 CLK_DIV)
{
	WriteCommand(0x06);
	WriteData(CLK_DIV);
}

//-------------------------------//
//REG[10h]
void Color_256(void)
{
	u8 temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
} 

void Color_65K(void)
{
	u8 temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	WriteCommand(0x10);//SYSR
	WriteData(temp);
}
void MPU_8u8_Interface(void)
{
	u8 temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp &= cClrD1D0 ;
	WriteData(temp);
}

void MPU_16u8_Interface(void)
{
	u8 temp;
	WriteCommand(0x10);//SYSR
	temp = ReadData();
	temp |= cSetD1ClrD0 ;
	WriteData(temp);
} 								 

//--------------------------------//
//REG[12h] 
u8 GPI_data(void)
{
	u8 temp;
	WriteCommand(0x12);//SYSR
	temp = ReadData();
	return temp;
}

void GPO_data(u8 setx)
{
	WriteCommand(0x13); 
	WriteData(setx);
}

//---------------------------------//
//REG[20h]
void One_Layer(void)
{
	u8 temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}	
				 
void Two_Layers(void)
{
	u8 temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}

void HDIR_SEG0_SEGn(void)
{
	u8 temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD3 ;
	WriteData(temp);
}
void HDIR_SEGn_SEG0(void)
{
	u8 temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD3 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}
void VDIR_COM0_COMn(void)
{
	u8 temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp &= cClrD2 ;
	WriteData(temp);
}
void VDIR_COMn_COM0(void)
{
	u8 temp;
	WriteCommand(0x20);//DPCR
	temp = ReadData();
	temp |= cSetD2 ;
	WriteData(temp);
}


//----------------------------------//
//REG[21h] 
void CGROM_Font(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}
void CGRAM_Font(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}

void Internal_CGROM(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp &= cClrD5 ;
	WriteData(temp);
}
void External_CGROM(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp |= cSetD5 ;
	WriteData(temp);
}

void ISO8859_1(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp &= cClrD1D0 ;
	WriteData(temp);
}
void ISO8859_2(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	WriteData(temp);
}
void ISO8859_3(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	WriteData(temp);
}
void ISO8859_4(void)
{
	u8 temp;
	WriteCommand(0x21);//FNCR0
	temp = ReadData();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	WriteData(temp);
}								 

//--------------------------------//
//REG[22h]
void No_FullAlignment(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}
void FullAlignment(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}
void Font_with_BackgroundColor(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);
}
void Font_with_BackgroundTransparency(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	//temp = ReadData();
	temp = cSetD6 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

void NoRotate_Font(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD4 ;
	WriteData(temp);
}
void Rotate90_Font(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp |= cSetD4 ;
	WriteData(temp);
}

void Horizontal_FontEnlarge_x1(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
}
void Horizontal_FontEnlarge_x2(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	WriteData(temp);
}
void Horizontal_FontEnlarge_x3(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	WriteData(temp);
}
void Horizontal_FontEnlarge_x4(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	WriteData(temp);
}
void Vertical_FontEnlarge_x1(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD1D0 ;
	WriteData(temp);
}
void Vertical_FontEnlarge_x2(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD1D0 ;
	temp |= cClrD1SetD0 ;
	WriteData(temp);
}
void Vertical_FontEnlarge_x3(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD1D0 ;
	temp |= cSetD1ClrD0 ;
	WriteData(temp);
}
void Vertical_FontEnlarge_x4(void)
{
	u8 temp;
	WriteCommand(0x22);//FNCR1
	temp = ReadData();
	temp &= cClrD1D0 ;
	temp |= cSetD1D0 ;
	WriteData(temp);
}

//-------------------------------------------//
//REG[23h]									 
void CGRAM_Select_Number(u8 setx)
{ 
	WriteCommand(0x23);//CGSR
	WriteData(setx);
}	
								
//-------------------------------------------//
//REG[24h]REG[25h]REG[26h]REG[27h]
void Scroll_Offset(u16 X,u16 Y)
{
	WriteCommand(0x24);//HOFS0
	WriteData(X);
	//RA8875_WAITSTATUS();
	WriteCommand(0x25);//HOFS1	   
	WriteData(X>>8);
	//RA8875_WAITSTATUS();
	WriteCommand(0x26);//VOFS0
	WriteData(Y);  
	//RA8875_WAITSTATUS();
	WriteCommand(0x27);//VOFS1	   
	WriteData(Y>>8); 
	RA8875_WAITSTATUS();
}	 
//-------------------------------------------//
//REG[29h]
void Line_distance(u8 setx)
{ 
	WriteCommand(0x29);//LDS
	WriteData(setx);
}	  
//-------------------------------------------//
//REG[2Ah]REG[2Bh]REG[2Ch]REG[2Dh]
void Font_Coordinate(u16 X,u16 Y)
{
 
	WriteCommand(0x2A);//HOFS0
	WriteData(X);

	WriteCommand(0x2B);//HOFS1	   
	WriteData(X>>8);
	
	WriteCommand(0x2C);//VOFS0
	WriteData(Y);
	
	WriteCommand(0x2D);//VOFS1	   
	WriteData(Y>>8); 
}


//-------------------------------------------//
//REG[2Eh]
void Font_size_16x16_8x16(void) 
{
 	u8 temp;
	WriteCommand(0x2E);//FNCR1
	temp = ReadData();
	temp &= 0x3F;
	WriteData(temp);
}

void Font_size_24x24_12x24(void) 
{
 	u8 temp;
	WriteCommand(0x2E);//FNCR1
	temp = ReadData();
	temp &= 0x3F;
	temp |= 0x40; 
	WriteData(temp);
}

void Font_size_32x32_16x32(void) 
{
 	u8 temp;
	WriteCommand(0x2E);//FNCR1
	temp = ReadData();
	temp &= 0x3f;
	temp |= 0x80; 
	WriteData(temp);
}


void Font_spacing_set(u8 setx) //u8[5:0]
{
	u8 temp,temp1;
	temp1=setx&0x3F;
   	WriteCommand(0x2E);//FNCR1
	temp = ReadData();
	temp &= 0xC0;
	temp |= temp1; 
	WriteData(temp);
}

//-------------------------------------------//
//REG[2Fh]
void GT_serial_ROM_select_GT21L16T1W(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0x1F;
	WriteData(temp);
}

void GT_serial_ROM_select_GT23L16U2W(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0x1F;
	temp |= 0x20;
	WriteData(temp);
}

void GT_serial_ROM_select_GT23L24T3Y(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0x1F;
	temp |= 0x40;
	WriteData(temp);
}

void GT_serial_ROM_select_GT23L24M1Z(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0x1F;
	temp |= 0x60;
	WriteData(temp);
}

void GT_serial_ROM_select_GT23L32S4W(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0x1F;
	temp |= 0x80;
	WriteData(temp);
}

void Font_code_GB2312(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	WriteData(temp);
}

void Font_code_GB12345(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	temp |= 0x04;
	WriteData(temp);
}

void Font_code_BIG5(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	temp |= 0x08;
	WriteData(temp);
}

void Font_code_UNICODE(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	temp |= 0x0C;
	WriteData(temp);
}

void Font_code_ASCII(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	temp |= 0x10;
	WriteData(temp);
}

void Font_code_UNIJIS(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	temp |= 0x14;
	WriteData(temp);
}

void Font_code_JIS0208(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	temp |= 0x18;
	WriteData(temp);
}

void Font_code_LATIN(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	temp |= 0x1C;
	WriteData(temp);
}

void Font_Standard(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xFC;
	WriteData(temp);
}

void Font_Arial(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xFC;
	temp |= 0x01;
	WriteData(temp);
}

void Font_Roman(void)
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= 0xFC;
	temp |= 0x02;
	WriteData(temp);
}

void Font_Bold(void) //for ASCII
{
	u8 temp;
	WriteCommand(0x2F);//FNCR1
	temp = ReadData();
	temp &= cClrD1D0;
	temp |= cSetD1D0;
	WriteData(temp);
}

//-------------------------------------------//
//REG[30h]~REG[37h]
void Active_Window(u16 XL,u16 XR ,u16 YT ,u16 YB)
{
	//setting active window X
	
	WriteCommand(0x30);//HSAW0
	WriteData(XL); 
	WriteCommand(0x31);//HSAW1	   
	WriteData(XL>>8);
	
	WriteCommand(0x34);//HEAW0
	WriteData(XR);
	WriteCommand(0x35);//HEAW1	   
	WriteData(XR>>8);
	
	//setting active window Y
	
	WriteCommand(0x32);//VSAW0
	WriteData(YT);  
	WriteCommand(0x33);//VSAW1	   
	WriteData(YT>>8);
	
	WriteCommand(0x36);//VEAW0
	WriteData(YB); 
	WriteCommand(0x37);//VEAW1	   
	WriteData(YB>>8);
	RA8875_WAITSTATUS();
}
//--------------------------------------------//
//REG[38h]~REG[3Fh]

void Scroll_Window(u16 XL,u16 XR ,u16 YT ,u16 YB)
{
	WriteCommand(0x38);//HSSW0
	WriteData(XL);
	WriteCommand(0x39);//HSSW1	   
	WriteData(XL>>8);
	
	WriteCommand(0x3c);//HESW0
	WriteData(XR);  
	WriteCommand(0x3d);//HESW1	   
	WriteData(XR>>8);   
	
	WriteCommand(0x3a);//VSSW0
	WriteData(YT);
	WriteCommand(0x3b);//VSSW1	   
	WriteData(YT>>8);
	
	WriteCommand(0x3e);//VESW0
	WriteData(YB);  
	WriteCommand(0x3f);//VESW1	   
	WriteData(YB>>8);
}  


//--------------------------------------------//
//REG[40h]
void Graphic_Mode(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD7 ;
	temp &= 0x00 ;
	WriteData(temp);
}
void Graphic_Mode_Memery_Cursor_NoAdd(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD7 ;
	temp &= 0x00 ;
	temp |= 0x02 ;
	WriteData(temp);
}

void Text_Mode(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);	 
} 

void Text_Cursor_Disable(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	//temp &= cClrD6 ;
	temp &= 0x8F ;
	WriteData(temp);
}

void Text_Cursor_Enable(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	//temp |= cSetD6 ;//0x40
	temp |= 0x60;
	WriteData(temp);
} 

void Text_Cursor_Blink_Disable(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD5 ;
	//temp &= 0x8F ;
	WriteData(temp);
}

void Text_Cursor_Blink_Enable(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp |= cSetD5 ;//0x20
	WriteData(temp);
}

void Memory_Write_LeftRight_TopDown(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
}

void Memory_Write_RightLeft_TopDown(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cClrD3SetD2 ;
	WriteData(temp);
}

void Memory_Write_DownTop_LeftRight(void)
{
	u8 temp;	
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3D2 ;
	WriteData(temp);
}

void Memory_Write_TopDown_LeftRight(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD3D2 ;
	temp |= cSetD3ClrD2 ;
	WriteData(temp);
} 

void MemoryWrite_Cursor_autoIncrease(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD1 ;
	WriteData(temp);
}

void MemoryWrite_Cursor_NoautoIncrease(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp |= cSetD1 ;
	WriteData(temp);
}

void MemoryRead_Cursor_autoIncrease(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp &= cClrD0 ;
	WriteData(temp);
}

void MemoryRead_Cursor_NoautoIncrease(void)
{
	u8 temp;
	WriteCommand(0x40);//MWCR0
	temp = ReadData();
	temp |= cSetD0 ;
	WriteData(temp);
}  										 

//--------------------------------------------//
//REG[41h]
void No_Graphic_Cursor(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}

void Graphic_Cursor(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp |= cSetD7 ;
	temp |= 0xF0 ;
	WriteData(temp);
}

void Graphic_Cursor_Set1(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4;
	WriteData(temp);
}

void Graphic_Cursor_Set2(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6D5SetD4 ;
	WriteData(temp);
}

void Graphic_Cursor_Set3(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5ClrD4 ;
	WriteData(temp);
}

void Graphic_Cursor_Set4(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cClrD6SetD5D4 ;
	WriteData(temp);
}

void Graphic_Cursor_Set5(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5D4 ;
	WriteData(temp);
}

void Graphic_Cursor_Set6(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6ClrD5SetD4 ;
	WriteData(temp);
}

void Graphic_Cursor_Set7(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5ClrD4 ;
	WriteData(temp);
}

void Graphic_Cursor_Set8(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD6D5D4 ;
	temp |= cSetD6D5D4 ;
	WriteData(temp);
}

void Write_To_Bank1and2(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD3D2 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

void Write_To_CGRAM(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cClrD3SetD2 ;
	WriteData(temp);
}

void Write_To_GraphicCursor(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cSetD3ClrD2 ;
	WriteData(temp);
}

void Write_To_Pattern(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= 0xf3;
	temp |= cSetD3D2 ;
	WriteData(temp);
}
						   
void Write_To_Bank1(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp &= cClrD0 ;
	WriteData(temp);
}

void Write_To_Bank2(void)
{
	u8 temp;
	WriteCommand(0x41);//MWCR1
	temp = ReadData();
	temp |= cSetD0 ;
	WriteData(temp);  
}	
							
//--------------------------------------------//
//--------------------------------------------//
//REG[44h]
void Text_Blink_Time(u8 setx)
{	
	WriteCommand(0x44);//CURS
	WriteData(setx);
}

//--------------------------------------------//
//REG[45h]  
void Memory_read_LeftRight_TopDown(void)
{
	u8 temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp &= cClrD1D0;
	WriteData(temp);
}
void Memory_read_RightLeft_TopDown(void)
{
	u8 temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cClrD1SetD0;
	WriteData(temp);
}
void Memory_read_TopDown_LeftRight(void)
{
	u8 temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cSetD1ClrD0;
	WriteData(temp);
}
void Memory_read_DownTop_LeftRight(void)
{
	u8 temp;
	WriteCommand(0x45);//MRCD
	temp = ReadData();
	temp |= cSetD1D0;
	WriteData(temp);
}

//--------------------------------------------//
//REG[46h]~REG[49h]
void XY_Coordinate(u16 X,u16 Y)
{
	WriteCommand(0x46);
	WriteData(X);
	WriteCommand(0x47);
	WriteData(X>>8);
	
	WriteCommand(0x48);
	WriteData(Y);
	WriteCommand(0x49);
	WriteData(Y>>8);
}

//--------------------------------------------//
//REG[4Ah]~REG[4Dh]
void Memory_read_Coordinate(u16 X,u16 Y)
{
	WriteCommand(0x4a);
	WriteData(X);   
	WriteCommand(0x4b);	   
	WriteData(X>>8);
	
	WriteCommand(0x4c);
	WriteData(Y);
	WriteCommand(0x4d);	   
	WriteData(Y>>8);
} 
						  
//--------------------------------------------//
//REG[4Eh]
void Text_Cursor_Horizontal_Size(u8 setx)//u8[3:0]
{
	WriteCommand(0x4E);//CURS	   
	WriteData(setx);
} 
								
//REG[4Fh] 
void Text_Cursor_Vertical_Size(u8 setx)//u8[3:0]
{
	WriteCommand(0x4F);//CURS	   
	WriteData(setx);
} 	
	
//--------------------------------------------//
//REG[50h] 
void no_BTE_write(void)
{
	u8 temp;
	WriteCommand(0x50);//BECR0
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);  
}

void BTE_enable(void)
{
	u8 temp;
	WriteCommand(0x50);//BECR0
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);  
}

void BTE_contiguous_Data(void)
{
	u8 temp;
	WriteCommand(0x50);//BECR0
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);  
}

void BTE_rectangular_Data(void)
{
	u8 temp;												   
	WriteCommand(0x50);//BECR0
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}									  

//---------------------------------------------//
//REG[51h] 
void BTE_ROP_Code(u8 setx)
{	 
	WriteCommand(0x51);//BECR1	   
	WriteData(setx); 
}									   

//---------------------------------------------//
//REG[52h] 
void Layer1_Visible(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	WriteData(temp);  
}

void Layer2_Visible(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x01;
	WriteData(temp);  
}	
						
void Transparent_Mode(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x03;
	WriteData(temp);  
}

void Lighten_Overlay_Mode(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x02;
	WriteData(temp);  
}	
						
void Boolean_OR(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x04;
	WriteData(temp);
}

void Boolean_AND(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x05;
	WriteData(temp);  
}  	 
	
  
void Floating_window_mode(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0xf8;
	temp|=0x06;
	WriteData(temp);  
} 

void Floating_Window_transparent_with_BGTR_enable(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp|=cSetD5;
	WriteData(temp);  
}

void Floating_Window_transparent_with_BGTR_disable(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=cClrD5;
	WriteData(temp);  
}


void Layer1_2_scroll(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	WriteData(temp);  
}  	 

void Layer1_scroll(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	temp|=0x40;
	WriteData(temp);  
} 

void Layer2_scroll(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	temp|=0x80;
	WriteData(temp);  
} 

void Buffer_scroll(void)
{
	u8 temp;
	WriteCommand(0x52);//LTPR0
	temp = ReadData();
	temp&=0x3f;
	temp|=0xC0;
	WriteData(temp);  
} 

//---------------------------------------------//
//REG[53h] 
void layer2_1_transparency(u8 setx)
{  
	WriteCommand(0x53);//LTPR1
	WriteData(setx); 
}

//---------------------------------------------//
//REG[54h]~ [5Fh]
void BTE_Source_Destination	(u16 XL,u16 XR ,u16 YT ,u16 YB)
{
	u8 temp,temp1;
	
	temp=XL;   
	WriteCommand(0x54);//HSBE0
	WriteData(temp);
	temp=XL>>8;   
	WriteCommand(0x55);//HSBE1	   
	WriteData(temp);

	temp=XR;   
	WriteCommand(0x58);//HDBE0
	WriteData(temp);
	temp=XR>>8;   
	WriteCommand(0x59);//HDBE1	   
	WriteData(temp); 
    
	temp=YT;   
	WriteCommand(0x56);//VSBE0
	WriteData(temp);
	temp=YT>>8;   
	WriteCommand(0x57);//VSBE1
	temp1 = ReadData();
	temp1 &= 0x80;
	temp=temp|temp1; 
	WriteCommand(0x57);//VSBE1  
	WriteData(temp);


	temp=YB;   
	WriteCommand(0x5a);//VDBE0
	WriteData(temp);
	temp=YB>>8;   
	WriteCommand(0x5b);//VDBE1
	temp1 = ReadData();
	temp1 &= 0x80;
	temp=temp|temp1;	
	WriteCommand(0x5b);//VDBE1   
	WriteData(temp);
}							

void Source_Layer1(void)
{
	u8 temp;	
	WriteCommand(0x57);//VSBE1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}

void Source_Layer2(void)
{
	u8 temp;	
	WriteCommand(0x57);//VSBE1
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}

void Destination_Layer1(void)
{
	u8 temp;	
	WriteCommand(0x5b);//VDBE1
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
}

void Destination_Layer2(void)
{
	u8 temp;	
	WriteCommand(0x5b);//VDBE1
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
}
		  
//---------------------------------------------//
//---------------------------------------------//
void BTE_Size_setting(u16 X,u16 Y)
{	 
	WriteCommand(0x5c);//BEWR0
	WriteData(X); 
	WriteCommand(0x5d);//BEWR1	   
	WriteData(X>>8);
  
	WriteCommand(0x5e);//BEHR0
	WriteData(Y);   
	WriteCommand(0x5f);//BEHR1	   
	WriteData(Y>>8);
}									 

//---------------------------------------------//
//REG[60h]~REG[67h]

void Background_color(u16 color)
{
	WriteCommand(0x60);
	WriteData((color&0x001f));
		
	WriteCommand(0x61);
	WriteData((color&0x07e0)>>5);
	
	WriteCommand(0x62);
	WriteData((color&0xf800)>>11);
}

//--------------------------------------------//
void Text_Background_Color(u16 color)
{
	WriteCommand(0x60);//BGCR0
	WriteData((color&0x001f));

	WriteCommand(0x61);//BGCR0
	WriteData((color&0x07e0)>>5);
	
	WriteCommand(0x62);//BGCR0
	WriteData((color&0xf800)>>11);
	
	RA8875_WAITSTATUS();
} 

void Text_Foreground_Color(u16 color)
{	    
	WriteCommand(0x63);//BGCR0
	WriteData((color&0x001f));
	
	WriteCommand(0x64);//BGCR0
	WriteData((color&0x07e0)>>5);
	
	WriteCommand(0x65);//BGCR0
	WriteData((color&0xf800)>>11);
	RA8875_WAITSTATUS();
}


void BTE_Background_color(u8 setR,u8 setG,u8 setB)
{
	WriteCommand(0x60);//BGCR0
	WriteData(setB);
	WriteCommand(0x61);//BGCR0
	WriteData(setG);
	WriteCommand(0x62);//BGCR0
	WriteData(setR);
}


void BTE_Foreground_color(u8 setR,u8 setG,u8 setB)
{
	WriteCommand(0x63);//BGCR0
	WriteData(setB);
	WriteCommand(0x64);//BGCR0
	WriteData(setG);
	WriteCommand(0x65);//BGCR0
	WriteData(setR);
}


//======================================
//for test
void BTE_Background_red(u8 setx)
{	  
	WriteCommand(0x62);//BGCR0
	WriteData(setx);
}	
						
void BTE_Background_green(u8 setx)
{	  
	WriteCommand(0x61);//BGCR1
	WriteData(setx);
}							  

void BTE_Background_blue(u8 setx)
{	 
	WriteCommand(0x60);//BGCR2
	WriteData(setx);
} 								

void BTE_Foreground_red(u8 setx)
{	
	WriteCommand(0x65);//FGCR0
	WriteData(setx);
}								 

void BTE_Foreground_green(u8 setx)
{	  
	WriteCommand(0x64);//FGCR1
	WriteData(setx);
}							 

void BTE_Foreground_blue(u8 setx)
{  
	WriteCommand(0x63);//FGCR2
	WriteData(setx);
} 	

//======================================
	
void Pattern_Set_8x8(void)
{
	u8 temp;
	
	WriteCommand(0x66);//PTNO
	temp = ReadData();
	temp &= cClrD7;
	WriteData(temp);
}	

void Pattern_Set_16x16(void)
{
	u8  temp;

	WriteCommand(0x66);//PTNO
	temp = ReadData();
	temp |= cSetD7; 
	WriteData(temp);
}								


void Pattern_Set_number(u8 setx)
{
	u8 temp;
	
	WriteCommand(0x66);//PTNO
	temp = ReadData();
	temp &= 0x80;    
	WriteData(temp|setx);
} 	
							
void BackgroundColor_TransparentMode(u8 setR,u8 setG,u8 setB)
{
	WriteCommand(0x67);//BGTR_R
	WriteData(setR);

	WriteCommand(0x68);//BGTR_G
	WriteData(setG);

	WriteCommand(0x69);//BGTR_B
	WriteData(setB);
}	  

//REG[70h]~REG[74h]
//==============================================================================
//Subroutine:	Enable_TP 
//==============================================================================
void Enable_TP(void)
{
	u8 temp;	  
	WriteCommand(0x70);//TPCR0	
	temp = ReadData();
	temp |= cSetD7;   
	WriteData(temp);
	RA8875_WAITSTATUS();
}
//==============================================================================
//Subroutine:	Disable_TP 
//==============================================================================
void Disable_TP(void)
{
	u8 temp;
	WriteCommand(0x70);//TPCR0
	temp = ReadData();
	temp &= cClrD7;   
	WriteData(temp);
}

//==============================================================================
//Subroutine:	Enable_Touch_WakeUp
//==============================================================================
void Enable_Touch_WakeUp(void)
{
	u8 temp;
	WriteCommand(0x70);//TPCR0	
	temp = ReadData();
	temp |= cSetD3;   //cSetb3 = 00001000
	WriteData(temp);
}
//==============================================================================
//Subroutine:	Disable_Touch_WakeUp
//==============================================================================
void Disable_Touch_WakeUp(void)
{
	u8 temp;
	WriteCommand(0x70);//TPCR0	
	temp = ReadData();
	temp &= cClrD3;
	WriteData(temp);
}

//==============================================================================
//Subroutine:	TP manual or auto  mode enable
//==============================================================================
void TP_manual_mode(void)
{
	u8 temp;
  	WriteCommand(0x71);//TPCR1	
	temp = ReadData();
	temp |= cSetD6;
	WriteData(temp);
}
void TP_auto_mode(void)
{
	u8 temp;
  	WriteCommand(0x71);//TPCR1	
	temp = ReadData();
	temp &= cClrD6;//temp |= 0x3F;
	temp |= 0x3F;
	WriteData(temp);
	RA8875_WAITSTATUS();
}
//==============================================================================
//Subroutine:	TP_IDLE_mode
//==============================================================================
void TP_IDLE_mode(void)
{
	u8 temp;
	WriteCommand(0x71);//TPCR1
	temp=ReadData(); 
	temp &= 0xf0;
	WriteData(temp);
}
//==============================================================================
//Subroutine:	TP_event_mode
//==============================================================================
void TP_event_mode(void)
{
	u8 temp;
	WriteCommand(0x71);//TPCR1
	temp=ReadData(); 
	temp &= 0xf0;
	temp |= 0x01;
	WriteData(temp);
	//  WriteData(0xc1);
}
//==============================================================================
//Subroutine:	TP_latch_X
//==============================================================================
void TP_latch_X(void)
{
	u8 temp;
	WriteCommand(0x71);//TPCR1
	temp=ReadData(); 
	temp &= 0xc0;
	temp |= 0x02;
	WriteData(temp);
 }
 //==============================================================================
//Subroutine:	TP_latch_Y
//==============================================================================
void TP_latch_Y(void)
{
	u8 temp;
	WriteCommand(0x71);//TPCR1
	temp=ReadData(); 
	temp &= 0xc0;
	temp |= 0x03;
	WriteData(temp);
}
//==============================================================================
//Subroutine:	Detect_TP_X_Y coordinate 
//==============================================================================
u8 ADC_X(void)
{
	u8 temp;
	RA8875_WAITSTATUS();
	WriteCommand(0x72);//TPXH	 X_coordinate high byte
	RA8875_WAITSTATUS();
	temp=ReadData();
	return temp;
}

u8 ADC_Y(void)
{
	u8 temp;
	RA8875_WAITSTATUS();
	WriteCommand(0x73);//TPYH	  Y_coordinate high byte
	RA8875_WAITSTATUS();
	temp=ReadData();
	return temp;
}

u8 ADC_XY(void)
{
	u8 temp;
	RA8875_WAITSTATUS();
 	WriteCommand(0x74);//TPXYL	  u8[3:2] Y_coordinate low byte  u8[1:0] X_coordinate low byte 
	temp=ReadData();
	return temp;
}   



//REG[80h]~REG[83h]
void Graphic_Cursor_Coordinate(u16 X,u16 Y)
{
	WriteCommand(0x80);
	WriteData(X);  
	WriteCommand(0x81);	   
	WriteData(X>>8);
  
	WriteCommand(0x82);
	WriteData(Y);  
	WriteCommand(0x83);	   
	WriteData(Y>>8);
}

//---------------------------------//
//REG[84h]

void Graphic_Cursor_Color_0(u8 setx)
{	  
	WriteCommand(0x84);//GCC0
	WriteData(setx); 
}  									  

void Graphic_Cursor_Color_1(u8 setx)
{	 
	WriteCommand(0x85);//GCC1
	WriteData(setx); 
}  	
 	 						  
//---------------------------------------------//
//REG[8A]
void PWM1_enable(void)
{
	u8 temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
 }

void PWM1_disable(void)
{
	u8 temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);  
 }

void PWM1_disable_level_low(void)
{
	u8 temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}

void PWM1_disable_level_high(void)
{
	u8 temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);  
}

 void PWM1_fnuction_sel(void)
{
	u8 temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= cClrD4 ;
	WriteData(temp);
	RA8875_WAITSTATUS();  
 }

 void PWM1_system_clk_out(void)
{
	u8 temp;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp |= cSetD4 ;
	WriteData(temp);  
 }

 void PWM1_clock_ratio(u8 setx) //u80~3 
{
	u8 temp,temp1;
	temp1= setx&0x0f;
	WriteCommand(0x8a);//MCLR
	temp = ReadData();
	temp &= 0xf0;
	temp |= temp1 ;
	WriteData(temp);
	RA8875_WAITSTATUS(); 
 }

 void PWM1_duty_cycle(u8 setx) //u80~7
{
	WriteCommand(0x8b);//PTNO
	WriteData(setx);
	RA8875_WAITSTATUS();
}

void PWM2_enable(void)
{
	u8 temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp);  
 }

void PWM2_disable(void)
{
	u8 temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);  
 }

void PWM2_disable_level_low(void)
{
	u8 temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}

void PWM2_disable_level_high(void)
{
	u8 temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);  
}

 void PWM2_fnuction_sel(void)
{
	u8 temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= cClrD4 ;
	WriteData(temp);  
 }

  void PWM2_system_clk_out(void)
{
	u8 temp;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp |= cSetD4 ;
	WriteData(temp);  
 }

  void PWM2_clock_ratio(u8 setx) //u80~3 
{
	u8 temp,temp1;
	temp1= setx&0x0f;
	WriteCommand(0x8c);//MCLR
	temp = ReadData();
	temp &= 0xf0;
	temp |= temp1 ;
	WriteData(temp);  
 }
 void PWM2_duty_cycle(u8 setx) //u80~7
{  
    WriteCommand(0x8d);//PTNO
	WriteData(setx);
} 	


//---------------------------------------------//
//REG[8Eh]
void Stop_Memory_Clear(void)
{
	u8 temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp &= cClrD7 ;
	WriteData(temp);
	RA8875_WAITSTATUS(); 
}						 
void Memory_Clear(void)
{
	u8 temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp |= cSetD7 ;
	WriteData(temp); 
	RA8875_WAITSTATUS();
}
void Clear_Full_Window(void)
{
	u8 temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp &= cClrD6 ;
	WriteData(temp);  
}
void Clear_Active_Window(void)
{
	u8 temp;
	WriteCommand(0x8e);//MCLR
	temp = ReadData();
	temp |= cSetD6 ;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

									
//---------------------------------------------//
//REG[90h] 
void Draw_line(void)
{ 
	WriteCommand(0x90);//DCR
	WriteData(0x00);
	WriteData(0x80);
}

void Draw_square(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x10);
	WriteData(0x90);
}

void Draw_square_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x10);
	WriteData(0xb0);
}

void Draw_circle(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x40);
}

void Draw_circle_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x60);
}

void Draw_Triangle(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x00);
	WriteData(0x81);
}

void Draw_Triangle_fill(void)
{
	WriteCommand(0x90);//DCR
	WriteData(0x01);
	WriteData(0xA1);
}

//---------------------------------------------//
//REG[90h]~/EG[9Dh]  									
void Geometric_Coordinate(u16 XL,u16 XR ,u16 YT ,u16 YB)
{	  
	WriteCommand(0x91);//Hs
	WriteData(XL);   
	WriteCommand(0x92);	   
	WriteData(XL>>8);
	
	WriteCommand(0x95);//Vs
	WriteData(XR);  
	WriteCommand(0x96);	   
	WriteData(XR>>8);
	
	WriteCommand(0x93);
	WriteData(YT);  
	WriteCommand(0x94);	   
	WriteData(YT>>8);
	
	WriteCommand(0x97);
	WriteData(YB);   
	WriteCommand(0x98);	   
	WriteData(YB>>8);
	RA8875_WAITSTATUS();
}

void Circle_Coordinate_Radius(u16 X,u16 Y,u16 R)
{ 
	WriteCommand(0x99);
	WriteData(X);  
	WriteCommand(0x9a);	   
	WriteData(X>>8);  
	
	WriteCommand(0x9b);
	WriteData(Y); 
	WriteCommand(0x9c);	   
	WriteData(Y>>8);
	
	WriteCommand(0x9d);
	WriteData(R);
	RA8875_WAITSTATUS();
} 

//---------------------------------------------//
//REG[A0h] 
void Draw_Ellipse(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x00);
	WriteData(0x80);
	RA8875_WAITSTATUS();
}

void Draw_Ellipse_Curve(void)
{
	u8 temp;

	WriteCommand(0xA0);//DCR
	temp = ReadData();
	temp &=0x1F;
	temp |= 0x90;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

void Draw_Ellipse_Curve_Fill(void)
{
	u8 temp;
	
	WriteCommand(0xA0);//DCR
	temp = ReadData();
	temp &=0x1F;
	temp |= 0xD0;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

void Draw_Ellipse_fill(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x40);
	WriteData(0xC0);
}

void Draw_Circle_Square(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x20);
	WriteData(0xA0);
}

void Draw_Circle_Square_fill(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x60);
	WriteData(0xE0);
}


void Draw_Ellipse_Curve_part(u8 setx)
{
	u8 temp,temp1;
	WriteCommand(0xA0);//DCR
	temp = ReadData();
	temp1 = setx&0x03;
	temp &=0xfc;
	temp |=temp1;
	WriteData(temp);   
}


void Ellipse_Coordinate_setting(u16 X,u16 Y,u16 ELL_A,u16 ELL_B)
{

	WriteCommand(0xA5);
	WriteData(X);
	WriteCommand(0xA6);	   
	WriteData(X>>8);  
	   
	WriteCommand(0xA7);
	WriteData(Y); 
	WriteCommand(0xA8);	   
	WriteData(Y>>8);
	
	WriteCommand(0xA1);
	WriteData(ELL_A);
	WriteCommand(0xA2);
	WriteData(ELL_A>>8);
	
	WriteCommand(0xA3);
	WriteData(ELL_B);
	WriteCommand(0xA4);
	WriteData(ELL_B>>8);
} 



void Circle_Square_Coordinate_setting(u16 XL,u16 XR ,u16 YT ,u16 YB ,u16 ELL_A,u16 ELL_B)
{	    
  
	WriteCommand(0x91);
	WriteData(XL);  
	WriteCommand(0x92);	   
	WriteData(XL>>8);
	
	WriteCommand(0x95);
	WriteData(XR);   
	WriteCommand(0x96);	   
	WriteData(XR>>8);
	     
	WriteCommand(0x93);
	WriteData(YT);  
	WriteCommand(0x94);	   
	WriteData(YT>>8);
	
	WriteCommand(0x97);
	WriteData(YB);  
	WriteCommand(0x98);	   
	WriteData(YB>>8);
	
	WriteCommand(0xA1);
	WriteData(ELL_A);
	WriteCommand(0xA2);
	WriteData(ELL_A>>8);
	
	WriteCommand(0xA3);
	WriteData(ELL_B);
	WriteCommand(0xA4);
	WriteData(ELL_B>>8); 
}

 
void Draw_Triangle_3point_Coordinate(u16 P1_X,u16 P1_Y,u16 P2_X,u16 P2_Y,u16 P3_X,u16 P3_Y)
{
	WriteCommand(0x91);
	WriteData(P1_X);
	WriteCommand(0x92);	   
	WriteData(P1_X>>8);
	
	WriteCommand(0x93);
	WriteData(P1_Y);
	WriteCommand(0x94);	   
	WriteData(P1_Y>>8);
	
	WriteCommand(0x95);
	WriteData(P2_X);
	WriteCommand(0x96);	   
	WriteData(P2_X>>8);
	
	WriteCommand(0x97);
	WriteData(P2_Y);
	WriteCommand(0x98);	   
	WriteData(P2_Y>>8);
	
	WriteCommand(0xA9);
	WriteData(P3_X);
	WriteCommand(0xAA);	   
	WriteData(P3_X>>8);
	
	WriteCommand(0xAB);
	WriteData(P3_Y);
	WriteCommand(0xAC);	   
	WriteData(P3_Y>>8);   
}

//---------------------------------------------//
//REG[B0]~REG[B3]
void DMA_Start_address_setting(u32 set_address)
{ 
	WriteCommand(0xB0);
	WriteData(set_address);
	
	WriteCommand(0xB1);
	WriteData(set_address>>8);
	
	WriteCommand(0xB2);
	WriteData(set_address>>16);
	
	WriteCommand(0xB3);
	WriteData(set_address>>24);
}

//input pic data quantity  for example  pic size = 800*480 = 384000
void DMA_Continuous_mode_size_setting(u32 set_size)
{ 
	WriteCommand(0xB4);
	WriteData(set_size);
	
	WriteCommand(0xB6);
	WriteData(set_size>>8);
	
	WriteCommand(0xB8);
	WriteData(set_size>>16);
}

void DMA_block_mode_size_setting(u16 BWR,u16 BHR,u16 SPWR)
{
	WriteCommand(0xB4);
	WriteData(BWR);
	WriteCommand(0xB5);
	WriteData(BWR>>8);
	
	WriteCommand(0xB6);
	WriteData(BHR);
	WriteCommand(0xB7);
	WriteData(BHR>>8);
	
	WriteCommand(0xB8);
	WriteData(SPWR);
	WriteCommand(0xB9);
	WriteData(SPWR>>8);  
}

//---------------------------------------------//
//REG[BFh]
void DMA_Continuous_mode(void)
{ 
	WriteCommand(0xBF);
	WriteData(0x00);
}

void DMA_Block_mode(void)
{ 
	WriteCommand(0xBF);
	WriteData(0x02);
}

void DMA_Start_enable(void)
{
	u8 temp;
	WriteCommand(0xBF);
	temp = ReadData();
	temp |= 0x01;
	WriteData(temp);
}

//---------------------------------------------//
//REG[C0h]
void Key_Scan_Enable(void)
{
	u8 temp;
	WriteCommand(0xC0);
	temp = ReadData();
	temp |= 0x80;
	WriteData(temp);  
}


void Key_Scan_setting(u8 setx)//u8[2:0]
{
	u8 temp;
	WriteCommand(0xC0);
	temp = ReadData();
	temp &= 0xf0;
	temp|= (setx&0x07);
	WriteData(temp);  
} 

void Long_Key_Scan_enable(void)
{
	u8 temp;
	WriteCommand(0xC0);
	temp = ReadData();
	temp |= 0x40;
	WriteData(temp);
}

void Key_Scan_Wakeup_Function_Enable(void)
{
	u8 temp;
	WriteCommand(0xC1);	
	temp = ReadData();
	temp |= 0x80;	// 
	WriteData(temp);
}

void Long_Key_Timing_Adjustment(u8 setx)
{
	u8 temp,temp1;
	temp = setx & 0x1c;
	WriteCommand(0xC1);
	temp1 = ReadData();
	temp1|=temp;
	WriteData(temp1);
}

//---------------------------------------------//
//REG[D0]
void Floating_window_start_point(u16 X,u16 Y)
{ 
	WriteCommand(0xD0);
	WriteData(X);
	WriteCommand(0xD1);
	WriteData(X>>8);
	
	WriteCommand(0xD2);
	WriteData(Y);
	WriteCommand(0xD3);
	WriteData(Y>>8);  
}


void Floating_window_size(u16 width_X,u16 high_Y)
{
	WriteCommand(0xD4);
	WriteData(width_X);
	WriteCommand(0xD5);
	WriteData(width_X>>8);
	
	WriteCommand(0xD6);
	WriteData(high_Y);
	WriteCommand(0xD7);
	WriteData(high_Y>>8);
}

void Floating_window_display_point(u16 X,u16 Y)
{
	WriteCommand(0xD8);
	WriteData(X);
	WriteCommand(0xD9);
	WriteData(X>>8);
	
	WriteCommand(0xDA);
	WriteData(Y);
	WriteCommand(0xDB);
	WriteData(Y>>8);
}


//REG[E0h] 
void Serial_ROM_direct_access_mode_enable(void)
{
	WriteCommand(0xE0);
	WriteData(0x01);
}

void Serial_ROM_direct_access_mode_disable(void)
{
	WriteCommand(0xE0);
	WriteData(0x00);
}

void Direct_access_mode_Address_32u8s(u32 set_address)
{
	u8 temp;  
	temp = set_address>>24;
	WriteCommand(0xE1);
	WriteData(temp);
	temp = set_address>>16;
	WriteData(temp);
	temp = set_address>>8;
	WriteData(temp);
	temp = set_address;
	WriteData(temp);
}

void Direct_access_mode_Address_24u8s(u32 set_address)
{
	u8 temp=0;
	
	temp = set_address>>16;
	WriteCommand(0xE1);
	WriteData(temp);  
	temp = set_address>>8;
	WriteCommand(0xE1);
	WriteData(temp);   
	temp = set_address;
	WriteCommand(0xE1);
	WriteData(temp);
}

u8 Direct_access_mode_Read_Data_buffer(void)
{
	u8 Serial_Data;
	
	WriteCommand(0xE2);
	Serial_Data=ReadData(); 
	return Serial_Data;
}

//---------------------------------------------//



//---------------------------------------------//
//REG[F0h] 

void KEYSCAN_Interrupt_Enable(void)
{
	u8 temp;
	WriteCommand(0xF0);	
	temp = ReadData();
	temp |= 0x10;	// 
	WriteData(temp); 
}

void KEYSCAN_Interrupt_Disable(void)
{
	u8 temp;
	WriteCommand(0xF0);	
	temp = ReadData();
	temp &= 0x0F;	// 
	WriteData(temp); 
}


void DMA_Interrupt_Enable(void)
{
	u8 temp;
	WriteCommand(0xF0);	
	temp = ReadData();
	temp |= 0x08;	// 
	WriteData(temp); 
}

void DMA_Interrupt_Disable(void)
{
	u8 temp;
	WriteCommand(0xF0);	
	temp = ReadData();
	temp &= 0x17;	// 
	WriteData(temp); 
}

void Touch_Panel_Interrupt_Enable(void)
{
	u8 temp;
	WriteCommand(0xF0);//INTC
	temp = ReadData();
	temp |= 0x04;
	WriteData(temp);
	RA8875_WAITSTATUS();
}

void Touch_Panel_Interrupt_Disable(void)
{
	u8 temp;
	WriteCommand(0xF0);//INTC
	temp = ReadData(); 
	temp &= 0x1B;
	WriteData(temp);
}

void BTE_Interrupt_Enable(void)
{
	u8 temp;
	WriteCommand(0xF0);//INTC
	temp=ReadData(); 
	temp |= 0x02;
	WriteData(temp);
}

void BTE_Interrupt_Disable(void)
{
	u8 temp;
	WriteCommand(0xF0);//INTC
	temp = ReadData(); 
	temp &= 0x1D;
	WriteData(temp);
}

void Font_BTE_read_write_Interrupt_Enable(void)
{
	u8 temp;
	WriteCommand(0xF0);//INTC
	temp = ReadData(); 
	temp |= 0x01;
	WriteData(temp);
}

void Font_BTE_read_write_Interrupt_Disable(void)
{
	u8 temp;
	WriteCommand(0xF0);//INTC
	temp = ReadData(); 
	temp &= 0x1E;
	WriteData(temp);
}

//---------------------------------------------//
//REG[F1h] 
void Clear_keyscan_interrupt(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC
	temp = ReadData(); 
	temp |=cSetD4 ;
	WriteData(temp);
}

u8 Keyscan_Status(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC	
	temp = ReadData();
	if ((temp&0x10)==0x10)
	return 1;
	else 
	return 0;
}

void Clear_DMA_interrupt(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC
	temp = ReadData(); 
	temp |=cSetD3 ;
	WriteData(temp);
}

u8 DMA_Status(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC	
	temp = ReadData();
	if ((temp&0x08)==0x08)
	return 1;
	else 
	return 0;
}

void clear_TP_interrupt(void)
{
	u8 temp;
	RA8875_WAITSTATUS();
	WriteCommand(0xF1);//INTC
	temp=ReadData(); 
	temp |= cSetD2;
	WriteData(temp);
	RA8875_WAITSTATUS();
 }


u8 Touch_Status(void)
{
	u8 temp;
	RA8875_WAITSTATUS();
	WriteCommand(0xF1);//INTC	
	temp = ReadData();
	if((temp&0x04) == 0x04)
		return 1;
	else
		return 0;
}

void Clear_BTE_interrupt(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC
	temp=ReadData(); 
	temp |= cSetD1;
	WriteData(temp);
}

u8 BTE_Status(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC	
	temp = ReadData();
	if ((temp&0x02)==0x02)
		return 1;
	else 
		return 0;
}

void Clear_Font_BTE_read_write_Interrupt(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC
	temp=ReadData(); 
	temp |= cSetD0;
	WriteData(temp);
}

u8 Font_BTE_read_write_Status(void)
{
	u8 temp;
	WriteCommand(0xF1);//INTC	
	temp = ReadData();
	if ((temp&0x01)==0x01)
		return 1;
	else 
		return 0;
}



//void Chk_Wait(void)
//{
	
// while(MCU_WAIT==0);	   
//}


void Show_String(u8 *str,u8 n)
{
	Text_Mode();
	WriteCommand(0x02);
	while(*str != '\0')
	{
		WriteData(*str);
		++str;
		Chk_Busy();
		//Chk_Wait();
		DelayMs(1);
	}
	DelayMs(n);
}
//显示一个字母
void Show_WD(char *str,u8 n)
{
	RA8875_WAITSTATUS();
	Text_Mode();
	WriteCommand(0x02);
	//while(*str != '\0')
	//{
		WriteData(*str);
		//++str;
		//Chk_Busy();
		//Chk_Wait();
		//DelayMs(1);
	//}
	DelayMs(n);
}
void LCD_Reset(void)
{
	GPIOB->BRR  = GPIO_PIN_10; //RA8875 RESET pin
	DelayMs(110);
	GPIOB->BSRR  = GPIO_PIN_10;
	DelayMs(110);
}

//void Waitus(u16 time)
//{
//	TIM4->CNT = 0;			//计数器清零
//	while(TIM4->CNT < time);	//等待
//}

u16 Get_X_ADC_Value_10bit(void)
{
	u16 temp,temp1;
	RA8875_WAITSTATUS();
	
	temp=0;
	temp|=ADC_X();
	temp1=(temp<<2);
	temp= temp1|((ADC_XY()&0x03));
	
	return temp;
}


u16 Get_Y_ADC_Value_10bit(void)
{
	u16 temp,temp1;
	RA8875_WAITSTATUS();
	
	temp=0;
	temp|=ADC_Y();
	temp1=(temp<<2);
	temp=temp1|((ADC_XY()>>2)&0x03);
	
	return temp;
}
void delaysometime(void)
{
	u16 i;
	for(i=0;i<65530;i++);
}

