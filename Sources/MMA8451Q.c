/*
 * MMA8451Q.c
 *
 *  Created on: 16/10/2017
 *      Author: Lucas Dias
 */


#include "MQX1.h"
#include "mqxlite.h"
#include "MMA8451Q.h"
#include <stdio.h>
#include <string.h>

boolean DataTransmittedFlg 	= FALSE;
boolean DataReceivedFlg 	= FALSE;


LDD_TError MMA8451Q_Init(MMA8451Q_Data *tdata);
LDD_TError MMA8451Q_ActiveMode(void);
LDD_TError MMA8451Q_StandbyMode(void);
LDD_TError MMA8451Q_8gScaleSet(void);
LDD_TError MMA8451Q_SetDataRate(void);
LDD_TError MMA8451Q_Read(MMA8451Q_Data *dataCom);
LDD_TError MMA8451Q_Write(MMA8451Q_Data *dataCom);
LDD_TError MMA8451Q_configSetting(MMA8451Q_Config_t configType, LDD_TDeviceData *MyI2CPtr);
LDD_TError MMA8451Q_DataRead(MMA8451Q_AxisRead_t axis, MMA8451Q_Data *tdata);

MMA8451Q_Data accelerometerInfo;

void MMA8451Q_main(void){

	uint16_t xAxisDataRead = 0;
	uint16_t yAxisDataRead = 0;
	uint16_t zAxisDataRead = 0;

	accelerometerInfo.MyI2CPtr	= MMA8451Q_I2C_Init(NULL);
	accelerometerInfo.Error    	= MMA8451Q_I2C_Enable(accelerometerInfo.MyI2CPtr);
	accelerometerInfo.Error 	= MMA8451Q_Init(&accelerometerInfo);

	if(accelerometerInfo.Error == ERR_OK){
		accelerometerInfo.active = TRUE;
		do{
			accelerometerInfo.registerAddress = MMA8451Q_STATUS_REG_ADDRESS;
			accelerometerInfo.Error = MMA8451Q_Read(&accelerometerInfo);
			if(accelerometerInfo.bufferIn && MMA8451Q_ZYXDR_MASK){
				accelerometerInfo.Error = MMA8451Q_DataRead(MMA8451Q_X_AXIS, &accelerometerInfo);
				accelerometerInfo.Error = MMA8451Q_DataRead(MMA8451Q_Y_AXIS, &accelerometerInfo);
				accelerometerInfo.Error = MMA8451Q_DataRead(MMA8451Q_Z_AXIS, &accelerometerInfo);
				xAxisDataRead = accelerometerInfo.data[MMA8451Q_X_AXIS];
				yAxisDataRead = accelerometerInfo.data[MMA8451Q_Y_AXIS];
				zAxisDataRead = accelerometerInfo.data[MMA8451Q_Z_AXIS];
			}
			_time_delay_ticks(10);

		}while(accelerometerInfo.active);
	}
}


/***************************************************************************************
 *
 * Function name: MMA8451Q_XaxisRead
 *
 * Init MMA3451Q configurations module.
 *
 * Inputs:	dataCom - Struct contain MMA8451Q data handler.
 * Outputs:	dataCom->Error Receive status
 * 			- ERR_OK - OK
 *
 *
 ***************************************************************************************/

LDD_TError MMA8451Q_DataRead(MMA8451Q_AxisRead_t axis, MMA8451Q_Data *tdata){

	uint8_t  index = 0;
	uint16_t byteTemp = 0;

	do{

		switch(axis){
		case MMA8451Q_X_AXIS:
			if(index == MMA8451Q_MSB_REGISTER_DATA_READ){
				tdata->registerAddress = MMA8451Q_OUT_X_MSB_ADDRESS;
			}else{
				tdata->registerAddress = MMA8451Q_OUT_X_LSB_ADDRESS;
			}
			break;
		case MMA8451Q_Y_AXIS:
			if(index == MMA8451Q_MSB_REGISTER_DATA_READ){
				tdata->registerAddress = MMA8451Q_OUT_Y_MSB_ADDRESS;
			}else{
				tdata->registerAddress = MMA8451Q_OUT_Y_LSB_ADDRESS;
			}
			break;
		case MMA8451Q_Z_AXIS:
			if(index == MMA8451Q_MSB_REGISTER_DATA_READ){
				tdata->registerAddress = MMA8451Q_OUT_Z_MSB_ADDRESS;
			}else{
				tdata->registerAddress = MMA8451Q_OUT_Z_LSB_ADDRESS;
			}
			break;
		}

		tdata->Error = MMA8451Q_Read(&accelerometerInfo);

		if(index == MMA8451Q_MSB_REGISTER_DATA_READ){
			byteTemp = accelerometerInfo.bufferIn;
			tdata->data[axis] = byteTemp << 8;
		}else{
			tdata->data[axis] |= accelerometerInfo.bufferIn;
			memcpy(&byteTemp, &tdata->data[axis], 2);
			tdata->data[axis] = byteTemp >> 2;
		}
	}while(++index < 2);


}

/***************************************************************************************
 *
 * Function name: MMA8451Q_Init
 *
 * Init MMA3451Q configurations module.
 *
 * Inputs:	dataCom - Struct contain MMA8451Q data handler.
 * Outputs:	dataCom->Error Receive status
 * 			- ERR_OK - OK
 *
 *
 ***************************************************************************************/

LDD_TError MMA8451Q_Init(MMA8451Q_Data *tdata){

	tdata->active = TRUE;
	tdata->Error = ERR_PARAM_MODE;

	tdata->Error = MMA8451Q_configSetting(MMA8451Q_STANDBY_MODE     , tdata->MyI2CPtr);
	if(tdata->Error == ERR_OK){
		tdata->Error = MMA8451Q_configSetting(MMA8451Q_4G_SCALE_MODE    , tdata->MyI2CPtr);
		if(tdata->Error == ERR_OK){
			tdata->Error = MMA8451Q_configSetting(MMA8451Q_DATA_RATE_SETTING, tdata->MyI2CPtr);
			if(tdata->Error == ERR_OK){
				tdata->Error = MMA8451Q_configSetting(MMA8451Q_ACTIVE_MODE      , tdata->MyI2CPtr);
			}
		}
	}

	return accelerometerInfo.Error;
}

/***************************************************************************************
 *
 * Function name: MMA8451Q_configSetting
 *
 *
 *
 * Inputs:	dataCom - Struct contain MMA8451Q data handler.
 * Outputs:	dataCom->Error Receive status
 * 			- ERR_OK - OK
 *
 *
 ***************************************************************************************/

LDD_TError MMA8451Q_configSetting(MMA8451Q_Config_t configType, LDD_TDeviceData *MyI2CPtr){

	MMA8451Q_Data dataCom;

	dataCom.MyI2CPtr = MyI2CPtr;

	switch (configType) {
		case MMA8451Q_STANDBY_MODE:
			dataCom.registerAddress = MMA8451Q_CTRL_REG1_ADDRESS;
			dataCom.registerMask 	= MMA8451Q_ACTIVE_MASK;
			dataCom.configData 		= MMA8451Q_STANDBY_MODE_SET;
			break;

		case MMA8451Q_ACTIVE_MODE:
			dataCom.registerAddress = MMA8451Q_CTRL_REG1_ADDRESS;
			dataCom.registerMask 	= MMA8451Q_ACTIVE_MASK;
			dataCom.configData 		= MMA8451Q_ACTIVE_MODE_SET;
			break;

		case MMA8451Q_8G_SCALE_MODE:
			dataCom.registerAddress = MMA8451Q_XYZ_DATA_CFG_ADDRESS;
			dataCom.registerMask 	= MMA8451Q_FS_MASK;
			dataCom.configData 		= MMA8451Q_FULL_SCALE_8G_SET;
			break;

		case MMA8451Q_4G_SCALE_MODE:
			dataCom.registerAddress = MMA8451Q_XYZ_DATA_CFG_ADDRESS;
			dataCom.registerMask 	= MMA8451Q_FS_MASK;
			dataCom.configData 		= MMA8451Q_FULL_SCALE_4G_SET;
			break;

		case MMA8451Q_DATA_RATE_SETTING:
			dataCom.registerAddress = MMA8451Q_CTRL_REG1_ADDRESS;
			dataCom.registerMask 	= MMA8451Q_ODR_MASK;
			dataCom.configData 		= MMA8451Q_ODR_100HZ_SET;
			break;
		default:
			break;
	}

	dataCom.Error = MMA8451Q_Read(&dataCom);

	if(dataCom.Error == ERR_OK){
		dataCom.bufferOut[MMA8451Q_DATA_INDEX] = (dataCom.bufferIn & ~dataCom.registerMask);
		dataCom.bufferOut[MMA8451Q_DATA_INDEX] = (dataCom.bufferOut[MMA8451Q_DATA_INDEX] | dataCom.configData);
		dataCom.bufferOut[MMA8451Q_REG_ADDRESS_INDEX] = dataCom.registerAddress;
		dataCom.Error = MMA8451Q_Write(&dataCom);
	}
	return dataCom.Error;
}

/***************************************************************************************
 *
 * Function name: MMA8451Q_Read (Following MMA8451Q Protocol)
 *
 * Read data from MMA8451Q registers.
 *
 * Inputs:	dataCom - Struct contain MMA8451Q data handler.
 * Outputs:	dataCom->Error Receive status
 * 			- ERR_OK - OK
 *
 *
 ***************************************************************************************/
LDD_TError MMA8451Q_Read(MMA8451Q_Data *dataCom){

	/* Firstly REGISTER_ADDRES sent on the I2C bus and no send stop condition */
	dataCom->Error = MMA8451Q_I2C_MasterSendBlock(dataCom->MyI2CPtr,  &dataCom->registerAddress, 1U, LDD_I2C_NO_SEND_STOP);
	while (!DataTransmittedFlg) {
		_time_delay_ticks(5);
	}
	DataTransmittedFlg = FALSE;

	/* REGISTER_ADDRES data is read and send stop condition */
	if(dataCom->Error == ERR_OK){
		dataCom->Error = MMA8451Q_I2C_MasterReceiveBlock(dataCom->MyI2CPtr, &dataCom->bufferIn, 1U, LDD_I2C_SEND_STOP);
		while (!DataReceivedFlg) {
			_time_delay_ticks(5);
		}
		DataReceivedFlg = FALSE;
	}
	return dataCom->Error;
}


/***************************************************************************************
 *
 * Function name: MMA8451Q_Write
 *
 * Write data on MMA8451Q registers.
 *
 * Inputs:	dataCom - Struct contain MMA8451Q data handler.
 * Outputs:	dataCom->Error Transmission status
 * 			- ERR_OK - OK
 *
 *
 ***************************************************************************************/
LDD_TError MMA8451Q_Write(MMA8451Q_Data *dataCom){

	/* bufferOut first index = [REGISTER_ADDRES] - second index = [data] + send stop condition */
	if(dataCom->Error == ERR_OK){
		dataCom->Error = MMA8451Q_I2C_MasterSendBlock(dataCom->MyI2CPtr,  &dataCom->bufferOut, 2U, LDD_I2C_SEND_STOP);
		while (!DataTransmittedFlg) {
			_time_delay_ticks(5);
		}
		DataTransmittedFlg = FALSE;
	}
	return dataCom->Error;
}

