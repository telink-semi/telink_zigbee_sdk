/********************************************************************************************************
 * @file	zcl_level.h
 *
 * @brief	This is the header file for zcl_level
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
#ifndef ZCL_LEVEL_H
#define ZCL_LEVEL_H



/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */
 
/** @addtogroup  ZCL_CLUSTER_LEVEL LEVEL Cluster
 *  @{
 */

/** @addtogroup  ZCL_LEVEL_Constant LEVEL Cluster Constants
 *  @{
 */

/** @addtogroup zcl_level_cmdId LEVEL Command Ids
 * @{
 */
#define ZCL_CMD_LEVEL_MOVE_TO_LEVEL                       0x00
#define ZCL_CMD_LEVEL_MOVE                                0x01
#define ZCL_CMD_LEVEL_STEP                                0x02
#define ZCL_CMD_LEVEL_STOP                                0x03
#define ZCL_CMD_LEVEL_MOVE_TO_LEVEL_WITH_ON_OFF           0x04
#define ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF                    0x05
#define ZCL_CMD_LEVEL_STEP_WITH_ON_OFF                    0x06
#define ZCL_CMD_LEVEL_STOP_WITH_ON_OFF                    0x07
#define ZCL_CMD_LEVEL_MOVE_TO_CLOSEST_FREQUENCY			  0x08
/** @} end of group zcl_level_cmdId */

/** @addtogroup zcl_level_moveSubCmdId LEVEL Control Move (Mode) Sub Command Ids
 * @{
 */
#define LEVEL_MOVE_UP                                     0x00
#define LEVEL_MOVE_DOWN                                   0x01
/** @} end of group zcl_level_moveSubCmdId */

/** @addtogroup zcl_level_stepSubCmdId LEVEL Control Step Sub Command Ids
 * @{
 */
#define LEVEL_STEP_UP                                     0x00
#define LEVEL_STEP_DOWN                                   0x01
/** @} end of group zcl_level_stepSubCmdId */


/** @addtogroup zcl_level_attrId LEVEL Attribute Ids
 * @{
 */
#define ZCL_ATTRID_LEVEL_CURRENT_LEVEL                    0x0000
#define ZCL_ATTRID_LEVEL_REMAINING_TIME                   0x0001
#define ZCL_ATTRID_LEVEL_MIN_LEVEL						  0x0002
#define ZCL_ATTRID_LEVEL_MAX_LEVEL						  0x0003
#define ZCL_ATTRID_LEVEL_CURRENT_FREQUENCY				  0x0004
#define ZCL_ATTRID_LEVEL_MIN_FREQUENCY				  	  0x0005
#define ZCL_ATTRID_LEVEL_MAX_FREQUENCY				  	  0x0006
#define ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME           0x0010
#define ZCL_ATTRID_LEVEL_ON_LEVEL                         0x0011
#define ZCL_ATTRID_LEVEL_ON_TRANSITION_TIME               0x0012
#define ZCL_ATTRID_LEVEL_OFF_TRANSITION_TIME              0x0013
#define ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE                0x0014
#define ZCL_ATTRID_LEVEL_OPTIONS				  	  	  0x000F
#define ZCL_ATTRID_LEVEL_START_UP_CURRENT_LEVEL		  	  0x4000
/** @} end of group zcl_level_attrId */


#define ZCL_LEVEL_ATTR_MIN_LEVEL						  0x01
#define ZCL_LEVEL_ATTR_MAX_LEVEL						  0xFE
#define ZCL_LEVEL_ATTR_ON_LEVEL_NO_EFFECT				  0xFF


#define ZCL_START_UP_CURRENT_LEVEL_TO_MIN				  0x00
#define ZCL_START_UP_CURRENT_LEVEL_TO_PREVIOUS			  0xFF

/** @} end of group ZCL_LEVEL_Constant */


/** @addtogroup  ZCL_LEVEL_Types LEVEL Cluster Types
 *  @{
 */


/**
 *  @brief  Definition format for move to level command
 */
typedef struct {
	u8  level;             //!< new level to move to
	u16 transitionTime;    //!< time to take to move to the new level (in tenth of a second)
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} moveToLvl_t;

/**
 *  @brief  Definition format for move command
 */
typedef struct {
	u8 moveMode;           //!< move mode.
	u8 rate;               //!< rate of movement in steps per second
	u8 optPresent;         //!< options field present
	u8 optionsMask;
	u8 optionsOverride;
} move_t;

/**
 *  @brief  Definition format for step command
 */
typedef struct {
	u8  stepMode;          //!< step mode
	u8  stepSize;          //!< number of levels to step
	u16 transitionTime;    //!< time, in 1/10ths of a second, to take to perform the step
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} step_t;

/**
 *  @brief  Definition format for stop command
 */
typedef struct {
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} stop_t;

/**
 *  @brief  Definition format for level command payload
 */
typedef union{
	moveToLvl_t		moveToLevel;
	move_t			move;
	step_t			step;
	stop_t			stop;
}zcl_level_cmdPayload_t;

#if 0
/** @addtogroup  ZCL_LEVEL_CmdCallback LEVEL Cluster Callbacks
 *  @{
 */

/**
 *  @brief  Definition for move to level command callback
 */
typedef void (*zcl_levelCb_t)(u8 cmdId, zcl_level_cmdPayload_t *cmdPayload);

/**
 *  @brief  App layer should use this structure to register LEVEL clusters callbacks
 */
typedef struct {
	zcl_levelCb_t			levelCbFunc;
} zcl_level_AppCallbacks_t;


/** @} end of group ZCL_LEVEL_CmdCallback */


/** @addtogroup  ZCL_LEVEL_Variables LEVEL Cluster Variables
 *  @{
 */


/**
 *  @brief  Definition format for level control attributes
 */
typedef struct {
#ifdef ZCL_ATTR_REMAINING_TIME_ENABLE
    u16 remainingTime;        //!< the time remaining until the current command is complete
#endif
#ifdef ZCL_ATTR_ONOFF_TRANS_TIME_ENABLE
    u16 onOffTransTime;       //!< the time taken to move to or from the target level
#endif
#ifdef ZCL_ATTR_ON_TRANS_ENABLE
    u16 onTransitionTime;     //!< determines the value that the CurrentLevelattribute is set to
#endif
#ifdef ZCL_ATTR_OFF_TRANS_ENABLE
    u16 offTransitionTime;    //!< determines the value that the CurrentLevelattribute is set to
#endif
#ifdef ZCL_ATTR_CURRENT_LEVEL_ENABLE
    u8 curLevel;              //!< the current level of this device
#endif
#ifdef ZCL_ATTR_ON_LEVEL_ENABLE
    u8 onLevel;               //!< determines the value that the CurrentLevelattribute is set to
#endif
#ifdef ZCL_ATTR_DEFAULT_MOVE_RATE_ENABLE
    u8 defaultMoveRate;
#endif
#ifdef ZCL_ATTR_OPTIONS_ENABLE
    u8 options;
#endif
#ifdef ZCL_ATTR_START_UP_CURRENT_LEVEL_ENABLE
    u8 startUpCurrentLevel;
#endif
} zcl_levelAttr_t;

/** @} end of group ZCL_LEVEL_Types */

/**
 *  @brief  External variable for level Attribute
 */
extern zcl_levelAttr_t zcl_attr_level;
#define zcl_levelAttrGet()		&zcl_attr_level

/**
 *  @brief  External variable for level control Attribute tables
 */
extern const zclAttrInfo_t level_attrTbl[];

extern const u8 zcl_level_attrNum;

/** @} end of group ZCL_LEVEL_Variables */
#endif


/** @addtogroup  ZCL_LEVEL_Fountions LEVEL Cluster APIs
 *  @{
 */

/**
 * @brief       This function register LEVEL cluster server to ZCL. Only after registered,
 *              level control attributes could be used.
 *
 * @param[in]   cb       - pointer to the App layer command callbacks
 *
 * @return      Status
 */
status_t zcl_level_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);


/**
 * @brief       API to send Move To Level command in Level cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cmdId - ZCL_CMD_LEVEL_MOVE_TO_LEVEL / ZCL_CMD_LEVEL_MOVE_TO_LEVEL_WITH_ON_OFF
 * @param[in]   pMove2Level
 *
 * @return      status_t
 */
status_t zcl_level_move2level(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, moveToLvl_t *pMove2Level);
#define zcl_level_move2levelCmd(a,b,c,d)			(zcl_level_move2level((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_MOVE_TO_LEVEL, (d)))
#define zcl_level_move2levelWithOnOffCmd(a,b,c,d)	(zcl_level_move2level((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_MOVE_TO_LEVEL_WITH_ON_OFF, (d)))

/**
 * @brief       API to send Move command in Level cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cmdId - ZCL_CMD_LEVEL_MOVE / ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF
 * @param[in]   pMove
 *
 * @return      status_t
 */
status_t zcl_level_move(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, move_t *pMove);
#define zcl_level_moveCmd(a,b,c,d)				(zcl_level_move((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_MOVE, (d)))
#define zcl_level_moveWithOnOffCmd(a,b,c,d)		(zcl_level_move((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_MOVE_WITH_ON_OFF, (d)))

/**
 * @brief       API to send Step command in LEVEL cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cmdId - ZCL_CMD_LEVEL_STEP/ ZCL_CMD_LEVEL_STEP_WITH_ON_OFF
 * @param[in]   pStep
 *
 * @return      status_t
 */
status_t zcl_level_step(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, step_t *pStep);
#define zcl_level_stepCmd(a,b,c,d)				(zcl_level_step((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_STEP, (d)))
#define zcl_level_stepWithOnOffCmd(a,b,c,d)		(zcl_level_step((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_STEP_WITH_ON_OFF, (d)))

/**
 * @brief       API to send Stop command in Level cluster
 *
 * @param[in]   srcEp - source endpoint
 * @param[in]   pDstEpInfo - destination endpoint information
 * @param[in]   disableDefaultRsp - disable Default Response command
 * @param[in]   seqNo - identification number for the transaction
 * @param[in]   cmdId - ZCL_CMD_LEVEL_STOP/ ZCL_CMD_LEVEL_STOP_WITH_ON_OFF
 * @param[in]   pStop
 *
 * @return      status_t
 */
status_t zcl_level_stop(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, u8 cmdId, stop_t *pStop);
#define zcl_level_stopCmd(a,b,c,d)				(zcl_level_stop((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_STOP, (d)))
#define zcl_level_stopWithOnOffCmd(a,b,c,d)		(zcl_level_stop((a), (b), (c), ZCL_SEQ_NUM, ZCL_CMD_LEVEL_STOP_WITH_ON_OFF, (d)))


/** @} end of group ZCL_LEVEL_Fountions */

/** @} end of group ZCL_CLUSTER_LEVEL */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif	/* ZCL_LEVEL_H */
