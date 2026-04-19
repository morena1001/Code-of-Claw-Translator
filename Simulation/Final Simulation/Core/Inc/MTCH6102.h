/*
 * MTCH6102.h
 *
 *  Created on: Apr 19, 2026
 *      Author: josue
 */

#ifndef INC_MTCH6102_H_
#define INC_MTCH6102_H_

#include "stm32f3xx_hal.h"
#include "stdbool.h"

/*
 * I2C ADDRESSING
 */

#define BASE_ADDR	0x25
#define ADDR 		BASE_ADDR << 1
#define W_ADDR		0x4A
#define R_ADDR		0x4B

/*
 * GESTURE VALUES
 */

#define NO_GESTURE		0x00
#define SING_CLICK		0x10
#define HOLD_CLICK		0x11
#define DOUB_CLICK		0x20
#define SWIPE_DOWN		0x31
#define SWP_DOWN_HLD	0x32
#define SWIPE_RIGHT		0x41
#define SWP_RIGHT_HLD	0x42
#define SWIPE_UP		0x51
#define SWP_UP_HLD		0x52
#define SWIPE_LEFT		0x61
#define SWP_LEFT_HLD	0x62

/*
 * PROCESSING
 */

#define IS_TAP(input)		(input == SING_CLICK || input == DOUB_CLICK)
#define IS_SCRATCH(input)	(input == SWIPE_DOWN || input == SWIPE_RIGHT || input == SWIPE_UP || input == SWIPE_LEFT)


/*
 * REGISTERS
 */

#define MTCH6102_REG_CMD				0x04
#define MTCH6102_REG_MODE				0x05
#define MTCH6102_REG_GESTURESTATE		0x14
#define MTCH6102_REG_ACTIVEPERIODL		0x25
#define MTCH6102_REG_ACTIVEPERIODH		0x26
#define MTCH6102_REG_IDLEPERIODL		0x27
#define MTCH6102_REG_IDLEPERIODH		0x28
#define MTCH6102_REG_IDLE_TIMEOUT		0x29
#define MTCH6102_REG_MINSWIPEVELOCITY	0x40
#define MTCH6102_REG_I2C_ADDR			0x43


/*
 * BIT SHORTCUTS
 */

#define MTCH6102_CMD_NV				0x80
#define MTCH6102_CMD_DEF			0x40
#define MTCH6102_CMD_CFG			0x20
#define MTCH6102_CMD_MFG			0x08
#define MTCH6102_CMD_BS				0x01
#define MTCH6102_MODE_STANDBY		0x00
#define MTCH6102_MODE_GESTURE		0x01
#define MTCH6102_MODE_TOUCH			0x02
#define MTCH6102_MODE_FULL			0x03
#define MTCH6102_MODE_RAW_ADC		0x04
#define MTCH6102_FRAME_RATE_10MS	0x0142
#define MTCH6102_FRAME_RATE_20MS	0x0284
#define MTCH6102_FRAME_RATE_50MS	0x064C
#define MTCH6102_FRAME_RATE_100MS	0x0C99

/*
 * STRUCT
 */

typedef struct {
	I2C_HandleTypeDef* i2c_handle;
	GPIO_TypeDef* rst_port;
	uint16_t rst_pin;
	uint8_t gesture;
	bool tap, scratch;
} mtch6102_t;

/*
 * INITILIZATION FUNCTION
 */

HAL_StatusTypeDef MTCH6102_Init (mtch6102_t* device, I2C_HandleTypeDef* i2c_handle, GPIO_TypeDef* rst_port, uint16_t rst_pin);

/*
 * HELPER FUNCTIONS
 */

HAL_StatusTypeDef MTCH6102_Confirm_Config (mtch6102_t* device);
bool MTCH6102_Read_Tap_Status (mtch6102_t* device);
bool MTCH6102_Read_Scratch_Status (mtch6102_t* device);
void MTCH6102_Clear_Tap_Status (mtch6102_t* device);
void MTCH6102_Clear_Scratch_Status (mtch6102_t* device);
void MTCH6102_Reset_Device (mtch6102_t* device);
void MTCH6102_Update_Reset_Pin (mtch6102_t* device, GPIO_TypeDef* new_port, uint16_t new_pin);

/*
 * LOW LEVEL FUNCTIONS
 */

HAL_StatusTypeDef MTCH6102_Transmit (mtch6102_t* device, uint8_t reg, uint8_t data);
HAL_StatusTypeDef MTCH6102_Receive (mtch6102_t* device, uint8_t reg, uint8_t* data_buffer, uint8_t buffer_size);

#endif /* INC_MTCH6102_H_ */
