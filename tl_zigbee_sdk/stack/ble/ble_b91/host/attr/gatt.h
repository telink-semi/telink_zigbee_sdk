/********************************************************************************************************
 * @file     gatt.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
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
 *******************************************************************************************************/

#ifndef GATT_H_
#define GATT_H_






/**
 * @brief	   This function is used to notify a client of the value of a Characteristic Value from a server.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle  -  attribute handle.
 * @param[in]  *p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushHandleValueNotify  (u16 connHandle, u16 attHandle, u8 *p, int len);


/**
 * @brief	   This function is used to indicate the Characteristic Value from a server to a client.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  *p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushHandleValueIndicate(u16 connHandle, u16 attHandle, u8 *p, int len);


/**
 * @brief	   This function is used to This function is used to request the server to write the value of an attribute without response.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  *p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushWriteCommand (u16 connHandle, u16 attHandle, u8 *p, int len);


/**
 * @brief	   This function is used to request the server to write the value of an attribute.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  *p -  data buffer pointer
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
 * @param[in]  uuid
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
 * @param[in]  uuid
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
 * @param[in]  offset
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushReadBlobRequest (u16 connHandle, u16 attHandle, u16 offset);


/**
 * @brief	   This function is used to obtain the values of attributes according to the uuid.
 * @param[in]  connHandle -  connection handle
 * @param[in]  start_attHandle - start attribute handle.
 * @param[in]  end_attHandle   - end attribute handle.
 * @param[in]  uuid -
 * @param[in]  uuid_len - uuid byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushReadByGroupTypeRequest (u16 connHandle, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);


/**
 * @brief	   This function is used to send the error code to the server.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - attribute handle.
 * @param[in]  errOpcode -  operate code
 * @param[in]  errReason - error reason
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t 	blc_gatt_pushErrorResponse(u16 connHandle, u16 attHandle, u8 errOpcode, u8 errReason);

#endif /* GATT_H_ */
