/********************************************************************************************************
 * @file     zcl_include.h
 *
 * @brief	 list all the header files relevant to cluster
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

#include "../include/zb_common.h"

#include "zcl_const.h"
#include "zcl_config.h"
#include "zcl.h"
#include "zcl_nv.h"
/* General */
#include "general/zcl_basic.h"
#include "general/zcl_powerCfg.h"
#include "general/zcl_devTemperatureCfg.h"
#include "general/zcl_identify.h"
#include "general/zcl_group.h"
#include "general/zcl_scene.h"
#include "general/zcl_onoff.h"
#include "general/zcl_level.h"
#include "general/zcl_alarm.h"
#include "general/zcl_time.h"
#include "general/zcl_diagnostics.h"
#include "general/zcl_pollCtrl.h"
#include "general/zcl_greenPower.h"

//Closures
#include "closures/zcl_window_covering.h"
#include "closures/zcl_door_lock.h"

/* Security and Safety */
#include "security_safety/zcl_ias_zone.h"
#include "security_safety/zcl_ias_ace.h"
#include "security_safety/zcl_ias_wd.h"

/* Measurement and Sensing */
#include "measument_sensing/zcl_illuminance_measurement.h"
#include "measument_sensing/zcl_occupancy_sensing.h"
#include "measument_sensing/zcl_temperature_measurement.h"
#include "measument_sensing/zcl_electrical_measurement.h"

/* Color Control */
#include "light_color_control/zcl_light_colorCtrl.h"

/* HVAC */
#include "hvac/zcl_thermostat.h"

/* Smart Energy */
#include "smart_energy/zcl_metering.h"

/* ZCL Commissioning */
#include "commissioning/zcl_commissioning.h"

/* ZLL Commissioning */
#include "zll_commissioning/zcl_zll_commissioning.h"
#include "zll_commissioning/zcl_touchlink_attr.h"

/* OTA Upgrading */
#include "ota_upgrading/zcl_ota.h"

/* Work with all hub */
#include "wwah/zcl_wwah.h"
