//
// Created by weilifox on 2022/6/23.
//

#ifndef __IMAGES_H__
#define __IMAGES_H__

#include "fatfs.h"
#include "cwl_bmp_format.h"

#include "TFTLCD.h"
#include "Graphics.h"

// have factor of 1, 2, 3
// to support 8bits 16bits and 24bits
#define IMG_BUF_SIZE 			6144U

void cwl_lcd_init(void);
void cwl_lcd_clear(void);
void cwl_lcd_white(void);
void cwl_print_bmp_r(u16* image, u16 x, u16 y, u16 width, u16 height);
void cwl_lcd_log(char msg[], int32_t num);
uint8_t cwl_lcd_bmp(FIL *fp);

#endif
