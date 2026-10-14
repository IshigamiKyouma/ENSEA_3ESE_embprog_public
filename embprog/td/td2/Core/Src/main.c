/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_BLINK_PERIOD 250

#define CCR_T0H 68
#define CCR_T1H 136

#define LED_NUMBER 1
#define COLOR_NUMBER 3
#define BITS_PER_COLOR 8
#define NP_BUFFER_LENGTH (LED_NUMBER * COLOR_NUMBER * BITS_PER_COLOR +1)

#define S 250
#define V 50
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int __io_putchar(int ch) {
	// Write a single character via HAL UART
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
static volatile bool sw1_pressed = false;
static volatile bool sw2_pressed = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void hsv_to_rgb(int h, int s, int v, uint8_t *r, uint8_t *g, uint8_t *b) {
	h = ((h % 360) + 360) % 360;
	if (s <= 0) {
		*r = *g = *b = (uint8_t)v;
		return;
	}
	int region = h / 60;
	int rem = h % 60;
	int p = (v * (255 - s)) / 255;
	int q = (v * (255 - (s * rem) / 60)) / 255;
	int t = (v * (255 - (s * (60 - rem)) / 60)) / 255;
	switch (region) {
	case 0: *r = (uint8_t)v; *g = (uint8_t)t; *b = (uint8_t)p; break;
	case 1: *r = (uint8_t)q; *g = (uint8_t)v; *b = (uint8_t)p; break;
	case 2: *r = (uint8_t)p; *g = (uint8_t)v; *b = (uint8_t)t; break;
	case 3: *r = (uint8_t)p; *g = (uint8_t)q; *b = (uint8_t)v; break;
	case 4: *r = (uint8_t)t; *g = (uint8_t)p; *b = (uint8_t)v; break;
	default: *r = (uint8_t)v; *g = (uint8_t)p; *b = (uint8_t)q; break;
	}
}

typedef struct color_struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}color_t;
uint8_t np_dma_buffer[NP_BUFFER_LENGTH];
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration --------------------------------------------------------*/
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C1_Init();
	MX_TIM3_Init();
	MX_TIM2_Init();
	MX_USART2_UART_Init();
	MX_SPI1_Init();

	/* USER CODE BEGIN 2 */
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)np_dma_buffer, NP_BUFFER_LENGTH);
	void color_to_buffer(color_t * color, uint8_t * buffer){
		for (int i=0;i < NP_BUFFER_LENGTH;i++){
			buffer[i] = ((color->g >> (7-i) & 0x01)) ? CCR_T1H : CCR_T0H;
		}
		for (int i=0;i < NP_BUFFER_LENGTH;i++){
			buffer[BITS_PER_COLOR+i] = ((color->r >> (7-i) & 0x01)) ? CCR_T1H : CCR_T0H;
		}
		for (int i=0;i < NP_BUFFER_LENGTH;i++){
			buffer[2*BITS_PER_COLOR+i] = ((color->b >> (7-i) & 0x01)) ? CCR_T1H : CCR_T0H;
		}
		/* Dernier bit force à 0 pour 0V sura Neopixel */
		buffer[NP_BUFFER_LENGTH - 1] = 0;
	}
	uint32_t last_tick = 0;
	color_t color;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		uint32_t position = __HAL_TIM_GET_COUNTER(&htim3);
		printf("encoder position %lu \n\r", position);
		static uint32_t previous_position = 0;
		static uint8_t hue = 0;
		if (previous_position!=position){
			hsv_to_rgb(hue++, S, V, &color.r, &color.g, &color.b);
			color_to_buffer(&color, np_dma_buffer);
		}
		previous_position = position;

//		uint32_t tick = HAL_GetTick();
//		if (last_tick != tick) {
//			last_tick = tick;
//			if (0 == (tick % LED_BLINK_PERIOD)) {
//				HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//			}
//		}
//
//		if (sw1_pressed) {
//			sw1_pressed = false;
//			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
//		}
//
//		if (sw2_pressed) {
//			sw2_pressed = false;
//			HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
//		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
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

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == BTN_SW1_Pin) {
		sw1_pressed = true;
	} else if (GPIO_Pin == BTN_SW2_Pin) {
		sw2_pressed = true;
	}
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
#ifdef USE_FULL_ASSERT
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
