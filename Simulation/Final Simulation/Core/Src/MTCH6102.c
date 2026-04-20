/*
 * MTCH6102.c
 *
 *  Created on: Apr 19, 2026
 *      Author: josue
 */

#include "MTCH6102.h"

/*
 * @brief	Initialize an instance of the MTCH6102 struct with default values.
 * 				Set up IC using I2C communication
 * @param	device		A user declared instance of the mtch6102 struct
 * @param	i2c_handle	The I2C handle used to communicate with the IC
 * @param	rst_port	The GPIO port of the reset pin present on the mikroe touchpad click
 * 							breakout board
 * @param	rst_pin		The GPIO pin of the reset pin present on the mikroe touchpad click
 * 							breakout board
 * @retval 	HAL status
 */
HAL_StatusTypeDef MTCH6102_Init (mtch6102_t* device, I2C_HandleTypeDef* i2c_handle, GPIO_TypeDef* rst_port, uint16_t rst_pin) {
	device->i2c_handle = i2c_handle;
	device->rst_port = rst_port;
	device->rst_pin = rst_pin;
	device-> gesture = NO_GESTURE;
	device->tap = false;
	device->scratch = false;

	// Check that communication works
	uint8_t data = 0;
	if (MTCH6102_Receive (device, MTCH6102_REG_I2C_ADDR, &data, 1) != HAL_OK && data != BASE_ADDR)	return HAL_ERROR;

	HAL_StatusTypeDef status = HAL_OK;

	// Setup gesture mode
	status = MTCH6102_Transmit (device, MTCH6102_REG_MODE, MTCH6102_MODE_GESTURE);
	if (status != HAL_OK)	return status;

	// Set idle timeout to 0x08
	status = MTCH6102_Transmit (device, MTCH6102_REG_IDLE_TIMEOUT, 0x08);
	if (status != HAL_OK)	return status;

	// Set active period to 10 ms
	status = MTCH6102_Transmit (device, MTCH6102_REG_ACTIVEPERIODL, (uint8_t) (MTCH6102_FRAME_RATE_10MS & 0x00FF));
	if (status != HAL_OK)	return status;
	status = MTCH6102_Transmit (device, MTCH6102_REG_ACTIVEPERIODH, (uint8_t) (MTCH6102_FRAME_RATE_10MS >> 8));
	if (status != HAL_OK)	return status;

	// Set idle period to 10 ms
	status = MTCH6102_Transmit (device, MTCH6102_REG_IDLEPERIODL, (uint8_t) (MTCH6102_FRAME_RATE_10MS & 0x00FF));
	if (status != HAL_OK)	return status;
	status = MTCH6102_Transmit (device, MTCH6102_REG_IDLEPERIODH, (uint8_t) (MTCH6102_FRAME_RATE_10MS >> 8));
	if (status != HAL_OK)	return status;

	// Set minimum swipe velocity to 0x01
	status = MTCH6102_Transmit (device, MTCH6102_REG_MINSWIPEVELOCITY, 0x01);
	if (status != HAL_OK)	return status;

	// Confirm new configuration settings
	status = MTCH6102_Transmit (device, MTCH6102_REG_CMD, MTCH6102_CMD_CFG);

	return status;
}

/*
 * @brief	Confirms the new configuration settings as per pages 15 and 26 of the datasheet
 * @param	device		A user declared instance of the mtch6102 struct
 * @retval 	HAL status
 */
HAL_StatusTypeDef MTCH6102_Confirm_Config (mtch6102_t* device) {
	return MTCH6102_Transmit (device, MTCH6102_REG_CMD, MTCH6102_CMD_CFG);
}

/*
 * @brief	Provides a volatile, single use read of the tap register on the struct by
 * 				clearing it afterward
 * @param	device		A user declared instance of the mtch6102 struct
 * @retval 	value of tap register on struct
 */
bool MTCH6102_Read_Tap_Status (mtch6102_t* device) {
	bool val = device->tap;
	device->tap = false;
	return val;
}

/*
 * @brief	Provides a volatile, single use read of the scratch register on the struct by
 * 				clearing it afterward
 * @param	device		A user declared instance of the mtch6102 struct
 * @retval 	value of scratch register on struct
 */
bool MTCH6102_Read_Scratch_Status (mtch6102_t* device) {
	bool val = device->scratch;
	device->scratch = false;
	return val;
}

/*
 * @brief	Clears the value of the tap register
 * @param	device		A user declared instance of the mtch6102 struct
 */
void MTCH6102_Clear_Tap_Status (mtch6102_t* device) {
	device->tap = false;
}

/*
 * @brief	Clears the value of the scratch register
 * @param	device		A user declared instance of the mtch6102 struct
 */
void MTCH6102_Clear_Scratch_Status (mtch6102_t* device) {
	device->scratch = false;
}

/*
 * @brief	Performs a hardware reset using the reset pin on the breakout board
 * @param	device		A user declared instance of the mtch6102 struct
 */
void MTCH6102_Reset_Device (mtch6102_t* device) {
	HAL_GPIO_WritePin (device->rst_port, device->rst_pin, GPIO_PIN_RESET);
	HAL_Delay (100);
	HAL_GPIO_WritePin (device->rst_port, device->rst_pin, GPIO_PIN_SET);
}

/*
 * @brief	Updates the STM32 pin used for the reset pin on the breakout board
 * @param	device		A user declared instance of the mtch6102 struct
 * @param	new_port	The new GPIO port for the reset pin
 * @param	new_pin		The new GPIO pin for the reset pin
 */
void MTCH6102_Update_Reset_Pin (mtch6102_t* device, GPIO_TypeDef* new_port, uint16_t new_pin) {
	device->rst_port = new_port;
	device->rst_pin = new_pin;
}

/*
 * @brief	Transmits data to the IC via I2C as per page 8 of the datasheet
 * @param	device		A user declared instance of the mtch6102 struct
 * @param	reg			The address of the reg to change
 * @param	data		The new value to be stored in the register
 * @retval 	HAL status
 */
HAL_StatusTypeDef MTCH6102_Transmit (mtch6102_t* device, uint8_t reg, uint8_t data) {
	uint8_t cmd[2] = { reg, data };
	return HAL_I2C_Master_Transmit (device->i2c_handle, W_ADDR, cmd, 2, HAL_MAX_DELAY);
}

/*
 * @brief	Transceives data to the IC via I2C as per page 8 of the datasheet
 * @param	device		A user declared instance of the mtch6102 struct
 * @param	reg			The address of the register to receive data from
 * @param	data_buffer	The location in STM32 memory to store the incoming data
 * @param	buffer_size	The size of the buffer, or the size of the data to be received from
 * @retval 	HAL status
 */
HAL_StatusTypeDef MTCH6102_Receive (mtch6102_t* device, uint8_t reg, uint8_t* data_buffer, uint8_t buffer_size) {
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit (device->i2c_handle, W_ADDR, &reg, 1, 100);
	status = HAL_I2C_Master_Receive (device->i2c_handle, R_ADDR, data_buffer, buffer_size, 100);
	return status;
}
