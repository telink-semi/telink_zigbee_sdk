/********************************************************************************************************
 * @file     gpEpCfg.c
 *
 * @brief	 Coordinator and Router should support GPPB.
 *
 * @author
 * @date     Dec. 1, 2016
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

/**********************************************************************
 * INCLUDES
 */
#include "../zcl/zcl_include.h"
#include "gp.h"

#if GP_SUPPORT_ENABLE
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 gp_inClusterList[] =
{
	//ZCL_CLUSTER_GEN_GREEN_POWER
};


/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 gp_outClusterList[] =
{
	ZCL_CLUSTER_GEN_GREEN_POWER
};


/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define GP_IN_CLUSTER_NUM		0
#define GP_OUT_CLUSTER_NUM		1

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t gp_simpleDesc =
{
	GP_PROFILE_ID,                      	/* Application profile identifier */
	GP_DEVICE_ID_PROXY_BASIC,               /* Application device identifier */
	GREEN_POWER_ENDPOINT,              		/* Endpoint */
	0,                                  	/* Application device version */
	0,										/* Reserved */
	GP_IN_CLUSTER_NUM,           			/* Application input cluster count */
	GP_OUT_CLUSTER_NUM,          			/* Application output cluster count */
	(u16 *)gp_inClusterList,    			/* Application input cluster list */
	(u16 *)gp_outClusterList,   			/* Application output cluster list */
};



/* Attribute default */
u8 zclGpAttr_gppMaxProxyTabEntries = GPP_MAX_PROXY_TABLE_ENTRIES;
u16 zclGpAttr_clusterRev = GP_CLUSTER_REVISION;
u8	zclGpAttr_proxyTabEntry[ZCL_GP_MAX_PROXY_TABLE_ATTR_LEN + 2] = {0x0, 0x0,};
u8	zclGpAttr_gppFunc[3] = {0};
u8 	zclGpAttr_gppActiveFunc[3] = {0};
u8  zclGpAttr_gpSharedSecKeyType = 0;//GP_SEC_KEY_TYPE_NO_KEY;
u8  zclGpAttr_gpSharedSecKey[SEC_KEY_LEN] = {0};
u8  zclGpAttr_gpLinkKey[SEC_KEY_LEN] = GP_LINK_KEY;

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
	{ ZCL_ATTRID_GP_GPP_MAX_PROXY_TABLE_ENTRIES,  ZCL_DATA_TYPE_UINT8, 	  		 ACCESS_CONTROL_READ, 						 (u8*)&zclGpAttr_gppMaxProxyTabEntries },
	{ ZCL_ATTRID_GP_PROXY_TABLE,  				  ZCL_DATA_TYPE_LONG_OCTET_STR,  ACCESS_CONTROL_READ, 						 (u8*)&zclGpAttr_proxyTabEntry },
	{ ZCL_ATTRID_GP_GPP_FUNCTIONALITY,  		  ZCL_DATA_TYPE_BITMAP24, 	     ACCESS_CONTROL_READ, 						 (u8*)&zclGpAttr_gppFunc },
	{ ZCL_ATTRID_GP_GPP_ACTIVE_FUNCTIONALITY,  	  ZCL_DATA_TYPE_BITMAP24, 	     ACCESS_CONTROL_READ, 						 (u8*)&zclGpAttr_gppActiveFunc },
	/* Attributes shared by client and server of GP cluster */
	{ ZCL_ATTRID_GP_SHARED_SECURITY_KEY_TYPE, 	  ZCL_DATA_TYPE_BITMAP8, 	   	 ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpSharedSecKeyType },
	{ ZCL_ATTRID_GP_SHARED_SECURITY_KEY,  	      ZCL_DATA_TYPE_128_BIT_SEC_KEY, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpSharedSecKey },
	{ ZCL_ATTRID_GP_LINK_KEY,  				      ZCL_DATA_TYPE_128_BIT_SEC_KEY, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&zclGpAttr_gpLinkKey },
	{ ZCL_ATTRID_GP_CLUSTER_REVISION,  		      ZCL_DATA_TYPE_UINT16, 	  	 ACCESS_CONTROL_READ, 						 (u8*)&zclGpAttr_clusterRev },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	      ZCL_DATA_TYPE_UINT16,  		 ACCESS_CONTROL_READ,  						 (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_GP_ATTR_NUM		sizeof(gp_attrTbl) / sizeof(zclAttrInfo_t)


/**
 *  @brief Definition for Green Power ZCL specific cluster
 */
const zcl_specClusterInfo_t g_gpClusterList[] =
{
	{ZCL_CLUSTER_GEN_GREEN_POWER, ZCL_GP_ATTR_NUM,	gp_attrTbl, zcl_gp_register, zcl_gpCb},
};

u8 GP_CB_CLUSTER_NUM = (sizeof(g_gpClusterList)/sizeof(g_gpClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */



#endif	/* #if GP_SUPPORT_ENABLE */
