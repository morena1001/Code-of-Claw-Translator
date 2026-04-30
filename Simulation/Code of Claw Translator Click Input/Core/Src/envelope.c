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

	device->baseline 			= BASELINE_DEF;
	device->threshold 			= THRESHOLD_DEF;
	device->equilibrium 		= EQUILIBRIUM_DEF;
	device->leeway				= LEEWAY_DEF;
	device->entry_value			= 0;
	device->sample_size			= (uint8_t) SAMPLE_SIZE_DEF;
	device->sample_func_switch	= SAMPLE_FUNC_SWITCH_DEF;
	device->current_sample 		= 0;
	device->click_checking 		= false;
	device->failed				= false;
	device->click				= false;
	device->v1_scaling			= 0.0;
	device->failure = 0;

//	device->switch_freq			= 0.0;

	device->input.procsd_val 	= 1;
	device->input.buffer_size	= 1;
	device->input.raw_buffer 	= (uint16_t*) malloc (sizeof (uint16_t) * device->input.buffer_size);

	return true;
}

/*
 * @brief	Calculates how many samples to do based on the dynamic envelope, as well as the
 * 				sample where the envelope switches to the second function and the scaling
 * 				for the envelope function
 * @param	device		A user declared instance of the envelope struct
 */
void Envelope_Set_Sample_Size (envelope_t* device) {
	device->entry_value = device->input.procsd_val;
	device->sample_size = ceil((double) SAMPLE_SIZE_DEF * V2 (device->entry_value));
	device->sample_func_switch = ceil((double) SAMPLE_FUNC_SWITCH_DEF * V2 (device->entry_value));
	device->v1_scaling = V1 (device->entry_value);
}

/*
 * @brief	Calculates the frequency needed to switch from the first envelope function to
 * 				the second envelope function and the scaling for the envelope function
 * @param	device		A user declared instance of the envelope struct
 */
void Envelope_Set_Switch_Frequency (envelope_t* device) {
//	device->v1_scaling = V1 (device->input.procsd_val);
//	device->switch_freq = ENV_FUNC_1 (device->v1_scaling, 0.7 * (double) V2 (device->input.procsd_val));
}

/*
 * @brief	Check if the value in the struct is valid in the envelope based on the current sample
 * @param	device		A user declared instance of the envelope struct
 * @retval 	enum indicating if the click is still within the envelope, has failed, or has passed
 */
enum STATUS Envelope_Formula_Check (envelope_t* device) {

	// First function
//	if (device->input.procsd_val <= device->switch_freq &&
//	    device->input.procsd_val <= ENV_FUNC_1 (device->v1_scaling, device->current_sample) + (double) device->leeway) {
	if (device->current_sample <= device->sample_func_switch &&
	    device->input.procsd_val <= ENV_FUNC_1 (device->entry_value, device->v1_scaling, device->current_sample) + (double) device->leeway) {
//	    device->input.procsd_val <= ENV_FUNC_1 (device->baseline, device->v1_scaling, device->current_sample) + (double) device->leeway) {

//	} else if (device->input.procsd_val > device->switch_freq &&
//			   device->input.procsd_val <= ENV_FUNC_2 (device->v1_scaling, device->current_sample) + (double) device->leeway) {
	} else if (device->current_sample > device->sample_func_switch &&
			   device->input.procsd_val <= ENV_FUNC_2 (device->entry_value, device->v1_scaling, device->current_sample) + (double) device->leeway) {
//			   device->input.procsd_val <= ENV_FUNC_2 (device->baseline, device->v1_scaling, device->current_sample) + (double) device->leeway) {

	} else {
		return FAILED;
	}

	return PASSED;
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
