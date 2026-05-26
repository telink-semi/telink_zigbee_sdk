/********************************************************************************************************
 * @file    nwk_addr_map.h
 *
 * @brief   This is the header file for nwk_addr_map
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
#ifndef NWK_ADDR_MAP_H
#define NWK_ADDR_MAP_H


#define ZB_UNKNOWN_SHORT_ADDR           (u16)(-1)

#if (ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE)
#ifndef TL_ZB_NWK_ADDR_MAP_NUM
#define TL_ZB_NWK_ADDR_MAP_NUM          128
#endif
#elif ZB_ED_ROLE
#ifndef TL_ZB_NWK_ADDR_MAP_NUM
#define TL_ZB_NWK_ADDR_MAP_NUM          8
#endif
#endif

typedef struct addrMapping_t {
    struct addrMapping_t *freeNext;
    struct addrMapping_t *activeNext;
    addrExt_t extAddr;
    u16 shortAddr;
    u8 aps_dup_cnt;
    u8 used:1;
    u8 aps_dup_clock:3;
    u8 bind:1;
    u8 verifiedFrameCounter:1;
    u8 linkType:1;    //0-zigbee link, 1-trust link
    u8 resv:1;
    u32 incomingFrameCounter;
} tl_zb_addr_map_entry_t;//size 24

typedef struct {
    tl_zb_addr_map_entry_t *freeHead;
    tl_zb_addr_map_entry_t *activeHead;
    u32 validNum;
    tl_zb_addr_map_entry_t addrMap[TL_ZB_NWK_ADDR_MAP_NUM]; //shall be allocated at the last field in the structure of the tl_zb_addr_map_t
} tl_zb_addr_map_t;

typedef struct _attribute_packed_ {
    u16 shortAddr;
    addrExt_t extAddr;
    u8 depth:7;
    u8 linkType:1;
    u8 rxOnWhileIdle:1;
    u8 deviceType:2;
    u8 relationship_hb:1;
    u8 relationship:3;
    u8 used:1;
} zb_addrForNeighbor_t;

typedef struct _attribute_packed_ {
    u16 mask_dstTable;
    addrExt_t dstExtAddr;
} zb_addrForBind_t;

typedef struct _attribute_packed_ {
    u32 devTimeout;
    addrExt_t extAddr;
} zb_addrForEDTimeout_t;

extern u16 TL_ZB_NWK_ADDR_MAP_SIZE;
extern tl_zb_addr_map_t g_nwkAddrMap;

#define zb_addressTableNumGet()         (g_nwkAddrMap.validNum)


u8 zb_address_ieee_by_short(u16 short_addr, addrExt_t ieee_address);

void tl_addrMappingForBind(u8 bind, u16 idx);

addrExt_t *tl_zbExtAddrPtrByShortAddr(u16 shortAddr);

/* get address entry based on the extAddr */
tl_zb_addr_map_entry_t *tl_zbNwkAddrMapEntryGet(addrExt_t extAddr);

/* store address information for child table */
void nwk_nodeAddrInfoStore(void *arg);

s32 nwk_nodeAddrInfoDelete(zb_addrForNeighbor_t *pAddrNv);

/* store address information for parent table */
s32 nwk_parentNodeInfoStore(void);

/* store address information for bind table */
s32 nwk_bindAddrInfoUpdate(zb_addrForBind_t *pAddrNv);

s32 nwk_bindAddrInfoDelete(zb_addrForBind_t *pAddrNv);

#endif /* NWK_ADDR_MAP_H */
