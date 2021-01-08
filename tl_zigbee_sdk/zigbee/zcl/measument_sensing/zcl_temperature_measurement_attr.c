/********************************************************************************************************
 * @file	zcl_temperature_measurement_attr.c
 *
 * @brief	This is the source file for zcl_temperature_measurement_attr
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

/**********************************************************************
 * INCLUDES
 */
#include "../zcl_include.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_TEMPERATURE_MEASUREMENT

#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MEASUREDVALUE_ENABLE
s16 zcl_attr_MeasuredValue = 0xFFFF;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MINMEASUREDVALUE_ENABLE
s16 zcl_attr_MinMeasuredValue = 0x8000;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MAXMEASUREDVALUE_ENABLE
s16 zcl_attr_MaxMeasuredValue = 0x8000;
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_TOLERANCE_ENABLE
u16 zcl_attr_Tolerance = 0;
#endif

const zclAttrInfo_t temperature_measurement_attrTbl[] =
{
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MEASUREDVALUE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MEASUREDVALUE,       ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&zcl_attr_MeasuredValue},
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MINMEASUREDVALUE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MINMEASUREDVALUE,    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ, (u8*)&zcl_attr_MinMeasuredValue},
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_MAXMEASUREDVALUE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_MAXMEASUREDVALUE,    ZCL_DATA_TYPE_INT16,    ACCESS_CONTROL_READ, (u8*)&zcl_attr_MaxMeasuredValue},
#endif
#ifdef ZCL_TEMPERATURE_MEASUREMENT_ATTR_TOLERANCE_ENABLE
	{ ZCL_TEMPERATURE_MEASUREMENT_ATTRID_TOLERANCE,           ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ, (u8*)&zcl_attr_Tolerance},
#endif
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 					  ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_temperature_measurement_attrNum = ( sizeof(temperature_measurement_attrTbl) / sizeof(zclAttrInfo_t) );

#endif	/* ZCL_TEMPERATURE_MEASUREMENT */

#endif
