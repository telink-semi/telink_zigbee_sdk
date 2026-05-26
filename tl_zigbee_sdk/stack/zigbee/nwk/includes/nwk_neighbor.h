/********************************************************************************************************
 * @file    nwk_neighbor.h
 *
 * @brief   This is the header file for nwk_neighbor
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
#ifndef NWK_NEIGHBOR_H
#define NWK_NEIGHBOR_H


#if (ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE)
#ifndef TL_ZB_NEIGHBOR_TABLE_NUM
#define TL_ZB_NEIGHBOR_TABLE_NUM                26
#endif
#ifndef TL_ZB_CHILD_TABLE_NUM
#define TL_ZB_CHILD_TABLE_NUM                   25
#endif
#elif ZB_ED_ROLE
#ifndef TL_ZB_NEIGHBOR_TABLE_NUM
#define TL_ZB_NEIGHBOR_TABLE_NUM                2
#endif
#ifndef TL_ZB_CHILD_TABLE_NUM
#define TL_ZB_CHILD_TABLE_NUM                   0
#endif
#endif

#define TL_ZB_NEIGHBOR_TOTAL_NUM                (TL_ZB_NEIGHBOR_TABLE_NUM + TL_ZB_CHILD_TABLE_NUM)

#define TL_ZB_ADDITION_NEIGHBOR_TABLE_SIZE      6

#define	TRANSFAILURE_CNT_MAX                    2

typedef enum {
    ZVD_JOIN_IDLE,
    ZVD_JOIN_INIT_JOIN,
    ZVD_JOIN_INIT_JOIN_EPH,
    ZVD_JOIN_REJOIN,
    ZVD_JOIN_TC_REJOIN,
    ZVD_JOIN_EPH_DONE,
} tl_zvdJoinState_e;

typedef enum {
    LINK_TYPE_ZIGBEE,
    LINK_TYPE_TRUSTED,
} tl_macLinkType_e;

/**
 * @brief Neighbor relationship
 */
typedef enum {
    NEIGHBOR_IS_PARENT,
    NEIGHBOR_IS_CHILD,
    NEIGHBOR_IS_SIBLING,
    NEIGHBOR_IS_NONE_OF_ABOVE,
    NEIGHBOR_IS_PREVIOUS_CHILD,
    NEIGHBOR_IS_UNAUTH_CHILD,
    NEIGHBOR_IS_UNAUTH_CHILD_RELAY_ALLOWED,
    NEIGHBOR_IS_A_LOST_CHILD,
    NEIGHBOR_IS_A_CHILD_ADDR_CONFLICT,
    NEIGHBOR_IS_A_BACKBONE_SIBLING,
} tl_relationship_e;

typedef struct nebTbl_t {
    struct nebTbl_t *freeNext;
    struct nebTbl_t *activeNext;
    u32 authTimeout;
    u32 timeoutCnt;             //this field indicates the current time remaining, in seconds, for the end device.
    u32 devTimeout;             //this field indicates the timeout, in seconds, for the end device child.
    u16 endDevCfg;              //bit mask of the end device's cfg. the default value shall be 0.
    u16 addrmapIdx;
#if ZB_SECURITY
    u32 frameCounterFailCnt;
    u32 receivedFrameCnt;
    u32 incomingFrameCnt;       //incoming frame counter for this device after key change.
    u8 keySeqNum;               //key number for which incoming_frame_counter is valid.
#else
    u8 rsv;
#endif
    bool keepaliveRcvd;         //this value indicates at least one keepalive has been received from the end device since the router has rebooted.
    u8 rxOnWhileIdle:1;
    u8 deviceType:2;            //NWK_DEVICE_TYPE_COORDINATOR = 0,
                                //NWK_DEVICE_TYPE_ROUTER = 1,
                                //NWK_DEVICE_TYPE_ED = 2
    u8 relationship:4;          //NEIGHBOR_IS_PARENT = 0,
                                //NEIGHBOR_IS_CHILD = 1,
                                //NEIGHBOR_IS_SIBLING = 2,
                                //NEIGHBOR_IS_UNAUTH_CHILD = 5
    u8 used:1;
    u8 age;
    u8 depth:7;
    u8 linkType:1;
    u8 transFailure;
    u8 lqi;
    u8 outgoingCost;
} tl_zb_normal_neighbor_entry_t;//36

typedef struct {
    extPANId_t extPanId;
    addrExt_t extAddr;
    u16 shortAddr;
    u16 panId;
    u8 addrMode;
    u8 logicChannel;
    u8 depth:4;
    u8 beaconOrder:4;
    u8 permitJoining:1;
    u8 potentialParent:1;
    u8 routerCapacity:1;
    u8 edCapacity:1;
    u8 stackProfile:4;
    u8 deviceType:2;
    u8 superframeOrder:6;
    s8 rssi;
    u8 lqi;
    u8 nwkUpdateId;
    u8 hubConnectivity:1;
    u8 uptime:1;
    u8 preferredParent:1;
    u8 resv:5;
    u8 beaconTLVsLen;

#if (ZB_STACK_COMPLIANCE_REVISION == STACK_RVE_23)
    u8 beaconAppendixTLVs[127];
#endif
} tl_zb_addition_neighbor_entry_t;//156

typedef struct {
    tl_zb_addition_neighbor_entry_t additionNeighborTbl[TL_ZB_ADDITION_NEIGHBOR_TABLE_SIZE];//936
    tl_zb_normal_neighbor_entry_t *freeHead;
    tl_zb_normal_neighbor_entry_t *activeHead;
    u8 additionNeighborNum;
    u8 normalNeighborNum;
    u8 childrenNum;
    u8 resv;
    /* shall be allocated at the last field in the structure of the tl_zb_neighbor_entry_t */
    tl_zb_normal_neighbor_entry_t neighborTbl[TL_ZB_NEIGHBOR_TOTAL_NUM];
} tl_zb_neighbor_entry_t _attribute_aligned_(4);


#define ZBHCI_CHILD_LIST_NUM_MAX        5

typedef struct {
    addrExt_t extAddr;
    u16 nwkAddr;
} nwk_childNodesList_t;

typedef struct {
    u8 status;                  /*! status, 0: success, 1: failure */
    u8 totalChildNodesNum;      /*! total number of the child nodes */
    u8 startIdx;                /*! start of the child table index */
    u8 childNodesNum;           /*! number of the child nodes in this response packet */
} nwk_childNodesInfo_t;

typedef struct {
    nwk_childNodesInfo_t info;                                  /*! child table info */
    nwk_childNodesList_t list[ZBHCI_CHILD_LIST_NUM_MAX];        /*! child list */
} nwk_childTableInfo_t;

extern u8 NWK_NEIGHBORTBL_ADD_LQITHRESHOLD;
extern u32 NWK_UNAUTH_CHILD_TABLE_LIFE_TIME;

extern u8 TL_ZB_NEIGHBOR_TABLE_SIZE;
extern u8 TL_ZB_CHILD_TABLE_SIZE;
extern tl_zb_neighbor_entry_t g_zb_neighborTbl;

void tl_zbNeighborTableInit(void);

tl_zb_normal_neighbor_entry_t *tl_zbNeighborTableUpdate(tl_zb_normal_neighbor_entry_t *entry, u8 delete);

tl_zb_normal_neighbor_entry_t *tl_zbNeighborTableSearchFromExtAddr(u16 *shortAddr, addrExt_t extAddr, u16 *idx);

tl_zb_normal_neighbor_entry_t *tl_zbNeighborTableSearchFromShortAddr(u16 shortAddr, addrExt_t extAddr, u16 *idx);

void tl_zbNeighborTableDelete(tl_zb_normal_neighbor_entry_t *entry);

tl_zb_normal_neighbor_entry_t *tl_zbNeighborTableSearchForParent(void);

u16 tl_zbNeighborParentShortAddrGet(void);

tl_zb_normal_neighbor_entry_t *tl_zbNeighborEntryGetFromIdx(u8 idx);

u8 tl_nwkNeighborDeleteByAddrmapIdx(u16 idx);

tl_zb_normal_neighbor_entry_t *nwk_neTblGetByShortAddr(u16 shortAddr);

tl_zb_addition_neighbor_entry_t *tl_zbAdditionNeighborEntryGetFromIdx(u8 idx);
u8 tl_neighborFrameCntReset(void);
u8 tl_zbAdditionNeighborTableNumGet(void);

void tl_zbAdditionNeighborReset(void);

u8 tl_zbAdditionNeighborTableUpdate(tl_zb_addition_neighbor_entry_t *entry);

u8 tl_zbNeighborTableNumGet(void);

u8 tl_zbNeighborTableRouterValidNumGet(void);

u8 tl_zbNeighborTableChildEDNumGet(void);
u8 tl_zbNeighborTableRouterNumGet(void);

tl_zb_normal_neighbor_entry_t *nwk_neTblGetByExtAddr(addrExt_t extAddr);

u8 nwk_neTblLinkTypeNodeCount(void);
u8 tl_zbNeighborTableRouterNumGet(void);

/**
 * @fn     tl_allChildNodesRemove
 *
 * @brief  remove all the child nodes
 *
 * @param  none
 *
 * @return none
 */
void tl_allChildNodesRemove(void);

/**
 * @fn     tl_childNodesListGet
 *
 * @brief  get the child nodes list
 *
 * @param  startIdx - the start index
 * @param  t        - the pointer to the child nodes list
 *
 * @return none
 */
void tl_childNodesListGet(u8 startIdx, nwk_childTableInfo_t *t);


#endif /* NWK_NEIGHBOR_H */
