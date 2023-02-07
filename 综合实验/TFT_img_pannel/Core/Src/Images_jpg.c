
#include <stdlib.h>
#include <math.h>

#include "Images_jpg.h"
#include "Images.h"

extern unsigned char img_data[IMG_BUF_SIZE];

// show jpg file
// do not forget to set JD_FORMAT	to 1 in tjpgdcnf.h
uint8_t cwl_lcd_jpg(FIL *fp)
{
	jpgf_t *jpgf;
	JDEC jdec;
	JRESULT res;
	BYTE scale = 0;
	
	jpgf = (jpgf_t*)malloc(sizeof(jpgf_t));
	if (!jpgf) {
		cwl_lcd_log("Parseing jpg malloc failed", 0);
		HAL_Delay(500);
		return 1U;
	} /*else {
		cwl_lcd_log("Parseing jpg malloc get", (uint32_t)jpgf);
		HAL_Delay(1000);
	}*/
	jpgf->fil = fp; jpgf->ofx = jpgf->ofy = 0;
	// turn off light
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	res = jd_prepare(&jdec, in_jpg_func, img_data, IMG_BUF_SIZE, jpgf);
	if (res == JDR_OK) {
		if (jdec.width <= 800U && jdec.height <= 480U) {
			jpgf->ofx = (800U - jdec.width) >> 1; jpgf->ofy = (480U - jdec.height) >> 1;
		} else {	// calculate scale
			uint32_t sc, sh;
			sc = (uint32_t)ceil((double)jdec.width / 800.0);
			sh = (uint32_t)ceil((double)jdec.height / 480.0);
			sc = sh > sc ? sh : sc;
			if (sc > 8) {
				cwl_lcd_log("Image size so large", 0);
				HAL_Delay(500);
			} else if (sc > 4) {
				scale = 3;
			} else if (sc > 2) {
				scale = 2;
			} else {
				scale = 1;
			}
			jpgf->ofx = (800U - (jdec.width >> scale)) >> 1;
			jpgf->ofy = (480U - (jdec.height >> scale)) >> 1;
		}
		
		// decode jpg
		res = jd_decomp(&jdec, out_jpg_func, scale);
		if (res != JDR_OK) {
			cwl_lcd_log("Decompress jpg image failed" , res);
		} else { // success
			free(jpgf);
			return 0U;
		}
	} else if (res == JDR_FMT1) {
		cwl_lcd_log("JPG data format error", res);
	} else if (res == JDR_FMT2) {
		cwl_lcd_log("Unsupported jpg format", res);
	} else if (res == JDR_FMT3) {
		cwl_lcd_log("Unsupported JPEG standard", res);
	} else {
		cwl_lcd_log("Prepare jpg image failed" , res);
	}
	
	free(jpgf);
	HAL_Delay(500);
	return 1U;
}

size_t in_jpg_func(JDEC *jd, BYTE *buf, UINT nbyte)
{
	jpgf_t *jpgf = NULL;
	FIL *fp = NULL;
	//FIL *fp = (FIL*) jd->device;
	FRESULT fr;
	UINT br;
	
	
	jpgf = (jpgf_t*)(jd->device);
	/*if (!jpgf) {
		cwl_lcd_log("in_jpg_func get NULL jpgf_t ptr", 0);
		HAL_Delay(1000);
		return 0;
	} else {
		cwl_lcd_log("in_jpg_func get jpgf_t ptr", (uint32_t)jd->device);
		HAL_Delay(1000);
	}*/
	fp = jpgf->fil;
	if (!fp) {
		cwl_lcd_log("in_jpg_func get NULL FIL ptr", 0);
		HAL_Delay(1000);
		return 0;
	}
	if (buf) {
		//cwl_lcd_log("cwl_in_jpg_func_ptr", (int32_t)fp);
		//HAL_Delay(500);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		fr = f_read(fp, buf, nbyte, &br);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		if (fr != FR_OK) {
			cwl_lcd_log("Read jpg file error", fr);
			HAL_Delay(500);
		}
		return br;
	} else {
		return f_lseek(fp, f_tell(fp) + nbyte) != FR_OK ? 0 : nbyte;
	}
}

int out_jpg_func(JDEC *jd, void *bitmap, JRECT *rect)
{
	jpgf_t *jpgf = (jpgf_t*)jd->device;
	uint32_t sx = jpgf->ofx + rect->left, sy = jpgf->ofy + rect->top, sh, sw;
	//uint32_t sx = rect->top, sy = rect->left, sh, sw;
	uint32_t ph = rect->bottom - rect->top + 1, pw = rect->right - rect->left + 1;
	uint32_t pos = 0;
	u16 *bit_data = (u16*) bitmap, tmp;	// 16bits RGB565

	//cwl_lcd_log("top", rect->top);
	//cwl_lcd_log("bottom", rect->bottom);
	//cwl_lcd_log("right", rect->right);
	//cwl_lcd_log("left", rect->left);
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	for (sh = 0; sh < ph; sh++) {
		XY_Coordinate(sx, sy+sh);
		WriteCommand(0x02);
		for (sw = 0; sw < pw; sw++) {
			tmp = bit_data[pos++];
			tmp = (tmp & 0x7E0) | (tmp >> 11) | (tmp << 11);
			WriteData(tmp);
		}
	}
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	
	return 1;	// 1 continue to decode
}
