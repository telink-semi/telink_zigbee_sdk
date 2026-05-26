/********************************************************************************************************
 * @file    leg_adv.h
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
#ifndef LEG_ADV_H_
#define LEG_ADV_H_

/*
 *                     1. Slave Connect                            2. Slave Disconnect                      3. Set ADV enable when ADV task is not running
 *
 *
 * Strategy 1    if slave number reach MAX, stop ADV            if MAX slave number disconnect and ADV         if slave number do not reach MAX, start ADV
 *              (stop ADV task, keep ADV enable status)         enable status is 1, start ADV
 *
 * Strategy 2               no change                                 no change                                     start ADV
 *
 *
 * Strategy 3               stop ADV                                  no change                                     start ADV
 *              (stop ADV task, clear ADV enable status)
 *
 *
 */
typedef enum
{
    LEG_ADV_EN_STRATEGY_1 = 0, /*!< SDK default strategy */

    LEG_ADV_EN_STRATEGY_2 = 1, /*!< ADV task running depends only on API "blc_ll_setAdvEnable" */

    LEG_ADV_EN_STRATEGY_3 = 2, /*!< ADV stop when peripheral connect, upper layer should set ADV enable again
                                        if needing another slave connected */
} legadv_en_str_t;             //legacy ADV enable strategy

/**
 * @brief      for user to initialize legacy advertising module
 *             notice that only one module can be selected between legacy advertising module and extended advertising module
 * @param      none
 * @return     none
 */
void blc_ll_initLegacyAdvertising_module(void);


/**
 * @brief      set the data used in advertising packets that have a data field.
 * @param[in]  *data -  advertising data buffer
 * @param[in]  len - The number of significant octets in the Advertising_Data.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setAdvData(const u8 *data, u8 len);


/**
 * @brief      This function is used to provide data used in Scanning Packets that have a data field.
 * @param[in]  *data -  Scan_Response_Data buffer
 * @param[in]  len - The number of significant octets in the Scan_Response_Data.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setScanRspData(const u8 *data, u8 len);


/**
 * @brief      This function is used to set the advertising parameters.
 * @param[in]  intervalMin - Minimum advertising interval(Time = N * 0.625 ms, Range: 0x0020 to 0x4000)
 * @param[in]  intervalMin - Maximum advertising interval(Time = N * 0.625 ms, Range: 0x0020 to 0x4000)
 * @param[in]  advType - Advertising_Type
 * @param[in]  ownAddrType - Own_Address_Type
 * @param[in]  peerAddrType - Peer_Address_Type
 * @param[in]  *peerAddr - Peer_Address
 * @param[in]  adv_channelMap - Advertising_Channel_Map
 * @param[in]  advFilterPolicy - Advertising_Filter_Policy
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setAdvParam(adv_inter_t intervalMin, adv_inter_t intervalMax, adv_type_t advType, own_addr_type_t ownAddrType, u8 peerAddrType, u8 *peerAddr, adv_chn_map_t adv_channelMap, adv_fp_type_t advFilterPolicy);


/**
 * @brief      This function is used to request the Controller to start or stop advertising.
 * @param      adv_enable - Advertising_Enable
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setAdvEnable(adv_en_t adv_enable);


/**
 * @brief      this function is used to set whether to continue sending broadcast packets when receiving scan request in the current adv interval.
 * @param[in]  enable - 1:continue sending broadcast packets when receiving scan request.
 *                      0:default value, stop sending broadcast packets when receiving scan request.
 * @return     none.
 */
void blc_ll_continue_adv_after_scan_req(u8 enable);


/**
 * @brief      this function is used to set whether to send scan response after receiving scan request manually.
 * @param[in]  enable - 1:configure sending scan response after receiving scan request manually.
 *                      0:do not respond scan request.
 * @return     none.
 */
void blc_ll_set_scan_rsp_en(u8 enable);


/**
 * @brief      This function is used to configure leg_adv enabling by API only.
 *             e.g. M4S4, even slave connection number is 4, leg_adv still work but can not be connected
 * @param[in]  strategy -
 * @return     none
 */
void blc_ll_configLegacyAdvEnableStrategy(legadv_en_str_t strategy);


#endif /* LEG_ADV_H_ */
