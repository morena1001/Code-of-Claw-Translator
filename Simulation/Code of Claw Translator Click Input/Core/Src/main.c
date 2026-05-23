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
#include "arm_math.h"
#include "stdio.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define BUFFER_SIZE 256
//#define INT16_TO_FLOAT 	1.0f / 32768.0f
//#define INT32_TO_FLOAT 	1.0f / 2147483648.0f

//#define FFT_LENGTH	1024
//#define SAMPLE_RATE	31250.0f
#define FFT_LENGTH 		256
#define SAMPLING_RATE	16000.0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
//int16_t audio_data[BUFFER_SIZE];
//volatile int16_t sample_i2s;
//volatile int16_t* in_buf_ptr;
//
//uint8_t data_ready_flag;

arm_rfft_fast_instance_f32 fft_inst;
float32_t frequency = 500.0;

char msg[50];

float32_t input_fft[FFT_LENGTH];
float32_t output_fft[FFT_LENGTH];
float32_t output_fft_mag[FFT_LENGTH / 2];

//float peak_val = 0.0f;
//float peak_Hz = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void HAL_I2S_RxHalfCpltCallback (I2S_HandleTypeDef* hi2s) {
//	in_buf_ptr = &(audio_data[0]);
//	data_ready_flag = 1;
//}

//void HAL_I2S_RxCpltCallback (I2S_HandleTypeDef* hi2s) {
////	in_buf_ptr = &(audio_data[BUFFER_SIZE / 2]);
//	in_buf_ptr = &(audio_data[0]);
////	sample_i2s = audio_data[0];
//	data_ready_flag = 1;
//}
//
//void Process_Data () {
//	static float left_in, right_in;
//	static int16_t fft_idx = 0;
//
//	for (uint16_t n = 0; n < BUFFER_SIZE; n += 2) {
//		left_in = (float) (in_buf_ptr[n] < 0 ? in_buf_ptr[n] + 1900 : in_buf_ptr[n] - 1900);
////		left_in = INT16_TO_FLOAT * ((float) (in_buf_ptr[n] < 0 ? in_buf_ptr[n] + 1900 : in_buf_ptr[n] - 1900));
//		right_in = INT16_TO_FLOAT * ((float) in_buf_ptr[n + 1]);
//
//		fft_buff_in[fft_idx] = left_in;
//		fft_idx++;
//
//		if (fft_idx == FFT_BUFFER_SIZE) {
//			arm_rfft_fast_f32 (&fft_handler, fft_buff_in, fft_buff_out, 1);
//			fft_flag = 1;
//			fft_idx = 0;
//		}
//	}
//
//	data_ready_flag = 0;
//}
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
//  HAL_I2S_Receive_DMA (&hi2s2, (uint16_t *) audio_data, BUFFER_SIZE);
  HAL_Delay (100);
    for (int i = 0; i < FFT_LENGTH; i++)
  	  input_fft[i] = arm_sin_f32 (2.0 * PI * frequency * (float32_t) i / SAMPLING_RATE);

    arm_rfft_fast_f32 (&fft_inst, input_fft, output_fft, 0);
    arm_cmplx_mag_f32 (output_fft, output_fft_mag, FFT_LENGTH / 2);

    for (int i = 0; i < FFT_LENGTH / 2; i++) {
  	  sprintf (msg, "frequency %f: %f\r\n", ((float32_t)(i * SAMPLING_RATE) / FFT_LENGTH), output_fft_mag[i]);
  	  HAL_UART_Transmit (&huart2, (uint8_t *) msg, 50, 100);
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
//	  if (data_ready_flag) {
//		  Process_Data ();
//	  }
//
//	  if (fft_flag) {
//		  peak_val = 0.0f;
//		  peak_Hz = 0;
//
//		  uint16_t freq_idx = 0;
//		  for (uint16_t idx = 0; idx < FFT_BUFFER_SIZE; idx += 2) {
//			  float cur_val = sqrtf ((fft_buff_out[idx] * fft_buff_out[idx]) + (fft_buff_out[idx + 1] * fft_buff_out[idx + 1]));
//			  if (cur_val > peak_val) {
//				  peak_Hz = ((float) (freq_idx * SAMPLE_RATE_HZ) / ((float) FFT_BUFFER_SIZE));
//				  peak_val = cur_val;
//			  }
//		  }
//
//		  freq_idx++;
//	  }
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
