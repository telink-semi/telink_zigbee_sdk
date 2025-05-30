/********************************************************************************************************
 * @file    zcl_config.h
 *
 * @brief   This is the header file for zcl_config
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
#ifndef ZCL_CONFIG_H
#define ZCL_CONFIG_H


#include "tl_common.h"


#define ZCL_READ
#define ZCL_WRITE
#define ZCL_REPORT
#define ZCL_DISCOVER


/********************************************* General ********************************************/
/********************* Basic Cluster Configuration **********************/
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

/********************* Green Power Configuration ***********************/
#if ZCL_GP_SUPPORT
#define ZCL_GREEN_POWER
#endif

/********************* Binary Input Configuration **********************/
#if ZCL_BINARY_INPUT_SUPPORT
#define ZCL_BINARY_INPUT
#endif

/********************* Binary Output Configuration *********************/
#if ZCL_BINARY_OUTPUT_SUPPORT
#define ZCL_BINARY_OUTPUT
#endif

/******************** Multistate Input Configuration *******************/
#if ZCL_MULTISTATE_INPUT_SUPPORT
#define ZCL_MULTISTATE_INPUT
#endif

/******************** Multistate Output Configuration ******************/
#if ZCL_MULTISTATE_OUTPUT_SUPPORT
#define ZCL_MULTISTATE_OUTPUT
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
