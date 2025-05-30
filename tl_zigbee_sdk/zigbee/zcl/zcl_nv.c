/********************************************************************************************************
 * @file    zcl_nv.c
 *
 * @brief   This is the source file for zcl_nv
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

/**********************************************************************
 * INCLUDES
 */
#include "zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      zcl_reportingTab_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_reportingTab_save(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_REPORT, sizeof(zcl_reportingTab_t), (u8 *)&reportingTab);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_reportingTab_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_reportingTab_restore(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_REPORT, sizeof(zcl_reportingTab_t), (u8 *)&reportingTab);
    for (u8 i = 0; i < reportingTab.reportNum; i++) {
        memset(reportingTab.reportCfgInfo[i].prevData, 0xFF, REPORTABLE_CHANGE_MAX_ANALOG_SIZE);
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}
#endif /* ZCL_REPORT */

/*********************************************************************
 * @fn      zcl_sceneTable_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_sceneTable_save(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_SCENE
#if NV_ENABLE
    st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_SCENE_TABLE, sizeof(zcl_sceneTable_t) * ZCL_SCENE_TABLE_NUM, (u8 *)&g_zcl_sceneTab);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_sceneTable_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_sceneTable_restore(void)
{
    nv_sts_t st = NV_SUCC;

#ifdef ZCL_SCENE
#if NV_ENABLE
    nv_itemLengthCheckAdd(NV_ITEM_ZCL_SCENE_TABLE, ZCL_SCENE_TABLE_NUM * sizeof(zcl_sceneTable_t));
    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_SCENE_TABLE, sizeof(zcl_sceneTable_t), (u8 *)&g_zcl_sceneTab);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

#ifdef ZCL_WWAH
/*********************************************************************
 * @fn      zcl_apsLinkKeyAuthInfo_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_apsLinkKeyAuthInfo_save(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WWAH_APS_AUTH_INFO, sizeof(wwah_apsLinkKeyAuthInfo_t), (u8 *)&g_zcl_apsLinkKeyAuthInfo);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_apsLinkKeyAuthInfo_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_apsLinkKeyAuthInfo_restore(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WWAH_APS_AUTH_INFO, sizeof(wwah_apsLinkKeyAuthInfo_t), (u8 *)&g_zcl_apsLinkKeyAuthInfo);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_apsAcksRequireInfo_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_apsAcksRequireInfo_save(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WWAH_APS_ACKS_INFO, sizeof(wwah_apsAcksRequireInfo_t), (u8 *)&g_zcl_apsAcksRequireInfo);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_apsAcksRequireInfo_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_apsAcksRequireInfo_restore(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WWAH_APS_ACKS_INFO, sizeof(wwah_apsAcksRequireInfo_t), (u8 *)&g_zcl_apsAcksRequireInfo);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_useTCForClusterInfo_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_useTCForClusterInfo_save(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WWAH_USE_TC_CLUSTER_INFO, sizeof(wwah_useTCForClusterInfo_t), (u8 *)&g_zcl_useTCForClusterInfo);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

/*********************************************************************
 * @fn      zcl_useTCForClusterInfo_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_useTCForClusterInfo_restore(void)
{
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE
    st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WWAH_USE_TC_CLUSTER_INFO, sizeof(wwah_useTCForClusterInfo_t), (u8 *)&g_zcl_useTCForClusterInfo);
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

#endif /* ZCL_WWAH */
