/********************************************************************************************************
 * @file    gp_base.h
 *
 * @brief   This is the header file for gp_base
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#ifndef GP_BASE_H
#define GP_BASE_H


typedef struct {
    ev_timer_event_t *transmitChannelTimeoutEvt;
    u8 gpOperationalChannel;    //hold the operational channel
    u8 gpDuplicateTimeout;      //the default value of 2 seconds can be modified by the application profile
} gp_base_t;

extern gp_base_t g_gpBaseCtx;


void gpSwitchToTransmitChannel(u8 operationChannel, u8 tempMasterTxChannel);
void gpTransmitChannelTimeoutStop(void);
u16 gpAliasSrcAddrDerived(u8 appId, gpdId_t gpdId);
u8 gppTunnelingDelayGet(bool rxAfterTx, u8 lqi, bool firstToForward, bool noRoute);
void gpTranimitGPDF(gp_data_req_t *pGpDataReq);
void gpDevAnnceAliasSend(u16 aliasNwkAddr);
bool gpDataIndDuplicateFind(u8 appId, gpdId_t gpdId, u32 secFrameCounter, u8 handle);
bool gpDataTunneledDuplicateCheck(u8 appId, gpdId_t gpdId, u32 secFrameCounter);
bool gpSecKeyTypeMappingChk(u8 secKeyTypeInProxy, u8 secKeyTypeFromGPDF);
gpSecRsp_status_t gpKeyRecovery(u8 gpdfKeyType, u8 entryKeyType, u8 *entryKey, gpSecRsp_status_t status, u8 *pKeyType, u8 *pKey);

#endif	/* GP_BASE_H */
