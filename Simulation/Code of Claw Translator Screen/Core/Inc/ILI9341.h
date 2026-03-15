/*
 * ILI9341.h
 *
 *  Created on: Mar 8, 2026
 *      Author: josue
 */

#ifndef INC_ILI9341_H_
#define INC_ILI9341_H_

#include "stm32f3xx_hal.h"

/*
 * MISC
 */

#define COLOR_WHITE		0x000000
#define COLOR_BLACK		0xFFFFFF
#define COLOR_RED		0xFF0000
#define COLOR_GREEN		0x00FF00
#define COLOR_BLUE		0x0000FF

enum rw_length { _8_BITS = 1, _16_BITS = 2, _24_BITS = 4, _32_BITS = 5 };

/*
 * REGISTERS
 */

#define ILI9341_COMMAND_NOP						0x00
#define ILI9341_COMMAND_RESET					0x01
#define ILI9341_COMMAND_READ_ID					0x04
#define ILI9341_COMMAND_READ_DISPLAY_STATUS		0x09
#define ILI9341_COMMAND_READ_POWER_MODE			0x0A
#define ILI9341_COMMAND_SLEEP_OUT				0x11
#define ILI9341_COMMAND_DISPLAY_OFF				0x28
#define ILI9341_COMMAND_DISPLAY_ON				0x29
#define ILI9341_COMMAND_COLOR_SET				0x2D
#define ILI9341_COMMAND_COLUMN_ADDRESS_SET		0x2A
#define ILI9341_COMMAND_PAGE_ADDRESS_SET		0x2B
#define ILI9341_COMMAND_MEMORY_WRITE			0x2C
#define ILI9341_COMMAND_COLOR_SET				0x2D

/*
 * STRUCT
 */

typedef struct {
	SPI_HandleTypeDef* spi_handle;
	GPIO_TypeDef* cs_port,* rs_port,* dc_port;
	uint16_t cs_pin, rs_pin, dc_pin;
} ILI9341_t;

/*
 * INITIALIZATION FUNCTION
 */

HAL_StatusTypeDef ILI9341_Init (ILI9341_t* device, SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* rs_port, uint16_t rs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin);

/*
 * HELPER FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Reset (ILI9341_t* device);
HAL_StatusTypeDef ILI9341_Sleep_Out (ILI9341_t* device);
HAL_StatusTypeDef ILI9341_Display_On (ILI9341_t* device);
HAL_StatusTypeDef ILI9341_Set_Column_Address (ILI9341_t* device, uint16_t start_column_address, uint16_t end_column_address);
HAL_StatusTypeDef ILI9341_Set_Page_Address (ILI9341_t* device, uint16_t start_page_address, uint16_t end_page_address);
HAL_StatusTypeDef ILI9341_Write_Pixel (ILI9341_t* device, uint32_t color_value);


/*
 * LOW-LEVEL FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Transmit (ILI9341_t* device, uint8_t command);
HAL_StatusTypeDef ILI9341_Transmit_Data (ILI9341_t* device, uint8_t command, enum rw_length length, uint8_t* parameters);
HAL_StatusTypeDef ILI9341_Transmit_Irregular_Data (ILI9341_t* device, uint8_t command, uint8_t param_length, uint8_t* parameters);

HAL_StatusTypeDef ILI9341_Receive (ILI9341_t* device, enum rw_length length, uint8_t command, uint8_t* read_buffer);


//HAL_StatusTypeDef ILI9341_Receive (ILI9341_t* device, uint8_t com, uint8_t* data);
//HAL_StatusTypeDef ILI9341_Transmit_Receive (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data);
//
//HAL_StatusTypeDef ILI9341_Transmit_Multiple (ILI9341_t* device, uint8_t com, uint8_t* data, uint8_t length);
//HAL_StatusTypeDef ILI9341_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* data, uint8_t length);
//HAL_StatusTypeDef ILI9341_Transmit_Multiple_Receive (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t t_length);
//HAL_StatusTypeDef ILI9341_Transmit_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t r_length);
//HAL_StatusTypeDef ILI9341_Transmit_Multiple_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t t_length, uint8_t r_length);

#endif /* INC_ILI9341_H_ */
