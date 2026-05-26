/********************************************************************************************************
 * @file    zbd_tunnel.c
 *
 * @brief   This is the source file for zbd_tunnel
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
#include "zbd_tunnel.h"
#include "zb_common.h"
#include "zbd_session.h"
#include "tl_common.h"
#include "ble.h"

#if ZIGBEE_DIRECT_TUNNEL_ENABLE
u8 tunnelSrviceUUID[16] = {ZBD_TUNNEL_UUID_SERVICE};

zbd_data_pend_t pendData[ZBD_TUNNEL_PEND_DATA_NUM];
u16 zbd_tunnelDpHandle = 0;
u8 txDataReadPtr = 0;
u8 txDataWritePtr = 0;

static u8 zbd_tunnelDataPush(u8 *linkNpdu, u8 dataLen)
{
    if (txDataWritePtr - txDataReadPtr >= ZBD_TUNNEL_PEND_DATA_NUM) {
        ev_buf_free(linkNpdu);
        return ZBD_STA_FAILURE;
    } else {
        u8 dataIdx = txDataWritePtr++ % ZBD_TUNNEL_PEND_DATA_NUM;
        pendData[dataIdx].data = linkNpdu;
        pendData[dataIdx].dataLen = dataLen;
        pendData[dataIdx].sendCnt = 0;
    }

    return ZBD_STA_SUCCESS;
}

void zbd_tunnelPendingDataClear(void)
{
    zvdJoinState = ZVD_JOIN_IDLE;
    while (txDataWritePtr != txDataReadPtr) {
        u8 readIdx = txDataReadPtr % ZBD_TUNNEL_PEND_DATA_NUM;
        if (pendData[readIdx].data) {
            ev_buf_free(pendData[readIdx].data);
        }
        memset(&pendData[readIdx], 0, sizeof(zbd_data_pend_t));
        txDataReadPtr++;
    }
}

u8 zbd_tunnelPendingDataSend(u16 connHandle)
{
    if (txDataWritePtr != txDataReadPtr) {
        u8 readIdx = txDataReadPtr % ZBD_TUNNEL_PEND_DATA_NUM;
        u8 retBle = blc_gatt_pushHandleValueIndicate(connHandle, zbd_tunnelDpHandle, pendData[readIdx].data, pendData[readIdx].dataLen);
        if (retBle == GATT_ERR_PREVIOUS_INDICATE_DATA_HAS_NOT_CONFIRMED) {
            return 0;
        } else if (GATT_ERR_DATA_PENDING_DUE_TO_SERVICE_DISCOVERY_BUSY == retBle ) {
#if defined(MCU_CORE_8258) || defined(MCU_CORE_B91)
            extern void blc_att_setServerDataPendingTime_upon_ClientCmd(u8 num_10ms);
            blc_att_setServerDataPendingTime_upon_ClientCmd(0);
#else
            blc_gap_setSingleServerDataPendingTime_upon_ClientCmd(connHandle, 0);
#endif
            blc_gatt_pushHandleValueIndicate (connHandle, zbd_tunnelDpHandle, pendData[readIdx].data, pendData[readIdx].dataLen);
        }

        if (retBle == ZBD_STA_SUCCESS) {
            if (pendData[readIdx].data) {
                ev_buf_free(pendData[readIdx].data);
            }
            memset(&pendData[readIdx], 0, sizeof(zbd_data_pend_t));
            txDataReadPtr++;
        } else {
            if (pendData[readIdx].sendCnt++ >= ZBD_PEND_DATA_SEND_CNT_MAX) {
                txDataReadPtr++;
                if (pendData[readIdx].data) {
                    ev_buf_free(pendData[readIdx].data);
                }
                memset(&pendData[readIdx], 0, sizeof(zbd_data_pend_t));
            }
        }
    }
    return 0;
}

static u8 zbd_tunnelPostReqHandle(void *arg, bool encrypt)
{
    zb_mcps_data_req_t *req = (zb_mcps_data_req_t *)arg;
    u8 *npdu = req->msdu;
    u8 allocLen = req->msduLength + 4 + 8;

    nwkFrameCtrl_t frameControl;
    memcpy(&frameControl, npdu, sizeof(nwkFrameCtrl_t));

    if (zbd_sessionStateGet() == ZBD_BLE_INTERFACE_OFF) {
        return ZBD_STA_ACCESS_FAILURE;
    }
    if (zbd_sessionIsLimitedGet() && encrypt) {
        return ZBD_STA_DATA_OUT_RANGE;
    }

    u8 *linkNpdu = (u8 *)ev_buf_allocate(allocLen);//flag + npdu length + npdu + security + type + length
    if (!linkNpdu) {
        return ZBD_STA_BUFF_FAILURE;
    }
    u8 *ptr = linkNpdu;
    *ptr++ = ZBD_TUNNEL_NPDU_MESSAGE_ID;
    *ptr++ = req->msduLength + 1;
    *ptr++ = encrypt;   //Assuming encryption or not
    *ptr++ = req->msduLength;
    memcpy(ptr, npdu, req->msduLength);
    ptr += req->msduLength;

    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_PROV | ZBD_SESS_TYPE_BASIC_AUTH;

    u8 dataLen = (u8)(ptr - linkNpdu);
    DEBUG(ZIGBEE_DIRECT_DEBUG_EN, "zbd_tunnelPostReqHandle,data:");
    DEBUG_ARRAY(ZIGBEE_DIRECT_DEBUG_EN, linkNpdu, dataLen);
    if (zbd_sessionSecureFrameProcess(zbd_tunnelDpHandle, tunnelSrviceUUID, supportSsLevel, linkNpdu, (u8*)&dataLen, 1) == ZBD_STA_SUCCESS) {
        zbd_tunnelDataPush(linkNpdu, dataLen);
        return ZBD_STA_SUCCESS; //ZBD_STA_BLE_DATA_PENDING
    } else {
        ev_buf_free(linkNpdu);
        return ZBD_STA_INSUFF_AUTH_LEVEL;
    }
    ev_buf_free(linkNpdu);
    return ZBD_STA_SUCCESS;
}

static ZBD_RET_E zbd_tunnelNpduWriteDataHandle(u16 connHandle, u16 attHandle, u8 totalLen, u8 *commandData)
{
    if (zbd_sessionStateGet() == ZBD_BLE_INTERFACE_OFF) {
        return ZBD_STA_ACCESS_FAILURE;
    }

    if (totalLen < 2) { //at least include the command tlv
        return ZBD_STA_LEN_OUT_RANGE;
    }
    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_PROV | ZBD_SESS_TYPE_BASIC_AUTH;

    u8 secuRet = zbd_sessionSecureFrameProcess(attHandle, tunnelSrviceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0);
    if (secuRet != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }
    u8 *ptr = commandData;
    if (*ptr++ == ZBD_TUNNEL_NPDU_MESSAGE_ID) {
        ptr++;//length field
        u8 securityEnable = *ptr++;
        u8 npduLen = *ptr++;
        if (zbd_sessionIsLimitedGet() && securityEnable) {
            return ZBD_STA_STACK_INTERNAL_ERROR;
        } else {
            DEBUG(ZIGBEE_DIRECT_DEBUG_EN, "zbd_tunnelNpduWriteDataHandle,securityEnable:%d,data:", securityEnable);
            DEBUG_ARRAY(ZIGBEE_DIRECT_DEBUG_EN, ptr, npduLen);
            if (tl_zbTLinkRecvDataHandler(ptr, npduLen, securityEnable) != 0) {
                return ZBD_STA_STACK_INTERNAL_ERROR;
            }
        }
    } else {
        return ZDB_STA_TLVID_UNSUPPORTED;
    }
    return ZBD_STA_SUCCESS;
}

u8 zbd_tunnelNpduWriteHandle(u16 connHandle, u16 attHandle, u8 totalLen, u8 *commandData)
{
    ZBD_RET_E ret = zbd_tunnelNpduWriteDataHandle(connHandle, attHandle, totalLen, commandData);
    if (ret) {
        return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    }

    return ATT_SUCCESS;
}

void zbd_tunnelInit(u16 handle)
{
    zbd_tunnelDpHandle = handle;
    memset(pendData, 0, sizeof(zbd_data_pend_t)*ZBD_TUNNEL_PEND_DATA_NUM);
    tl_zbdPostproReqRegister(zbd_tunnelPostReqHandle);
}
#endif
