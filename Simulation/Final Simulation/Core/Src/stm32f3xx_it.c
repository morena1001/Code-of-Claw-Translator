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
#include "envelope.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
bool toggle = false;

uint8_t end_of_letter_counter = 0;
char cur_let[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void Check_Trie_Root (char letter);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
/* USER CODE BEGIN EV */
extern ILI9341_t ili9341;
extern trie_node* travel;
extern mtch6102_t mtch6102;
extern envelope_t env;

extern ADC_HandleTypeDef hadc1;
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
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
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

			ILI9341_Clear_Screen(&ili9341);
			*cur_let = '\0';
			end_of_letter_counter = 0;
			travel = root;
		}
	} else if (!HAL_GPIO_ReadPin (BB_GPIO_Port, BB_Pin)) {
		if (!toggle) {
			toggle = true;

			ILI9341_Rewrite_Character (&ili9341, ' ');
			*cur_let = '\0';
			end_of_letter_counter = 0;
			travel = root;
			ILI9341_Delete_Character (&ili9341);
		}
	} else if (!HAL_GPIO_ReadPin (NC_GPIO_Port, NC_Pin)) {
		if (!toggle) {
			toggle = true;

			*cur_let = '\0';
			end_of_letter_counter = 0;
			travel = root;
			ILI9341_Increment_Char_Pos (&ili9341);
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
		}
	} else if (!HAL_GPIO_ReadPin (CL_GPIO_Port, CL_Pin) || Envelope_Read_Click_Status (&env)) {
		if (!toggle) {
			toggle = true;

			*cur_let = Traverse_Tree (&travel, '\\');
			ILI9341_Rewrite_Character (&ili9341, *cur_let);
			Check_Trie_Root('\\');

			end_of_letter_counter = 251;
		}
	} else if (!HAL_GPIO_ReadPin (ST_GPIO_Port, ST_Pin) || MTCH6102_Read_Scratch_Status (&mtch6102)) {
		if (!toggle) {
			toggle = true;

			*cur_let = Traverse_Tree (&travel, '|');
			ILI9341_Rewrite_Character (&ili9341, *cur_let);
			Check_Trie_Root('|');

			end_of_letter_counter = 251;
		}
	} else if (!HAL_GPIO_ReadPin (TP_GPIO_Port, TP_Pin) || MTCH6102_Read_Tap_Status (&mtch6102)) {
		if (!toggle) {
			toggle = true;

			*cur_let = Traverse_Tree (&travel, '/');
			ILI9341_Rewrite_Character (&ili9341, *cur_let);
			Check_Trie_Root('/');

			end_of_letter_counter = 251;
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
	if ((env.is_loud_input && env.current_sample > env.loud_sample_size) ||
	   (!env.is_loud_input && env.current_sample > env.quiet_sample_size)) {
		env.click = true;
		env.click_checking = false;
		HAL_TIM_Base_Stop_IT (&htim6);
	}

	if ((env.is_loud_input &&
			(double) abs (*(env.input.raw_buffer)) > LOUD_FUNC (env.current_sample)) ||
	   (!env.is_loud_input &&
			(double) abs (*(env.input.raw_buffer)) > QUIET_FUNC (env.current_sample))) {
		env.click_checking = false;
		HAL_TIM_Base_Stop_IT (&htim6);
	} else {
		(env.current_sample)++;
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

void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef *hadc) {
	env.input.procsd_val= abs (*(env.input.raw_buffer) - env.baseline);
	HAL_ADC_Start_DMA (&hadc1, (uint32_t *) env.input.raw_buffer, env.input.buffer_size);

	if (env.input.procsd_val > env.threshold && !env.click_checking) {
		if (env.input.procsd_val > 1097) 	env.is_loud_input = true; // WHAT DOES THE 1907 MEAN ?!?!?!?!?!?!
		else 				env.is_loud_input = false;

		env.current_sample = 0;
		env.click_checking = true;

		HAL_TIM_Base_Start_IT (&htim6);
	}
}
/* USER CODE END 1 */
