//
// Created by weilifox on 2022/6/26.
//

#ifndef __IMAGES_JPG_H__
#define __IMAGES_JPG_H__

#include "fatfs.h"
#include "tjpgd.h"

typedef struct jpgf_t jpgf_t;
struct jpgf_t {
	FIL *fil;						// file pointer
	uint32_t ofx, ofy;	// lcd show offset
};

uint8_t cwl_lcd_jpg(FIL *fp);

static size_t in_jpg_func(JDEC *jd, BYTE *buf, UINT nbyte);
static int out_jpg_func(JDEC *jd, void *bitmap, JRECT *rect);

#endif
