/********************************************************************************************************
 * @file	zcl_powerCfg_attr.c
 *
 * @brief	This is the source file for zcl_powerCfg_attr
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
#if 0
#include "tl_common.h"
#include "../zcl_include.h"

#ifdef ZCL_POWER

u16 zcl_power_config_attr_MainsVoltage                      ;
u8 zcl_power_config_attr_MainsFrequency                    ;
u8 zcl_power_config_attr_MainsAlarmMask                    ;
u16 zcl_power_config_attr_MainsVoltageMinThreshold          ;
u16 zcl_power_config_attr_MainsVoltageMaxThreshold = 0xffff         ;
u16 zcl_power_config_attr_MainsVoltageDwellTripPoint        ;
u8 zcl_power_config_attr_BatteryVoltage                    ;
u8 zcl_power_config_attr_BatteryPercentageRemaining        ;
u8 zcl_power_config_attr_BatteryManufacturer[ZCL_BATTERYMANUFACTURER_MAX_LENGTH + 1]             ;
u8 zcl_power_config_attr_BatterySize  = 0xff                     ;
u16 zcl_power_config_attr_BatteryAHrRating                  ;
u8 zcl_power_config_attr_BatteryQuantity                   ;
u8 zcl_power_config_attr_BatteryRatedVoltage               ;
u8 zcl_power_config_attr_BatteryAlarmMask                  ;
u8 zcl_power_config_attr_BatteryVoltageMinThreshold        ;
u8 zcl_power_config_attr_BatteryVoltageThreshold1          ;
u8 zcl_power_config_attr_BatteryVoltageThreshold2          ;
u8 zcl_power_config_attr_BatteryVoltageThreshold3          ;
u8 zcl_power_config_attr_BatteryPercentageMinThreshold     ;
u8 zcl_power_config_attr_BatteryPercentageThreshold1       ;
u8 zcl_power_config_attr_BatteryPercentageThreshold2       ;
u8 zcl_power_config_attr_BatteryPercentageThreshold3       ;
u32 zcl_power_config_attr_BatteryAlarmState                 ;


const zclAttrInfo_t power_config_attrTbl[] =
{
{ZCL_POWER_CONFIG_ATTRID_MAINSVOLTAGE				    ,ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,(u8*)&zcl_power_config_attr_MainsVoltage					},
{ZCL_POWER_CONFIG_ATTRID_MAINSFREQUENCY				    ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ,(u8*)&zcl_power_config_attr_MainsFrequency                  },
{ZCL_POWER_CONFIG_ATTRID_MAINSALARMMASK				    ,ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_MainsAlarmMask                  },
{ZCL_POWER_CONFIG_ATTRID_MAINSVOLTAGEMINTHRESHOLD	    ,ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_MainsVoltageMinThreshold        },
{ZCL_POWER_CONFIG_ATTRID_MAINSVOLTAGEMAXTHRESHOLD	    ,ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_MainsVoltageMaxThreshold        },
{ZCL_POWER_CONFIG_ATTRID_MAINSVOLTAGEDWELLTRIPPOINT	    ,ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_MainsVoltageDwellTripPoint      },
{ZCL_POWER_CONFIG_ATTRID_BATTERYVOLTAGE				    ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ,(u8*)&zcl_power_config_attr_BatteryVoltage                  },
{ZCL_POWER_CONFIG_ATTRID_BATTERYPERCENTAGEREMAINING	    ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_REPORTABLE,(u8*)&zcl_power_config_attr_BatteryPercentageRemaining      },
{ZCL_POWER_CONFIG_ATTRID_BATTERYMANUFACTURER	        ,ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,zcl_power_config_attr_BatteryManufacturer             },
{ZCL_POWER_CONFIG_ATTRID_BATTERYSIZE			        ,ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatterySize                     },
{ZCL_POWER_CONFIG_ATTRID_BATTERYAHRRATING		        ,ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryAHrRating                },
{ZCL_POWER_CONFIG_ATTRID_BATTERYQUANTITY		        ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryQuantity                 },
{ZCL_POWER_CONFIG_ATTRID_BATTERYRATEDVOLTAGE	        ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryRatedVoltage             },
{ZCL_POWER_CONFIG_ATTRID_BATTERYALARMMASK			    ,ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryAlarmMask                },
{ZCL_POWER_CONFIG_ATTRID_BATTERYVOLTAGEMINTHRESHOLD	    ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryVoltageMinThreshold      },
{ZCL_POWER_CONFIG_ATTRID_BATTERYVOLTAGETHRESHOLD1	    ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryVoltageThreshold1        },
{ZCL_POWER_CONFIG_ATTRID_BATTERYVOLTAGETHRESHOLD2	    ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryVoltageThreshold2        },
{ZCL_POWER_CONFIG_ATTRID_BATTERYVOLTAGETHRESHOLD3	    ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryVoltageThreshold3        },
{ZCL_POWER_CONFIG_ATTRID_BATTERYPERCENTAGEMINTHRESHOLD  ,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryPercentageMinThreshold   },
{ZCL_POWER_CONFIG_ATTRID_BATTERYPERCENTAGETHRESHOLD1	,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryPercentageThreshold1     },
{ZCL_POWER_CONFIG_ATTRID_BATTERYPERCENTAGETHRESHOLD2	,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryPercentageThreshold2     },
{ZCL_POWER_CONFIG_ATTRID_BATTERYPERCENTAGETHRESHOLD3	,ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ|ACCESS_CONTROL_WRITE,(u8*)&zcl_power_config_attr_BatteryPercentageThreshold3     },
{ZCL_POWER_CONFIG_ATTRID_BATTERYALARMSTATE			    ,ZCL_DATA_TYPE_BITMAP32, ACCESS_CONTROL_READ,(u8*)&zcl_power_config_attr_BatteryAlarmState               },
{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_powerConfig_attrNum = ( sizeof(power_config_attrTbl) / sizeof(zclAttrInfo_t) );

#endif

#endif
