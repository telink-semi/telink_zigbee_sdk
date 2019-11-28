/********************************************************************************************************
 * @file     zcl_window_covering.h
 *
 * @brief	 statement of APIs for window covering cluster
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


status_t zcl_windowCovering_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_windowCovering_upOpen(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_windowCovering_upOpenCmdSend(a,b,c)	(zcl_windowCovering_upOpen((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_windowCovering_downClose(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_windowCovering_downCloseCmdSend(a,b,c)	(zcl_windowCovering_downClose((a), (b), (c), ZCL_SEQ_NUM))

status_t zcl_windowCovering_stop(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo);
#define zcl_windowCovering_stopCmdSend(a,b,c)	(zcl_windowCovering_stop((a), (b), (c), ZCL_SEQ_NUM))
