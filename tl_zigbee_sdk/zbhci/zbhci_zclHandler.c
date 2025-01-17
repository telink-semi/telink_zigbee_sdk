/********************************************************************************************************
 * @file    zbhci_zclHandler.c
 *
 * @brief   This is the source file for zbhci_zclHandler
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
#include "zcl_include.h"
#include "ota.h"
#include "zbhci.h"
#if ZB_TEST_ENABLE
#include "zbhci_testcase.h"
#endif


#if ZBHCI_EN
const zbhciTxMode_e zbhciConvertSet[] = {
    APS_DSTADDR_EP_NOTPRESETNT, /* for bind, without address and Endpoint */
    APS_SHORT_GROUPADDR_NOEP,   /* for group-casting: only need group address */
    APS_SHORT_DSTADDR_WITHEP,   /* for unicasting with nwk address, with Endpoint */
    APS_LONG_DSTADDR_WITHEP,    /* for unicasting with ieee address, with Endpoint */
    APS_SHORT_DSTADDR_WITHEP,   /* for unicasting with nwk address, with Endpoint */
    APS_DSTADDR_EP_NOTPRESETNT,
    APS_SHORT_DSTADDR_WITHEP,
    APS_LONG_DSTADDR_WITHEP
};


void zbhciTxClusterCmdAddrResolve(epInfo_t *dstEpInfo, u8 *srcEp, u8 **payload)
{
    memset((u8 *)dstEpInfo, 0, sizeof(*dstEpInfo));

    zbhciTxMode_e apsTxMode = (zbhciTxMode_e)(**payload);
    (*payload)++;

    if ((apsTxMode < ZBHCI_ADDRMODE_BRC) &&
        (apsTxMode != ZBHCI_ADDRMODE_GROUP)) {
        dstEpInfo->txOptions |= APS_TX_OPT_ACK_TX;
    }
    dstEpInfo->profileId = 0x0104;//HA_PROFILE_ID
    dstEpInfo->dstAddrMode = (u8)zbhciConvertSet[apsTxMode];
    if (dstEpInfo->dstAddrMode == APS_LONG_DSTADDR_WITHEP) {
        ZB_IEEE_ADDR_REVERT(dstEpInfo->dstAddr.extAddr,*payload);
        (*payload) += EXT_ADDR_LEN;
    } else if (dstEpInfo->dstAddrMode == APS_DSTADDR_EP_NOTPRESETNT) {
        //COPY_BUFFERTOU16_BE(dstEpInfo->dstAddr.shortAddr,*payload);
        //(*payload) += sizeof(u16);
    } else if (dstEpInfo->dstAddrMode == APS_SHORT_GROUPADDR_NOEP ||
               dstEpInfo->dstAddrMode == APS_SHORT_DSTADDR_WITHEP) {
        COPY_BUFFERTOU16_BE(dstEpInfo->dstAddr.shortAddr, *payload);
        (*payload) += sizeof(u16);
    }
    *srcEp = **payload;
    (*payload)++;

    if (dstEpInfo->dstAddrMode == APS_LONG_DSTADDR_WITHEP ||
        dstEpInfo->dstAddrMode == APS_SHORT_DSTADDR_WITHEP) {
        dstEpInfo->dstEp = **payload;
        (*payload)++;
    }
}

/*
 * zbhci_zclIdentifyCmdHandle
 *
 * cluster of IDENTIFY handler
 *
 * */
void zbhci_zclIdentifyCmdHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u16 cmdId = cmdInfo->cmdId;
    u8 *pCmd = cmdInfo->payload;

    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo,&srcEp,&pCmd);

    if (cmdId == ZBHCI_CMD_ZCL_IDENTIFY) {
        u16 identifyTime;
        //u32 manuCode = 0x54555657;
        COPY_BUFFERTOU16_BE(identifyTime, pCmd);
        u32 manuCode = 0;
        zcl_identify_identifyCmd(srcEp, &dstEpInfo, FALSE, identifyTime, manuCode);
    } else if (cmdId == ZBHCI_CMD_ZCL_IDENTIFY_QUERY) {
        zcl_identify_identifyQueryCmd(srcEp, &dstEpInfo, FALSE);
    }

    ev_buf_free(arg);
}

/*
 * zbhci_zclOnoffCmdHandle
 *
 * cluster of ON/OFF handler
 *
 * */
void zbhci_zclOnoffCmdHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u16 cmdId = cmdInfo->cmdId;
    u8 *p = cmdInfo->payload;

    u8 *ptr = p;
    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo,&srcEp,&ptr);

    if (cmdId == ZBHCI_CMD_ZCL_ONOFF_ON) {
        zcl_onOff_onCmd(srcEp, &dstEpInfo, 0);
    } else if (cmdId == ZBHCI_CMD_ZCL_ONOFF_OFF) {
        zcl_onOff_offCmd(srcEp, &dstEpInfo, 0);
    } else if (cmdId == ZBHCI_CMD_ZCL_ONOFF_TOGGLE) {
        zcl_onOff_toggleCmd(srcEp, &dstEpInfo, 0);
    }

    ev_buf_free(arg);
}

/*
 * zbhci_zclLevelCtrlCmdHandle
 *
 * cluster of LEVEL handler
 *
 * */
void zbhci_zclLevelCtrlCmdHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u16 cmdId = cmdInfo->cmdId;
    u8 *p = cmdInfo->payload;

    u8 *ptr = p;
    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo,&srcEp,&ptr);

    switch (cmdId) {
    case ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL:
    case ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL_WITHONOFF:
        {
            u8 level = ptr[0];
            u16 transTime;
            COPY_BUFFERTOU16_BE(transTime, &ptr[1]);

            moveToLvl_t move2Level;
            move2Level.level = level;
            move2Level.transitionTime = transTime;
            move2Level.optPresent = 0;

            if (cmdId == ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL) {
                zcl_level_move2levelCmd(srcEp, &dstEpInfo, FALSE, &move2Level);
            } else {
                zcl_level_move2levelWithOnOffCmd(srcEp, &dstEpInfo, FALSE, &move2Level);
            }
        }
        break;

    case ZBHCI_CMD_ZCL_LEVEL_MOVE:
    case ZBHCI_CMD_ZCL_LEVEL_MOVE_WITHONOFF:
        {
            move_t move;
            move.moveMode = ptr[0];
            move.rate = ptr[1];
            move.optPresent = 0;

            if (cmdId == ZBHCI_CMD_ZCL_LEVEL_MOVE) {
                zcl_level_moveCmd(srcEp, &dstEpInfo, FALSE, &move);
            } else {
                zcl_level_moveWithOnOffCmd(srcEp, &dstEpInfo, FALSE, &move);
            }
        }
        break;

    case ZBHCI_CMD_ZCL_LEVEL_STEP:
    case ZBHCI_CMD_ZCL_LEVEL_STEP_WITHONOFF:
        {
            u8 stepMode = ptr[0];
            u8 stepSize = ptr[1];
            u16 transTime;
            COPY_BUFFERTOU16_BE(transTime, &ptr[2]);

            step_t step;
            step.stepMode = stepMode;
            step.stepSize = stepSize;
            step.transitionTime = transTime;
            step.optPresent = 0;

            if (cmdId == ZBHCI_CMD_ZCL_LEVEL_STEP) {
                zcl_level_stepCmd(srcEp, &dstEpInfo, FALSE, &step);
            } else {
                zcl_level_stepWithOnOffCmd(srcEp, &dstEpInfo, FALSE, &step);
            }
        }
        break;

    case ZBHCI_CMD_ZCL_LEVEL_STOP:
    case ZBHCI_CMD_ZCL_LEVEL_STOP_WITHONOFF:
        {
            stop_t stop;
            stop.optPresent = 0;

            if (cmdId == ZBHCI_CMD_ZCL_LEVEL_STOP) {
                zcl_level_stopCmd(srcEp, &dstEpInfo, FALSE, &stop);
            } else {
                zcl_level_stopWithOnOffCmd(srcEp, &dstEpInfo, FALSE, &stop);
            }
        }
        break;

    default:
        break;
    }

    ev_buf_free(arg);
}

//color
void zbhci_zclColorCtrlCmdHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u16 cmdId = cmdInfo->cmdId;
    u8 *p = cmdInfo->payload;

    u8 *ptr = p;
    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo, &srcEp, &ptr);

    switch (cmdId) {
    case ZBHCI_CMD_ZCL_COLOR_MOVE2HUE:
        {
            u8 hue = ptr[0];
            u8 direction = ptr[1];
            u16 transTime;
            COPY_BUFFERTOU16_BE(transTime, &ptr[2]);
			
            zcl_colorCtrlMoveToHueCmd_t move2Hue;
            move2Hue.hue = hue;
            move2Hue.direction = direction;
            move2Hue.transitionTime = transTime;
            move2Hue.optPresent = 0;

            zcl_lightColorCtrl_move2hueCmd(srcEp, &dstEpInfo, FALSE, &move2Hue);
        }
        break;

    case ZBHCI_CMD_ZCL_COLOR_MOVE2COLOR:
        {
            u16 colorX;
            u16 colorY; 
            u16 transTime;

            COPY_BUFFERTOU16_BE(colorX, &ptr[0]);
            COPY_BUFFERTOU16_BE(colorY, &ptr[2]);
            COPY_BUFFERTOU16_BE(transTime, &ptr[4]);

            zcl_colorCtrlMoveToColorCmd_t move2Color;
            move2Color.colorX = colorX;
            move2Color.colorY = colorY;
            move2Color.transitionTime = transTime;
            move2Color.optPresent = 0;

            zcl_lightColorCtrl_move2colorCmd(srcEp, &dstEpInfo, FALSE, &move2Color);
        }
        break;

    case ZBHCI_CMD_ZCL_COLOR_MOVE2SAT:
        {
            u8 saturation = ptr[0];
            u16 transTime;
            COPY_BUFFERTOU16_BE(transTime, &ptr[1]);

            zcl_colorCtrlMoveToSaturationCmd_t move2Sat;
            move2Sat.saturation = saturation;
            move2Sat.transitionTime = transTime;
            move2Sat.optPresent = 0;

            zcl_lightColorCtrl_move2saturationCmd(srcEp, &dstEpInfo, FALSE, &move2Sat);
        }
        break;

    case ZBHCI_CMD_ZCL_COLOR_MOVE2TEMP:
        {
            u16 colorTemperature;
            u16 transTime;

            COPY_BUFFERTOU16_BE(colorTemperature, &ptr[0]);
            COPY_BUFFERTOU16_BE(transTime, &ptr[2]);

            zcl_colorCtrlMoveToColorTemperatureCmd_t move2ColorTemp;
            move2ColorTemp.colorTemperature = colorTemperature;
            move2ColorTemp.transitionTime = transTime;
            move2ColorTemp.optPresent = 0;

            zcl_lightColorCtrl_move2colorTemperatureCmd(srcEp, &dstEpInfo, FALSE, &move2ColorTemp);
        }
        break;

    default:
        break;
    }
    ev_buf_free(arg);
}
//end

/*
 * zbhci_clusterGroupHandle
 *
 * cluster of GROUP handler
 *
 * */
void zbhci_clusterGroupHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u16 cmdId = cmdInfo->cmdId;
    u8 *pCmd = cmdInfo->payload;

    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo, &srcEp, &pCmd);

    if (cmdId == ZBHCI_CMD_ZCL_GROUP_ADD) {
        u16 groupID;
        COPY_BUFFERTOU16_BE(groupID, pCmd);
        pCmd += 2;
        u8 groupName[8];
        memcpy(groupName, pCmd, 8);
        zcl_group_addCmd(srcEp, &dstEpInfo, FALSE, groupID, groupName);
    } else if (cmdId == ZBHCI_CMD_ZCL_GROUP_VIEW) {
        u16 groupID;
        COPY_BUFFERTOU16_BE(groupID, pCmd);
        zcl_group_viewCmd(srcEp, &dstEpInfo, FALSE, groupID);
    } else if (cmdId == ZBHCI_CMD_ZCL_GROUP_GET_MEMBERSHIP) {
        u8 groupCnt = *pCmd;
        pCmd++;
        u16 groupList[8];
        int i = 0;
        for (i = 0; i < groupCnt && i < 8; i++) {
            COPY_BUFFERTOU16_BE(groupList[i], pCmd);
            pCmd += 2;
        }
        groupCnt = i;
        zcl_group_getMembershipCmd(srcEp, &dstEpInfo, FALSE, groupCnt, groupList);
    } else if (cmdId == ZBHCI_CMD_ZCL_GROUP_REMOVE) {
        u16 groupID;
        COPY_BUFFERTOU16_BE(groupID, pCmd);
        zcl_group_removeCmd(srcEp, &dstEpInfo, FALSE, groupID);
    } else if (cmdId == ZBHCI_CMD_ZCL_GROUP_REMOVE_ALL) {
        zcl_group_removeAllCmd(srcEp, &dstEpInfo, FALSE);
    } else if (cmdId == ZBHCI_CMD_ZCL_GROUP_ADD_IF_IDENTIFY) {
        u16 groupID;
        COPY_BUFFERTOU16_BE(groupID, pCmd);
        pCmd += 2;

        u8 groupName[8];
        memcpy(groupName, pCmd, 8);
        zcl_group_addIfIdentifyCmd(srcEp, &dstEpInfo, FALSE, groupID, groupName);
    }

    ev_buf_free(arg);
}

/*
 * zbhci_clusterSceneHandle
 *
 * cluster of Scene handler
 *
 * */
void zbhci_clusterSceneHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u16 cmdId = cmdInfo->cmdId;
    u8 *pCmd = cmdInfo->payload;

    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo, &srcEp, &pCmd);

    switch (cmdId) {
    case ZBHCI_CMD_ZCL_SCENE_ADD:
        {
            addScene_t *pAddScene = (addScene_t *)ev_buf_allocate(sizeof(addScene_t));
            if (!pAddScene) {
                break;
            }

            COPY_BUFFERTOU16_BE(pAddScene->scene.groupId, pCmd);
            pCmd += 2;
            pAddScene->scene.sceneId = *pCmd++;
            COPY_BUFFERTOU16_BE(pAddScene->scene.transTime, pCmd);
            pCmd += 2;
            u8 nameLen = *pCmd++;
            pAddScene->scene.sceneName[0] = (nameLen > (ZCL_MAX_SCENE_NAME_LEN - 1)) ? (ZCL_MAX_SCENE_NAME_LEN - 1) : nameLen;
            memcpy(&pAddScene->scene.sceneName[1], pCmd, pAddScene->scene.sceneName[0]);
            pCmd += nameLen;
            pAddScene->scene.extFieldLen = *pCmd++;
            if (pAddScene->scene.extFieldLen) {
                pAddScene->scene.extFieldLen = (pAddScene->scene.extFieldLen > ZCL_MAX_SCENE_EXT_FIELD_SIZE) ? ZCL_MAX_SCENE_EXT_FIELD_SIZE
						   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	   	    	 : pAddScene->scene.extFieldLen;
                memcpy(pAddScene->scene.extField, pCmd, pAddScene->scene.extFieldLen);
            }

            zcl_scene_addSceneCmd(srcEp, &dstEpInfo, FALSE, pAddScene);

            ev_buf_free((u8 *)pAddScene);
        }
        break;
    case ZBHCI_CMD_ZCL_SCENE_VIEW:
        {
            viewScene_t viewScene;

            COPY_BUFFERTOU16_BE(viewScene.groupId, pCmd);
            pCmd += 2;
            viewScene.sceneId = *pCmd++;

            zcl_scene_viewSceneCmd(srcEp, &dstEpInfo, FALSE, &viewScene);
        }
        break;
    case ZBHCI_CMD_ZCL_SCENE_REMOVE:
        {
            removeScene_t removeScene;

            COPY_BUFFERTOU16_BE(removeScene.groupId, pCmd);
            pCmd += 2;
            removeScene.sceneId = *pCmd++;

            zcl_scene_removeSceneCmd(srcEp, &dstEpInfo, FALSE, &removeScene);
        }
        break;
    case ZBHCI_CMD_ZCL_SCENE_REMOVE_ALL:
        {
            removeAllScene_t removeAllScene;

            COPY_BUFFERTOU16_BE(removeAllScene.groupId, pCmd);
            pCmd += 2;

            zcl_scene_removeAllSceneCmd(srcEp, &dstEpInfo, FALSE, &removeAllScene);
        }
        break;
    case ZBHCI_CMD_ZCL_SCENE_STORE :
        {
            storeScene_t storeScene;

            COPY_BUFFERTOU16_BE(storeScene.groupId, pCmd);
            pCmd += 2;
            storeScene.sceneId = *pCmd++;

            zcl_scene_storeSceneCmd(srcEp, &dstEpInfo, FALSE, &storeScene);
        }
        break;
    case ZBHCI_CMD_ZCL_SCENE_RECALL:
        {
            recallScene_t recallScene;

            COPY_BUFFERTOU16_BE(recallScene.groupId, pCmd);
            pCmd += 2;
            recallScene.sceneId = *pCmd++;
            recallScene.transTime = 0;

            zcl_scene_recallSceneCmd(srcEp, &dstEpInfo, FALSE, &recallScene);
        }
        break;
    case ZBHCI_CMD_ZCL_SCENE_GET_MEMBERSHIP:
        {
            getSceneMemship_t getSceneMemship;

            COPY_BUFFERTOU16_BE(getSceneMemship.groupId, pCmd);
            pCmd += 2;

            zcl_scene_getSceneMemshipCmd(srcEp, &dstEpInfo, FALSE, &getSceneMemship);
        }
        break;
    default:
        break;
    }

    ev_buf_free(arg);
}

void cust_ota_start_req(epInfo_t *dstEpInfo,u8 srcEp,u8 **payload)
{
    //u32 image_length;
    ota_hdrFields_t ota_hdr;
    u32 ota_start_addr = mcuBootAddrGet();

    if (ota_start_addr == 0xFFFFFFFF) {
        return;
    }

    ota_start_addr = (ota_start_addr) ? 0 : FLASH_ADDR_OF_OTA_IMAGE;

    flash_read(ota_start_addr, sizeof(ota_hdrFields_t), (u8*)&ota_hdr);
    //image_length = ota_hdr.totalImageSize;

    ota_imageNotify_t in;
    in.payloadType = **payload;
    (*payload) ++;
    in.imageType = ota_hdr.imageType;
    in.manuCode = ota_hdr.manufacturerCode;
    in.newFileVer = ota_hdr.fileVer;
    in.queryJitter = **payload;

    zcl_ota_imageNotifyCmdSend(srcEp, dstEpInfo, FALSE, &in);
}

void zbhci_clusterOTAHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    //u16 cmdId = cmdInfo->cmdId;
    u8 *pCmd = cmdInfo->payload;

    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo, &srcEp, &pCmd);

    cust_ota_start_req(&dstEpInfo, srcEp, &pCmd);

    ev_buf_free(arg);
}

volatile status_t basic_status = 0;
void zbhci_clusterBasicHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u8 *pCmd = cmdInfo->payload;

    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo, &srcEp, &pCmd);

    basic_status = zcl_basic_resetFactoryDftCmd(srcEp, &dstEpInfo, FALSE);
    ev_buf_free(arg);
}

void zbhci_clusterCommonCmdHandle(void *arg)
{
    zbhci_cmdHandler_t *cmdInfo = arg;
    u16 cmdId = cmdInfo->cmdId;
    u8 *pCmd = cmdInfo->payload;

    epInfo_t dstEpInfo;
    u8 srcEp;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    zbhciTxClusterCmdAddrResolve(&dstEpInfo, &srcEp, &pCmd);

    dstEpInfo.profileId = pCmd[1] | (pCmd[0] << 8);//get the profileId from the host(not HA_PROFILE_ID)
    pCmd += 2;

    u8 dir = *pCmd;
    pCmd++;
    u16 clusterId = pCmd[1] | (pCmd[0] << 8);//HCI uart command format is big-endian.
    pCmd += 2;
    u8 numAttr = *pCmd++;                 //!< number of attribute records in the list

    if (cmdId == ZBHCI_CMD_ZCL_ATTR_READ) {
        zclReadCmd_t *pReadCmd = (zclReadCmd_t *)ev_buf_allocate(sizeof(zclReadCmd_t) + numAttr * sizeof(u16));
        if (pReadCmd) {
            pReadCmd->numAttr = numAttr;

            for (u8 i = 0; i < numAttr; i++) {
                pReadCmd->attrID[i] = BUILD_U16(pCmd[1], pCmd[0]);
                pCmd += 2;
            }

            zcl_sendReadCmd(srcEp, &dstEpInfo, clusterId, TRUE, dir, pReadCmd);

            ev_buf_free((u8 *)pReadCmd);
        }
    } else if (cmdId == ZBHCI_CMD_ZCL_ATTR_WRITE) {
        u16 dataLen = 0;
        u16 attrDataLen = 0;
        u8 *pBuf = pCmd;

        for (u8 i = 0; i < numAttr; i++) {
            pBuf += 2;

            u8 dataType = *pBuf++;
            attrDataLen = zcl_getAttrSize(dataType, pBuf);
            pBuf += attrDataLen;

            dataLen += attrDataLen;
        }

        zclWriteCmd_t *pWriteCmd = (zclWriteCmd_t *)ev_buf_allocate(sizeof(zclWriteCmd_t) + numAttr * sizeof(zclWriteRec_t) + dataLen);
        if (pWriteCmd) {
            //point to actual attribute data.
            pBuf = (u8 *)pWriteCmd + sizeof(zclWriteCmd_t) + numAttr * sizeof(zclWriteRec_t);

            pWriteCmd->numAttr = numAttr;

            zclWriteRec_t *pWriteRec = NULL;
            for (u8 i = 0; i < numAttr; i++) {
                pWriteRec = &(pWriteCmd->attrList[i]);

                pWriteRec->attrID = BUILD_U16(pCmd[1], pCmd[0]);
                pCmd += 2;
                pWriteRec->dataType = *pCmd++;
                attrDataLen = zcl_getAttrSize(pWriteRec->dataType, pCmd);
                memcpy(pBuf, pCmd, attrDataLen);
                if ((pWriteRec->dataType != ZCL_DATA_TYPE_LONG_CHAR_STR) &&
                    (pWriteRec->dataType != ZCL_DATA_TYPE_LONG_OCTET_STR) &&
                    (pWriteRec->dataType != ZCL_DATA_TYPE_CHAR_STR) &&
                    (pWriteRec->dataType != ZCL_DATA_TYPE_OCTET_STR) &&
                    (pWriteRec->dataType != ZCL_DATA_TYPE_STRUCT)) {
                    ZB_LEBESWAP(pBuf, attrDataLen);
                }
                pWriteRec->attrData = pBuf;

                pBuf += attrDataLen;
                pCmd += attrDataLen;
            }

            zcl_sendWriteCmd(srcEp, &dstEpInfo, clusterId, TRUE, dir, pWriteCmd);

            ev_buf_free((u8 *)pWriteCmd);
        }
    } else if (cmdId == ZBHCI_CMD_ZCL_CONFIG_REPORT) {
        u16 dataLen = 0;
        u16 reportChangeLen = 0;
        u8 *pBuf = pCmd;

        for (u8 i = 0; i < numAttr; i++) {
            u8 direction = *pBuf++;
            pBuf += 2;

            if (direction == ZCL_SEND_ATTR_REPORTS) {
                u8 dataType = *pBuf++;
                pBuf += 4;

                if (zcl_analogDataType(dataType)) {
                    reportChangeLen = zcl_getDataTypeLen(dataType);
                    pBuf += reportChangeLen;
                    dataLen += reportChangeLen;
                } else {
                    pBuf++;
                }
            } else {
                pBuf += 2;
            }
        }

        zclCfgReportCmd_t *pCfgReportCmd = (zclCfgReportCmd_t *)ev_buf_allocate(sizeof(zclCfgReportCmd_t) + numAttr * sizeof(zclCfgReportRec_t) + dataLen);
        if (pCfgReportCmd) {
            //point to actual attribute data.
            pBuf = (u8 *)pCfgReportCmd + sizeof(zclCfgReportCmd_t) + numAttr * sizeof(zclCfgReportRec_t);

            pCfgReportCmd->numAttr = numAttr;

            zclCfgReportRec_t *pCfgReportRec = NULL;
            for (u8 i = 0; i < numAttr; i++) {
                pCfgReportRec = &(pCfgReportCmd->attrList[i]);

                pCfgReportRec->direction = *pCmd++;
                pCfgReportRec->attrID = BUILD_U16(pCmd[1], pCmd[0]);
                pCmd += 2;
                if (pCfgReportRec->direction == ZCL_SEND_ATTR_REPORTS) {
                    pCfgReportRec->dataType = *pCmd++;
                    pCfgReportRec->minReportInt = BUILD_U16(pCmd[1], pCmd[0]);
                    pCmd += 2;
                    pCfgReportRec->maxReportInt = BUILD_U16(pCmd[1], pCmd[0]);
                    pCmd += 2;
                    if (zcl_analogDataType(pCfgReportRec->dataType)) {
                        reportChangeLen = zcl_getDataTypeLen(pCfgReportRec->dataType);
                        memcpy(pBuf, pCmd, reportChangeLen);
                        if ((pCfgReportRec->dataType != ZCL_DATA_TYPE_LONG_CHAR_STR) &&
                            (pCfgReportRec->dataType != ZCL_DATA_TYPE_LONG_OCTET_STR) &&
                            (pCfgReportRec->dataType != ZCL_DATA_TYPE_CHAR_STR) &&
                            (pCfgReportRec->dataType != ZCL_DATA_TYPE_OCTET_STR) &&
                            (pCfgReportRec->dataType != ZCL_DATA_TYPE_STRUCT)) {
                            ZB_LEBESWAP(pBuf, reportChangeLen);
                        }
                        pCfgReportRec->reportableChange = pBuf;

                        pBuf += reportChangeLen;
                        pCmd += reportChangeLen;
                    }
                } else {
                    pCfgReportRec->timeoutPeriod = BUILD_U16(pCmd[1], pCmd[0]);
                    pCmd += 2;
                }
            }

            zcl_sendCfgReportCmd(srcEp, &dstEpInfo, clusterId, TRUE, dir, pCfgReportCmd);

            ev_buf_free((u8 *)pCfgReportCmd);
        }
    } else if (cmdId == ZBHCI_CMD_ZCL_READ_REPORT_CFG) {
        zclReadReportCfgCmd_t *pReadReportCfgCmd = (zclReadReportCfgCmd_t *)ev_buf_allocate(sizeof(zclReadReportCfgCmd_t) + numAttr * sizeof(zclReadReportCfgRec_t));
        if (pReadReportCfgCmd) {
            pReadReportCfgCmd->numAttr = numAttr;

            zclReadReportCfgRec_t *pReadReportCfgRec = NULL;
            for (u8 i = 0; i < numAttr; i++) {
                pReadReportCfgRec = &(pReadReportCfgCmd->attrList[i]);

                pReadReportCfgRec->direction = *pCmd++;
                pReadReportCfgRec->attrID = BUILD_U16(pCmd[1], pCmd[0]);
                pCmd += 2;
            }

            zcl_sendReadReportCfgCmd(srcEp, &dstEpInfo, clusterId, TRUE, dir, pReadReportCfgCmd);

            ev_buf_free((u8 *)pReadReportCfgCmd);
        }
    }

    ev_buf_free(arg);
}

#endif	/* ZBHCI_EN */
