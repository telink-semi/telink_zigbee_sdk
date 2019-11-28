/********************************************************************************************************
 * @file     zcl_powerCfg_attr.c
 *
 * @brief	 attribute configuration for power cluster
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
