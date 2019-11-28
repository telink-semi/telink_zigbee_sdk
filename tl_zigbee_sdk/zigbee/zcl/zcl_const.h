/********************************************************************************************************
 * @file     zcl_const.h
 *
 * @brief	 the constant define of the cluster
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


/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

//#define HA_PROFILE_ID                      				0x0104
//#define LL_PROFILE_ID                      				0xC05E

/** @addtogroup  ZCL_Module ZCL
 *  @{
 */
 
/** @addtogroup  ZCL_Constant ZCL Constants
 *  @{
 */
//Global Attribute
#define ZCL_ATTRID_GLOBAL_CLUSTER_REVISION				 0xFFFD
#define ZCL_ATTRID_GLOBAL_ATTR_REPORTING_STATUS			 0xFFFE

/** @addtogroup zcl_cluster_id ZCL Cluster Identifier //2.2.2.1
 * @{
 */
#define ZCL_CLUSTER_GEN_BASIC                             0x0000
#define ZCL_CLUSTER_GEN_POWER_CFG                         0x0001
#define ZCL_CLUSTER_GEN_DEVICE_TEMP_CONFIG                0x0002
#define ZCL_CLUSTER_GEN_IDENTIFY                          0x0003
#define ZCL_CLUSTER_GEN_GROUPS                            0x0004
#define ZCL_CLUSTER_GEN_SCENES                            0x0005
#define ZCL_CLUSTER_GEN_ON_OFF                            0x0006
#define ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG              0x0007
#define ZCL_CLUSTER_GEN_LEVEL_CONTROL                     0x0008
#define ZCL_CLUSTER_GEN_ALARMS                            0x0009
#define ZCL_CLUSTER_GEN_TIME                              0x000A
#define ZCL_CLUSTER_GEN_LOCATION                          0x000B
#define ZCL_CLUSTER_GEN_ANALOG_INPUT_BASIC                0x000C
#define ZCL_CLUSTER_GEN_ANALOG_OUTPUT_BASIC               0x000D
#define ZCL_CLUSTER_GEN_ANALOG_VALUE_BASIC                0x000E
#define ZCL_CLUSTER_GEN_BINARY_INPUT_BASIC                0x000F
#define ZCL_CLUSTER_GEN_BINARY_OUTPUT_BASIC               0x0010
#define ZCL_CLUSTER_GEN_BINARY_VALUE_BASIC                0x0011
#define ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC            0x0012
#define ZCL_CLUSTER_GEN_MULTISTATE_OUTPUT_BASIC           0x0013
#define ZCL_CLUSTER_GEN_MULTISTATE_VALUE_BASIC            0x0014
#define ZCL_CLUSTER_GEN_COMMISSIONING                     0x0015
#define ZCL_CLUSTER_GEN_PARTITION						  0x0016
#define ZCL_CLUSTER_OTA                                   0x0019
#define ZCL_CLUSTER_GEN_POWER_PROFILE					  0x001A
#define ZCL_CLUSTER_GEN_APPLIANCE_CONTROL				  0x001B
#define ZCL_CLUSTER_GEN_POLL_CONTROL					  0x0020
#define ZCL_CLUSTER_GEN_GREEN_POWER		                  0x0021
#define ZCL_CLUSTER_GEN_DIAGNOSTICS                       0x0B05

// Closures Clusters
#define ZCL_CLUSTER_CLOSURES_SHADE_CONFIG                 0x0100
#define ZCL_CLUSTER_CLOSURES_DOOR_LOCK                    0x0101
#define ZCL_CLUSTER_CLOSURES_WINDOW_COVERING              0x0102

// HVAC Clusters
#define ZCL_CLUSTER_HVAC_PUMP_CONFIG_CONTROL              0x0200
#define ZCL_CLUSTER_HAVC_THERMOSTAT                       0x0201
#define ZCL_CLUSTER_HAVC_FAN_CONTROL                      0x0202
#define ZCL_CLUSTER_HAVC_DIHUMIDIFICATION_CONTROL         0x0203
#define ZCL_CLUSTER_HAVC_USER_INTERFACE_CONFIG            0x0204

// Lighting Clusters
#define ZCL_CLUSTER_LIGHTING_COLOR_CONTROL                0x0300
#define ZCL_CLUSTER_LIGHTING_BALLAST_CONFIG               0x0301

// Measurement and Sensing Clusters
#define ZCL_CLUSTER_MS_ILLUMINANCE_MEASUREMENT            0x0400
#define ZCL_CLUSTER_MS_ILLUMINANCE_LEVEL_SENSING_CONFIG   0x0401
#define ZCL_CLUSTER_MS_TEMPERATURE_MEASUREMENT            0x0402
#define ZCL_CLUSTER_MS_PRESSURE_MEASUREMENT               0x0403
#define ZCL_CLUSTER_MS_FLOW_MEASUREMENT                   0x0404
#define ZCL_CLUSTER_MS_RELATIVE_HUMIDITY                  0x0405
#define ZCL_CLUSTER_MS_OCCUPANCY_SENSING                  0x0406
#define ZCL_CLUSTER_MS_LEAF_WETNESS						  0x0407
#define ZCL_CLUSTER_MS_SOIL_MOISTURE					  0x0408
#define ZCL_CLUSTER_MS_ELECTRICAL_MEASUREMENT			  0x0B04

// Security and Safety (SS) Clusters
#define ZCL_CLUSTER_SS_IAS_ZONE                           0x0500
#define ZCL_CLUSTER_SS_IAS_ACE                            0x0501
#define ZCL_CLUSTER_SS_IAS_WD                             0x0502

// Protocol Interfaces
#define ZCL_CLUSTER_PI_GENERIC_TUNNEL                     0x0600
#define ZCL_CLUSTER_PI_BACNET_PROTOCOL_TUNNEL             0x0601
#define ZCL_CLUSTER_PI_ANALOG_INPUT_BACNET_REG            0x0602
#define ZCL_CLUSTER_PI_ANALOG_INPUT_BACNET_EXT            0x0603
#define ZCL_CLUSTER_PI_ANALOG_OUTPUT_BACNET_REG           0x0604
#define ZCL_CLUSTER_PI_ANALOG_OUTPUT_BACNET_EXT           0x0605
#define ZCL_CLUSTER_PI_ANALOG_VALUE_BACNET_REG            0x0606
#define ZCL_CLUSTER_PI_ANALOG_VALUE_BACNET_EXT            0x0607
#define ZCL_CLUSTER_PI_BINARY_INPUT_BACNET_REG            0x0608
#define ZCL_CLUSTER_PI_BINARY_INPUT_BACNET_EXT            0x0609
#define ZCL_CLUSTER_PI_BINARY_OUTPUT_BACNET_REG           0x060A
#define ZCL_CLUSTER_PI_BINARY_OUTPUT_BACNET_EXT           0x060B
#define ZCL_CLUSTER_PI_BINARY_VALUE_BACNET_REG            0x060C
#define ZCL_CLUSTER_PI_BINARY_VALUE_BACNET_EXT            0x060D
#define ZCL_CLUSTER_PI_MULTISTATE_INPUT_BACNET_REG        0x060E
#define ZCL_CLUSTER_PI_MULTISTATE_INPUT_BACNET_EXT        0x060F
#define ZCL_CLUSTER_PI_MULTISTATE_OUTPUT_BACNET_REG       0x0610
#define ZCL_CLUSTER_PI_MULTISTATE_OUTPUT_BACNET_EXT       0x0611
#define ZCL_CLUSTER_PI_MULTISTATE_VALUE_BACNET_REG        0x0612
#define ZCL_CLUSTER_PI_MULTISTATE_VALUE_BACNET_EXT        0x0613
#define ZCL_CLUSTER_PI_11073_PROTOCOL_TUNNEL              0x0614

// Smart Energy Clusters
#define ZCL_CLUSTER_SE_PRICE                              0x0700
#define ZCL_CLUSTER_SE_Demand_RSP_AND_LOAD_CONTROL        0x0701
#define ZCL_CLUSTER_SE_METERING                    		  0x0702
#define ZCL_CLUSTER_SE_MESSAGING                          0x0703
#define ZCL_CLUSTER_SE_SE_TUNNELING                       0x0704
#define ZCL_CLUSTER_SE_PREPAYMENT                         0x0705
#define ZCL_CLUSTER_GEN_KEY_ESTABLISHMENT                 0x0800

// Light Link cluster
#define ZCL_CLUSTER_TOUCHLINK_COMMISSIONING               0x1000

// Work with all hub cluster
#define ZCL_CLUSTER_WWAH								  0xFEDC


// test cluster
#define ZCL_CLUSTER_TELINK_SDK_TEST						  0xFF00
#define ZCL_CLUSTER_TELINK_SDK_TEST_REQ					  0xFF01
#define ZCL_CLUSTER_TELINK_SDK_TEST_RSP					  0xFF02
#define ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_REQ			  0xFF03
#define ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_RSP			  0xFF04

/** @} end of group zcl_cluster_id */



/** @addtogroup zcl_data_types ZCL Attribute Data Types
 * @{
 */
#define ZCL_DATA_TYPE_NO_DATA                            0x00
#define ZCL_DATA_TYPE_DATA8                              0x08
#define ZCL_DATA_TYPE_DATA16                             0x09
#define ZCL_DATA_TYPE_DATA24                             0x0a
#define ZCL_DATA_TYPE_DATA32                             0x0b
#define ZCL_DATA_TYPE_DATA40                             0x0c
#define ZCL_DATA_TYPE_DATA48                             0x0d
#define ZCL_DATA_TYPE_DATA56                             0x0e
#define ZCL_DATA_TYPE_DATA64                             0x0f
#define ZCL_DATA_TYPE_BOOLEAN                            0x10
#define ZCL_DATA_TYPE_BITMAP8                            0x18
#define ZCL_DATA_TYPE_BITMAP16                           0x19
#define ZCL_DATA_TYPE_BITMAP24                           0x1a
#define ZCL_DATA_TYPE_BITMAP32                           0x1b
#define ZCL_DATA_TYPE_BITMAP40                           0x1c
#define ZCL_DATA_TYPE_BITMAP48                           0x1d
#define ZCL_DATA_TYPE_BITMAP56                           0x1e
#define ZCL_DATA_TYPE_BITMAP64                           0x1f
#define ZCL_DATA_TYPE_UINT8                              0x20
#define ZCL_DATA_TYPE_UINT16                             0x21
#define ZCL_DATA_TYPE_UINT24                             0x22
#define ZCL_DATA_TYPE_UINT32                             0x23
#define ZCL_DATA_TYPE_UINT40                             0x24
#define ZCL_DATA_TYPE_UINT48                             0x25
#define ZCL_DATA_TYPE_UINT56                             0x26
#define ZCL_DATA_TYPE_UINT64                             0x27
#define ZCL_DATA_TYPE_INT8                               0x28
#define ZCL_DATA_TYPE_INT16                              0x29
#define ZCL_DATA_TYPE_INT24                              0x2a
#define ZCL_DATA_TYPE_INT32                              0x2b
#define ZCL_DATA_TYPE_INT40                              0x2c
#define ZCL_DATA_TYPE_INT48                              0x2d
#define ZCL_DATA_TYPE_INT56                              0x2e
#define ZCL_DATA_TYPE_INT64                              0x2f
#define ZCL_DATA_TYPE_ENUM8                              0x30
#define ZCL_DATA_TYPE_ENUM16                             0x31
#define ZCL_DATA_TYPE_SEMI_PREC                          0x38
#define ZCL_DATA_TYPE_SINGLE_PREC                        0x39
#define ZCL_DATA_TYPE_DOUBLE_PREC                        0x3a
#define ZCL_DATA_TYPE_OCTET_STR                          0x41
#define ZCL_DATA_TYPE_CHAR_STR                           0x42
#define ZCL_DATA_TYPE_LONG_OCTET_STR                     0x43
#define ZCL_DATA_TYPE_LONG_CHAR_STR                      0x44
#define ZCL_DATA_TYPE_ARRAY                              0x48
#define ZCL_DATA_TYPE_STRUCT                             0x4c
#define ZCL_DATA_TYPE_SET                                0x50
#define ZCL_DATA_TYPE_BAG                                0x51
#define ZCL_DATA_TYPE_TOD                                0xe0
#define ZCL_DATA_TYPE_DATE                               0xe1
#define ZCL_DATA_TYPE_UTC                                0xe2
#define ZCL_DATA_TYPE_CLUSTER_ID                         0xe8
#define ZCL_DATA_TYPE_ATTR_ID                            0xe9
#define ZCL_DATA_TYPE_BAC_OID                            0xea
#define ZCL_DATA_TYPE_IEEE_ADDR                          0xf0
#define ZCL_DATA_TYPE_128_BIT_SEC_KEY                    0xf1
#define ZCL_DATA_TYPE_UNKNOWN                            0xff
/** @} end of group zcl_data_types */



/** @addtogroup zcl_attribute_accessCtrl ZCL Attribute Access Control
 * @{
 */
#define ACCESS_CONTROL_READ                             0x01
#define ACCESS_CONTROL_WRITE                            0x02
#define ACCESS_CONTROL_REPORTABLE                       0x04
#define ACCESS_CONTROL_SCENE							0x08
#define ACCESS_CONTROL_AUTH_READ                        0x10
#define ACCESS_CONTROL_AUTH_WRITE                       0x20
#define ACCESS_CONTROL_COMMAND                          0x40
/* To differentiate between server side and client side attributes. Client side attributes will have this bit set in the access field */
#define ACCESS_CONTROL_CLIENT_ATTR                      0x80

/* for discovery attributes extended. read/write/reportable bits mask */
#define ACCESS_CONTROL_EXT_MASK							0x07

/** @} end of group zcl_attribute_accessCtrl */

/* used for configure reporting command */
#define ZCL_SEND_ATTR_REPORTS							0x00
#define ZCL_EXPECT_ATTR_REPORTS							0x01

/** @addtogroup zcl_error_codes ZCL Error Codes
 * @{
 */

/*** Error Status Codes ***/
#define ZCL_STA_SUCCESS                              0x00
#define ZCL_STA_FAILURE                              0x01

// 0x20- 0x7D are reserved.
#define ZCL_STA_NOT_AUTHORIZED                       0x7E
#define ZCL_STA_MALFORMED_COMMAND                    0x80
#define ZCL_STA_UNSUP_CLUSTER_COMMAND                0x81
#define ZCL_STA_UNSUP_GENERAL_COMMAND                0x82
#define ZCL_STA_UNSUP_MANU_CLUSTER_COMMAND           0x83
#define ZCL_STA_UNSUP_MANU_GENERAL_COMMAND           0x84
#define ZCL_STA_INVALID_FIELD                        0x85
#define ZCL_STA_UNSUPPORTED_ATTRIBUTE                0x86
#define ZCL_STA_INVALID_VALUE                        0x87
#define ZCL_STA_READ_ONLY                            0x88
#define ZCL_STA_INSUFFICIENT_SPACE                   0x89
#define ZCL_STA_DUPLICATE_EXISTS                     0x8a
#define ZCL_STA_NOT_FOUND                            0x8b
#define ZCL_STA_UNREPORTABLE_ATTRIBUTE               0x8c
#define ZCL_STA_INVALID_DATA_TYPE                    0x8d
#define ZCL_STA_INVALID_SELECTOR                     0x8e
#define ZCL_STA_WRITE_ONLY                           0x8f
#define ZCL_STA_INCONSISTENT_STARTUP_STATE           0x90
#define ZCL_STA_DEFINED_OUT_OF_BAND                  0x91
#define ZCL_STA_INCONSISTENT                         0x92
#define ZCL_STA_ACTION_DENIED                        0x93
#define ZCL_STA_TIMEOUT                              0x94
#define ZCL_STA_ABORT                                0x95
#define ZCL_STA_INVALID_IMAGE                        0x96
#define ZCL_STA_WAIT_FOR_DATA                        0x97
#define ZCL_STA_NO_IMAGE_AVAILABLE                   0x98
#define ZCL_STA_REQUIRE_MORE_IMAGE                   0x99

// 0xbd-bf are reserved.
#define ZCL_STA_HARDWARE_FAILURE                     0xc0
#define ZCL_STA_SOFTWARE_FAILURE                     0xc1
#define ZCL_STA_CALIBRATION_ERROR                    0xc2
// 0xc3-0xff are reserved.
#define ZCL_STA_CMD_HAS_RESP                         0xFF // Non-standard status (used for Default Rspi)
/** @} end of group zcl_error_codes */



/** @} end of group ZCL_Constant */

/** @} end of group ZCL_Module */

/** @} end of group TELINK_ZIGBEE_STACK */
