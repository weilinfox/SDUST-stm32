//
// Created by weilifox on 2022/6/27.
//

#ifndef __TOUCHES_H__
#define __TOUCHES_H__

#include "TouchPanel.h"

#include "Images.h"

extern volatile uint16_t PENX, PENY;

typedef struct point2 point2;
struct point2 {
	uint16_t x;
	uint16_t y;
};

uint8_t cwl_touch_init(void);
point2 cwl_touch_point(void);

#endif
