/********************************************************************************************************
 * @file    zcl_multistate_input.h
 *
 * @brief   This is the header file for zcl_multistate_input
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
#ifndef ZCL_MULTISTATE_INPUT_H
#define ZCL_MULTISTATE_INPUT_H


/*********************************************************************
 * CONSTANTS
 */

/**
 *  @brief	multistate input cluster Attribute IDs
 */
#define ZCL_MULTISTATE_INPUT_ATTRID_STATE_TEXT          0x000E
#define ZCL_MULTISTATE_INPUT_ATTRID_DESCRIPTION         0x001C
#define ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES       0x004A
#define ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE      0x0051
#define ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE       0x0055
#define ZCL_MULTISTATE_INPUT_ATTRID_RELIABLILITY        0x0067
#define ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS        0x006F
#define ZCL_MULTISTATE_INPUT_ATTRID_APP_TYPE            0x0100

/**
 *  @brief Max length of char string
 */
#define ZCL_MULTISTATE_INPUT_MAX_LENGTH                 24

#if 0
#ifdef ZCL_MULTISTATE_INPUT_ATTR_STATE_TEXT_ENABLE
extern u8 zcl_attr_mulInput_stateText[];
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_DESCRIPTION_ENABLE
extern u8 zcl_attr_mulInput_description[];
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_NUM_OF_STATES_ENABLE
extern u16 zcl_attr_mulInput_numOfStates;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_OUT_OF_SERVICE_ENABLE
extern bool zcl_attr_mulInput_outOfService;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_PRESENT_VALUE_ENABLE
extern u16 zcl_attr_mulInput_presentValue;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_RELIABLILITY_ENABLE
extern u8 zcl_attr_mulInput_reliability;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_STATUS_FLAGS_ENABLE
extern u8 zcl_attr_mulInput_statusFlags;
#endif
#ifdef ZCL_MULTISTATE_INPUT_ATTR_APP_TYPE_ENABLE
extern u32 zcl_attr_mulInput_appType;
#endif

extern const zclAttrInfo_t multistate_input_attrTbl[];
extern const u8 zcl_multistate_input_attrNum;
#endif


status_t zcl_multistate_input_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif	/* ZCL_MULTISTATE_INPUT_H */
