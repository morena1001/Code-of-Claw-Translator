/*
 * envelope.c
 *
 *  Created on: Apr 22, 2026
 *      Author: josue
 */

#include "envelope.h"
#include "stdlib.h"

/*
 * @brief	Initialize an instance of the envelope struct with default values
 * @param	device		A user declared instance of the envelope struct
 * @param	adc_handle	The adc handle used to read the amplitude of the microphone
 * @param	dma_handle	The dma handle used to store the amplitude of the microphone
 * @retval 	boolean
 */
bool Envelope_Init (envelope_t* device, ADC_HandleTypeDef* adc_handle, DMA_HandleTypeDef* dma_handle) {
	device->adc_handle = adc_handle;
	device->dma_handle = dma_handle;

	device->baseline 			= 1900;
	device->threshold 			= 600;
	device->equilibrium 		= 75;
	device->quiet_sample_size	= 20;
	device->loud_sample_size 	= 30;
	device->current_sample 		= 0;
	device->is_loud_input 		= false;
	device->click_checking 		= false;
	device->click				= false;

	device->input.procsd_val 	= 1;
	device->input.buffer_size	= 1;
	device->input.raw_buffer 	= (uint16_t*) malloc (sizeof (uint16_t) * device->input.buffer_size);

	return true;
}

/*
 * @brief	Provides a volatile, single use read of the click register on the struct by
 * 				clearing it afterward
 * @param	device		A user declared instance of the envelope struct
 * @retval 	value of click register on struct
 */
bool Envelope_Read_Click_Status (envelope_t* device) {
	bool val = device->click;
	device->click = false;
	return val;
}

/*
 * @brief	Clears the value of the click register
 * @param	device		A user declared instance of the envelope struct
 */
void Envelope_Clear_Click_Status (envelope_t* device) {
	device->click = false;
}
