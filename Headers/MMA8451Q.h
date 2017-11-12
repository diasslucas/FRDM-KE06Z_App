/*
 * MMA8451Q.h
 *
 *  Created on: 16/10/2017
 *      Author: Lucas Dias
 */

#ifndef HEADERS_MMA8451Q_H_
#define HEADERS_MMA8451Q_H_

#include "MMA8451Q_I2C.h"

#define MMA8451Q_STATUS_REG_ADDRESS		0x00
#define MMA8451Q_OUT_X_MSB_ADDRESS		0x01
#define MMA8451Q_OUT_X_LSB_ADDRESS		0x02
#define MMA8451Q_OUT_Y_MSB_ADDRESS		0x03
#define MMA8451Q_OUT_Y_LSB_ADDRESS		0x04
#define MMA8451Q_OUT_Z_MSB_ADDRESS		0x05
#define MMA8451Q_OUT_Z_LSB_ADDRESS		0x06
#define MMA8451Q_CTRL_REG1_ADDRESS		0x2A
#define MMA8451Q_XYZ_DATA_CFG_ADDRESS	0x0E

#define MMA8451Q_REG_ADDRESS_INDEX 		0x00
#define MMA8451Q_DATA_INDEX				0x01

#define MMA8451Q_MSB_REGISTER_DATA_READ 0x00
#define MMA8451Q_LSB_REGISTER_DATA_READ 0x01

/* _______________ Registers config mask ________________*/

#define MMA8451Q_ACTIVE_MASK			0b00000001
#define MMA8451Q_FS_MASK				0b00000011
#define MMA8451Q_ODR_MASK				0b00111000
#define MMA8451Q_ZYXDR_MASK				0b00001000

/* _________________ Registers config  __________________*/

#define MMA8451Q_STANDBY_MODE_SET		0b00000000
#define MMA8451Q_ACTIVE_MODE_SET		0b00000001
#define MMA8451Q_FULL_SCALE_4G_SET		0b00000001
#define MMA8451Q_FULL_SCALE_8G_SET		0b00000010
#define MMA8451Q_ODR_100HZ_SET			0b00011000		// Set Output Data Rates - bits 3 (DR0) and 4 (DR1)


extern boolean DataTransmittedFlg;
extern boolean DataReceivedFlg;

struct {
	boolean active;
	uint16_t data[3];
	uint32_t registerAddress;
	uint8_t bufferIn;
	uint8_t bufferOut[2];
	LDD_TError Error;
	uint8_t registerMask;
	uint8_t configData;
	LDD_TDeviceData *MyI2CPtr;
} typedef MMA8451Q_Data;

typedef enum{
	MMA8451Q_STANDBY_MODE = 0,
	MMA8451Q_ACTIVE_MODE,
	MMA8451Q_4G_SCALE_MODE,
	MMA8451Q_8G_SCALE_MODE,
	MMA8451Q_DATA_RATE_SETTING
} MMA8451Q_Config_t;

typedef enum{
	MMA8451Q_X_AXIS = 0,
	MMA8451Q_Y_AXIS,
	MMA8451Q_Z_AXIS
} MMA8451Q_AxisRead_t;

void MMA8451Q_main(void);



#endif /* HEADERS_MMA8451Q_H_ */
