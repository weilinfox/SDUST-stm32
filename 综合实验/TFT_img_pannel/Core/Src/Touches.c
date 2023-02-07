#include "Touches.h"

uint8_t cwl_touch_init(void)
{
	return CWL_TP_Init();
}

point2 cwl_touch_point(void)
{
	point2 p;

	GSL2681_IRQ_Callback();
	
	p.x = PENX; p.y = PENY;
	return p;
}
