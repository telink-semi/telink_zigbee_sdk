/********************************************************************************************************
 * @file    zcl_include.h
 *
 * @brief   This is the header file for zcl_include
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

#ifndef ZCL_INCLUDE_H
#define ZCL_INCLUDE_H

#include "../common/includes/zb_common.h"

#include "zcl_config.h"
#include "zcl_const.h"
#include "zcl_nv.h"
#include "zcl.h"

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
#include "zcl_wwah/zcl_wwah.h"

#endif
