/*
 * ILI9341.c
 *
 *  Created on: Mar 8, 2026
 *      Author: josue
 */

#include "ILI9341.h"
#include "stdlib.h"

/*
 * INITIALIZATION FUNCTION
 */

HAL_StatusTypeDef ILI9341_Init (ILI9341_t* device, SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* rs_port, uint16_t rs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin) {
	device->spi_handle = spi_handle;
	device->cs_port = cs_port;
	device->cs_pin = cs_pin;
	device->rs_port = rs_port;
	device->rs_pin = rs_pin;
	device->dc_port = dc_port;
	device->dc_pin = dc_pin;

//	HAL_StatusTypeDef status = HAL_ERROR;

	// Read device ID information to make sure connection is made
//	uint8_t data_buffer[2] = { 0 };
//	if( ILI9341_Receive (device, _16_BITS, ILI9341_COMMAND_READ_POWER_MODE, data_buffer) != HAL_OK)	return HAL_ERROR;
//	uint8_t single = 0x04;
//	uint8_t tx_data[5] = { 0x04, 0, 0, 0, 0} ;
//	uint8_t rx_data[5] = { 0 };
//	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin (device->rs_port, device->rs_pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit (device->spi_handle, &single, 1, 100);
//	HAL_SPI_Transmit (device->spi_handle, tx_data, 2, 100);
//	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
//	HAL_SPI_TransmitReceive (device->spi_handle, rx_data, rx_data, 1, 100);
//	HAL_SPI_Receive (device->spi_handle, rx_data, 4, 100);
//	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);


//	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
//	HAL_SPI_TransmitReceive (device->spi_handle, tx_data, rx_data, 5, 100);
//	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
//	for (uint8_t i = 1; i < 4; i++)	{
//		if (data_buffer[i] == 0)	return HAL_ERROR;
//	}

	// Reset device commands and parameters
//	if (ILI9341_Reset (device) != HAL_OK)	return HAL_ERROR;
//	HAL_Delay (5);
//
//	if (ILI9341_Sleep_Out (device) != HAL_OK)	return HAL_ERROR;
//	HAL_Delay (5);
//
//	 if (ILI9341_Display_On (device) != HAL_OK)	return HAL_ERROR;
//
//	// set block address
//	ILI9341_Set_Column_Address (device, 0x00A0, 0x00A9);
//	ILI9341_Set_Page_Address (device, 0x00A0, 0x00AF);
//	ILI9341_Write_Pixel (device, COLOR_BLUE);
//	ILI9341_Transmit (device, ILI9341_COMMAND_NOP);
//
//	uint8_t data_buffer_2[5] = {0};
//	status = ILI9341_Receive (device, _32_BITS, ILI9341_COMMAND_READ_DISPLAY_STATUS, data_buffer_2);
//
//	uint8_t data_buffer_3[2] = {0};
//	status = ILI9341_Receive (device, _16_BITS, ILI9341_COMMAND_READ_POWER_MODE, data_buffer_3);

	return HAL_OK;
}

/*
 * HELPER FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Reset (ILI9341_t* device) {
	return ILI9341_Transmit (device, ILI9341_COMMAND_RESET);
}

HAL_StatusTypeDef ILI9341_Sleep_Out (ILI9341_t* device) {
	return ILI9341_Transmit (device, ILI9341_COMMAND_SLEEP_OUT);
}

HAL_StatusTypeDef ILI9341_Display_On (ILI9341_t* device) {
	return ILI9341_Transmit (device, ILI9341_COMMAND_DISPLAY_ON);
}

HAL_StatusTypeDef ILI9341_Set_Column_Address (ILI9341_t* device, uint16_t start_column_address, uint16_t end_column_address) {
	uint8_t* parameters = malloc (sizeof (uint8_t) * 4);
	parameters[0] = (uint8_t) (start_column_address >> 8);
	parameters[1] = (uint8_t) (start_column_address & 0x00FF);
	parameters[2] = (uint8_t) (end_column_address >> 8);
	parameters[3] = (uint8_t) (end_column_address & 0x00FF);

	return ILI9341_Transmit_Irregular_Data (device, ILI9341_COMMAND_COLUMN_ADDRESS_SET, 4, parameters);
}

HAL_StatusTypeDef ILI9341_Set_Page_Address (ILI9341_t* device, uint16_t start_page_address, uint16_t end_page_address) {
	uint8_t* parameters = malloc (sizeof (uint8_t) * 4);
	parameters[0] = (uint8_t) (start_page_address >> 8);
	parameters[1] = (uint8_t) (start_page_address & 0xFF);
	parameters[2] = (uint8_t) (end_page_address >> 8);
	parameters[3] = (uint8_t) (end_page_address & 0xFF);

	return ILI9341_Transmit_Irregular_Data (device, ILI9341_COMMAND_PAGE_ADDRESS_SET, 4, parameters);
}

HAL_StatusTypeDef ILI9341_Write_Pixel (ILI9341_t* device, uint32_t color_value) {
	uint8_t* parameters = malloc (sizeof (uint8_t) * 3);
	parameters[1] = (uint8_t) ((color_value >> 16) & 0xFF);
	parameters[2] = (uint8_t) ((color_value >> 8) & 0xFF);
	parameters[3] = (uint8_t) (color_value & 0xFF);

	return ILI9341_Transmit_Irregular_Data (device, ILI9341_COMMAND_MEMORY_WRITE, 3, parameters);
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

HAL_StatusTypeDef ILI9341_Transmit_Data (ILI9341_t* device, uint8_t command, enum rw_length length, uint8_t* parameters) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit (device->spi_handle, &command, 1, 100);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
	status = HAL_SPI_Transmit (device->spi_handle, parameters, length, 100);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return status;
}

HAL_StatusTypeDef ILI9341_Transmit_Irregular_Data (ILI9341_t* device, uint8_t command, uint8_t param_length, uint8_t* parameters) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Transmit (device->spi_handle, &command, 1, 100);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
	status = HAL_SPI_Transmit (device->spi_handle, parameters, param_length, 100);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return status;
}

HAL_StatusTypeDef ILI9341_Receive (ILI9341_t* device, enum rw_length length, uint8_t command, uint8_t* read_buffer) {
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
