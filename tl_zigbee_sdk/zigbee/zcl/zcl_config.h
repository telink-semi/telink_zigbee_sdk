/********************************************************************************************************
 * @file     zcl_config.h
 *
 * @brief	 configuration of zigbee cluster
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
#ifndef _ZCL_CONFIG_H_
#define _ZCL_CONFIG_H_


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


#endif	/* _ZCL_CONFIG_H_ */
