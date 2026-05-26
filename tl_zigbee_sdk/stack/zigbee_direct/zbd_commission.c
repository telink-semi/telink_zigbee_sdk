/********************************************************************************************************
 * @file    zbd_commission.c
 *
 * @brief   This is the source file for zbd_commission
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
#include "zb_common.h"
#include "tl_common.h"
#include "zbd_session.h"
#include "zcl_include.h"
#include "ble.h"
#include "zbd_commission.h"

#if ZIGBEE_DIRECT_ENABLE
extern u8 permitJoinTimerChange;
u8 actionDomin = GENERAL_UNSPECIFIC_OPERATION;
u8 actionSubDomin = 0;
extern void ss_zdoNwkKeySwitch(u8 keySeqNum);
extern void bdb_factoryNewDevCfg(u8 touchLinkEn, u8 chan);

//128_bit_UUID = 16_bit_UUID * 2^96 + Bluetooth_Base_UUID, 0xfff7 * 2^96 + SERVICE_BASE_UUID
u8 commServiceUUID[16] = {0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,0x00,0x10,0x00,0x00,0xF7,0xFF,0x00,0x00};

/**
 * @brief The call back to application when the zigbee network parameter update
 */
app_zbd_commChange_t app_comm_status_change_cb;
app_zbd_advDataChange_t app_adv_data_change_cb;
void zbd_commissionInit(app_zbd_commChange_t commCb, app_zbd_advDataChange_t advCb)
{
    app_comm_status_change_cb = commCb;
    app_adv_data_change_cb = advCb;
}

u8 preconfLinkkeyJoin[16] = {0};
u8 joinedStatusPre = 0;
u8 openStatusPre = 0;
u16 nwkAddrPre = 0;
u8 nwkChannelPre = 0;
u8 permitJoinCntPre = 0;
u16 panIdPre = 0;
bool anJoinFlagPre = TRUE;

/* A ZDD shall send a notification in case one of the following changed on the device: Joined Status, Open/Close Status,
 * Network address, Network Channel.
 */
void zbd_zbNwkParaChangeDetect(void)
{
    u8 paraChange = 0;
    u8 advdataChange = 0;
    if (joinedStatusPre != zb_isDeviceJoinedNwk()) {
        joinedStatusPre = zb_isDeviceJoinedNwk();
        paraChange = 1;
        advdataChange = 1;
    }
    if (zb_isDeviceJoinedNwk()) {
        if (openStatusPre != g_zbMacPib.associationPermit) {
            openStatusPre = g_zbMacPib.associationPermit;
            paraChange = 2;
            advdataChange = 2;
        }
#if (ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE)
        if (permitJoinTimerChange) {
            permitJoinTimerChange = 0;
            paraChange = 3;
        }
#endif
        if (nwkAddrPre != NWK_NIB().nwkAddr) {
            nwkAddrPre = NWK_NIB().nwkAddr;
            paraChange = 4;
            advdataChange = 3;
        }
        if (nwkChannelPre != g_zbMacPib.phyChannelCur) {
            nwkChannelPre = g_zbMacPib.phyChannelCur;
            paraChange = 5;
        }
        if (panIdPre != NWK_NIB().panId) {
            panIdPre = NWK_NIB().panId;
            advdataChange = 4;
        }
    }
    if (paraChange) {
        DEBUG(ZIGBEE_DIRECT_DEBUG_EN,
              "zbd_zbNwkParaChangeDetect: paraChange = %d\n", paraChange);
        if (app_comm_status_change_cb) {
            app_comm_status_change_cb();
        }
    }

    if (advdataChange) {
        if (app_adv_data_change_cb) {
            app_adv_data_change_cb();
        }
    }
}

static void zbd_commissTlvFill(u8 typeId, u8 *data, u8 dataLen, u8 **ptr)
{
    (*ptr)[0] = typeId;
    (*ptr)[1] = dataLen - 1;
    memcpy(*ptr + 2, data, dataLen);
    (*ptr) += dataLen + 2;
}

static u8 zbd_commissTlvFind(u8 *pBuf, u16 bufSize, u8 typeId, u8 minLen, tlv_t **pTlv)
{
    u16 remainLen = bufSize;
    u8 *ptr = pBuf;
    bool find = FALSE;

    while (remainLen) {
        if (remainLen <= 2) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }

        u8 tlvType = *ptr++;
        u16 tlvLen = *ptr++;
        tlvLen += 1;//real TLV value length

        if (tlvType == typeId) {
            //Check for any duplicate.
            if (find && (typeId != G_TLV_MANUFACTURER_SPEC)) {
                return ZBD_STA_TLV_PARSE_ERROR;
            } else {
                if (!find) {
                    *pTlv = (tlv_t*)(ptr - 2);
                }
                find = TRUE;
            }
            //if less than the minimum required length
            if (tlvLen != minLen) {
                return ZBD_STA_TLV_PARSE_ERROR;
            }
        }
        ptr += tlvLen;

        if (ptr - pBuf > bufSize) {
            return ZBD_STA_TLV_PARSE_ERROR;
        } else {
            remainLen = bufSize - (ptr - pBuf);
        }
    }
    if (find == FALSE) {
        return ZBD_STA_TLV_NOT_FOUND;
    }

    return ZBD_STA_SUCCESS;
}

static void zbd_commissNwkStatusGet(zbd_comm_netStatus_t *nwkStatus)
{
    /* ZED lose connection with parent, the joined status is not commissioned */
    if (!zb_isDeviceJoinedNwk()) {
        if (BDB_STATE_GET() == BDB_STATE_IDLE) {
            nwkStatus->statusBits.joinedStatus = NWKSTATUS_NOT_COMMISSIONED;
        } else {
            nwkStatus->statusBits.joinedStatus = NWKSTATUS_COMMISSIONING_IN_PROGRESS;
        }
    } else {
        nwkStatus->statusBits.joinedStatus = NWKSTATUS_COMMISSIONED;
        nwkStatus->statusBits.openStatus = g_zbMacPib.associationPermit;
        if (ss_securityModeIsDistributed()) {
            nwkStatus->statusBits.networkType = DISTRIBUTED_NETWORK;
        } else {
            nwkStatus->statusBits.networkType = CENTRALIZED_NETWORK;
        }
    }
}

static ZBD_RET_E zbd_commissStatusGet(u16 attHandle, u8 *statusNoti, u8 *realLen, u8 ret)
{
    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_PROV | ZBD_SESS_TYPE_ADMIN_AUTH | ZBD_SESS_TYPE_BASIC_AUTH;
    u8 *ptr = statusNoti;

    zbd_comm_netStatus_t nwkStatus;
    memset(&nwkStatus, 0, sizeof(zbd_comm_netStatus_t));
    zbd_commissNwkStatusGet(&nwkStatus);

    zbd_commissTlvFill(NETWORK_STATUS_TYPE_ID, &nwkStatus.status, sizeof(zbd_comm_netStatus_t), (u8 **)&ptr);
    zbd_commissTlvFill(IEEE_ADDRESS_TYPE_ID, NIB_IEEE_ADDRESS(), EXT_ADDR_LEN, (u8 **)&ptr);
    if (!zb_isDeviceFactoryNew()) {
        u8 channelInfo[5] = {0};
        tl_zb_channelList_t* channelList = (tl_zb_channelList_t *)channelInfo;
        channelList->channelCnt = 1;
        channelList->channelMask[0] = (u32)(1 << g_zbMacPib.phyChannelCur);
        zbd_commissTlvFill(NETWORK_CHANNEL_TYPE_ID, (u8 *)channelList, 5, (u8 **)&ptr);
        zbd_commissTlvFill(EXTENDED_PANID_TYPE_ID, NWK_NIB().extPANId, EXT_ADDR_LEN, (u8 **)&ptr);
        zbd_commissTlvFill(SHORT_PANID_TYPE_ID, (u8 *)&NWK_NIB().panId, 2, (u8 **)&ptr);
        zbd_commissTlvFill(NWK_ADDRESS_TYPE_ID, (u8 *)&NWK_NIB().nwkAddr, 2, (u8 **)&ptr);
        zbd_commissTlvFill(TRUST_CENTER_ADDRESS_TYPE_ID, ss_ib.trust_center_address, EXT_ADDR_LEN, (u8 **)&ptr);

        u8 devType = af_nodeDevTypeGet();
        zbd_commissTlvFill(DEVICE_TYPE_TYPE_ID, (u8 *)&devType, sizeof(devType), (u8 **)&ptr);
        zbd_commissTlvFill(NWK_UPDATEID_TYPE_ID, (u8 *)&g_zbNIB.updateId, 1, (u8 **)&ptr);
        zbd_commissTlvFill(NWK_ACTIVE_KEY_SEQNUM_TYPE_ID, &ss_ib.activeKeySeqNum, 1, (u8 **)&ptr);
    }

    if (actionDomin != GENERAL_UNSPECIFIC_OPERATION) {
        zbd_comm_statuscode_t commSta;
        commSta.operCode = actionDomin;
        commSta.statusCode = ret;
        actionDomin = GENERAL_UNSPECIFIC_OPERATION;
        zbd_commissTlvFill(STATUS_CODE_TYPE_ID, (u8 *)&commSta, sizeof(zbd_comm_statuscode_t), (u8 **)&ptr);
    }

    u8 dataLen = (u8)(ptr-statusNoti);
    if (dataLen > COMMISSION_STATUS_COMMAND_LEN_MAX - 8) {
        *realLen = 0;
        memset(statusNoti, 0, COMMISSION_STATUS_COMMAND_LEN_MAX);
        return ZBD_STA_LEN_OUT_RANGE;
    }
    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, statusNoti, (u8 *)&dataLen, 1) == ZBD_STA_SUCCESS) {
        *realLen = dataLen;
    } else {
        *realLen = 0;
        memset(statusNoti, 0, COMMISSION_STATUS_COMMAND_LEN_MAX);
        return ZBD_STA_ENCRYPT_FAIL;
    }
    return ZBD_STA_SUCCESS;
}

ZBD_RET_E zbd_commissStatusReadHandle(u16 connHandle, u16 attHandle, u8 *statusNoti, u8 *realLen)
{
    ZBD_RET_E ret = ZBD_STA_SUCCESS;
    if (zbd_sessionStateGet() == ZBD_BLE_INTERFACE_OFF) {
        ret = ZBD_STA_ACCESS_FAILURE;
    } else {
        ret = zbd_commissStatusGet(attHandle, statusNoti, realLen, ZBD_STA_SUCCESS);
    }
    if (ret != ZBD_STA_SUCCESS) {
        *realLen = 0;
    }
    return ret;
}

ZBD_RET_E zbd_commissStatusNotifSend(u16 connHandle, u16 attHandle, u8 *statusNoti, u8 *realLen)
{
    if (zbd_commissStatusGet(attHandle, statusNoti, realLen, ZBD_STA_SUCCESS) == ZBD_STA_SUCCESS) {
        if (*realLen != 0) {
            blc_gatt_pushHandleValueNotify(connHandle, attHandle, statusNoti, *realLen);
            return ZBD_STA_SUCCESS;
        }
    }
    return ZBD_STA_ENCRYPT_FAIL;
}

u8 zbd_commissDomainStatusNotifSend(u16 connHandle, u16 attHandle, ZBD_RET_E retStatus)
{
    u8 *statusNoti = ev_buf_allocate(COMMISSION_STATUS_COMMAND_LEN_MAX);
    u8 realLen = 0;
    u8 ret = ZBD_STA_SUCCESS;
    if (statusNoti) {
        if (zbd_commissStatusGet(attHandle, statusNoti, &realLen, retStatus) == ZBD_STA_SUCCESS) {
            if (realLen != 0) {
                blc_gatt_pushHandleValueNotify(connHandle, attHandle, statusNoti, realLen);
            } else {
                ret = ZBD_STA_LEN_OUT_RANGE;
            }
        } else {
            ret = ZBD_STA_ENCRYPT_FAIL;
        }
        ev_buf_free(statusNoti);
    } else {
        ret = ZBD_STA_BUFF_FAILURE;
    }

    return ret;
}

static void zbd_commissClearNewDevPreConfig(void)
{
    //tl_bdbReset2FN();//Clear the previous configuration
    zbd_preAdminKeyDelete();
    zb_reset();
    bdb_commissionSetting_t *commissionSet = g_bdbCtx.commissionSettings;
    u8 touchlinkEn = g_bdbAttrs.nodeCommissioningCapability & BDB_NODE_COMMISSION_CAP_TOUCHLINK;
    bdb_factoryNewDevCfg(touchlinkEn, commissionSet->touchlinkChannel);
    bdb_linkKeyCfg(commissionSet, 1);
}

static ZBD_RET_E zbd_commissFormNetworkWriteProcess(u16 attHandle, u8 totalLen, u8 *commandData)
{
    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_PROV;
    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0) != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }

#if ZB_ED_ROLE
    return ZBD_STA_DEVICE_TYPE_NOT_ALLOWED;
#endif
    if (!zb_isDeviceFactoryNew()) {
        return ZBD_STA_NETWORK_STATE_NOT_ALLOWED;
    }

    zbd_commissClearNewDevPreConfig();

    if (totalLen == 0) {
        if (bdb_networkFormationStart() == BDB_STATE_COMMISSIONING_BUSY) {
            return ZBD_STA_PREPROCESS_ONGOING;
        } else {
            return ZBD_STA_COMMAND_HANDLING;
        }
    }

    u8 oft = 0;
    tlv_t *tlvData;
    u8 ret = ZBD_STA_SUCCESS;
    ret = zbd_commissTlvFind(commandData, totalLen, ADMIN_KEY_TYPE_ID, SEC_KEY_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        if (!ZB_SEC_KEY_IS_NULL(tlvData->value)) {
            zbd_preAdminKeySave2Flash(tlvData->value);
        } else {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, TRUST_CENTER_ADDRESS_TYPE_ID, EXT_ADDR_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
#if ZB_COORDINATOR_ROLE
        return ZBD_STA_TLV_PARSE_ERROR;
#else
        u8 tcAddr[8] = {0};
        memcpy(tcAddr, tlvData->value, EXT_ADDR_LEN);
        if (!ZB_IEEE_ADDR_IS_INVALID(tcAddr)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
#endif
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, EXTENDED_PANID_TYPE_ID, EXT_ADDR_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        if (ZB_EXTPANID_IS_ZERO(tlvData->value) || ZB_IS_64BIT_ADDR_INVALID(tlvData->value)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        memcpy(NWK_NIB().extPANId, tlvData->value, EXT_ADDR_LEN);
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, SHORT_PANID_TYPE_ID, 2, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u16 inputPanid = BUILD_U16(tlvData->value[0], tlvData->value[1]);
        if ((inputPanid == 0) || (inputPanid == 0xffff)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        MAC_IB()->panId = NWK_NIB().panId = inputPanid;
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NETWORK_CHANNEL_TYPE_ID, 5, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u32 channelMask = BUILD_U32(tlvData->value[1], tlvData->value[2], tlvData->value[3], tlvData->value[4]);
        g_bdbAttrs.primaryChannelSet = (channelMask & BDBC_TL_PRIMARY_CHANNEL_SET);
        g_bdbAttrs.secondaryChannelSet = (channelMask & BDBC_TL_SECONDARY_CHANNEL_SET);

        if ((g_bdbAttrs.primaryChannelSet == 0) && (g_bdbAttrs.secondaryChannelSet == 0)) {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NETWORK_KEY_TYPE_ID, SEC_KEY_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        if (!ZB_SEC_KEY_IS_NULL(tlvData->value)) {
            zb_preConfigNwkKey(tlvData->value, TRUE);
            ss_ib.preConfiguredKeyType &= (~SS_PRECONFIGURED_NWKKEY);
        } else {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, LINK_KEY_TYPE_ID, SEC_KEY_LEN + 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u8 *ptr = tlvData->value;
        ptr++;
        if (!ZB_SEC_KEY_IS_NULL(ptr)) {
            memcpy(preconfLinkkeyJoin, ptr, SEC_KEY_LEN);
            ss_ib.preConfiguredKeyType |= SS_PRECONFIGURED_GLOBALLINKKEY;
#if ZB_COORDINATOR_ROLE
            ss_ib.tcLinkKeyType = SS_GLOBAL_LINK_KEY;
            ss_ib.tcLinkKey = preconfLinkkeyJoin;
#else
            ss_ib.distributeLinkKey = preconfLinkkeyJoin;
#endif
        } else {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NWK_ADDRESS_TYPE_ID, SHORT_ADDR_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
#if ZB_ROUTER_ROLE
    	u16 nwkAddr = BUILD_U16(tlvData->value[0], tlvData->value[1]);
        if ((nwkAddr == 0x0000) || (ZB_NWK_IS_ADDRESS_BROADCAST(nwkAddr))) {
            return ZBD_STA_TLV_PARSE_ERROR;
        } else {
            NIB_NETWORK_ADDRESS() = nwkAddr;
        }
#endif
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NWK_UPDATEID_TYPE_ID, 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        g_zbNIB.updateId = tlvData->value[0];
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NWK_ACTIVE_KEY_SEQNUM_TYPE_ID, 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        ss_zdoNwkKeySwitch(tlvData->value[0]);
        oft += tlvData->len + 3;
    }

    if (oft != totalLen) {
        return ZBD_STA_TLV_PARSE_ERROR;
    }

    if (bdb_networkFormationStart() == BDB_STATE_COMMISSIONING_BUSY) {
        return ZBD_STA_PREPROCESS_ONGOING;
    }
    return ZBD_STA_COMMAND_HANDLING;;
}

#if (!ZB_COORDINATOR_ROLE)
extern u16 TL_ZB_ASSOCJOIN_PERMIT_PANID;
static ZBD_RET_E zbd_commissJoinAssociateHandle(u8 totalLen, u8 *commandData)
{
    u8 oft = 0;
    tlv_t *tlvData;
    u8 ret = ZBD_STA_SUCCESS;
    ret = zbd_commissTlvFind(commandData, totalLen, EXTENDED_PANID_TYPE_ID, EXT_ADDR_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        if (ZB_EXTPANID_IS_ZERO(tlvData->value) || ZB_IS_64BIT_ADDR_INVALID(tlvData->value)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        memcpy(NWK_NIB().extPANId, tlvData->value, EXT_ADDR_LEN);
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NETWORK_CHANNEL_TYPE_ID, 5, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u32 channelMask = BUILD_U32(tlvData->value[1], tlvData->value[2], tlvData->value[3], tlvData->value[4]);
        g_bdbAttrs.primaryChannelSet = (channelMask & BDBC_TL_PRIMARY_CHANNEL_SET);
        g_bdbAttrs.secondaryChannelSet = (channelMask & BDBC_TL_SECONDARY_CHANNEL_SET);

        if ((g_bdbAttrs.primaryChannelSet == 0) && (g_bdbAttrs.secondaryChannelSet == 0)) {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, LINK_KEY_TYPE_ID, SEC_KEY_LEN + 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u8 *ptr = tlvData->value;
        u8 keyType = *ptr & BIT(0);
        //u8 keyTime = *ptr & BIT(3);
        ptr++;
        memcpy(preconfLinkkeyJoin, ptr, CCM_KEY_SIZE);
        if (!ZB_SEC_KEY_IS_NULL(preconfLinkkeyJoin)) {
            ss_ib.tcLinkKey = preconfLinkkeyJoin;
            if (keyType == 0) {//global key
                ss_ib.tcLinkKeyType = SS_GLOBAL_LINK_KEY;
            } else {//unique key
                ss_ib.tcLinkKeyType = SS_UNIQUE_LINK_KEY;
            }
            ss_ib.distributeLinkKey = preconfLinkkeyJoin;
        } else {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NWK_ADDRESS_TYPE_ID, 2, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u16 netAddr = BUILD_U16(tlvData->value[0], tlvData->value[1]);

        /*R22:the network address is configured by parent, R23:the network address is specified by itself*/
        if ((netAddr != 0x0000) && (!ZB_NWK_IS_ADDRESS_BROADCAST(netAddr))) {
            NIB_NETWORK_ADDRESS() = g_zbMacPib.shortAddress = netAddr;
        } else {
            return ZBD_STA_DATA_OUT_RANGE;
        }

        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, TRUST_CENTER_ADDRESS_TYPE_ID, EXT_ADDR_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        memcpy(ss_ib.trust_center_address, tlvData->value, EXT_ADDR_LEN);
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, ADMIN_KEY_TYPE_ID, SEC_KEY_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        if (!ZB_SEC_KEY_IS_NULL(tlvData->value)) {
            zbd_preAdminKeySave2Flash(tlvData->value);
        } else {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    if (oft != (totalLen - 3)) {
        return ZBD_STA_TLV_PARSE_ERROR;
    }
    if (bdb_networkSteerStart() == BDB_STATE_COMMISSIONING_BUSY) {
        return ZBD_STA_PREPROCESS_ONGOING;
    }
    return ZBD_STA_COMMAND_HANDLING;
}

static ZBD_RET_E zbd_commissJoinRejoinHandle(u16 attHandle, u8 totalLen, u8 *commandData)
{
    TL_ZB_ASSOCJOIN_PERMIT_PANID = MAC_INVALID_PANID;

    u8 oft = 0;
    tlv_t *tlvData;
    u8 ret = ZBD_STA_SUCCESS;
    ret = zbd_commissTlvFind(commandData, totalLen, EXTENDED_PANID_TYPE_ID, EXT_ADDR_LEN, &tlvData);
    if (ret != ZBD_STA_SUCCESS) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else {
        if (ZB_EXTPANID_IS_ZERO(tlvData->value) || ZB_IS_64BIT_ADDR_INVALID(tlvData->value)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        memcpy(NWK_NIB().extPANId, tlvData->value, EXT_ADDR_LEN);
        oft += tlvData->len + 3;
    }

    u32 channelMask = 0;
    ret = zbd_commissTlvFind(commandData, totalLen, NETWORK_CHANNEL_TYPE_ID, 5, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        channelMask = BUILD_U32(tlvData->value[1], tlvData->value[2], tlvData->value[3], tlvData->value[4]);
        g_bdbAttrs.primaryChannelSet = (channelMask & BDBC_TL_PRIMARY_CHANNEL_SET);
        g_bdbAttrs.secondaryChannelSet = (channelMask & BDBC_TL_SECONDARY_CHANNEL_SET);

        if ((g_bdbAttrs.primaryChannelSet == 0) && (g_bdbAttrs.secondaryChannelSet == 0)) {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    u8 tcExtAddr[8] = {0};
    ret = zbd_commissTlvFind(commandData, totalLen, TRUST_CENTER_ADDRESS_TYPE_ID, EXT_ADDR_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if(ret == ZBD_STA_SUCCESS) {
        if (!ZB_IS_64BIT_ADDR_ZERO(tlvData->value)) {
            memcpy(tcExtAddr, tlvData->value, EXT_ADDR_LEN);
        } else {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, LINK_KEY_TYPE_ID, SEC_KEY_LEN + 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u8 *ptr = tlvData->value;
        u8 keyType = *ptr & BIT(0);
        //u8 keyTime = *ptr & BIT(3);
        ptr++;
        memcpy(preconfLinkkeyJoin, ptr, CCM_KEY_SIZE);
        if (!ZB_SEC_KEY_IS_NULL(preconfLinkkeyJoin)) {
            if (ZB_EXTPANID_IS_ZERO(tcExtAddr)) {
                ss_ib.tcLinkKey = preconfLinkkeyJoin;
                if (keyType == 0) {//global key
                    ss_ib.tcLinkKeyType = SS_GLOBAL_LINK_KEY;
                } else {//unique key
                    ss_ib.tcLinkKeyType = SS_UNIQUE_LINK_KEY;
                }
                ss_ib.distributeLinkKey = preconfLinkkeyJoin;
            } else {
                ss_dev_pair_set_t keyPair;
                memset((u8 *)&keyPair, 0, sizeof(ss_dev_pair_set_t));
                memcpy(keyPair.linkKey, ptr, 16);

                memcpy(keyPair.device_address, tcExtAddr, 8);

                keyPair.incomingFrameCounter = keyPair.outgoingFrameCounter = 0;
                keyPair.apsLinkKeyType = SS_UNIQUE_LINK_KEY;
                keyPair.keyAttr = SS_VERIFIED_KEY;
                keyPair.used = 1;
                keyPair.newFeatFlag = 1;

                ss_devKeyPairSave(&keyPair);
            }
        } else {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, ADMIN_KEY_TYPE_ID, SEC_KEY_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        if (!ZB_SEC_KEY_IS_NULL(tlvData->value)) {
            zbd_preAdminKeySave2Flash(tlvData->value);
        } else {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NETWORK_KEY_TYPE_ID, SEC_KEY_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        ss_nwkKeyStore(tlvData->value);
        oft += tlvData->len + 3;
    } else if (ret == ZBD_STA_TLV_NOT_FOUND) {
        zb_rejoinSecModeSet(REJOIN_INSECURITY); //a few example??
    }

    if (oft != (totalLen - 3)) { //3 is the length of join method
        return ZBD_STA_TLV_PARSE_ERROR;
    }
    if (zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration) != ZDO_SUCCESS) {
        return ZBD_STA_STACK_INTERNAL_ERROR;
    }
    return ZBD_STA_COMMAND_HANDLING;
}

static ZBD_RET_E zbd_commissJoinOutBandComm(u16 attHandle, u8 totalLen, u8 *commandData)
{
    TL_ZB_ASSOCJOIN_PERMIT_PANID = MAC_INVALID_PANID;

    u32 channelMask = 0;
    u8 workChannel = 0;
    u16 panid = 0x0000;
    u8 extPanid[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    u8 tcExtAddr[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    u16 nwkAddr = 0;
    u8 nwkKey[16] = {0};
    u8 channelCnt = 0;

    ss_dev_pair_set_t linkKey;
    memset(&linkKey, 0, sizeof(ss_dev_pair_set_t));

    u8 oft = 0;
    tlv_t *tlvData;
    u8 ret = ZBD_STA_SUCCESS;
    ret = zbd_commissTlvFind(commandData, totalLen, EXTENDED_PANID_TYPE_ID, EXT_ADDR_LEN, &tlvData);
    if (ret != ZBD_STA_SUCCESS) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else {
        if (ZB_EXTPANID_IS_ZERO(tlvData->value) || ZB_IS_64BIT_ADDR_INVALID(tlvData->value)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        memcpy(extPanid, tlvData->value, EXT_ADDR_LEN);
        oft += tlvData->len + 3;
    }
    ret = zbd_commissTlvFind(commandData, totalLen, SHORT_PANID_TYPE_ID, 2, &tlvData);
    if (ret != ZBD_STA_SUCCESS) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else {
        panid = BUILD_U16(tlvData->value[0], tlvData->value[1]);
        if ((panid == 0) || (panid == 0xffff)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        oft += tlvData->len + 3;
    }
    ret = zbd_commissTlvFind(commandData, totalLen, NETWORK_CHANNEL_TYPE_ID, 5, &tlvData);
    if (ret != ZBD_STA_SUCCESS) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else {
        channelMask = BUILD_U32(tlvData->value[1], tlvData->value[2], tlvData->value[3], tlvData->value[4]);
        for (u8 chan = TL_ZB_MAC_CHANNEL_START; chan < TL_ZB_MAC_CHANNEL_START + TL_ZB_MAC_CHANNEL_NUM; chan++) {
            if (channelMask & (1<<chan)) {
                workChannel = chan;
                channelCnt++;
            }
        }
        if (channelCnt != 1) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        oft += tlvData->len + 3;
    }
    ret = zbd_commissTlvFind(commandData, totalLen, NETWORK_KEY_TYPE_ID, SEC_KEY_LEN, &tlvData);
    if (ret != ZBD_STA_SUCCESS) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else {
        memcpy(nwkKey, tlvData->value, SEC_KEY_LEN);
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, TRUST_CENTER_ADDRESS_TYPE_ID, 8, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else {
        if (ret == ZBD_STA_SUCCESS) {
            memcpy(tcExtAddr, tlvData->value, EXT_ADDR_LEN);
            oft += tlvData->len + 3;
        }
    }
    ret = zbd_commissTlvFind(commandData, totalLen, LINK_KEY_TYPE_ID, SEC_KEY_LEN + 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        u8 *ptr = tlvData->value;
        u8 keyType = *ptr & BIT(0);
        u8 keyTime = *ptr & BIT(3);
        ptr++;
        if (!ZB_SEC_KEY_IS_NULL(ptr)) {
            if (keyType == 0) {//global key
                memcpy(preconfLinkkeyJoin, ptr, SEC_KEY_LEN);
                ss_ib.preConfiguredKeyType |= SS_PRECONFIGURED_GLOBALLINKKEY;

                ss_ib.tcLinkKeyType = SS_GLOBAL_LINK_KEY;
                ss_ib.tcLinkKey = preconfLinkkeyJoin;
                ss_ib.distributeLinkKey = preconfLinkkeyJoin;
            } else {//unique key
                memcpy(linkKey.linkKey, ptr, SEC_KEY_LEN);
                linkKey.apsLinkKeyType = SS_UNIQUE_LINK_KEY;
                if (keyTime == 0) {//permanemt key
                    linkKey.keyAttr = SS_UNVERIFIED_KEY;
                } else {
                    linkKey.keyAttr = SS_PROVISIONAL_KEY;
                }
                linkKey.used = 1;
                linkKey.newFeatFlag = 1;
                linkKey.outgoingFrameCounter = linkKey.incomingFrameCounter = 0;
            }
        } else {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NWK_ADDRESS_TYPE_ID, 2, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        nwkAddr = BUILD_U16(tlvData->value[0], tlvData->value[1]);
        if (nwkAddr == 0x0000) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }

        if (ZB_NWK_IS_ADDRESS_BROADCAST(nwkAddr)) {
            return ZBD_STA_TLV_PARSE_ERROR;
        }
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NWK_UPDATEID_TYPE_ID, 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        g_zbNIB.updateId = tlvData->value[0];
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, NWK_ACTIVE_KEY_SEQNUM_TYPE_ID, 1, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if(ret == ZBD_STA_SUCCESS) {
        u8 seqNum = tlvData->value[0];
        ss_ib.activeKeySeqNum = seqNum;
        ss_ib.activeSecureMaterialIndex = seqNum;
        oft += tlvData->len + 3;
    }

    ret = zbd_commissTlvFind(commandData, totalLen, ADMIN_KEY_TYPE_ID, SEC_KEY_LEN, &tlvData);
    if (ret == ZBD_STA_TLV_PARSE_ERROR) {
        return ZBD_STA_TLV_PARSE_ERROR;
    } else if (ret == ZBD_STA_SUCCESS) {
        if (!ZB_SEC_KEY_IS_NULL(tlvData->value)) {
            zbd_preAdminKeySave2Flash(tlvData->value);
        } else {
            return ZBD_STA_DATA_OUT_RANGE;
        }
        oft += tlvData->len + 3;
    }

    if (oft != (totalLen - 3)) { //3 is the length of join method
        return ZBD_STA_TLV_PARSE_ERROR;
    }

    while ((nwkAddr == 0x0000)||(ZB_NWK_IS_ADDRESS_BROADCAST(nwkAddr))) {
        nwkAddr = ZB_RANDOM();
    }

    u8 netType = SS_SEMODE_CENTRALIZED;
    if (ZB_IS_64BIT_ADDR_INVALID(tcExtAddr)) {
        netType = SS_SEMODE_DISTRIBUTED;
    }

    if (SUCCESS == bdb_join_direct(workChannel, panid, nwkAddr, extPanid, nwkKey, netType, 1, tcExtAddr)) {
        if (!ZB_IS_64BIT_ADDR_INVALID(tcExtAddr) && (linkKey.used)) {
            memcpy(linkKey.device_address, tcExtAddr, EXT_ADDR_LEN);
            ss_devKeyPairDelete(linkKey.device_address);
            ss_devKeyPairSave(&linkKey);
        }
    } else {
        return ZBD_STA_PREPROCESS_ONGOING;
    }
    return ZBD_STA_COMMAND_HANDLING;
}
#endif

static ZBD_RET_E zbd_commissJoinNetworkWriteProcess(u16 attHandle, u8 totalLen, u8 *commandData)
{
    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_PROV;
    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0) != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }
    if (totalLen < 3) {//join method tlv
        return ZBD_STA_LEN_OUT_RANGE;
    }

#if ZB_COORDINATOR_ROLE
    return ZBD_STA_DEVICE_TYPE_NOT_ALLOWED;
#else
    u8 ret = ZBD_STA_SUCCESS;
    tlv_t *tlvData;
    ret = zbd_commissTlvFind(commandData, totalLen, JOINING_METHOD_TYPE_ID, 1, &tlvData);
    if (ret != ZBD_STA_SUCCESS) {
        return ZBD_STA_TLV_PARSE_ERROR;
	}

    u8 joinMethod = tlvData->value[0];
    actionSubDomin = joinMethod;

    DEBUG(ZIGBEE_DIRECT_DEBUG_EN,
          "JoinNetwork: joinMethod = %d\n", joinMethod);
    switch (joinMethod) {
    case JOIN_MAC_ASSOCIATION:
        if (!zb_isDeviceFactoryNew()) {
            return ZBD_STA_NETWORK_STATE_NOT_ALLOWED;
        }
        zbd_commissClearNewDevPreConfig();
        return zbd_commissJoinAssociateHandle(totalLen, commandData);
    case JOIN_SECURE_REJOINING:
        if (zb_isDeviceFactoryNew()) {
            return ZBD_STA_NETWORK_STATE_NOT_ALLOWED;
        }
        return zbd_commissJoinRejoinHandle(attHandle, totalLen, commandData);
    case JOIN_OUT_OF_BAND_COMMISSIONING:
        if (!zb_isDeviceFactoryNew()) {
            return ZBD_STA_NETWORK_STATE_NOT_ALLOWED;
        }
        zbd_commissClearNewDevPreConfig();//Clear the previous configuration
        return zbd_commissJoinOutBandComm(attHandle, totalLen, commandData);
    default:
        return ZDB_STA_TLVID_UNSUPPORTED;
    }
#endif
}

static ZBD_RET_E zbd_commissPermitJoinWriteProcess(u16 attHandle, u8 totalLen, u8 *commandData)
{
    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_ADMIN_AUTH;
    if (g_zbdSession.unproZdd) {
        supportSsLevel |= ZBD_SESS_TYPE_PROV;
    }
    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0) != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }
    if (totalLen != 1) {
        return ZBD_STA_LEN_OUT_RANGE;
    }
#if ZB_ED_ROLE
    return ZBD_STA_DEVICE_TYPE_NOT_ALLOWED;
#endif
    if (zb_isDeviceFactoryNew()) {
        return ZBD_STA_NETWORK_STATE_NOT_ALLOWED;
    }

    u8 permitJoinValue = *commandData;
    u16 targetAddr = 0xffff;
    u8 sn = 0;
    zb_mgmtPermitJoinReq(targetAddr, permitJoinValue, 1, &sn, NULL);
    if ((!g_zbMacPib.associationPermit) && (!permitJoinValue)) {
        return ZBD_STA_SUCCESS;
    }
    return ZBD_STA_COMMAND_HANDLING;
}

static ZBD_RET_E zbd_commissLeaveNetworkWriteProcess(u16 attHandle, u8 totalLen, u8 *commandData)
{
    u8 *ptr = commandData;

    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_ADMIN_AUTH;
    if (g_zbdSession.unproZdd) {
        supportSsLevel |= ZBD_SESS_TYPE_PROV;
    }
    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0) != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }
    if (totalLen != 2) {
        return ZBD_STA_LEN_OUT_RANGE;
    }

    if (zb_isDeviceFactoryNew()) { //test4.1.6, should check incoming frame counter first
        return ZBD_STA_NETWORK_STATE_NOT_ALLOWED;
    }

    u8 removeChildren = *ptr++;
    u8 rejoin = *ptr++;
    nlme_leave_req_t req;

    memset(req.deviceAddr, 0, 8);
    req.removeChildren = removeChildren;
    req.rejoin = rejoin;

    if (zb_nlmeLeaveReq(&req)) {
        return ZBD_STA_STACK_INTERNAL_ERROR;
    }

    return ZBD_STA_COMMAND_HANDLING;
}

static ZBD_RET_E zbd_commissManageJoinersWriteProcess(u16 attHandle, u8 totalLen, u8 *commandData)
{
    u8 *ptr = commandData;

    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_ADMIN_AUTH;
    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0) != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }

#if (!ZB_COORDINATOR_ROLE)
    return ZBD_STA_DEVICE_TYPE_NOT_ALLOWED;
#endif

    if (totalLen < 3) { //at least include the command tlv
        return ZBD_STA_LEN_OUT_RANGE;
    }

    u8 ret = ZBD_STA_SUCCESS;
    u8 oft = 0;
    tlv_t *tlvData;
    ret = zbd_commissTlvFind(ptr, totalLen, MANAGE_JOINERS_COMMAND, 1, &tlvData);
    if (ret != ZBD_STA_SUCCESS) {
        return ZDB_STA_TLVID_UNSUPPORTED;
    }
    oft += tlvData->len + 3;
    u8 typeId = tlvData->value[0];

    switch (typeId) {
    case DROP_ALL_JOINERS_KEYS:
        {
            if (oft != totalLen) {
                return ZBD_STA_TLV_PARSE_ERROR;
            }
            ss_unverifiedKeypairDelete();
        }
        break;
    case ADD_A_JOINER_KEY:
        {
            ss_dev_pair_set_t preLinkKey;
            memset(&preLinkKey, 0, sizeof(ss_dev_pair_set_t));
            ret = zbd_commissTlvFind(ptr, totalLen, PROVISIONAL_LINK_KEY, CCM_KEY_SIZE, &tlvData);
            if (ret != ZBD_STA_SUCCESS) {
                return ZBD_STA_TLV_PARSE_ERROR;
            } else if (ret == ZBD_STA_SUCCESS) {
                memcpy(preLinkKey.linkKey, tlvData->value, CCM_KEY_SIZE);
                if (ZB_SEC_KEY_IS_NULL(preLinkKey.linkKey)) {
                    return ZBD_STA_TLV_PARSE_ERROR;
                }
                oft += tlvData->len + 3;
            }

            ret = zbd_commissTlvFind(ptr, totalLen, IEEE_ADDRESS, EXT_ADDR_LEN, &tlvData);
            if (ret != ZBD_STA_SUCCESS) {
                return ZBD_STA_TLV_PARSE_ERROR;
            } else if (ret == ZBD_STA_SUCCESS) {
                memcpy(preLinkKey.device_address, tlvData->value, EXT_ADDR_LEN);
                if (ZB_IS_64BIT_ADDR_INVALID(preLinkKey.device_address) ||
                   (ZB_IS_64BIT_ADDR_ZERO(preLinkKey.device_address))) {
                    return ZBD_STA_TLV_PARSE_ERROR;
                }
                oft += tlvData->len + 3;
            }

            if (oft != totalLen) {
                return ZBD_STA_TLV_PARSE_ERROR;
            }

            ss_dev_pair_set_t keyPair;
            if (NV_SUCC == ss_devKeyPairFind(preLinkKey.device_address, &keyPair)) {
            	if (keyPair.keyAttr == SS_VERIFIED_KEY) {
            		zdo_nlmeForgetDev(preLinkKey.device_address, 0);
            	} else {
					ss_devKeyPairDelete(preLinkKey.device_address);
            	}
            }

            preLinkKey.apsLinkKeyType = SS_UNIQUE_LINK_KEY;
            preLinkKey.keyAttr = SS_UNVERIFIED_KEY;
            preLinkKey.newFeatFlag = 1;
            preLinkKey.used = 1;
            ss_devKeyPairSave(&preLinkKey);
            g_bdbAttrs.joinUsesInstallCodeKey = 1;
        }
        break;
    case REMOVE_A_JOINER_KEY:
        {
            ret = zbd_commissTlvFind(ptr, totalLen, IEEE_ADDRESS, EXT_ADDR_LEN, &tlvData);
            if (ret == ZBD_STA_SUCCESS) {
                oft += tlvData->len + 3;
                if (oft != totalLen) {
                    return ZBD_STA_TLV_PARSE_ERROR;
                }
                ss_dev_pair_set_t keyPair;
                if ((NV_SUCC == ss_devKeyPairFind(tlvData->value, &keyPair)) && (keyPair.keyAttr == SS_UNVERIFIED_KEY)) {
                    ss_devKeyPairDelete(ptr);
                }
            } else {
                return ZDB_STA_TLVID_UNSUPPORTED;
            }
        }
        break;
    default:
        return ZDB_STA_TLVID_UNSUPPORTED;
        break;
    }

    return ZBD_STA_SUCCESS;
}

static ZBD_RET_E zbd_commissIdentifyWriteProcess(u16 attHandle, u8 totalLen, u8 *commandData)
{
    u8 *ptr = commandData;

    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_ADMIN_AUTH | ZBD_SESS_TYPE_BASIC_AUTH;
    if (g_zbdSession.unproZdd) {
        supportSsLevel |= ZBD_SESS_TYPE_PROV;
    }
    u8 secuRet = zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0);
    if (secuRet != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }
    if (totalLen < 2) { //at least include the command tlv
        return ZBD_STA_LEN_OUT_RANGE;
    }

    u16 idenTime = BUILD_U16(ptr[0], ptr[1]);
    ptr += 2;

    if (g_bdbCtx.bdbAppCb->bdbIdentifyCb) {
        g_bdbCtx.bdbAppCb->bdbIdentifyCb(g_bdbCtx.simpleDesc->endpoint, NIB_NETWORK_ADDRESS(), idenTime);
    } else {
        return ZBD_STA_STACK_INTERNAL_ERROR;
    }
    return ZBD_STA_SUCCESS;
}

static ZBD_RET_E zbd_commissIdentifyTimeGet(u16 attHandle, u8 *identifyData, u8 *realLen)
{
    u16 identifyTime = 0;
    u16 attrLen = 0;
    u8 dataLen = 0;

    if (zcl_getAttrVal(g_bdbCtx.simpleDesc->endpoint, ZCL_CLUSTER_GEN_IDENTIFY, ZCL_ATTRID_IDENTIFY_TIME, &attrLen, (u8 *)&identifyTime)) {
        *realLen = 0;
        return ZBD_STA_STACK_INTERNAL_ERROR;
    }

    identifyData[dataLen++] = LO_UINT16(identifyTime);
    identifyData[dataLen++] = HI_UINT16(identifyTime);

    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_ADMIN_AUTH | ZBD_SESS_TYPE_BASIC_AUTH;
    if (zb_isDeviceFactoryNew()) {
        supportSsLevel |= ZBD_SESS_TYPE_PROV;
    }

    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, identifyData, (u8 *)&dataLen, 1) == ZBD_STA_SUCCESS) {
        *realLen = dataLen;
    } else {
        *realLen = 0;
        return ZBD_STA_ENCRYPT_FAIL;
    }
    return ZBD_STA_SUCCESS;
}

void zbd_commissIdentifyTimeRead(u16 connHandle, u16 attHandle, u8 *identifyData, u8 *realLen)
{
    ZBD_RET_E ret = ZBD_STA_SUCCESS;
    if (zbd_sessionStateGet() == ZBD_BLE_INTERFACE_OFF) {
        ret = ZBD_STA_ACCESS_FAILURE;
    } else {
        ret = zbd_commissIdentifyTimeGet(attHandle, identifyData, realLen);
    }
    if (ret != ZBD_STA_SUCCESS) {
        *realLen = 0;
    }
}

static ZBD_RET_E zbd_commissFindbindWriteProcess(u16 attHandle, u8 totalLen, u8 *commandData)
{
    u8 *ptr = commandData;

    ZBD_SESS_TYPE_e supportSsLevel = ZBD_SESS_TYPE_ADMIN_AUTH | ZBD_SESS_TYPE_BASIC_AUTH;
    if (zbd_sessionSecureFrameProcess(attHandle, commServiceUUID, supportSsLevel, commandData, (u8 *)&totalLen, 0) != ZBD_STA_SUCCESS) {
        return ZBD_STA_DECRYPT_FAIL;
    }

    if (zb_isDeviceFactoryNew()) {
        return ZBD_STA_NETWORK_STATE_NOT_ALLOWED;
    }

    if (totalLen < 2) { //at least include the command tlv
        return ZBD_STA_LEN_OUT_RANGE;
    }

    u8 endpoint = *ptr++;
    u8 flag = *ptr;
    u8 role = 0;
    if ((flag) & BIT(0)) {
        role = BDB_COMMISSIONING_ROLE_INITIATOR;
    } else {
        role = BDB_COMMISSIONING_ROLE_TARGET;
    }

    af_simple_descriptor_t *pSimpleDesc = af_simpleDescGet(endpoint);
    if (pSimpleDesc) {
        g_bdbCtx.simpleDesc = pSimpleDesc;
    } else {
        return ZBD_STA_STACK_INTERNAL_ERROR;
    }
    if (bdb_findAndBindStart(role) == BDB_STATE_COMMISSIONING_BUSY) {
        return ZBD_STA_PREPROCESS_ONGOING;
    }
    return ZBD_STA_SUCCESS;
}

void zbd_extendAdvData(u8 *advDataExt, u8 *len, u8 suppTunnel)
{
    u8 advLen = *len;
    u16 serviceUuid = SERVICE_UUID_COMMISSIONING;

    if (31 - advLen >= DIRECT_ADV_EXTENSION_LEN) {
        advDataExt[advLen++] = 0x08; //length
        advDataExt[advLen++] = GAP_ADTYPE_SERVICE_DATA; //AD type
        advDataExt[advLen++] = LO_UINT16(serviceUuid);
        advDataExt[advLen++] = HI_UINT16(serviceUuid);

        zbd_adv_flag_t advFlag;
        advFlag.statusBits.version = 0x01;
        advFlag.statusBits.zdts = suppTunnel;
        if (g_zbMacPib.associationPermit) {
            advFlag.statusBits.permitJoin = 1;
        } else {
            advFlag.statusBits.permitJoin = 0;
        }
        advDataExt[advLen++] = advFlag.flag; //version and capabilities

        u16 panID = 0xffff;
        u16 nwkAddr = 0xffff;
        if (!zb_isDeviceFactoryNew()) {
            panID = NIB_PAN_ID();
            nwkAddr = NIB_NETWORK_ADDRESS();
        }
        advDataExt[advLen++] = LO_UINT16(panID);
        advDataExt[advLen++] = HI_UINT16(panID);
        advDataExt[advLen++] = LO_UINT16(nwkAddr);
        advDataExt[advLen++] = HI_UINT16(nwkAddr);
    }
    *len = advLen;
}

void zbd_commissionRetNotify(u16 connHandle, u16 attrHandle, u8 status)
{
    if (zbd_sessionStateGet() == ZBD_BLE_INTERFACE_OFF) {
        return;
    }
    if ((status == BDB_COMMISSION_STA_SUCCESS) || (status == BDB_COMMISSION_STA_FORMATION_DONE)) {
        /*the other domains will send notification because of the change of network parameter*/
        if (actionDomin == FINDING_BINDING_DOMAIN) {
            zbd_commissDomainStatusNotifSend(connHandle, attrHandle, ZBD_STA_SUCCESS);
            actionDomin = GENERAL_UNSPECIFIC_OPERATION;
        }
    } else {
    	if ((actionDomin == JOIN_NETWORK_OPERATION) && (actionSubDomin = JOIN_OUT_OF_BAND_COMMISSIONING)) {
    		zbd_commissClearNewDevPreConfig();
    		actionSubDomin = 0;
    	}
        zbd_commissDomainStatusNotifSend(connHandle, attrHandle, ZBD_STA_STACK_INTERNAL_ERROR);
        actionDomin = GENERAL_UNSPECIFIC_OPERATION;
    }
}

u8 zbd_commissionCharaWriteHandle(u16 connHandle, u16 attHandle, u16 statusHandle, u16 operCode, u8 totalLen, u8 *commandData)
{
    DEBUG(ZIGBEE_DIRECT_DEBUG_EN, "zbd_commission: operCode = %d, data:", operCode);

    ZBD_RET_E ret = ZBD_STA_SUCCESS;
    if (zbd_sessionStateGet() == ZBD_BLE_INTERFACE_OFF) {
        ret = ZBD_STA_ACCESS_FAILURE;
    } else {
        actionDomin = operCode;
        switch (operCode) {
        case FORM_NETWORK_OPERATION:
            ret = zbd_commissFormNetworkWriteProcess(attHandle, totalLen, commandData);
            break;
        case JOIN_NETWORK_OPERATION:
            ret = zbd_commissJoinNetworkWriteProcess(attHandle, totalLen, commandData);
            break;
        case PERMIT_JOINING_OPERATION:
            ret = zbd_commissPermitJoinWriteProcess(attHandle, totalLen, commandData);
            break;
        case LEAVE_NETWORK_OPERATION:
            ret = zbd_commissLeaveNetworkWriteProcess(attHandle, totalLen, commandData);
            break;
        case MANAGE_JOINERS_DOMAIN:
            ret = zbd_commissManageJoinersWriteProcess(attHandle, totalLen, commandData);
            break;
        case IDENTIFY_OPERATION:
            ret = zbd_commissIdentifyWriteProcess(attHandle, totalLen, commandData);
            break;
        case FINDING_BINDING_DOMAIN:
            ret = zbd_commissFindbindWriteProcess(attHandle, totalLen, commandData);
            break;
        default:
            ret = ZBD_STA_OPERCODE_INVALID;
            break;
        }
    }
    DEBUG(ZIGBEE_DIRECT_DEBUG_EN,
          "zbd_commission: operCode = %d, ret = 0x%x\n", operCode, ret);
    //6.4.7.3--6.4.7.4
    if ((ret == ZBD_STA_DECRYPT_FAIL) || (ret == ZBD_STA_ACCESS_FAILURE) || (ret == ZBD_STA_OPERCODE_INVALID)) {
    	actionDomin = GENERAL_UNSPECIFIC_OPERATION;
    	return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
    } else {
        if (ret != ZBD_STA_COMMAND_HANDLING) {
            if (zbd_commissDomainStatusNotifSend(connHandle, statusHandle, ret)) {
            	actionDomin = GENERAL_UNSPECIFIC_OPERATION;
            	return ATT_ERR_ZIGBEE_DIRECT_APP_ERROR;
            }
        }
    }
    return ATT_SUCCESS;
}
#endif
