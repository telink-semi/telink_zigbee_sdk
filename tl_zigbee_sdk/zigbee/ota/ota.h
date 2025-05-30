/********************************************************************************************************
 * @file    ota.h
 *
 * @brief   This is the header file for ota
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
#ifndef OTA_H
#define OTA_H


#define	OTA_IMAGESECTORHDR_SIZE                 6
#define OTA_IMAGE_MAX_DATA_SIZE                 48

#define OTA_HDR_LEN_OFFSET                      6
#define OTA_STACK_VER_OFFSET                    18

#define OTA_UPGRADE_FILE_ID                     0x0BEEF11E

#define OTA_UPGRADE_IMAGE_TAG_ID                0x0000//Upgrade Image
#define OTA_UPGRADE_IMAGE_AES_TAG_ID            0xF000//Upgrade Image with AES, Manufacturer Specific Use

#define OTA_QUERY_START_JITTER                  (5 * 1000)//ms
#define OTA_PERIODIC_QUERY_INTERVAL             (5 * 60)//s
#define OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME       5//s
#define OTA_IEEE_ADDR_RSP_WAIT_TIME             2//s

#define OTA_IMAGE_BLOCK_FC                      BLOCK_FC_BITMASK_MIN_PERIOD_PRESENT//BLOCK_FC_BITMASK_GENERIC


#define OTA_MAX_IMAGE_BLOCK_RETRIES             10
#define OTA_MAX_UPGRADE_END_REQ_RETRIES         2
#define OTA_IEEE_ADDR_REQ_RETRIES               3

typedef enum {
    OTA_TYPE_CLIENT,
    OTA_TYPE_SERVER
} ota_type_e;

typedef enum {
    ZB_STACKVER_2006                            = 0x0000,
    ZB_STACKVER_2007                            = 0x0001,
    ZB_STACKVER_PRO                             = 0x0002,
    ZB_STACKVER_IP                              = 0x0003
} zb_stackVer_e;



typedef struct _attribute_packed_ {
    /*The value is a unique 4-byte value that is included at the beginning of all ZigBee OTA upgrade image
    files in order to quickly identify and distinguish the file as being a ZigBee OTA cluster upgrade file,
    without having to examine the whole file content. This helps distinguishing the file from other file
    types on disk. The value is defined to be "0x0BEEF11E".*/
    u32 otaUpgradeFileID;

    /*The current OTA header version shall be 0x0100 with major version of "01" and minor version of "00".*/
    u16 otaHdrVer;

    /*This value indicates full length of the OTA header in bytes, including the OTA upgrade file identifier,
    OTA header length itself to any optional fields.*/
    u16 otaHdrLen;
    //The bit mask indicates whether additional information such as Image Signature or Signing Certificate
    //are included as part of the OTA Upgrade Image.
    u16 otaHdrFC;

    u16 manufacturerCode;

    /*The manufacturer should assign an appropriate and unique image type value to each of its devices in
    651 order to distinguish the products.*/
    u16 imageType;

    //
    u32 fileVer;
    u16 zbStackVer;
    //TLNK
    u8 otaHdrString[32];
    //The value represents the total image size in bytes.
    u32 totalImageSize;

    u8 securityCredVer;
    addrExt_t fileDest;
    u16 minHdrVer;
    u16 maxHdrVer;
} ota_hdrFields_t; //69-bytes

typedef struct {
    u32 fileVer;
    //The value represents the total image size in bytes.
    u32 totalImageSize;
    u16 imageType;
    u16 manufacturerCode;
} ota_preamble_t;



typedef enum {
    OTA_FLAG_INIT_DONE,
    OTA_FLAG_IMAGE_PULL_READY,
    OTA_FLAG_IMAGE_PULL_CONTINUE,

    OTA_FLAG_IMAGE_MAGIC_0,
    OTA_FLAG_IMAGE_MAGIC_1,
    OTA_FLAG_IMAGE_MAGIC_2,
    OTA_FLAG_IMAGE_MAGIC_3,
    OTA_FLAG_IMAGE_HDR_VER1,
    OTA_FLAG_IMAGE_HDR_VER2,
    OTA_FLAG_IMAGE_HDR_LEN1,
    OTA_FLAG_IMAGE_HDR_LEN2,
    OTA_FLAG_IMAGE_HDR_FC1,
    OTA_FLAG_IMAGE_HDR_FC2,
    OTA_FLAG_IMAGE_MANU_CODE1,
    OTA_FLAG_IMAGE_MANU_CODE2,
    OTA_FLAG_IMAGE_TYPE1,
    OTA_FLAG_IMAGE_TYPE2,
    OTA_FLAG_IMAGE_FILE_VER1,
    OTA_FLAG_IMAGE_FILE_VER2,
    OTA_FLAG_IMAGE_FILE_VER3,
    OTA_FLAG_IMAGE_FILE_VER4,
    OTA_FLAG_IMAGE_STACK_VER1,
    OTA_FLAG_IMAGE_STACK_VER2,
    OTA_FLAG_IMAGE_CONT_HDR,
    OTA_FLAG_IMAGE_ELEM_TAG1,
    OTA_FLAG_IMAGE_ELEM_TAG2,
    OTA_FLAG_IMAGE_ELEM_LEN1,
    OTA_FLAG_IMAGE_ELEM_LEN2,
    OTA_FLAG_IMAGE_ELEM_LEN3,
    OTA_FLAG_IMAGE_ELEM_LEN4,
    OTA_FLAG_IMAGE_ELEM_INFO1,//manufacturer specific use
    OTA_FLAG_IMAGE_ELEM_INFO2,//manufacturer specific use
    OTA_FLAG_IMAGE_ELEMENT,
} otaFlg_t;

typedef struct {
    af_simple_descriptor_t *simpleDesc;
    ota_preamble_t *pOtaPreamble;//run code bin firmware info
    u32 downloadImageSize;
    epInfo_t otaServerEpInfo;//server endPoint information
    u8 imageBlockRetry;
    u8 upgradeEndRetry;
    bool isOtaServer;
} ota_ctx_t;

typedef struct _attribute_packed_ {
    u32 offset;
    u32 crcValue;
    u32 otaElementPos;
    u32 otaElementLen;
    u16 otaElementTag;
    u8 otaElementInfo1;//manufacturer specific use
    u8 otaElementInfo2;//manufacturer specific use, fillNum
    u8 clientOtaFlg;
} ota_clientInfo_t;

typedef enum {
    OTA_EVT_START,
    OTA_EVT_IMAGE_DONE,
    OTA_EVT_COMPLETE,
} ota_evt_t;

typedef void (*ota_processMsg_t)(u8 evt, u8 status);

typedef struct {
    ota_processMsg_t processMsgCbFunc;
} ota_callBack_t;



extern addrExt_t zcl_attr_upgradeServerID;
extern u32 zcl_attr_fileOffset;
extern u32 zcl_attr_currFileVer;
extern u16 zcl_attr_currZigbeeStackVer;
extern u32 zcl_attr_downloadFileVer;
extern u16 zcl_attr_downloadZigbeeStackVer;
extern u8 zcl_attr_imageUpgradeStatus;
extern u16 zcl_attr_manufacturerID;
extern u16 zcl_attr_imageTypeID;
extern u16 zcl_attr_minBlockPeriod;//in seconds
//extern u32 zcl_attr_imageStamp;

extern const zcl_specClusterInfo_t g_otaClusterList[];
extern u8 OTA_CB_CLUSTER_NUM;


extern bool g_otaEncryptionNeeded;
#define OTA_IMAGE_UNENCRYPTION_SUPPORT()    g_otaEncryptionNeeded = 0
#define OTA_IMAGE_UNENCRYPTION_REJECT()     g_otaEncryptionNeeded = 1


status_t zcl_otaCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

void ota_wwah_useTrustCenter(u8 endpoint);

u8 ota_loadImageInfo(ota_hdrFields_t *oh);
void ota_init(ota_type_e type, af_simple_descriptor_t *simpleDesc, ota_preamble_t *otaPreamble, ota_callBack_t *cb);
void ota_queryStart(u16 seconds);
void ota_serverAddrPerprogrammed(addrExt_t ieeeAddr, u8 srvEndPoint);
void ota_mcuReboot(void);
void ota_upgradeAbort(void);
u32 mcuBootAddrGet(void);
bool ota_newImageValid(u32 new_image_addr);

#endif	/* OTA_H */
