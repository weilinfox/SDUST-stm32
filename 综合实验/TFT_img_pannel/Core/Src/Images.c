
#pragma pack(1)

#include <stdio.h>

#include "Images.h"

unsigned char img_data[IMG_BUF_SIZE];

void cwl_lcd_init(void)
{
	HAL_CWL_TFTLCD_Init();
}

// clear lcd with black background
void cwl_lcd_clear(void)
{
	LcdClear(BLACK);
}

// clear lcd with white background
void cwl_lcd_white(void)
{
	LcdClear(WHITE);
}

// log on left top of the lcd
void cwl_lcd_log(char msg[], int32_t num)
{
	char str[64];
	// 16 * 8 per charactor
	RA8875_RTERect(2, 2, 2+64*8, 2+16, BLACK);
	// LcdFillRec(2, 2, 2+50*9, 2+12, 0, BLACK);
	if (num) {
		snprintf(str, 64, "%s: %d", msg, num);
		LcdPrintStr(str, 2, 2, WHITE, BLACK, 0);
	} else {
		LcdPrintStr(msg, 2, 2, WHITE, BLACK, 0);
	}
}

void cwl_print_bmp_r(u16* image, u16 x, u16 y, u16 width, u16 height)
{
	u16 w,h;
	
	for(h=0;h<height;h++)
	{
		XY_Coordinate(x,y+h);
		WriteCommand(0x02);		//MRWC  REG[02h] -- Memory Read/Write Command
		for(w = 0; w< width; w++)
		{
			WriteData(~*image++);
		}
	}
}

// show bmp file
uint8_t cwl_lcd_bmp(FIL *fp)
{
	FRESULT fresult;
	UINT br;
	bitmapFileHeader bmp_header;
	bitmapInfoHeader bmp_info;
	
	// read file header
	fresult = f_read(fp, &bmp_header, sizeof(bitmapFileHeader), &br);
	if (fresult == FR_OK) fresult = f_read(fp, &bmp_info, sizeof(bitmapInfoHeader), &br);
	if (fresult == FR_OK) {
		// check bmp image
		uint32_t linebytes = (bmp_info.bi_width * bmp_info.bi_bit_count) >> 3;
		if (linebytes % 4U) linebytes += 4U - (linebytes % 4U);
		if (bmp_header.bf_type != 0x4D42 || bmp_info.bi_compression || \
			(bmp_info.bi_bit_count!=8 && bmp_info.bi_bit_count!=16 && bmp_info.bi_bit_count != 24)) {
			// support 8bits 16bits 24bits bmp file
			cwl_lcd_log("Unsupported bmp file.", fresult);
		} else if (bmp_info.bi_size_image != bmp_info.bi_height*linebytes) {
			// file size and image size
			cwl_lcd_log("Unmatched bmp file size.", fresult);
		} else if (bmp_info.bi_width > 800U || bmp_info.bi_height > 480U) {
			// height and weight
			cwl_lcd_log("Image size should not larger than 800x480", 0);
		} else {
			// center align
			uint32_t sx = (800U - bmp_info.bi_width) >> 1, sy = (480U - bmp_info.bi_height) >> 1;
			uint32_t sh, sw;
			UINT pos;
			u16 pix16;
			// read rest data and show image
			fresult = f_lseek(fp, bmp_header.bf_off_bits);
			// turn off light
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
			for (sh = bmp_info.bi_height-1; fresult == FR_OK && sh != 0; sh--) { // see LcdPrint16bitBmp
				XY_Coordinate(sx, sy+sh);
				WriteCommand(0x02);
				// read one line
				HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
				fresult = f_read(fp, img_data, linebytes, &br);
				HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
				for(sw = pos = 0; fresult == FR_OK && sw < bmp_info.bi_width; sw++) {
					switch (bmp_info.bi_bit_count) {
						case 8:
							pix16 = img_data[pos];	// bgr233
							pix16 = ((pix16 << 8) & 0xC000) | ((pix16 << 5) & 0x700) | ((pix16 << 2) & 0x1C);
							WriteData(pix16);
							pos++;
							break;
						
						case 16:
							WriteData(*(u16*)(&img_data[pos]));	// bgr565
							pos += 2;
							break;
						
						case 24:
							pix16 = img_data[pos+2] >> 3;
							pix16 |= ((u16)img_data[pos+1] << 3) & 0x7E0;
							pix16 |= ((u16)img_data[pos] << 8) & 0xF800;
							WriteData(pix16);
							pos += 3;
							break;
					}
				}
			}

			// read error
			if (fresult != FR_OK) {
				cwl_lcd_log("File read error", fresult);
			} else { // success
				//HAL_Delay(2000);
				return 0;
			}
		}
	}
	HAL_Delay(500);
	return 1; // failed
}

#pragma pack()
