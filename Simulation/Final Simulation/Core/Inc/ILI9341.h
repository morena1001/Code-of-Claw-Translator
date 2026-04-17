/*
 * ILI9341.h
 *
 *  Created on: Apr 16, 2026
 *      Author: josue
 */

#ifndef INC_ILI9341_H_
#define INC_ILI9341_H_

#include "stm32f3xx_hal.h"
#include "stdbool.h"

/*
 * MISC
 */

#define COL_SIZE			7
#define ROW_SIZE 			5
#define CHAR_SPACE			2
#define SCREEN_WIDTH		240
#define SCREEN_HEIGHT		320
#define CHAR_COL_LENGTH		13
#define CHAR_ROW_LENGTH		14
#define DEF_CHAR_SIZE		3

#define X_LEFT_PADDING		1
#define Y_TOP_PADDING		11
#define WINDOW_WIDTH		17
#define WINDOW_HEIGHT		23

#define COLOR_WHITE		0xFFFFFF
#define COLOR_BLACK		0x000000
#define COLOR_RED		0xFF0000
#define COLOR_GREEN		0x00FF00
#define COLOR_BLUE		0x0000FF
#define COLOR_YELLOW	0xFFFF00

#define PIXELS_A	(uint8_t[]) { 0x0E, 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11 }
#define PIXELS_B	(uint8_t[]) { 0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E }
#define PIXELS_C	(uint8_t[]) { 0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E }
#define PIXELS_D	(uint8_t[]) { 0x1C, 0x12, 0x11, 0x11, 0x11, 0x12, 0x1C }
#define PIXELS_E	(uint8_t[]) { 0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F }
#define PIXELS_F	(uint8_t[]) { 0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10 }
#define PIXELS_G	(uint8_t[]) { 0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F }
#define PIXELS_H	(uint8_t[]) { 0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11 }
#define PIXELS_I	(uint8_t[]) { 0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E }
#define PIXELS_J	(uint8_t[]) { 0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C }
#define PIXELS_K	(uint8_t[]) { 0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11 }
#define PIXELS_L	(uint8_t[]) { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F }
#define PIXELS_M	(uint8_t[]) { 0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11 }
#define PIXELS_N	(uint8_t[]) { 0x11, 0x11, 0x19, 0x15, 0x13, 0x11, 0x11 }
#define PIXELS_O	(uint8_t[]) { 0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E }
#define PIXELS_P	(uint8_t[]) { 0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10 }
#define PIXELS_Q	(uint8_t[]) { 0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D }
#define PIXELS_R	(uint8_t[]) { 0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11 }
#define PIXELS_S	(uint8_t[]) { 0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E }
#define PIXELS_T	(uint8_t[]) { 0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 }
#define PIXELS_U	(uint8_t[]) { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E }
#define PIXELS_V	(uint8_t[]) { 0x11, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x04 }
#define PIXELS_W	(uint8_t[]) { 0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0A }
#define PIXELS_X	(uint8_t[]) { 0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11 }
#define PIXELS_Y	(uint8_t[]) { 0x11, 0x11, 0x11, 0x0A, 0x04, 0x04, 0x04 }
#define PIXELS_Z	(uint8_t[]) { 0x1F, 0x01, 0x02, 0x04, 0x08, 0x10, 0x1F }
#define PIXELS_PRD	(uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C }
#define PIXELS_SPC	(uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define PIXELS_CRS	(uint8_t[]) { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F }

#define GET_PIXELS(let)	(let == 'A' ? PIXELS_A : (let == 'B' ? PIXELS_B : (let == 'C' ? PIXELS_C : (let == 'D' ? PIXELS_D : (let == 'E' ? PIXELS_E : (let == 'F' ? PIXELS_F : (let == 'G' ? PIXELS_G : (let == 'H' ? PIXELS_H : (let == 'I' ? PIXELS_I : (let == 'J' ? PIXELS_J : (let == 'K' ? PIXELS_K : (let == 'L' ? PIXELS_L : (let == 'M' ? PIXELS_M : (let == 'N' ? PIXELS_N : (let == 'O' ? PIXELS_O : (let == 'P' ? PIXELS_P : (let == 'Q' ? PIXELS_Q : (let == 'R' ? PIXELS_R : (let == 'S' ? PIXELS_S : (let == 'T' ? PIXELS_T : (let == 'U' ? PIXELS_U : (let == 'V' ? PIXELS_V : (let == 'W' ? PIXELS_W : (let == 'X' ? PIXELS_X : (let == 'Y' ? PIXELS_Y : (let == 'Z' ? PIXELS_Z : (let == '.' ? PIXELS_PRD : (let == ' ' ? PIXELS_SPC : PIXELS_SPC))))))))))))))))))))))))))))

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
	uint8_t char_size;
	char** characters;
	uint32_t bg_color, char_color, cursor_color;
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
HAL_StatusTypeDef ILI9341_Set_Window_Location_Size (ILI9341_t* device, uint16_t x_start, uint16_t width, uint16_t y_start, uint16_t height);
//HAL_StatusTypeDef ILI9341_Set_Rotation LOOK FOR THE COMMAND FOR THIS
HAL_StatusTypeDef ILI9341_Write_Pixel (ILI9341_t* device, uint32_t color_value);
HAL_StatusTypeDef ILI9341_Write_Pixels (ILI9341_t* device, uint32_t* color_values, uint16_t length);
void ILI9341_Change_Font_Size (ILI9341_t* device, uint8_t size);
HAL_StatusTypeDef ILI9341_Write_Character (ILI9341_t* device, char letter);
HAL_StatusTypeDef ILI9341_Rewrite_Character (ILI9341_t* device, char letter);
HAL_StatusTypeDef ILI9341_Delete_Character (ILI9341_t* device);
HAL_StatusTypeDef ILI9341_Write_String (ILI9341_t* device, char* string);
HAL_StatusTypeDef ILI9341_Fill_Screen (ILI9341_t* device, uint32_t color_value);
HAL_StatusTypeDef ILI9341_Update_Cursor (ILI9341_t* device);
void ILI9341_Increment_Char_Pos (ILI9341_t* device);
HAL_StatusTypeDef ILI9341_Clear_Cursor (ILI9341_t* device);

/*
 * LOW-LEVEL FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Transmit_Cmd (ILI9341_t* device, uint8_t cmd, bool cs_set_low);
HAL_StatusTypeDef ILI9341_Transmit_Data (ILI9341_t* device, uint8_t* data, uint8_t length, bool cs_set_low);

#endif /* INC_ILI9341_H_ */
