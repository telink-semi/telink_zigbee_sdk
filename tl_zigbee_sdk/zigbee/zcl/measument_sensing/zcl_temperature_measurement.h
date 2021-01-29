/********************************************************************************************************
 * @file	zcl_temperature_measurement.h
 *
 * @brief	This is the header file for zcl_temperature_measurement
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
#ifndef ZCL_TEMPERATURE_MEASUREMENT_H
#define ZCL_TEMPERATURE_MEASUREMENT_H



/*********************************************************************
 * CONSTANTS
 */

/**
 *  @brief	temperature measurement cluster Attribute IDs
 */
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE     0x0000
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MINMEASUREDVALUE  0x0001
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MAXMEASUREDVALUE  0x0002
#define ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TOLERANCE         0x0003


#if 0
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MEASUREDVALUE_ENABLE
extern s16 zcl_attr_MeasuredValue;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MINMEASUREDVALUE_ENABLE
extern s16 zcl_attr_MinMeasuredValue;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MAXMEASUREDVALUE_ENABLE
extern s16 zcl_attr_MaxMeasuredValue;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_TOLERANCE_ENABLE
extern u16 zcl_attr_Tolerance;
#endif

extern const zclAttrInfo_t temperature_measurement_attrTbl[];
extern const u8 zcl_temperature_measurement_attrNum;
#endif


status_t zcl_temperature_measurement_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif	/* ZCL_TEMPERATURE_MEASUREMENT_H */
