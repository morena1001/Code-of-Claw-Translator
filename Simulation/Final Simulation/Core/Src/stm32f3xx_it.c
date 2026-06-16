/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI9341.h"
#include "code_tree.h"
#include "MTCH6102.h"
#include "arm_math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MIC_SAMPLE_RATE		31250.0f
#define FFT_LENGTH			256
#define MIC_DC_OFFSET		255344 // 2^18 - 6800
#define MIC_PEAK_AMP		262144 // 2^18
#define BASE_MIC_THRESHOLD	0.5f
#define FFT_COOLDOWN		40
#define FFT_INIT_COOLDOWN	75
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
bool toggle = false;

uint8_t end_of_letter_counter = 0;
char cur_let[2];

uint8_t mic_data_ready = 0;
uint8_t mic_mag_ready = 0;
float32_t input_fft[FFT_LENGTH];
float32_t output_fft[FFT_LENGTH];
float32_t output_fft_mag[FFT_LENGTH / 2];
int32_t* buf_ptr;
float mic_threshold = BASE_MIC_THRESHOLD;
uint8_t click_cooldown = FFT_INIT_COOLDOWN;
uint8_t click_peaks[3] = { 0 };
uint8_t recognized_click = 0;

uint8_t timer_started = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void Check_Trie_Root (char letter);
uint8_t Read_Click_Status ();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_spi3_rx;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim16;
/* USER CODE BEGIN EV */
extern ILI9341_t ili9341;
extern trie_node* travel;
extern mtch6102_t mtch6102;

extern arm_rfft_fast_instance_f32 fft_inst;
extern int32_t input_dma[FFT_LENGTH * 4];
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
//	MTCH6102_REG_GESTURESTATE
	MTCH6102_Receive (&mtch6102, MTCH6102_REG_GESTURESTATE, &(mtch6102.gesture), 1);
	mtch6102.tap = IS_TAP (mtch6102.gesture);
	mtch6102.scratch = IS_SCRATCH (mtch6102.gesture);
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(INT_Pin);
  /* USER CODE BEGIN EXTI1_IRQn 1 */

  /* USER CODE END EXTI1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi3_rx);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update and TIM16 interrupts.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
	if (timer_started == 1) {
		ILI9341_Delete_Cue (&ili9341);
		timer_started = 0;
		HAL_TIM_Base_Stop_IT (&htim16);
	} else timer_started = 1;
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	if (end_of_letter_counter == 1) {
		Add_Letter (*cur_let);
		*cur_let = '\0';
		end_of_letter_counter = 0;
		travel = root;
		ILI9341_Increment_Char_Pos (&ili9341);
	}

	if (!HAL_GPIO_ReadPin (CB_GPIO_Port, CB_Pin)) {
		if (!toggle) {
			toggle = true;

			ILI9341_Clear_Screen (&ili9341);
			*cur_let = '\0';
			end_of_letter_counter = 0;
			travel = root;

			__HAL_TIM_SET_COUNTER (&htim16, 0);
			ILI9341_Print_Cue (&ili9341, CLEAR);
			HAL_TIM_Base_Start_IT (&htim16);
		}
	} else if (!HAL_GPIO_ReadPin (BB_GPIO_Port, BB_Pin)) {
		if (!toggle) {
			toggle = true;

			ILI9341_Rewrite_Character (&ili9341, ' ');
			*cur_let = '\0';
			end_of_letter_counter = 0;
			travel = root;
			ILI9341_Delete_Character (&ili9341);

			__HAL_TIM_SET_COUNTER (&htim16, 0);
			ILI9341_Print_Cue (&ili9341, DELETE);
			HAL_TIM_Base_Start_IT (&htim16);
		}
	} else if (!HAL_GPIO_ReadPin (NC_GPIO_Port, NC_Pin)) {
		if (!toggle) {
			toggle = true;

			*cur_let = '\0';
			end_of_letter_counter = 0;
			travel = root;
			ILI9341_Increment_Char_Pos (&ili9341);

			__HAL_TIM_SET_COUNTER (&htim16, 0);
			ILI9341_Print_Cue (&ili9341, SPACE);
			HAL_TIM_Base_Start_IT (&htim16);
		}
	} else if (!HAL_GPIO_ReadPin (PB_GPIO_Port, PB_Pin)) {
		if (!toggle) {
			toggle = true;

			*cur_let = '.';
			ILI9341_Rewrite_Character (&ili9341, '.');
			*cur_let = '\0';
			end_of_letter_counter = 0;
			travel = root;
			ILI9341_Increment_Char_Pos (&ili9341);

			__HAL_TIM_SET_COUNTER (&htim16, 0);
			ILI9341_Print_Cue (&ili9341, PERIOD);
			HAL_TIM_Base_Start_IT (&htim16);
		}
	} else if (!HAL_GPIO_ReadPin (CL_GPIO_Port, CL_Pin) || Read_Click_Status () == 1) {
		if (!toggle) {
			toggle = true;

			*cur_let = Traverse_Tree (&travel, '\\');
			ILI9341_Rewrite_Character (&ili9341, *cur_let);
			Check_Trie_Root('\\');

			end_of_letter_counter = 251;

			__HAL_TIM_SET_COUNTER (&htim16, 0);
			ILI9341_Print_Cue (&ili9341, CLICK);
			HAL_TIM_Base_Start_IT (&htim16);
		}
	} else if (!HAL_GPIO_ReadPin (ST_GPIO_Port, ST_Pin) || MTCH6102_Read_Scratch_Status (&mtch6102)) {
		if (!toggle) {
			toggle = true;

			*cur_let = Traverse_Tree (&travel, '|');
			ILI9341_Rewrite_Character (&ili9341, *cur_let);
			Check_Trie_Root('|');

			end_of_letter_counter = 251;

			__HAL_TIM_SET_COUNTER (&htim16, 0);
			ILI9341_Print_Cue (&ili9341, SCRATCH);
			HAL_TIM_Base_Start_IT (&htim16);
		}
	} else if (!HAL_GPIO_ReadPin (TP_GPIO_Port, TP_Pin) || MTCH6102_Read_Tap_Status (&mtch6102)) {
		if (!toggle) {
			toggle = true;

			*cur_let = Traverse_Tree (&travel, '/');
			ILI9341_Rewrite_Character (&ili9341, *cur_let);
			Check_Trie_Root('/');

			end_of_letter_counter = 251;

			__HAL_TIM_SET_COUNTER (&htim16, 0);
			ILI9341_Print_Cue (&ili9341, TAP);
			HAL_TIM_Base_Start_IT (&htim16);
		}
	} else {
		toggle = false;
		if (end_of_letter_counter > 0) {
			end_of_letter_counter--;
		}
	}
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	if (mic_data_ready) {
		for (int i = 0, j = 0; i < FFT_LENGTH * 2; i+= 2, j++)
			input_fft[j] = ((float) (buf_ptr[i] << 2) - MIC_DC_OFFSET) / MIC_PEAK_AMP;
		input_fft[0] = 0;

		arm_rfft_fast_f32 (&fft_inst, input_fft, output_fft, 0);
		arm_cmplx_mag_f32 (output_fft, output_fft_mag, FFT_LENGTH / 2);
		mic_data_ready = 0;
		mic_mag_ready = 1;

		if (click_cooldown != 0)		click_cooldown--;
	}

	if (mic_mag_ready && click_cooldown == 0) {
		for (uint8_t i = 4; i <= 9; i++)	if (output_fft_mag[i] >= mic_threshold)		click_peaks[0] = 1;
		for (uint8_t i = 28; i <= 33; i++)	if (output_fft_mag[i] >= mic_threshold)		click_peaks[1] = 1;
		for (uint8_t i = 57; i <= 82; i++)	if (output_fft_mag[i] >= mic_threshold)		click_peaks[2] = 1;

		if (click_peaks[0] & click_peaks[1] & click_peaks[2]) {
			recognized_click = 1;//++;
			click_cooldown = FFT_COOLDOWN;
		}

		click_peaks[0] = 0; click_peaks[1] = 0; click_peaks[2] = 0;
		mic_mag_ready = 0;
	}
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void Check_Trie_Root (char letter) {
	if (travel->data == '-') {
		ILI9341_Increment_Char_Pos (&ili9341);
		*cur_let = Traverse_Tree (&travel, letter);
		ILI9341_Rewrite_Character (&ili9341, *cur_let);
	}
}

uint8_t Read_Click_Status () {
	uint8_t val = recognized_click;
	recognized_click = 0;
	return val;
}

void HAL_I2S_RxHalfCpltCallback (I2S_HandleTypeDef* hi2s) {
	buf_ptr = &input_dma[0];
	mic_data_ready = 1;
}

void HAL_I2S_RxCpltCallback (I2S_HandleTypeDef* hi2s) {
	buf_ptr = &input_dma[FFT_LENGTH * 2];
	mic_data_ready = 1;
}
/* USER CODE END 1 */
