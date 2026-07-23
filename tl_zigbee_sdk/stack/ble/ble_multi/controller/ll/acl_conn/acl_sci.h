/********************************************************************************************************
 * @file    acl_sci.h
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
#ifndef ACL_SCI_H
#define ACL_SCI_H


/**
 * @brief  Common connection interval values for convenience.
 *         Unit: 125us. Pass directly to blc_ll_handle_HostConnRateReq().
 *         Valid range: 3(375us) ~ 32000(4s). Out-of-range returns error.
 */
typedef enum {
    SCI_CONN_INTVL_375US = 3,       //0.375ms (min)
    SCI_CONN_INTVL_500US = 4,       //0.5ms
    SCI_CONN_INTVL_1MS   = 8,       //1ms
    SCI_CONN_INTVL_2MS   = 16,      //2ms
    SCI_CONN_INTVL_2_5MS = 20,      //2.5ms
    SCI_CONN_INTVL_5MS   = 40,      //5ms
    SCI_CONN_INTVL_7_5MS = 60,      //7.5ms (standard LE min)
    SCI_CONN_INTVL_10MS  = 80,      //10ms
    SCI_CONN_INTVL_15MS  = 120,     //15ms
    SCI_CONN_INTVL_20MS  = 160,     //20ms
    SCI_CONN_INTVL_30MS  = 240,     //30ms
    SCI_CONN_INTVL_50MS  = 400,     //50ms
    SCI_CONN_INTVL_100MS = 800,     //100ms
    SCI_CONN_INTVL_200MS = 1600,    //200ms
    SCI_CONN_INTVL_500MS = 4000,    //500ms
    SCI_CONN_INTVL_1S    = 8000,    //1s
    SCI_CONN_INTVL_2S    = 16000,   //2s
    SCI_CONN_INTVL_4S    = 32000,   //4s (max)
} sci_conn_intvl_t;

/**
 * @brief  Common supervision timeout values for convenience.
 *         Unit: 10ms. Pass directly to blc_ll_handle_HostConnRateReq().
 *         Valid range: 10(100ms) ~ 3200(32s). Out-of-range returns error.
 */
typedef enum {
    SCI_TIMEOUT_100MS = 10,         //100ms (min)
    SCI_TIMEOUT_200MS = 20,         //200ms
    SCI_TIMEOUT_500MS = 50,         //500ms
    SCI_TIMEOUT_1S    = 100,        //1s
    SCI_TIMEOUT_2S    = 200,        //2s
    SCI_TIMEOUT_4S    = 400,        //4s
    SCI_TIMEOUT_6S    = 600,        //6s
    SCI_TIMEOUT_8S    = 800,        //8s
    SCI_TIMEOUT_10S   = 1000,       //10s
    SCI_TIMEOUT_16S   = 1600,       //16s
    SCI_TIMEOUT_32S   = 3200,       //32s (max)
} sci_timeout_t;


/**
 * @brief  Set default SCI rate params for Central. When Central receives
 *         LL_CONNECTION_RATE_REQ from Peripheral, it uses these params to
 *         send LL_CONNECTION_RATE_IND. Central-only API.
 * @return BLE_SUCCESS / HCI_ERR_INVALID_HCI_CMD_PARAMS
 */
ble_sts_t blc_hci_sci_setDefaultRateParam(hci_le_sci_setDefaultRateParam * cmdPara);

/**
 * @brief  HCI wrapper for LE Connection Rate Request (7.8.154).
 *         Internally calls blc_ll_handle_HostConnRateReq().
 */
ble_sts_t blc_hci_sci_setConnRateReq(hci_le_sci_connRateReq *cmdPara);

/**
 * @brief  Host-initiated SCI connection rate request. Primary API for both roles.
 *         - Peripheral: sends LL_CONNECTION_RATE_REQ to Central
 *         - Central: sends LL_CONNECTION_RATE_IND to Peripheral
 *         Actual LL PDU is sent in mainloop.
 *
 * @param  connHandle          ACL connection handle
 * @param  connIntvlMin/Max    Connection interval, unit 125us. Range: 3~32000.
 *                             Use sci_conn_intvl_t enum for common values.
 * @param  subrateMin/Max      Subrate factor, 1~500. Max >= Min.
 * @param  maxLatency          Max skip events, 0~499. <= (subrateFactor - 1).
 * @param  continueNum         Burst events after each subrated event, 0~499. < subrateMax.
 * @param  supervisionTimeOut  Timeout, unit 10ms. Range: 10~3200.
 *                             Use sci_timeout_t enum for common values.
 *
 * @return BLE_SUCCESS / HCI_ERR_UNKNOWN_CONN_ID / HCI_ERR_INVALID_HCI_CMD_PARAMS /
 *         HCI_ERR_CMD_DISALLOWED / HCI_ERR_CONN_REJ_LIMITED_RESOURCES
 *
 * @code
 * //2ms interval, subrate 1, 2s timeout (using enums)
 * blc_ll_handle_HostConnRateReq(h, SCI_CONN_INTVL_2MS, SCI_CONN_INTVL_2MS, 1, 1, 0, 0, SCI_TIMEOUT_2S);
 * //5ms interval, subrate 3 (effective 15ms), 2s timeout (using raw values)
 * blc_ll_handle_HostConnRateReq(h, 40, 40, 3, 3, 0, 1, 200);
 * @endcode
 */
ble_sts_t blc_ll_handle_HostConnRateReq(u16 connHandle, u16 connIntvlMin, u16 connIntvlMax, u16 subrateMin,
                                        u16 subrateMax, u16 maxLatency, u16 continueNum, u16 supervisionTimeOut);

/**
 * @brief  Internal: send LL_CONNECTION_RATE_REQ from Peripheral. Called by mainloop.
 *         Use blc_ll_handle_HostConnRateReq() instead.
 */
bool blt_ll_peripheral_sendConnRateReq(u16 conn_handle);

/**
 * @brief  Internal: send LL_CONNECTION_RATE_IND from Central. Called by mainloop.
 *         Use blc_ll_handle_HostConnRateReq() instead.
 */
ble_sts_t blt_ll_central_send_connRateInd(u16 connHandle);

/**
 * @brief  Internal: SCI mainloop task. Called by blc_sdk_main_loop().
 */
int blc_ll_sci_mainloop_task(int flag, void *p);

/**
 * @brief  Internal: init SCI feature, enable LL feature bits.
 */
void blc_ll_sci_init_common(void);

#endif
