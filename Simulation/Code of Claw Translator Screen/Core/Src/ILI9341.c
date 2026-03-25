/*
 * ILI9341.c
 *
 *  Created on: Mar 8, 2026
 *      Author: josue
 */

#include "ILI9341.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

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

	// Initialize the character font size and cursor position
	device->char_size = DEF_CHAR_SIZE;
	device->x_pos = 0;
	device->y_pos = 0;

	// Initialize default colors for bg and char
	device->bg_color = COLOR_WHITE;
	device->char_color = COLOR_BLACK;
	device->cursor_color = COLOR_YELLOW;

	device->characters = malloc (sizeof (char*) * CHAR_COL_LENGTH);
	for (uint8_t i = 0; i < CHAR_COL_LENGTH; i++)		device->characters[i] = malloc (sizeof (char) * CHAR_ROW_LENGTH);

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

    // Set bg to background color
    ILI9341_Fill_Screen(device, device->bg_color);

    // Set window location for the cursor to print characters
    ILI9341_Set_Window_Location (device, 0x0001, 0x0001 + (ROW_SIZE * DEF_CHAR_SIZE) - 1, 0x000B, 0x000B + (COL_SIZE * DEF_CHAR_SIZE) - 1);

    // Set cursor
    ILI9341_Update_Cursor (device);
    ILI9341_Set_Window_Location (device, 0x0001, 0x0001 + (ROW_SIZE * DEF_CHAR_SIZE) - 1, 0x000B, 0x000B + (COL_SIZE * DEF_CHAR_SIZE) - 1);

//	return status;
	return HAL_OK;
}

/*
 * HELPER FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Reset (ILI9341_t* device) {
	// Reset screen IC
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
	uint8_t cmd = ILI9341_COMMAND_SWRESET;
	HAL_StatusTypeDef status = HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	HAL_Delay (1000);

	// Reset cursor position
	device->x_pos = 0;
	device->y_pos = 0;
	ILI9341_Set_Window_Location (device, 0x0001, 0x0001 + (ROW_SIZE * device->char_size) - 1, 0x000B, 0x000B + (COL_SIZE * device->char_size) - 1);

	// Reset characters in track array
	for (uint8_t i = 0; i < CHAR_COL_LENGTH; i++)		memset (device->characters[i], ' ', sizeof (char) * CHAR_ROW_LENGTH);

	// Reset screen
	ILI9341_Fill_Screen (device, device->bg_color);

	// Reset cursor
    ILI9341_Update_Cursor (device);
	return status;
}

HAL_StatusTypeDef ILI9341_Set_Window_Location (ILI9341_t* device, uint16_t x_left, uint16_t x_right, uint16_t y_up, uint16_t y_down) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	device->win_s_x = x_left;
	device->win_e_x = x_right;
	device->win_s_y = y_up;
	device->win_e_y = y_down;

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

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Set_Window_Location_Size (ILI9341_t* device, uint16_t x_start, uint16_t width, uint16_t y_start, uint16_t height) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	device->win_s_x = x_start;
	device->win_e_x = x_start + width;
	device->win_s_y = y_start;
	device->win_e_y = y_start + height;

	{
		uint8_t cmd = ILI9341_COMMAND_CASET;
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);
		uint8_t data[] = { (device->win_s_x >> 8) & 0xFF, device->win_s_x & 0xFF, (device->win_e_x >> 8) & 0xFF, device->win_e_x & 0xFF };
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
		HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);
	}
	{
		uint8_t cmd = ILI9341_COMMAND_PASET;
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit (device->spi_handle, &cmd, sizeof (cmd), 100);
		uint8_t data[] = { (device->win_s_y >> 8) & 0xFF, device->win_s_y & 0xFF, (device->win_e_y >> 8) & 0xFF, device->win_e_y & 0xFF };
		HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
		HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);
	}

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return HAL_OK;
}

//HAL_StatusTypeDef ILI9341_Set_Rotation LOOK FOR THE COMMAND FOR THIS

HAL_StatusTypeDef ILI9341_Write_Pixel (ILI9341_t* device, uint32_t color_value) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_RAMWR, true);
	uint8_t data[] = { (color_value >> 16) & 0xFC, (color_value >> 8) & 0xFC, color_value & 0xFC };
	ILI9341_Transmit_Data (device, data, sizeof (data), true);

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Write_Pixels (ILI9341_t* device, uint32_t* color_values, uint16_t length) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_RAMWR, true); // PUT THIS INTO THE LOOP
	uint8_t data[3] = { 0 };
	data[0] = (color_values[0] >> 16) & 0xFC;
	data[1] = (color_values[0] >> 8)  & 0xFC;
	data[2] = (color_values[0])       & 0xFC;
	ILI9341_Transmit_Data (device, data, sizeof (data), true);

	for (uint16_t i = 1; i < length; i++) {
		ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_W_M_C, true);
		data[0] = (color_values[i] >> 16) & 0xFC;
		data[1] = (color_values[i] >> 8)  & 0xFC;
		data[2] = (color_values[i])       & 0xFC;
		ILI9341_Transmit_Data (device, data, sizeof (data), true);
	}

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);
	return HAL_OK;
}

void ILI9341_Change_Font_Size (ILI9341_t* device, uint8_t size) {
	device->char_size = size;
}

HAL_StatusTypeDef ILI9341_Write_Character (ILI9341_t* device, char letter) {
	// Allocate the memory to send all of the data for a character at the same time
    uint32_t* colors = malloc (sizeof (uint32_t) * ROW_SIZE * device->char_size * COL_SIZE * device->char_size);

	// Grab the pixel format at size 1 for a given character
	uint8_t format[COL_SIZE];
	if (letter > 96)	letter -= 32; // Make all letters capitalized
	memcpy (format, GET_PIXELS(letter), sizeof (uint8_t) * COL_SIZE);

	// Add color to the pixel format and copy it to the big array
	for (uint8_t i = 0; i < COL_SIZE * device->char_size; i++) {
		for (uint8_t j = ROW_SIZE * device->char_size; j > 0; j--) {
			uint8_t c = ceil (i / device->char_size);
			uint8_t d = ceil ((float) j / (float) device->char_size) - 1;

			if ((format[c] >> d) & 1)   colors[((i + 1) * ROW_SIZE * device->char_size) - j] = device->char_color;
			else                        colors[((i + 1) * ROW_SIZE * device->char_size) - j] = device->bg_color;
		}
	}

	ILI9341_Write_Pixels (device, colors, ROW_SIZE * device->char_size * COL_SIZE * device->char_size);
	free (colors);

	// Add character to array
	device->characters[device->y_pos][device->x_pos] = letter;

	// Update character position
	if (device->x_pos == CHAR_ROW_LENGTH - 1) {
		device->x_pos = 0;
		if (device->y_pos == CHAR_COL_LENGTH)	device->y_pos = 0;
		else	device->y_pos++;
	} else		device->x_pos++;

	// Update character drawing window according to position
	ILI9341_Set_Window_Location_Size (device, (device->x_pos * WINDOW_WIDTH) + X_LEFT_PADDING, (ROW_SIZE * device->char_size) - 1, (device->y_pos * WINDOW_HEIGHT) + Y_TOP_PADDING, (COL_SIZE * device->char_size) - 1);

	// Update cursor
	ILI9341_Update_Cursor (device);

	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Rewrite_Character (ILI9341_t* device, char letter) {
	// Allocate the memory to send all of the data for a character at the same time
    uint32_t* colors = malloc (sizeof (uint32_t) * ROW_SIZE * device->char_size * COL_SIZE * device->char_size);

	// Grab the pixel format at size 1 for a given character
	uint8_t format[COL_SIZE];
	if (letter > 96)	letter -= 32; // Make all letters capitalized
	memcpy (format, GET_PIXELS(letter), sizeof (uint8_t) * COL_SIZE);

	// Add color to the pixel format and copy it to the big array
	for (uint8_t i = 0; i < COL_SIZE * device->char_size; i++) {
		for (uint8_t j = ROW_SIZE * device->char_size; j > 0; j--) {
			uint8_t c = ceil (i / device->char_size);
			uint8_t d = ceil ((float) j / (float) device->char_size) - 1;

			if ((format[c] >> d) & 1)   colors[((i + 1) * ROW_SIZE * device->char_size) - j] = device->char_color;
			else                        colors[((i + 1) * ROW_SIZE * device->char_size) - j] = device->bg_color;
		}
	}

	ILI9341_Write_Pixels (device, colors, ROW_SIZE * device->char_size * COL_SIZE * device->char_size);
	free (colors);

	// Change character in the array at the same position
	device->characters[device->y_pos][device->x_pos] = ' ';

	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Delete_Character (ILI9341_t* device) {
	// Update character position
	if (device->x_pos == 0) {
		device->x_pos = CHAR_ROW_LENGTH - 1;
		if (device->y_pos == 0)	device->y_pos = CHAR_COL_LENGTH - 1;
		else	device->y_pos--;
	} else		device->x_pos--;

	// Update window location
	ILI9341_Set_Window_Location_Size (device, (device->x_pos * WINDOW_WIDTH) + X_LEFT_PADDING, (ROW_SIZE * device->char_size) - 1, (device->y_pos * WINDOW_HEIGHT) + Y_TOP_PADDING, (COL_SIZE * device->char_size) - 1);

	// Remove character from screen
	ILI9341_Rewrite_Character(device, ' ');

	// Update cursor
	ILI9341_Update_Cursor (device);

	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Write_String (ILI9341_t* device, char* string) {
	uint8_t i = 0;
	while (string[i] != '\0' && string[i] != '\n')
		ILI9341_Write_Character(device, string[i++]);

	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Fill_Screen (ILI9341_t* device, uint32_t color_value) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	uint16_t prev_x_left = device->win_s_x;
	uint16_t prev_x_right = device->win_e_x;
	uint16_t prev_y_up = device->win_s_y;
	uint16_t prev_y_down = device->win_e_y;

	ILI9341_Set_Window_Location (device, 0x0000, SCREEN_WIDTH - 1, 0x0000, SCREEN_HEIGHT - 1);

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_RAMWR, true); // PUT THIS INTO THE LOOP
	uint8_t data[3] = { (color_value >> 16) & 0xFC, (color_value >> 8)  & 0xFC, color_value  & 0xFC };
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);

	for (uint16_t i = 1; i <= SCREEN_WIDTH; i++) {
		for (uint16_t j = 0; j <= SCREEN_HEIGHT; j++) {
			HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);
		}
	}

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);

	device-> win_s_x = prev_x_left;
	device->win_e_x = prev_x_right;
	device->win_s_y = prev_y_up;
	device->win_e_y = prev_y_down;

	return HAL_OK;
}

HAL_StatusTypeDef ILI9341_Update_Cursor (ILI9341_t* device) {
	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);

	uint16_t prev_x_left = device->win_s_x;
	uint16_t prev_x_right = device->win_e_x;
	uint16_t prev_y_up = device->win_s_y;
	uint16_t prev_y_down = device->win_e_y;

	ILI9341_Set_Window_Location (device, device->win_s_x - 1, device->win_e_x + 1, device->win_e_y + 1, device->win_e_y + 2);

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_RESET);
	ILI9341_Transmit_Cmd (device, ILI9341_COMMAND_RAMWR, true);
	uint8_t data[3] = { (COLOR_YELLOW >> 16) & 0xFC, (COLOR_YELLOW >> 8) & 0xFC, COLOR_YELLOW & 0xFC };
	HAL_GPIO_WritePin (device->dc_port, device->dc_pin, GPIO_PIN_SET);
	for (uint8_t i = 0; i < (device->win_e_x - device->win_s_x); i++) {
		HAL_SPI_Transmit (device->spi_handle, data, sizeof (data), 100);
	}

	HAL_GPIO_WritePin (device->cs_port, device->cs_pin, GPIO_PIN_SET);

	device-> win_s_x = prev_x_left;
	device->win_e_x = prev_x_right;
	device->win_s_y = prev_y_up;
	device->win_e_y = prev_y_down;

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

