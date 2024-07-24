/********************************************************************************************************
 * @file    gp_proxy.h
 *
 * @brief   This is the header file for gp_proxy
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

#ifndef GP_PROXY_H
#define GP_PROXY_H

//Max proxy table entries.
#define GPP_MAX_PROXY_TABLE_ENTRIES		   			5

#define PROXY_LIGHTWEIGHT_SINK_ADDR_LIST_NUM		2
#define PROXY_SINK_GROUP_LIST_NUM					2
#define PROXY_FULL_UNICAST_SINK_ADDR_LIST_NUM		2

#define ZCL_GP_MAX_PROXY_TABLE_ATTR_LEN				75

/**
 *  @brief	Proxy entry options
 */
typedef union
{
	u16	opts;
	struct
	{
		u16	appId:3;
		u16	entryActive:1;
		u16	entryValid:1;
		u16	seqNumCap:1;
		u16	lightWeightUnicastGPS:1;
		u16	derivedGroupGPS:1;
		u16	commGroupGPS:1;
		u16	firstToForward:1;
		u16	inRange:1;
		u16	gpdFixed:1;
		u16	hasAllUnicastRoutes:1;
		u16	assignedAlias:1;
		u16	secUse:1;
		u16	optsExtension:1;
	}bits;
}gpProxyEntryOpt_t;

typedef struct
{
	addrExt_t sinkIeeeAddr;
	u16		  sinkNwkAddr;
}gpLwSinkAddrListItem_t;

typedef struct
{
	u16	fullUnicastGPS:1;
	u16	reserved:15;
}gpExtendedOpt_t;

/**
 *  @brief	Proxy table entry
 */
typedef struct _attribute_packed_{
	gpdId_t					gpdId;				//8-bytes
	gpProxyEntryOpt_t		options;
	u16						gpdAssignedAlias;
	gpSecOpt_t				secOptions;
	u8						endpoint;
	u8						groupcastRadius;
	u8						searchCnt;			//8-bytes
	u8						gpdKey[16];			//16-bytes
	u32						gpdSecFrameCnt;		//4-bytes
	gpLwSinkAddrListItem_t	lightweightSinkAddrList[PROXY_LIGHTWEIGHT_SINK_ADDR_LIST_NUM];	//20-bytes
	gpSinkGroupListItem_t	sinkGroupList[PROXY_SINK_GROUP_LIST_NUM];						//8-bytes
	u8						used:1;
	u8						lwSinkCnt:2;
	u8						sinkGroupCnt:2;
	u8						reserved:3;
}gpProxyTabEntry_t;	//65-bytes

/***************************************************************************
* @brief	Define for GP Proxy Table
*/
typedef struct _attribute_packed_{
	gpProxyTabEntry_t gpProxyTab[GPP_MAX_PROXY_TABLE_ENTRIES];
	u8 gpProxyTabNum;
}gp_proxyTab_t;

/***************************************************************************
* @brief	Define for GP
*/
typedef struct
{
	ev_timer_event_t *aliasConflictTimeoutEvt;
	ev_timer_event_t *commissioningWindowTimeoutEvt;
	u16 gpAliasConflictAddr;
	u16 gpCommissionerAddr; //addr to which send the notifications during commissioning mode
	u16 gpCommissioningWindow;//default value 180 seconds
	gpProxyCommModeOpt_t gpCommissioningModeOpt;//commissioning option copy from Commissioning Mode command
	u8	gppInCommMode;		//global flag, in commissioning mode or in operational mode
	bool firstToForward;
}gpp_ctx_t;


//global value
extern gp_proxyTab_t g_gpProxyTab;
extern gpp_ctx_t g_gppCtx;

gpSecRsp_status_t gpProxySecOperation(u8 appId, gpdId_t gpdId,
									  u8 endpoint, u32 gpdSecFrameCnt,
									  u8 gpdfSecurityLevel, u8 gpdfKeyType,
									  u8 *pKeyType, u8 *pKey);

void gppGpDataIndProcess(gp_data_ind_t *pGpDataInd);

void gpProxyTabInit(void);
void gpProxyTabUpdate(void);
void gp_proxyInit(void);
void gp_proxyTabEntryClear(gpProxyTabEntry_t *pEntry);
u8 gp_getProxyTabEntryTotalNum(void);
u8 gp_getProxyTabEntryLen(gpProxyTabEntry_t *pEntry);
gpProxyTabEntry_t *gp_proxyTabEntryFreeGet(void);
gpProxyTabEntry_t *gp_getProxyTabByGpdId(u8 appId, gpdId_t gpdId);
u8 lwSinkAddrListAdd(gpProxyTabEntry_t *pEntry, addrExt_t sinkIeeeAddr, u16 sinkNwkAddr);
u8 lwSinkAddrListRemove(gpProxyTabEntry_t *pEntry, addrExt_t sinkIeeeAddr, u16 sinkNwkAddr);
u8 sinkGroupListAdd(gpProxyTabEntry_t *pEntry, u16 sinkGroupID, u16 alias);
u8 sinkGroupListRemove(gpProxyTabEntry_t *pEntry, u16 sinkGroupID);
u8 gp_buildProxyTabEntryFormat(gpProxyTabEntry_t *pEntry, u8 *pBuf);

void gpProxyCommissioningModeCmdProcess(u16 srcAddr, zcl_gp_proxyCommissioningModeCmd_t *pCmd);
void gpResponseCmdProcess(zcl_gp_responseCmd_t *pCmd);

status_t zcl_gpServerCmdHandler(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

#endif	/* GP_PROXY_H */
