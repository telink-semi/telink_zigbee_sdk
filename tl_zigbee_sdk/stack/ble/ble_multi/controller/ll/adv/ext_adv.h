/********************************************************************************************************
 * @file    ext_adv.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef LL_EXT_ADV_H_
#define LL_EXT_ADV_H_

#include "stack/ble/ble_multi/hci/hci_cmd.h"


/* maximum number of advertising sets this SDK can support, periodic advertising is included. */
#define ADV_SETS_NUMBER_MAX 4 //user can't modify this value !!!


/* extended ADV parameters buffer length for each ADV set */
#ifndef ADV_SET_PARAM_LENGTH
#define ADV_SET_PARAM_LENGTH (540) //user can't modify this value !!!
#endif


/**
  * @brief      for user to initialize extended advertising module and allocate single or multiple advertising sets parameter buffer
  *                notice that: this API must be used before any other extended ADV initialization APIs.
  * @param[in]  pBuff_advSets - global extended ADV parameters buffer allocated by application layer.
  * @param[in]  num_advSets - number of application adv_sets
  * @return     Status - 0x00: command succeeded;
  *                      0x12: num_advSets exceed maximum number of supported adv_sets.
  */
ble_sts_t blc_ll_initExtendedAdvModule_initExtendedAdvSetParamBuffer(u8 *pBuff_advSets, int num_advSets);


/**
 * @brief      initialize Advertising Data buffer for all ADV sets
 * @param[in]  pExtAdvData - global Extended ADV data buffer allocated by application layer.
 * @param[in]  max_len_advData - extended ADV data maximum length
 * @return     none
 */
void blc_ll_initExtendedAdvDataBuffer(u8 *pExtAdvData, int max_len_advData);


/**
 * @brief      initialize Scan Response Data Buffer for all adv_set
 * @param[in]  pScanRspData - global extended ADV scan response data buffer allocated by application layer.
 * @param[in]  max_len_scanRspData - extended ADV scan response data maximum length
 * @return     none
 */
void blc_ll_initExtendedScanRspDataBuffer(u8 *pScanRspData, int max_len_scanRspData);


/**
 * @brief      This function is used to set the advertising parameters
 * @param[in]  adv_handle - Used to identify an advertising set
 * @param[in]  adv_evt_prop - describes the type of advertising event that is being configured and its basic properties
 * @param[in]  pri_advInter_min - Minimum advertising interval for undirected and low duty cycle directed advertising.
 * @param[in]  pri_advInter_max - Maximum advertising interval for undirected and low duty cycle directed advertising.
 * @param[in]  pri_advChnMap - primary advertisement channel
 * @param[in]  ownAddrType - Own Address Type
 * @param[in]  peerAddrType - Peer Address Type
 * @param[in]  peerAddr - Peer Address
 * @param[in]  advFilterPolicy - Advertising Filter Policy
 * @param[in]  adv_tx_pow - Advertising TX Power
 * @param[in]  pri_adv_phy - primary advertisement PHY
 * @param[in]  sec_adv_max_skip - Maximum advertising events the Controller can skip
 * @param[in]  sec_adv_phy - Secondary advertisement PHY
 * @param[in]  adv_sid - Value of the Advertising SID subfield in the ADI field of the PDU
 * @param[in]  scan_req_notify_en - Scan Request Notification Enable
 * @return     Status - 0x00: command succeeded;
 *                      0x12:  1. adv_handle out of range;
 *                             2. pri_advChnMap out of range
 *                      0x0C:  advertising is enabled for the specified advertising set
 */
ble_sts_t blc_ll_setExtAdvParam(u8 adv_handle, advEvtProp_type_t adv_evt_prop, u32 pri_advInter_min, u32 pri_advInter_max, adv_chn_map_t pri_advChnMap, own_addr_type_t ownAddrType, u8 peerAddrType, u8 *peerAddr, adv_fp_type_t advFilterPolicy, tx_power_t adv_tx_pow, le_phy_type_t pri_adv_phy, u8 sec_adv_max_skip, le_phy_type_t sec_adv_phy, u8 adv_sid, u8 scan_req_notify_en);


/**
 * @brief      This function is used to set the data used in advertising PDU that have a data field
 *             notice that: setting legacy ADV data also use this API, data length can not exceed 31
 * @param[in]  adv_handle - Used to identify an advertising set
 * @param[in]  advData_len - The number of octets in the Advertising Data parameter
 * @param[in]  *advData - Advertising data
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setExtAdvData(u8 adv_handle, int advData_len, const u8 *advData);

/**
 * @brief  This function is used to set the ADV_DECISION_IND data.
 * @param  adv_handle      - Used to identify an advertising set
 * @param  decisionType    - set the decision type.such as whether a Resolvable Tag subfield is present in the decision data.
 * @param  decisionDataLen - The number of octets in the Decision_Data parameter. the range: 0~8
 * @param  decisionData    - Decision data.
 * @return Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setDecisionData(u8 adv_handle, u8 decisionType, int decisionDataLen, const u8 *decisionData);

/**
 * @brief      This function is used to provide scan response data used in scanning response PDUs.
 *             notice that: setting legacy scan response data also use this API, data length can not exceed 31
 * @param[in]  adv_handle - Used to identify an advertising set
 * @param[in]  scanRspData_len - The number of octets in the Scan_Response Data parameter
 * @param[in]  *scanRspData - Scan response data
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setExtScanRspData(u8 adv_handle, int scanRspData_len, const u8 *scanRspData);


/**
 * @brief      This function is used to request the Controller to enable or disable one or more advertising sets using the
               advertising sets identified by the adv_handle
 * @param[in]  enable -
 * @param[in]  adv_handle - Used to identify an advertising set
 * @param[in]  duration -   the duration for which that advertising set is enabled
 *                          Range: 0x0001 to 0xFFFF, Time = N * 10 ms, Time Range: 10 ms to 655,350 ms
 * @param[in]  max_extAdvEvt - Maximum number of extended advertising events the Controller shall
 *                             attempt to send prior to terminating the extended advertising
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setExtAdvEnable(adv_en_t enable, u8 adv_handle, u16 duration, u8 max_extAdvEvt);


/**
 * @brief      This function is used by the Host to set the random device address specified by the Random_Address
               parameter
 * @param[in]  adv_handle - Used to identify an advertising set
 * @param[in]  *rand_addr - Random Device Address
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setAdvRandomAddr(u8 adv_handle, u8 *rand_addr);


/**
 * @brief      This function is is used to remove an advertising set from the Controller.
 * @param[in]  adv_handle - Used to identify an advertising set
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_removeAdvSet(u8 adv_handle);


/**
 * @brief      This function is used to remove all existing advertising sets from the Controller.
 * @param[in]  none.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_clearAdvSets(void);


/**
 * @brief      If one connection Peripheral is established by Extended ADV, user can use this API to locate the Extended ADV set
 * @param[in]  connHandle - connection handle of ACL connection.
 * @return     Extended ADV handle:
 *                  0xFF: invalid ADV handle, e.g. connection is not Peripheral role, or Peripheral is established by legacy ADV
 *                  Others: Extended ADV handle
 */
u8 blc_ll_getExtendedAdvHandleForAclConnection(u16 connHandle);


#endif /* LL_EXT_ADV_H_ */
