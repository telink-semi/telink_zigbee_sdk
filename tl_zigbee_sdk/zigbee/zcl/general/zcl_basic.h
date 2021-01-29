/********************************************************************************************************
 * @file	zcl_basic.h
 *
 * @brief	This is the header file for zcl_basic
 *
 * @author	Zigbee Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#define ZCL_CMD_BASIC_RESET_FAC_DEFAULT                     0x00

/** @} end of group zcl_basic_cmdId */

/** @addtogroup zcl_basic_attrId BASIC Attribute Ids
 * @{
 */
#define ZCL_ATTRID_BASIC_ZCL_VER                          0x0000
#define ZCL_ATTRID_BASIC_APP_VER                          0x0001
#define ZCL_ATTRID_BASIC_STACK_VER                        0x0002
#define ZCL_ATTRID_BASIC_HW_VER                           0x0003
#define ZCL_ATTRID_BASIC_MFR_NAME                         0x0004
#define ZCL_ATTRID_BASIC_MODEL_ID                         0x0005
#define ZCL_ATTRID_BASIC_DATE_CODE                        0x0006
#define ZCL_ATTRID_BASIC_POWER_SOURCE                     0x0007
#define ZCL_ATTRID_BASIC_LOC_DESC                         0x0010
#define ZCL_ATTRID_BASIC_PHY_ENV                          0x0011
#define ZCL_ATTRID_BASIC_DEV_ENABLED                      0x0012
#define ZCL_ATTRID_BASIC_ALARM_MASK                       0x0013
#define ZCL_ATTRID_BASIC_DISABLE_LOCAL_CFG                0x0014
#define ZCL_ATTRID_BASIC_SW_BUILD_ID                      0x4000

/** @} end of group zcl_basic_attrId */

/* Power Source Attribute values */
#define POWER_SOURCE_UNKNOWN                              	0x00
#define POWER_SOURCE_MAINS_1_PHASE                        	0x01
#define POWER_SOURCE_MAINS_3_PHASE                        	0x02
#define POWER_SOURCE_BATTERY                              	0x03
#define POWER_SOURCE_DC                                   	0x04
#define POWER_SOURCE_EMERG_MAINS_CONST_PWR                	0x05
#define POWER_SOURCE_EMERG_MAINS_XFER_SW                  	0x06

/**
 *  @brief Max length of char string
 */
#define ZCL_BASIC_MAX_LENGTH 								24

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
	zcl_rstFacDefaultCb_t    rstFacDefaultCbFunc;
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
 * @param[in]   pAttrTbl - pointer to server side attribute table. It is implented in zcl_basic_attr.c
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
#define zcl_basic_resetFactoryDftCmd(a,b,c)	(zcl_basic_reset((a), (b), (c), ZCL_SEQ_NUM))

/** @} end of group ZCL_BASIC_Fountions */

/** @} end of group ZCL_CLUSTER_BASIC */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif	/* ZCL_BASIC_H */
