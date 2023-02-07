//
// Created by weilifox on 2022/6/22.
//

#ifndef __CWL_BMP_FORMAT_H__
#define __CWL_BMP_FORMAT_H__

#include <stdint.h>

/*
typedef enum {
    CWL_BMP_OK = 0,
    CWL_BMP_ERR
} CWL_BmpStatus;
*/

typedef struct {            // 14 bytes header
    uint16_t bf_type;       // should be 0x4d42 "MB"
    uint32_t bf_size;
    uint16_t bf_reserved1;
    uint16_t bf_reserved2;
    uint32_t bf_off_bits;
} bitmapFileHeader;

typedef struct {
    uint32_t bi_size;       // header size = 40
    uint32_t bi_width;
    uint32_t bi_height;
    uint16_t bi_planes;
    uint16_t bi_bit_count;	// 8 and 16 is supported
    uint32_t bi_compression;
    uint32_t bi_size_image;
    uint32_t bi_x_pix_per_peter;
    uint32_t bi_y_pix_per_meter;
    uint32_t bi_clr_used;
    uint32_t bi_clr_important;
} bitmapInfoHeader;

/*
typedef struct {            // not in true colour
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
} bitmapRgbQuad;
*/

#endif //__CWL_BMP_FORMAT_H__
