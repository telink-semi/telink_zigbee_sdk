/********************************************************************************************************
 * @file    zbd_tunnel.h
 *
 * @brief   This is the header file for zbd_tunnel
 *
 * @author  Zigbee Group
 * @date    2021
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
#ifndef _ZBD_TUNNEL_H_
#define _ZBD_TUNNEL_H_

#include "tl_common.h"
#include "zbd_common.h"

#define ZBD_TUNNEL_NDPU_LEN_MIN                 0x0A //type + length, length: 0 + security frame counter + mic
#define ZBD_TUNNEL_NPDU_LEN_MAX                 0x76

#define ZBD_TUNNEL_NPDU_MESSAGE_ID              0x00

#define ZBD_TUNNEL_PEND_DATA_NUM                8
#define ZBD_PEND_DATA_SEND_CNT_MAX              3

typedef union {
    u8 flags;
    struct {
        u8 secEnable:1;
        u8 resv:7;
    };
} zbd_tunnel_flags;

typedef struct {
    u8 *data;
    u8 dataLen;
    u8 sendCnt;
} zbd_data_pend_t;

/**
 * @brief  To initial the tunnel service.
 *
 * @param  handle - the handle of tunnel write characteristic
 *
 * @return none
 */
void zbd_tunnelInit(u16 handle);

/**
 * @brief  Get the type of current session.
 *
 * @param  connHandle  - ble connect handle
 * @param  attHandle   - attribute handle
 * @param  totalLen    - the total length of received data
 * @param  commandData - the received command data
 *
 * @return u8
 */
u8 zbd_tunnelNpduWriteHandle(u16 connHandle, u16 attHandle, u8 totalLen, u8 *commandData);

/**
 * @brief  Send the pending tunnel data
 *
 * @param  connHandle
 *
 * @return 0 - success
 */
u8 zbd_tunnelPendingDataSend(u16 connHandle);

/**
 * @brief  Clear the pending data for ble
 *
 * @param  none
 *
 * @return none
 */
void zbd_tunnelPendingDataClear(void);
#endif /* _ZBD_TUNNEL_H_ */
