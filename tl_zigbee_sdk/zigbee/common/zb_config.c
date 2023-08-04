/********************************************************************************************************
 * @file    zb_config.c
 *
 * @brief   This is the source file for zb_config
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

/**********************************************************************
 * INCLUDES
 */
#include "includes/zb_common.h"

sys_diagnostics_t g_sysDiags;

/* PAN ID setting */
/*
 * Setting this to a value other than MAC_INVALID_PANID (0xFFFF) causes
 * ZDO_COORD to use this value as its PAN ID and
 * Routers and end devices to join PAN with this ID
 */
#define DEFAULT_PANID		MAC_INVALID_PANID
u16 TL_ZB_ASSOCJOIN_PERMIT_PANID = DEFAULT_PANID;

/*
 * Setting this to a value other than MAC_INVALID_PANID (0xFFFF) causes
 * Routers and end devices to filter the PAN ID when associate join a network
 */
#define FILTER_PANID		MAC_INVALID_PANID
u16 TL_ZB_ASSOCJOIN_FILTER_PANID = FILTER_PANID;

/* APS data fragmentation setting */
u8 APS_INTERFRAME_DELAY = 100;//ms
u8 APS_MAX_WINDOW_SIZE = 1;
u8 APS_FRAGMEMT_PAYLOAD_SIZE = 64;

//The maximum number of retries allowed after a transmission failure.
u8 APS_MAX_FRAME_RETRIES = 3;
u8 APS_ACK_EXPIRY = 2;//seconds

/* buffer pool size for zigbee pro */
u8 ZB_BUF_POOL_SIZE = ZB_BUF_POOL_NUM;
zb_buf_pool_t g_mPool;

/* taskQ size */
u8 ZB_TASKQ_USERUSE_SIZE = TL_ZBTASKQ_USERUSE_SIZE;
tl_zbtaskq_user_t taskQ_user;

/* for indirect pending queue */
u8 ZB_MAC_PENDING_TRANS_QUEUE_SIZE = ZB_MAC_PENDING_TRANS_QUEUE_NUM;
u8 ZB_MAC_EXT_EXPEIRY_CNT = ZB_MAC_INTERNAL_EXPIRY_CNT;

/* the lqi threshold for neighbor */
u8 NWK_NEIGHBORTBL_ADD_LQITHRESHOLD = 0x45;

/* the life time for unauthorized child table, 5 seconds */
u32 NWK_UNAUTH_CHILD_TABLE_LIFE_TIME = (5 * 1000);

/* timeout waiting for transport nwk key during association join or security rejoin, 2 seconds */
u32 TRANSPORT_NETWORK_KEY_WAIT_TIME = (2 * 1000);

/* the cost threshold for one hop */
u8 NWK_COST_THRESHOLD_ONEHOP = 7;

/* the cost threshold to choose next hop from neighbor table */
u8 NWK_NEIGHBOR_SEND_OUTGOING_THRESHOLD = 5;

/* whether add src ieee address in nwk header for the data from higher level */
bool NWK_HEADER_SRC_IEEE_INCLUDE = FALSE;

/* whether a node on the nwk that transmits a Mgmt_permit_join is allowed */
#if ZB_COORDINATOR_ROLE
bool SS_ALLOW_REMOTE_TC_POLICY_CHANGE = TRUE;//FALSE;
#endif

/* address mapping table */
u16 TL_ZB_NWK_ADDR_MAP_SIZE = TL_ZB_NWK_ADDR_MAP_NUM;
tl_zb_addr_map_t g_nwkAddrMap;

/* neighbor table */
u8 TL_ZB_NEIGHBOR_TABLE_SIZE = TL_ZB_NEIGHBOR_TABLE_NUM;
u8 TL_ZB_CHILD_TABLE_SIZE = TL_ZB_CHILD_TABLE_NUM;
tl_zb_neighbor_entry_t g_zb_neighborTbl;

/* routing table */
#if ZB_ROUTER_ROLE
u8 NWKC_TRANSFAILURE_CNT_THRESHOLD = TRANSFAILURE_CNT_MAX;
u8 NWKC_INITIAL_RREQ_RETRIES = NWK_INITIAL_RREQ_RETRIES;
u8 NWKC_RREQ_RETRIES = NWK_RREQ_RETRIES;
u16 ROUTING_TABLE_SIZE = ROUTING_TABLE_NUM;
#if (!ROUTING_TABLE_NUM)
nwk_routingTabEntry_t g_routingTab[1];
#else
nwk_routingTabEntry_t g_routingTab[ROUTING_TABLE_NUM];
#endif
#endif

/* routing record table */
#if ZB_COORDINATOR_ROLE
u16 NWK_ROUTE_RECORD_TABLE_SIZE = NWK_ROUTE_RECORD_TABLE_NUM;
#if (!NWK_ROUTE_RECORD_TABLE_NUM)
nwk_routeRecordTabEntry_t g_routeRecTab[1];
#else
nwk_routeRecordTabEntry_t g_routeRecTab[NWK_ROUTE_RECORD_TABLE_NUM];
#endif
#endif

/* Broadcasting table */
#if ZB_ROUTER_ROLE
bool NWK_BRC_PASSIVE_ACK_ENABLE = TRUE;
u8 NWK_BRC_TRANSTBL_SIZE = NWK_BRC_TRANSTBL_NUM;
u32 NWK_BRC_JITTER = NWK_MAX_BROADCAST_JITTER;
nwk_brcTransRecordEntry_t g_brcTransTab[NWK_BRC_TRANSTBL_NUM];
#endif

#if ZB_ED_ROLE
bool AUTO_QUICK_DATA_POLL_ENABLE = TRUE;
u32 AUTO_QUICK_DATA_POLL_INTERVAL = POLL_RATE_QUARTERSECONDS;//ms
u8 AUTO_QUICK_DATA_POLL_TIMES = 3;
#endif

/* choose the previous parent first when rejoin. */
bool PRE_PARENT_FIRST_WHEN_REJOIN = TRUE;

/* binding table */
u8 APS_BINDING_TABLE_SIZE = APS_BINDING_TABLE_NUM;
aps_binding_entry_t g_apsBindingTbl[APS_BINDING_TABLE_NUM];

/* group table */
u8 APS_GROUP_TABLE_SIZE = APS_GROUP_TABLE_NUM;
aps_group_tbl_ent_t aps_group_tbl[APS_GROUP_TABLE_NUM];
u16 GROUP_MESSAGE_SEND_ADDRESS = NWK_BROADCAST_RX_ON_WHEN_IDLE;

/* APS layer TX cache size */
u8 APS_TX_CACHE_TABLE_SIZE = APS_TX_CACHE_TABLE_NUM;
aps_tx_cache_list_t aps_txCache_tbl[APS_TX_CACHE_TABLE_NUM];

/* the offset of the rx buffer to the zb-buffer */
u8 RX_ZBBUF_OFFSET = TL_RXPRIMITIVEHDR;

/* MAC layer TX Queue size */
u8 MAC_TX_QUEUE_SIZE = TX_QUEUE_BN;
tx_data_queue g_txQueue[TX_QUEUE_BN];

/* WWAH long up time threshold, (24 * 60 * 60) seconds. */
u32 LONG_UPTIME_THRESHOLD = 86400;

//default network key
/* If all zero, will generate 16-bytes network key randomly. */
#if 1
const u8 nwkKeyDefault[] = {0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0};
#else
const u8 nwkKeyDefault[] = {0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89,
			  	  	  	    0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 0x01};
#endif


//Global trust center link key which used in centralized security network
const u8 tcLinkKeyCentralDefault[] = {0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c,
  	   	 	 	 	 	 	 	 	  0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x30, 0x39};


//certification link key for distributed network
const u8 linkKeyDistributedCertification[] = {0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
										      0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf};

/* touch-link link key */
const u8 touchLinkKeyCertification[] = {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
        								0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf};

/* Default PIB settings for MAC layer. */
const tl_zb_mac_pib_t macPibDefault = {
	.ackWaitDuration = (ZB_MAC_UNIT_BACKOFF_PERIOD + ZB_PHY_TURNROUNDTIME + ZB_PHY_SHR_DURATION + (u16)(6 * ZB_PHY_SYMBOLS_PER_OCTET)),
	.frameRetryNum = ZB_MAC_FRAME_RETRIES_MAX,
	.transactionPersistenceTime = 0x01f4,
	.panId = DEFAULT_PANID,
	.shortAddress = MAC_SHORT_ADDR_BROADCAST,
	.respWaitTime = ZB_MAC_WAIT_RESP_TIME_DEFAULT,
	.phyChannelCur = DEFAULT_CHANNEL,
	.autoReq = 0,
#if ZB_ED_ROLE
	.minBe = 0,
#else
	.minBe = 5,
#endif
	.maxBe = 8,
	.beaconOrder = 15,
	.superframeOrder = 0,
	.maxCsmaBackoffs = 4,
	.associationPermit = 0,
	.coordShortAddress = 0xffff,
#if ZB_ED_ROLE
	.rxOnWhenIdle = ZB_MAC_RX_ON_WHEN_IDLE,
#else
	.rxOnWhenIdle = 1,
#endif
};

/* Default NIB settings for NWK layer. */
const nwk_nib_t nwkNibDefault = {
#if defined ZB_NWK_DISTRIBUTED_ADDRESS_ASSIGN && defined ZB_ROUTER_ROLE
	.maxRouters = NWK_MAX_ROUTERS,
	.addrAlloc = NWK_ADDRESS_ALLOC_METHOD_DISTRIBUTED,
#else
	.addrAlloc = NWK_ADDRESS_ALLOC_METHOD_STOCHASTIC,
#endif
	.maxDepth = NWK_MAX_DEPTH,
	.stackProfile = ZB_STACK_PROFILE,
#ifdef ZB_ROUTER_ROLE
	.maxChildren = DEFAULT_MAX_CHILDREN,
	.maxBroadcastRetries = NWK_MAX_BROADCAST_RETRIES,
	.passiveAckTimeout = NWK_PASSIVE_ACK_TIMEOUT,
	.nwkBroadcastDeliveryTime = NWK_BROADCAST_DELIVERY_TIME,
 	.linkStatusPeriod = ZB_NWK_LINK_STATUS_PERIOD_DEFAULT,//45
	.routerAgeLimit = 3,
	.maxSourceRoute = NWK_MAX_SOURCE_ROUTE,
	.concentratorRadius = 0,
	.concentratorDiscoveryTime = 120,//120s
	.symLink = 1,
	.reportConstantCost = 0,
#ifdef ZB_COORDINATOR_ROLE
	.depth = 0,
	.isConcentrator = 1,
#else
	.depth = 1,
	.isConcentrator = 0,
#endif
#endif
	.managerAddr = 0x0000,
    .leaveReqAllowed = 1,
    .useMulticast = 0,
    .panId = DEFAULT_PANID,
    .nwkAddr = NWK_BROADCAST_RESERVED,
    .uniqueAddr = 0,
    .parentInfo = 0,
    .endDevTimeoutDefault = NWK_ENDDEV_TIMEOUT_DEFAULT,
    .leaveReqWithoutRejoinAllowed = 1,
};

/* ZDO default configuration attributes. */
const zdo_attrCfg_t zdoCfgAttrDefault = {
	.config_nwk_indirectPollRate 		= 0,
	.config_parent_link_retry_threshold = ZDO_MAX_PARENT_THRESHOLD_RETRY,
	.config_nwk_scan_attempts 			= ZDO_NWK_SCAN_ATTEMPTS,
	.config_nwk_time_btwn_scans			= ZDO_NWK_TIME_BTWN_SCANS,
	.config_permit_join_duration 		= ZDO_PERMIT_JOIN_DURATION,
	.config_rejoin_times				= ZDO_REJOIN_TIMES,
	.config_rejoin_duration 			= ZDO_REJOIN_DURATION,
	.config_rejoin_backoff_time			= ZDO_REJOIN_BACKOFF_TIME,
	.config_max_rejoin_backoff_time		= ZDO_MAX_REJOIN_BACKOFF_TIME,
	.config_rejoin_backoff_iteration	= ZDO_REJOIN_BACKOFF_ITERATION,
};


#if ZB_ROUTER_ROLE
/*
 * @brief:		get record entry for broadcasting record table
 */
nwk_brcTransRecordEntry_t *brcTransRecordEntryGet(u8 idx){
	return &g_brcTransTab[idx];
}

/*
 * @brief:		get the size of the broadcasting record table
 */
u32 brcTransRecordTblSizeGet(void){
	return (sizeof(g_brcTransTab));
}
#endif

/*
 * @brief:		get the entry of the mapping table of the binding list
 */
aps_binding_entry_t *bindTblEntryGet(void){
	return &g_apsBindingTbl[0];
}

/*
 * @brief:		get the size of zigbee buffer
 */
u32 zbBufferSizeGet(void){
	return (sizeof(g_mPool));
}

/*
 * @brief:		get the size of the neighbor table
 */
u32 neighborTblSizeGet(void){
	return (sizeof(tl_zb_neighbor_entry_t));
}

/*
 * @brief:		get the size of the address mapping table
 */
u32 addrMapTblSizeGet(void){
	return (sizeof(tl_zb_addr_map_t));
}
