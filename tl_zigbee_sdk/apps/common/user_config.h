/********************************************************************************************************
 * @file     user_config.c
 *
 * @brief    User build configuration
 *
 * @author
 * @date     Feb. 1, 2017
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



#if (__PROJECT_TL_GW__)
	#include "../sampleGW/app_cfg.h"
#elif (__PROJECT_TL_DIMMABLE_LIGHT__)
	#include "../sampleLight/app_cfg.h"
#elif (__PROJECT_TL_SWITCH__)
	#include "../sampleSwitch/app_cfg.h"
#elif (__PROJECT_TL_SWITCH_8258__)
    #include "../sampleSwitch_8258/app_cfg.h"
#elif (__PROJECT_TL_SWITCH_8278__)
    #include "../sampleSwitch_8278/app_cfg.h"
#elif (__PROJECT_TL_CONTACT_SENSOR__)
	#include "../sampleContactSensor/app_cfg.h"
#elif (__PROJECT_TL_CONTACT_SENSOR_8258__)
	#include "../sampleContactSensor_8258/app_cfg.h"
#elif (__PROJECT_TL_TEMPERATURE_SENSOR_8258__)
	#include "../sampleTemperatureSensor_8258/app_cfg.h"
#elif (__PROJECT_TL_TEMPERATURE_SENSOR__)
	#include "../sampleTemperatureSensor/app_cfg.h"
#elif (__PROJECT_TL_DOOR_LOCK_8258__)
	#include "../sampleDoorLock_8258/app_cfg.h"
#elif (__PROJECT_TL_SWITCH_HAWK__)
    #include "../sampleSwitch_hawk/app_cfg.h"
#elif (__PROJECT_DUAL_MODE_LIGHT__)
	#include "../dualModeLight/app_cfg.h"
#elif (__PROJECT_TLZBRC__)
	#include "../TelinkZbRC/app_cfg.h"
#elif (__PROJECT_TELINK_LIGHT__)
	#include "../telinkLight/app_cfg.h"
#elif (__PROJECT_HUE_DIM_LIGHT__)
	#include "../hueDimLight/app_cfg.h"
#elif (__PROJECT_GW_MAIKE__)
	#include "../gw_Maike/app_cfg.h"
#elif (__PROJECT_LIGHTING_HT__)
	#include "../lighting_HT/app_cfg.h"
#elif (__PROJECT_PERFORMANCE_TEST__)
	#include "../appNetworkPerformanceTest/app_cfg.h"
#elif (__PROJECT_TL_SNIFFER__)
	#include "../zbSniffer/app_cfg.h"
#else

#endif


