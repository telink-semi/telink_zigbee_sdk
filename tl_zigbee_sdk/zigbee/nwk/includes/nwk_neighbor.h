/********************************************************************************************************
 * @file	nwk_neighbor.h
 *
 * @brief	This is the header file for nwk_neighbor
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
#ifndef NWK_NEIGHBOR_H
#define NWK_NEIGHBOR_H


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

#define	TRANSFAILURE_CNT_MAX				2


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
}tl_zb_normal_neighbor_entry_t;

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
	bool endDevTimeoutExpiryRun;//12
	tl_zb_normal_neighbor_entry_t  neighborTbl[TL_ZB_NEIGHBOR_TABLE_NUM]; //shall be allocated at the last field in the structure of the tl_zb_neighbor_entry_t
}tl_zb_neighbor_entry_t _attribute_aligned_(4);

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

bool tl_nwkChildrenExist(void);
u8 tl_zbNeighborTableChildrenNumGet(void);

bool tl_zbNeighborTableAccessAllowed(void);

tl_zb_normal_neighbor_entry_t *nwk_neTblGetByExtAddr(addrExt_t extAddr);


#endif /* NWK_NEIGHBOR_H */
