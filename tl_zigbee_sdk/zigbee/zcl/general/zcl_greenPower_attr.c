/********************************************************************************************************
 * @file     zcl_greenPower_attr.c
 *
 * @brief	 attribute configuration for green power cluster
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
