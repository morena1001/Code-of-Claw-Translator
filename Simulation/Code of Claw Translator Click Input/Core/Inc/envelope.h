/*
 * envelope.h
 *
 *  Created on: Apr 22, 2026
 *      Author: josue
 */

#ifndef INC_ENVELOPE_H_
#define INC_ENVELOPE_H_

#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "math.h"

/*
 * FORMULAS
 */

#define V_C		0.000432525951557

#define V1_B	1.25951557093
#define V1(v)	(-1.0 * ((double) v * V_C) + V1_B) // Function for scaling functions according to click check start value

#define V2_B	0.740484429066
#define V2(v)	(((double) v * V_C) + V2_B) // Function for scaling the Heavyside step function

#define HS_S(x)	(0.7 * V2 (x)) // Heavyside step function to differentiate the end of one function and the start of the other

//#define ENV_FUNC_1(base, v1, sample_num)	((double) base + (double) v1 * (1.25 * cos (2.55 * (double) v1 * ((double) sample_num - 0.25))))
#define ENV_FUNC_1(entry_val, v1, sample_num)			((double) entry_val * (1.25 * cos (2.55 * (double) v1 * (((double) sample_num / 10) - 0.25))))
//#define ENV_FUNC_1(v1, x)			((double) v1 * (1.25 * cos (2.55 * (double) v1 * ((double) x - 0.25))))
//#define ENV_FUNC_2(base, v1, sample_num)	((double) base + (double) v1 * 1.83034 * pow (0.162931, (double) v1 * (double) sample_num))
#define ENV_FUNC_2(entry_val, v1, sample_num)			((double) entry_val * 1.83034 * pow (0.162931, (double) v1 * ((double) sample_num / 10)))
//#define ENV_FUNC_2(v1, x)			((double) v1 * 1.83034 * pow (0.162931, (double) v1 * (double) x))

/*
 * DEFAULT DEFINES
 */
#define BASELINE_DEF			1900
#define EQUILIBRIUM_DEF			5
#define THRESHOLD_DEF			600
#define	LEEWAY_DEF				50
#define SAMPLE_SIZE_DEF			30
#define SAMPLE_FUNC_SWITCH_DEF	7

/*
 * STRUCTS
 */

enum STATUS { FAILED, PASSED };

typedef struct {
	uint16_t procsd_val;
	uint16_t* raw_buffer;
	uint32_t buffer_size;
} adc_input_t;

typedef struct {
	ADC_HandleTypeDef* adc_handle;
	DMA_HandleTypeDef* dma_handle;
	uint16_t baseline;
	uint16_t threshold;
	uint16_t equilibrium;
	uint16_t leeway;
	uint16_t entry_value;
	uint8_t sample_size;
	uint8_t sample_func_switch;
	uint8_t current_sample;
	adc_input_t input;
	bool click_checking;
	bool failed;
	bool click;

	uint8_t failure;

//	double switch_freq;
	double v1_scaling;
} envelope_t;

/*
 * INITIALIZATION FUNCTION
 */

bool Envelope_Init (envelope_t* device, ADC_HandleTypeDef* adc_handle, DMA_HandleTypeDef* dma_handle);

/*
 * HELPER FUNCTIONS
 */

void Envelope_Set_Sample_Size (envelope_t* device);
void Envelope_Set_Switch_Frequency (envelope_t* device);
enum STATUS Envelope_Formula_Check (envelope_t* device);
bool Envelope_Read_Click_Status (envelope_t* device);
void Envelope_Clear_Click_Status (envelope_t* device);

#endif /* INC_ENVELOPE_H_ */
