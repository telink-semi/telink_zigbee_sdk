/********************************************************************************************************
 * @file	zcl_ota.h
 *
 * @brief	This is the header file for zcl_ota
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
#ifndef ZCL_OTA_H
#define ZCL_OTA_H



/**
 *  @brief	zcl_ota_cmdId OTA Command Ids
 */
#define ZCL_CMD_OTA_IMAGE_NOTIFY						0x00
#define ZCL_CMD_OTA_QUERY_NEXT_IMAGE_REQ				0x01
#define ZCL_CMD_OTA_QUERY_NEXT_IMAGE_RSP				0x02
#define ZCL_CMD_OTA_IMAGE_BLOCK_REQ						0x03
#define ZCL_CMD_OTA_IMAGE_PAGE_REQ						0x04
#define ZCL_CMD_OTA_IMAGE_BLOCK_RSP						0x05
#define ZCL_CMD_OTA_UPGRADE_END_REQ						0x06
#define ZCL_CMD_OTA_UPGRADE_END_RSP						0x07
#define ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_REQ		0x08
#define ZCL_CMD_OTA_QUERY_DEVICE_SPECIFIC_FILE_RSP		0x09

/**
 *  @brief	attributes of OTA upgrade cluster
 */
#define ZCL_ATTRID_OTA_UPGRADE_SERVER_ID				0x0000
#define ZCL_ATTRID_OTA_FILE_OFFSET						0x0001
#define ZCL_ATTRID_OTA_CURRENT_FILE_VER					0x0002
#define ZCL_ATTRID_OTA_CURRENT_ZIGBEE_STACK_VER			0x0003
#define ZCL_ATTRID_OTA_DOWNLOAD_FILE_VER				0x0004
#define ZCL_ATTRID_OTA_DOWNLOAD_ZIGBEE_STACK_VER		0x0005
#define ZCL_ATTRID_OTA_IMAGE_UPGRADE_STAUS				0x0006
#define ZCL_ATTRID_OTA_MANUFACTURER_ID					0x0007
#define ZCL_ATTRID_OTA_IMAGE_TYPE_ID					0x0008
#define ZCL_ATTRID_OTA_MINIMUM_BLOCK_PERIOD				0x0009
#define ZCL_ATTRID_OTA_IMAGE_STAMP						0x000a

/**
 *  @brief	image upgrade status attribute values
 */
#define IMAGE_UPGRADE_STATUS_NORMAL						0x00
#define IMAGE_UPGRADE_STATUS_DOWNLOAD_IN_PROGRESS		0x01
#define IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE			0x02
#define IMAGE_UPGRADE_STATUS_WAITING_TO_UPGRADE			0x03
#define IMAGE_UPGRADE_STATUS_COUNT_DOWN					0x04
#define IMAGE_UPGRADE_STATUS_WAIT_FOR_MORE				0x05


//query next image request field control bitmask
#define IMAGE_FC_BITMASK_HARDWARE_VERSION_PRESENT		0x01

//image block request field control bitmask
#define BLOCK_FC_BITMASK_GENERIC						0x00
#define BLOCK_FC_BITMASK_NODE_IEEE_PRESENT				0x01
#define BLOCK_FC_BITMASK_MIN_PERIOD_PRESENT				0x02

//image page request field control bitmask
#define PAGE_FC_BITMASK_NODE_IEEE_PRESENT				0x01

//image notify command payload type
#define IMAGE_NOTIFY_QUERY_JITTER						0x00
#define IMAGE_NOTIFY_QUERT_JITTER_MFG					0x01
#define IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE				0x02
#define IMAGE_NOTIFY_QUERY_JITTER_MFG_TYPE_VER			0x03


typedef struct{
	u8	payloadType;
	u8	queryJitter;
	u16	manuCode;
	u16	imageType;
	u32	newFileVer;
}ota_imageNotify_t;

typedef struct{
	u8	fc;//BIT(0) hardware version present
	u16	manuCode;
	u16	imageType;
	u32	curFileVer;
	u16	hdrwareVer;
}ota_queryNextImageReq_t;

typedef struct{
	u8	st;
	u16	manuCode;
	u16	imageType;
	u32	fileVer;
	u32	imageSize;
}ota_queryNextImageRsp_t;

typedef struct{
	u8	fc;//b0 req node's IEEE address present; b1 block request delay present
	u16	manuCode;
	u16	imageType;
	u32	fileVer;
	u32	fileOffset;
	u8	maxDataSize;
	addrExt_t reqNodeAddr;
	u16	blockReqDelay;
}ota_imageBlockReq_t;

typedef struct{
	u8	fc;//b0 req node's IEEE address present; b1 block request delay present
	u16	manuCode;
	u16	imageType;
	u32	fileVer;
	u32	fileOffset;
	u8	maxDataSize;
	u16	pageSize;
	u16	rspSpacing;
	addrExt_t reqNodeAddr;
}ota_imagePageReq_t;

typedef struct{
	u8	st;
	u16	manuCode;
	u16	imageType;
	u32	fileVer;
}ota_upgradeEndReq_t;

typedef struct{
	u16	manuCode;
	u16	imageType;
	u32	fileVer;
	u32	currentTime;
	u32	upgradeTime;
}ota_upgradeEndRsp_t;

typedef struct{
	addrExt_t		reqNodeAddr;
	u16				manuCode;
	u16				imageType;
	u32				fileVer;
	u16				zbStackVer;
}ota_querySpecFileReq_t;

typedef struct{
	u8				st;
	u16				manuCode;
	u16				imageType;
	u32				fileVer;
	u32				imageSize;
}ota_querySpecFileRsp_t;

typedef struct{
	u16	manuCode;
	u16	imageType;
	u32	fileVer;
	u32	fileOffset;
	u8	dataSize;
	u8	*data;
}ota_imageBlcokRspSuccess_t;

typedef struct{
	u32	currTime;
	u32	reqTime;
	u16	blockReqDelay;
}ota_imageBlcokRspWait_t;

typedef union{
	ota_imageBlcokRspSuccess_t  success;
	ota_imageBlcokRspWait_t 	wait;
}imageBlockRsp_t;

typedef struct{
	u8 st;
	imageBlockRsp_t rsp;
}ota_imageBlockRsp_t;

#if 0
/**
 *  @brief  Definition for ota command callback function type
 */
typedef status_t (*zcl_ota_imageNotifyCb_t)(apsdeDataInd_t *pInd, ota_imageNotify_t *pImageNotify);
typedef status_t (*zcl_ota_queryNextImageReqCb_t)(apsdeDataInd_t *pInd, ota_queryNextImageReq_t *pQueryNextImageReq);
typedef status_t (*zcl_ota_queryNextImageRspCb_t)(apsdeDataInd_t *pInd, ota_queryNextImageRsp_t *pQueryNextImageRsp);
typedef status_t (*zcl_ota_imageBlockReqCb_t)(apsdeDataInd_t *pInd, ota_imageBlockReq_t *pImageBlockReq);
typedef status_t (*zcl_ota_imagePageReqCb_t)(apsdeDataInd_t *pInd, ota_imagePageReq_t *pImagePageReq);
typedef status_t (*zcl_ota_imageBlockRspCb_t)(apsdeDataInd_t *pInd, ota_imageBlockRsp_t *pImageBlockRsp);
typedef status_t (*zcl_ota_upgradeEndReqCb_t)(apsdeDataInd_t *pInd, ota_upgradeEndReq_t *pUpgradeEndReq);
typedef status_t (*zcl_ota_upgradeEndRspCb_t)(apsdeDataInd_t *pInd, ota_upgradeEndRsp_t *pUpgradeEndRsp);
typedef status_t (*zcl_ota_queryDevSpecFileReqCb_t)(apsdeDataInd_t *pInd, ota_querySpecFileReq_t *pQueryDevSpecFileReq);
typedef status_t (*zcl_ota_queryDevSpecFileRspCb_t)(apsdeDataInd_t *pInd, ota_querySpecFileRsp_t *pQueryDevSpecFileRsp);


/**
 *  @brief  Structure definition for commands callback functions in OTA cluster
 */
typedef struct {
	zcl_ota_imageNotifyCb_t			imageNotifyCbFunc;
	zcl_ota_queryNextImageReqCb_t	queryNextImageReqCbFunc;
	zcl_ota_queryNextImageRspCb_t	queryNextImageRspCbFunc;
	zcl_ota_imageBlockReqCb_t		imageBlockReqCbFunc;
	zcl_ota_imagePageReqCb_t		imagePageReqCbFunc;
	zcl_ota_imageBlockRspCb_t		imageBlockRspCbFunc;
	zcl_ota_upgradeEndReqCb_t		upgradeEndReqCbFunc;
	zcl_ota_upgradeEndRspCb_t		upgradeEndRspCbFunc;
	zcl_ota_queryDevSpecFileReqCb_t	queryDevSpecFileReqCbFunc;
	zcl_ota_queryDevSpecFileRspCb_t	queryDevSpecFileRspCbFunc;
}zcl_ota_AppCallbacks_t;


/**
 *  @brief  External variable for OTA Attribute
 */
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

/**
 *  @brief  External variable for OTA Attribute info table
 */
extern const zclAttrInfo_t ota_attrTbl[];
extern const u8 zcl_ota_attrNum;
#endif

status_t zcl_ota_register(u8 endpoint, u8 arrtNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

status_t zcl_ota_imageNotifySend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imageNotify_t *pReq);
#define zcl_ota_imageNotifyCmdSend(a,b,c,d)	(zcl_ota_imageNotifySend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_ota_queryNextImageReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_queryNextImageReq_t *pReq);
#define zcl_ota_queryNextImageReqCmdSend(a,b,c,d)	(zcl_ota_queryNextImageReqSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_ota_queryNextImageRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_queryNextImageRsp_t *pRsp);
#define zcl_ota_queryNextImageRspCmdSend(a,b,c,d)	(zcl_ota_queryNextImageRspSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_ota_imageBlockReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imageBlockReq_t *pReq);
#define zcl_ota_imageBlockReqCmdSend(a,b,c,d)	(zcl_ota_imageBlockReqSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_ota_imagePageReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imagePageReq_t *pReq);

status_t zcl_ota_imageBlockRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_imageBlockRsp_t *pRsp);
#define zcl_ota_imageBlockRspCmdSend(a,b,c,d)	(zcl_ota_imageBlockRspSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_ota_upgradeEndReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_upgradeEndReq_t *pReq);
#define zcl_ota_upgradeEndReqCmdSend(a,b,c,d)	(zcl_ota_upgradeEndReqSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_ota_upgradeEndRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_upgradeEndRsp_t *pRsp);
#define zcl_ota_upgradeEndRspCmdSend(a,b,c,d)	(zcl_ota_upgradeEndRspSend((a), (b), (c), ZCL_SEQ_NUM, (d)))

status_t zcl_ota_queryDevSpecFileReqSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_querySpecFileReq_t *pReq);

status_t zcl_ota_queryDevSpecFileRspSend(u8 srcEp, epInfo_t *pDstEpInfo, u8 disableDefaultRsp, u8 seqNo, ota_querySpecFileRsp_t *pRsp);


#endif	/* ZCL_OTA_H */
