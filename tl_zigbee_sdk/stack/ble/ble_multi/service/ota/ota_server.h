/********************************************************************************************************
 * @file    ota_server.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef STACK_BLE_SERVICE_OTA_OTA_SERVER_H_
#define STACK_BLE_SERVICE_OTA_OTA_SERVER_H_


/**
 * @brief   OTA start command callback declaration
 */
typedef void (*ota_startCb_t)(void);

/**
 * @brief   OTA version callback declaration
 */
typedef void (*ota_versionCb_t)(void);

/**
 * @brief       OTA result indicate callback declaration
 * @param[in]   result - OTA result
 */
typedef void (*ota_resIndicateCb_t)(int result);


/**
 * @brief      this function is used for user to initialize OTA server module.
 * @param      none
 * @return     none
 */
void blc_ota_initOtaServer_module(void);


/**
 * @brief      This function is used to set maximum firmware size and OTA new firmware boot address in Flash.
 *             attention: 1. If this API is used, must be called before "sys_init" or "cpu_wakeup_init" when initialization !!!
 *                        2. If this API never used, default maximum firmware size is 252K byte(last 4K can not used for special reason)
 *                                                   default OTA new firmware boot address is 0x40000(256K).
 * @param[in]  firmware_size_k - firmware maximum size unit: K Byte; must be 4K aligned
 *             attention: should consider not only current firmware size, but future new firmware upgraded by OTA.
 *                        For example, if current firmware size is 250K, but future firmware size maybe 260K due to some new function,
 *                        boot address 0x80000(512K) should be used instead of 0x40000(256K).
 * @param[in]  boot_addr - new firmware storage address, can only choose from multiple boot address supported by MCU
 * @return     Status - 0x00: command succeeded;
 *                      Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ota_setFirmwareSizeAndBootAddress(int firmware_size_k, multi_boot_addr_e boot_addr);


/**
 * @brief      This function is used to read current used multiple boot address.
 *             return value is set by API "blc ota_setFirmwareSizeAndBootAddress"
 * @param[in]  none
 * @return     multiple boot address
 */
u32 blc_ota_getCurrentUsedMultipleBootAddress(void);


/**
 * @brief      This function is used to set OTA firmware version number.
 *             if user use version compare to decide if OTA update, should use this API to set firmware version.
 * @param[in]  version_number - firmware version number
 * @return     none
 */
void blc_ota_setFirmwareVersionNumber(u16 version_number);


/**
 * @brief      This function is used to register OTA start command callback.
 *             when local device receive OTA command  "CMD_OTA_START" and  "CMD_OTA_START_EXT"  form peer device,
 *             after checking all parameters are correct, local device will enter OTA update and trigger OTA start command callback.
 * @param[in]  cb - callback function
 * @return     none
 */
void blc_ota_registerOtaStartCmdCb(ota_startCb_t cb);


/**
 * @brief      This function is used to register OTA version command callback
 *             when local device receive OTA command  "CMD_OTA_VERSION", trigger this callback.
 * @param[in]  cb - callback function
 * @return     none
 */
void blc_ota_registerOtaFirmwareVersionReqCb(ota_versionCb_t cb);


/**
 * @brief      This function is used to register OTA result indication callback
 * @param[in]  cb - callback function
 * @return     none
 */
void blc_ota_registerOtaResultIndicationCb(ota_resIndicateCb_t cb);


/**
 * @brief      This function is used to set OTA whole process timeout value
 *             if not set, default value is 30 seconds
 * @param[in]  timeout_second - timeout value, unit: second, should in range of 5 ~ 1000 seconds
 * @return     Status - 0x00: command succeeded;
 *                      Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ota_setOtaProcessTimeout(int timeout_second);


/**
 * @brief      This function is used to set OTA packet interval timeout value
 *             if not set, default value is 5 S
 * @param[in]  timeout_second - timeout value, unit: mS, should in range of 1 ~ 20
 * @return     Status - 0x00: command succeeded;
 *                      Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ota_setOtaDataPacketTimeout(int timeout_second);


/**
 * @brief      This function is used to set resolution of OTA schedule indication by PDU number
 *             OTA server will send a "CMD_OTA_SCHEDULE_PDU_NUM" command with packet structure "ota_sche_pdu_num_t" to notify OTA client
 *             as an OTA process indication.  For example, if user set pdu_num to 10
 *                  when receive PDU number 10, send notification, "success_pdu_cnt" is 10
 *                  when receive PDU number 20, send notification, "success_pdu_cnt" is 20
 *                  ...
 *                  when receive PDU number 100, send notification, "success_pdu_cnt" is 100
 *                  ...
 *             attention: If this API is used, must be called after "blc ota_initOtaServer_module" when initialization !!!
 * @param[in]  pdu_num - number of OTA PDU
 * @return     Status - 0x00: command succeeded;
 *                      Others: command failed, refer to definition of "ble_sts_t" to know the reason
 */
ble_sts_t blc_ota_setOtaScheduleIndication_by_pduNum(int pdu_num);


/**
 * @brief      This function is used to calculate OTA notify data ATT handle by OTA write data ATT handle
 * @param[in]  attHandle_offset - offset value from OTA write handle to OTA notify handle.
 *             If not set, default value is 0 which means OTA write and notify in a same ATT handle.
 * @return     none
 */
void blc_ota_setAttHandleOffset(s8 attHandle_offset);


/**
 * @brief      This function is used to write OTA data to flash
 * @param[in]  p - GATT data buffer pointer of write_req or write_cmd
 * @return     0
 */
int otaWrite(u16 connHandle, void *p);


/**
 * @brief      This function is used to read current running firmware start address on Flash
 * @param[in]  none
 * @return     current running firmware start address on Flash
 */
u32 blc_ota_getCurrentFirmwareStartAddress(void);


/**
 * @brief      This function is used to read next firmware start address on Flash
 * @param[in]  none
 * @return     next firmware start address on Flash, for example, if user set boot address 0x40000
 *             If OTA new firmware boot address is 0x40000:
 *             return 0x40000 if current running firmware start address is 0x00000
 *             return 0x00000 if current running firmware start address is 0x40000
 */
u32 blc_ota_getNextFirmwareStartAddress(void);


/**
 * @brief      this function is used to enable Firmware encryption.
 *             attention: If this API is used, must be called before "blc ota_initOtaServer_module" when initialization !!!
                          this API must be used on MCU that support hardware firmware encryption function !!!
 * @param      none
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ota_enableFirmwareEncryption(void);


/**
 * @brief      this function is used to enable secure boot.
 *             attention: If this API is used, must be called before "blc ota_initOtaServer_module" when initialization !!!
                          this API must be used on MCU that support hardware secure boot function !!!
 * @param      none
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ota_enableSecureBoot(void);


#endif /* STACK_BLE_SERVICE_OTA_OTA_SERVER_H_ */
