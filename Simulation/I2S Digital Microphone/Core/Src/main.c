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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#define ARM_MATH_CM4
#include "arm_math.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SAMPLING_RATE	31250.0f
#define FFT_LENGTH 		256
#define LEADING_ZEROS	14
#define DC_OFFSET		255344 // 2^18 - 6800
#define PEAK_AMP		262144 // 2^18
#define COOLDOWN		40
#define INIT_COOLDOWN	150
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;

/* USER CODE BEGIN PV */
//volatile int32_t sample_i2s;
volatile float32_t sample_i2s;
uint8_t data_ready = 0;
uint8_t mag_ready = 0;

float32_t input_fft[FFT_LENGTH];
float32_t output_fft[FFT_LENGTH];
float32_t output_fft_mag[FFT_LENGTH / 2];
int32_t input_dma[FFT_LENGTH * 4];
int32_t* buf_ptr;

float peak_val;
float peak_Hz;

char msg[50];

arm_rfft_fast_instance_f32 fft_inst;

float threshold = 0.5f;
uint16_t cooldown = INIT_COOLDOWN;
uint8_t peaks[3] = { 0 };
uint16_t recognized_count = 0;
//float32_t frequency = 10000.0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write (int file, char *ptr, int len) {

	for (int idx = 0; idx < len; idx++) {
		ITM_SendChar (*ptr++);
	}
	return len;
}

void HAL_I2S_RxHalfCpltCallback (I2S_HandleTypeDef* hi2s) {
//	sample_i2s = ((float) (input_dma[0] << 2) - DC_OFFSET) / PEAK_AMP;

//	sample_i2s = ((float) ((input_dma[0] << 2) | (input_dma[1] >> LEADING_ZEROS)) - DC_OFFSET) / PEAK_AMP;
	buf_ptr = &input_dma[0];
	data_ready = 1;
}

void HAL_I2S_RxCpltCallback (I2S_HandleTypeDef* hi2s) {
//	sample_i2s = ((float) (input_dma[FFT_LENGTH * 2] << 2) - DC_OFFSET) / PEAK_AMP;

//	sample_i2s = ((float) ((input_dma[FFT_LENGTH * 4] << 2) | (input_dma[(FFT_LENGTH * 4) + 1] >> LEADING_ZEROS)) - DC_OFFSET) / PEAK_AMP;
//	buf_ptr = &input_dma[0];
	buf_ptr = &input_dma[FFT_LENGTH * 2];
	data_ready = 1;
}

void Process_Data () {
	for (int i = 0, j = 0; i < FFT_LENGTH * 2; i += 2, j++) {
		input_fft[j] = ((float) (buf_ptr[i] << 2) - DC_OFFSET) / PEAK_AMP;
	}
	input_fft[0] = 0;

	arm_rfft_fast_f32 (&fft_inst, input_fft, output_fft, 0);
	arm_cmplx_mag_f32 (output_fft, output_fft_mag, FFT_LENGTH / 2);
	data_ready = 0;
	mag_ready = 1;

	if (cooldown != 0)	cooldown--;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	arm_rfft_fast_init_f32 (&fft_inst, FFT_LENGTH);
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
  MX_I2S2_Init();
  /* USER CODE BEGIN 2 */
  HAL_I2S_Receive_DMA (&hi2s2, (uint16_t *) input_dma, FFT_LENGTH * 4);
//  HAL_Delay (100);
//  for (int i = 0; i < FFT_LENGTH; i++)
//	  input_fft[i] = arm_sin_f32 (2.0 * PI * frequency * (float32_t) i / SAMPLING_RATE);
//
//  arm_rfft_fast_f32 (&fft_inst, input_fft, output_fft, 0);
//  arm_cmplx_mag_f32 (output_fft, output_fft_mag, FFT_LENGTH / 2);

//  for (int i = 0; i < FFT_LENGTH / 2; i++) {
//	  sprintf (msg, "frequency %f: %f\r\n", ((float32_t)(i * SAMPLING_RATE) / FFT_LENGTH), output_fft_mag[i]);
//	  HAL_UART_Transmit (&huart2, (uint8_t *) msg, 50, 100);
//  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  printf ("Hello world\n");
//	  HAL_Delay (1000);

	  if (data_ready)		Process_Data ();

	  if (mag_ready && cooldown == 0) {

		  for (uint8_t i = 4; i <= 9; i++)		if (output_fft_mag[i] >= threshold)		peaks[0] = 1;
		  for (uint8_t i = 28; i <= 33; i++)	if (output_fft_mag[i] >= threshold)		peaks[1] = 1;
		  for (uint8_t i = 57; i <= 82; i++)	if (output_fft_mag[i] >= threshold)		peaks[2] = 1;

//		  for (uint16_t i = 0; i < FFT_LENGTH / 2; i++) {
//			  if (i >= 4 && i <= 9 && output_fft_mag[i] >= threshold)			peaks[0] = 1;
//			  else if (i >= 28 && i <= 33 && output_fft_mag[i] >= threshold)		peaks[0] = 1;
//			  else if (i >= 57 && i <= 82 && output_fft_mag[i] >= threshold)		peaks[0] = 1;
//			  else if (output_fft_mag[i] >= threshold) {
//				  peaks[0] = 0;
//				  peaks[1] = 0;
//				  peaks[2] = 0;
//				  break;
//			  }
//		  }

		  if (peaks[0] & peaks[1] & peaks[2]) {
			  recognized_count++;
			  cooldown = COOLDOWN;
		  }
		  peaks[0] = 0;
		  peaks[1] = 0;
		  peaks[2] = 0;


//		  peak_val = 0.0f;
//		  peak_Hz = 0.0f;
//
//		  for (uint16_t i = 0; i < FFT_LENGTH / 2; i++) {
//			  if (output_fft_mag[i] > peak_val) {
//				  peak_Hz = (i * SAMPLING_RATE) / FFT_LENGTH;
//				  peak_val = output_fft_mag[i];
//			  }
//		  }

//		  printf ("%.6f | %.6f\r\n", peak_Hz, peak_val);
//		  sprintf (msg, "%.6f | %.6f\r\n", peak_Hz, peak_val);
//		  HAL_UART_Transmit (&huart2, (uint8_t *) msg, 50, 100);
	  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2S2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_RX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_32K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_SYSCLK;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */

  /* USER CODE END I2S2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
