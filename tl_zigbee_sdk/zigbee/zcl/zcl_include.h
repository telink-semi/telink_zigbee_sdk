/********************************************************************************************************
 * @file	zcl_include.h
 *
 * @brief	This is the header file for zcl_include
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
#include "wwah/zcl_wwah.h"

#endif
