/********************************************************************************************************
 * @file     zcl_ias_wd_attr.h
 *
 * @brief	 APIs statement for Warning Device equipment of the IAS system.
 *
 * @author
 * @date     June. 10, 2017
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#pragma once

#include "tl_common.h"
#include "../../zdo/zb_af.h"
#include "../zcl.h"

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


status_t zcl_iasWd_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


status_t zcl_iasWd_startWarning(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, startWarning_t *pStartWarning);
#define zcl_iasWd_startWarningCmd(a,b,c,d)	(zcl_iasWd_startWarning((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_iasWd_squawk(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, squawk_t *pSquawk);
#define zcl_iasWd_squawkCmd(a,b,c,d)	(zcl_iasWd_squawk((a), (b), (c), ZCL_SEQ_NUM, (d)))

