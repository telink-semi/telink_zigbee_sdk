/********************************************************************************************************
 * @file     zb_config.c
 *
 * @brief	 table/buffer size configuration for zigbee stack
 *
 * @author
 * @date     Match. 1, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
#include "../include/zb_common.h"


/* PAN ID setting */
/*
 * Setting this to a value other than MAC_INVALID_PANID (0xFFFF) causes
 * ZDO_COORD to use this value as its PAN ID and
 * Routers and end devices to join PAN with this ID
 */
#define DEFAULT_PANID		MAC_INVALID_PANID
/*
 * Setting this to a value other than MAC_INVALID_PANID (0xFFFF) causes
 * Routers and end devices to filter the PAN ID when associate join a network
 */
#define FILTER_PANID		MAC_INVALID_PANID
u16 TL_ZB_ASSOCJOIN_FILTER_PANID = FILTER_PANID;

/* APS data fragmentation setting */
u8 APS_INTERFRAME_DELAY = 100;
u8 APS_MAX_WINDOW_SIZE = 1;
u8 APS_FRAGMEMT_PAYLOAD_SIZE = 64;

/* queue size of the software timer event */
u8 TIMER_EVENT_SIZE = TIMER_EVENT_NUM;

/* buffer pool size for zigbee pro */
u8 ZB_BUF_POOL_SIZE = ZB_BUF_POOL_NUM;
zb_buf_pool_t g_mPool;

/* for indirect pending queue */
u8 ZB_MAC_PENDING_TRANS_QUEUE_SIZE = ZB_MAC_PENDING_TRANS_QUEUE_NUM;
u8 ZB_MAC_EXT_EXPEIRY_CNT = ZB_MAC_INTERNAL_EXPIRY_CNT;

/* the lqi threshold for neighbor */
u8 NWK_NEIGHBORTBL_ADD_LQITHRESHOLD = 0x5F;

/* the cost threshold for one hop */
u8 NWK_COST_THRESHOLD_ONEHOP = 3;

/* address mapping table */
u16 TL_ZB_NWK_ADDR_MAP_SIZE = TL_ZB_NWK_ADDR_MAP_NUM;
tl_zb_addr_map_t g_nwkAddrMap;

/* neighbor table */
u8 TL_ZB_NEIGHBOR_TABLE_SIZE = TL_ZB_NEIGHBOR_TABLE_NUM;
u8 TL_ZB_CHILD_TABLE_SIZE = TL_ZB_CHILD_TABLE_NUM;
tl_zb_neighbor_entry_t g_zb_neighborTbl;

/* routing table */
#if ZB_ROUTER_ROLE
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
nwk_brcTransRecordEntry_t g_brcTransTab[NWK_BRC_TRANSTBL_NUM];
#endif

/* binding table */
u8 APS_BINDING_TABLE_SIZE = APS_BINDING_TABLE_NUM;
aps_binding_table_t aps_binding_tbl;

/* group table */
u8 APS_GROUP_TABLE_SIZE = APS_GROUP_TABLE_NUM;
aps_group_tbl_ent_t aps_group_tbl[APS_GROUP_TABLE_NUM];

/* APS layer security */
bool APS_SECUTIRY_ENABLE = TRUE;

/* the offset of the rx buffer to the zb-buffer*/
u8 RX_ZBBUF_OFFSET = TL_RXPRIMITIVEHDR;

/* MAC layer TX Queue size */
u8 MAC_TX_QUEUE_SIZE = TX_QUEUE_BN;

//default network key
/* If all zero, will generate 16-bytes network key randomly. */

const u8 nwkKeyDefault[] = {0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0};

/*const u8 nwkKeyDefault[] = { 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89,
			  	  	  	0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, 0x01};
*/


//Global trust center link key which used in centralized security network
const u8 tcLinkKeyCentralDefault[] = {0x5a, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6c,
  	   	 	 	 	 	 	 	 	 	0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65, 0x30, 0x39};


//certification link key for distributed network
const u8 linkKeyDistributedCertification[] = {0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
										0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf};

/* touch-link link key */
const u8 touchLinkKeyCertification[] = {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
        								0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf};

/* default mac PIB setting */
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
	.frameRetryNum = 3,
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


/* default NIB setting for netwrok layer */
const nwk_nib_t nib_default = {
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
 		.linkStatusPeriod = ZB_NWK_LINK_STATUS_PEROID_DEFAULT,
		.routerAgeLimit = 20,
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

#if ZB_ROUTER_ROLE
/*
 * @brief:		get record entry for broadcasting record table
 *
 * @idx:		the index of the broadcasting record table
 *
 * */
nwk_brcTransRecordEntry_t *brcTransRecordEntryGet(u8 idx){
	return &g_brcTransTab[idx];
}


/*
 * @brief:		get the size of the broadcasting record table
 *
 * @idx:
 *
 * */
u32 brcTransRecordTblSizeGet(void){
	return (sizeof(g_brcTransTab));
}
#endif

/*
 * @brief:		get the entry of the mapping table of the binding list
 *
 * @idx:
 *
 * */
boundTblMapList_t *bindTblMapListGet(void){
	return &aps_binding_tbl.BoudList[0];
}


/*
 * @brief:		get the size of the neighbor table
 *
 * @idx:
 *
 * */
u32 neighborTblSizeGet(void){
	return (sizeof(tl_zb_neighbor_entry_t));
}


/*
 * @brief:		get the size of the address mapping table
 *
 * @idx:
 *
 * */
u32 addrMapTblSizeGet(void){
	return (sizeof(tl_zb_addr_map_t));
}
