/********************************************************************************************************
 * @file	zcl_window_covering.h
 *
 * @brief	This is the header file for zcl_window_covering
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
#ifndef ZCL_WINDOW_COVERING_H
#define ZCL_WINDOW_COVERING_H



/*********************************************************************
 * CONSTANTS
 */
/**
 *  @brief	Window covering Attribute IDs
 */
#define ZCL_ATTRID_WINDOW_COVERING_TYPE                 0x0000
#define ZCL_ATTRID_PHYSICAL_CLOSED_LIMIT_LIFT			0x0001
#define ZCL_ATTRID_PHYSICAL_CLOSED_LIMIT_TILT			0x0002
#define ZCL_ATTRID_CURRENT_POSITION_LIFT				0x0003
#define ZCL_ATTRID_CURRENT_POSITION_TILT				0x0004
#define ZCL_ATTRID_NUM_OF_ACTUATIONS_LIFT				0x0005
#define ZCL_ATTRID_NUM_OF_ACTUATIONS_TILT				0x0006
#define ZCL_ATTRID_CONFIG_STATUS                        0x0007
#define ZCL_ATTRID_CURRENT_POSITION_LIFT_PERCENTAGE		0x0008
#define ZCL_ATTRID_CURRENT_POSITION_TILT_PERCENTAGE		0x0009
#define ZCL_ATTRID_INSTALLED_OPEN_LIMIT_LIFT			0x0010
#define ZCL_ATTRID_INSTALLED_CLOSED_LIMIT_LIFT			0x0011
#define ZCL_ATTRID_INSTALLED_OPEN_LIMIT_TILT			0x0012
#define ZCL_ATTRID_INSTALLED_CLOSED_LIMIT_TILT			0x0013
#define ZCL_ATTRID_VELOCITY_LIFT						0x0014
#define ZCL_ATTRID_ACCELERATION_TIME_LIFT				0x0015
#define ZCL_ATTRID_DECELERATION_TIME_LIFT				0x0016
#define ZCL_ATTRID_MODE                                 0x0017
#define ZCL_ATTRID_INTERMEDIATE_SETPOINTS_LIFT			0x0018
#define ZCL_ATTRID_INTERMEDIATE_SETPOINTS_TILT			0x0019


/**
 *  @brief	Window covering cluster Command IDs
 */
#define ZCL_CMD_UP_OPEN                                 0x00
#define ZCL_CMD_DOWN_CLOSE                              0x01
#define ZCL_CMD_STOP                                    0x02
#define ZCL_CMD_GO_TO_LIFT_VALUE						0x04
#define ZCL_CMD_GO_TO_LIFT_PERCENTAGE					0x05
#define ZCL_CMD_GO_TO_TILT_VALUE						0x07
#define ZCL_CMD_GO_TO_TILT_PERCENTAGE					0x08



/**
 *  @brief  Defined for Go To Lift Value command
 */
typedef struct{
	u16 liftValue;
}zcl_goToLiftValue_t;

/**
 *  @brief  Defined for Go To Lift Percentage command
 */
typedef struct{
	u8 perLiftValue;
}zcl_goToLiftPer_t;

/**
 *  @brief  Defined for Go To Tilt Value command
 */
typedef struct{
	u16 tiltValue;
}zcl_goToTilttValue_t;

/**
 *  @brief  Defined for Go To Tilt Percentage command
 */
typedef struct{
	u8 perTiltValue;
}zcl_goToTiltPer_t;



/**
 *  @brief  Definition format for window covering command payload
 */
typedef union{
	zcl_goToLiftValue_t	goToLiftValue;
	zcl_goToLiftPer_t	goToLiftPer;
	zcl_goToTilttValue_t goToTiltValue;
	zcl_goToTiltPer_t	goToTiltPer;
}zcl_winCovering_cmdPayload_t;


#if 0
/* Command callBack Function Type */
typedef status_t (*zcl_windowCovering_genCmdCb_t)(apsdeDataInd_t *pInd, u8 cmdId, zcl_winCovering_cmdPayload_t *cmdPayload);


/**
 *  @brief  Structure definition for commands callBack functions in door lock cluster
 */
typedef struct {
	zcl_windowCovering_genCmdCb_t    genCmdCbFunc;
}zcl_WindowCovering_AppCallbacks_t;


extern u8 zcl_attr_WindowCoveringType;
extern u8 zcl_attr_ConfigStatus;
extern u8 zcl_attr_Mode;

extern const zclAttrInfo_t windowCovering_attrTbl[];
extern const u8 zcl_windowCovering_attrNum;
#endif


status_t zcl_windowCovering_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_windowCovering_upOpen(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_windowCovering_upOpenCmdSend(a,b,c)	(zcl_windowCovering_upOpen((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_windowCovering_downClose(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_windowCovering_downCloseCmdSend(a,b,c)	(zcl_windowCovering_downClose((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_windowCovering_stop(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_windowCovering_stopCmdSend(a,b,c)	(zcl_windowCovering_stop((a), (b), (c), ZCL_SEQ_NUM))

#endif	/* ZCL_WINDOW_COVERING_H */
