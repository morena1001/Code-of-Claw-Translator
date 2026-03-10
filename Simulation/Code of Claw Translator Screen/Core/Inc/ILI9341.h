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

enum read_length { _8_BITS = 1, _24_BITS = 4, _32_BITS = 5 };

/*
 * REGISTERS
 */

#define ILI9341_COMMAND_RESET		0x01
#define ILI9341_COMMAND_READ_ID		0x04


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

//HAL_StatusTypeDef ILI9341_Init (ILI9341_t* device, SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* rs_port, uint16_t rs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin);
int* ILI9341_Init (ILI9341_t* device, SPI_HandleTypeDef* spi_handle, GPIO_TypeDef* cs_port, uint16_t cs_pin, GPIO_TypeDef* rs_port, uint16_t rs_pin, GPIO_TypeDef* dc_port, uint16_t dc_pin);

/*
 * HELPER FUNCTIONS
 */
HAL_StatusTypeDef ILI9341_Reset (ILI9341_t* device);


/*
 * LOW-LEVEL FUNCTIONS
 */

HAL_StatusTypeDef ILI9341_Transmit (ILI9341_t* device, uint8_t command);

HAL_StatusTypeDef ILI9341_Receive (ILI9341_t* device, enum read_length length, uint8_t command, uint8_t* read_buffer);


//HAL_StatusTypeDef ILI9341_Receive (ILI9341_t* device, uint8_t com, uint8_t* data);
//HAL_StatusTypeDef ILI9341_Transmit_Receive (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data);
//
//HAL_StatusTypeDef ILI9341_Transmit_Multiple (ILI9341_t* device, uint8_t com, uint8_t* data, uint8_t length);
//HAL_StatusTypeDef ILI9341_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* data, uint8_t length);
//HAL_StatusTypeDef ILI9341_Transmit_Multiple_Receive (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t t_length);
//HAL_StatusTypeDef ILI9341_Transmit_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t r_length);
//HAL_StatusTypeDef ILI9341_Transmit_Multiple_Receive_Multiple (ILI9341_t* device, uint8_t com, uint8_t* t_data, uint8_t* r_data, uint8_t t_length, uint8_t r_length);

#endif /* INC_ILI9341_H_ */
