/********************************************************************************************************
 * @file	zcl_greenPower_attr.c
 *
 * @brief	This is the source file for zcl_greenPower_attr
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
#include "tl_common.h"
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
#ifdef ZCL_GREEN_POWER

/* Attribute default */
const u8 zclGpAttr_gppMaxProxyTabEntries = GPP_MAX_PROXY_TABLE_ENTRIES;
const u16 zclGpAttr_clusterRev = GP_CLUSTER_REVISION;
u8	zclGpAttr_proxyTabEntry[ZCL_GP_MAX_PROXY_TABLE_ATTR_LEN + 2] = {0x0, 0x0,};
#if 0
u8	zclGpAttr_gppNotificationRetryNum = GPP_NOTIFICATION_RETRY_NUM;
u8	zclGpAttr_gppNotificationTimer = GPP_NOTIFICATION_RETRY_TIMER;
u8	zclGpAttr_gppMaxSearchCnt = GPP_MAX_SEARCH_CNT;
u8	*pZclGpAttr_gppBlockedGPDID = NULL;
#endif
u8	zclGpAttr_gppFunc[3] = {0};
u8 	zclGpAttr_gppActiveFunc[3] = {0};

u8 zclGpAttr_gpSharedSecKeyType = 0;//GP_SEC_KEY_TYPE_NO_KEY;
u8 zclGpAttr_gpSharedSecKey[SEC_KEY_LEN] = {0};
u8 zclGpAttr_gpLinkKey[SEC_KEY_LEN] = GP_LINK_KEY;



/* Attribute record list */
const zclAttrInfo_t gp_attrTbl[] =
{
#if 0
	/* Attributes of the GP server cluster */
	{ ZCL_ATTRID_GP_GPS_MAX_SINK_TABLE_ENTRIES, ZCL_DATA_TYPE_UINT8, 		  ACCESS_CONTROL_READ, 						  (u8*)NULL },
	{ ZCL_ATTRID_GP_SINK_TABLE, 				ZCL_DATA_TYPE_LONG_OCTET_STR, ACCESS_CONTROL_READ, 						  (u8*)NULL },
	{ ZCL_ATTRID_GP_GPS_COMMUNICATION_MODE, 	ZCL_DATA_TYPE_BITMAP8, 		  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)NULL },
	{ ZCL_ATTRID_GP_GPS_COMMISSIONING_EXIT_MODE,ZCL_DATA_TYPE_BITMAP8, 		  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)NULL },
	{ ZCL_ATTRID_GP_GPS_COMMISSIONING_WINDOW, 	ZCL_DATA_TYPE_UINT16, 		  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)NULL },
	{ ZCL_ATTRID_GP_GPS_SECURITY_LEVEL,			ZCL_DATA_TYPE_BITMAP8, 		  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)NULL },
	{ ZCL_ATTRID_GP_GPS_FUNCTIONALITY, 			ZCL_DATA_TYPE_BITMAP24, 	  ACCESS_CONTROL_READ, 						  (u8*)NULL },
	{ ZCL_ATTRID_GP_GPS_ACTIVE_FUNCTIONALITY,  	ZCL_DATA_TYPE_BITMAP24, 	  ACCESS_CONTROL_READ, 						  (u8*)NULL },
#endif
	/* Attributes of the GP client cluster */
	{ ZCL_ATTRID_GP_GPP_MAX_PROXY_TABLE_ENTRIES,  ZCL_DATA_TYPE_UINT8, 	  		ACCESS_CONTROL_READ, 						(u8*)&zclGpAttr_gppMaxProxyTabEntries },
	{ ZCL_ATTRID_GP_PROXY_TABLE,  				  ZCL_DATA_TYPE_LONG_OCTET_STR, ACCESS_CONTROL_READ, 						(u8*)&zclGpAttr_proxyTabEntry },
#if 0
	{ ZCL_ATTRID_GP_GPP_NOTIFICATION_RETRY_NUMBER,ZCL_DATA_TYPE_UINT8, 			ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gppNotificationRetryNum },
	{ ZCL_ATTRID_GP_GPP_NOTIFICATION_RETRY_TIMER, ZCL_DATA_TYPE_UINT8, 	  		ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gppNotificationTimer },
	{ ZCL_ATTRID_GP_GPP_MAX_SEARCH_COUNTER,  	  ZCL_DATA_TYPE_UINT8, 	  		ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gppMaxSearchCnt },
	{ ZCL_ATTRID_GP_GPP_BLOCKED_GPD_ID,  		  ZCL_DATA_TYPE_LONG_OCTET_STR, ACCESS_CONTROL_READ, 						(u8*)&pZclGpAttr_gppBlockedGPDID },
#endif
	{ ZCL_ATTRID_GP_GPP_FUNCTIONALITY,  		  ZCL_DATA_TYPE_BITMAP24, 	    ACCESS_CONTROL_READ, 						(u8*)&zclGpAttr_gppFunc },
	{ ZCL_ATTRID_GP_GPP_ACTIVE_FUNCTIONALITY,  	  ZCL_DATA_TYPE_BITMAP24, 	    ACCESS_CONTROL_READ, 						(u8*)&zclGpAttr_gppActiveFunc },
	/* Attributes shared by client and server of GP cluster */
	{ ZCL_ATTRID_GP_SHARED_SECURITY_KEY_TYPE, ZCL_DATA_TYPE_BITMAP8, 	   	 ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpSharedSecKeyType },
	{ ZCL_ATTRID_GP_SHARED_SECURITY_KEY,  	  ZCL_DATA_TYPE_128_BIT_SEC_KEY, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpSharedSecKey },
	{ ZCL_ATTRID_GP_LINK_KEY,  				  ZCL_DATA_TYPE_128_BIT_SEC_KEY, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpLinkKey },
	{ ZCL_ATTRID_GP_CLUSTER_REVISION,  		  ZCL_DATA_TYPE_UINT16, 	  	 ACCESS_CONTROL_READ, 						 (u8*)&zclGpAttr_clusterRev },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

const u8 zcl_gp_attrNum = ( sizeof(gp_attrTbl) / sizeof(zclAttrInfo_t) );

#endif    /* ZCL_GREEN_POWER */
#endif
