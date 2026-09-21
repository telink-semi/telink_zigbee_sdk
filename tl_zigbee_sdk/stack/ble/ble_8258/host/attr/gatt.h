/********************************************************************************************************
 * @file    gatt.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef GATT_H_
#define GATT_H_






/**
 * @brief	   This function is used to notify a client of the value of a Characteristic Value from a server.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle  -  attribute handle.
 * @param[in]  p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushHandleValueNotify  (u16 connHandle, u16 attHandle, u8 *p, int len);

/**
 * @brief	   This function is used to notify a client of the Handle-Length-Value Tuple of a Characteristic Value from a server
 * @param[in]  connHandle - connection handle
 * @param[in]  lists - Handle Length Value Tuple
 * @param[in]  listNum - number of handle value length tuple list
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushMultiHandleValueNotify  (u16 connHandle, atts_mulHandleNtf_t* lists, u8 listNum);

/**
 * @brief	   This function is used to indicate the Characteristic Value from a server to a client.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushHandleValueIndicate(u16 connHandle, u16 attHandle, u8 *p, int len);


/**
 * @brief	   This function is used to request the server to write the value of an attribute without response.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushWriteCommand (u16 connHandle, u16 attHandle, u8 *p, int len);


/**
 * @brief	   This function is used to request the server to write the value of an attribute.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushWriteRequest (u16 connHandle, u16 attHandle, u8 *p, int len);




/**
 * @brief	   This function is used to obtain the mapping of attribute handles with their associated types
 * 			   and allows a client to discover the list of attributes and their types on a server.
 * @param[in]  connHandle -  connection handle
 * @param[in]  start_attHandle - start attribute handle.
 * @param[in]  end_attHandle -  end attribute handle.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushFindInformationRequest(u16 connHandle, u16 start_attHandle, u16 end_attHandle);


/**
 * @brief	   This function is used to obtain the handles of attributes that have a 16bit uuid attribute type and attribute value.
 * @param[in]  connHandle -  connection handle
 * @param[in]  start_attHandle - start attribute handle.
 * @param[in]  end_attHandle   - end attribute handle.
 * @param[in]  uuid - universally unique identifier.
 * @param[in]  attr_value - attribute value
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushFindByTypeValueRequest (u16 connHandle, u16 start_attHandle, u16 end_attHandle, u16 uuid, u8* attr_value, int len);


/**
 * @brief	   This function is used to obtain the values of attributes where the attribute type is known but handle is not known.
 * @param[in]  connHandle -  connection handle
 * @param[in]  start_attHandle - start attribute handle.
 * @param[in]  end_attHandle   - end attribute handle.
 * @param[in]  uuid - universally unique identifier.
 * @param[in]  uuid_len -uuid byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushReadByTypeRequest (u16 connHandle, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);


/**
 * @brief	   This function is used to request the server to read the value of an attribute.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle -   attribute handle.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushReadRequest (u16 connHandle, u16 attHandle);


/**
 * @brief	   This function is used to request the server to read part of the value of an attribute at a given offset
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  offset - the data offset.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushReadBlobRequest (u16 connHandle, u16 attHandle, u16 offset);


/**
 * @brief	   This function is used to obtain the values of attributes according to the uuid.
 * @param[in]  connHandle -  connection handle
 * @param[in]  start_attHandle - start attribute handle.
 * @param[in]  end_attHandle   - end attribute handle.
 * @param[in]  uuid - universally unique identifier.
 * @param[in]  uuid_len - uuid byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushReadByGroupTypeRequest (u16 connHandle, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);


/**
 * @brief	   This function is used to transmit prepare data to peer side.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle -  attribute handle.
 * @param[in]  valOffset   - data offset.
 * @param[in]  data - transmit data.
 * @param[in]  data_len - transmit data len.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushPrepareWriteRequest (u16 connHandle, u16 attHandle, u16 valOffset,u8 *data, int data_len);


/**
 * @brief	   This function is used to execute prepare write.
 * @param[in]  connHandle -  connection handle
 * @param[in]  value - 0x00:Cancel all prepared writes
 * 					   0x01:Immediately write all pending prepared values
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushExecuteWriteRequest(u16 connHandle,u8 value);


/**
 * @brief       This function is used to send ATT handle value confirm.
 * @param[in]   connHandle - connection handle.
 * @return      Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushConfirm(u16 connHandle);

/**
 * @brief		This function is used to send read multiple request
 * @param[in]	connHandle - connection handle
 * @param[in]	numHandles - number of ATT handle to send
 * @param[in]	pHandle - ATT handles of read multiple request
 * @return		Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushReadMultiRequest(u16 connHandle, u8 numHandles, u16 *pHandle);

/**
 * @brief		This function is used to send read multiple variable request
 * @param[in]	connHandle - connection handle
 * @param[in]	numHandles - number of ATT handle to send
 * @param[in]	pHandle - ATT handles of read multiple request
 * @return		Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushReadMultiVariableRequest(u16 connHandle, u8 numHandles, u16 *pHandle);

/**
 * @brief       This function is used to send ATT Error Response.
 * @param[in]   connHandle - connection handle.
 * @param[in]   reqOpcode - request option code.
 * @param[in]   attHdlInErr - ATT handle in error.
 * @param[in]   ErrorCode - error code.
 * @return      Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushErrResponse(u16 connHandle, u8 reqOpcode, u16 attHdlInErr, u8 ErrorCode);
#endif /* GATT_H_ */
