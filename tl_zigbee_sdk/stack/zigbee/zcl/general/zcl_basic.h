/********************************************************************************************************
 * @file    zcl_basic.h
 *
 * @brief   This is the header file for zcl_basic
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
#ifndef ZCL_BASIC_H
#define ZCL_BASIC_H



/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */
 
/** @addtogroup  ZCL_CLUSTER_BASIC BASIC Cluster
 *  @{
 */

/** @addtogroup  ZCL_BASIC_Constant BASIC Cluster Constants
 *  @{
 */

/** @addtogroup zcl_basic_cmdId BASIC Command Ids
 * @{
 */
#define ZCL_CMD_BASIC_RESET_FAC_DEFAULT                 0x00

/** @} end of group zcl_basic_cmdId */

/** @addtogroup zcl_basic_attrId BASIC Attribute Ids
 * @{
 */
#define ZCL_ATTRID_BASIC_ZCL_VER                        0x0000
#define ZCL_ATTRID_BASIC_APP_VER                        0x0001
#define ZCL_ATTRID_BASIC_STACK_VER                      0x0002
#define ZCL_ATTRID_BASIC_HW_VER                         0x0003
#define ZCL_ATTRID_BASIC_MFR_NAME                       0x0004
#define ZCL_ATTRID_BASIC_MODEL_ID                       0x0005
#define ZCL_ATTRID_BASIC_DATE_CODE                      0x0006
#define ZCL_ATTRID_BASIC_POWER_SOURCE                   0x0007
#define ZCL_ATTRID_BASIC_LOC_DESC                       0x0010
#define ZCL_ATTRID_BASIC_PHY_ENV                        0x0011
#define ZCL_ATTRID_BASIC_DEV_ENABLED                    0x0012
#define ZCL_ATTRID_BASIC_ALARM_MASK                     0x0013
#define ZCL_ATTRID_BASIC_DISABLE_LOCAL_CFG              0x0014
#define ZCL_ATTRID_BASIC_SW_BUILD_ID                    0x4000

/** @} end of group zcl_basic_attrId */

/* Power Source Attribute values */
#define POWER_SOURCE_UNKNOWN                            0x00
#define POWER_SOURCE_MAINS_1_PHASE                      0x01
#define POWER_SOURCE_MAINS_3_PHASE                      0x02
#define POWER_SOURCE_BATTERY                            0x03
#define POWER_SOURCE_DC                                 0x04
#define POWER_SOURCE_EMERG_MAINS_CONST_PWR              0x05
#define POWER_SOURCE_EMERG_MAINS_XFER_SW                0x06

/**
 *  @brief Max length of char string
 */
#define ZCL_BASIC_MAX_LENGTH                            24

/** @} end of group ZCL_BASIC_Constant */

#if 0
/** @addtogroup  ZCL_BASIC_Callbacks BASIC Cluster Callbacks
 *  @{
 */

/**
 *  @brief  Definition for Basic RESET command callback format
 */
typedef void (*zcl_rstFacDefaultCb_t)(apsdeDataInd_t *pApsdeInd);

/**
 *  @brief  Structure definition for commands callback functions in BASIC cluster
 */
typedef struct {
    zcl_rstFacDefaultCb_t rstFacDefaultCbFunc;
} zcl_basic_AppCallbacks_t;

/** @} end of group ZCL_BASIC_Callbacks */


/** @addtogroup  ZCL_BASIC_Variables BASIC Cluster Variables
 *  @{
 */

/**
 *  @brief  External variable for basic Attribute
 */
extern const u8 zcl_attr_zclVer;
extern const u8 zcl_attr_appVer;
extern const u8 zcl_attr_stackVer;
extern const u8 zcl_attr_hwVer;
extern const u8 zcl_attr_mfName[];
extern const u8 zcl_attr_dateCode[];
extern const u8 zcl_attr_powerSrc;
extern const u8 zcl_attr_swBuildId[];
extern u8 zcl_attr_modelID[];
extern u8 zcl_attr_locDesc[];
extern u8 zcl_attr_phyEnv;
extern u8 zcl_attr_devEnabled;
extern u8 zcl_attr_alarmMask;
extern u8 zcl_attr_localCfg;

/**
 *  @brief  External variable for basic Attribute table
 */
extern const zclAttrInfo_t basic_attrTbl[];

extern const u8 zcl_basic_attrNum;

/** @} end of group ZCL_BASIC_Variables */
#endif

/** @addtogroup  ZCL_BASIC_Fountions BASIC Cluster APIs
 *  @{
 */

/**
 * @brief       This function register BASIC cluster server to ZCL. Only after registered,
 *              basic attributes could be used.
 *
 * @param[in]   attrNum  - used attribute number
 * @param[in]   pAttrTbl - pointer to server side attribute table. It is implemented in zcl_basic_attr.c
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_basic_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

/**
 * @brief       API to send Basic Reset to factory default command 
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 *
 * @return      None
 */
status_t zcl_basic_reset(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_basic_resetFactoryDftCmd(a,b,c)     (zcl_basic_reset((a), (b), (c), ZCL_SEQ_NUM))

/** @} end of group ZCL_BASIC_Fountions */

/** @} end of group ZCL_CLUSTER_BASIC */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif	/* ZCL_BASIC_H */
