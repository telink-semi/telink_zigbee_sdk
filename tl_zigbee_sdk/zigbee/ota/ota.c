/********************************************************************************************************
 * @file    ota.c
 *
 * @brief   This is the source file for ota
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
#include "../common/includes/zb_common.h"
#include "../zcl/zcl_include.h"
#include "ota.h"

#ifdef ZCL_OTA
/**********************************************************************
 * LOCAL CONSTANTS
 */
/* 4 + 8 + 8N + 69 + 9 + 8 + (20 + 8)N = 4096; N = 110*/
#define FLASH_WRITE_COUNT_GET(size)     ((size / (110 * OTA_IMAGE_MAX_DATA_SIZE)) + 1)
#define TL_IMAGE_START_FLAG             0x4b
#define TL_START_UP_FLAG_WHOLE          0x544c4e4b

/**********************************************************************
 * TYPEDEFS
 */
typedef struct _attribute_packed_ {
    addrExt_t extAddr;
    u16 profileId;
    u8 endpoint;
    u8 txOptions;
} ota_serverAddr_t;

typedef struct _attribute_packed_ {
    ota_hdrFields_t hdrInfo;
    ota_serverAddr_t otaServerAddrInfo;
} ota_updateInfo_t;

#if 0
u8 ota_imageNotifyHandler(apsdeDataInd_t *pInd, ota_imageNotify_t *pImageNotify);
u8 ota_queryNextImageReqHandler(apsdeDataInd_t *pInd, ota_queryNextImageReq_t *pQueryNextImageReq);
u8 ota_queryNextImageRspHandler(apsdeDataInd_t *pInd, ota_queryNextImageRsp_t *pQueryNextImageRsp);
u8 ota_imageBlockReqHandler(apsdeDataInd_t *pInd, ota_imageBlockReq_t *pImageBlockReq);
u8 ota_imagePageReqHandler(apsdeDataInd_t *pInd, ota_imagePageReq_t *pImagePageReq);
u8 ota_imageBlockRspHandler(apsdeDataInd_t *pInd, ota_imageBlockRsp_t *pImageBlockRsp);
u8 ota_upgradeEndReqHandler(apsdeDataInd_t *pInd, ota_upgradeEndReq_t *pUpgradeEndReq);
u8 ota_upgradeEndRspHandler(apsdeDataInd_t *pInd, ota_upgradeEndRsp_t *pUpgradeEndRsp);
u8 ota_queryDevSpecFileReqHandler(apsdeDataInd_t *pInd, ota_querySpecFileReq_t *pQueryDevSpecFileReq);
u8 ota_queryDevSpecFileRspHandler(apsdeDataInd_t *pInd, ota_querySpecFileRsp_t *pQueryDevSpecFileRsp);

zcl_ota_AppCallbacks_t zcl_otaCb = {
    ota_imageNotifyHandler,
    ota_queryNextImageReqHandler,
    ota_queryNextImageRspHandler,
    ota_imageBlockReqHandler,
    ota_imagePageReqHandler,
    ota_imageBlockRspHandler,
    ota_upgradeEndReqHandler,
    ota_upgradeEndRspHandler,
    ota_queryDevSpecFileReqHandler,
    ota_queryDevSpecFileRspHandler,
};
#endif

/**********************************************************************
 * GLOBAL VARIABLES
 */

//boot address flag
u32 mcuBootAddr = 0;
const u8 otaHdrMagic[] = {
    0x1e, 0xf1, 0xee, 0x0b
};

const u8 otaAesKey[16] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

ota_ctx_t g_otaCtx;
ota_callBack_t *otaCb = NULL;

//client side
ota_clientInfo_t otaClientInfo;
ota_updateInfo_t *pOtaUpdateInfo = NULL;

//server side
ota_preamble_t otaServerBinInfo;

bool g_otaEncryptionNeeded = 0;


/**********************************************************************
 * LOCAL VARIABLES
 */

ev_timer_event_t otaTimer;

/**********************************************************************
 * FUNCTIONS
 */
void ota_queryNextImageReq(u8 dstEp, u16 dstAddr, u16 profileId);
void ota_imageBlockReq(u8 dstEp, u16 dstAddr, u16 profileId);
void ota_sendImageBlockReq(void *arg);
void ota_upgradeEndReqSend(ota_upgradeEndReq_t *req);
void ota_upgradeWait(u32 seconds);
void ota_upgradeComplete(u8 status);

/**********************************************************************
 * @brief		get mcu boot address
 *
 * return 		boot form 0x0 or FLASH_ADDR_OF_OTA_IMAGE
 * 				all 0xFF means invalid
 */
u32 mcuBootAddrGet(void)
{
#if (BOOT_LOADER_MODE)
    return 0;
#else
    u32 bootAddr = 0xFFFFFFFF;
    u32 flashInfo = 0;

    flash_read(0 + FLASH_TLNK_FLAG_OFFSET, 4, (u8 *)&flashInfo);
    if (flashInfo == TL_START_UP_FLAG_WHOLE) {
            bootAddr = 0;
    } else {
        flash_read(FLASH_ADDR_OF_OTA_IMAGE + FLASH_TLNK_FLAG_OFFSET, 4, (u8 *)&flashInfo);
        if (flashInfo == TL_START_UP_FLAG_WHOLE) {
            bootAddr = FLASH_ADDR_OF_OTA_IMAGE;
        }
    }

    return bootAddr;
#endif
}

bool ota_newImageValid(u32 new_image_addr)
{
    bool ret = FALSE;
    u32 bufCache[256/4];  //align-4
    u8 *buf = (u8 *)bufCache;
    flash_read(new_image_addr, 256, (u8 *)buf);
    u32 fw_size = *(u32 *)(buf + 0x18);

    if (fw_size <= FLASH_OTA_IMAGE_MAX_SIZE) {
        s32 totalLen = fw_size - 4;
        u32 wLen = 0;
        u32 sAddr = new_image_addr;
        u32 oft = 0;

        u32 crcVal = 0;
        flash_read(new_image_addr + fw_size - 4, 4, (u8 *)&crcVal);

        u32 *pStartFlag = (u32 *)(buf + FLASH_TLNK_FLAG_OFFSET);
        if ((*pStartFlag & 0xffffff00) != 0x544c4e00) {
            return FALSE;
        }

        u32 curCRC = 0xffffffff;

        while (totalLen > 0) {
            wLen = (totalLen > 256) ? 256 : totalLen;
            flash_read(sAddr, wLen, buf);
            if (oft == 0) {
                buf[FLASH_TLNK_FLAG_OFFSET] = TL_IMAGE_START_FLAG;
            }
            curCRC = xcrc32(buf, wLen, curCRC);

            totalLen -= wLen;
            sAddr += wLen;
            oft += wLen;
        }

        if (curCRC == crcVal) {
            ret = TRUE;
        }
    }

    return ret;
}

void ota_mcuReboot(void)
{
    u8 flashInfo = TL_IMAGE_START_FLAG;
    u32 newAddr = FLASH_ADDR_OF_OTA_IMAGE;
    bool reboot = 0;

#if (BOOT_LOADER_MODE)
    if (!ota_newImageValid(newAddr)) {
        return;
    }

#if FLASH_PROTECT_ENABLE
    flash_unlock();
#endif

    if (flash_writeWithCheck((newAddr + FLASH_TLNK_FLAG_OFFSET), 1, &flashInfo) == TRUE) {
        reboot = 1;
    }
#else
    u32 baseAddr = 0;

    if (mcuBootAddr) {
        baseAddr = FLASH_ADDR_OF_OTA_IMAGE;
        newAddr = 0;
    }
    if (!ota_newImageValid(newAddr)) {
        return;
    }

#if FLASH_PROTECT_ENABLE
    flash_unlock();
#endif

    if (flash_writeWithCheck((newAddr + FLASH_TLNK_FLAG_OFFSET), 1, &flashInfo) == TRUE) {
        flashInfo = 0;
        flash_write((baseAddr + FLASH_TLNK_FLAG_OFFSET), 1, &flashInfo);//disable boot-up flag

        reboot = 1;
    }
#endif

#if FLASH_PROTECT_ENABLE
    flash_lock();
#endif

    if (reboot) {
        SYSTEM_RESET();
    }
}

/**********************************************************************
 * @brief		server IEEE address preprogrammed for application
 *
 */
void ota_serverAddrPerprogrammed(addrExt_t ieeeAddr, u8 srvEndPoint)
{
    if (ZB_IEEE_ADDR_IS_INVALID(zcl_attr_upgradeServerID)) {
        g_otaCtx.otaServerEpInfo.dstEp = srvEndPoint;
        ZB_IEEE_ADDR_COPY(zcl_attr_upgradeServerID, ieeeAddr);
    }
}

u8 ota_loadImageInfo(ota_hdrFields_t *oh)
{
    ota_hdrFields_t *lh = oh;
    ota_hdrFields_t hdr;
    if (!lh) {
        lh = &hdr;
        u32 flashAddr = (mcuBootAddr) ? 0 : FLASH_ADDR_OF_OTA_IMAGE;
        flash_read(flashAddr, sizeof(hdr), (u8 *)&hdr);
    }
    if (lh->otaUpgradeFileID == OTA_UPGRADE_FILE_ID) {
        otaServerBinInfo.fileVer = lh->fileVer;
        otaServerBinInfo.imageType = lh->imageType;
        otaServerBinInfo.manufacturerCode = lh->manufacturerCode;
        otaServerBinInfo.totalImageSize = lh->totalImageSize;
    } else {
        return RET_ERROR;
    }
    return RET_OK;
}

void ota_clientInfoRecover(void)
{
    //TODO: read the download code OTA file header
    ota_updateInfo_t *pInfo = (ota_updateInfo_t *)ev_buf_allocate(sizeof(ota_updateInfo_t));
    if (pInfo) {
        if (nv_flashReadNew(1, NV_MODULE_OTA, NV_ITEM_OTA_HDR_SERVERINFO, sizeof(ota_updateInfo_t), (u8 *)pInfo) == NV_SUCC) {
            if (pInfo->hdrInfo.otaUpgradeFileID == OTA_UPGRADE_FILE_ID) {
                if (nv_flashReadNew(1, NV_MODULE_OTA, NV_ITEM_OTA_CODE, sizeof(ota_clientInfo_t), (u8 *)&otaClientInfo) != NV_SUCC) {
                    ev_buf_free((u8 *)pInfo);
                    return;
                }

                zcl_attr_fileOffset = otaClientInfo.offset;
                zcl_attr_downloadFileVer = pInfo->hdrInfo.fileVer;
                zcl_attr_downloadZigbeeStackVer = pInfo->hdrInfo.zbStackVer;
                zcl_attr_manufacturerID = pInfo->hdrInfo.manufacturerCode;
                zcl_attr_imageTypeID = pInfo->hdrInfo.imageType;
                g_otaCtx.downloadImageSize = pInfo->hdrInfo.totalImageSize;
            }
            if (!ZB_IEEE_ADDR_IS_INVALID(pInfo->otaServerAddrInfo.extAddr)) {
                ZB_IEEE_ADDR_COPY(zcl_attr_upgradeServerID, pInfo->otaServerAddrInfo.extAddr);
                g_otaCtx.otaServerEpInfo.profileId = pInfo->otaServerAddrInfo.profileId;
                g_otaCtx.otaServerEpInfo.dstEp = pInfo->otaServerAddrInfo.endpoint;
                g_otaCtx.otaServerEpInfo.txOptions = pInfo->otaServerAddrInfo.txOptions;
            }
        }

        ev_buf_free((u8 *)pInfo);
    }
}

/**********************************************************************
 * @brief		ota init
 *
 */
void ota_init(ota_type_e type, af_simple_descriptor_t *simpleDesc, ota_preamble_t *otaPreamble, ota_callBack_t *cb)
{
    //get current boot-up address
    mcuBootAddr = mcuBootAddrGet();
    if (mcuBootAddr == 0xFFFFFFFF) {
        //should not happen
        ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_BOOT_ADDR_ERROR);
        return;
    }

    otaCb = cb;

    memset((u8 *)&g_otaCtx, 0, sizeof(g_otaCtx));
    memset((u8 *)&otaClientInfo, 0, sizeof(otaClientInfo));
    memset((u8 *)&otaServerBinInfo, 0, sizeof(otaServerBinInfo));

    g_otaCtx.isOtaServer = (type == OTA_TYPE_SERVER) ? TRUE : FALSE;
    g_otaCtx.simpleDesc = simpleDesc;

    zcl_attr_currZigbeeStackVer = ZB_STACKVER_PRO;
    zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_NORMAL;

    zcl_register(simpleDesc->endpoint, OTA_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)g_otaClusterList);

    if (g_otaCtx.isOtaServer) {
        ota_loadImageInfo(NULL);
    } else {
        //load the run code OTA file header
        g_otaCtx.pOtaPreamble = otaPreamble;

        zcl_attr_currFileVer = otaPreamble->fileVer;
        zcl_attr_manufacturerID = otaPreamble->manufacturerCode;

        otaClientInfo.clientOtaFlg = OTA_FLAG_INIT_DONE;
        otaClientInfo.crcValue = 0xffffffff;
        ota_clientInfoRecover();
    }
}

#ifdef ZCL_WWAH
void ota_wwah_useTrustCenter(u8 endpoint)
{
    if (!ZB_IEEE_ADDR_CMP(zcl_attr_upgradeServerID, ss_ib.trust_center_address)) {
        ZB_IEEE_ADDR_INVALID(zcl_attr_upgradeServerID);

        ev_unon_timer(&otaTimer);

        ota_upgradeComplete(ZCL_STA_INVALID_IMAGE);

        ota_serverAddrPerprogrammed(ss_ib.trust_center_address, endpoint);
    }

    ota_updateInfo_t *pInfo = (ota_updateInfo_t *)ev_buf_allocate(sizeof(ota_updateInfo_t));
    if (pInfo) {
        if (nv_flashReadNew(1, NV_MODULE_OTA, NV_ITEM_OTA_HDR_SERVERINFO, sizeof(ota_updateInfo_t), (u8 *)pInfo) == NV_SUCC) {
            if (ZB_IEEE_ADDR_CMP(pInfo->otaServerAddrInfo.extAddr, ss_ib.trust_center_address) &&
               (pInfo->otaServerAddrInfo.endpoint == endpoint)) {
                ev_buf_free((u8 *)pInfo);
                return;
            }
        }

        memset((u8 *)pInfo, 0, sizeof(ota_updateInfo_t));

        pInfo->otaServerAddrInfo.endpoint = endpoint;
        pInfo->otaServerAddrInfo.profileId = HA_PROFILE_ID;
        ZB_IEEE_ADDR_COPY(pInfo->otaServerAddrInfo.extAddr, ss_ib.trust_center_address);

        nv_flashWriteNew(1, NV_MODULE_OTA, NV_ITEM_OTA_HDR_SERVERINFO, sizeof(ota_updateInfo_t), (u8 *)pInfo);

        ev_buf_free((u8 *)pInfo);
    }
}
#endif

static void ota_ieeeAddrRspCb(void *arg)
{
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
    zdo_ieee_addr_resp_t *rsp = (zdo_ieee_addr_resp_t*)p->zpdu;

    if (rsp->status == ZDO_SUCCESS) {
        ZB_IEEE_ADDR_COPY(zcl_attr_upgradeServerID, rsp->ieee_addr_remote);
    }

    if ((zcl_attr_imageUpgradeStatus != IMAGE_UPGRADE_STATUS_DOWNLOAD_IN_PROGRESS) ||
        (otaClientInfo.clientOtaFlg >= OTA_FLAG_IMAGE_MAGIC_0)) {
        return;
    }

    //stop ieee address rsp timer
    ev_unon_timer(&otaTimer);

    if ((rsp->status == ZDO_SUCCESS)) {
        //stop server query start timer

        ota_sendImageBlockReq(NULL);
        otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_MAGIC_0;
        if (otaCb) {
            otaCb->processMsgCbFunc(OTA_EVT_START, ZCL_STA_SUCCESS);
        }
    } else {
        ota_upgradeComplete(ZCL_STA_ABORT);
    }
}

static void ota_ieeeAddrReq(u16 dstAddr)
{
    zdo_ieee_addr_req_t pReq;
    u8 sn = 0;
    pReq.nwk_addr_interest = dstAddr;
    pReq.req_type = ZDO_ADDR_REQ_SINGLE_RESP;
    pReq.start_index = 0;
    zb_zdoIeeeAddrReq(dstAddr, &pReq, &sn, ota_ieeeAddrRspCb);
}

static s32 ota_ieeeAddrRspWait(void *arg)
{
    static u8 cnt = 0;

    if (++cnt >= OTA_IEEE_ADDR_REQ_RETRIES) {
        cnt = 0;

        ota_upgradeComplete(ZCL_STA_ABORT);

        return -1;
    }

    ota_ieeeAddrReq(g_otaCtx.otaServerEpInfo.dstAddr.shortAddr);

    return 0;
}

static void ota_ieeeAddrReqSend(void *arg)
{
    ota_ieeeAddrReq(g_otaCtx.otaServerEpInfo.dstAddr.shortAddr);

    //start a timer to wait ieee address rsp
    otaTimer.cb = ota_ieeeAddrRspWait;
    otaTimer.data = NULL;
    ev_on_timer(&otaTimer, OTA_IEEE_ADDR_RSP_WAIT_TIME * 1000);
}

static s32 ota_queryNextImageReqDelay(void *arg)
{
    ota_queryNextImageReq(g_otaCtx.otaServerEpInfo.dstEp, g_otaCtx.otaServerEpInfo.dstAddr.shortAddr, g_otaCtx.otaServerEpInfo.profileId);
    return -1;
}

static void ota_matchDescRspCb(void *arg)
{
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
    zdo_match_descriptor_resp_t *rsp = (zdo_match_descriptor_resp_t*)p->zpdu;

    if ((rsp->status == ZDO_SUCCESS) && (rsp->matchLen)) {
        //ota server matched
        g_otaCtx.otaServerEpInfo.dstEp = rsp->matchList[0];
        g_otaCtx.otaServerEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        g_otaCtx.otaServerEpInfo.dstAddr.shortAddr = rsp->nwk_addr_interest;
        g_otaCtx.otaServerEpInfo.profileId = g_otaCtx.simpleDesc->app_profile_id;

        ota_ieeeAddrReq(g_otaCtx.otaServerEpInfo.dstAddr.shortAddr);   //ota_ieeeAddrReqSend(NULL);

        if (otaClientInfo.clientOtaFlg == OTA_FLAG_INIT_DONE) {
            TL_ZB_TIMER_SCHEDULE(ota_queryNextImageReqDelay, NULL, 500);
        }
    }
}

static void ota_clusterMatchReq(u16 dstAddr)
{
    zdo_match_descriptor_req_t *pReq = (zdo_match_descriptor_req_t *)ev_buf_allocate(6+2);
    if (pReq) {
        pReq->nwk_addr_interest = dstAddr;
        pReq->profile_id = g_otaCtx.simpleDesc->app_profile_id;
        pReq->num_in_clusters = 1;
        pReq->num_out_clusters = 0;
        pReq->cluster_list[0] = ZCL_CLUSTER_OTA;

        u8 sn = 0;
        zb_zdoMatchDescReq(dstAddr, pReq, &sn, ota_matchDescRspCb);

        ev_buf_free((u8 *)pReq);
    }
}

static void ota_nwkAddrReqCb(void *arg)
{
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
    zdo_nwk_addr_resp_t *rsp = (zdo_nwk_addr_resp_t *)p->zpdu;

    if (rsp->status == ZDO_SUCCESS) {
        g_otaCtx.otaServerEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        g_otaCtx.otaServerEpInfo.dstAddr.shortAddr = rsp->nwk_addr_remote;

        if (otaClientInfo.clientOtaFlg == OTA_FLAG_INIT_DONE) {
            ota_queryNextImageReq(g_otaCtx.otaServerEpInfo.dstEp, g_otaCtx.otaServerEpInfo.dstAddr.shortAddr, g_otaCtx.otaServerEpInfo.profileId);
        } else {
            g_otaCtx.imageBlockRetry = 0;
            ev_unon_timer(&otaTimer);
            ota_sendImageBlockReq(NULL);
            if (otaCb) {
                otaCb->processMsgCbFunc(OTA_EVT_START, ZCL_STA_SUCCESS);
            }
        }
    }
}

static void ota_nwkAddrReq(addrExt_t extAddr)
{
    u8 sn = 0;
    zdo_nwk_addr_req_t req;
    ZB_IEEE_ADDR_COPY(req.ieee_addr_interest, extAddr);
    req.req_type = ZDO_ADDR_REQ_SINGLE_RESP;
    req.start_index = 0;
    zb_zdoNwkAddrReq(NWK_BROADCAST_ROUTER_COORDINATOR, &req, &sn, ota_nwkAddrReqCb);
}

s32 ota_periodicQueryServerCb(void *arg)
{
    u16 seconds = ((u32)arg) & 0xffff;

    if (otaClientInfo.clientOtaFlg == OTA_FLAG_INIT_DONE) {
        if (ZB_IEEE_ADDR_IS_INVALID(zcl_attr_upgradeServerID)) {
            //Match descriptor request cmd
            ota_clusterMatchReq(NWK_BROADCAST_RX_ON_WHEN_IDLE);
        } else {
            u16 dstAddr = NWK_BROADCAST_RESERVED;
            u8 ret = zb_getNwkAddrByExtAddr(zcl_attr_upgradeServerID, &dstAddr);

            if ((ret == SUCCESS) && (dstAddr == g_otaCtx.otaServerEpInfo.dstAddr.shortAddr)) {
                ota_queryNextImageReq(g_otaCtx.otaServerEpInfo.dstEp, g_otaCtx.otaServerEpInfo.dstAddr.shortAddr, g_otaCtx.otaServerEpInfo.profileId);
            } else {
                //Short address request
                ota_nwkAddrReq(zcl_attr_upgradeServerID);
            }
        }
    } else {
        /*
          * send the command of "NEXT_IMAGE_REQ" to validate the image
          * and then decides to restart OTA from 0 or zcl_attr_fileOffset once receiving
          * the command of "NEXT_IMAGE_RESP"
          *
          */
        ota_queryNextImageReq(g_otaCtx.otaServerEpInfo.dstEp, g_otaCtx.otaServerEpInfo.dstAddr.shortAddr, g_otaCtx.otaServerEpInfo.profileId);
    }

    return (seconds * 1000);
}

/**********************************************************************
 * @brief		ota query start
 *
 * @param		period - in seconds
 *
 */
void ota_queryStart(u16 seconds)
{
    if (!g_otaCtx.isOtaServer) {
        if (!ev_timer_exist(&otaTimer)) {
            otaTimer.cb = ota_periodicQueryServerCb;
            otaTimer.data = (void *)((u32)seconds);

            u16 jitter = 0;
            do {
                jitter = zb_random() % OTA_QUERY_START_JITTER;
            } while (jitter == 0);

            ev_on_timer(&otaTimer, (seconds * 1000) + jitter);
        }
    }
}

void ota_upgradeComplete(u8 status)
{
    /* shall free this buffer */
    if (pOtaUpdateInfo) {
        ev_buf_free((u8 *)pOtaUpdateInfo);
        pOtaUpdateInfo = NULL;
    }

    zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_NORMAL;

#if FLASH_PROTECT_ENABLE
    flash_lock();
#endif

    if (status == ZCL_STA_SUCCESS) {
        nv_resetModule(NV_MODULE_OTA);
        if (otaCb) {
            otaCb->processMsgCbFunc(OTA_EVT_COMPLETE, status);
        }
    } else {
        /*
         * if the image is invalid, all the information shall be reset,
         * so that the OTA will restart from address of 0
         *
         */
        if (status == ZCL_STA_INVALID_IMAGE) {
            nv_resetModule(NV_MODULE_OTA);

            memset((u8 *)&otaClientInfo, 0, sizeof(otaClientInfo));
            otaClientInfo.clientOtaFlg = OTA_FLAG_INIT_DONE;
            otaClientInfo.crcValue = 0xffffffff;

            zcl_attr_imageTypeID = 0xffff;
            zcl_attr_fileOffset = 0xffffffff;
            zcl_attr_downloadFileVer = 0xffffffff;
        }

        if (otaCb) {
            otaCb->processMsgCbFunc(OTA_EVT_COMPLETE, status);
        }
    }
}

static void ota_imageBlockRspTimeout(void *arg)
{
    ota_upgradeComplete(ZCL_STA_ABORT);
}

void ota_upgradeAbort(void)
{
    ota_upgradeComplete(ZCL_STA_ABORT);
}

s32 ota_imageBlockRspWait(void *arg)
{
    if (++g_otaCtx.imageBlockRetry >= OTA_MAX_IMAGE_BLOCK_RETRIES) {
        //send a failed upgrade end req
        ota_upgradeEndReq_t req;

        req.st = ZCL_STA_ABORT;
        req.fileVer = zcl_attr_downloadFileVer;
        req.imageType = zcl_attr_imageTypeID;
        req.manuCode = zcl_attr_manufacturerID;

        ota_upgradeEndReqSend(&req);

        /*
         * Notes: must not call ota_upgradeComplete directly, use TL_SCHEDULE_TASK() to achieve it
         * The function "ota_imageBlockRspWait" is the callback of the timer event otaTimer,
         * so that the timer event(otaTimer) modified via otaCb->processMsgCbFunc():ota_queryStart()
         * will be killed when it returns -1.
         *
         * */
        TL_SCHEDULE_TASK(ota_imageBlockRspTimeout, NULL);  //ota_upgradeComplete(ZCL_STA_ABORT);
    } else {
        //send another image block req
        TL_SCHEDULE_TASK(ota_sendImageBlockReq, NULL);
    }

    return -1;
}

void sendImageBlockReq(void *arg)
{
    ota_imageBlockReq(g_otaCtx.otaServerEpInfo.dstEp, g_otaCtx.otaServerEpInfo.dstAddr.shortAddr, g_otaCtx.otaServerEpInfo.profileId);

    //start a timer to wait image block rsp
    otaTimer.cb = ota_imageBlockRspWait;
    otaTimer.data = NULL;
    ev_on_timer(&otaTimer, OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME * 1000);
}

s32 ota_sendImageBlockReqDelay(void *arg)
{
    TL_SCHEDULE_TASK(sendImageBlockReq, NULL);
    return -1;
}

void ota_sendImageBlockReq(void *arg)
{
    //stop server query start or image block rsp timer
    //ev_unon_timer(&otaTimer);

    if (zcl_attr_minBlockPeriod) {
        otaTimer.cb = ota_sendImageBlockReqDelay;
        otaTimer.data = NULL;
        ev_on_timer(&otaTimer, zcl_attr_minBlockPeriod);
    } else {
        sendImageBlockReq(NULL);
    }
}

s32 ota_imageBlockWaitCb(void *arg)
{
    if (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_IN_PROGRESS) {
        TL_SCHEDULE_TASK(sendImageBlockReq, NULL);
    }

    return -1;
}

void ota_imageBlockWait(u32 seconds)
{
    if (seconds) {
        otaTimer.cb = ota_imageBlockWaitCb;
        otaTimer.data = NULL;
        ev_on_timer(&otaTimer, seconds * 1000);
    } else {
        ota_sendImageBlockReq(NULL);
    }
}

void ota_upgradeWithWait(void *arg)
{
    ota_upgradeEndReq_t req;

    req.st = ZCL_STA_SUCCESS;
    req.fileVer = zcl_attr_downloadFileVer;
    req.imageType = zcl_attr_imageTypeID;
    req.manuCode = zcl_attr_manufacturerID;

    ota_upgradeEndReqSend(&req);

    //restart the timer for another hour
    u32 time = (u32)arg;
    ota_upgradeWait(time);
}

void ota_upgrade(void)
{
    if (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE) {
        if (++g_otaCtx.upgradeEndRetry >= OTA_MAX_UPGRADE_END_REQ_RETRIES) {
            ota_upgradeComplete(ZCL_STA_SUCCESS);
        } else {
            TL_SCHEDULE_TASK(ota_upgradeWithWait, (void *)OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME);
        }
    } else if (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_COUNT_DOWN) {
        ota_upgradeComplete(ZCL_STA_SUCCESS);
    } else if (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_WAITING_TO_UPGRADE) {
        if (++g_otaCtx.upgradeEndRetry >= OTA_MAX_UPGRADE_END_REQ_RETRIES) {
            ota_upgradeComplete(ZCL_STA_SUCCESS);
        } else {
            //send another upgrade end req
            TL_SCHEDULE_TASK(ota_upgradeWithWait, (void *)3600);
        }
    }
}

s32 ota_upgradeWaitCb(void *arg)
{
    ota_upgrade();
    return -1;
}

void ota_upgradeWait(u32 seconds)
{
    if (seconds) {
        otaTimer.cb = ota_upgradeWaitCb;
        otaTimer.data = NULL;
        ev_on_timer(&otaTimer, seconds * 1000);
    } else {
        ota_upgrade();
    }
}

bool ota_fileIdCmp(u16 currManuCode, u16 newManuCode, u16 currImageType, u16 newImageType, u32 currFileVer, u32 newFileVer)
{
    if ((currManuCode == 0xFFFF) || (newManuCode == 0xFFFF) || (currManuCode == newManuCode)) {
        if ((currImageType == 0xFFFF) || (newImageType == 0xFFFF) || (currImageType == newImageType)) {
            if ((currFileVer == 0xFFFFFFFF) || (newFileVer == 0xFFFFFFFF) || (currFileVer == newFileVer)) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

void ota_saveUpdateInfo2NV(void *arg)
{
    static u8 cnt = 0;

    if ((++cnt >= FLASH_WRITE_COUNT_GET(g_otaCtx.downloadImageSize) ||
        (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE))) {
        cnt = 0;
        nv_flashWriteNew(1, NV_MODULE_OTA, NV_ITEM_OTA_CODE, sizeof(ota_clientInfo_t), (u8 *)&otaClientInfo);
    }
}

u8 ota_imageDataProcess(u8 len, u8 *pData)
{
    bool validChecked = 0;

    if (zcl_attr_imageUpgradeStatus != IMAGE_UPGRADE_STATUS_DOWNLOAD_IN_PROGRESS) {
        return ZCL_STA_ABORT;
    }
    if (!pData) {
        return ZCL_STA_INVALID_IMAGE;
    }

    for (u8 i = 0; i < len; i++) {
        switch (otaClientInfo.clientOtaFlg) {
        case OTA_FLAG_IMAGE_MAGIC_0:
            if (pData[i] != otaHdrMagic[i] || !pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            } else {
                pOtaUpdateInfo->hdrInfo.otaUpgradeFileID = pData[i];
            }
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_MAGIC_1;
            break;
        case OTA_FLAG_IMAGE_MAGIC_1:
            if (pData[i] != otaHdrMagic[i]|| !pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            } else {
                pOtaUpdateInfo->hdrInfo.otaUpgradeFileID |= ((u32)pData[i] << 8) & 0xff00;
            }
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_MAGIC_2;
            break;
        case OTA_FLAG_IMAGE_MAGIC_2:
            if (pData[i] != otaHdrMagic[i]|| !pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            } else {
                pOtaUpdateInfo->hdrInfo.otaUpgradeFileID |= ((u32)pData[i] << 16) & 0xff0000;
            }
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_MAGIC_3;
            break;
        case OTA_FLAG_IMAGE_MAGIC_3:
            if (pData[i] != otaHdrMagic[i]) {
                return ZCL_STA_INVALID_IMAGE;
            } else {
                if (!pOtaUpdateInfo) {
                    return ZCL_STA_INVALID_IMAGE;
                }
                pOtaUpdateInfo->hdrInfo.otaUpgradeFileID |= ((u32)pData[i] << 24) & 0xff000000;
            }
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_HDR_VER1;
            break;
        case OTA_FLAG_IMAGE_HDR_VER1:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.otaHdrVer = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_HDR_VER2;
            break;
        case OTA_FLAG_IMAGE_HDR_VER2:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.otaHdrVer |= ((u16)pData[i] << 8) & 0xff00;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_HDR_LEN1;
            break;
        case OTA_FLAG_IMAGE_HDR_LEN1:
            //get header length
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            if (zcl_attr_fileOffset == OTA_HDR_LEN_OFFSET) {
                pOtaUpdateInfo->hdrInfo.otaHdrLen = pData[i];
                otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_HDR_LEN2;
            }
            break;
        case OTA_FLAG_IMAGE_HDR_LEN2:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.otaHdrLen |= ((u16)pData[i] << 8) & 0xff00;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_HDR_FC1;
            break;
        case OTA_FLAG_IMAGE_HDR_FC1:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.otaHdrFC = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_HDR_FC2;
            break;
        case OTA_FLAG_IMAGE_HDR_FC2:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.otaHdrFC |= ((u16)pData[i] << 8) & 0xff00;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_MANU_CODE1;
            break;
        case OTA_FLAG_IMAGE_MANU_CODE1:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.manufacturerCode = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_MANU_CODE2;
            break;
        case OTA_FLAG_IMAGE_MANU_CODE2:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.manufacturerCode |= ((u16)pData[i] << 8) & 0xff00;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_TYPE1;
            break;
        case OTA_FLAG_IMAGE_TYPE1:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.imageType = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_TYPE2;
            break;
        case OTA_FLAG_IMAGE_TYPE2:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.imageType |= ((u16)pData[i] << 8) & 0xff00;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_FILE_VER1;
            break;
        case OTA_FLAG_IMAGE_FILE_VER1:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.fileVer = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_FILE_VER2;
            break;
        case OTA_FLAG_IMAGE_FILE_VER2:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.fileVer |= ((u32)pData[i] << 8) & 0xff00;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_FILE_VER3;
            break;
        case OTA_FLAG_IMAGE_FILE_VER3:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.fileVer |= ((u32)pData[i] << 16) & 0xff0000;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_FILE_VER4;
            break;
        case OTA_FLAG_IMAGE_FILE_VER4:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.fileVer |= ((u32)pData[i] << 24) & 0xff000000;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_STACK_VER1;
            break;
        case OTA_FLAG_IMAGE_STACK_VER1:
            //get stack ver
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            if (zcl_attr_fileOffset == OTA_STACK_VER_OFFSET) {
                pOtaUpdateInfo->hdrInfo.zbStackVer = pData[i];
                otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_STACK_VER2;
            }
            break;
        case OTA_FLAG_IMAGE_STACK_VER2:
            if (!pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            pOtaUpdateInfo->hdrInfo.zbStackVer |= ((u16)pData[i] << 8) & 0xff00;
            if (pOtaUpdateInfo->hdrInfo.zbStackVer != ZB_STACKVER_PRO) {
                return ZCL_STA_INVALID_IMAGE;
            } else {
                zcl_attr_downloadZigbeeStackVer = pOtaUpdateInfo->hdrInfo.zbStackVer;
            }
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_CONT_HDR;
            break;
        case OTA_FLAG_IMAGE_CONT_HDR:
            //complete header
            if (zcl_attr_fileOffset >= sizeof(ota_hdrFields_t) || !pOtaUpdateInfo) {
                return ZCL_STA_INVALID_IMAGE;
            }
            memcpy((u8 *)&(pOtaUpdateInfo->hdrInfo) + zcl_attr_fileOffset, &pData[i], 1);
            if (zcl_attr_fileOffset == pOtaUpdateInfo->hdrInfo.otaHdrLen - 1) {
                //TODO: save ota header to flash

                ZB_IEEE_ADDR_COPY(pOtaUpdateInfo->otaServerAddrInfo.extAddr, zcl_attr_upgradeServerID);
                pOtaUpdateInfo->otaServerAddrInfo.profileId = g_otaCtx.otaServerEpInfo.profileId;
                pOtaUpdateInfo->otaServerAddrInfo.endpoint = g_otaCtx.otaServerEpInfo.dstEp;
                pOtaUpdateInfo->otaServerAddrInfo.txOptions = g_otaCtx.otaServerEpInfo.txOptions;

                if (nv_flashWriteNew(1, NV_MODULE_OTA, NV_ITEM_OTA_HDR_SERVERINFO, sizeof(ota_updateInfo_t), (u8 *)pOtaUpdateInfo) != NV_SUCC) {
                    return ZCL_STA_INVALID_IMAGE;
                }
                ev_buf_free((u8 *)pOtaUpdateInfo);
                pOtaUpdateInfo = NULL;

                otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_TAG1;
            }
            break;
        case OTA_FLAG_IMAGE_ELEM_TAG1:
            otaClientInfo.otaElementTag = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_TAG2;
            break;
        case OTA_FLAG_IMAGE_ELEM_TAG2:
            otaClientInfo.otaElementTag |= ((u16)pData[i] << 8) & 0xff00;
            otaClientInfo.otaElementPos = 0;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_LEN1;
            break;
        case OTA_FLAG_IMAGE_ELEM_LEN1:
            otaClientInfo.otaElementLen = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_LEN2;
            break;
        case OTA_FLAG_IMAGE_ELEM_LEN2:
            otaClientInfo.otaElementLen |= ((u32)pData[i] << 8) & 0xff00;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_LEN3;
            break;
        case OTA_FLAG_IMAGE_ELEM_LEN3:
            otaClientInfo.otaElementLen |= ((u32)pData[i] << 16) & 0xff0000;
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_LEN4;
            break;
        case OTA_FLAG_IMAGE_ELEM_LEN4:
            otaClientInfo.otaElementLen |= ((u32)pData[i] << 24) & 0xff000000;
            otaClientInfo.clientOtaFlg = (otaClientInfo.otaElementTag == OTA_UPGRADE_IMAGE_AES_TAG_ID) ? OTA_FLAG_IMAGE_ELEM_INFO1
                                                                                                       : OTA_FLAG_IMAGE_ELEMENT;

            //make sure the length of element is not bigger than the image
            if ((otaClientInfo.otaElementLen > (g_otaCtx.downloadImageSize - zcl_attr_fileOffset)) ||
                (otaClientInfo.otaElementLen == 0)) {
                return ZCL_STA_INVALID_IMAGE;
            }
            break;
        case OTA_FLAG_IMAGE_ELEM_INFO1:
            otaClientInfo.otaElementInfo1 = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_INFO2;
            break;
        case OTA_FLAG_IMAGE_ELEM_INFO2:
            otaClientInfo.otaElementInfo2 = pData[i];
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEMENT;
            break;
        case OTA_FLAG_IMAGE_ELEMENT:
            if ((otaClientInfo.otaElementLen == 0) ||
                (otaClientInfo.otaElementPos > otaClientInfo.otaElementLen)) {
                return ZCL_STA_INVALID_IMAGE;
            }

            if ((otaClientInfo.otaElementTag == OTA_UPGRADE_IMAGE_TAG_ID) ||
                (otaClientInfo.otaElementTag == OTA_UPGRADE_IMAGE_AES_TAG_ID)) {
                //if g_otaEncryptionNeeded is set, it will reject the un-encrypted ota image data.
                if ((otaClientInfo.otaElementTag == OTA_UPGRADE_IMAGE_TAG_ID) &&
                    g_otaEncryptionNeeded) {
                    return ZCL_STA_INVALID_IMAGE;
                }

                u8 dataSize = len - i;

                u8 copyLen = dataSize;
                if (otaClientInfo.otaElementLen - otaClientInfo.otaElementPos < dataSize) {
                    copyLen = otaClientInfo.otaElementLen - otaClientInfo.otaElementPos;
                }

                if (otaClientInfo.otaElementTag == OTA_UPGRADE_IMAGE_AES_TAG_ID) {
                    u8 tmpBuf[16];
                    for (u8 j = 0; j < copyLen; j += 16) {
                        memset(tmpBuf, 0xff, 16);
                        memcpy(tmpBuf, &pData[i + j], 16);
                        drv_aes_decrypt((u8 *)otaAesKey, tmpBuf, &pData[i + j]);
                    }
                }

                //Calculate CRC value
                u32 dataEndPos = otaClientInfo.otaElementLen - otaClientInfo.otaElementInfo2;
                u8 crcLen = 0;
                if (dataEndPos - otaClientInfo.otaElementPos <= 4) {
                    crcLen = 0;
                } else if (dataEndPos - 4 - otaClientInfo.otaElementPos >= dataSize) {
                    crcLen = dataSize;
                } else {
                    crcLen = dataEndPos - 4 - otaClientInfo.otaElementPos;
                }
                otaClientInfo.crcValue = xcrc32(&pData[i], crcLen, otaClientInfo.crcValue);

                //write image to flash
                if ((otaClientInfo.otaElementPos < FLASH_TLNK_FLAG_OFFSET + 1) &&
                   ((otaClientInfo.otaElementPos + dataSize) >= FLASH_TLNK_FLAG_OFFSET + 1)) {
                    if (pData[i + (FLASH_TLNK_FLAG_OFFSET - otaClientInfo.otaElementPos)] != TL_IMAGE_START_FLAG) {
                        return ZCL_STA_INVALID_IMAGE;
                    }
                    pData[i + (FLASH_TLNK_FLAG_OFFSET - otaClientInfo.otaElementPos)] = 0xff;
                }
                u32 baseAddr = (mcuBootAddr) ? 0 : FLASH_ADDR_OF_OTA_IMAGE;
                if (flash_writeWithCheck(baseAddr + otaClientInfo.otaElementPos, copyLen, &pData[i]) != TRUE) {
                    return ZCL_STA_INVALID_IMAGE;
                }

                otaClientInfo.otaElementPos += copyLen;
                zcl_attr_fileOffset += copyLen;

                otaClientInfo.offset = zcl_attr_fileOffset;

                if (otaClientInfo.otaElementPos == otaClientInfo.otaElementLen) {
                    u16 crcFirmware = 0;
                    flash_read(baseAddr + 6, 2, (u8 *)&crcFirmware);
                    if (((crcFirmware & 0xff) == 0x5D) && ((crcFirmware >> 8) & 0xff) == 0x02) {
                        u32 crcReceived;
                        flash_read(baseAddr + dataEndPos - 4, 4, (u8 *)&crcReceived);

                        if (crcReceived != otaClientInfo.crcValue) {
                            return ZCL_STA_INVALID_IMAGE;
                        }
                        validChecked = 1;
                    } else {
                        return ZCL_STA_INVALID_IMAGE;
                    }

                    otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_ELEM_TAG1;
                }

                if (copyLen == dataSize) {
                    i = len - 1;
                } else {
                    i += copyLen - 1;
                }
                zcl_attr_fileOffset--;
            } else {
                return ZCL_STA_INVALID_IMAGE;
            }

            break;
        default:
            break;
        }

        zcl_attr_fileOffset++;

        //check if the download is complete
        if (zcl_attr_fileOffset >= g_otaCtx.downloadImageSize) {
            if (validChecked) {
                zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE;
                return ZCL_STA_SUCCESS;
            } else {
                return ZCL_STA_INVALID_IMAGE;
            }
        }
    }

    return ZCL_STA_SUCCESS;
}

void ota_queryNextImageReq(u8 dstEp, u16 dstAddr, u16 profileId)
{
    ota_queryNextImageReq_t req;
    memset((u8 *)&req, 0, sizeof(req));
    req.fc = 0;
    req.manuCode = g_otaCtx.pOtaPreamble->manufacturerCode;
    req.imageType = g_otaCtx.pOtaPreamble->imageType;
    req.curFileVer = zcl_attr_currFileVer;

    epInfo_t dstEpInfo;
    memset((u8 *)&dstEpInfo, 0, sizeof(dstEpInfo));
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = dstAddr;
    dstEpInfo.dstEp = dstEp;
    dstEpInfo.profileId = profileId;
    dstEpInfo.txOptions |= APS_TX_OPT_ACK_TX;
    dstEpInfo.txOptions |= g_otaCtx.otaServerEpInfo.txOptions;

    zcl_ota_queryNextImageReqCmdSend(g_otaCtx.simpleDesc->endpoint, &dstEpInfo, FALSE, &req);
}

void ota_imageBlockReq(u8 dstEp, u16 dstAddr, u16 profileId)
{
    ota_imageBlockReq_t req;
    memset((u8 *)&req, 0, sizeof(req));
    req.fc = OTA_IMAGE_BLOCK_FC;
    req.manuCode = g_otaCtx.pOtaPreamble->manufacturerCode;
    req.imageType = g_otaCtx.pOtaPreamble->imageType;
    req.fileVer = zcl_attr_downloadFileVer;
    req.fileOffset = zcl_attr_fileOffset;

    if (g_otaCtx.downloadImageSize - zcl_attr_fileOffset < OTA_IMAGE_MAX_DATA_SIZE) {
        req.maxDataSize = g_otaCtx.downloadImageSize - zcl_attr_fileOffset;
    } else {
        req.maxDataSize = OTA_IMAGE_MAX_DATA_SIZE;
    }

    if (req.fc & BLOCK_FC_BITMASK_MIN_PERIOD_PRESENT) {
        req.blockReqDelay = zcl_attr_minBlockPeriod;
    }

    epInfo_t dstEpInfo;
    memset((u8 *)&dstEpInfo, 0, sizeof(dstEpInfo));
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = dstAddr;
    dstEpInfo.dstEp = dstEp;
    dstEpInfo.profileId = profileId;
    dstEpInfo.txOptions |= APS_TX_OPT_ACK_TX;
    dstEpInfo.txOptions |= g_otaCtx.otaServerEpInfo.txOptions;

    zcl_ota_imageBlockReqCmdSend(g_otaCtx.simpleDesc->endpoint, &dstEpInfo, FALSE, &req);
}

void ota_upgradeEndReqSend(ota_upgradeEndReq_t *req)
{
    epInfo_t dstEpInfo;
    memset((u8 *)&dstEpInfo, 0, sizeof(dstEpInfo));
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = g_otaCtx.otaServerEpInfo.dstAddr.shortAddr;
    dstEpInfo.dstEp = g_otaCtx.otaServerEpInfo.dstEp;
    dstEpInfo.profileId = g_otaCtx.otaServerEpInfo.profileId;
    dstEpInfo.txOptions |= APS_TX_OPT_ACK_TX;
    dstEpInfo.txOptions |= g_otaCtx.otaServerEpInfo.txOptions;

    zcl_ota_upgradeEndReqCmdSend(g_otaCtx.simpleDesc->endpoint, &dstEpInfo, FALSE, req);
}

static status_t ota_queryNextImageReqHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_queryNextImageReq_t *pQueryNextImageReq)
{
    u8 status = ZCL_STA_NO_IMAGE_AVAILABLE;
    ota_queryNextImageRsp_t rsp;

    if ((otaServerBinInfo.manufacturerCode == pQueryNextImageReq->manuCode) &&
        (otaServerBinInfo.imageType == pQueryNextImageReq->imageType) &&
        (otaServerBinInfo.fileVer != pQueryNextImageReq->curFileVer)) {
        status = ZCL_STA_SUCCESS;
    }

    rsp.st = status;
    rsp.manuCode = pQueryNextImageReq->manuCode;
    rsp.imageType = pQueryNextImageReq->imageType;
    rsp.fileVer = otaServerBinInfo.fileVer;
    rsp.imageSize = otaServerBinInfo.totalImageSize;

    epInfo_t dstEpInfo;
    memset((u8 *)&dstEpInfo, 0, sizeof(dstEpInfo));
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
    dstEpInfo.dstEp = pAddrInfo->srcEp;
    dstEpInfo.profileId = pAddrInfo->profileId;
    dstEpInfo.txOptions |= APS_TX_OPT_ACK_TX;

    zcl_ota_queryNextImageRspCmdSend(g_otaCtx.simpleDesc->endpoint, &dstEpInfo, TRUE, pAddrInfo->seqNum, &rsp);

    return ZCL_STA_CMD_HAS_RESP;
}

static status_t ota_imageBlockReqHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_imageBlockReq_t *pImageBlockReq)
{
    if ((otaServerBinInfo.manufacturerCode != pImageBlockReq->manuCode) ||
        (otaServerBinInfo.fileVer != pImageBlockReq->fileVer) ||
        (otaServerBinInfo.imageType != pImageBlockReq->imageType)) {
        return ZCL_STA_NO_IMAGE_AVAILABLE;
    }

    u8 *buf = NULL;
    ota_imageBlockRsp_t rsp;

    if ((pImageBlockReq->fc & BLOCK_FC_BITMASK_MIN_PERIOD_PRESENT) && (pImageBlockReq->blockReqDelay != zcl_attr_minBlockPeriod)) {
        rsp.st = ZCL_STA_WAIT_FOR_DATA;
        rsp.rsp.wait.currTime = 0;
        rsp.rsp.wait.reqTime = 0;
        rsp.rsp.wait.blockReqDelay = zcl_attr_minBlockPeriod;
    } else {
        u8 len = (pImageBlockReq->maxDataSize > OTA_IMAGE_MAX_DATA_SIZE) ? OTA_IMAGE_MAX_DATA_SIZE : pImageBlockReq->maxDataSize;
        buf = (u8 *)ev_buf_allocate(len);
        if (!buf) {
            rsp.st = ZCL_STA_WAIT_FOR_DATA;
            rsp.rsp.wait.currTime = 0;
            rsp.rsp.wait.reqTime = 0;
            rsp.rsp.wait.blockReqDelay = zcl_attr_minBlockPeriod;
        } else {
            u32 flashAddr = (mcuBootAddr) ? 0 : FLASH_ADDR_OF_OTA_IMAGE;
            flash_read(flashAddr + pImageBlockReq->fileOffset, len, buf);
            rsp.st = ZCL_STA_SUCCESS;
            rsp.rsp.success.manuCode = pImageBlockReq->manuCode;
            rsp.rsp.success.imageType = pImageBlockReq->imageType;
            rsp.rsp.success.fileVer = otaServerBinInfo.fileVer;
            rsp.rsp.success.fileOffset = pImageBlockReq->fileOffset;
            rsp.rsp.success.dataSize = len;
            rsp.rsp.success.data = buf;
        }
    }

    epInfo_t dstEpInfo;
    memset((u8 *)&dstEpInfo, 0, sizeof(dstEpInfo));
    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
    dstEpInfo.dstEp = pAddrInfo->srcEp;
    dstEpInfo.profileId = pAddrInfo->profileId;

    zcl_ota_imageBlockRspCmdSend(g_otaCtx.simpleDesc->endpoint, &dstEpInfo, TRUE, pAddrInfo->seqNum, &rsp);

    if (buf) {
        ev_buf_free(buf);
    }

    return ZCL_STA_CMD_HAS_RESP;
}

static status_t ota_imagePageReqHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_imagePageReq_t *pImagePageReq)
{
    return ZCL_STA_UNSUP_CLUSTER_COMMAND;
}

static status_t ota_upgradeEndReqHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_upgradeEndReq_t *pUpgradeEndReq)
{
    u8 status = ZCL_STA_SUCCESS;

    if (pUpgradeEndReq->st == ZCL_STA_SUCCESS) {
        ota_upgradeEndRsp_t rsp;

        rsp.fileVer = pUpgradeEndReq->fileVer;
        rsp.imageType = pUpgradeEndReq->imageType;
        rsp.manuCode = pUpgradeEndReq->manuCode;
        rsp.currentTime = 0;
        rsp.upgradeTime = 60;

        epInfo_t dstEpInfo;
        memset((u8 *)&dstEpInfo, 0, sizeof(dstEpInfo));
        dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        dstEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
        dstEpInfo.dstEp = pAddrInfo->srcEp;
        dstEpInfo.profileId = pAddrInfo->profileId;

        zcl_ota_upgradeEndRspCmdSend(g_otaCtx.simpleDesc->endpoint, &dstEpInfo, TRUE, pAddrInfo->seqNum, &rsp);

        status = ZCL_STA_CMD_HAS_RESP;
    }

    return status;
}

static status_t ota_queryDevSpecFileReqHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_querySpecFileReq_t *pQueryDevSpecFileReq)
{
    return ZCL_STA_UNSUP_CLUSTER_COMMAND;
}

static status_t ota_imageNotifyHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_imageNotify_t *pImageNotify)
{
    if (!ZB_IEEE_ADDR_IS_INVALID(zcl_attr_upgradeServerID)) {
        addrExt_t *pExtAddr = tl_zbExtAddrPtrByShortAddr(pAddrInfo->srcAddr);
        if (pExtAddr && !ZB_IEEE_ADDR_CMP(zcl_attr_upgradeServerID, pExtAddr)) {
            return ZCL_STA_CMD_HAS_RESP;
        }
    }

    if (zcl_attr_imageUpgradeStatus != IMAGE_UPGRADE_STATUS_NORMAL) {
        return ZCL_STA_FAILURE;
    }

    if (ZB_NWK_IS_ADDRESS_BROADCAST(pAddrInfo->dstAddr)) {
        if ((pImageNotify->payloadType >= IMAGE_NOTIFY_QUERT_JITTER_MFG) &&
            (pImageNotify->manuCode != g_otaCtx.pOtaPreamble->manufacturerCode)) {
            return ZCL_STA_SUCCESS;
        }
        if ((pImageNotify->payloadType >= IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE) &&
            (pImageNotify->imageType != g_otaCtx.pOtaPreamble->imageType)) {
            return ZCL_STA_SUCCESS;
        }
        if ((pImageNotify->payloadType >= IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE_VER) &&
            (pImageNotify->newFileVer == zcl_attr_currFileVer)) {
            return ZCL_STA_SUCCESS;
        }

        if ((zb_random() % 100) > pImageNotify->queryJitter) {
            return ZCL_STA_SUCCESS;
        }
    }

    if (pAddrInfo->apsSec) {
        g_otaCtx.otaServerEpInfo.txOptions |= APS_TX_OPT_SECURITY_ENABLED;
    } else {
        g_otaCtx.otaServerEpInfo.txOptions &= (u8)(~(u16)APS_TX_OPT_SECURITY_ENABLED);
    }

    ota_queryNextImageReq(pAddrInfo->srcEp, pAddrInfo->srcAddr, pAddrInfo->profileId);

    return ZCL_STA_CMD_HAS_RESP;
}

static status_t ota_queryNextImageRspHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_queryNextImageRsp_t *pQueryNextImageRsp)
{
    if (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_IN_PROGRESS) {
        return ZCL_STA_SUCCESS;
    }

    g_otaCtx.otaServerEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    g_otaCtx.otaServerEpInfo.dstAddr.shortAddr = pAddrInfo->srcAddr;
    g_otaCtx.otaServerEpInfo.dstEp = pAddrInfo->srcEp;
    g_otaCtx.otaServerEpInfo.profileId = pAddrInfo->profileId;
    g_otaCtx.otaServerEpInfo.radius = 0;

    g_otaCtx.imageBlockRetry = 0;

    if ((pQueryNextImageRsp->st == ZCL_STA_SUCCESS) &&
        (pQueryNextImageRsp->imageType == g_otaCtx.pOtaPreamble->imageType) &&
        (pQueryNextImageRsp->manuCode == g_otaCtx.pOtaPreamble->manufacturerCode)) {

#ifdef ZCL_WWAH
        bool disable = FALSE;
        u16 len;

        if (zcl_getAttrVal(pAddrInfo->dstEp, ZCL_CLUSTER_WWAH, ZCL_ATTRID_WWAH_DISABLE_OTA_DOWNGRADES, &len, (u8 *)&disable) == ZCL_STA_SUCCESS) {
            if (disable && (pQueryNextImageRsp->fileVer < g_otaCtx.pOtaPreamble->fileVer)) {
                ota_upgradeEndReq_t req;

                req.st = ZCL_STA_INVALID_IMAGE;
                req.fileVer = pQueryNextImageRsp->fileVer;
                req.imageType = pQueryNextImageRsp->imageType;
                req.manuCode = pQueryNextImageRsp->manuCode;

                ota_upgradeEndReqSend(&req);

                return ZCL_STA_SUCCESS;
            }
        }
#endif

        if (pQueryNextImageRsp->fileVer == g_otaCtx.pOtaPreamble->fileVer) {
            return ZCL_STA_SUCCESS;
        }

        if (pQueryNextImageRsp->imageSize > FLASH_OTA_IMAGE_MAX_SIZE) {
            return ZCL_STA_INSUFFICIENT_SPACE;
        }

        zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_DOWNLOAD_IN_PROGRESS;

        //stop server query start timer
        ev_unon_timer(&otaTimer);

#if FLASH_PROTECT_ENABLE
        flash_unlock();
#endif

        if (g_otaCtx.downloadImageSize == pQueryNextImageRsp->imageSize &&
            zcl_attr_imageTypeID == pQueryNextImageRsp->imageType &&
            zcl_attr_downloadFileVer == pQueryNextImageRsp->fileVer &&
            zcl_attr_fileOffset > 4096) {
            /* OTA from offset xxxx */

            if (otaClientInfo.offset == g_otaCtx.downloadImageSize) {
                /* the whole image already has been received, shall validate the image */
                if (otaClientInfo.otaElementPos == otaClientInfo.otaElementLen) {
                    /* validate the CRC */
                    u32 baseAddr = (mcuBootAddr) ? 0 : FLASH_ADDR_OF_OTA_IMAGE;
                    u16 crcFirmware = 0;
                    flash_read(baseAddr + 6, 2, (u8 *)&crcFirmware);
                    if (((crcFirmware & 0xff) == 0x5D) && ((crcFirmware >> 8) & 0xff) == 0x02) {
                        u32 crcReceived;
                        flash_read(baseAddr + otaClientInfo.otaElementLen - otaClientInfo.otaElementInfo2 - 4, 4, (u8 *)&crcReceived);

                        /* if CRC is correct, exit and reboot directly, or restart OTA from 0 */
                        if (crcReceived == otaClientInfo.crcValue) {
                            ota_upgradeComplete(ZCL_STA_SUCCESS);

                            return ZCL_STA_SUCCESS;
                        }
                    }
                }
            } else {
                /* start OTA from the offset of zcl_attr_file Offset */
                ota_sendImageBlockReq(NULL);
                if (otaCb) {
                    otaCb->processMsgCbFunc(OTA_EVT_START, pQueryNextImageRsp->st);
                }
                return ZCL_STA_SUCCESS;
            }
        }

        /* OTA from offset 0*/
        zcl_attr_fileOffset = 0;

        otaClientInfo.crcValue = 0xffffffff;

        zcl_attr_imageTypeID = pQueryNextImageRsp->imageType;
        zcl_attr_downloadFileVer = pQueryNextImageRsp->fileVer;
        g_otaCtx.downloadImageSize = pQueryNextImageRsp->imageSize;

        //double check the boot-up address to prevent unexpected
        if (mcuBootAddr != mcuBootAddrGet()) {
#if FLASH_PROTECT_ENABLE
            flash_lock();
#endif
            ZB_EXCEPTION_POST(SYS_EXCEPTTION_COMMON_BOOT_ADDR_ERROR);
            return ZCL_STA_ABORT;
        }

        u16 sectorNumUsed = g_otaCtx.downloadImageSize / FLASH_SECTOR_SIZE + 1;
        u32 baseAddr = (mcuBootAddr) ? 0 : FLASH_ADDR_OF_OTA_IMAGE;

        for (u16 i = 0; i < sectorNumUsed; i++) {
            flash_erase(baseAddr + i * FLASH_SECTOR_SIZE);
        }

        pOtaUpdateInfo = (ota_updateInfo_t *)ev_buf_allocate(sizeof(ota_updateInfo_t));
        if (!pOtaUpdateInfo) {
#if FLASH_PROTECT_ENABLE
            flash_lock();
#endif
            return ZCL_STA_INSUFFICIENT_SPACE;
        }

        memset((u8 *)pOtaUpdateInfo, 0, sizeof(ota_updateInfo_t));

        if (ZB_IEEE_ADDR_IS_INVALID(zcl_attr_upgradeServerID)) {
            TL_SCHEDULE_TASK(ota_ieeeAddrReqSend, NULL);
        } else {
            ota_sendImageBlockReq(NULL);
            otaClientInfo.clientOtaFlg = OTA_FLAG_IMAGE_MAGIC_0;
            if (otaCb) {
                otaCb->processMsgCbFunc(OTA_EVT_START, pQueryNextImageRsp->st);
            }
        }
    }

    return ZCL_STA_SUCCESS;
}

static status_t ota_imageBlockRspHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_imageBlockRsp_t *pImageBlockRsp)
{
    u8 status = ZCL_STA_SUCCESS;

    ota_upgradeEndReq_t req;

    if (zcl_attr_imageUpgradeStatus != IMAGE_UPGRADE_STATUS_DOWNLOAD_IN_PROGRESS) {
        return ZCL_STA_SUCCESS;
    }

    if (pImageBlockRsp->st == ZCL_STA_SUCCESS) {
        if ((pImageBlockRsp->rsp.success.imageType != g_otaCtx.pOtaPreamble->imageType) ||
            (pImageBlockRsp->rsp.success.manuCode != g_otaCtx.pOtaPreamble->manufacturerCode) ||
            (pImageBlockRsp->rsp.success.fileVer != zcl_attr_downloadFileVer)) {
            status = ZCL_STA_INVALID_IMAGE;
        } else {
            if (pImageBlockRsp->rsp.success.fileOffset != zcl_attr_fileOffset) {
                return ZCL_STA_SUCCESS;
            }

            //stop image block rsp wait timer
            ev_unon_timer(&otaTimer);
            //clear image block req retry count
            g_otaCtx.imageBlockRetry = 0;

            status = ota_imageDataProcess(pImageBlockRsp->rsp.success.dataSize,
                                          pImageBlockRsp->rsp.success.data);

            if (status == ZCL_STA_SUCCESS) {
                //save download code info
                TL_SCHEDULE_TASK(ota_saveUpdateInfo2NV, NULL);

                if (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE) {
                    req.st = ZCL_STA_SUCCESS;
                    req.fileVer = pImageBlockRsp->rsp.success.fileVer;
                    req.imageType = pImageBlockRsp->rsp.success.imageType;
                    req.manuCode = pImageBlockRsp->rsp.success.manuCode;

                    ota_upgradeEndReqSend(&req);

                    g_otaCtx.upgradeEndRetry = 0;
                    ota_upgradeWait(OTA_MAX_IMAGE_BLOCK_RSP_WAIT_TIME);
                } else {
                    ota_sendImageBlockReq(NULL);
                }
            } else {
                ota_upgradeComplete(status);
            }
        }
    } else if (pImageBlockRsp->st == ZCL_STA_WAIT_FOR_DATA) {
        //stop the block rsp wait timer
        ev_unon_timer(&otaTimer);

        //check if device supports block req delay rate limiting
        if ((OTA_IMAGE_BLOCK_FC & BLOCK_FC_BITMASK_MIN_PERIOD_PRESENT) != 0) {
            if (pImageBlockRsp->rsp.wait.reqTime - pImageBlockRsp->rsp.wait.currTime) {
                //clear the retry count
                g_otaCtx.imageBlockRetry = 0;

                ota_imageBlockWait(pImageBlockRsp->rsp.wait.reqTime - pImageBlockRsp->rsp.wait.currTime);
            } else {
                //if wait timer delta is 0, then update device with block req delay value and use rate limiting
                zcl_attr_minBlockPeriod = pImageBlockRsp->rsp.wait.blockReqDelay;
                ota_sendImageBlockReq(NULL);
            }
        } else {
            //clear the retry count
            g_otaCtx.imageBlockRetry = 0;

            ota_imageBlockWait(pImageBlockRsp->rsp.wait.reqTime - pImageBlockRsp->rsp.wait.currTime);
        }
    } else if (pImageBlockRsp->st == ZCL_STA_ABORT) {
        zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_NORMAL;

        //stop the block rsp wait timer
        ev_unon_timer(&otaTimer);

        //clear the retry count
        g_otaCtx.imageBlockRetry = 0;

        ota_upgradeComplete(ZCL_STA_ABORT);

        return ZCL_STA_SUCCESS;
    } else {
        return ZCL_STA_MALFORMED_COMMAND;
    }

    if (status != ZCL_STA_SUCCESS) {
        zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_NORMAL;

        req.st = status;
        req.fileVer = pImageBlockRsp->rsp.success.fileVer;
        req.imageType = pImageBlockRsp->rsp.success.imageType;
        req.manuCode = pImageBlockRsp->rsp.success.manuCode;

        ota_upgradeEndReqSend(&req);
    }

    return ZCL_STA_SUCCESS;
}

static status_t ota_upgradeEndRspHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_upgradeEndRsp_t *pUpgradeEndRsp)
{
    if ((zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE) ||
       ((zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_WAITING_TO_UPGRADE) &&
        (pUpgradeEndRsp->upgradeTime != 0xFFFFFFFF))) {

        if (ota_fileIdCmp(g_otaCtx.pOtaPreamble->manufacturerCode, pUpgradeEndRsp->manuCode,
                          g_otaCtx.pOtaPreamble->imageType, pUpgradeEndRsp->imageType,
                          zcl_attr_downloadFileVer, pUpgradeEndRsp->fileVer) == FALSE) {
            return ZCL_STA_SUCCESS;
        }

#if 0
        zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_COUNT_DOWN;

        u32 jitter = zb_random();
        while (1) {
            if ((jitter >= 5) && (jitter <= 10)) {
                break;
            } else {
                jitter = zb_random();
            }
        }

        ota_upgradeWait(jitter);
#else
        if (pUpgradeEndRsp->upgradeTime != 0xFFFFFFFF) {
            u32 notifyDelay = 0;
            if (pUpgradeEndRsp->upgradeTime > pUpgradeEndRsp->currentTime) {
                //time to wait before notification
                notifyDelay = pUpgradeEndRsp->upgradeTime - pUpgradeEndRsp->currentTime;
            }

            zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_COUNT_DOWN;

            ota_upgradeWait(notifyDelay);
        } else {
            //wait for another upgrade end rsp
            zcl_attr_imageUpgradeStatus = IMAGE_UPGRADE_STATUS_WAITING_TO_UPGRADE;

            //set a timer for 60 minutes to send another upgrade end req
            ota_upgradeWait(3600);

            g_otaCtx.upgradeEndRetry = 0;
        }

        if (otaCb) {
            otaCb->processMsgCbFunc(OTA_EVT_IMAGE_DONE, zcl_attr_imageUpgradeStatus);
        }
#endif
    }

    return ZCL_STA_SUCCESS;
}

static status_t ota_queryDevSpecFileRspHandler(zclIncomingAddrInfo_t *pAddrInfo, ota_querySpecFileRsp_t *pQueryDevSpecFileRsp)
{
    return ZCL_STA_UNSUP_CLUSTER_COMMAND;
}

status_t zcl_otaCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
    status_t status = ZCL_STA_SUCCESS;

    if (pAddrInfo->dstEp == g_otaCtx.simpleDesc->endpoint) {
        if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR) {
            switch (cmdId) {
            case ZCL_CMD_OTA_QUERY_NEXT_IMAGE_REQ:
                status = ota_queryNextImageReqHandler(pAddrInfo, (ota_queryNextImageReq_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_IMAGE_BLOCK_REQ:
                status = ota_imageBlockReqHandler(pAddrInfo, (ota_imageBlockReq_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_IMAGE_PAGE_REQ:
                status = ota_imagePageReqHandler(pAddrInfo, (ota_imagePageReq_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_UPGRADE_END_REQ:
                status = ota_upgradeEndReqHandler(pAddrInfo, (ota_upgradeEndReq_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_REQ:
                status = ota_queryDevSpecFileReqHandler(pAddrInfo, (ota_querySpecFileReq_t *)cmdPayload);
                break;
            default:
                status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
                break;
            }
        } else {
            switch (cmdId) {
            case ZCL_CMD_OTA_IMAGE_NOTIFY:
                status = ota_imageNotifyHandler(pAddrInfo, (ota_imageNotify_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_QUERY_NEXT_IMAGE_RSP:
                status = ota_queryNextImageRspHandler(pAddrInfo, (ota_queryNextImageRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_IMAGE_BLOCK_RSP:
                status = ota_imageBlockRspHandler(pAddrInfo, (ota_imageBlockRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_UPGRADE_END_RSP:
                status = ota_upgradeEndRspHandler(pAddrInfo, (ota_upgradeEndRsp_t *)cmdPayload);
                break;
            case ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_RSP:
                status = ota_queryDevSpecFileRspHandler(pAddrInfo, (ota_querySpecFileRsp_t *)cmdPayload);
                break;
            default:
                status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
                break;
            }
        }
    }

    return status;
}

#endif	/* ZCL_OTA */
