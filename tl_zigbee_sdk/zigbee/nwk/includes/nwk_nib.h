/********************************************************************************************************
 * @file     nwk_nib.h
 *
 * @brief	 Network layer information base
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
#ifndef ZB_NWK_NIB_H
#define ZB_NWK_NIB_H 1

#include "tl_common.h"
#include "../../mac/includes/tl_zb_mac.h"


/**
 *  @brief  NWK: max network depth
 */
#define NWK_MAX_DEPTH                          	15


#define NWK_MAX_ROUTERS  						8


/**
 *  @brief  Default value of nib.max_children
            max number of children which can join to this device
 */
#define DEFAULT_MAX_CHILDREN                   	25
/***************************************************************************
* @brief	max number of routers per node
*/
#if !defined ( NWK_MAX_ROUTERS )
#define NWK_MAX_ROUTERS  8
#endif


/***************************************************************************
* @brief	max network depth
*/
#if !defined ( NWK_MAX_DEPTH )
#define NWK_MAX_DEPTH    5
#endif


#define ZB_NWK_LINK_STATUS_PEROID_DEFAULT              	15


/***************************************************************************
* @brief	NIB struct
*/
typedef struct
{
#ifdef ZB_ROUTER_ROLE
	u32		passiveAckTimeout;

	u8		linkStatusPeriod;
	u8		maxBroadcastRetries;
	u8		maxChildren;
	u8		maxRouters;

	u8		maxSourceRoute;
	u8		concentratorRadius;
	u8		concentratorDiscoveryTime;
	u8		routerAgeLimit;
#endif
	u32		nwkBroadcastDeliveryTime;

	extPANId_t extPANId;

	u16		panId;
	u16		nwkAddr;

	u16		managerAddr;
	u16		txTotal;

	u16		txFail;
	u16		transactionPersistenceTime;

	u8		seqNum;
	u8      depth;
	capability_info_t	capabilityInfo;
	u8		addrAlloc;

	u8		updateId;
	u8		stackProfile;
	u8		allFresh;
	u8		secAllFrames;

	u8		maxDepth;
	bool	reportConstantCost:1;
	bool	symLink:1;
	bool	useTreeRouting:1;
	bool	useMulticast:1;
	bool	isConcentrator:1;
	bool	uniqueAddr:1;
	bool	timeStamp:1;
	bool	leaveReqAllowed:1;
	u8		parentInfo;	//the behavior depends upon whether the device is an FFD or RFD.
	u8		endDevTimeoutDefault;

	addrExt_t	ieeeAddr;

	bool	leaveReqWithoutRejoinAllowed;
	u8		resv[3];
}nwk_nib_t; //60-bytes


#define NWK_NIB() 						g_zbNIB
#define NIB_NETWORK_ADDRESS() 			g_zbNIB.nwkAddr//g_zbMacPib.shortAddress
#define NIB_IEEE_ADDRESS()  			g_zbMacPib.extAddress
#define NIB_PAN_ID()					g_zbNIB.panId//g_zbMacPib.panId


#define NIB_GET_USE_MULTICAST()   		g_zbNIB.useMulticast
#define NIB_SET_USE_MULTICAST(v) 		(g_zbNIB.useMulticast = (v))




#endif /* ZB_NWK_MIB_H */
