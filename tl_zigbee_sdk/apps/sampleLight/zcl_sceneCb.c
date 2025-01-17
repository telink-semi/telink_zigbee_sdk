/********************************************************************************************************
 * @file    zcl_sceneCb.c
 *
 * @brief   This is the source file for zcl_sceneCb
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
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


/*********************************************************************
 * @fn      sampleLight_sceneRecallReqHandler
 *
 * @brief   Handler for ZCL scene recall command. This function will recall scene.
 *
 * @param	pApsdeInd
 * @param	pScene
 *
 * @return  None
 */
static void sampleLight_sceneRecallReqHandler(zclIncomingAddrInfo_t *pAddrInfo, zcl_sceneEntry_t *pScene)
{
    u8 *pData = pScene->extField;
    u16 clusterID = 0xFFFF;
    u8 extLen = 0;

    while (pData < pScene->extField + pScene->extFieldLen) {
        clusterID = BUILD_U16(pData[0], pData[1]);
        pData += 2;//cluster id

        extLen = *pData++;//length

#ifdef ZCL_ON_OFF
        if (clusterID == ZCL_CLUSTER_GEN_ON_OFF) {
            if (extLen >= 1) {
                u8 onOff = *pData++;

                sampleLight_onOffCb(pAddrInfo, onOff, NULL);
                extLen--;
            }
        } else
#endif
#ifdef ZCL_LEVEL_CTRL
        if (clusterID == ZCL_CLUSTER_GEN_LEVEL_CONTROL) {
            if (extLen >= 1) {
                moveToLvl_t moveToLevel = {0};
                moveToLevel.optPresent = 0;
                moveToLevel.level = *pData++;
                moveToLevel.transitionTime = pScene->transTime * 10 + pScene->transTime100ms;

                sampleLight_levelCb(pAddrInfo, ZCL_CMD_LEVEL_MOVE_TO_LEVEL, &moveToLevel);
                extLen--;
            }
        }else
#endif
#ifdef ZCL_LIGHT_COLOR_CONTROL
        if (clusterID == ZCL_CLUSTER_LIGHTING_COLOR_CONTROL) {
            if (extLen >= 13) {
#if COLOR_RGB_SUPPORT && !defined(COLOR_X_Y_DISABLE)
                zcl_colorCtrlMoveToColorCmd_t move2Color = {0};
                move2Color.optPresent = 0;
                move2Color.colorX = BUILD_U16(pData[0], pData[1]);
                move2Color.colorY = BUILD_U16(pData[2], pData[3]);
                move2Color.transitionTime = pScene->transTime * 10 + pScene->transTime100ms;
#endif
                pData += 4;
                extLen -= 4;//x + y

#if COLOR_RGB_SUPPORT && !defined(COLOR_X_Y_DISABLE)
                if (move2Color.colorX || move2Color.colorY) {
                    sampleLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR, &move2Color);
                } else
#endif
                {
#if COLOR_RGB_SUPPORT
                    zcl_colorCtrlEnhancedMoveToHueAndSaturationCmd_t move2EnhHueAndSat = {0};
                    move2EnhHueAndSat.optPresent = 0;
                    move2EnhHueAndSat.enhancedHue = BUILD_U16(pData[0], pData[1]);
                    move2EnhHueAndSat.saturation = pData[2];
                    move2EnhHueAndSat.transitionTime = pScene->transTime * 10 + pScene->transTime100ms;
#endif
                    pData += 3;
                    extLen -= 3;//enhHue + sat
#if COLOR_RGB_SUPPORT
                    zcl_colorCtrlColorLoopSetCmd_t colorLoopSet = {0};
                    colorLoopSet.optPresent = 0;
                    colorLoopSet.updateFlags.bits.action = 1;
                    colorLoopSet.updateFlags.bits.direction = 1;
                    colorLoopSet.updateFlags.bits.time = 1;
                    colorLoopSet.action = pData[0] ? COLOR_LOOP_SET_ACTION_FROM_ENHANCED_CURRENT_HUE : COLOR_LOOP_SET_DEACTION;
                    colorLoopSet.direction = pData[1];
                    colorLoopSet.time = BUILD_U16(pData[2], pData[3]);
#endif
                    pData += 4;
                    extLen -= 4;//active + direction + time
#if COLOR_RGB_SUPPORT
                    if (move2EnhHueAndSat.enhancedHue || move2EnhHueAndSat.saturation) {
                        sampleLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_AND_SATURATION, &move2EnhHueAndSat);
                    }

                    if (colorLoopSet.action || colorLoopSet.direction || colorLoopSet.time) {
                        sampleLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_COLOR_LOOP_SET, &colorLoopSet);
                    }
#endif
#if COLOR_CCT_SUPPORT
                    zcl_colorCtrlMoveToColorTemperatureCmd_t move2CT = {0};
                    move2CT.optPresent = 0;
                    move2CT.colorTemperature = BUILD_U16(pData[0], pData[1]);
                    move2CT.transitionTime = pScene->transTime * 10 + pScene->transTime100ms;
#endif
                    pData += 2;
                    extLen -= 2;//colorTemperature
#if COLOR_CCT_SUPPORT
                    if (move2CT.colorTemperature) {
                        sampleLight_colorCtrlCb(pAddrInfo, ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE, &move2CT);
                    }
#endif
                }
            }
        }else
#endif
        {
            //do nothing
        }

        pData += extLen;
    }
}

/*********************************************************************
 * @fn      sampleLight_sceneStoreReqHandler
 *
 * @brief   Handler for ZCL scene store command. This function will set scene attribute first.
 *
 * @param   pApsdeInd
 * @param   pScene
 *
 * @return  None
 */
static void sampleLight_sceneStoreReqHandler(zcl_sceneEntry_t *pScene)
{
    u8 extLen = 0;

#ifdef ZCL_ON_OFF
    zcl_onOffAttr_t *pOnOff = zcl_onoffAttrGet();

    pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
    pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_GEN_ON_OFF);
    pScene->extField[extLen++] = sizeof(u8);
    pScene->extField[extLen++] = pOnOff->onOff;
#endif

#ifdef ZCL_LEVEL_CTRL
    zcl_levelAttr_t *pLevel = zcl_levelAttrGet();

    pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_GEN_LEVEL_CONTROL);
    pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_GEN_LEVEL_CONTROL);
    pScene->extField[extLen++] = sizeof(u8);
    pScene->extField[extLen++] = pLevel->curLevel;
#endif

#ifdef ZCL_LIGHT_COLOR_CONTROL
    zcl_lightColorCtrlAttr_t *pColor = zcl_colorAttrGet();

    pScene->extField[extLen++] = LO_UINT16(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL);
    pScene->extField[extLen++] = HI_UINT16(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL);
    pScene->extField[extLen++] = 13;

    if (pColor->colorMode == ZCL_COLOR_MODE_CURRENT_X_Y) {
#if COLOR_RGB_SUPPORT && !defined(COLOR_X_Y_DISABLE)
        //currentX
        pScene->extField[extLen++] = LO_UINT16(pColor->currentX);
        pScene->extField[extLen++] = HI_UINT16(pColor->currentX);
        //currentY
        pScene->extField[extLen++] = LO_UINT16(pColor->currentY);
        pScene->extField[extLen++] = HI_UINT16(pColor->currentY);
#else
        //currentX
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //currentY
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
#endif
        //enhancedCurrentHue
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //currentSaturation
        pScene->extField[extLen++] = 0;
        //colorLoopActive
        pScene->extField[extLen++] = 0;
        //colorLoopDirection
        pScene->extField[extLen++] = 0;
        //colorLoopTime
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //colorTemperatureMireds
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
    } else if (pColor->colorMode == ZCL_COLOR_MODE_CURRENT_HUE_SATURATION) {
        //currentX
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //currentY
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
#if COLOR_RGB_SUPPORT
        //enhancedCurrentHue
        pScene->extField[extLen++] = LO_UINT16(pColor->enhancedCurrentHue);
        pScene->extField[extLen++] = HI_UINT16(pColor->enhancedCurrentHue);
        //currentSaturation
        pScene->extField[extLen++] = pColor->currentSaturation;
        //colorLoopActive
        pScene->extField[extLen++] = pColor->colorLoopActive;
        //colorLoopDirection
        pScene->extField[extLen++] = pColor->colorLoopDirection;
        //colorLoopTime
        pScene->extField[extLen++] = LO_UINT16(pColor->colorLoopTime);
        pScene->extField[extLen++] = HI_UINT16(pColor->colorLoopTime);
#else
        //enhancedCurrentHue
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //currentSaturation
        pScene->extField[extLen++] = 0;
        //colorLoopActive
        pScene->extField[extLen++] = 0;
        //colorLoopDirection
        pScene->extField[extLen++] = 0;
        //colorLoopTime
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
#endif
        //colorTemperatureMireds
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
    } else if(pColor->colorMode == ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS) {
        //currentX
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //currentY
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //enhancedCurrentHue
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
        //currentSaturation
        pScene->extField[extLen++] = 0;
        //colorLoopActive
        pScene->extField[extLen++] = 0;
        //colorLoopDirection
        pScene->extField[extLen++] = 0;
        //colorLoopTime
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
#if COLOR_CCT_SUPPORT
        //colorTemperatureMireds
        pScene->extField[extLen++] = LO_UINT16(pColor->colorTemperatureMireds);
        pScene->extField[extLen++] = HI_UINT16(pColor->colorTemperatureMireds);
#else
        //colorTemperatureMireds
        pScene->extField[extLen++] = 0;
        pScene->extField[extLen++] = 0;
#endif
    }
#endif

    pScene->extFieldLen = extLen;
}

/*********************************************************************
 * @fn      sampleLight_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t sampleLight_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pAddrInfo->dstEp == SAMPLE_LIGHT_ENDPOINT) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR) {
            switch (cmdId) {
            case ZCL_CMD_SCENE_STORE_SCENE:
                sampleLight_sceneStoreReqHandler((zcl_sceneEntry_t *)cmdPayload);
                break;
            case ZCL_CMD_SCENE_RECALL_SCENE:
                sampleLight_sceneRecallReqHandler(pAddrInfo, (zcl_sceneEntry_t *)cmdPayload);
                break;
            default:
                status = ZCL_STA_UNSUP_MANU_CLUSTER_COMMAND;
                break;
            }
        }
    }

    return status;
}

#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
