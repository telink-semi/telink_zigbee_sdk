/********************************************************************************************************
 * @file     zcl_light_colorCtrl.h
 *
 * @brief	 APIs statement for color control cluster
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


/* Attribute ID */
#define ZCL_ATTRID_CURRENT_HUE                         			0x0000
#define ZCL_ATTRID_CURRENT_SATURATION                  			0x0001
#define ZCL_ATTRID_REMAINING_TIME                      			0x0002
#define ZCL_ATTRID_CURRENT_X                           			0x0003
#define ZCL_ATTRID_CURRENT_Y                           			0x0004
#define ZCL_ATTRID_DRIFT_COMPENSATION                  			0x0005
#define ZCL_ATTRID_COMPENSATION_TEXT                   			0x0006
#define ZCL_ATTRID_COLOR_TEMPERATURE_MIREDS            			0x0007
#define ZCL_ATTRID_COLOR_MODE                          			0x0008
#define ZCL_ATTRID_COLOR_OPTIONS					   			0x000F
#define ZCL_ATTRID_ENHANCED_CURRENT_HUE				   			0x4000
#define ZCL_ATTRID_ENHANCED_COLOR_MODE				   			0x4001
#define ZCL_ATTRID_COLOR_LOOP_ACTIVE				   			0x4002
#define ZCL_ATTRID_COLOR_LOOP_DIRECTION				   			0x4003
#define ZCL_ATTRID_COLOR_LOOP_TIME					   			0x4004
#define ZCL_ATTRID_COLOR_LOOP_START_ENHANCED_HUE	   			0x4005
#define ZCL_ATTRID_COLOR_LOOP_STORED_ENHANCED_HUE	   			0x4006
#define ZCL_ATTRID_COLOR_CAPABILITIES				   			0x400A
#define ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MIN_MIREDS	   			0x400B
#define ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MAX_MIREDS	   			0x400C
#define ZCL_ATTRID_COUPLE_COLOR_TEMP_TO_LEVEL_MIN_MIREDS	    0x400D
#define ZCL_ATTRID_START_UP_COLOR_TEMPERATURE_MIREDS	   		0x4010

#define ZCL_ATTRID_NUMBER_OF_PRIMARIES                 			0x0010
#define ZCL_ATTRID_PRIMARY1_X                          			0x0011
#define ZCL_ATTRID_PRIMARY1_Y                          			0x0012
#define ZCL_ATTRID_PRIMARY1_INTENSITY                  			0x0013
#define ZCL_ATTRID_RESERVED_1                          			0x0014

#define ZCL_ATTRID_PRIMARY2_X                          			0x0015
#define ZCL_ATTRID_PRIMARY2_Y                          			0x0016
#define ZCL_ATTRID_PRIMARY2_INTENSITY                  			0x0017
#define ZCL_ATTRID_RESERVED_2                          			0x0018

#define ZCL_ATTRID_PRIMARY3_X                          			0x0019
#define ZCL_ATTRID_PRIMARY3_Y                          			0x001A
#define ZCL_ATTRID_PRIMARY3_INTENSITY                  			0x001B

#define ZCL_ATTRID_PRIMARY4_X                          			0x0020
#define ZCL_ATTRID_PRIMARY4_Y                          			0x0021
#define ZCL_ATTRID_PRIMARY4_INTENSITY                  			0x0022
#define ZCL_ATTRID_RESERVED_4                          			0x0023

#define ZCL_ATTRID_PRIMARY5_X                          			0x0024
#define ZCL_ATTRID_PRIMARY5_Y                          			0x0025
#define ZCL_ATTRID_PRIMARY5_INTENSITY                  			0x0026
#define ZCL_ATTRID_RESERVED_5                          			0x0027

#define ZCL_ATTRID_PRIMARY6_X                          			0x0028
#define ZCL_ATTRID_PRIMARY6_Y                          			0x0029
#define ZCL_ATTRID_PRIMARY6_INTENSITY                  			0x002A
 
#define ZCL_ATTRID_WHITE_POINT_X                       			0x0030
#define ZCL_ATTRID_WHITE_POINT_Y                       			0x0031
#define ZCL_ATTRID_COLOR_POINT_RX                      			0x0032
#define ZCL_ATTRID_COLOR_POINT_RY                      			0x0033
#define ZCL_ATTRID_COLOR_POINT_RINTENSITY              			0x0034
#define ZCL_ATTRID_RESERVED_7                          			0x0035

#define ZCL_ATTRID_COLOR_POINT_GX                      			0x0036
#define ZCL_ATTRID_COLOR_POINT_GY                      			0x0037
#define ZCL_ATTRID_COLOR_POINT_GINTENSITY              			0x0038
#define ZCL_ATTRID_RESERVED_8                          			0x0039

#define ZCL_ATTRID_COLOR_POINT_BX                      			0x003A
#define ZCL_ATTRID_COLOR_POINT_BY                      			0x003B
#define ZCL_ATTRID_COLOR_POINT_BINTENSITY              			0x003C






#define ZCL_COLOR_ATTR_HUE_MIN									0
#define ZCL_COLOR_ATTR_HUE_MAX									0xFE
#define ZCL_COLOR_ATTR_SATURATION_MIN							0
#define ZCL_COLOR_ATTR_SATURATION_MAX							0xFE
#define ZCL_COLOR_ATTR_ENHANCED_HUE_MIN							0
#define ZCL_COLOR_ATTR_ENHANCED_HUE_MAX							0xFFFF
#define ZCL_COLOR_ATTR_XY_MIN									0
#define ZCL_COLOR_ATTR_XY_MAX									0xFEFF
#define ZCL_COLOR_ATTR_TEMPERATURE_MIRDES_MIN					0
#define ZCL_COLOR_ATTR_TEMPERATURE_MIRDES_MAX					0xFEFF


//Values of the ColorMode Attribute
#define ZCL_COLOR_MODE_CURRENT_HUE_SATURATION					0x00
#define ZCL_COLOR_MODE_CURRENT_X_Y								0x01
#define ZCL_COLOR_MODE_COLOR_TEMPERATURE_MIREDS					0x02
#define ZCL_ENHANCED_COLOR_MODE_CURRENT_HUE_SATURATION			0x03

//Bit Values of the ColorCapabilities Attribute
#define ZCL_COLOR_CAPABILITIES_BIT_HUE_SATURATION               0x0001
#define ZCL_COLOR_CAPABILITIES_BIT_ENHANCED_HUE                 0x0002
#define ZCL_COLOR_CAPABILITIES_BIT_COLOR_LOOP                   0x0004
#define ZCL_COLOR_CAPABILITIES_BIT_X_Y_ATTRIBUTES               0x0008
#define ZCL_COLOR_CAPABILITIES_BIT_COLOR_TEMPERATURE            0x0010

//Values of the StartUpColorTemperatureMireds attribute
#define ZCL_START_UP_COLOR_TEMPERATURE_MIREDS_TO_PREVIOUS	  	0xFFFF


/*********************************************************************
 * ENUMS
 */
// Command IDs for the Color Control Cluster

typedef enum  {
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE                   		= 0x00,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_HUE                      		= 0x01,
    ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_HUE                      		= 0x02,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_SATURATION            		= 0x03,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_SATURATION               		= 0x04,
    ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_SATURATION               		= 0x05,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_HUE_AND_SATURATION    		= 0x06,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR                 		= 0x07,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR                    		= 0x08,
    ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR                    		= 0x09,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE     		= 0x0A,
    ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE		  		= 0x40,
    ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_HUE			  		= 0x41,
    ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_STEP_HUE			  		= 0x42,
    ZCL_CMD_LIGHT_COLOR_CONTROL_ENHANCED_MOVE_TO_HUE_AND_SATURATION	= 0x43,
    ZCL_CMD_LIGHT_COLOR_CONTROL_COLOR_LOOP_SET						= 0x44,
    ZCL_CMD_LIGHT_COLOR_CONTROL_STOP_MOVE_STEP						= 0x47,
    ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_COLOR_TEMPERATURE				= 0x4B,
    ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR_TEMPERATURE				= 0x4C,
} zcl_colorCtrlCmdId_t;


// Values of the Driection Field
typedef enum {
    COLOR_CTRL_DIRECTION_SHORTEST_DISTANCE      = 0x00,                          
    COLOR_CTRL_DIRECTION_LONGEST_DISTANCE       = 0x01,                         
    COLOR_CTRL_DIRECTION_UP                     = 0x02,           
    COLOR_CTRL_DIRECTION_DOWN                   = 0x03,
} zcl_colorCtrlDir_t;


// Values of the Move Mode Field
typedef enum {
    COLOR_CTRL_MOVE_STOP      = 0x00,                          
    COLOR_CTRL_MOVE_UP        = 0x01,                         
    COLOR_CTRL_MOVE_RESERVED  = 0x02,           
    COLOR_CTRL_MOVE_DOWN      = 0x03,
} zcl_colorCtrlMoveMode_t;

// Values of the step Mode Field
typedef enum {
    COLOR_CTRL_STEP_MODE_RESERVED1      = 0x00,                          
    COLOR_CTRL_STEP_MODE_UP             = 0x01,                         
    COLOR_CTRL_STEP_MODE_RESERVED2      = 0x02,           
    COLOR_CTRL_STEP_MODE_DOWN           = 0x03,
} zcl_colorCtrlStepMode_t;

// Update Flags of the color loop set command
typedef union {
	u8	updateFlags;
	struct {
		u8	action:1;
		u8	direction:1;
		u8	time:1;
		u8	startHue:1;
		u8	reserved:4;
	} bits;
} zcl_colorLoopSetUpdateFlags_t;

// Values of the color loop set action Field
typedef enum {
    COLOR_LOOP_SET_DEACTION			    						= 0x00,
    COLOR_LOOP_SET_ACTION_FROM_COLOR_LOOP_START_ENHANCED_HUE   	= 0x01,
    COLOR_LOOP_SET_ACTION_FROM_ENHANCED_CURRENT_HUE      		= 0x02,
} zcl_colorLoopSetAction_t;

// Values of the color loop set direction Field
typedef enum {
    COLOR_LOOP_SET_DECREMENT	= 0x00,
    COLOR_LOOP_SET_INCREMENT   	= 0x01,
} zcl_colorLoopSetDir_t;

/*********************************************************************
 * TYPES
 */
typedef struct {
    u8  hue;
    zcl_colorCtrlDir_t direction;
    u16 transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveToHueCmd_t;

typedef struct {
    zcl_colorCtrlMoveMode_t moveMode;
    u8  rate;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveHueCmd_t;

typedef struct {
    zcl_colorCtrlStepMode_t stepMode;
    u8  stepSize;
    u8  transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlStepHueCmd_t;

typedef struct {
    u8  saturation;
    u16 transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveToSaturationCmd_t;

typedef struct {
    zcl_colorCtrlMoveMode_t moveMode;
    u8  rate;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveSaturationCmd_t;

typedef struct {
    zcl_colorCtrlStepMode_t stepMode;
    u8  stepSize;
    u8  transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlStepSaturationCmd_t;

typedef struct {
    u8 hue;
    u8 saturation;
    u16 transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveToHueAndSaturationCmd_t;

typedef struct {
    u16 colorX;
    u16 colorY;
    u16 transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveToColorCmd_t;

typedef struct {
    s16 rateX;
    s16 rateY;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveColorCmd_t;

typedef struct {
    s16 stepX;
    s16 stepY;
    u16 transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlStepColorCmd_t;

typedef struct {
    u16 colorTemperature;
    u16 transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveToColorTemperatureCmd_t;

typedef struct {
    u16 enhancedHue;
    u16 transitionTime;
    zcl_colorCtrlDir_t	direction;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlEnhancedMoveToHueCmd_t;

typedef struct {
    zcl_colorCtrlMoveMode_t	moveMode;
    u16	rate;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlEnhancedMoveHueCmd_t;

typedef struct {
    zcl_colorCtrlStepMode_t stepMode;
    u16 stepSize;
    u16 transitionTime;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlEnhancedStepHueCmd_t;

typedef struct {
    u16 enhancedHue;
    u16 transitionTime;
    u8  saturation;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlEnhancedMoveToHueAndSaturationCmd_t;

typedef struct {
    u16 time;
    u16 startHue;
    zcl_colorLoopSetUpdateFlags_t	updateFlags;
    zcl_colorLoopSetAction_t		action;
    zcl_colorLoopSetDir_t			direction;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlColorLoopSetCmd_t;

typedef struct {
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlStopMoveStepCmd_t;

typedef struct {
	u16	rate;
	u16	colorTempMinMireds;
	u16	colorTempMaxMireds;
	zcl_colorCtrlMoveMode_t moveMode;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlMoveColorTemperatureCmd_t;

typedef struct {
	u16	stepSize;
	u16	transitionTime;
	u16	colorTempMinMireds;
	u16	colorTempMaxMireds;
	zcl_colorCtrlStepMode_t stepMode;
	u8  optPresent;        //!< options field present
	u8	optionsMask;
	u8	optionsOverride;
} zcl_colorCtrlStepColorTemperatureCmd_t;

typedef union{
	zcl_colorCtrlMoveToHueCmd_t 						moveToHue;
	zcl_colorCtrlMoveHueCmd_t   						moveHue;
	zcl_colorCtrlStepHueCmd_t   						stepHue;
	zcl_colorCtrlMoveToSaturationCmd_t  				moveToSaturation;
	zcl_colorCtrlMoveSaturationCmd_t					moveSaturation;
	zcl_colorCtrlStepSaturationCmd_t    				stepSaturation;
	zcl_colorCtrlMoveToHueAndSaturationCmd_t    		moveToHueAndSaturation;
	zcl_colorCtrlMoveToColorCmd_t						moveToColor;
	zcl_colorCtrlMoveColorCmd_t							moveColor;
	zcl_colorCtrlStepColorCmd_t							stepColor;
	zcl_colorCtrlMoveToColorTemperatureCmd_t			moveToColorTemperature;
	zcl_colorCtrlEnhancedMoveToHueCmd_t					enhancedMoveToHue;
	zcl_colorCtrlEnhancedMoveHueCmd_t					enhancedMoveHue;
	zcl_colorCtrlEnhancedStepHueCmd_t					enhancedStepHue;
	zcl_colorCtrlEnhancedMoveToHueAndSaturationCmd_t	enhancedMoveToHueAndSaturation;
	zcl_colorCtrlColorLoopSetCmd_t						colorLoopSet;
	zcl_colorCtrlStopMoveStepCmd_t						stopMoveStep;
	zcl_colorCtrlMoveColorTemperatureCmd_t				moveColorTemperature;
	zcl_colorCtrlStepColorTemperatureCmd_t				stepColorTemperature;
}zcl_colorCtrl_cmdPayload_t;


/*********************************************************************
 * Public Functions
 */

#if 0
typedef void (*zcl_lightColorCtrlCb_t)(u8 cmdId, zcl_colorCtrl_cmdPayload_t *cmdPayload);

typedef struct {
	zcl_lightColorCtrlCb_t                  lightColorCtrlCbFunc;
} zcl_lightColorCtrl_AppCallbacks_t;



typedef struct {
#ifdef ZCL_ATTRID_CURRENT_HUE_ENABLE
    u8  zcl_attr_currentHue;
#endif
#ifdef ZCL_ATTRID_CURRENT_SATURATION_ENABLE
    u8  zcl_attr_currentSaturation;
#endif
#ifdef ZCL_ATTRID_COLOR_MODE_ENABLE
    u8  zcl_attr_colorMode;
#endif
#ifdef ZCL_ATTRID_COLOR_OPTIONS_ENABLE
    u8	zcl_attr_options;
#endif
#ifdef ZCL_ATTRID_REMAINING_TIME_ENABLE
    u16 zcl_attr_remainingTime;
#endif
#ifdef ZCL_ATTRID_CURRENT_X_ENABLE
    u16 zcl_attr_currentX;
#endif
#ifdef ZCL_ATTRID_CURRENT_Y_ENABLE
    u16 zcl_attr_currentY;
#endif
#ifdef ZCL_ATTRID_DRIFT_COMPENSATION_ENABLE
    u8  zcl_attr_driftComensation;
#endif
#ifdef ZCL_ATTRID_COMPENSATION_TEXT_ENABLE
    u8* zcl_attr_compensationText;
#endif
#ifdef ZCL_ATTRID_COLOR_TEMPERATURE_MIREDS_ENABLE
    u16 zcl_attr_colorTemperatureMireds;
#endif
#ifdef ZCL_ATTRID_ENHANCED_CURRENT_HUE_ENABLE
    u16 zcl_attr_enhancedCurrentHue;
#endif
#ifdef ZCL_ATTRID_ENHANCED_COLOR_MODE_ENABLE
    u8	zcl_attr_enhancedColorMode;
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_ACTIVE_ENABLE
    u8	zcl_attr_colorLoopActive;
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_DIRECTION_ENABLE
    u8	zcl_attr_colorLoopDirection;
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_TIME_ENABLE
    u16	zcl_attr_colorLoopTime;
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_START_ENHANCED_HUE_ENABLE
    u16	zcl_attr_colorLoopStartEnhancedHue;
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_STORED_ENHANCED_HUE_ENABLE
    u16	zcl_attr_colorLoopStoredEnhancedHue;
#endif
#ifdef ZCL_ATTRID_COLOR_CAPABILITIES_ENABLE
    u16	zcl_attr_colorCapabilities;
#endif
#ifdef ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MIN_MIREDS_ENABLE
    u16	zcl_attr_colorTempPhysicalMinMireds;
#endif
#ifdef ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MAX_MIREDS_ENABLE
    u16	zcl_attr_colorTempPhysicalMaxMireds;
#endif
#ifdef ZCL_ATTRID_COUPLE_COLOR_TEMP_TO_LEVEL_MIN_MIREDS_ENABLE
    u16	zcl_attr_coupleColorTempToLevelMinMireds;
#endif
#ifdef ZCL_ATTRID_START_UP_COLOR_TEMPERATURE_MIREDS_ENABLE
    u16	zcl_attr_startUpColorTemperatureMireds;
#endif

#ifdef ZCL_ATTRID_NUMBER_OF_PRIMARIES_ENABLE
    u8  zcl_attr_numOfprimaries;
#endif
#ifdef ZCL_ATTRID_PRIMARY1_X_ENABLE
    u16 zcl_attr_primary1X;
#endif
#ifdef ZCL_ATTRID_PRIMARY1_Y_ENABLE
    u16 zcl_attr_primary1Y;
#endif
#ifdef ZCL_ATTRID_PRIMARY1_INTENSITY_ENABLE
    u8  zcl_attr_primary1Intensity;
#endif
#ifdef ZCL_ATTRID_PRIMARY2_X_ENABLE
    u16 zcl_attr_primary2X;
#endif
#ifdef ZCL_ATTRID_PRIMARY2_Y_ENABLE
    u16 zcl_attr_primary2Y;
#endif
#ifdef ZCL_ATTRID_PRIMARY2_INTENSITY_ENABLE
    u8  zcl_attr_primary2Intensity;
#endif
#ifdef ZCL_ATTRID_PRIMARY3_X_ENABLE
    u16 zcl_attr_primary3X;
#endif
#ifdef ZCL_ATTRID_PRIMARY3_Y_ENABLE
    u16 zcl_attr_primary3Y;
#endif
#ifdef ZCL_ATTRID_PRIMARY3_INTENSITY_ENABLE
    u8  zcl_attr_primary3Intensity;
#endif
#ifdef ZCL_ATTRID_PRIMARY4_X_ENABLE
    u16 zcl_attr_primary4X;
#endif
#ifdef ZCL_ATTRID_PRIMARY4_Y_ENABLE
    u16 zcl_attr_primary4Y;
#endif
#ifdef ZCL_ATTRID_PRIMARY4_INTENSITY_ENABLE
    u8  zcl_attr_primary4Intensity;
#endif
#ifdef ZCL_ATTRID_PRIMARY5_X_ENABLE
    u16 zcl_attr_primary5X;
#endif
#ifdef ZCL_ATTRID_PRIMARY5_Y_ENABLE
    u16 zcl_attr_primary5Y;
#endif
#ifdef ZCL_ATTRID_PRIMARY5_INTENSITY_ENABLE
    u8  zcl_attr_primary5Intensity;
#endif
#ifdef ZCL_ATTRID_PRIMARY6_X_ENABLE
    u16 zcl_attr_primary6X;
#endif
#ifdef ZCL_ATTRID_PRIMARY6_Y_ENABLE
    u16 zcl_attr_primary6Y;
#endif
#ifdef ZCL_ATTRID_PRIMARY6_INTENSITY_ENABLE
    u8  zcl_attr_primary6Intensity;
#endif

#ifdef ZCL_ATTRID_WHITE_POINT_X_ENABLE
    u16 zcl_attr_whitePointX;
#endif
#ifdef ZCL_ATTRID_WHITE_POINT_Y_ENABLE
    u16 zcl_attr_whitePointY;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_RX_ENABLE
    u16 zcl_attr_colorPointRX;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_RY_ENABLE
    u16 zcl_attr_colorPointRY;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_RINTENSITY_ENABLE
    u8 zcl_attr_colorPointRIntensity;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_GX_ENABLE
    u16 zcl_attr_colorPointGX;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_GY_ENABLE
    u16 zcl_attr_colorPointGY;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_GINTENSITY_ENABLE
    u8 zcl_attr_colorPointGIntensity;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_BX_ENABLE
    u16 zcl_attr_colorPointBX;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_BY_ENABLE
    u16 zcl_attr_colorPointBY;
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_BINTENSITY_ENABLE
    u8 zcl_attr_colorPointBIntensity;
#endif
} zcl_lightColorCtrlAttr_t;

// variable 
extern zcl_lightColorCtrlAttr_t zcl_attr_lightColorCtrl;
extern const zclAttrInfo_t lightColorCtrl_attrTbl[];
extern const u8 zcl_lightColorCtrl_attrNum;

#define zcl_colorAttrGet()		&zcl_attr_lightColorCtrl

#endif

status_t zcl_lightColorCtrl_register(u8 endpoint, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_lightColorCtrl_move2hue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToHueCmd_t *pMove2Hue);
#define zcl_lightColorCtrl_move2hueCmd(a,b,c,d)		(zcl_lightColorCtrl_move2hue((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_moveHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveHueCmd_t *pMoveHue);
#define zcl_lightColorCtrl_moveHueCmd(a,b,c,d)		(zcl_lightColorCtrl_moveHue((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_stepHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepHueCmd_t *pStepHue);
#define zcl_lightColorCtrl_stepHueCmd(a,b,c,d)		(zcl_lightColorCtrl_stepHue((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_move2saturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToSaturationCmd_t *pMove2Sat);
#define zcl_lightColorCtrl_move2saturationCmd(a,b,c,d)		(zcl_lightColorCtrl_move2saturation((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_moveSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveSaturationCmd_t *pMoveSat);
#define zcl_lightColorCtrl_moveSaturationCmd(a,b,c,d)		(zcl_lightColorCtrl_moveSaturation((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_stepSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepSaturationCmd_t *pStepSat);
#define zcl_lightColorCtrl_stepSaturationCmd(a,b,c,d)		(zcl_lightColorCtrl_stepSaturation((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_move2hueAndSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToHueAndSaturationCmd_t *pMove2HueAndSat);
#define zcl_lightColorCtrl_move2hueAndSaturationCmd(a,b,c,d)		(zcl_lightColorCtrl_move2hueAndSaturation((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_move2color(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToColorCmd_t *pMove2Color);
#define zcl_lightColorCtrl_move2colorCmd(a,b,c,d)		(zcl_lightColorCtrl_move2color((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_moveColor(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveColorCmd_t *pMoveColor);
#define zcl_lightColorCtrl_moveColorCmd(a,b,c,d)		(zcl_lightColorCtrl_moveColor((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_stepColor(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepColorCmd_t *pStepColor);
#define zcl_lightColorCtrl_stepColorCmd(a,b,c,d)		(zcl_lightColorCtrl_stepColor((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_move2colorTemperature(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveToColorTemperatureCmd_t *pMove2ColorTemp);
#define zcl_lightColorCtrl_move2colorTemperatureCmd(a,b,c,d)		(zcl_lightColorCtrl_move2colorTemperature((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_enhancedMove2Hue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedMoveToHueCmd_t *pEnhancedMove2Hue);
#define zcl_lightColorCtrl_enhancedMove2hueCmd(a,b,c,d)		(zcl_lightColorCtrl_enhancedMove2hue((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_enhancedMoveHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedMoveHueCmd_t *pEnhancedMoveHue);
#define zcl_lightColorCtrl_enhancedMoveHueCmd(a,b,c,d)		(zcl_lightColorCtrl_enhancedMoveHue((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_enhancedStepHue(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedStepHueCmd_t *pEnhancedStepHue);
#define zcl_lightColorCtrl_enhancedStepHueCmd(a,b,c,d)		(zcl_lightColorCtrl_enhancedStepHue((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_enhancedMove2HueAndSaturation(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlEnhancedMoveToHueAndSaturationCmd_t *pEnhancedMove2HueAndSat);
#define zcl_lightColorCtrl_enhancedMove2HueAndSaturationCmd(a,b,c,d)	(zcl_lightColorCtrl_enhancedMove2HueAndSaturation((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_colorLoopSet(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlColorLoopSetCmd_t *pColorLoopSet);
#define zcl_lightColorCtrl_colorLoopSetCmd(a,b,c,d)		(zcl_lightColorCtrl_colorLoopSet((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_stopMoveStep(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStopMoveStepCmd_t *pStopMoveStep);
#define zcl_lightColorCtrl_stopMoveStepCmd(a,b,c,d)		(zcl_lightColorCtrl_stopMoveStep((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_moveColorTemperature(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlMoveColorTemperatureCmd_t *pMoveColorTemp);
#define zcl_lightColorCtrl_moveColorTemperatureCmd(a,b,c,d)		(zcl_lightColorCtrl_moveColorTemperature((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_lightColorCtrl_stepColorTemperature(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, zcl_colorCtrlStepColorTemperatureCmd_t *pStepColorTemp);
#define zcl_lightColorCtrl_stepColorTemperatureCmd(a,b,c,d)		(zcl_lightColorCtrl_stepColorTemperature((a), (b), (c), ZCL_SEQ_NUM, (d)))



