/********************************************************************************************************
 * @file     bdb.c
 *
 * @brief	 base device behavior
 *
 * @author
 * @date     May. 27, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "../zcl/zll_commissioning/zcl_zll_commissioning_internal.h"


/**********************************************************************
 * GLOBAL VARIABLES
 */
bdb_ctx_t g_bdbCtx = {0};

/**********************************************************************
 * MACRO DEFINE
 */
#define	BDB_COMMISSION_TIME_INTV				5   //second

#define	BDB_TOUCHLINK_CAP_EN()		(g_bdbAttrs.nodeCommissioningCapability & BDB_NODE_COMMISSION_CAP_TOUCHLINK)

/**********************************************************************
 * LOCAL FUNCTIONS
 */
static void bdb_task(void *arg);
static void bdb_commissioningInfoSave(void *arg);
static void bdb_binding(zdo_bind_dstAddr_t *pDstAddr, u8 clusterNum, u16 *clusterList);
static void bdb_findingAndBinding(findBindDst_t *pDstInfo);
static void bdb_simpleDescReqSend(void *arg);
static u8 bdb_commissioningFindBind(void);
static u8 bdb_commissioningNetworkFormation(void);
static u8 bdb_commissioningNetworkSteer(void);
static u8 bdb_topLevelCommissioning(u8 role);
static u8 bdb_topLevelCommissiongConfirm(void);
static void bdb_retrieveTcLinkKeyTimerStop(void);
static void bdb_touchLinkCallback(u8 status, void *arg);
static void bdb_commssionUtilityCallback(u8 cmd, void *arg);

extern void bdb_linkKeyCfg( bdb_commissionSetting_t *setting, u8 fn);
extern void bdb_touchLinkPreCfg(u8 endpoint, bdb_commissionSetting_t *setting, const zcl_touchlinkAppCallbacks_t *tlCb);
extern void bdb_coordinatorStart(void);
extern void bdb_routerStart(void);
extern void bdb_endDeviceStart(u8 repower);
extern void bdb_outgoingFrameCountUpdate(u8 repower);
extern void bdb_scanCfg(u32 chanMask, u8 duration);
extern void bdb_factoryNewDevCfg(u8 touchLinkEn, u8 chan);

/**********************************************************************
 * LOCAL CONSTANTS
 */
const bdb_attrTbl_t g_bdbAttrTbl[] = {
	{OFFSETOF(bdb_attr_t, primaryChannelSet), 			4},
	{OFFSETOF(bdb_attr_t, secondaryChannelSet), 		4},
	{OFFSETOF(bdb_attr_t, commissioningGroupId), 		2},
	{OFFSETOF(bdb_attr_t, commissioningMode), 			1},
	{OFFSETOF(bdb_attr_t, commissioningStatus), 		1},
	{OFFSETOF(bdb_attr_t, joiningNodeEui64), 			8},
	{OFFSETOF(bdb_attr_t, joiningNodeNewTCLinkKey), 	16},
	{OFFSETOF(bdb_attr_t, nodeCommissioningCapability), 1},
	{OFFSETOF(bdb_attr_t, joinUsesInstallCodeKey), 		1},
	{OFFSETOF(bdb_attr_t, nodeIsOnANetwork), 			1},
	{OFFSETOF(bdb_attr_t, nodeJoinLinkKeyType), 		1},
	{OFFSETOF(bdb_attr_t, tcRequireKeyExchange), 		1},
	{OFFSETOF(bdb_attr_t, scanDuration), 				1},
	{OFFSETOF(bdb_attr_t, tcLinkKeyExchangeAttempts), 	1},
	{OFFSETOF(bdb_attr_t, tcLinkKeyExchangeAttemptsMax),1},
	{OFFSETOF(bdb_attr_t, tcLinkKeyExchangeMethod), 	1},
	{OFFSETOF(bdb_attr_t, tcNodeJoinTimeout), 			1},
};

const bdb_attr_t g_bdbAttrsDft = {
		.primaryChannelSet = BDBC_TL_PRIMARY_CHANNEL_SET,
		.secondaryChannelSet = BDBC_TL_SECONDARY_CHANNEL_SET,
		.commissioningGroupId = 0xffff,
		.commissioningStatus = BDB_COMMISSION_STA_SUCCESS,
		.commissioningMode.networkSteer = 1,
		.nodeJoinLinkKeyType = LINKKEY_TYPE_TC,
		.tcRequireKeyExchange = 1,		//tcRequireKeyExchange: true
		.scanDuration = 3,
		.tcLinkKeyExchangeAttemptsMax = BDBC_REC_SAME_NETWROK_RETRY_ATTEMPTS,
		.tcLinkKeyExchangeMethod = TCKEY_EXCHANGE_METHOD_APSRK,
		.tcNodeJoinTimeout = 0x0f, //seconds
};

const zcl_touchlinkAppCallbacks_t bdb_touchlinkCb = {bdb_touchLinkCallback, bdb_commssionUtilityCallback};


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      bdb_commissioningInfoSave
 *
 * @brief
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_commissioningInfoSave(void *arg)
{
#if NV_ENABLE
	nv_nwkFrameCountSaveToFlash(ss_ib.outgoingFrameCounter);
	zdo_ssInfoSaveToFlash();
	zb_info_save(NULL);

//#ifdef ZB_ED_ROLE
    /* store neighbor table */
    nwk_parentNodeInfoStore();
//#endif

	g_bdbCtx.factoryNew = 0;
	g_bdbAttrs.commissioningMode.networkSteer = 1;
	//tl_bdbAttrSave2Flash();
#endif
}


/*********************************************************************
 * @fn      queueInit
 *
 * @brief
 *
 * @param   pFindBindQ
 *
 * @return  TRUE / FALSE
 */
_CODE_BDB_ static findBindQ_t *queueInit(void)
{
	findBindQ_t *pFindBindQ = (findBindQ_t *)ev_buf_allocate(sizeof(findBindQ_t));

	if(pFindBindQ){
		pFindBindQ->front = 0;
		pFindBindQ->rear = 0;
	}

	return pFindBindQ;
}

static void queueClear(findBindQ_t *pFindBindQ)
{
	if(pFindBindQ){
		ev_buf_free((u8 *)pFindBindQ);
		g_bdbCtx.pFindBindQ = NULL;
	}
}

/*********************************************************************
 * @fn      isQueueEmpty
 *
 * @brief
 *
 * @param   pFindBindQ
 *
 * @return  TRUE / FALSE
 */
_CODE_BDB_ static bool isQueueEmpty(findBindQ_t *pFindBindQ)
{
	if(!pFindBindQ){
		return TRUE;
	}

	if(pFindBindQ->front == pFindBindQ->rear){
		return TRUE;
	}else{
		return FALSE;
	}
}

/*********************************************************************
 * @fn      isQueueFull
 *
 * @brief
 *
 * @param   pFindBindQ
 *
 * @return  TRUE / FALSE
 */
_CODE_BDB_ static bool isQueueFull(findBindQ_t *pFindBindQ)
{
	if(((pFindBindQ->rear + 1) % FIND_BIND_QUEUE_SIZE) == pFindBindQ->front){
		return TRUE;
	}else{
		return FALSE;
	}
}

/*********************************************************************
 * @fn      queuePush
 *
 * @brief
 *
 * @param   pFindBindQ
 * 			dstInfo
 *
 * @return  TRUE / FALSE
 */
_CODE_BDB_ static bool queuePush(findBindQ_t *pFindBindQ, findBindDst_t dstInfo)
{
	if(isQueueFull(pFindBindQ)){
		return FALSE;
	}

	pFindBindQ->rear = (pFindBindQ->rear + 1) % FIND_BIND_QUEUE_SIZE;
	pFindBindQ->findBindDstInfo[pFindBindQ->rear].addr = dstInfo.addr;
	pFindBindQ->findBindDstInfo[pFindBindQ->rear].endpoint = dstInfo.endpoint;

	return TRUE;
}

/*********************************************************************
 * @fn      queuePop
 *
 * @brief
 *
 * @param   pFindBindQ
 * 			dstInfo
 *
 * @return  TRUE / FALSE
 */
_CODE_BDB_ static bool queuePop(findBindQ_t *pFindBindQ, findBindDst_t *pDstInfo)
{
	if(isQueueEmpty(pFindBindQ)){
		return FALSE;
	}

	pFindBindQ->front = (pFindBindQ->front + 1) % FIND_BIND_QUEUE_SIZE;
	pDstInfo->addr = pFindBindQ->findBindDstInfo[pFindBindQ->front].addr;
	pDstInfo->endpoint = pFindBindQ->findBindDstInfo[pFindBindQ->front].endpoint;

	return TRUE;
}

#if 0
/*********************************************************************
 * @fn      queueHeadGet
 *
 * @brief
 *
 * @param   pFindBindQ
 * 			pDstInfo
 *
 * @return  TRUE / FALSE
 */
_CODE_BDB_ static bool queueHeadGet(findBindQ_t *pFindBindQ, findBindDst_t *pDstInfo)
{
	if(isQueueEmpty(pFindBindQ)){
		return FALSE;
	}

	pDstInfo->addr = pFindBindQ->findBindDstInfo[(pFindBindQ->front + 1) % FIND_BIND_QUEUE_SIZE].addr;
	pDstInfo->endpoint = pFindBindQ->findBindDstInfo[(pFindBindQ->front + 1) % FIND_BIND_QUEUE_SIZE].endpoint;

	return TRUE;
}
#endif

static void bdb_findBindRst(void)
{
	queueClear(g_bdbCtx.pFindBindQ);
	memset((u8 *)&g_bdbCtx.findDstInfo, 0, sizeof(findBindDst_t));
	memset((u8 *)&g_bdbCtx.bindDstInfo, 0, sizeof(findBindDst_t));
}

/*********************************************************************
 * @fn      bdb_binding
 *
 * @brief   for initiator
 *
 * @param   pDstAddr
 * 			clusterNum
 * 			clusterList
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_binding(zdo_bind_dstAddr_t *pDstAddr, u8 clusterNum, u16 *clusterList)
{
	zdo_bind_req_t req;
	memset(&req, 0, sizeof(zdo_bind_req_t));

	ZB_IEEE_ADDR_COPY(req.src_addr, NIB_IEEE_ADDRESS());
	req.src_endpoint = g_bdbCtx.simpleDesc->endpoint;
	req.dst_addr_mode = pDstAddr->dstAddrMode;
	if(req.dst_addr_mode == APS_LONG_DSTADDR_WITHEP){
		ZB_IEEE_ADDR_COPY(req.dst_ext_addr, pDstAddr->dstAddr.dstExtAddr);
		req.dst_endpoint = pDstAddr->dstAddr.dstEp;
	}else if(req.dst_addr_mode == APS_SHORT_GROUPADDR_NOEP){
		req.dst_group_addr = pDstAddr->dstAddr.dstGroupId;
	}

	for(u8 i = 0; i < clusterNum; i++){
		req.cid16_l = clusterList[i];
		req.cid16_h = clusterList[i]>>8;

		if((aps_me_find_dst_ref((aps_me_bind_req_t *)&req) == TL_RETURN_INVALID) && (aps_me_free_src_table_find() == TL_RETURN_INVALID)){
			//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_BINDING_TABLE_FULL;
			BDB_STATUS_SET(BDB_COMMISSION_STA_BINDING_TABLE_FULL);
			u32 evt = BDB_EVT_COMMISSIONING_FINDORBIND_FINISH;
			TL_SCHEDULE_TASK(bdb_task, (void *)evt);
			break;
		}else{
			u8 sn = 0;
			zb_zdoBindUnbindReq(TRUE, &req, &sn, NULL);
		}
	}

	ev_buf_free((u8 *)g_bdbCtx.matchClusterList);
	g_bdbCtx.matchClusterNum = 0;
	g_bdbCtx.matchClusterList = NULL;

	if(BDB_STATUS_GET() != BDB_COMMISSION_STA_BINDING_TABLE_FULL){
		if(g_bdbCtx.bdbAppCb && g_bdbCtx.bdbAppCb->bdbFindBindSuccessCb){
			g_bdbCtx.bdbAppCb->bdbFindBindSuccessCb(&g_bdbCtx.bindDstInfo);
		}

		TL_SCHEDULE_TASK(bdb_simpleDescReqSend, NULL);
	}
}

/*********************************************************************
 * @fn      bdb_ieeeAddrResp
 *
 * @brief   for initiator
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_ieeeAddrResp(void *arg)
{
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_ieee_addr_resp_t *resp = (zdo_ieee_addr_resp_t *)p->zpdu;

	if(resp->status == ZDO_SUCCESS){
		zdo_bind_dstAddr_t bindDstAddr;
		memset((u8 *)&bindDstAddr, 0, sizeof(zdo_bind_dstAddr_t));

		bindDstAddr.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
		bindDstAddr.dstAddr.dstEp = g_bdbCtx.bindDstInfo.endpoint;
		ZB_IEEE_ADDR_COPY(bindDstAddr.dstAddr.dstExtAddr, resp->ieee_addr_remote);

		bdb_binding(&bindDstAddr, g_bdbCtx.matchClusterNum, g_bdbCtx.matchClusterList);
	}
}

/*********************************************************************
 * @fn      bdb_findingAndBinding
 *
 * @brief   for initiator
 *
 * @param   pDstInfo
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_findingAndBinding(findBindDst_t *pDstInfo)
{
	zdo_bind_dstAddr_t bindDstAddr;
	memset((u8 *)&bindDstAddr, 0, sizeof(zdo_bind_dstAddr_t));

	if(g_bdbAttrs.commissioningGroupId != 0xFFFF){
		epInfo_t dstEpInfo;
		TL_SETSTRUCTCONTENT(dstEpInfo, 0);

		dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEpInfo.dstEp = pDstInfo->endpoint;
		dstEpInfo.dstAddr.shortAddr = pDstInfo->addr;
		dstEpInfo.profileId = HA_PROFILE_ID;
		dstEpInfo.txOptions = 0;//APS_TX_OPT_ACK_TX;
		dstEpInfo.radius = 0;

		u8 groupName[8] = {0};
		zcl_group_addCmd(g_bdbCtx.simpleDesc->endpoint, &dstEpInfo, FALSE, g_bdbAttrs.commissioningGroupId, groupName);

		bindDstAddr.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
		bindDstAddr.dstAddr.dstGroupId = g_bdbAttrs.commissioningGroupId;

		bdb_binding(&bindDstAddr, g_bdbCtx.matchClusterNum, g_bdbCtx.matchClusterList);
	}else{
		addrExt_t dstExtAddr;
		ZB_IEEE_ADDR_ZERO(dstExtAddr);

		u8 extAddrGot = zb_address_ieee_by_short(pDstInfo->addr, dstExtAddr);

		if((extAddrGot != SUCCESS) || ((extAddrGot == SUCCESS) && ZB_IEEE_ADDR_IS_ZERO(dstExtAddr))){
			zdo_ieee_addr_req_t ieeeAddrReq;
			u8 sn = 0;
			memset(&ieeeAddrReq, 0, sizeof(zdo_ieee_addr_req_t));

			ieeeAddrReq.nwk_addr_interest = pDstInfo->addr;
			ieeeAddrReq.req_type = ZDO_ADDR_REQ_SINGLE_RESP;

			zb_zdoIeeeAddrReq(pDstInfo->addr, &ieeeAddrReq, &sn, bdb_ieeeAddrResp);
		}else{
			bindDstAddr.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
			bindDstAddr.dstAddr.dstEp = pDstInfo->endpoint;
			ZB_IEEE_ADDR_COPY(bindDstAddr.dstAddr.dstExtAddr, dstExtAddr);

			bdb_binding(&bindDstAddr, g_bdbCtx.matchClusterNum, g_bdbCtx.matchClusterList);
		}
	}
}

/*********************************************************************
 * @fn      bdb_SimpleDescResp
 *
 * @brief   for initiator
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_SimpleDescResp(void *arg)
{
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_simple_descriptor_resp_t *simpDescResp = (zdo_simple_descriptor_resp_t *)p->zpdu;

	if(simpDescResp->status == SUCCESS){
		if(g_bdbCtx.matchClusterList || g_bdbCtx.matchClusterNum){
			return;
		}

		if(g_bdbCtx.identifyTimer){
			TL_ZB_TIMER_CANCEL(&g_bdbCtx.identifyTimer);
		}

		u8 *pClusterListInfo = simpDescResp->simple_descriptor.listInfo;
		u8 inClusterNum = *pClusterListInfo++;

		if(inClusterNum){
			g_bdbCtx.matchClusterList = (u16 *)ev_buf_allocate(sizeof(u16) * inClusterNum);
			if(g_bdbCtx.matchClusterList){
				for(u8 i = 0; i < g_bdbCtx.clusterNum; i++){
					u8 *pClusterListInfo_temp = pClusterListInfo;
					for(u8 j = 0; j < inClusterNum; j++){
						u16 clusterId = BUILD_U16(pClusterListInfo_temp[0], pClusterListInfo_temp[1]);
						if(g_bdbCtx.clusterList[i] == clusterId){
							g_bdbCtx.matchClusterList[g_bdbCtx.matchClusterNum++] = clusterId;
						}
						pClusterListInfo_temp += 2;
					}
				}

				if(g_bdbCtx.matchClusterNum){
					g_bdbCtx.bindDstInfo.addr = simpDescResp->nwk_addr_interest;
					g_bdbCtx.bindDstInfo.endpoint = simpDescResp->simple_descriptor.endpoint;

					bdb_findingAndBinding(&g_bdbCtx.bindDstInfo);
					return;
				}
			}
		}
	}

	TL_SCHEDULE_TASK(bdb_simpleDescReqSend, NULL);
}

/*********************************************************************
 * @fn      bdb_commissioningFindBindSimpleDescReq
 *
 * @brief   for initiator
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_commissioningFindBindSimpleDescReq(void)
{
	if(BDB_STATE_GET() == BDB_STATE_COMMISSIONING_FINDORBIND && g_bdbCtx.role == BDB_COMMISSIONING_ROLE_INITIATOR){
		zdo_simple_descriptor_req_t req;
		u8 sn = 0;
		memset(&req, 0, sizeof(zdo_simple_descriptor_req_t));

		req.endpoint = g_bdbCtx.findDstInfo.endpoint;
		req.nwk_addr_interest = g_bdbCtx.findDstInfo.addr;

		zb_zdoSimpleDescReq(g_bdbCtx.findDstInfo.addr, &req, &sn, bdb_SimpleDescResp);
	}
}

/*********************************************************************
 * @fn      bdb_simpleDescReqTimeoutCb
 *
 * @brief   for initiator
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static s32 bdb_simpleDescReqTimeoutCb(void *arg)
{
	TL_SCHEDULE_TASK(bdb_simpleDescReqSend, NULL);

	g_bdbCtx.identifyTimer = NULL;

	return -1;
}

/*********************************************************************
 * @fn      bdb_simpleDescReqSend
 *
 * @brief   for initiator
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static void bdb_simpleDescReqSend(void *arg)
{
	if(queuePop(g_bdbCtx.pFindBindQ, &g_bdbCtx.findDstInfo)){
		if(g_bdbCtx.identifyTimer){
			TL_ZB_TIMER_CANCEL(&g_bdbCtx.identifyTimer);
		}

		u32 evt = BDB_EVT_COMMISSIONING_FINDORBIND_SIMPLE_DESC_REQ;
		TL_SCHEDULE_TASK(bdb_task, (void *)evt);

#ifdef ZB_ED_ROLE
		u32 simpleDescReqTimeout = zb_getPollRate() * 3;
#else
		u32 simpleDescReqTimeout = 5000;
#endif

		g_bdbCtx.identifyTimer = TL_ZB_TIMER_SCHEDULE(bdb_simpleDescReqTimeoutCb, NULL, simpleDescReqTimeout * 1000);
	}else{
		//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS;
		BDB_STATUS_SET(BDB_COMMISSION_STA_SUCCESS);
		u32 evt = BDB_EVT_COMMISSIONING_FINDORBIND_FINISH;
		TL_SCHEDULE_TASK(bdb_task, (void *)evt);
	}
}

/*********************************************************************
 * @fn      bdb_findBindIdentifyQueryTimeout
 *
 * @brief   the callback of the Find and Bind Identify query response received timeout for initiator.
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static s32 bdb_findBindIdentifyQueryTimeout(void *arg)
{
	if(isQueueEmpty(g_bdbCtx.pFindBindQ)){
		//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE;

		BDB_STATUS_SET(BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE);
		u32 evt = BDB_EVT_COMMISSIONING_FINDORBIND_FINISH;
		TL_SCHEDULE_TASK(bdb_task, (void *)evt);
	}else{
		TL_SCHEDULE_TASK(bdb_simpleDescReqSend, NULL);
	}

	g_bdbCtx.identifyTimer = NULL;

	return -1;
}

/*********************************************************************
 * @fn      bdb_findBindIdentifyTimeout
 *
 * @brief   the callback of the Find and Bind Identify timeout for target
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static s32 bdb_findBindIdentifyTimeout(void *arg)
{
	static u32 timerIdentifyCnt = 0;

	u16 identifyTime = 0;
	u16 attrLen = 0;

	zcl_getAttrVal(g_bdbCtx.simpleDesc->endpoint, ZCL_CLUSTER_GEN_IDENTIFY, ZCL_ATTRID_IDENTIFY_TIME, &attrLen, (u8 *)&identifyTime);

	if(identifyTime == 0 || timerIdentifyCnt++ >= BDBC_MIN_COMMISSIONING_TIME / BDB_COMMISSION_TIME_INTV){
		timerIdentifyCnt = 0;

		//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS;
		BDB_STATUS_SET(BDB_COMMISSION_STA_SUCCESS);
		u32 evt = BDB_EVT_COMMISSIONING_FINDORBIND_FINISH;
		TL_SCHEDULE_TASK(bdb_task, (void *)evt);

		g_bdbCtx.identifyTimer = NULL;
		return -1;
	}
	return 0;
}

/*********************************************************************
 * @fn      bdb_commissioningFindBind
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ static u8 bdb_commissioningFindBind(void)
{
	if(g_bdbAttrs.commissioningMode.findOrBind){
		if(g_bdbAttrs.nodeIsOnANetwork){
			/* Perform finding & binding according to cluster class */
			if(g_bdbCtx.role == BDB_COMMISSIONING_ROLE_TARGET){
				/*
				 * Set IdentifyTime to ¡Ý bdbMinCommissioningTime
				 *
				 * */
				/* Handle identify query requests from the initiator until Identify timer expired*/
				if(!g_bdbCtx.identifyTimer){
					g_bdbCtx.identifyTimer = TL_ZB_TIMER_SCHEDULE(bdb_findBindIdentifyTimeout, NULL, BDB_COMMISSION_TIME_INTV * 1000 * 1000);
				}

				if(g_bdbCtx.bdbAppCb->bdbIdentifyCb){
					g_bdbCtx.bdbAppCb->bdbIdentifyCb(g_bdbCtx.simpleDesc->endpoint, NIB_NETWORK_ADDRESS(), BDBC_MIN_COMMISSIONING_TIME);
				}
			}else{
				epInfo_t dstEpInfo;
				TL_SETSTRUCTCONTENT(dstEpInfo, 0);

				dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
				dstEpInfo.dstAddr.shortAddr = 0xffff;
				dstEpInfo.dstEp = 0xff;
				dstEpInfo.profileId = HA_PROFILE_ID;

				zcl_identify_identifyQueryCmd(g_bdbCtx.simpleDesc->endpoint, &dstEpInfo, TRUE);

#ifdef ZB_ED_ROLE
				u32 identifyQueryRcvTimeout = zb_getPollRate() * 3;;
#else
				u32 identifyQueryRcvTimeout = 5000;
#endif

				/* Start a timer to check if identify query response are received. */
				if(!g_bdbCtx.identifyTimer){
					g_bdbCtx.identifyTimer = TL_ZB_TIMER_SCHEDULE(bdb_findBindIdentifyQueryTimeout, NULL, identifyQueryRcvTimeout * 1000);
				}
			}
			return BDB_STATE_COMMISSIONING_FINDORBIND;
		}
	}
	return BDB_STATE_IDLE;
}

/*********************************************************************
 * @fn      bdb_commissioningNetworkFormation
 *
 * @brief   start Network formation during commissioning
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ static u8 bdb_commissioningNetworkFormation(void)
{
	u8 bdbFormation = BDB_STATE_IDLE;
	if(!g_bdbAttrs.commissioningMode.networkFormation){
		return bdb_commissioningFindBind();
	}else{
		if(g_bdbAttrs.nodeIsOnANetwork){
			return bdb_commissioningFindBind();
		}else{
#if ZB_ROUTER_ROLE
			u32 scanChannels = aps_ib.aps_channel_mask;

			/* network formation */
			zb_nwkFormation(scanChannels, g_bdbAttrs.scanDuration);

			bdbFormation = 1;
			return BDB_STATE_COMMISSIONING_NETWORK_FORMATION;
#endif
		}
	}

	return bdbFormation;
}

/*********************************************************************
 * @fn      bdb_mgmtPermitJoiningConfirm
 *
 * @brief   Mgmt_PermitJoining confirm handle
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_mgmtPermitJoiningConfirm(void *arg)
{
#if ZB_ROUTER_ROLE
	/* Enable permit join for ¡ÝbdbcMinCommissioningTime	seconds */
#if ZB_COORDINATOR_ROLE
	zb_nlmePermitJoiningRequest(BDBC_MIN_COMMISSIONING_TIME);
#else
	zb_nlmePermitJoiningRequest(BDBC_MIN_COMMISSIONING_TIME);
#endif
#endif

	//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS;
	BDB_STATUS_SET(BDB_COMMISSION_STA_SUCCESS);
	if(BDB_STATE_GET() == BDB_STATE_COMMISSIONING_NETWORK_STEER){
		u32 evt = BDB_EVT_COMMISSIONING_NETWORK_STEER_FINISH;
		TL_SCHEDULE_TASK(bdb_task, (void *)evt);
	}else if(BDB_STATE_GET() == BDB_STATE_COMMISSIONING_NETWORK_FORMATION){
		u32 evt = BDB_EVT_COMMISSIONING_NETWORK_FORMATION_FINISH;
		TL_SCHEDULE_TASK(bdb_task, (void *)evt);
	}

	if(g_bdbAttrs.nodeIsOnANetwork){
		bdb_commissioningInfoSave(NULL);
	}
}

/*********************************************************************
 * @fn      bdb_mgmtPermitJoiningTrig
 *
 * @brief   send bdb_mgmtPermitJoining
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_mgmtPermitJoiningTrig(void *arg)
{
	u8 sn = 0;
	zb_mgmtPermitJoinReqTx(0xffff, BDBC_MIN_COMMISSIONING_TIME, 0x01, &sn, NULL);
	TL_SCHEDULE_TASK(bdb_mgmtPermitJoiningConfirm, NULL);
}

/*********************************************************************
 * @fn      bdb_retrieveTcLinkKeyTimerStop
 *
 * @brief   stop the procedure of retrieving trust center link key
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_retrieveTcLinkKeyTimerStop(void)
{
	if(g_bdbCtx.retrieveTcLkKeyTimer){
		TL_ZB_TIMER_CANCEL(&g_bdbCtx.retrieveTcLkKeyTimer);
	}
}

/*********************************************************************
 * @fn      bdb_retrieveTcLinkKeyDone
 *
 * @brief   retrieving Trust center link Key is done
 *
 * @param   status
 *
 * @return  None
 */
_CODE_BDB_ void bdb_retrieveTcLinkKeyDone(u8 status)
{
	/* now send permit join command */
	u32 evt = BDB_EVT_IDLE;
	bdb_retrieveTcLinkKeyTimerStop();
	if(status == BDB_COMMISSION_STA_SUCCESS){
		g_bdbAttrs.nodeIsOnANetwork = 1;
		g_zbNwkCtx.joinAccept = 1;
		evt = BDB_EVT_COMMISSIONING_NETWORK_STEER_PERMITJOIN;
	}else{
		g_bdbAttrs.nodeIsOnANetwork = 0;
		evt = BDB_EVT_COMMISSIONING_NETWORK_STEER_FINISH;
	}
	//g_bdbAttrs.commissioningStatus = status;
	BDB_STATUS_SET(status);
	TL_SCHEDULE_TASK(bdb_task, (void *)evt);
}

/*********************************************************************
 * @fn      bdb_nodeDescRespHandler
 *
 * @brief   the handler of receiving node description response
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_BDB_ static void bdb_nodeDescRespHandler(void *arg)
{
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_node_descript_resp_t *rsp = (zdo_node_descript_resp_t*)p->zpdu;

	/*Did the server mask indicate revision r21 or greater?*/
	//TODO
	if((rsp->status == ZDO_SUCCESS)){
		u8 stackRev = (rsp->node_descriptor.server_mask >> 9) & 0x3f;
		if(stackRev >= 21){
			if(ss_apsmeRequestKeyReq(SS_KEYREQ_TYPE_TCLK, ss_ib.trust_center_address, NULL) != APS_STATUS_ILLEGAL_REQUEST){
				g_bdbAttrs.nodeIsOnANetwork = 0;
				return;
			}
		}
	}else{
		return;
	}

	bdb_retrieveTcLinkKeyDone(BDB_COMMISSION_STA_SUCCESS);
}

/*********************************************************************
 * @fn      bdb_retrieveTcLinkKeyTimeout
 *
 * @brief   the handler for the timeout of retrieving trust center link key
 *
 * @param   arg
 *
 * @return  None
 */
_CODE_BDB_ static s32 bdb_retrieveTcLinkKeyTimeout(void *arg)
{
	if(g_bdbAttrs.tcLinkKeyExchangeAttempts++ < g_bdbAttrs.tcLinkKeyExchangeAttemptsMax){
		/* send node description */
		u8 sn = 0;
		zdo_node_descriptor_req_t req;
		req.nwk_addr_interest = 0x0000;
		zb_zdoNodeDescReq(0x0000, &req, &sn, bdb_nodeDescRespHandler);
		return 0;
	}else{
		if(!zb_isDeviceJoinedNwk()){
			g_bdbAttrs.tcLinkKeyExchangeAttempts = 0;
			g_bdbCtx.retrieveTcLkKeyTimer = NULL;
			/*wait until leave confirm */
			bdb_retrieveTcLinkKeyDone(BDB_COMMISSION_STA_TCLK_EX_FAILURE);
			return -1;
		}
		/* leave the network */
		nlme_leave_req_t req;
		ZB_IEEE_ADDR_COPY(req.deviceAddr, g_zbMacPib.extAddress);
		req.rejoin = 0;
		req.removeChildren = 0;
		if(!g_bdbCtx.leaveDoing){
			if(RET_OK == zb_nlmeLeaveReq(&req)){
				g_bdbCtx.leaveDoing = 1;
			}
		}
		return 3*1000 * 1000;
	}
}

/*********************************************************************
 * @fn      bdb_retrieveTcLinkKeyStart
 *
 * @brief   retrieve trust center link key
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ static s32 bdb_retrieveTcLinkKeyStart(void *arg)
{
	if(g_bdbAttrs.tcLinkKeyExchangeMethod == TCKEY_EXCHANGE_METHOD_APSRK){
		/* send node description */
		u8 sn = 0;
		zdo_node_descriptor_req_t req;
		req.nwk_addr_interest = 0x0000;
		zb_zdoNodeDescReq(0x0000, &req, &sn, bdb_nodeDescRespHandler);

		//zb_setPollRate(POLL_RATE);
		g_bdbCtx.leaveDoing = 0;
		g_bdbAttrs.tcLinkKeyExchangeAttempts = 0;
		g_bdbAttrs.tcLinkKeyExchangeAttemptsMax = 3;
		if(!g_bdbCtx.retrieveTcLkKeyTimer){
			g_bdbCtx.retrieveTcLkKeyTimer = TL_ZB_TIMER_SCHEDULE(bdb_retrieveTcLinkKeyTimeout, NULL, BDBC_TC_LINK_KEY_EXCHANGE_TIMEOUT * 1000 * 1000);
		}
	}else{
		bdb_retrieveTcLinkKeyDone(BDB_COMMISSION_STA_SUCCESS);
	}

	return -1;
}


/*********************************************************************
 * @fn      bdb_networkSteerNonFactoryNew
 *
 * @brief   start Network Steer during commissioning for non-factory new node
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static void bdb_networkSteerNonFactoryNew(void)
{
	/* step2: Broadcast	Mgmt_Permit_Joining_req	command */
	bdb_mgmtPermitJoiningTrig(NULL);
}

/*********************************************************************
 * @fn      bdb_networkSteerFactoryNew
 *
 * @brief   start Network Steer during commissioning for factory new node
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static void bdb_networkSteerFactoryNew(void)
{
	//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_IN_PROGRESS;
	u32 scanChannels = aps_ib.aps_channel_mask;

	/* perform join work flow */
	nlme_nwkDisc_req_t req;
	req.scanChannels = scanChannels;
	req.scanDuration = g_bdbAttrs.scanDuration;

	zdo_nwk_discovery_Start(&req, NULL);
}

/*********************************************************************
 * @fn      bdb_commissioningNetworkSteer
 *
 * @brief   start Network Steer during commissioning
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static u8 bdb_commissioningNetworkSteer(void)
{
	u8 status = BDB_STATE_IDLE;
	if(!g_bdbAttrs.commissioningMode.networkSteer){
		status = bdb_commissioningNetworkFormation();
	}else{
		/*
		 * perform network steer
		 * */
		status = BDB_STATE_COMMISSIONING_NETWORK_STEER;
		if(g_bdbAttrs.nodeIsOnANetwork){
			/* rejoin */
			bdb_networkSteerNonFactoryNew();
		}else{
			/* join */
			bdb_networkSteerFactoryNew();
		}

	}
	return status;
}

/*********************************************************************
 * @fn      bdb_task
 *
 * @brief
 *
 * @param   arg
 *
 * @return
 */
static void bdb_task(void *arg)
{
	u32 evt = (u32)arg;
	u8 status = BDB_STATE_IDLE;

	switch (BDB_STATE_GET()){
		case BDB_STATE_INIT:   //factory new or non-factory new device
			BDB_STATE_SET(BDB_STATE_IDLE);
			if(evt == BDB_EVT_INIT_DONE){
				if(g_bdbCtx.bdbAppCb->bdbInitCb){
					g_bdbCtx.bdbAppCb->bdbInitCb(g_bdbCtx.initResult, g_bdbAttrs.nodeIsOnANetwork);
				}

				if(g_bdbAttrs.nodeIsOnANetwork){
					nv_nwkFrameCountSaveToFlash(ss_ib.outgoingFrameCounter);

					if(g_bdbCtx.initResult == BDB_INIT_STATUS_SUCCESS){
						if((zdo_ssInfoKeyGet() != ss_ib.activeSecureMaterialIndex) || g_zbNwkCtx.parentIsChanged){
							g_zbNwkCtx.parentIsChanged = 0;
							TL_SCHEDULE_TASK(bdb_commissioningInfoSave, NULL);  //for tcRejoin when the network key is changed
						}

#if ZB_ROUTER_ROLE
						g_zbNwkCtx.joinAccept = 1;

						/* check if sending parent announcement */
						zb_zdoSendParentAnnce();
#endif
					}
				}

				g_bdbCtx.inited = 0;
			}
			break;

		case BDB_STATE_IDLE:
			/*
			 * commissioning start handle
			 * */
			if(evt == BDB_EVT_COMMISSIONING_START){
				if(!g_bdbAttrs.commissioningMode.touchlink){
					status = bdb_commissioningNetworkSteer();
				}else{
					/*
					 * perform touch link for initiator
					 *
					 * */
					if(!g_bdbAttrs.nodeIsOnANetwork){
						ss_securityModeSet(SS_SEMODE_DISTRIBUTED);
					}

					if(g_bdbCtx.role == BDB_COMMISSIONING_ROLE_INITIATOR){
						zcl_touchLinkStart();
						status = BDB_STATE_COMMISSIONING_TOUCHLINK;
					}
				}

				if(status == BDB_STATE_IDLE){
					/* confirm to application */
					g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_NO_NETWORK;
					if(g_bdbCtx.bdbAppCb->bdbcommissioningCb){
						g_bdbCtx.bdbAppCb->bdbcommissioningCb(g_bdbAttrs.commissioningStatus, NULL);
					}
				}
				BDB_STATE_SET(status);
			}else if(evt == BDB_STATE_REJOIN_DONE){
				bdb_topLevelCommissiongConfirm();
			}
			break;

		case BDB_STATE_COMMISSIONING_TOUCHLINK:
			if(evt == BDB_EVT_COMMISSIONING_TOUCHLINK_NO_RESP){
				/*
				 * if no scan response received, start network steer
				 * */
				g_bdbAttrs.commissioningMode.touchlink = 0;
				BDB_STATE_SET(BDB_STATE_IDLE);
#if ZB_ED_ROLE
				bdb_topLevelCommissioning(BDB_COMMISSIONING_ROLE_INITIATOR);
#else
				bdb_topLevelCommissioning(BDB_COMMISSIONING_ROLE_TARGET);
#endif
			}else if(evt == BDB_EVT_COMMISSIONING_TOUCHLINK_FINISH){
				if(BDB_STATUS_GET() == BDB_COMMISSION_STA_SUCCESS){
					bdb_commissioningInfoSave(NULL);
				}
				/* confirm to application */
				bdb_topLevelCommissiongConfirm();
				BDB_STATE_SET(BDB_STATE_IDLE);
			}
			break;

		case BDB_STATE_COMMISSIONING_NETWORK_STEER:
			if(evt == BDB_EVT_COMMISSIONING_NETWORK_STEER_RETRIEVE_TCLINK_KEY){
				TL_ZB_TIMER_SCHEDULE(bdb_retrieveTcLinkKeyStart, NULL, 1*1000*1000);
			}else if(evt == BDB_EVT_COMMISSIONING_NETWORK_STEER_PERMITJOIN){
				u8 sn = 0;
				zb_mgmtPermitJoinReqTx(0xffff, BDBC_MIN_COMMISSIONING_TIME, 0x01, &sn, NULL);
				TL_SCHEDULE_TASK(bdb_mgmtPermitJoiningConfirm,NULL);
			}else if(evt == BDB_EVT_COMMISSIONING_NETWORK_STEER_FINISH){
				status = bdb_commissioningNetworkFormation();
				if(status == BDB_STATE_IDLE){
					/* confirm to application */
					bdb_topLevelCommissiongConfirm();
				}
				BDB_STATE_SET(status);
			}
			break;

		case BDB_STATE_COMMISSIONING_NETWORK_FORMATION:
			if( evt == BDB_EVT_COMMISSIONING_NETWORK_FORMATION_PERMITJOIN){
				u8 sn = 0;
				g_bdbAttrs.commissioningMode.networkSteer = 1;
				zb_mgmtPermitJoinReqTx(0xffff, BDBC_MIN_COMMISSIONING_TIME, 0x01, &sn, NULL);
				TL_SCHEDULE_TASK(bdb_mgmtPermitJoiningConfirm,NULL);
			}else if(evt == BDB_EVT_COMMISSIONING_NETWORK_FORMATION_FINISH){
				status = bdb_commissioningFindBind();
				if(status == BDB_STATE_IDLE){
					/* confirm to application */
					bdb_topLevelCommissiongConfirm();
				}
				BDB_STATE_SET(status);
			}
			break;

		case BDB_STATE_COMMISSIONING_FINDORBIND:
			if(evt == BDB_EVT_COMMISSIONING_FINDORBIND_SIMPLE_DESC_REQ){
				bdb_commissioningFindBindSimpleDescReq();
			}else if(evt == BDB_EVT_COMMISSIONING_FINDORBIND_FINISH){
				bdb_findBindRst();
				bdb_topLevelCommissiongConfirm();
				BDB_STATE_SET(BDB_STATE_IDLE);
			}
			break;

		default:
			break;
	}
}

/*********************************************************************
 * @fn      bdb_task_delay
 *
 * @brief
 *
 * @param   arg
 *
 * @return
 */
_CODE_BDB_ static s32 bdb_task_delay(void *arg)
{
	TL_SCHEDULE_TASK(bdb_task, arg);
	return -1;
}

/*********************************************************************
 * @fn      bdb_zdoStartDevCnf
 *
 * @brief   node start confirm handle
 *
 * @param   startDevCnf
 *
 * @return
 */
_CODE_BDB_ void bdb_zdoStartDevCnf(void *arg){
	zdo_start_device_confirm_t *startDevCnf = (zdo_start_device_confirm_t *)arg;
	u32 evt = BDB_EVT_IDLE;
	u8 state = BDB_STATE_GET();

	switch(state){
		case BDB_STATE_IDLE:
			/* for rejoin indication by the stack */
			if(startDevCnf->status == SUCCESS){
				//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS;
				BDB_STATUS_SET(BDB_COMMISSION_STA_SUCCESS);
				if((zdo_ssInfoKeyGet() != ss_ib.activeSecureMaterialIndex) || g_zbNwkCtx.parentIsChanged){
					g_zbNwkCtx.parentIsChanged = 0;
					TL_SCHEDULE_TASK(bdb_commissioningInfoSave, NULL);
				}
			}else{
				BDB_STATUS_SET(BDB_COMMISSION_STA_PARENT_LOST);
				//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_TARGET_FAILURE;
			}
			evt = BDB_STATE_REJOIN_DONE;
			TL_SCHEDULE_TASK(bdb_task, (void *)evt);
			break;

		case BDB_STATE_INIT:
			evt = BDB_EVT_INIT_DONE;
			g_bdbCtx.initResult = BDB_INIT_STATUS_SUCCESS;
			if(startDevCnf->status != SUCCESS){
				g_bdbCtx.initResult = BDB_INIT_STATUS_FAILURE;
			}
			TL_SCHEDULE_TASK(bdb_task, (void *)evt);
			break;

		case BDB_STATE_COMMISSIONING_TOUCHLINK:
			if(startDevCnf->status == SUCCESS){
				g_bdbAttrs.nodeIsOnANetwork = 1;
				g_zbNwkCtx.joinAccept = 1;
				zcl_touchLinkDevStartIndicate();
			}else{
				//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_NO_NETWORK;
				BDB_STATUS_SET(BDB_COMMISSION_STA_NO_NETWORK);
				evt = BDB_EVT_COMMISSIONING_NETWORK_STEER_FINISH;
				TL_SCHEDULE_TASK(bdb_task, (void *)evt);
			}
			break;

		case BDB_STATE_COMMISSIONING_NETWORK_STEER:
			if(startDevCnf->status == SUCCESS){
				//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS;
				g_bdbAttrs.nodeIsOnANetwork = 1;
				BDB_STATUS_SET(BDB_COMMISSION_STA_SUCCESS);
				if(!ZB_IEEE_ADDR_IS_INVAILD(ss_ib.trust_center_address)){
					evt = BDB_EVT_COMMISSIONING_NETWORK_STEER_RETRIEVE_TCLINK_KEY;
				}else{
					evt = BDB_EVT_COMMISSIONING_NETWORK_STEER_PERMITJOIN;
				}

	#if ZB_ED_ROLE
				u32 nwkStartDelay = 3*1000*1000;
	#else
				u32 nwkStartDelay = 200*1000;
	#endif
				TL_ZB_TIMER_SCHEDULE(bdb_task_delay, (void *)evt, nwkStartDelay);
			}else{
				//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_NO_NETWORK;
				BDB_STATUS_SET(BDB_COMMISSION_STA_NO_NETWORK);
				evt = BDB_EVT_COMMISSIONING_NETWORK_STEER_FINISH;
				TL_SCHEDULE_TASK(bdb_task, (void *)evt);
			}
			break;

		case BDB_STATE_COMMISSIONING_NETWORK_FORMATION:
			if(startDevCnf->status == SUCCESS){
				g_bdbAttrs.nodeIsOnANetwork = 1;
				g_zbNwkCtx.joinAccept = 1;

				//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS;
				BDB_STATUS_SET(BDB_COMMISSION_STA_SUCCESS);
				evt = BDB_EVT_COMMISSIONING_NETWORK_FORMATION_PERMITJOIN;

				if(g_bdbCtx.bdbAppCb->bdbcommissioningCb){
					g_bdbCtx.bdbAppCb->bdbcommissioningCb(BDB_COMMISSION_STA_FORMATION_DONE, NULL);
				}
			}else{
				/* confirm to application */
				//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_FORMATION_FAILURE;
				BDB_STATUS_SET(BDB_COMMISSION_STA_FORMATION_FAILURE);
				evt = BDB_EVT_COMMISSIONING_NETWORK_FORMATION_FINISH;
			}
			TL_SCHEDULE_TASK(bdb_task, (void *)evt);
			break;

		default:
			//TL_SCHEDULE_TASK(zb_info_save,NULL);

			break;
	}
}

/*********************************************************************
 * @fn      bdb_touchLinkCallback
 *
 * @brief
 *
 * @param   status
 * 			arg
 *
 * @return
 */
_CODE_BDB_ static void bdb_touchLinkCallback(u8 status, void *arg)
{
	u32 evt = BDB_EVT_IDLE;
	if(status == ZCL_ZLL_TOUCH_LINK_STA_TARGET_START){
		BDB_STATE_SET(BDB_STATE_COMMISSIONING_TOUCHLINK);
		g_bdbAttrs.commissioningMode.touchlink = 1;
		g_bdbCtx.role = BDB_COMMISSIONING_ROLE_TARGET;
	}else if (status == ZCL_ZLL_TOUCH_LINK_STA_NO_SERVER){
		/*
		 * if no scan response received, start classic commissioning,
		 * resume some information which is changed during touch link
		 *
		 * */
		if(is_device_factory_new()){
			bdb_scanCfg(g_bdbAttrs.primaryChannelSet | g_bdbAttrs.secondaryChannelSet, g_bdbAttrs.scanDuration);
			/* start classic commissioning */
			evt = BDB_EVT_COMMISSIONING_TOUCHLINK_NO_RESP;
			//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_NO_SCAN_RESPONSE;
			BDB_STATUS_SET(BDB_COMMISSION_STA_NO_SCAN_RESPONSE);
		}else{
			evt = BDB_EVT_COMMISSIONING_TOUCHLINK_FINISH;
			//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_TARGET_FAILURE;
			BDB_STATUS_SET(BDB_COMMISSION_STA_TARGET_FAILURE);
		}
    }else{
    	evt = BDB_EVT_COMMISSIONING_TOUCHLINK_FINISH;
    	if(status == ZCL_ZLL_TOUCH_LINK_STA_SUCC || status == ZCL_ZLL_TOUCH_LINK_STA_EXIST){
    		ss_securityModeSet(SS_SEMODE_DISTRIBUTED);/* AIB */

			//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS;
    		BDB_STATUS_SET(BDB_COMMISSION_STA_SUCCESS);
		}else{
			//g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_TARGET_FAILURE;
			BDB_STATUS_SET(BDB_COMMISSION_STA_TARGET_FAILURE);
		}
    }

	TL_SCHEDULE_TASK(bdb_task, (void *)evt);
}

/*********************************************************************
 * @fn      bdb_commssionUtilityCallback
 *
 * @brief
 *
 * @param   cmd
 * 			arg
 *
 * @return
 */
_CODE_BDB_ static void bdb_commssionUtilityCallback(u8 cmd, void *arg)
{

}

/*********************************************************************
 * @fn      bdb_topLevelCommissiongConfirm
 *
 * @brief   the top level commissioning  callback
 *
 * @param   status
 *
 * @return
 */
_CODE_BDB_ static u8 bdb_topLevelCommissiongConfirm(void){
	g_bdbAttrs.commissioningStatus = BDB_STATUS_GET();

	if(g_bdbCtx.identifyTimer){
		TL_ZB_TIMER_CANCEL(&g_bdbCtx.identifyTimer);
	}

	if(g_bdbCtx.bdbAppCb->bdbcommissioningCb){
		g_bdbCtx.bdbAppCb->bdbcommissioningCb(g_bdbAttrs.commissioningStatus, NULL);
	}

	if(g_bdbAttrs.commissioningStatus == BDB_COMMISSION_STA_SUCCESS){
		u8 len = 0;
		tl_zbMacAttrGet(MAC_PHY_ATTR_CURRENT_CHANNEL, &g_bdbCtx.channel, &len);
#if ZB_COORDINATOR_ROLE
		if(g_bdbCtx.factoryNew == 1){
			bdb_commissioningInfoSave(NULL);
			g_bdbCtx.factoryNew = 0;
		}
#endif
	}else{
#if ZB_ROUTER_ROLE
		if(BDB_TOUCHLINK_CAP_EN() && is_device_factory_new()){
			/*
			 * set channel from primaryChannelSet in bdb attributes
			 * */
			ZB_TRANSCEIVER_SET_CHANNEL(g_bdbCtx.channel);
			rf_setTrxState(RF_STATE_RX);
		}
#endif
	}

	return 0;
}

/*********************************************************************
 * @fn      bdb_topLevelCommissioning
 *
 * @brief   the top level commissioning procedure that is activated on some trigger
 *
 * @param   target
 *
 * @return
 */
_CODE_BDB_ static u8 bdb_topLevelCommissioning(u8 target){
	/* restore 	persistent ZigBee data */

	if(BDB_STATE_GET() != BDB_STATE_IDLE || g_bdbAttrs.commissioningStatus == BDB_COMMISSION_STA_IN_PROGRESS){
		return BDB_STATE_COMMISSIONING_BUSY;
	}

	g_bdbCtx.role = target;
	g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_IN_PROGRESS;

	u32 evt = BDB_EVT_COMMISSIONING_START;
	TL_SCHEDULE_TASK(bdb_task, (void *)evt);

	return BDB_STATE_IDLE;
}

/*********************************************************************
 * @fn      bdb_networkFormationStart
 *
 * @brief   start network formation
 *
 * @param   bdb state:
 *
 * @return
 */
_CODE_BDB_ u8 bdb_networkFormationStart(void)
{
	g_bdbAttrs.commissioningMode.networkSteer = 0;
	g_bdbAttrs.commissioningMode.touchlink = 0;
	g_bdbAttrs.commissioningMode.findOrBind = 0;
	g_bdbAttrs.commissioningMode.networkFormation = 1;

#if ZB_COORDINATOR_ROLE
	ss_securityModeSet(SS_SEMODE_CENTRALIZED);
#else
	ss_securityModeSet(SS_SEMODE_DISTRIBUTED);
#endif

	return bdb_topLevelCommissioning(BDB_COMMISSIONING_ROLE_TARGET);
}

/*********************************************************************
 * @fn      bdb_networkSteerStart
 *
 * @brief   classic join start
 *
 * @param   None
 *
 * @return
 */
_CODE_BDB_ u8 bdb_networkSteerStart(void)
{
	g_bdbAttrs.commissioningMode.networkFormation = 0;
	g_bdbAttrs.commissioningMode.touchlink = 0;
	g_bdbAttrs.commissioningMode.findOrBind = 0;
	g_bdbAttrs.commissioningMode.networkSteer = 1;
	return bdb_topLevelCommissioning(BDB_COMMISSIONING_ROLE_TARGET);
}

/*********************************************************************
 * @fn      bdb_networkTouchLinkStart
 *
 * @brief
 *
 * @param   role: BDB_COMMISSIONING_ROLE_INITIATOR or BDB_COMMISSIONING_ROLE_TARGET
 *
 * @return
 */
_CODE_BDB_ u8 bdb_networkTouchLinkStart(u8 role)
{
	g_bdbAttrs.commissioningMode.networkSteer = 1;
	g_bdbAttrs.commissioningMode.networkFormation = 0;
	g_bdbAttrs.commissioningMode.findOrBind = 0;
	g_bdbAttrs.commissioningMode.touchlink = 1;
	return bdb_topLevelCommissioning(role);
}

/*********************************************************************
 * @fn      bdb_findAndBindStart
 *
 * @brief
 *
 * @param   role: BDB_COMMISSIONING_ROLE_INITIATOR or BDB_COMMISSIONING_ROLE_TARGET
 *
 * @return
 */
_CODE_BDB_ u8 bdb_findAndBindStart(u8 role)
{
	g_bdbAttrs.commissioningMode.networkSteer = 0;
	g_bdbAttrs.commissioningMode.networkFormation = 0;
	g_bdbAttrs.commissioningMode.touchlink = 0;
	g_bdbAttrs.commissioningMode.findOrBind = 1;
	return bdb_topLevelCommissioning(role);
}

/*********************************************************************
 * @fn      check the bdb state
 *
 * @brief
 *
 * @param   1: idle, 0: busy
 *
 * @return
 */
bool bdb_isIdle(void){
	if(BDB_STATE_GET() == BDB_STATE_IDLE && g_bdbAttrs.commissioningStatus != BDB_COMMISSION_STA_IN_PROGRESS){
		return 1;
	}
	return 0;
}

/*********************************************************************
 * @fn      bdb_init
 *
 * @brief   bdb Initialization
 *
 * @param   simple_desc
 * 			setting
 * 			cb
 * 			repower
 *
 * @return  None
 */
_CODE_BDB_ u8 bdb_init(af_simple_descriptor_t *simple_desc, bdb_commissionSetting_t *setting, bdb_appCb_t *cb, u8 repower){
	if(g_bdbCtx.inited){
		return 1;
	}
	g_bdbCtx.inited = 1;
	g_bdbCtx.bdbAppCb = cb;
	g_bdbCtx.simpleDesc = simple_desc;
	g_bdbCtx.commisionSettings = setting;
	BDB_STATE_SET(BDB_STATE_INIT);


	/* check if it's a factory-new device */
	g_bdbCtx.factoryNew = zb_isDeviceFactoryNew();

	/* security configure */
	ss_zdoInit(TRUE, SS_PRECONFIGURED_NOKEY);

	/* link configure */
	bdb_linkKeyCfg(setting, g_bdbCtx.factoryNew);

	/* pre-configure for touch link */
	bdb_touchLinkPreCfg(simple_desc->endpoint, setting, &bdb_touchlinkCb);

	bdb_scanCfg(g_bdbAttrs.primaryChannelSet | g_bdbAttrs.secondaryChannelSet, g_bdbAttrs.scanDuration);

	g_bdbAttrs.commissioningStatus = BDB_COMMISSION_STA_SUCCESS,
	g_bdbAttrs.nodeIsOnANetwork = g_bdbCtx.factoryNew ? 0 : 1;
	if(g_bdbAttrs.nodeIsOnANetwork){
		/* update outgoing frame count */
		bdb_outgoingFrameCountUpdate(repower);

#if ZB_COORDINATOR_ROLE
		bdb_coordinatorStart();
#elif ZB_ROUTER_ROLE
		bdb_routerStart();
#elif ZB_ED_ROLE
		bdb_endDeviceStart(repower);
#endif
	}else{
		/* if it's a factory-new device,do nothing except configure some attribute for touch link */
		bdb_factoryNewDevCfg(BDB_TOUCHLINK_CAP_EN(), setting->touchlinkChannel);

		u32 evt = BDB_EVT_INIT_DONE;
		TL_SCHEDULE_TASK(bdb_task, (void *)evt);
	}

	/* get the real operation channel for non-factory new device */
	u8 len = 0;
	tl_zbMacAttrGet(MAC_PHY_ATTR_CURRENT_CHANNEL, &g_bdbCtx.channel, &len);
	return 1;
}


/*********************************************************************
 * @fn      bdb_findBindMatchClusterSet
 *
 * @brief   for initiator
 *
 * @param   clusterNum
 * 			clusterId
 *
 * @return  None
 */
_CODE_BDB_ void bdb_findBindMatchClusterSet(u8 clusterNum, u16 clusterId[])
{
	g_bdbCtx.clusterNum = clusterNum;
	g_bdbCtx.clusterList = clusterId;
}

/*********************************************************************
 * @fn      bdb_addIdentifyActiveEpForFB
 *
 * @brief   for initiator
 *
 * @param   dstInfo
 *
 * @return  TRUE / FALSE
 */
_CODE_BDB_ bool bdb_addIdentifyActiveEpForFB(findBindDst_t dstInfo)
{
	if(BDB_STATE_GET() == BDB_STATE_COMMISSIONING_FINDORBIND && g_bdbCtx.role == BDB_COMMISSIONING_ROLE_INITIATOR){
		if(!g_bdbCtx.pFindBindQ){
			g_bdbCtx.pFindBindQ = queueInit();
			if(!g_bdbCtx.pFindBindQ){
				return FALSE;
			}
		}

		return queuePush(g_bdbCtx.pFindBindQ, dstInfo);
	}

	return FALSE;
}

/*********************************************************************
 * @fn      bdb_defaultReportingCfg
 *
 * @brief
 *
 * @param   endpoint
 * 			profileID
 * 			clusterID
 * 			attrID
 * 			minReportInt
 * 			maxReportInt
 * 			reportableChange
 *
 * @return  zcl status
 */
_CODE_BDB_ status_t bdb_defaultReportingCfg(u8 endpoint, u16 profileID, u16 clusterID, u16 attrID, u16 minReportInt, u16 maxReportInt, u8 *reportableChange)
{
	zclCfgReportRec_t cfgReportRec;
	TL_SETSTRUCTCONTENT(cfgReportRec, 0);

	reportCfgInfo_t *pEntry = zcl_reportCfgInfoEntryFind(endpoint, clusterID, attrID);
	if(pEntry){
		return ZCL_STA_DUPLICATE_EXISTS;
	}

	zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, clusterID, attrID);
	if(!pAttrEntry){
		return ZCL_STA_UNSUPPORTED_ATTRIBUTE;
	}

	cfgReportRec.attrID = attrID;
	cfgReportRec.dataType = pAttrEntry->type;
	cfgReportRec.direction = 0;
	cfgReportRec.minReportInt = minReportInt;
	cfgReportRec.maxReportInt = maxReportInt;
	cfgReportRec.reportableChange = reportableChange;

	return zcl_configureReporting(endpoint, profileID, clusterID, &cfgReportRec);
}

/*********************************************************************
 * @fn      bdb_attrSet
 *
 * @brief   set attributes for base device behavior
 *
 * @param   attrId  - attribute identify
 * 			value	- the value set to bdb's attribute
 *
 * @return  None
 */
_CODE_BDB_ void bdb_attrSet(u8 attrId, u8 *value)
{
	if(attrId > BDB_ATTR_TC_NODE_JOIN_TIMEOUT || attrId < BDB_ATTR_PRIMARY_CHANNEL_SET){
		return;
	}

	u8 len = g_bdbAttrTbl[attrId-BDB_ATTR_PRIMARY_CHANNEL_SET].len;
	memcpy((u8 *)&g_bdbAttrs+g_bdbAttrTbl[attrId-BDB_ATTR_PRIMARY_CHANNEL_SET].offset, value, len);
	return;
}

/*********************************************************************
 * @fn      bdb_attrGet
 *
 * @brief   get attributes for base device behavior
 *
 * @param   attrId	- attribute identify
 * 			value	- the value gotten from bdb's attribute
 * 			len		- the length of the  bdb's attribute
 *
 * @return  BDB_COMMISSION_STA_SUCCESS / BDB_COMMISSION_STA_NOT_PERMITTED
 */
_CODE_BDB_ u8 bdb_attrGet(u8 attrId, u8* value, u8* len)
{
	if(attrId > BDB_ATTR_TC_NODE_JOIN_TIMEOUT || attrId < BDB_ATTR_PRIMARY_CHANNEL_SET){
		return BDB_COMMISSION_STA_NOT_PERMITTED;
	}
	*len = g_bdbAttrTbl[attrId-BDB_ATTR_PRIMARY_CHANNEL_SET].len;
	memcpy(value, (u8 *)&g_bdbAttrs+g_bdbAttrTbl[attrId-BDB_ATTR_PRIMARY_CHANNEL_SET].offset, *len);

	return BDB_COMMISSION_STA_SUCCESS;
}

/*********************************************************************
 * @fn      tl_bdbReset
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ void tl_bdbReset(void)
{
	af_simple_descriptor_t *sd = g_bdbCtx.simpleDesc;
	bdb_appCb_t *bcb = g_bdbCtx.bdbAppCb;
	bdb_commissionSetting_t *commisionSet = g_bdbCtx.commisionSettings;
	g_bdbAttrs.nodeIsOnANetwork = 0;
	memset((u8 *)&g_bdbCtx,0,sizeof(g_bdbCtx));
	bdb_init(sd, commisionSet, bcb, 1);
}

/*********************************************************************
 * @fn      tl_bdbReset2FN
 *
 * @brief   Clear all ZigBee persistent data except the outgoing NWK frame counter
 * 			and perform a reset such that the target is in much the same state as
 * 			it was when it left the factory.
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ void tl_bdbReset2FN(void)
{
	nv_resetToFactoryNew();
	zb_reset();
	tl_bdbReset();
}

/*********************************************************************
 * @fn      tl_bdbAttrInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
_CODE_BDB_ void tl_bdbAttrInit(void)
{
	memcpy(&g_bdbAttrs, &g_bdbAttrsDft, sizeof(bdb_attr_t));
}

