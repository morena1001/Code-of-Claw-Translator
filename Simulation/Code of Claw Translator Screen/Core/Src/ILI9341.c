/*
 * ILI9341.c
 *
 *  Created on: Mar 8, 2026
 *      Author: josue
 */

#include "ILI9341.h"

/*
 * INITIALIZATION FUNCTION
 */

//HAL_StatusTypeDef ILI9341_Init (ILI9341_t* device, SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* rs_port, uint16_t rs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin) {
int* ILI9341_Init (ILI9341_t* device, SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* rs_port, uint16_t rs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin) {
	device->spi_handle = spi_handle;
	device->cs_port = cs_port;
	device->cs_pin = cs_pin;
	device->rs_port = rs_port;
	device->rs_pin = rs_pin;
	device->dc_port = dc_port;
	device->dc_pin = dc_pin;

	HAL_StatusTypeDef status = HAL_TIMEOUT;

	// Read device ID information to make sure connection is made
	uint8_t data_buffer[4] = { 0 };
	status = ILI9341_Receive (device, _24_BITS, ILI9341_COMMAND_READ_ID, data_buffer);



	// Reset device commands and parameters
	status = ILI9341_Reset (device);

	return data_buffer;
//	return status;
}

/*
 * HELPER FUNCTIONS
 */
HAL_StatusTypeDef ILI9341_Reset (ILI9341_t* device) {
	return ILI9341_Transmit (device, ILI9341_COMMAND_RESET);
}


/*
 * LOW-LEVEL FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Transmit (ILI9341_t* device, uint8_t command) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit (device->spi_handle, &command, 1, 100);
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return status;
}

HAL_StatusTypeDef ILI9341_Receive (ILI9341_t* device, enum read_length length, uint8_t command, uint8_t* read_buffer) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit (device->spi_handle, &command, 1, 100);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
	status = HAL_SPI_Receive (device->spi_handle, read_buffer, length, 100);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return status;
}

//HAL_StatusTypeDef ILI9341_Receive (ILI9341_t* device, uint8_t com, uint8_t* data) {
//	return HAL_OK;
//}
//
//HAL_StatusTypeDef ILI9341_Transmit_Receive (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data) {
//	return HAL_OK;
//}
//
//HAL_StatusTypeDef ILI9341_Transmit_Multiple (ILI9341_t* device, uint8_t com, uint8_t* data, uint8_t length);
//HAL_StatusTypeDef ILI9341_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* data, uint8_t length);
//HAL_StatusTypeDef ILI9341_Transmit_Multiple_Receive (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t t_length);
//HAL_StatusTypeDef ILI9341_Transmit_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t r_length);
//HAL_StatusTypeDef ILI9341_Transmit_Multiple_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t t_length, uint8_t r_length);
