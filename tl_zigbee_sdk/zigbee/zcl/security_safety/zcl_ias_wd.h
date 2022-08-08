/********************************************************************************************************
 * @file    zcl_ias_wd.h
 *
 * @brief   This is the header file for zcl_ias_wd
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

#ifndef ZCL_IAS_WD_H
#define ZCL_IAS_WD_H



/*********************************************************************
 * CONSTANTS
 */

/* Command ID */
// Client generated command
#define ZCL_CMD_START_WARNING                               0x00
#define ZCL_CMD_SQUAWK                                      0x01


/* Attribute ID */
#define ZCL_ATTRID_WD_MAX_DURATION                          0x0000


/*********************************************************************
 * TYPES
 */

typedef union {
	u8 byte;
	struct{
		u8 warningMode:4;
		u8 strobe:2;
		u8 sirenLevel:2;
	}bits;
} warning_t;

/**
 *  @brief  Structure definition for start warning command
 */
typedef struct {
    u16 warningDuration;
    warning_t warning;
    u8	strobeDutyCycle;
    u8	strobeLevel;
} startWarning_t;

/**
 *  @brief  Structure definition for squawk command
 */
typedef union {
	u8	byte;
	struct{
		u8 squawk:4;
		u8 strobe:1;
		u8 reserved:1;
		u8 squawkLevel:2;
	}bits;
} squawk_t;

#if 0
/* Command Callback Function Type */
typedef void (*zcl_startWarningCb_t)(apsdeDataInd_t *pApsdeInd, startWarning_t *pStartWarning);
typedef void (*zcl_SquawkCb_t)(apsdeDataInd_t *pApsdeInd, squawk_t *pSquawk);

/**
 *  @brief  Structure definition for commands callback functions in ias wd cluster
 */
typedef struct {
	zcl_startWarningCb_t    startWarningCbFunc;
    zcl_SquawkCb_t 			squawkCbFunc;
} zcl_iasWd_AppCallbacks_t;



extern u16 zcl_attr_MaxDuration;

extern const zclAttrInfo_t iasWD_attrTbl[];

extern const u8 zcl_iasWD_attrNum;
#endif


status_t zcl_iasWd_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


status_t zcl_iasWd_startWarning(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, startWarning_t *pStartWarning);
#define zcl_iasWd_startWarningCmd(a,b,c,d)	(zcl_iasWd_startWarning((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_iasWd_squawk(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, squawk_t *pSquawk);
#define zcl_iasWd_squawkCmd(a,b,c,d)	(zcl_iasWd_squawk((a), (b), (c), ZCL_SEQ_NUM, (d)))

#endif	/* ZCL_IAS_WD_H */
