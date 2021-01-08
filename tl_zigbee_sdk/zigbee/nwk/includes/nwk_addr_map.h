/********************************************************************************************************
 * @file	nwk_addr_map.h
 *
 * @brief	This is the header file for nwk_addr_map
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
#ifndef NWK_ADDR_MAP_H
#define NWK_ADDR_MAP_H



#define ZB_UNKNOWN_SHORT_ADDR 			(u16)(-1)

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
	u16 shortAddr;
	addrExt_t 	extAddr;
	u8	depth;
	u8 	rxOnWhileIdle:1;
	u8 	deviceType:3;
	u8 	relationship:3;
	u8 	used:1;
}zb_addrForNeighbor_t;

typedef struct{
	u16			mask_dstTable;
	addrExt_t 	dstExtAddr;
}zb_addrForBind_t;

extern u16 TL_ZB_NWK_ADDR_MAP_SIZE;
extern tl_zb_addr_map_t g_nwkAddrMap;


#define zb_addressTableNumGet()			(g_nwkAddrMap.validNum)





u8 zb_address_ieee_by_short(u16 short_addr, addrExt_t ieee_address) ;

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


#endif /* NWK_ADDR_MAP_H */
