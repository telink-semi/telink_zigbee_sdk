/********************************************************************************************************
 * @file     nwk_addr_map.h
 *
 * @brief	 Network layer address map
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
#ifndef ZB_NWK_ADDR_MAP_H
#define ZB_NWK_ADDR_MAP_H 1

#include "tl_common.h"
#include "../../include/tl_config.h"
#include "nwk.h"


#define ZB_UNKNOWN_SHORT_ADDR (u16)(-1)

#ifndef TL_ZB_NWK_ADDR_MAP_NUM
#if (ZB_ROUTER_ROLE)
	#define TL_ZB_NWK_ADDR_MAP_NUM		128
#else
	#define TL_ZB_NWK_ADDR_MAP_NUM		8
#endif
#endif

typedef struct addrMapping_t{
	struct addrMapping_t *freeNext;
	struct addrMapping_t *activeNext;
	addrExt_t extAddr;

	u16 shortAddr;
	u8 aps_dup_cnt;
	u8 used:1;
	u8 aps_dup_clock:2;
	u8 bind:1;
	u8 resv:4;
}tl_zb_addr_map_entry_t;//size 20

typedef struct{
	tl_zb_addr_map_entry_t *freeHead;
	tl_zb_addr_map_entry_t *activeHead;
	u32 validNum;
	tl_zb_addr_map_entry_t addrMap[TL_ZB_NWK_ADDR_MAP_NUM]; //shall be allocated at the last field in the structure of the tl_zb_addr_map_t
}tl_zb_addr_map_t;


typedef struct{
	u16 		shortAddr;
	addrExt_t 	extAddr;
	u8			depth;
	u8 			rxOnWhileIdle : 1;
	u8 			deviceType : 3;
	u8 			relationship : 3;
	u8 			used : 1;
}zb_addrForNeighbor_t;


typedef struct{
	u16			mask_dstTable;
	addrExt_t 	dstExtAddr;
}zb_addrForBind_t;

extern u16 TL_ZB_NWK_ADDR_MAP_SIZE;
extern tl_zb_addr_map_t g_nwkAddrMap;


#define zb_addressTableNumGet()		g_nwkAddrMap.validNum


zb_nwk_status_t tl_addrByShort(u16 shortAddr, bool create, bool lock, u16 *idx);


u8 zb_address_ieee_by_short(u16 short_addr, addrExt_t ieee_address) ;
s32 tl_addrMapSave2Flash(void *arg);
void tl_addrMappingForBind(u8 bind, u16 idx);

addrExt_t *tl_zbExtAddrPtrByShortAddr(u16 shortAddr);

/* get address entry based on the index of the table */
tl_zb_addr_map_entry_t *tl_zbNwkAddrMapEntryGet(u16 orderIndex);

/* store address information for child table */
void nwk_nodeAddrInfoStore(void *arg);

s32 nwk_nodeAddrInfoDelete(zb_addrForNeighbor_t *pAddrNv);

/* store address information for parent table */
s32 nwk_parentNodeInfoStore(void);

/* store address information for bind table */
s32 nwk_bindAddrInfoUpdate(zb_addrForBind_t *pAddrNv);

s32 nwk_bindAddrInfoDelete(zb_addrForBind_t *pAddrNv);


#endif /* ZB_NWK_NEIGHBOR_H */
