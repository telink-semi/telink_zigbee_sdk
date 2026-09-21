/********************************************************************************************************
 * @file    drv_nv.h
 *
 * @brief   This is the header file for drv_nv
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
#pragma once

#include "drv_nv_cfg.h"

#define NV_SECT_INFO_CHECK_BITS         (6)
#define NV_SECT_INFO_CHECK_BITMASK      (0x3F)
#define NV_SECT_INFO_SECTNO_BITS        (8 - NV_SECT_INFO_CHECK_BITS)
#define NV_SECT_INFO_SECTNO_BITMASK     (0x03)

/* sector info(4 bytes) + index info(8 bytes) + index info(8 bytes) + ... */
typedef struct {
    u16 usedFlag;
    u8 idName;
    u8 opSect; // crcCheckBit(6bits) + opSect(2bits)
} nv_sect_info_t;

typedef struct {
    u32 offset;
    u16 size;
    u8 itemId;
    u8 usedState;
#if defined(MCU_CORE_B91) || defined(MCU_CORE_TL721X) || \
    defined(MCU_CORE_TL321X) || defined(MCU_CORE_TL323X) || \
    defined(MCU_CORE_TL521X)
    u8 resv[8]; // PUYA flash only supports re-write 64 times
#endif
} nv_info_idx_t;

/* item: item_hdr(8 bytes) + payload */
typedef struct {
    u32 checkSum;
    u16 size;
    u8 itemId;
    u8 used;
} itemHdr_t;

typedef struct _attribute_packed_ {
    u16 opIndex;
    u8 opSect;
} itemIfno_t;

/*****************************************************************************************************************************
 * Store zigbee information in flash.
 *   Module ID               |          1M Flash          |          2M Flash          |          4M Flash          |
 * --------------------------|----------------------------|----------------------------|----------------------------|
 * NV_MODULE_ZB_INFO         |      0xE6000 - 0xE7FFF     |     0x1E6000 - 0x1E7FFF    |     0x3E6000 - 0x1E7FFF    |
 * NV_MODULE_ADDRESS_TABLE   |      0xE8000 - 0xE9FFF     |     0x1E8000 - 0x1E9FFF    |     0x3E8000 - 0x1E9FFF    |
 * NV_MODULE_APS             |      0xEA000 - 0xEBFFF     |     0x1EA000 - 0x1EBFFF    |     0x3EA000 - 0x1EBFFF    |
 * NV_MODULE_ZCL             |      0xEC000 - 0xEDFFF     |     0x1EC000 - 0x1EDFFF    |     0x3EC000 - 0x1EDFFF    |
 * NV_MODULE_NWK_FRAME_COUNT |      0xEE000 - 0xEFFFF     |     0x1EE000 - 0x1EFFFF    |     0x3EE000 - 0x1EFFFF    |
 * NV_MODULE_OTA             |      0xF0000 - 0xF1FFF     |     0x1F0000 - 0x1F1FFF    |     0x3F0000 - 0x1F1FFF    |
 * NV_MODULE_APP             |      0xF2000 - 0xF3FFF     |     0x1F2000 - 0x1F3FFF    |     0x3F2000 - 0x1F3FFF    |
 * NV_MODULE_KEYPAIR         |      0xF4000 - 0xFBFFF     |     0x1F4000 - 0x1FBFFF    |     0x3F4000 - 0x1FBFFF    |
 *                           | *16K - can store 127 nodes | *32K - can store 302 nodes | *32K - can store 302 nodes |
 * NV_MAX_MODULS
 */
typedef enum {
    NV_MODULE_ZB_INFO                   = 0, /* mustn't modify it */
    NV_MODULE_ADDRESS_TABLE             = 1, /* mustn't modify it */
    NV_MODULE_APS                       = 2, /* mustn't modify it */
    NV_MODULE_ZCL                       = 3, /* mustn't modify it */
    NV_MODULE_NWK_FRAME_COUNT           = 4, /* mustn't modify it */
    NV_MODULE_OTA                       = 5,
    NV_MODULE_APP                       = 6,
    NV_MODULE_KEYPAIR                   = 7,
    NV_MAX_MODULES
} nv_module_t;

typedef enum {
    NV_ITEM_ID_INVALID                  = 0, /* Item id 0 should not be used. */

    NV_ITEM_ZB_INFO                     = 1, /* mustn't modify it */
    NV_ITEM_ADDRESS_FOR_NEIGHBOR,            /* mustn't modify it */
    NV_ITEM_ADDRESS_FOR_BIND,                /* mustn't modify it */
    NV_ITEM_APS_SSIB,                        /* mustn't modify it */
    NV_ITEM_APS_GROUP_TABLE,                 /* mustn't modify it */
    NV_ITEM_APS_BINDING_TABLE,               /* mustn't modify it */

    NV_ITEM_NWK_FRAME_COUNT,                 /* mustn't modify it */

    NV_ITEM_SS_KEY_PAIR,                     /* mustn't modify it */

    NV_ITEM_OTA_HDR_SERVERINFO,
    NV_ITEM_OTA_CODE,

    NV_ITEM_ED_TIMEOUT,

    NV_ITEM_SS_KEY_PAIR_ADD,

    NV_ITEM_ZCL_REPORT                  = 0x20,
    NV_ITEM_ZCL_ON_OFF,
    NV_ITEM_ZCL_LEVEL,
    NV_ITEM_ZCL_COLOR_CTRL,
    NV_ITEM_ZCL_SCENE_TABLE,
    NV_ITEM_ZCL_IAS_ZONE,
    NV_ITEM_ZCL_GP_PROXY_TABLE,
    NV_ITEM_ZCL_GP_SINK_TABLE,
    NV_ITEM_ZCL_WWAH_APS_AUTH_INFO,
    NV_ITEM_ZCL_WWAH_APS_ACKS_INFO,
    NV_ITEM_ZCL_WWAH_USE_TC_CLUSTER_INFO,
    NV_ITEM_ZCL_ZBD_INF_STATE_INFO,
    NV_ITEM_ZCL_ZBD_ANJOIN_TIMEOUT_INFO,

    NV_ITEM_APP_SIMPLE_DESC,
    NV_ITEM_APP_POWER_CNT,
    NV_ITEM_APP_GP_TRANS_TABLE,
    NV_ITEM_APP_ZBD_ADMIN_KEY,

    NV_ITEM_APS_BINDING_TABLE_V2        = 0x80, /* mustn't modify it */

    NV_ITEM_ID_MAX                      = 0xFF, /* Item id 0xFF should not be used. */
} nv_item_t;

typedef enum {
    NV_SUCC,
    NV_INVALID_MODULS                   = 1,
    NV_INVALID_ID,
    NV_ITEM_NOT_FOUND,
    NV_NOT_ENOUGH_SAPCE,
    NV_ITEM_LEN_NOT_MATCH,
    NV_CHECK_SUM_ERROR,
    NV_ENABLE_PROTECT_ERROR,
    NV_NO_MEDIA,
    NV_DATA_CHECK_ERROR,
    NV_ITEM_CHECK_ERROR,
    NV_MODULE_NOT_FOUND,
    NV_MODULE_ERASE_NEED,
    NV_NOT_PERMIT
} nv_sts_t;


#define FLASH_PAGE_SIZE                         256
#define	FLASH_SECTOR_SIZE                       4096//4K

#define NV_SECTOR_VALID                         0x5A5A
#define NV_SECTOR_VALID_CHECKCRC                0x7A7A
#define NV_SECTOR_VALID_READY_CHECKCRC          0xFAFA
#define NV_SECTOR_INVALID                       0x5050
#define NV_SECTOR_IDLE                          0xFFFF

#define ITEM_FIELD_VALID                        0x5A
#define ITEM_FIELD_VALID_SINGLE                 0x7A
#define ITEM_FIELD_INVALID                      0x50
#define ITEM_FIELD_OPERATION                    0xFA
#define ITEM_FIELD_IDLE                         0xFF

#define ITEM_HDR_FIELD_VALID_CHECKSUM           0x5A
#define ITEM_HDR_FIELD_VALID_CHECKCRC           0x7A

#define MODULES_START_ADDR(id)                  (NV_BASE_ADDRESS_ZB + FLASH_SECTOR_SIZE * (2 * id))
#define NV_SECTOR_SIZE(id)                      ((id == NV_MODULE_KEYPAIR) ? (4 * FLASH_SECTOR_SIZE) : FLASH_SECTOR_SIZE)
#define MODULE_INFO_SIZE(id)                    ((id == NV_MODULE_OTA || id == NV_MODULE_KEYPAIR || id == NV_MODULE_ADDRESS_TABLE) ? \
                                                ((id == NV_MODULE_KEYPAIR) ? (12 * FLASH_PAGE_SIZE) : (4 * FLASH_PAGE_SIZE)) : (2 * FLASH_PAGE_SIZE))

#define MODULE_SECTOR_NUM                       (2)

#define MODULE_IDX_SIZE(id)                     (MODULE_INFO_SIZE(id) - sizeof(nv_sect_info_t))
#define MODULE_SECT_START(id, sectNo)           (MODULES_START_ADDR(id) + sectNo * (NV_SECTOR_SIZE(id)))
#define MODULE_SECT_END(id, sectNo)             (MODULES_START_ADDR(id) + (sectNo + 1) * (NV_SECTOR_SIZE(id)))
#define MODULE_IDX_START(id, sectNo)            (MODULE_SECT_START(id, sectNo) + sizeof(nv_sect_info_t))
#define MODULE_IDX_NUM(id)                      (MODULE_IDX_SIZE(id) / (sizeof(nv_info_idx_t)))
#define MODULE_CONTEXT_START(id, sectNo, len)  	(MODULE_SECT_START(id, sectNo) + MODULE_INFO_SIZE(id))
#define ITEM_TOTAL_LEN(len)                     (len + sizeof(itemHdr_t))
#define FRAMECOUNT_PAYLOAD_START(opSect)        (((MODULE_SECT_START(NV_MODULE_NWK_FRAME_COUNT, opSect) + sizeof(nv_sect_info_t)) + 0x03) & (~0x03))
#define FRAMECOUNT_NUM_PER_SECT()               ((((FLASH_SECTOR_SIZE - sizeof(nv_sect_info_t)) & (~0x03))) / 4)


nv_sts_t nv_resetAll(void);
nv_sts_t nv_resetModule(u8 modules);
nv_sts_t nv_flashWriteNew(u8 single, u16 id, u8 itemId, u16 len, u8 *buf);
nv_sts_t nv_flashReadNew(u8 single, u8 id, u8 itemId, u16 len, u8 *buf);
nv_sts_t nv_itemDeleteByIndex(u8 id, u8 itemId, u8 opSect, u16 opIdx);
nv_sts_t nv_flashSingleItemRemove(u8 id, u8 itemId, u16 len);
nv_sts_t nv_flashReadByIndex(u8 id, u8 itemId, u8 opSect, u16 opIdx, u16 len, u8 *buf);
void nv_itemLengthCheckAdd(u8 itemId, u16 len);
nv_sts_t nv_resetToFactoryNew(void);
bool nv_facrotyNewRstFlagCheck(void);
void nv_facrotyNewRstFlagSet(void);
void nv_facrotyNewRstFlagClear(void);
nv_sts_t nv_nwkFrameCountSaveToFlash(u32 frameCount);
nv_sts_t nv_nwkFrameCountFromFlash(u32 *frameCount);
nv_sts_t nv_flashSingleItemSizeGet(u8 id, u8 itemId, u16 *len);
