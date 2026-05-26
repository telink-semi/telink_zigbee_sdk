/********************************************************************************************************
 * @file    zcl_zbdCb.c
 *
 * @brief   This is the source file for zcl_zbdCb
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

#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"

#ifdef ZCL_ZBD_CONFIG
#include "zbd_include.h"

ev_timer_event_t *sampleLightZbdAnjoinTimerEvt = NULL;
u32 anJoinTimeCnt = 0;
s32 sampleLightAnJoinTimerCb(void* arg)
{
    anJoinTimeCnt--;

    if(anJoinTimeCnt == 0){
        sampleLightZbdAnjoinTimerEvt = NULL;
        zbd_sessionAnjFlagSet(FALSE);
        return -1;
    }
    return 0;
}

void sampleLightAnJoinTimerStart(void)
{
    if(g_zcl_zbdAttrs.anJoinTimeout != 0){
        zbd_sessionAnjFlagSet(TRUE);
        if(sampleLightZbdAnjoinTimerEvt){
            TL_ZB_TIMER_CANCEL(&sampleLightZbdAnjoinTimerEvt);
        }
        if(g_zcl_zbdAttrs.anJoinTimeout != 0xffffff){
            anJoinTimeCnt = g_zcl_zbdAttrs.anJoinTimeout;
            sampleLightZbdAnjoinTimerEvt = TL_ZB_TIMER_SCHEDULE(sampleLightAnJoinTimerCb, NULL, 1000);
        }
    }else{
        zbd_sessionAnjFlagSet(FALSE);
        if(sampleLightZbdAnjoinTimerEvt){
            TL_ZB_TIMER_CANCEL(&sampleLightZbdAnjoinTimerEvt);
        }
    }
}

extern int app_zbd_advDataUpdateHandle(void);
void app_zbd_interfaceStateUpdate(bool state)
{
    app_zbd_advDataUpdateHandle();
    if(state == ZBD_INTERFACE_ENABLE){
        zdb_sessionStateSet(ZBD_OPEN_TO_CONNECT_ZVD);
    }else{
        zdb_sessionStateSet(ZBD_BLE_INTERFACE_OFF);
    }
}

void sampleLight_zbd_setInterState(u8 state)
{
    zcl_zbdAttr_t *pState = zcl_zbdAttrGet();
    pState->interfaceState = state;

    zcl_zbdInterfaceAttr_save();
    app_zbd_interfaceStateUpdate(state);
}

void sampleLight_zbd_setAJoinTimeout(u24 timeout)
{
    zcl_zbdAttr_t *pState = zcl_zbdAttrGet();

    pState->anJoinTimeout = timeout;
    zcl_zbdAnjoinTimeoutAttr_save();
    sampleLightAnJoinTimerStart();
}

/*********************************************************************
 * @fn      sampleLight_onOffCb
 *
 * @brief   Handler for ZCL ONOFF command. This function will set ONOFF attribute first.
 *
 * @param   pAddrInfo
 * @param   cmdId - onoff cluster command id
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t sampleLight_zbdCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    if((pAddrInfo->dstEp == SAMPLE_LIGHT_ENDPOINT) && (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR)){
        switch(cmdId){
            case ZCL_CMD_ZBD_SET_IF_STATE:
                sampleLight_zbd_setInterState(((zcl_zbd_cmdPayload_t*)cmdPayload)->interface.state);
                break;
            case ZCL_CMD_ZBD_SET_AJOIN_TIMEOUT:
                sampleLight_zbd_setAJoinTimeout(((zcl_zbd_cmdPayload_t*)cmdPayload)->anJoinTimeout.time);
                break;
            default:
                break;
        }
    }

    return ZCL_STA_SUCCESS;
}
#endif
#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
