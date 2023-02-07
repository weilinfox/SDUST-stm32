/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "StartBMP.h"
#include "Images.h"
#include "Images_jpg.h"
#include "Touches.h"
//#include "StartBMP.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define PICT_DIR					"pictures"
#define C_BUF_SIZE 				128U
#define PICT_PAUSE				"Paused, touch panel to show next..."
#define PICT_CONTINUE			"Continue..."
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//FATFS fatfs;
FRESULT fresult;
FIL pic_fil;
DIR pic_dir = {0};
FILINFO pic_info;
char cbuf[C_BUF_SIZE];
volatile int8_t pic_pause = 0;
int8_t pic_pause_old = 0;
volatile int32_t pic_pause_delay = 0;

point2 touch_p, touch_old = {0, 0};
uint32_t touch_delay;
const uint32_t touch_delay_init = 2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void error_handler(void)
{
	f_mount(NULL, SDPath, 1);
	// lcd_log("Umount disk", fresult);
	while (1) {
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(250);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	// uint32_t x, y;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	
	// initialize lcd screen
	cwl_lcd_init();
	cwl_lcd_clear();
	
	cwl_lcd_log("Initialize system...", 0);
	cwl_print_bmp_r((u16*)start_bmp, 300, 200, 200, 42);
	LcdPrintStr("By SDUST weilinfox", 350, 243, WHITE, BLACK, 0);
	
	
	if (cwl_touch_init()) {
		cwl_lcd_log("Load touch panel firmware failed.", 0);
		while (1) {
			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
			HAL_Delay(250);
		}
	}
	
	// mount fat file system
	fresult = f_mount(&SDFatFS, SDPath, 1);
	if (fresult != FR_OK) {
		if (fresult == FR_NO_FILESYSTEM)
			cwl_lcd_log("Not a FAT file system", 0);
		else
			cwl_lcd_log("Mount error", fresult);
		error_handler();
	}
	
	/*
	RA8875_DrawHLine(0,240,800,BLACK);
	RA8875_DrawVLine(400,0,480,BLACK);
	LcdPrint16bitBmp((u16*)0x0800F000,2,2,231,205);
	for(x=2;x<231;x++)
		for(y=2;y<205;y++) {
			RA8875_PutPixelGUI(x,y+240,RA8875_GetPixelGUI(x,y));
		}
	LcdFillRec(450,60,550,180,0,BLACK);
	RA8875_RTERect(650,60,750,180,BLACK);
	LcdPrintStr("LCD experiment by weilinfox",400+2,240+2,BLACK,WHITE,0);
	LcdPrintBIAS(400,480,800,240,BLACK);
	LcdPrintBIAS(600,480,800,360,BLACK);
	*/
	
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		uint32_t plen;
		uint32_t pic_cnt = 0;
		if (pic_pause != pic_pause_old) {
			cwl_lcd_log(PICT_PAUSE, 0);
			HAL_Delay(250);
			pic_pause_old = pic_pause;
		}
		touch_delay = touch_delay_init; touch_old = touch_p;
		while (pic_pause && touch_delay) {	// pause
			touch_p = cwl_touch_point();
			if (touch_old.x != touch_p.x && touch_old.y != touch_p.y) {
				//cwl_lcd_log("touch x: ", touch_p.x);
				touch_old = touch_p;
				touch_delay--;
			}
			HAL_Delay(2);
		}
		if (pic_pause != pic_pause_old) {
			cwl_lcd_log(PICT_CONTINUE, 0);
			HAL_Delay(250);
			pic_pause_old = pic_pause;
		}
		
		fresult = f_opendir(&pic_dir, PICT_DIR);
		if (fresult != FR_OK) { // open directory failed
			if (fresult == FR_NO_PATH) {
				f_mkdir(PICT_DIR);
				cwl_lcd_log("Please put your pictures under \""PICT_DIR"\"", 0);
			} else {
				cwl_lcd_log("Open "PICT_DIR" failed", fresult);
			}
			error_handler();
		}
		fresult = f_readdir(&pic_dir, &pic_info);
		if (fresult != FR_OK) {
			cwl_lcd_log("Read file error", fresult);
			error_handler();
		} else if (! pic_info.fname[0]) {
			cwl_lcd_log("No file found under "PICT_DIR, 0);
			error_handler();
		}
		
		do {
			snprintf(cbuf, C_BUF_SIZE, PICT_DIR"/%s", (char*)pic_info.fname);
			if (pic_pause != pic_pause_old) {
				cwl_lcd_log(PICT_PAUSE, 0);
				HAL_Delay(250);
				pic_pause_old = pic_pause;
			}
			touch_delay = touch_delay_init; touch_old = touch_p;
			while (pic_pause && touch_delay) {	// pause
				touch_p = cwl_touch_point();
				if (touch_old.x != touch_p.x && touch_old.y != touch_p.y) {
					//cwl_lcd_log("touch x: ", touch_p.x);
					touch_old = touch_p;
					touch_delay--;
				}
				HAL_Delay(2);
			}
			if (pic_pause != pic_pause_old) {
				cwl_lcd_log(PICT_CONTINUE, 0);
				HAL_Delay(250);
				pic_pause_old = pic_pause;
			}
			
			fresult = f_open(&pic_fil, cbuf, FA_READ);
			if (fresult != FR_OK) {
				cwl_lcd_log("File open error", fresult);
				error_handler();
			}
			
			plen = strlen(cbuf);

			//cwl_lcd_log(cbuf, 0);
			//cwl_lcd_white();
			cwl_lcd_clear();
			if (! strcmp(cbuf+plen-4, ".bmp") || ! strcmp(cbuf+plen-4, ".BMP")) {
				if (!cwl_lcd_bmp(&pic_fil) && !pic_pause) HAL_Delay(2000);
				pic_cnt++;
			} else if (! strcmp(cbuf+plen-4, ".jpg") || ! strcmp(cbuf+plen-4, ".JPG")) {
				//cwl_lcd_log("cwl_lcd_jpg_ptr", (int32_t)&pic_fil);
				//HAL_Delay(500);
				if (!cwl_lcd_jpg(&pic_fil) && !pic_pause) HAL_Delay(2000);
				//HAL_Delay(500);
				pic_cnt++;
			}
			
			// close file
			fresult = f_close(&pic_fil);
			if (fresult != FR_OK) {
				cwl_lcd_log("File close error", fresult);
				HAL_Delay(500);
			}

			fresult = f_readdir(&pic_dir, &pic_info);
		} while (fresult == FR_OK && pic_info.fname[0]);
		
		if (fresult != FR_OK) {
			cwl_lcd_log("Get next file failed", fresult);
			error_handler();
		}
		fresult = f_closedir(&pic_dir);
		if (fresult != FR_OK) {
			cwl_lcd_log("Close directory "PICT_DIR" error", fresult);
			error_handler();
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if (pic_cnt == 0) {
			cwl_lcd_log("No bmp or jpg file found.", 0);
			error_handler();
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// pause pin
	if (GPIO_Pin == KEY1_Pin && ! pic_pause_delay) {
		pic_pause = ! pic_pause;
		pic_pause_delay = 500;
	}
}

/**
  * @brief  SYSTICK callback.
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSTICK_Callback could be implemented in the user file
   */
	if (pic_pause_delay)
		pic_pause_delay--;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
