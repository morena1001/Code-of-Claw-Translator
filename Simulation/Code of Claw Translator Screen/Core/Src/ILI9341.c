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

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->rs_port, device->rs_pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin (device->rs_port, device->rs_pin, GPIO_PIN_SET);

    ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_SWRESET, true);
    HAL_Delay (1000);

    ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_MADCTL, true); {
    	uint8_t data[] = { 0x40 };
    	ILI9341_Transmit_Data(device, data, sizeof (data), true);
    }

    ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_ENABLE_3G, true); {
    	uint8_t data[] = { 0x03 };
    	ILI9341_Transmit_Data(device, data, sizeof (data), true);
    }

    ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_PGAMCTRL, true); {
    	uint8_t data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  				 	 	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    	ILI9341_Transmit_Data(device, data, sizeof (data), true);
    }

    ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_NGAMCTRL, true); {
    	uint8_t data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  				 	 	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    	ILI9341_Transmit_Data(device, data, sizeof (data), true);
    }

    ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_SLPOUT, true);
    ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_DISPON, true);

    HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);

//	return status;
	return HAL_OK;
}

/*
 * HELPER FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Reset (ILI9341_t* device) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	uint8_t cmd = ILI9341_COMMAND_SWRESET;
	HAL_StatusTypeDef status = HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	HAL_Delay (1000);
	return status;
}

HAL_StatusTypeDef ILI9341_Set_Window_Location (ILI9341_t* device, uint16_t x_left, uint16_t x_right, uint16_t y_up, uint16_t y_down) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	{
		uint8_t cmd = ILI9341_COMMAND_CASET;
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);
		uint8_t data[] = { (x_left >> 8) & 0xFF, x_left & 0xFF, (x_right >> 8) & 0xFF, x_right & 0xFF };
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
		HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);
	}
	{
		uint8_t cmd = ILI9341_COMMAND_PASET;
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);
		uint8_t data[] = { (y_up >> 8) & 0xFF, y_up & 0xFF, (y_down >> 8) & 0xFF, y_down & 0xFF };
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
		HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);
	}

	device->x_pos = x_left;
	device->y_pos = y_up;
	device->win_s_x = x_left;
	device->win_e_x = x_right;
	device->win_s_y = y_up;
	device->win_e_y = y_down;

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return HAL_OK;
}

//HAL_StatusTypeDef ILI9341_Set_Rotation LOOK FOR THE COMMAND FOR THIS

HAL_StatusTypeDef ILI9341_Write_Pixel (ILI9341_t* device, uint32_t color_value) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_RAMWR, true);
	uint8_t data[] = { (color_value >> 16) & 0xFC, (color_value >> 8) & 0xFC, color_value & 0xFC };
	ILI9341_Transmit_Data (device, data, sizeof (data), true);

//	uint8_t cmd = ILI9341_COMMAND_RAMWR;
//	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
//	HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);
//	uint8_t data[] = { (color_value >> 16) & 0xFC, (color_value >> 8) & 0xFC, color_value & 0xFC };
//	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
//	HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Write_Pixels (ILI9341_t* device, uint32_t* color_values, uint8_t length) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_RAMWR, true); // PUT THIS INTO THE LOOP
	uint8_t data[3] = { 0 };
	data[0] = (color_values[0] >> 16) & 0xFC;
	data[1] = (color_values[0] >> 8)  & 0xFC;
	data[2] = (color_values[0])       & 0xFC;
	ILI9341_Transmit_Data (device, data, sizeof (data), true);

	for (uint8_t i = 1; i < length; i++) {
		ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_W_M_C, true);
		data[0] = (color_values[i] >> 16) & 0xFC;
		data[1] = (color_values[i] >> 8)  & 0xFC;
		data[2] = (color_values[i])       & 0xFC;
		ILI9341_Transmit_Data (device, data, sizeof (data), true);
	}

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Write_Character (ILI9341_t* device, char character) {
	return HAL_OK;
}


/*
 * LOW-LEVEL FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Transmit_Cmd (ILI9341_t* device, uint8_t cmd, bool cs_set_low) {
	if (!cs_set_low)	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);

	return HAL_OK;
}
HAL_StatusTypeDef ILI9341_Transmit_Data (ILI9341_t* device, uint8_t* data, uint8_t length, bool cs_set_low) {
	if (!cs_set_low)	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
	HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);

	return HAL_OK;
}

