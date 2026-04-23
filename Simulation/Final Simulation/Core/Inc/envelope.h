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
 * ENVELOPE FUNCTIONS
 */

#define QUIET_FUNC(x) (1900 + (1097 * 1.00262 * pow (0.226198, (double) x / 10)))
#define LOUD_FUNC(x) (1900 + (2195 * 1.00262 * pow (0.371244, (double) x / 10)))

/*
 * STRUCTS
 */

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
	uint8_t quiet_sample_size;
	uint8_t loud_sample_size;
	uint8_t current_sample;
	adc_input_t input;
	bool is_loud_input;
	bool click_checking;
	bool click;
} envelope_t;

/*
 * INITIALIZATION FUNCTION
 */

bool Envelope_Init (envelope_t* device, ADC_HandleTypeDef* adc_handle, DMA_HandleTypeDef* dma_handle);

/*
 * HELPER FUNCTIONS
 */

bool Envelope_Read_Click_Status (envelope_t* device);
void Envelope_Clear_Click_Status (envelope_t* device);

#endif /* INC_ENVELOPE_H_ */
