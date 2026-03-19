/*
 * ILI9341.h
 *
 *  Created on: Mar 8, 2026
 *      Author: josue
 */

#ifndef INC_ILI9341_H_
#define INC_ILI9341_H_

#include "stm32f3xx_hal.h"
#include "stdbool.h"

/*
 * MISC
 */

#define COLOR_WHITE		0x000000
#define COLOR_BLACK		0xFFFFFF
#define COLOR_RED		0xFF0000
#define COLOR_GREEN		0x00FF00
#define COLOR_BLUE		0x0000FF

/*
 * REGISTERS
 */

#define ILI9341_COMMAND_SWRESET			0x01
#define ILI9341_COMMAND_SLPOUT			0x11
#define ILI9341_COMMAND_DISPON			0x29
#define ILI9341_COMMAND_CASET			0x2A
#define ILI9341_COMMAND_PASET			0x2B
#define ILI9341_COMMAND_RAMWR			0x2C
#define ILI9341_COMMAND_MADCTL			0x36
#define ILI9341_COMMAND_W_M_C			0x3C
#define ILI9341_COMMAND_PGAMCTRL		0xE0
#define ILI9341_COMMAND_NGAMCTRL		0xE1
#define ILI9341_COMMAND_ENABLE_3G		0xF2


/*
 * STRUCT
 */

typedef struct {
	SPI_HandleTypeDef* spi_handle;
	GPIO_TypeDef* cs_port,* rs_port,* dc_port;
	uint16_t cs_pin, rs_pin, dc_pin;
	uint16_t x_pos, y_pos;
	uint16_t win_s_x, win_e_x, win_s_y, win_e_y;
} ILI9341_t;

/*
 * INITIALIZATION FUNCTION
 */

HAL_StatusTypeDef ILI9341_Init (ILI9341_t* device, SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* rs_port, uint16_t rs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin);

/*
 * HELPER FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Reset (ILI9341_t* device);
HAL_StatusTypeDef ILI9341_Set_Window_Location (ILI9341_t* device, uint16_t x_left, uint16_t x_right, uint16_t y_up, uint16_t y_down);
//HAL_StatusTypeDef ILI9341_Set_Rotation LOOK FOR THE COMMAND FOR THIS
HAL_StatusTypeDef ILI9341_Write_Pixel (ILI9341_t* device, uint32_t color_value);
HAL_StatusTypeDef ILI9341_Write_Pixels (ILI9341_t* device, uint32_t* color_values, uint8_t length);
HAL_StatusTypeDef ILI9341_Write_Character (ILI9341_t* device, char character);


/*
 * LOW-LEVEL FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Transmit_Cmd (ILI9341_t* device, uint8_t cmd, bool cs_set_low);
HAL_StatusTypeDef ILI9341_Transmit_Data (ILI9341_t* device, uint8_t* data, uint8_t length, bool cs_set_low);

#endif /* INC_ILI9341_H_ */
