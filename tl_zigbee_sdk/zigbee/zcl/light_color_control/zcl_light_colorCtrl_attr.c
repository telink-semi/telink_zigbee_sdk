/********************************************************************************************************
 * @file     zcl_light_colorCtrl_attr.c
 *
 * @brief	 attribute configuration for color control cluster
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
#if 0
#include "tl_common.h"

/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"



/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_LIGHT_COLOR_CONTROL

zcl_lightColorCtrlAttr_t zcl_attr_lightColorCtrl = {
#ifdef ZCL_ATTRID_CURRENT_HUE_ENABLE
		.zcl_attr_currentHue						=	0,
#endif
#ifdef ZCL_ATTRID_CURRENT_SATURATION_ENABLE
		.zcl_attr_currentSaturation					=	0,
#endif
#ifdef ZCL_ATTRID_REMAINING_TIME_ENABLE
		.zcl_attr_remainingTime						=	0,
#endif
#ifdef ZCL_ATTRID_CURRENT_X_ENABLE
		.zcl_attr_currentX							=	0x616B,//0.381
#endif
#ifdef ZCL_ATTRID_CURRENT_Y_ENABLE
		.zcl_attr_currentY							=	0x607D,//0.377
#endif
#ifdef ZCL_ATTRID_DRIFT_COMPENSATION_ENABLE
		.zcl_attr_driftComensation					=	0,
#endif
#ifdef ZCL_ATTRID_COMPENSATION_TEXT_ENABLE
		.zcl_attr_compensationText					=	NULL,
#endif
#ifdef ZCL_ATTRID_COLOR_TEMPERATURE_MIREDS_ENABLE
		.zcl_attr_colorTemperatureMireds			=	0x00FA,//4000K
#endif
#ifdef ZCL_ATTRID_COLOR_MODE_ENABLE
		.zcl_attr_colorMode							=	ZCL_COLOR_MODE_CURRENT_X_Y,
#endif
#ifdef ZCL_ATTRID_COLOR_OPTIONS_ENABLE
		.zcl_attr_options							=	0,
#endif
#ifdef ZCL_ATTRID_ENHANCED_CURRENT_HUE_ENABLE
		.zcl_attr_enhancedCurrentHue				=	0,
#endif
#ifdef ZCL_ATTRID_ENHANCED_COLOR_MODE_ENABLE
		.zcl_attr_enhancedColorMode					=	ZCL_COLOR_MODE_CURRENT_X_Y,
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_ACTIVE_ENABLE
		.zcl_attr_colorLoopActive					=	0,
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_DIRECTION_ENABLE
		.zcl_attr_colorLoopDirection				=	0,
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_TIME_ENABLE
		.zcl_attr_colorLoopTime						=	0x0019,
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_START_ENHANCED_HUE_ENABLE
		.zcl_attr_colorLoopStartEnhancedHue			=	0x2300,
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_STORED_ENHANCED_HUE_ENABLE
		.zcl_attr_colorLoopStoredEnhancedHue		=	0,
#endif
#ifdef ZCL_ATTRID_COLOR_CAPABILITIES_ENABLE
		.zcl_attr_colorCapabilities					=	0,
#endif
#ifdef ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MIN_MIREDS_ENABLE
		.zcl_attr_colorTempPhysicalMinMireds		=	0,
#endif
#ifdef ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MAX_MIREDS_ENABLE
		.zcl_attr_colorTempPhysicalMaxMireds		=	0xFEFF,
#endif
#ifdef ZCL_ATTRID_COUPLE_COLOR_TEMP_TO_LEVEL_MIN_MIREDS_ENABLE
		.zcl_attr_coupleColorTempToLevelMinMireds	=	0x0001,
#endif
#ifdef ZCL_ATTRID_START_UP_COLOR_TEMPERATURE_MIREDS_ENABLE
		.zcl_attr_startUpColorTemperatureMireds		=	ZCL_START_UP_COLOR_TEMPERATURE_MIREDS_TO_PREVIOUS,
#endif
};

/* Attribute record list */
const zclAttrInfo_t lightColorCtrl_attrTbl[] =
{
/* Attributes of the Color Information Attribute Set */
#ifdef ZCL_ATTRID_CURRENT_HUE_ENABLE
    { ZCL_ATTRID_CURRENT_HUE,             ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_currentHue },
#endif
#ifdef ZCL_ATTRID_CURRENT_SATURATION_ENABLE
    { ZCL_ATTRID_CURRENT_SATURATION,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_currentSaturation },
#endif
#ifdef ZCL_ATTRID_REMAINING_TIME_ENABLE
    { ZCL_ATTRID_REMAINING_TIME,          ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_remainingTime },
#endif
#ifdef ZCL_ATTRID_CURRENT_X_ENABLE
    { ZCL_ATTRID_CURRENT_X,               ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_currentX },
#endif
#ifdef ZCL_ATTRID_CURRENT_Y_ENABLE
    { ZCL_ATTRID_CURRENT_Y,               ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_currentY },
#endif
#ifdef ZCL_ATTRID_DRIFT_COMPENSATION_ENABLE
    { ZCL_ATTRID_DRIFT_COMPENSATION,      ZCL_DATA_TYPE_ENUM8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_driftComensation },
#endif    
#ifdef ZCL_ATTRID_COMPENSATION_TEXT_ENABLE
    { ZCL_ATTRID_COMPENSATION_TEXT,       ZCL_DATA_TYPE_CHAR_STR,   ACCESS_CONTROL_READ,     (u8*) zcl_attr_lightColorCtrl.zcl_attr_compensationText },
#endif
#ifdef ZCL_ATTRID_COLOR_TEMPERATURE_MIREDS_ENABLE
    { ZCL_ATTRID_COLOR_TEMPERATURE_MIREDS,ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorTemperatureMireds },
#endif
#ifdef ZCL_ATTRID_COLOR_MODE_ENABLE
    { ZCL_ATTRID_COLOR_MODE,              ZCL_DATA_TYPE_ENUM8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorMode },
#endif
#ifdef ZCL_ATTRID_COLOR_OPTIONS_ENABLE
    { ZCL_ATTRID_COLOR_OPTIONS,           ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_options },
#endif
#ifdef ZCL_ATTRID_ENHANCED_CURRENT_HUE_ENABLE
    { ZCL_ATTRID_ENHANCED_CURRENT_HUE,    ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_enhancedCurrentHue },
#endif
#ifdef ZCL_ATTRID_ENHANCED_COLOR_MODE_ENABLE
    { ZCL_ATTRID_ENHANCED_COLOR_MODE,     ZCL_DATA_TYPE_ENUM8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_enhancedColorMode },
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_ACTIVE_ENABLE
    { ZCL_ATTRID_COLOR_LOOP_ACTIVE,       ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorLoopActive },
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_DIRECTION_ENABLE
    { ZCL_ATTRID_COLOR_LOOP_DIRECTION,    ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorLoopDirection },
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_TIME_ENABLE
    { ZCL_ATTRID_COLOR_LOOP_TIME,         ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorLoopTime },
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_START_ENHANCED_HUE_ENABLE
    { ZCL_ATTRID_COLOR_LOOP_START_ENHANCED_HUE,   ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorLoopStartEnhancedHue },
#endif
#ifdef ZCL_ATTRID_COLOR_LOOP_STORED_ENHANCED_HUE_ENABLE
    { ZCL_ATTRID_COLOR_LOOP_STORED_ENHANCED_HUE,  ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorLoopStoredEnhancedHue },
#endif
#ifdef ZCL_ATTRID_COLOR_CAPABILITIES_ENABLE
    { ZCL_ATTRID_COLOR_CAPABILITIES,       ZCL_DATA_TYPE_BITMAP16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorCapabilities },
#endif
#ifdef ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MIN_MIREDS_ENABLE
    { ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MIN_MIREDS,  ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorTempPhysicalMinMireds },
#endif
#ifdef ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MAX_MIREDS_ENABLE
    { ZCL_ATTRID_COLOR_TEMP_PHYSICAL_MAX_MIREDS,  ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorTempPhysicalMaxMireds },
#endif
#ifdef ZCL_ATTRID_COUPLE_COLOR_TEMP_TO_LEVEL_MIN_MIREDS_ENABLE
    { ZCL_ATTRID_COUPLE_COLOR_TEMP_TO_LEVEL_MIN_MIREDS,  ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_coupleColorTempToLevelMinMireds },
#endif
#ifdef ZCL_ATTRID_START_UP_COLOR_TEMPERATURE_MIREDS_ENABLE
    { ZCL_ATTRID_START_UP_COLOR_TEMPERATURE_MIREDS,  ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_startUpColorTemperatureMireds },
#endif
/* Defined Primaries Information Attribute Set */
#ifdef ZCL_ATTRID_NUMBER_OF_PRIMARIES_ENABLE    
    { ZCL_ATTRID_NUMBER_OF_PRIMARIES,     ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_numOfprimaries },
#endif
#ifdef ZCL_ATTRID_PRIMARY1_X_ENABLE    
    { ZCL_ATTRID_PRIMARY1_X,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary1X },
#endif
#ifdef ZCL_ATTRID_PRIMARY1_Y_ENABLE    
    { ZCL_ATTRID_PRIMARY1_Y,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary1Y },
#endif
#ifdef ZCL_ATTRID_PRIMARY1_INTENSITY_ENABLE
    { ZCL_ATTRID_PRIMARY1_INTENSITY,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary1Intensity },
#endif    
#ifdef ZCL_ATTRID_PRIMARY2_X_ENABLE
    { ZCL_ATTRID_PRIMARY2_X,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary2X },
#endif    
#ifdef ZCL_ATTRID_PRIMARY2_Y_ENABLE
    { ZCL_ATTRID_PRIMARY2_Y,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary2Y },
#endif    
#ifdef ZCL_ATTRID_PRIMARY2_INTENSITY_ENABLE
    { ZCL_ATTRID_PRIMARY2_INTENSITY,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary2Intensity },
#endif    
#ifdef ZCL_ATTRID_PRIMARY3_X_ENABLE
    { ZCL_ATTRID_PRIMARY3_X,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary3X },
#endif    
#ifdef ZCL_ATTRID_PRIMARY3_Y_ENABLE
    { ZCL_ATTRID_PRIMARY3_Y,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary3Y },
#endif      
#ifdef ZCL_ATTRID_PRIMARY3_INTENSITY_ENABLE
    { ZCL_ATTRID_PRIMARY3_INTENSITY,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary3Intensity },
#endif
/* Additional Defined Primaries Information Attribute Set */
#ifdef ZCL_ATTRID_PRIMARY4_X_ENABLE
    { ZCL_ATTRID_PRIMARY4_X,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary4X },
#endif    
#ifdef ZCL_ATTRID_PRIMARY4_Y_ENABLE
    { ZCL_ATTRID_PRIMARY4_Y,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary4Y },
#endif    
#ifdef ZCL_ATTRID_PRIMARY4_INTENSITY_ENABLE
    { ZCL_ATTRID_PRIMARY4_INTENSITY,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary4Intensity },
#endif    
#ifdef ZCL_ATTRID_PRIMARY5_X_ENABLE
    { ZCL_ATTRID_PRIMARY5_X,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary5X },
#endif    
#ifdef ZCL_ATTRID_PRIMARY5_Y_ENABLE
    { ZCL_ATTRID_PRIMARY5_Y,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary5Y },
#endif    
#ifdef ZCL_ATTRID_PRIMARY5_INTENSITY_ENABLE
    { ZCL_ATTRID_PRIMARY5_INTENSITY,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary5Intensity },
#endif    
#ifdef ZCL_ATTRID_PRIMARY6_X_ENABLE
    { ZCL_ATTRID_PRIMARY6_X,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary6X },
#endif    
#ifdef ZCL_ATTRID_PRIMARY6_Y_ENABLE
    { ZCL_ATTRID_PRIMARY6_Y,              ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary6Y },
#endif    
#ifdef ZCL_ATTRID_PRIMARY6_INTENSITY_ENABLE
    { ZCL_ATTRID_PRIMARY6_INTENSITY,      ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_primary6Intensity },
#endif
/* Defined Color Points Settings Attribute Set */
#ifdef ZCL_ATTRID_WHITE_POINT_X_ENABLE
    { ZCL_ATTRID_WHITE_POINT_X,           ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_whitePointX },
#endif  
#ifdef ZCL_ATTRID_WHITE_POINT_Y_ENABLE
    { ZCL_ATTRID_WHITE_POINT_Y,           ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_whitePointY },
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_RX_ENABLE
    { ZCL_ATTRID_COLOR_POINT_RX,          ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointRX },
#endif    
#ifdef ZCL_ATTRID_COLOR_POINT_RY_ENABLE
    { ZCL_ATTRID_COLOR_POINT_RY,          ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointRY },
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_RINTENSITY_ENABLE
    { ZCL_ATTRID_COLOR_POINT_RINTENSITY,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointRIntensity },
#endif    
#ifdef ZCL_ATTRID_COLOR_POINT_GX_ENABLE
    { ZCL_ATTRID_COLOR_POINT_GX,          ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointGX },
#endif    
#ifdef ZCL_ATTRID_COLOR_POINT_GY_ENABLE
    { ZCL_ATTRID_COLOR_POINT_GY,          ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointGY },
#endif    
#ifdef ZCL_ATTRID_COLOR_POINT_GINTENSITY_ENABLE
    { ZCL_ATTRID_COLOR_POINT_GINTENSITY,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointGIntensity },
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_BX_ENABLE
    { ZCL_ATTRID_COLOR_POINT_BX,          ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointBX },
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_BY_ENABLE
    { ZCL_ATTRID_COLOR_POINT_BY,          ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointBY },
#endif
#ifdef ZCL_ATTRID_COLOR_POINT_BINTENSITY_ENABLE
    { ZCL_ATTRID_COLOR_POINT_BINTENSITY,  ZCL_DATA_TYPE_UINT8,   ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,     (u8*)&zcl_attr_lightColorCtrl.zcl_attr_colorPointBIntensity },
#endif
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_lightColorCtrl_attrNum = ( sizeof(lightColorCtrl_attrTbl) / sizeof(zclAttrInfo_t) );

nv_sts_t zcl_colorCtrl_saveAttr(void)
{
	return zcl_attr_colorCtrlParams_save();
}

nv_sts_t zcl_colorCtrl_loadAttr(void)
{
	return zcl_attr_colorCtrlParams_restore();
}

#endif  /* ZCL_LIGHT_COLOR_CONTROL */



#endif



