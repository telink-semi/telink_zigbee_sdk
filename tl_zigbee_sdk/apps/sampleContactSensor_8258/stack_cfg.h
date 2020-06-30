/********************************************************************************************************
 * @file     stack_config.h
 *
 * @brief    zigbee stack configuration
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

#pragma once



typedef enum{
	TL_IMAGETYPE_CONTACT_SENSOR		= 0xd3a5,
	TL_IMAGETYPE_LIGHT				= 0xd3a3,
	TL_IMAGETYPE_RC					= 0xd3a2,
	TL_IMAGETYPE_ONOFFSWITCH		= 0xd3a1,
}tl_imageType_e;



/**
 *  @brief  Working channel
 *          Valid value: 11 ~ 26
 */
#define DEFAULT_CHANNEL                           11

////
//typedef struct{
//	u8	stackBuildl;
//	u8	stackRelease;
//	u8	appBuild;
//	u8	appRelease;
//}tl_fileVersionDef_e;
#define	CURRENT_FILE_VERSION					  0x01000200//

/**
 *  @brief Telink Manufacturer Code
 */
#define TELINK_MANUFACTURER_CODE                  0x1141		//used in OTA

#define	IMAGE_TYPE								  TL_IMAGETYPE_CONTACT_SENSOR

/**
 *  @brief  Security
 */
#define SECURITY_ENABLE                           1


/**
 *  @brief  NVRAM
 */
#define NV_ENABLE                                 1


/**********************************************************************
 * Following parameter need user to adjust according the app requirement
 */
/**
 *  @brief  ZCL: MAX number of cluster list, in cluster number add  + out cluster number
 *
 */
#define	ZCL_CLUSTER_NUM_MAX						12

/**
 *  @brief  ZCL: maximum number for zcl reporting table
 *
 */
#define ZCL_REPORTING_TABLE_NUM					4

/**
 *  @brief  ZCL: maximum number for zcl scene table
 *
 */
#define	ZCL_SCENE_TABLE_NUM						8

/**
 *  @brief  APS: MAX number of groups size in the group table
 *          In each group entry, there is 8 endpoints existed.
 */
#define APS_GROUP_TABLE_NUM                   	8

/**
 *  @brief  APS: MAX number of binding table size
 */
#define APS_BINDING_TABLE_NUM                 	2

/**********************************************************************
 * Following parameters are for End Device only
 */

/**
 *  @brief  ZDO Indirect poll timer in millsecond
 */
#define ZB_ZDO_INDIRECT_POLL_TIMER              5000


/**********************************************************************
 * Following configuration will calculated automatically
 */

/**
   Auto definition for the role
 */
#if (COORDINATOR)
    #define ZB_ROUTER_ROLE                        1
    #define ZB_COORDINATOR_ROLE                   1
#elif (ROUTER)
    #define ZB_ROUTER_ROLE                        1
#elif (END_DEVICE)
    #define ZB_ED_ROLE                            1
#endif

#if ZB_ROUTER_ROLE
	#define GP_SUPPORT_ENABLE					  1
#endif

/***********************************************************************
 * If PM_ENABLE is set, the macro ZB_MAC_RX_ON_WHEN_IDLE must be ZERO.
 */
#if ZB_ED_ROLE
	#if PM_ENABLE
		#define ZB_MAC_RX_ON_WHEN_IDLE			  0
	#endif

	#ifndef ZB_MAC_RX_ON_WHEN_IDLE
		#define ZB_MAC_RX_ON_WHEN_IDLE			  0
	#endif
#endif


#if (SECURITY_ENABLE)
    #define ZB_SECURITY
#endif

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_UART)
#define ZBHCI_EN								  1
#endif

