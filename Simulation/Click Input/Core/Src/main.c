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
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define QUIET_00	2999.87414
#define QUIET_01	2847.96404882
#define QUIET_02	2717.03515446
#define QUIET_03	2604.18962032
#define QUIET_04	2506.9298471
#define QUIET_05	2423.10319361
#define QUIET_06	2350.85433263
#define QUIET_07	2288.58418709
#define QUIET_08	2234.91453786
#define QUIET_09	2188.65751977
#define QUIET_10	2148.78933072
#define QUIET_11	2114.42757192
#define QUIET_12	2084.81171787
#define QUIET_13	2059.28628374
#define QUIET_14	2037.28631755
#define QUIET_15	2018.32489619
#define QUIET_16	2001.98234833
#define QUIET_17	1987.89696595
#define QUIET_18	1975.75699863
#define QUIET_19	1965.29375366
#define QUIET_20	1956.27564903

#define QUIET_FUNC(x) (1900 + (1097 * 1.00262 * pow (0.226198, (double) x / 10)))

#define LOUD_00		4100.7509
#define LOUD_01		3893.13553662
#define LOUD_02		3705.10627865
#define LOUD_03		3534.81540384
#define LOUD_04		3380.58950116
#define LOUD_05		3240.91302651
#define LOUD_06		3114.41340982
#define LOUD_07		2999.84756715
#define LOUD_08		2896.08968509
#define LOUD_09		2802.12015772
#define LOUD_10		2717.01556712
#define LOUD_11		2639.93960916
#define LOUD_12		2570.13487531
#define LOUD_13		2506.91540978
#define LOUD_14		2449.65996877
#define LOUD_15		2397.80591561
#define LOUD_16		2350.84369192
#define LOUD_17		2308.31181022
#define LOUD_18		2269.79231905
#define LOUD_19		2234.90669583
#define LOUD_20		2203.3121272
#define LOUD_21		2174.69814026
#define LOUD_22		2148.78355165
#define LOUD_23		2125.31370439
#define LOUD_24		2104.05796545
#define LOUD_25		2084.80745934
#define LOUD_26		2067.37301556
#define LOUD_27		2051.58330967
#define LOUD_28		2037.28317969
#define LOUD_29		2024.33210139
#define LOUD_30		2012.60280735

#define LOUD_FUNC(x) (1900 + (2195 * 1.00262 * pow (0.371244, (double) x / 10)))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
//char message[225];
//char msg [8];
//int baseline = 1900; //1351
//int threshold = 600;
//int equilibrium = 75;
////
//uint16_t val;
//uint16_t val_buffer[1];
//uint16_t check_buffer[500];
//uint16_t check_buffer_start;
//uint16_t click_buffer[3];
//uint8_t cycle_count = 0;
//
//bool click_in_check = false;
//bool one_cycle_passed = false;
//bool why = false;

char message[100];
uint16_t val = 0;
uint16_t val_buffer[1];
uint32_t buffer_size = 1;

int baseline = 1900;
int threshold = 600;
int equilibrium = 75;
int quiet_sample_size = 20; // 13
int loud_sample_size = 30; //25
int current_sample = 0;
int is_loud_sound = false;
bool checking = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc) {
	val = abs (*val_buffer - baseline);
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *) val_buffer, 1);

	if (val > threshold && !checking) {
		if (val > 1097) 	is_loud_sound = true;
		else 				is_loud_sound = false;

		current_sample = 0;
		checking = true;

		HAL_TIM_Base_Start_IT (&htim2);
	}
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim) {
	if (is_loud_sound && current_sample >/*=*/ loud_sample_size) {
		sprintf (message, "YES  LOUD\r\n");
		HAL_UART_Transmit (&huart2, (uint8_t *) message, 100, HAL_MAX_DELAY);
		checking = false;
		HAL_TIM_Base_Stop_IT (&htim2);
	} else if (!is_loud_sound && current_sample >/*=*/ quiet_sample_size) {
		sprintf (message, "YES QUIET\r\n");
		HAL_UART_Transmit (&huart2, (uint8_t *) message, 100, HAL_MAX_DELAY);
		checking = false;
		HAL_TIM_Base_Stop_IT (&htim2);
	}

	if (is_loud_sound &&
			(double) abs (*val_buffer) > LOUD_FUNC (current_sample)) {
		sprintf (message, "NO  LOUD, %d %f %f %d\r\n", current_sample, (double) abs (*val_buffer), LOUD_FUNC (current_sample), (double) abs (*val_buffer) > LOUD_FUNC (current_sample));
		HAL_UART_Transmit (&huart2, (uint8_t *) message, 100, HAL_MAX_DELAY);
		checking = false;
		HAL_TIM_Base_Stop_IT (&htim2);
	} else if (!is_loud_sound &&
			(double) abs (*val_buffer) > QUIET_FUNC (current_sample)) {
		sprintf (message, "NO QUIET, %d %f %f %d\r\n", current_sample, (double) abs (*val_buffer), QUIET_FUNC (current_sample), (double) abs (*val_buffer) > QUIET_FUNC (current_sample));
		HAL_UART_Transmit (&huart2, (uint8_t *) message, 100, HAL_MAX_DELAY);
		checking = false;
		HAL_TIM_Base_Stop_IT (&htim2);
	} else {
		current_sample++;
	}
}




//void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc) {
//	val = abs (*val_buffer - baseline);
//	if (!click_in_check && val > threshold) {
//		click_in_check = true;
//		check_buffer_start = *val_buffer;
//		HAL_TIM_Base_Start_IT (&htim2);
//	}
//
//	HAL_ADC_Start_DMA (&hadc1, click_in_check ? (uint32_t *) check_buffer : (uint32_t *) val_buffer, click_in_check ? 100 : 1);
//}
//
//void HAL_TIM_PeriodElaspedCallback (TIM_HandleTypeDef *htim) {
//	if (click_in_check) val = 1;

//	if (why) {
//		why = false;
//		click_in_check = false;
//		HAL_TIM_Base_Stop_IT (&htim2);
//
//		for (int i = 0; i < 100; i++) {
//			sprintf (msg, "%d ", check_buffer [i]);
//			HAL_UART_Transmit (&huart2, (uint8_t *) msg, 8, HAL_MAX_DELAY);
//		}
//		sprintf (msg, "\r\n");
//		HAL_UART_Transmit (&huart2, (uint8_t *) msg, 8, HAL_MAX_DELAY);
//	} else	why = true;
//}

//void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc) {
//	val = abs (*check_buffer - baseline);
//
//	if (val > threshold) {
//		if (abs (check_buffer[499] - baseline) < abs (check_buffer[399] - baseline) &&
//				abs (check_buffer[399] - baseline) < abs (check_buffer[299] - baseline) &&
//				abs (check_buffer[299] - baseline) < abs (check_buffer[199] - baseline) &&
//				abs (check_buffer[199] - baseline) < abs (check_buffer[99] - baseline) &&
//				abs (check_buffer[99] - baseline) < abs (check_buffer[0] - baseline) && abs (check_buffer[499] - baseline) < equilibrium) {
//			sprintf (message, "%d %d %d %d %d %d\r\n",
//					abs (check_buffer[0] - baseline), abs (check_buffer[99] - baseline),
//					abs (check_buffer[199] - baseline), abs (check_buffer[299] - baseline), abs (check_buffer[399] - baseline), abs (check_buffer[499] - baseline));
//			HAL_UART_Transmit (&huart2, (uint8_t *) message, 225, HAL_MAX_DELAY);
//		}
//	}
//
//	HAL_ADC_Start_DMA (&hadc1, (uint32_t *) check_buffer, 500);
//}


//void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc) {
//	val = abs (*val_buffer - baseline);
////	HAL_ADC_Start_DMA (&hadc1, (uint32_t *) val_buffer, 1);
//	HAL_ADC_Start_DMA (&hadc1, (uint32_t *) check_buffer, 4);
//
//	if (!click_in_check && val > threshold) {
//		click_in_check = true;
//		click_buffer[0] = val;
//		HAL_TIM_Base_Start_IT (&htim2);
//	}
//}

//void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim) {
//	cycle_count++;
//	if (cycle_count == 1) {
//		click_buffer[1] = val;
//	} else if (cycle_count == 2) {
//		click_buffer[2] = val;
//		cycle_count = 0;
//		click_in_check = false;
//		HAL_TIM_Base_Stop_IT (&htim2);
//
////		if (click_buffer[1] > (click_buffer[0] - equilibrium) / 2 && click_buffer[2] > equilibrium) {}
////		  sprintf (message, "NO   : %d, %d, %d\r\n", click_buffer[0], click_buffer[1], click_buffer[2]);
//		if ((click_buffer[1] < click_buffer[0] / 4) && click_buffer[2] < equilibrium && click_buffer[2] < click_buffer[1]) {
//		  sprintf (message, "YES  : %d, %d, %d\r\n", click_buffer[0], click_buffer[1], click_buffer[2]);
//		  HAL_UART_Transmit (&huart2, (uint8_t *) message, 100, HAL_MAX_DELAY);
//		}
//	}
//
//}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_NVIC_SetPriority (TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ (TIM2_IRQn);
//  HAL_TIM_Base_Start_IT (&htim2);

  HAL_ADC_Start_DMA (&hadc1, (uint32_t *) val_buffer, buffer_size);
//  HAL_ADC_Start_DMA (&hadc1, (uint32_t *) check_buffer, 500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  if (click_gathered) {
//		  if (click_buffer[1] > ((click_buffer[0] - equilibrium) / 2) + equilibrium && click_buffer[2] > equilibrium)
//			  sprintf (message, "NO   : %d, %d, %d\r\n", click_buffer[0], click_buffer[1], click_buffer[2]);
//		  else
//			  sprintf (message, "YES  : %d, %d, %d\r\n", click_buffer[0], click_buffer[1], click_buffer[2]);
//
//		  HAL_UART_Transmit (&huart2, (uint8_t *) message, 100, HAL_MAX_DELAY);
//		  click_gathered = false;
//	  }
//	  sprintf (message, "%d\r\n", abs (*val_buffer - baseline));//abs (val - baseline));
//	  HAL_UART_Transmit (&huart2, (uint8_t *) message, 20, HAL_MAX_DELAY);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC1;
  PeriphClkInit.Adc1ClockSelection = RCC_ADC1PLLCLK_DIV1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 31;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 24;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  huart2.Init.BaudRate = 38400;
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
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
