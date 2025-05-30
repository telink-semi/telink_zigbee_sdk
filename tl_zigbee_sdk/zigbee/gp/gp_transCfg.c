/********************************************************************************************************
 * @file    gp_transCfg.c
 *
 * @brief   This is the source file for gp_transCfg
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
#include "../zcl/zcl_include.h"
#include "gp.h"
#include "gp_trans.h"

#if (GP_SUPPORT_ENABLE && GP_BASIC_COMBO)
/**********************************************************************
 * GLOBAL VARIABLES
 */
const gp_transCfg_t g_gpTransCfgList[] = {
    /* GPD_DEV_ID_TEMPERATURE_SENSOR */
    //zcl_tunneling
    {
        GPD_DEV_ID_TEMPERATURE_SENSOR,          /* gpdDevId */
        GPDF_CMD_ID_ZCL_TUNNELING,              /* gpdCmdId */
        ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT, /* zbClusterId */
        0xFF,                                   /* zbCmdId */
        0xFE,                                   /* zbCmdPayloadLen */
        {0},                                    /* zbCmdPayload */
    },
    //report
    {
        GPD_DEV_ID_TEMPERATURE_SENSOR,
        GPDF_CMD_ID_ANY_SENSOR_REPORT_COMMAND,
        ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT,
        ZCL_CMD_REPORT,
        0xFE,
        {0},
    },

    /* GPD_DEV_ID_ONOFF_SWITCH */
    //off
    {
        GPD_DEV_ID_ONOFF_SWITCH,                /* gpdDevId */
        GPDF_CMD_ID_ONOFF_OFF,                  /* gpdCmdId */
        ZCL_CLUSTER_GEN_ON_OFF,                 /* zbClusterId */
        ZCL_CMD_ONOFF_OFF,                      /* zbCmdId */
        0,                                      /* zbCmdPayloadLen */
        {0},                                    /* zbCmdPayload */
    },
    //on
    {
        GPD_DEV_ID_ONOFF_SWITCH,
        GPDF_CMD_ID_ONOFF_ON,
        ZCL_CLUSTER_GEN_ON_OFF,
        ZCL_CMD_ONOFF_ON,
        0,
        {0},
    },
    //toggle
    {
        GPD_DEV_ID_ONOFF_SWITCH,
        GPDF_CMD_ID_ONOFF_TOGGLE,
        ZCL_CLUSTER_GEN_ON_OFF,
        ZCL_CMD_ONOFF_TOGGLE,
        0,
        {0},
    },

    /* GPD_DEV_ID_LEVEL_CONTROL_SWITCH */
    //move_up
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,        /* gpdDevId */
        GPDF_CMD_ID_LEVEL_MOVE_UP,              /* gpdCmdId */
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,          /* zbClusterId */
        ZCL_CMD_LEVEL_MOVE,                     /* zbCmdId */
        2,                                      /* zbCmdPayloadLen */
        {LEVEL_MOVE_UP, 0x14},                  /* zbCmdPayload */
    },
    //move_down
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_MOVE_DOWN,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_MOVE,
        2,
        {LEVEL_MOVE_DOWN, 0x14},
    },
    //step_up
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_STEP_UP,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_STEP,
        4,
        {LEVEL_STEP_UP, 0x14, 0xFF, 0XFF},
    },
    //step_down
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_STEP_DOWN,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_STEP,
        4,
        {LEVEL_STEP_DOWN, 0x14, 0xFF, 0XFF},
    },
    //stop
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_STOP,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_STOP,
        0,
        {0},
    },
    //move_up_with_onoff
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_MOVE_UP_WITH_ONOFF,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF,
        2,
        {LEVEL_MOVE_UP, 0x14},
    },
    //move_down_with_onoff
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_MOVE_DOWN_WITH_ONOFF,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF,
        2,
        {LEVEL_MOVE_DOWN, 0x14},
    },
    //step_up_with_onoff
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_STEP_UP_WITH_ONOFF,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_STEP_WITH_ON_OFF,
        4,
        {LEVEL_STEP_UP, 0x14, 0xFF, 0xFF},
    },
    //step_down_with_onoff
    {
        GPD_DEV_ID_LEVEL_CONTROL_SWITCH,
        GPDF_CMD_ID_LEVEL_STEP_DOWN_WITH_ONOFF,
        ZCL_CLUSTER_GEN_LEVEL_CONTROL,
        ZCL_CMD_LEVEL_STEP_WITH_ON_OFF,
        4,
        {LEVEL_STEP_DOWN, 0x14, 0xFF, 0xFF},
    },
};

u8 GP_TRANS_CFG_LIST_NUM = (sizeof(g_gpTransCfgList) / sizeof(g_gpTransCfgList[0]));

#endif	/* GP_SUPPORT_ENABLE */
