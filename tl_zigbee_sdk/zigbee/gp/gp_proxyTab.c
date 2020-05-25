/********************************************************************************************************
 * @file     gp_proxyTab.c
 *
 * @brief	 Coordinator and Router should support GPPB.
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
#include "../include/zb_common.h"
#include "../zcl/zcl_include.h"
#include "gp.h"


#if GP_SUPPORT_ENABLE

gp_proxyTab_t g_gpProxyTab;


void zclGpProxyTabAttrUpdate(void);


void gp_proxyTabSave2Flash(void *arg)
{
#if NV_ENABLE
	nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_GP_PROXY_TABLE, sizeof(gp_proxyTab_t), (u8*)&g_gpProxyTab);
#endif
}

nv_sts_t gp_proxyTabRestoreFromFlash(void)
{
	u8 ret = NV_SUCC;

#if NV_ENABLE
	ret = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_GP_PROXY_TABLE, sizeof(gp_proxyTab_t), (u8*)&g_gpProxyTab);
#endif

	return ret;
}


void gp_proxyTabEntryClear(gpProxyTabEntry_t *pEntry)
{
	if(pEntry->used){
		memset((u8 *)pEntry, 0, sizeof(*pEntry));
		g_gpProxyTab.gpProxyTabNum--;
	}
}

void gp_ProxyTabInit(void)
{
	if(gp_proxyTabRestoreFromFlash() == NV_SUCC){
		zclGpProxyTabAttrUpdate();
	}else{
		memset((u8 *)&g_gpProxyTab, 0, sizeof(gp_proxyTab_t));
	}
}

u8 gp_getProxyTabEntryTotalNum(void)
{
	return g_gpProxyTab.gpProxyTabNum;
}

u8 gp_getProxyTabEntryLen(gpProxyTabEntry_t *pEntry)
{
	u8 entryLen = 2;//options

	if(pEntry->options.bits.appId == GP_APP_ID_SRC_ID){
		entryLen += 4;
	}else if(pEntry->options.bits.appId == GP_APP_ID_GPD){
		entryLen += EXT_ADDR_LEN;
		entryLen += 1;//endpoint
	}

	if(pEntry->options.bits.assignedAlias){
		entryLen += sizeof(u16);
	}

	if(pEntry->options.bits.secUse){
		entryLen += sizeof(u8);
	}

	if((pEntry->options.bits.secUse) || (pEntry->options.bits.seqNumCap)){
		entryLen += sizeof(u32);
	}

	if(pEntry->options.bits.secUse){
		entryLen += SEC_KEY_LEN;
	}

	if(pEntry->options.bits.lightWeightUnicastGPS){
		entryLen += (pEntry->lwSinkCnt) ? 1 : 0;
		for(u8 i = 0; i < pEntry->lwSinkCnt; i++){
			entryLen += 10;
		}
	}

	if(pEntry->options.bits.commGroupGPS){
		entryLen += (pEntry->sinkGroupCnt) ? 1 : 0;
		for(u8 i = 0; i < pEntry->sinkGroupCnt; i++){
			entryLen += 4;
		}
	}

	entryLen += sizeof(u8);//Groupcast radius

	if(!pEntry->options.bits.entryActive || !pEntry->options.bits.entryValid){
		entryLen += sizeof(u8);
	}

	return entryLen;
}

u8 gp_buildProxyTabEntryFormat(gpProxyTabEntry_t *pEntry, u8 *pBuf)
{
	u8 *ptr = pBuf;

	//Options
	*ptr++ = LO_UINT16(pEntry->options.opts);
	*ptr++ = HI_UINT16(pEntry->options.opts);

	//GPD ID and Endpoint
	if(pEntry->options.bits.appId == GP_APP_ID_SRC_ID){
		*ptr++ = U32_BYTE0(pEntry->gpdId.srcId);
		*ptr++ = U32_BYTE1(pEntry->gpdId.srcId);
		*ptr++ = U32_BYTE2(pEntry->gpdId.srcId);
		*ptr++ = U32_BYTE3(pEntry->gpdId.srcId);
	}else if(pEntry->options.bits.appId == GP_APP_ID_GPD){
		ZB_64BIT_ADDR_COPY(ptr, pEntry->gpdId.gpdIeeeAddr);
		ptr += EXT_ADDR_LEN;
		*ptr++ = pEntry->endpoint;
	}

	//GPD Assigned Alias
	if(pEntry->options.bits.assignedAlias){
		*ptr++ = LO_UINT16(pEntry->gpdAssignedAlias);
		*ptr++ = HI_UINT16(pEntry->gpdAssignedAlias);
	}

	//Security Options
	if(pEntry->options.bits.secUse){
		*ptr++ = pEntry->secOptions.opts;
	}

	//GPD security frame counter
	if((pEntry->options.bits.secUse) || (pEntry->options.bits.seqNumCap)){
		*ptr++ = U32_BYTE0(pEntry->gpdSecFrameCnt);
		*ptr++ = U32_BYTE1(pEntry->gpdSecFrameCnt);
		*ptr++ = U32_BYTE2(pEntry->gpdSecFrameCnt);
		*ptr++ = U32_BYTE3(pEntry->gpdSecFrameCnt);
	}

	//GPD key
	if(pEntry->options.bits.secUse){
		memcpy(ptr, pEntry->gpdKey, SEC_KEY_LEN);
		ptr += SEC_KEY_LEN;
	}

	//Lightweight sink address list
	if(pEntry->options.bits.lightWeightUnicastGPS){
		*ptr++ = pEntry->lwSinkCnt;
		for(u8 i = 0; i < pEntry->lwSinkCnt; i++){
			ZB_64BIT_ADDR_COPY(ptr, pEntry->lightweightSinkAddrList[i].sinkIeeeAddr);
			ptr += EXT_ADDR_LEN;
			*ptr++ = LO_UINT16(pEntry->lightweightSinkAddrList[i].sinkNwkAddr);
			*ptr++ = HI_UINT16(pEntry->lightweightSinkAddrList[i].sinkNwkAddr);
		}
	}

	//Sink group list
	if(pEntry->options.bits.commGroupGPS){
		*ptr++ = pEntry->sinkGroupCnt;
		for(u8 i = 0; i < pEntry->sinkGroupCnt; i++){
			*ptr++ = LO_UINT16(pEntry->sinkGroupList[i].groupId);
			*ptr++ = HI_UINT16(pEntry->sinkGroupList[i].groupId);
			*ptr++ = LO_UINT16(pEntry->sinkGroupList[i].alias);
			*ptr++ = HI_UINT16(pEntry->sinkGroupList[i].alias);
		}
	}

	//Groupcast radius
	*ptr++ = pEntry->groupcastRadius;

	//Search Counter
	if(!pEntry->options.bits.entryActive || !pEntry->options.bits.entryValid){
		*ptr++ = pEntry->searchCnt;
	}

	return (ptr - pBuf);
}

gpProxyTabEntry_t *gp_getProxyTabByGpdId(u8 appId, gpdId_t gpdId)
{
	gpProxyTabEntry_t *pEntry = NULL;

	if(g_gpProxyTab.gpProxyTabNum == 0){
		return NULL;
	}

	for(u8 i = 0; i < zclGpAttr_gppMaxProxyTabEntries; i++){
		pEntry = &g_gpProxyTab.gpProxyTab[i];
		if(pEntry->used){
			if(pEntry->options.bits.appId == appId){
				if((appId == GP_APP_ID_SRC_ID) && !memcmp((u8 *)&gpdId.srcId, (u8 *)&pEntry->gpdId.srcId, SRC_ID_LEN)){
					return pEntry;
				}else if((appId == GP_APP_ID_GPD) && !memcmp((u8 *)&gpdId.gpdIeeeAddr, (u8 *)&pEntry->gpdId.gpdIeeeAddr, EXT_ADDR_LEN)){
					return pEntry;
				}
			}
		}
	}

	return NULL;
}

gpProxyTabEntry_t *gp_proxyTabEntryFreeGet(void)
{
	gpProxyTabEntry_t *pEntry = NULL;

	if(g_gpProxyTab.gpProxyTabNum >= zclGpAttr_gppMaxProxyTabEntries){
		return NULL;
	}

	for(u8 i = 0; i < zclGpAttr_gppMaxProxyTabEntries; i++){
		pEntry = &g_gpProxyTab.gpProxyTab[i];
		if(!pEntry->used){
			pEntry->gpdSecFrameCnt = 0xffffffff;
			pEntry->groupcastRadius = 0xff;
			return pEntry;
		}
	}

	return NULL;
}

u8 lwSinkAddrListAdd(gpProxyTabEntry_t *pEntry, addrExt_t sinkIeeeAddr, u16 sinkNwkAddr)
{
	if(pEntry->lwSinkCnt == 0){
		//add new sink address list item
		ZB_64BIT_ADDR_COPY(pEntry->lightweightSinkAddrList[0].sinkIeeeAddr, sinkIeeeAddr);
		pEntry->lightweightSinkAddrList[0].sinkNwkAddr = sinkNwkAddr;
		pEntry->lwSinkCnt++;
		return SUCCESS;
	}else{
		for(u8 i = 0; i < pEntry->lwSinkCnt; i++){
			if( ZB_64BIT_ADDR_CMP(pEntry->lightweightSinkAddrList[i].sinkIeeeAddr, sinkIeeeAddr) &&
				(pEntry->lightweightSinkAddrList[i].sinkNwkAddr == sinkNwkAddr) ){
				//find item
				return SUCCESS;
			}
		}

		if(pEntry->lwSinkCnt < PROXY_LIGHTWEIGHT_SINK_ADDR_LIST_NUM){
			ZB_64BIT_ADDR_COPY(pEntry->lightweightSinkAddrList[pEntry->lwSinkCnt].sinkIeeeAddr, sinkIeeeAddr);
			pEntry->lightweightSinkAddrList[pEntry->lwSinkCnt].sinkNwkAddr = sinkNwkAddr;
			pEntry->lwSinkCnt++;
			return SUCCESS;
		}
	}

	return FAILURE;
}

u8 lwSinkAddrListRemove(gpProxyTabEntry_t *pEntry, addrExt_t sinkIeeeAddr, u16 sinkNwkAddr)
{
	if(pEntry->lwSinkCnt){
		for(u8 i = 0; i < pEntry->lwSinkCnt; i++){
			if( ZB_64BIT_ADDR_CMP(pEntry->lightweightSinkAddrList[i].sinkIeeeAddr, sinkIeeeAddr) &&
				(pEntry->lightweightSinkAddrList[i].sinkNwkAddr == sinkNwkAddr) ){
				//find item
				if(i == (pEntry->lwSinkCnt - 1)){
					ZB_64BIT_ADDR_ZERO(pEntry->lightweightSinkAddrList[i].sinkIeeeAddr);
					pEntry->lightweightSinkAddrList[i].sinkNwkAddr = 0;
				}else{
					ZB_64BIT_ADDR_COPY(pEntry->lightweightSinkAddrList[i].sinkIeeeAddr, pEntry->lightweightSinkAddrList[pEntry->lwSinkCnt - 1].sinkIeeeAddr);
					pEntry->lightweightSinkAddrList[i].sinkNwkAddr = pEntry->lightweightSinkAddrList[pEntry->lwSinkCnt - 1].sinkNwkAddr;

					ZB_64BIT_ADDR_ZERO(pEntry->lightweightSinkAddrList[pEntry->lwSinkCnt - 1].sinkIeeeAddr);
					pEntry->lightweightSinkAddrList[pEntry->lwSinkCnt - 1].sinkNwkAddr = 0;
				}
				pEntry->lwSinkCnt--;

				return SUCCESS;
			}
		}
	}

	return FAILURE;
}

u8 sinkGroupListAdd(gpProxyTabEntry_t *pEntry, u16 sinkGroupID, u16 alias)
{
	if(pEntry->sinkGroupCnt == 0){
		//add new item
		pEntry->sinkGroupList[0].groupId = sinkGroupID;
		pEntry->sinkGroupList[0].alias = alias;
		pEntry->sinkGroupCnt++;
		return SUCCESS;
	}else{
		for(u8 i = 0; i < pEntry->sinkGroupCnt; i++){
			if(pEntry->sinkGroupList[i].groupId == sinkGroupID){
				//find item
				pEntry->sinkGroupList[i].alias = alias;
				return SUCCESS;
			}
		}

		if(pEntry->sinkGroupCnt < PROXY_SINK_GROUP_LIST_NUM){
			pEntry->sinkGroupList[pEntry->sinkGroupCnt].groupId = sinkGroupID;
			pEntry->sinkGroupList[pEntry->sinkGroupCnt].alias = alias;
			pEntry->sinkGroupCnt++;
			return SUCCESS;
		}
	}

	return FAILURE;
}

u8 sinkGroupListRemove(gpProxyTabEntry_t *pEntry, u16 sinkGroupID)
{
	if(pEntry->sinkGroupCnt){
		for(u8 i = 0; i < pEntry->sinkGroupCnt; i++){
			if(pEntry->sinkGroupList[i].groupId == sinkGroupID){
				//find item
				if(i == (pEntry->sinkGroupCnt - 1)){
					pEntry->sinkGroupList[i].groupId = 0;
					pEntry->sinkGroupList[i].alias = 0;
				}else{
					pEntry->sinkGroupList[i].groupId = pEntry->sinkGroupList[pEntry->sinkGroupCnt -1].groupId;
					pEntry->sinkGroupList[i].alias = pEntry->sinkGroupList[pEntry->sinkGroupCnt -1].alias;

					pEntry->sinkGroupList[pEntry->sinkGroupCnt -1].groupId = 0;
					pEntry->sinkGroupList[pEntry->sinkGroupCnt -1].alias = 0;
				}
				pEntry->sinkGroupCnt--;

				return SUCCESS;
			}
		}
	}

	return FAILURE;
}

void zclGpProxyTabAttrUpdate(void)
{
	u8 *pBuf = zclGpAttr_proxyTabEntry;

	if(g_gpProxyTab.gpProxyTabNum == 0){
		//len = 0
		*pBuf++ = 0;
		*pBuf++ = 0;
	}else{
		u8 entryLen = 0;
		u8 maxEntryLen = 0;
		u8 entryCnt = 0;

		for(u8 i = 0; i < zclGpAttr_gppMaxProxyTabEntries; i++){
			maxEntryLen = entryLen;

			if(g_gpProxyTab.gpProxyTab[i].used){
				entryLen += gp_getProxyTabEntryLen(&g_gpProxyTab.gpProxyTab[i]);

				if(entryLen >= ZCL_GP_MAX_PROXY_TABLE_ATTR_LEN){
					entryLen = maxEntryLen;
					break;
				}else{
					entryCnt++;
				}
			}
		}

		*pBuf++ = LO_UINT16(entryLen);
//		*pBuf++ = HI_UINT16(entryLen);
		*pBuf++ = 0; //HI_UINT16(entryLen) always is 0

		for(u8 i = 0; i < entryCnt; i++){
			if(g_gpProxyTab.gpProxyTab[i].used){
				pBuf += gp_buildProxyTabEntryFormat(&g_gpProxyTab.gpProxyTab[i], pBuf);
			}
		}
	}
}

void gpProxyTabUpdate(void)
{
	zclGpProxyTabAttrUpdate();

	//write to NV
	TL_SCHEDULE_TASK(gp_proxyTabSave2Flash, NULL);
}


#endif
