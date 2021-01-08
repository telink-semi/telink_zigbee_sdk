/********************************************************************************************************
 * @file	zcl_config.h
 *
 * @brief	This is the header file for zcl_config
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
#ifndef ZCL_CONFIG_H
#define ZCL_CONFIG_H


#include "tl_common.h"


#define ZCL_READ
#define ZCL_WRITE
#define ZCL_REPORT
#define ZCL_DISCOVER


/********************************************* General ********************************************/
/********************* Baisc Cluster Configuration **********************/
#define ZCL_BASIC

/********************* Power Cluster Configuration **********************/
#if ZCL_POWER_CFG_SUPPORT
#define ZCL_POWER_CFG
#endif

/*********** Device Temperature Config Cluster Configuration ************/
#if ZCL_DEV_TEMPERATURE_CFG_SUPPORT
#define ZCL_DEV_TEMPERATURE_CFG
#endif

/******************** Identify Cluster Configuration ********************/
#define ZCL_IDENTIFY

/********************* Group Cluster Configuration **********************/
#if ZCL_GROUP_SUPPORT
#define ZCL_GROUP
#endif

/********************* Scene Cluster Configuration **********************/
#if ZCL_SCENE_SUPPORT
#define ZCL_SCENE
#endif

/********************* OnOff Cluster Configuration **********************/
#if ZCL_ON_OFF_SUPPORT
#define ZCL_ON_OFF
#endif

/************** OnOff Switch Config Cluster Configuration ***************/
#if ZCL_ON_OFF_SWITCH_CFG_SUPPORT
#define ZCL_ON_OFF_SWITCH_CFG
#endif

/***************** Level Control Cluster Configuration ******************/
#if ZCL_LEVEL_CTRL_SUPPORT
#define ZCL_LEVEL_CTRL
#endif

/********************* Alarm Cluster Configuration **********************/
#if ZCL_ALARMS_SUPPORT
#define ZCL_ALARMS
#endif

/********************* Time Cluster Configuration **********************/
#if ZCL_TIME_SUPPORT
#define ZCL_TIME
#endif

/***************** RSSI Location Cluster Configuration *****************/
#if ZCL_RSSI_LOCATION_SUPPORT
#define ZCL_RSSI_LOCATION
#endif

/********************* Diagnostics Configuration ***********************/
#if ZCL_DIAGNOSTICS_SUPPORT
#define ZCL_DIAGNOSTICS
#endif

/********************* Poll Control Configuration **********************/
#if ZCL_POLL_CTRL_SUPPORT
#define ZCL_POLL_CTRL
#endif

/********************* Green Power Configuration **********************/
#if GP_SUPPORT_ENABLE
#define ZCL_GREEN_POWER
#endif


/********************************************* Measurement and Sensing ********************************************/
/********************* Illuminance Measurement ************************/
#if ZCL_ILLUMINANCE_MEASUREMENT_SUPPORT
#define ZCL_ILLUMINANCE_MEASUREMENT
#endif

/********************* Illuminance Level Sensing **********************/
#if ZCL_ILLUMINANCE_LEVEL_SENSING_SUPPORT
#define ZCL_ILLUMINANCE_LEVEL_SENSING
#endif

/********************* Temperature Measurement ************************/
#if ZCL_TEMPERATURE_MEASUREMENT_SUPPORT
#define ZCL_TEMPERATURE_MEASUREMENT
#endif

/************************ Occupancy Sensing ***************************/
#if ZCL_OCCUPANCY_SENSING_SUPPORT
#define ZCL_OCCUPANCY_SENSING
#endif

/********************* Electrical Measurement ************************/
#if ZCL_ELECTRICAL_MEASUREMENT_SUPPORT
#define ZCL_ELECTRICAL_MEASUREMENT
#endif


/*************************************************** Lighting ****************************************************/
/************************* Color Control *****************************/
#if ZCL_LIGHT_COLOR_CONTROL_SUPPORT
#define ZCL_LIGHT_COLOR_CONTROL
#endif

/********************* Ballast Configuration *************************/
//#define ZCL_BALLAST_CFG


/***************************************************** HVAC ******************************************************/
/*************************** THERMOSTAT ******************************/
#if ZCL_THERMOSTAT_SUPPORT
#define ZCL_THERMOSTAT
#endif

/*************************************************** Closures ****************************************************/
/*************************** Door Lock *******************************/
#if ZCL_DOOR_LOCK_SUPPORT
#define ZCL_DOOR_LOCK
#endif

/************************* Window Covering ***************************/
#if ZCL_WINDOW_COVERING_SUPPORT
#define ZCL_WINDOW_COVERING
#endif


/************************************************** Safe and Security ********************************************/
/***************************** IAS Zone *****************************/
#if ZCL_IAS_ZONE_SUPPORT
#define ZCL_IAS_ZONE
#endif

/***************************** IAS ACE ******************************/
#if ZCL_IAS_ACE_SUPPORT
#define ZCL_IAS_ACE
#endif

/***************************** IAS WD *******************************/
#if ZCL_IAS_WD_SUPPORT
#define ZCL_IAS_WD
#endif


/*************************************************** Smart Energy ***********************************************/
/******************* Metering Cluster Configuration *****************/
#if ZCL_METERING_SUPPORT
#define ZCL_METERING
#endif


/************************************************* OTA Upgrading ************************************************/
/********************* OTA Configuration **********************/
/* OTA upgrading */
#if ZCL_OTA_SUPPORT
#define ZCL_OTA
#endif

/************************************************* Commissioning ************************************************/
/********************* Commissioning *************************/
//#define ZCL_COMMISSIONING

/**************** Touchlink Commissioning ********************/
/* ZLL commissioning */
#if ZCL_ZLL_COMMISSIONING_SUPPORT
#define ZCL_ZLL_COMMISSIONING
#endif


/****************************************************** WWAH ****************************************************/
/* Work with all hub */
#if ZCL_WWAH_SUPPORT
#define ZCL_WWAH
#endif


#endif	/* ZCL_CONFIG_H */
