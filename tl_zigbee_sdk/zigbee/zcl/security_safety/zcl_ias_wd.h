/********************************************************************************************************
 * @file	zcl_ias_wd.h
 *
 * @brief	This is the header file for zcl_ias_wd
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


status_t zcl_iasWd_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


status_t zcl_iasWd_startWarning(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, startWarning_t *pStartWarning);
#define zcl_iasWd_startWarningCmd(a,b,c,d)	(zcl_iasWd_startWarning((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_iasWd_squawk(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, squawk_t *pSquawk);
#define zcl_iasWd_squawkCmd(a,b,c,d)	(zcl_iasWd_squawk((a), (b), (c), ZCL_SEQ_NUM, (d)))

#endif	/* ZCL_IAS_WD_H */
