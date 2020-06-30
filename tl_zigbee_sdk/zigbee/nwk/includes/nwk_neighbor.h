/********************************************************************************************************
 * @file     nwk_neighbor.h
 *
 * @brief	 Network layer neighbor table (base and additional)
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
#ifndef ZB_NWK_NEIGHBOR_H
#define ZB_NWK_NEIGHBOR_H 1

#include "tl_common.h"


/**
    Node Relations
 */
#define NEIGHBOR_IS_PARENT              	0
#define NEIGHBOR_IS_CHILD               	1
#define NEIGHBOR_IS_SIBLING             	2
#define NEIGHBOR_IS_NONE_OF_ABOVE       	3
#define NEIGHBOR_IS_PREVIOUS_CHILD      	4
#define NEIGHBOR_IS_UNAUTH_CHILD        	5

#ifndef TL_ZB_NEIGHBOR_TABLE_NUM
#if ZB_ROUTER_ROLE
#define TL_ZB_NEIGHBOR_TABLE_NUM			26
#define TL_ZB_CHILD_TABLE_NUM				16
#else
#define TL_ZB_NEIGHBOR_TABLE_NUM			2
#define TL_ZB_CHILD_TABLE_NUM				0
#endif
#endif

#define TL_ZB_ADDITION_NEIGHBOR_TABLE_SIZE  6

#define	TRANSFAILURE_CNT_MAX		3

extern u8 NWK_NEIGHBORTBL_ADD_LQITHRESHOLD;

extern u32 NWK_UNAUTH_CHILD_TABLE_LIFE_TIME;

/***************************************************************************
* @brief	NWK static path cost
*/
#define NWK_STATIC_PATH_COST 7

typedef struct nebTbl_t{
	struct nebTbl_t *freeNext;
	struct nebTbl_t *activeNext;
	u32 incommingBeaconTimestamp;
	u32 beaconTransOffset;
	u32 timeoutCnt;			//this field indicates the current time remaining, in seconds, for the end device.

	u32 devTimeout;			//this field indicates the timeout, in seconds, for the end device child.
	u16 endDevCfg;			//Bitmask. the end device's cfg. the default value shall be 0.
	u16 addrmapIdx;
#ifdef ZB_SECURITY
	u32 incomingFrameCnt; /*!< incoming frame counter
                                                     * for this device after
                                                     * key change */
	u8 keySeqNum; /*!< key number for which
		                                            * incoming_frame_counter is valid  */
#else
	u8	 rsv;
#endif
	bool keepaliveRcvd;		//this value indicates at least one keepalive has been received from the end device since the router has rebooted.
	u8 rxOnWhileIdle : 1;
	u8 deviceType : 3;
	u8 relationship : 3;
	u8 used : 1;
	u8 age;
	u8 depth;
	u8 transFailure;
	u8 lqi;
	u8 outgoingCost;
}tl_zb_normal_neighbor_entry_t;//size 32 or 28

typedef struct{
	extPANId_t 		extPanId;
	addrExt_t 		extAddr;
	u16 			shortAddr;
	u16 			panId;
	u8				addrMode;
	u8 				logicChannel;
	u8 				depth : 4;
	u8 				beaconOrder : 4;
	u8 				permitJoining : 1;
	u8 				potentialParent : 1;
	u8 				routerCapacity : 1;
	u8 				edCapacity : 1;
	u8 				stackProfile : 4;
	u8 				deviceType : 2;
	u8 				superframeOrder : 6;

	u8 				lqi;
	u8 				nwkUpdateId;
	u8				rsv;
}tl_zb_addition_neighbor_entry_t;

typedef struct{
	tl_zb_addition_neighbor_entry_t  additionNeighborTbl[TL_ZB_ADDITION_NEIGHBOR_TABLE_SIZE];//168
	tl_zb_normal_neighbor_entry_t *freeHead;
	tl_zb_normal_neighbor_entry_t *activeHead;
	u8 additionNeighborNum;
	u8 normalNeighborNum;
	u8 childrenNum;
	bool endDevTimeoutExpiryRun;
	tl_zb_normal_neighbor_entry_t  neighborTbl[TL_ZB_NEIGHBOR_TABLE_NUM]; //shall be allocated at the last field in the structure of the tl_zb_neighbor_entry_t
}tl_zb_neighbor_entry_t _attribute_aligned_(4);


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

s32 tl_neighborTblSave2Flash(void *arg);

u8 tl_zbNeighborTableNumGet(void);

u8 tl_zbNeighborTableRouterValidNumGet(void);

u8 tl_zbNeighborTableChildEDNumGet(void);

bool tl_nwkChildrenExist(void );
u8 tl_zbNeighborTableChildrenNumGet(void);

bool tl_zbNeighborTableAccessAllowed(void);

tl_zb_normal_neighbor_entry_t *nwk_neTblGetByExtAddr(addrExt_t extAddr);


#endif /* ZB_NWK_NEIGHBOR_H */
