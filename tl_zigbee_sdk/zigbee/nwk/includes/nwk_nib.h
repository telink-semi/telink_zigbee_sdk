/********************************************************************************************************
 * @file	nwk_nib.h
 *
 * @brief	This is the header file for nwk_nib
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
#ifndef NWK_NIB_H
#define NWK_NIB_H


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
#define NWK_MAX_ROUTERS  						8
#endif

/***************************************************************************
* @brief	max network depth
*/
#if !defined ( NWK_MAX_DEPTH )
#define NWK_MAX_DEPTH    						5
#endif

#define ZB_NWK_LINK_STATUS_PEROID_DEFAULT       15


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

#endif /* NWK_NIB_H */
