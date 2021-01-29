/********************************************************************************************************
 * @file	zcl_binary_output.h
 *
 * @brief	This is the header file for zcl_binary_output
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
#ifndef ZCL_BINARY_OUTPUT_H
#define ZCL_BINARY_OUTPUT_H



/*********************************************************************
 * CONSTANTS
 */

/**
 *  @brief	binary cluster Attribute IDs
 */
#define ZCL_BINARY_OUTPUT_ATTRID_ACTIVE_TEXT     			0x0004
#define ZCL_BINARY_OUTPUT_ATTRID_DESCRIPTION    			0x001C
#define ZCL_BINARY_OUTPUT_ATTRID_INACTIVE_TEXT   			0x002E
#define ZCL_BINARY_OUTPUT_ATTRID_MIN_OFF_TIME   			0x0042
#define ZCL_BINARY_OUTPUT_ATTRID_MIN_ON_TIME       			0x0043
#define ZCL_BINARY_OUTPUT_ATTRID_OUT_OF_SERVICE   			0x0051
#define ZCL_BINARY_OUTPUT_ATTRID_POLARITY    				0x0054
#define ZCL_BINARY_OUTPUT_ATTRID_PRESENT_VALUE      		0x0055
#define ZCL_BINARY_OUTPUT_ATTRID_PRIORITY_ARRAY   			0x0057
#define ZCL_BINARY_OUTPUT_ATTRID_RELIABLILITY         		0x0067
#define ZCL_BINARY_OUTPUT_ATTRID_RELINQUISH_DEFAULT     	0x0068
#define ZCL_BINARY_OUTPUT_ATTRID_STATUS_FLAGS           	0x006F
#define ZCL_BINARY_OUTPUT_ATTRID_APP_TYPE       			0x0100

/**
 *  @brief Max length of char string
 */
#define ZCL_BINARY_OUTPUT_MAX_LENGTH 						24


/**
 *  @brief  Definition format for priority array
 */
typedef struct {
	bool priority1;
	bool priority2;
} zcl_binOutput_priorityArray_t;

#if 0
#ifdef ZCL_BINARY_OUTPUT_ATTR_ACTIVE_TEXT_ENABLE
extern u8 zcl_attr_binOutput_activeText[];
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_DESCRIPTIONT_ENABLE
extern u8 zcl_attr_binOutput_description[];
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_INACTIVE_TEXT_ENABLE
extern u8 zcl_attr_binOutput_inactiveText[];
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_MIN_OFF_TIME_ENABLE
extern u32 zcl_attr_binOutput_minOffTime;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_MIN_ON_TIME_ENABLE
extern u32 zcl_attr_binOutput_minOnTime;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_OUT_OF_SERVICE_ENABLE
extern bool zcl_attrr_binOutput_outOfService;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_POLARITY_ENABLE
extern u8 zcl_attr_binOutput_polarity;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_PRESENT_VALUE_ENABLE
extern bool zcl_attr_binOutput_presentValue;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_PRIORITY_ARRAY_ENABLE
extern zcl_binOutput_priorityArray_t zcl_attr_binOutput_priorityArray[];
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_RELIABILITY_ENABLE
extern u8 zcl_attr_binOutput_reliability;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_RELINQUISH_DEFAULT_ENABLE
extern bool zcl_attr_binOutput_relinquishDefault;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_STATUS_FLAGS_ENABLE
extern u8 zcl_attr_binOutput_statusFlags;
#endif
#ifdef ZCL_BINARY_OUTPUT_ATTR_APP_TYPE_ENABLE
extern u32 zcl_attr_binOutput_appType;
#endif

extern const zclAttrInfo_t binary_output_attrTbl[];
extern const u8 zcl_binary_output_attrNum;
#endif


status_t zcl_binary_output_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif	/* ZCL_BINARY_OUTPUT_H */
