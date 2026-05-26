/********************************************************************************************************
 * @file    zb_common.h
 *
 * @brief   This is the header file for zb_common
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
#ifndef ZB_COMMON_H
#define ZB_COMMON_H

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "tlstk_common.h"

#include "zb_config.h"
//MAC layer
#include "mac_hook.h"

#include "../../ss/security_service.h"
#include "../../nwk/includes/nwk_addr_map.h"
#include "../../nwk/includes/nwk_neighbor.h"
#include "../../nwk/includes/nwk.h"
#include "../../nwk/includes/nwk_nib.h"
#include "../../nwk/includes/nwk_ctx.h"
#include "../../aps/aps_api.h"
#include "../../af/zb_af.h"
#include "../../zdo/zdp.h"
#include "../../zdo/zdo_api.h"
#include "../../zbapi/zb_api.h"
#include "../../bdb/includes/bdb.h"
#include "../../zcl/zll_commissioning/zcl_touchlink_attr.h"
#include "../../gp/cGP_stub.h"
#include "../../gp/dGP_stub.h"

/**********************************************************************
 * TYPEDEFS
 */
//for .bin map
#if 0
#define _CODE_MAC_ __attribute__((section(".sect_mac")))
#define _CODE_NWK_ __attribute__((section(".sect_nwk")))
#define _CODE_APS_ __attribute__((section(".sect_aps")))
#define _CODE_AF_  __attribute__((section(".sect_af")))
#define _CODE_ZDO_ __attribute__((section(".sect_zdo")))
#define _CODE_SS_  __attribute__((section(".sect_ss")))
#define _CODE_BDB_ __attribute__((section(".sect_bdb")))
#define _CODE_ZCL_ __attribute__((section(".sect_zcl")))
#else
#define _CODE_MAC_
#define _CODE_NWK_
#define _CODE_APS_
#define _CODE_AF_
#define _CODE_ZDO_
#define _CODE_SS_
#define _CODE_BDB_
#define _CODE_ZCL_
#endif

typedef struct {
    tl_zb_mac_pib_t macPib; //76B
    nwk_nib_t nwkNib; //60B or 48B
    touchlink_attr_t touchlinkAttr; //8B
    bdb_attr_t bdbAttr; //48B
} zb_info_t;

extern zb_info_t g_zbInfo;

#define g_zbMacPib              g_zbInfo.macPib
#define g_zbNIB                 g_zbInfo.nwkNib
#define g_touchlinkAttr         g_zbInfo.touchlinkAttr
#define g_bdbAttrs              g_zbInfo.bdbAttr

/* Default settings. */
extern const tl_zb_mac_pib_t macPibDefault;
extern const nwk_nib_t nwkNibDefault;
extern const zdo_attrCfg_t zdoCfgAttrDefault;

extern const u8 g_null_securityKey[];
extern const u8 nwkKeyDefault[];
extern const u8 tcLinkKeyCentralDefault[];         /* Default global tc link key */
extern const u8 apsWellKnownPSK[];                 /* APS well-known PSK */
extern const u8 linkKeyDistributedCertification[]; /* Certification distribute link key */
extern const u8 linkKeyDistributedMaster[];
extern const u8 touchLinkKeyCertification[];       /* Certification touch-link key */
extern const u8 touchLinkKeyMaster[];

extern u8 zvdJoinState;

/* diagnostics for stack */
typedef struct {
    u16 numberOfResets;
    u16 persistentMemoryWrites;

    mac_diagnostics_t mac_diags;

    u16 nwkTxCnt;
    u16 nwkTxEnDecryptFail;

    u16 apsRxBcast;
    u16 apsTxBcast;
    u16 apsRxUcast;
    u16 apsTxUcastSuccess;
    u16 apsTxUcastRetry;
    u16 apsTxUcastFail;

    u16 routeDiscInitiated;
    u16 neighborAdded;
    u16 neighborRemoved;
    u16 neighborStale;
    u16 joinIndication;
    u16 childMoved;

    u32 panIdConflictCheck;

    u16 nwkFCFailure;
    u16 apsFCFailure;
    u16 apsUnauthorizedKey;
    u16 nwkDecryptFailures;
    u16 apsDecryptFailures;
    u16 packetBufferAllocateFailures;
    u16 relayedUcast;
    u16 packetValidateDropCount;

    u8 lastMessageLQI;
    s8 lastMessageRSSI;
    u8 panIdConflict;
    u8 panIdModified;
    u8 nwkAddrConflict;
} sys_diagnostics_t;

extern sys_diagnostics_t g_sysDiags;

u8 zb_info_load(void);
void zb_info_save(void *arg);

void zb_reset(void);
void zb_init(void);

void tl_zbTaskInit(bool reset);
void tl_zbTaskProcedure(void);

#if (ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE)
u32 brcTransRecordTblSizeGet(void);
nwk_brcTransRecordEntry_t *brcTransRecordEntryGet(u8 idx);
#endif /* ZB_COORDINATOR_ROLE || ZB_ROUTER_ROLE */
u32 zbBufferSizeGet(void);
u32 neighborTblSizeGet(void);
u32 addrMapTblSizeGet(void);
aps_binding_entry_t *bindTblEntryGet(void);

#endif /* ZB_COMMON_H */
