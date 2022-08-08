/********************************************************************************************************
 * @file    gp_trans.c
 *
 * @brief   This is the source file for gp_trans
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

/**********************************************************************
 * INCLUDES
 */
#include "../zcl/zcl_include.h"
#include "gp.h"
#include "gp_sink.h"
#include "gp_trans.h"

#if (GP_SUPPORT_ENABLE && GP_BASIC_COMBO)
/**********************************************************************
 * GLOBAL VARIABLES
 */
gp_translationTab_t g_gpTransTab;

/**********************************************************************
 * FUNCTIONS
 */
void gp_transTabSave2Flash(void *arg)
{
#if NV_ENABLE
	nv_flashWriteNew(1, NV_MODULE_APP, NV_ITEM_APP_GP_TRANS_TABLE, sizeof(gp_translationTab_t), (u8*)&g_gpTransTab);
#endif
}

nv_sts_t gp_transTabRestoreFromFlash(void)
{
	u8 ret = NV_SUCC;

#if NV_ENABLE
	ret = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_GP_TRANS_TABLE, sizeof(gp_translationTab_t), (u8*)&g_gpTransTab);
#endif

	return ret;
}

void gp_transTabNvUpdate(void)
{
	TL_SCHEDULE_TASK(gp_transTabSave2Flash, NULL);
}

void gp_transTabInit(void)
{
	if(gp_transTabRestoreFromFlash() != NV_SUCC){
		memset((u8 *)&g_gpTransTab, 0, sizeof(g_gpTransTab));
	}
}


u8 gp_transTabNumGet(void)
{
	return g_gpTransTab.transTabNum;
}

void gp_transTabEntryClear(transTabEntry_t *pTransEntry)
{
	if(pTransEntry && pTransEntry->used){
		//printf("transClr: gpdCmd = %x, zbCmd = %x\n", pTransEntry->gpdCommand, pTransEntry->zbCommandId);

		memset((u8 *)pTransEntry, 0, sizeof(transTabEntry_t));
		g_gpTransTab.transTabNum--;

		//printf("transTabNum = %d\n", g_gpTransTab.transTabNum);
	}
}

u8 gp_transTabEntryLenGet(transTabEntry_t *pTransEntry)
{
	u8 entryLen = 0;

	if(pTransEntry->options.bits.appId == GP_APP_ID_SRC_ID){
		entryLen += 4;
	}else if(pTransEntry->options.bits.appId == GP_APP_ID_GPD){
		entryLen += EXT_ADDR_LEN;
		entryLen += 1;//GPD endpoint
	}

	entryLen += 2;//GPD command + endpoint
	entryLen += 4;//zigbee profile + zigbee cluster

	entryLen += 2;//zigbee commandId + payloadLen

	if(pTransEntry->zbCmdPayloadLen &&
	  (pTransEntry->zbCmdPayloadLen != 0xfe) &&
	  (pTransEntry->zbCmdPayloadLen != 0xff)){
		entryLen += pTransEntry->zbCmdPayloadLen;
	}

	if(pTransEntry->options.bits.addInfoPresent){
		entryLen += 1;//additional info block length

		if((pTransEntry->gpdCommand == GPDF_CMD_ID_ANY_SENSOR_REPORT_COMMAND) ||
		  ((pTransEntry->gpdCommand >= GPDF_CMD_ID_ATTR_REPORT) &&
		   (pTransEntry->gpdCommand <= GPDF_CMD_ID_MANU_MULTI_CLUSTER_REPORT)) ||
		   (pTransEntry->gpdCommand == GPDF_CMD_ID_COMPACT_ATTR_REPORT)){
			if(pTransEntry->optRecordUsed){
				entryLen += 1;//option selector

				if(pTransEntry->optRecord.compactReport.report.attrOpt.bits.manuIdPresent){
					entryLen += sizeof(compactAttrReport_t);
				}else{
					entryLen += (sizeof(compactAttrReport_t) - 2);
				}
			}
		}else if((pTransEntry->gpdCommand == GPDF_CMD_ID_8BIT_VECTOR_PRESS) ||
				 (pTransEntry->gpdCommand == GPDF_CMD_ID_8BIT_VECTOR_RELEASE)){
			if(pTransEntry->optRecordUsed){
				entryLen += 1;//option selector
				entryLen += 2;//contact status + bit mask
			}
		}
	}

	return entryLen;
}

u8 gp_buildTransTabEntryFormat(transTabEntry_t *pTransEntry, u8 *pBuf)
{
	u8 *ptr = pBuf;

	if(pTransEntry->options.bits.appId == GP_APP_ID_SRC_ID){
		*ptr++ = U32_BYTE0(pTransEntry->gpdId.srcId);
		*ptr++ = U32_BYTE1(pTransEntry->gpdId.srcId);
		*ptr++ = U32_BYTE2(pTransEntry->gpdId.srcId);
		*ptr++ = U32_BYTE3(pTransEntry->gpdId.srcId);
	}else if(pTransEntry->options.bits.appId == GP_APP_ID_GPD){
		ZB_64BIT_ADDR_COPY(ptr, pTransEntry->gpdId.gpdIeeeAddr);
		ptr += EXT_ADDR_LEN;
		*ptr++ = pTransEntry->gpdEndpoint;
	}

	*ptr++ = pTransEntry->gpdCommand;
	*ptr++ = pTransEntry->endpoint;
	*ptr++ = LO_UINT16(pTransEntry->profileId);
	*ptr++ = HI_UINT16(pTransEntry->profileId);
	*ptr++ = LO_UINT16(pTransEntry->clusterId);
	*ptr++ = HI_UINT16(pTransEntry->clusterId);

	*ptr++ = pTransEntry->zbCommandId;
	*ptr++ = pTransEntry->zbCmdPayloadLen;

	if(pTransEntry->zbCmdPayloadLen &&
	  (pTransEntry->zbCmdPayloadLen != 0xfe) &&
	  (pTransEntry->zbCmdPayloadLen != 0xff)){
		memcpy(ptr, pTransEntry->zbCmdPayload, pTransEntry->zbCmdPayloadLen);
		ptr += pTransEntry->zbCmdPayloadLen;
	}

	if(pTransEntry->options.bits.addInfoPresent){
		u8 *pAddInfoLen = ptr;
		ptr++;

		if((pTransEntry->gpdCommand == GPDF_CMD_ID_ANY_SENSOR_REPORT_COMMAND) ||
		  ((pTransEntry->gpdCommand >= GPDF_CMD_ID_ATTR_REPORT) &&
		   (pTransEntry->gpdCommand <= GPDF_CMD_ID_MANU_MULTI_CLUSTER_REPORT)) ||
		   (pTransEntry->gpdCommand == GPDF_CMD_ID_COMPACT_ATTR_REPORT)){
			if(pTransEntry->optRecordUsed){
				*ptr++ = pTransEntry->optRecord.compactReport.optSelector.opt;
				*ptr++ = pTransEntry->optRecord.compactReport.report.reportId;
				*ptr++ = pTransEntry->optRecord.compactReport.report.attrOffsetWithinReport;
				*ptr++ = LO_UINT16(pTransEntry->optRecord.compactReport.report.clusterId);
				*ptr++ = HI_UINT16(pTransEntry->optRecord.compactReport.report.clusterId);
				*ptr++ = LO_UINT16(pTransEntry->optRecord.compactReport.report.attrId);
				*ptr++ = HI_UINT16(pTransEntry->optRecord.compactReport.report.attrId);
				*ptr++ = pTransEntry->optRecord.compactReport.report.attrDataType;
				*ptr++ = pTransEntry->optRecord.compactReport.report.attrOpt.opt;
				if(pTransEntry->optRecord.compactReport.report.attrOpt.bits.manuIdPresent){
					*ptr++ = LO_UINT16(pTransEntry->optRecord.compactReport.report.manuId);
					*ptr++ = HI_UINT16(pTransEntry->optRecord.compactReport.report.manuId);
				}
			}
		}else if((pTransEntry->gpdCommand == GPDF_CMD_ID_8BIT_VECTOR_PRESS) ||
				 (pTransEntry->gpdCommand == GPDF_CMD_ID_8BIT_VECTOR_RELEASE)){
			if(pTransEntry->optRecordUsed){
				*ptr++ = pTransEntry->optRecord.vectorCmd.optSelector.opt;
				*ptr++ = pTransEntry->optRecord.vectorCmd.cmd.contactStatus;
				*ptr++ = pTransEntry->optRecord.vectorCmd.cmd.contactBitmask;
			}
		}

		*pAddInfoLen = (u8)(ptr - pAddInfoLen) - 1;
	}

	return (ptr - pBuf);
}

transTabEntry_t *gp_transTabEntryFreeGet(void)
{
	transTabEntry_t *pTransEntry = NULL;

	if(g_gpTransTab.transTabNum >= GPS_MAX_TRANS_TABLE_ENTRIES){
		return NULL;
	}

	for(u8 i = 0; i < GPS_MAX_TRANS_TABLE_ENTRIES; i++){
		pTransEntry = &g_gpTransTab.transTab[i];
		if(!pTransEntry->used){
			memset(pTransEntry, 0, sizeof(transTabEntry_t));
			return pTransEntry;
		}
	}

	return NULL;
}

transTabEntry_t *gp_transTabEntryFind(u8 appId, gpdId_t gpdId, u8 endpoint,
									  u8 gpdCommand, u8 zbEndpoint,
									  u16 clusterId, u16 profileId,
									  u8 reportId, u16 attrId)
{
	transTabEntry_t *pTransEntry = NULL;

	if(g_gpTransTab.transTabNum == 0){
		return NULL;
	}

	for(u8 i = 0; i < GPS_MAX_TRANS_TABLE_ENTRIES; i++){
		pTransEntry = &g_gpTransTab.transTab[i];
		if(pTransEntry->used){
			bool gpdMatch = 0;

			if(pTransEntry->options.bits.appId == appId){
				if((appId == GP_APP_ID_SRC_ID) &&
				  ((pTransEntry->gpdId.srcId == gpdId.srcId) ||
				   (pTransEntry->gpdId.srcId == 0xFFFFFFFF))){
					gpdMatch = 1;
				}else if((appId == GP_APP_ID_GPD) && ((pTransEntry->gpdEndpoint == endpoint) ||
						(pTransEntry->gpdEndpoint == 0) || (pTransEntry->gpdEndpoint == 0xff) ||
						(endpoint == 0) || (endpoint == 0xff)) &&
						(!memcmp(pTransEntry->gpdId.gpdIeeeAddr, gpdId.gpdIeeeAddr, EXT_ADDR_LEN) ||
						 ZB_IEEE_ADDR_IS_INVALID(pTransEntry->gpdId.gpdIeeeAddr))){
					gpdMatch = 1;
				}
			}

			if(gpdMatch){
				if(((pTransEntry->profileId == profileId) || (profileId == 0xFFFF)) &&
				  ((pTransEntry->endpoint == zbEndpoint) || ((zbEndpoint >= 0xFD) && (zbEndpoint <= 0xFF))) &&
				  ((pTransEntry->clusterId == clusterId) || (pTransEntry->clusterId == 0xFFFF) || (clusterId == 0xFFFF))){
					if(((gpdCommand >= GPDF_CMD_ID_ATTR_REPORT) &&
						(gpdCommand <= GPDF_CMD_ID_MANU_MULTI_CLUSTER_REPORT)) ||
						(gpdCommand == GPDF_CMD_ID_COMPACT_ATTR_REPORT)){
						if(((pTransEntry->gpdCommand == gpdCommand) ||
							(pTransEntry->gpdCommand == GPDF_CMD_ID_ANY_SENSOR_REPORT_COMMAND)) &&
							((pTransEntry->optRecord.compactReport.report.reportId == reportId) || (reportId == 0xFF)) &&
							((pTransEntry->optRecord.compactReport.report.clusterId == clusterId) || (clusterId == 0xFFFF)) &&
							((pTransEntry->optRecord.compactReport.report.attrId == attrId) || (attrId == 0xFFFF))){
							return pTransEntry;
						}
					}else if(pTransEntry->gpdCommand == gpdCommand){
						return pTransEntry;
					}
				}
			}
		}
	}

	return NULL;
}

void gp_transTabEntryRemove(u8 appId, gpdId_t gpdId, u8 endpoint)
{
	transTabEntry_t *pTransEntry = NULL;

	//printf("transEntryRemove: appId = %x, ep = %x\n", appId, endpoint);
	if(appId == GP_APP_ID_GPD){
		DEBUG_ARRAY(1, gpdId.gpdIeeeAddr, EXT_ADDR_LEN);
	}

	if(g_gpTransTab.transTabNum == 0){
		//printf("empty\n");
		return;
	}

	bool nvUpdate = 0;

	for(u8 i = 0; i < GPS_MAX_TRANS_TABLE_ENTRIES; i++){
		pTransEntry = &g_gpTransTab.transTab[i];
		if(pTransEntry->used && (pTransEntry->options.bits.appId == appId)){
			bool findEntry = 0;

			if((appId == GP_APP_ID_SRC_ID) && (gpdId.srcId == pTransEntry->gpdId.srcId)){
				findEntry = 1;
			}else if((appId == GP_APP_ID_GPD) && !memcmp(gpdId.gpdIeeeAddr, pTransEntry->gpdId.gpdIeeeAddr, EXT_ADDR_LEN)){
				if((endpoint == pTransEntry->gpdEndpoint) || (endpoint == 0) || (endpoint == 0xff) ||
				   (pTransEntry->gpdEndpoint == 0) || (pTransEntry->gpdEndpoint == 0xff)){
					findEntry = 1;
				}
			}

			if(findEntry){
				nvUpdate = 1;

				gp_transTabEntryClear(pTransEntry);
			}
		}
	}

	if(nvUpdate){
		gp_transTabNvUpdate();
	}
}

transTabEntry_t *gp_transTabReportUpdate(u8 appId, gpdId_t gpdId, u8 endpoint,
					   	   	 	 	 	 u8 reportId, bool side,
					   	   	 	 	 	 u16 clusterId, u16 manuId,
					   	   	 	 	 	 gpAttrRecord_t *pAttrRecord)
{
//	printf("transTabReportUpdate: reportId = %x, clusterId = %x, attrId = %x, manuId = %x\n",
//			reportId, clusterId, pAttrRecord->attrId, manuId);

	bool clusterIdFind = FALSE;

	af_endpoint_descriptor_t *aed = af_epDescriptorGet();

	for(u8 i = 0; i < af_availableEpNumGet(); i++){
		if(af_clsuterIdMatched(clusterId, aed[i].correspond_simple_desc)){
			clusterIdFind = TRUE;
			break;
		}
	}

	if(!clusterIdFind){
		return NULL;
	}

	if(!pAttrRecord->attrOption.bits.reported){
		return NULL;
	}

	transTabEntry_t *pTransEntry = gp_transTabEntryFind(appId, gpdId, endpoint,
														GPDF_CMD_ID_COMPACT_ATTR_REPORT, 0xFF,
														clusterId, HA_PROFILE_ID,
														reportId, pAttrRecord->attrId);
	if(!pTransEntry){
		pTransEntry = gp_transTabEntryFreeGet();
		if(!pTransEntry){
			return NULL;
		}

		//printf("newTransEntry: %x\n", pTransEntry);

		pTransEntry->used = 1;
		pTransEntry->options.bits.addInfoPresent = 1;
		pTransEntry->options.bits.appId = appId;
		if(appId == GP_APP_ID_SRC_ID){
			pTransEntry->gpdId.srcId = gpdId.srcId;
		}else if(appId == GP_APP_ID_GPD){
			ZB_64BIT_ADDR_COPY(pTransEntry->gpdId.gpdIeeeAddr, gpdId.gpdIeeeAddr);
			pTransEntry->gpdEndpoint = endpoint;
		}
		pTransEntry->gpdCommand = GPDF_CMD_ID_COMPACT_ATTR_REPORT;
		pTransEntry->endpoint = 0xFF;
		pTransEntry->profileId = HA_PROFILE_ID;
		pTransEntry->clusterId = 0xFFFF;
		pTransEntry->zbCommandId = ZCL_CMD_REPORT;
		pTransEntry->zbCmdPayloadLen = 0;

		g_gpTransTab.transTabNum++;
	}

	pTransEntry->optRecord.compactReport.report.reportId = reportId;
	pTransEntry->optRecord.compactReport.report.attrOffsetWithinReport = pAttrRecord->attrOffsetWithinReport;
	pTransEntry->optRecord.compactReport.report.clusterId = clusterId;
	pTransEntry->optRecord.compactReport.report.attrId = pAttrRecord->attrId;
	pTransEntry->optRecord.compactReport.report.attrDataType = pAttrRecord->attrDataType;
	pTransEntry->optRecord.compactReport.report.attrOpt.bits.side = side;
	pTransEntry->optRecord.compactReport.report.attrOpt.bits.manuIdPresent = manuId ? 1 : 0;
	pTransEntry->optRecord.compactReport.report.manuId = manuId;
	pTransEntry->optRecord.compactReport.optSelector.bits.optId = 0;
	pTransEntry->optRecord.compactReport.optSelector.bits.optLen = sizeof(compactAttrReport_t) - 1;
	if(!pTransEntry->optRecord.compactReport.report.attrOpt.bits.manuIdPresent){
		pTransEntry->optRecord.compactReport.optSelector.bits.optLen -= 2;
	}

	pTransEntry->optRecordUsed = 1;

	return pTransEntry;
}

transTabEntry_t *gp_transTabGenSwitchUpdate(u8 appId, gpdId_t gpdId, u8 endpoint,
											gpSwitchInfo_t *pSwitchInfo)
{
	if(!pSwitchInfo->switchCfg.bits.contactNum){
		return NULL;
	}

	transTabEntry_t *pTransEntry = gp_transTabEntryFind(appId, gpdId, endpoint,
														GPDF_CMD_ID_8BIT_VECTOR_PRESS, 0xFF,
														0xFFFF, HA_PROFILE_ID,
														0xFF, 0xFFFF);
	if(!pTransEntry){
		pTransEntry = gp_transTabEntryFreeGet();
		if(!pTransEntry){
			return NULL;
		}

		pTransEntry->used = 1;
		pTransEntry->options.bits.addInfoPresent = 1;
		pTransEntry->options.bits.appId = appId;
		if(appId == GP_APP_ID_SRC_ID){
			pTransEntry->gpdId.srcId = gpdId.srcId;
		}else if(appId == GP_APP_ID_GPD){
			ZB_64BIT_ADDR_COPY(pTransEntry->gpdId.gpdIeeeAddr, gpdId.gpdIeeeAddr);
			pTransEntry->gpdEndpoint = endpoint;
		}
		//TODO: right?
		pTransEntry->gpdCommand = GPDF_CMD_ID_8BIT_VECTOR_PRESS;
		pTransEntry->endpoint = 0xFF;
		pTransEntry->profileId = HA_PROFILE_ID;
		pTransEntry->clusterId = 0xFFFF;
		pTransEntry->zbCommandId = 0xFF;
		pTransEntry->zbCmdPayloadLen = 0;

		g_gpTransTab.transTabNum++;
	}

	pTransEntry->optRecord.vectorCmd.switchCfg.cfg = pSwitchInfo->switchCfg.cfg;

	pTransEntry->optRecord.vectorCmd.cmd.contactStatus = pSwitchInfo->contactStatus;
	for(u8 i = 0; i < pSwitchInfo->switchCfg.bits.contactNum; i++){
		pTransEntry->optRecord.vectorCmd.cmd.contactBitmask |= BIT(i);
	}
	pTransEntry->optRecord.vectorCmd.optSelector.bits.optId = 0;
	pTransEntry->optRecord.vectorCmd.optSelector.bits.optLen = 1;

	pTransEntry->optRecordUsed = 1;

	return pTransEntry;
}

transTabEntry_t *gpTransTabUpdate(u8 appId, gpdId_t gpdId, u8 endpoint,
								  transTabEntry_t *pTransTabEntry,
								  gpTranslation_t *pTranslation)
{
	transTabEntry_t *pTransEntry = pTransTabEntry;

//	printf("gpTransTabUpdate: pTransEntry = %x, gpdCmd = %x, zbCmd = %x\n",
//			pTransTabEntry, pTranslation->gpdCmdId, pTranslation->zbCmdId);

	if(!pTransEntry){
		pTransEntry = gp_transTabEntryFreeGet();
		if(!pTransEntry){
			return NULL;
		}

		pTransEntry->used = 1;

		pTransEntry->options.bits.appId = appId;
		if(appId == GP_APP_ID_SRC_ID){
			pTransEntry->gpdId.srcId = gpdId.srcId;
		}else if(appId == GP_APP_ID_GPD){
			ZB_64BIT_ADDR_COPY(pTransEntry->gpdId.gpdIeeeAddr, gpdId.gpdIeeeAddr);
			pTransEntry->gpdEndpoint = endpoint;
		}

		g_gpTransTab.transTabNum++;
	}

	pTransEntry->options.bits.addInfoPresent = pTranslation->addInfoLen ? 1 : 0;
	pTransEntry->gpdCommand = pTranslation->gpdCmdId;
	pTransEntry->endpoint = pTranslation->endpoint;//TODO
	pTransEntry->profileId = pTranslation->profile;
	pTransEntry->clusterId = pTranslation->cluster;
	pTransEntry->zbCommandId = pTranslation->zbCmdId;
	pTransEntry->zbCmdPayloadLen = pTranslation->zbCmdPayloadLen;
	if((pTransEntry->zbCmdPayloadLen != 0) &&
	   (pTransEntry->zbCmdPayloadLen != 0xff) &&
	   (pTransEntry->zbCmdPayloadLen != 0xfe)){
		if(pTransEntry->zbCmdPayloadLen >= ZB_CMD_PAYLOAD_LEN_MAX){
			pTransEntry->zbCmdPayloadLen = ZB_CMD_PAYLOAD_LEN_MAX;
		}
		memcpy(pTransEntry->zbCmdPayload, pTranslation->pZbCmdPayload, pTransEntry->zbCmdPayloadLen);
	}

	if(pTranslation->addInfoLen && pTranslation->pAdditionalInfo){
		pTransEntry->optRecordUsed = 1;

		u8 *pData = pTranslation->pAdditionalInfo;

		if(pTranslation->gpdCmdId == GPDF_CMD_ID_COMPACT_ATTR_REPORT){
			pTransEntry->optRecord.compactReport.optSelector.opt = *pData++;
			pTransEntry->optRecord.compactReport.report.reportId = *pData++;
			pTransEntry->optRecord.compactReport.report.attrOffsetWithinReport = *pData++;
			pTransEntry->optRecord.compactReport.report.clusterId = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			pTransEntry->optRecord.compactReport.report.attrId = BUILD_U16(pData[0], pData[1]);
			pData += 2;
			pTransEntry->optRecord.compactReport.report.attrDataType = *pData++;
			pTransEntry->optRecord.compactReport.report.attrOpt.opt = *pData++;
			if(pTransEntry->optRecord.compactReport.report.attrOpt.bits.manuIdPresent){
				pTransEntry->optRecord.compactReport.report.manuId = BUILD_U16(pData[0], pData[1]);
				pData += 2;
			}
		}else if((pTranslation->gpdCmdId == GPDF_CMD_ID_8BIT_VECTOR_PRESS) ||
				 (pTranslation->gpdCmdId == GPDF_CMD_ID_8BIT_VECTOR_RELEASE)){
			pTransEntry->optRecord.vectorCmd.optSelector.opt = *pData++;
			pTransEntry->optRecord.vectorCmd.cmd.contactBitmask = *pData++;
			pTransEntry->optRecord.vectorCmd.cmd.contactStatus = *pData++;
		}
	}else{
		pTransEntry->optRecordUsed = 0;
	}

	return pTransEntry;
}

void gp_transTabUpdateWithCfg(u8 appId, gpdId_t gpdId, u8 endpoint,
							  u8 devId, u8 numOfCmd,
							  u8 *pCmdList, u8 *pClusterList)
{
//	printf("transUpdateDevID: devId = %x, cmdNum = %d, pCmd = %x, pCluster = %x\n",
//			devId, numOfCmd, pCmdList, pClusterList);

	gpTranslation_t translation;
	TL_SETSTRUCTCONTENT(translation, 0);

	gp_transCfg_t *pTransCfg = NULL;
	transTabEntry_t *pTransEntry = NULL;

	if(pClusterList || pCmdList){
		u16 clusterId = 0xFFFF;
		s8 clusterNum = 0;
		u8 *pClusterData = NULL;

		if(pClusterList){
			pClusterData = pClusterList;
			clusterNum = (pClusterData[0] & 0x0f) + ((pClusterData[0] >> 4) & 0x0f);
			pClusterData++;
		}

		do{
			if(clusterNum--){
				clusterId = BUILD_U16(pClusterData[0], pClusterData[1]);
				pClusterData += 2;
			}

			for(u8 cmdCnt = 0; cmdCnt < numOfCmd; cmdCnt++){
				u8 cmdId = pCmdList[cmdCnt];

				for(u8 listCnt = 0; listCnt < GP_TRANS_CFG_LIST_NUM; listCnt++){
					pTransCfg = (gp_transCfg_t *)&g_gpTransCfgList[listCnt];

					if(((pTransCfg->gpdDevId == devId) || (devId == GPD_DEV_ID_NOT_SPECIFIC_FUNCTION)) &&
					   ((pTransCfg->zbClusterId == clusterId) || (clusterId == 0xFFFF)) &&
					   ((pTransCfg->gpdCmdId == cmdId) || ((pTransCfg->gpdCmdId == GPDF_CMD_ID_ANY_SENSOR_REPORT_COMMAND) &&
						(cmdId >= GPDF_CMD_ID_ATTR_REPORT) && (cmdId <= GPDF_CMD_ID_MANU_MULTI_CLUSTER_REPORT)))){
						pTransEntry = gp_transTabEntryFind(appId, gpdId, endpoint,
														   pTransCfg->gpdCmdId, 0xFF,
														   clusterId, HA_PROFILE_ID,
														   0xFF, 0xFFFF);
						if(!pTransEntry){
							translation.index = 0xFF;
							translation.gpdCmdId = pTransCfg->gpdCmdId;
							translation.endpoint = g_gpsCtx.gpsAppEndpoint;//0xFF;
							translation.profile = HA_PROFILE_ID;
							translation.cluster = 0xFFFF;
							translation.zbCmdId = pTransCfg->zbCmdId;
							translation.zbCmdPayloadLen = pTransCfg->zbCmdPayloadLen;
							translation.pZbCmdPayload = pTransCfg->zbCmdPayload;
							translation.addInfoLen = 0;
							translation.pAdditionalInfo = NULL;

							gpTransTabUpdate(appId, gpdId, endpoint, pTransEntry, &translation);
						}
						break;
					}
				}
			}
		}while(clusterNum);
	}else{
		for(u8 i = 0; i < GP_TRANS_CFG_LIST_NUM; i++){
			pTransCfg = (gp_transCfg_t *)&g_gpTransCfgList[i];

			if(devId == pTransCfg->gpdDevId){
				pTransEntry = gp_transTabEntryFind(appId, gpdId, endpoint,
												   pTransCfg->gpdCmdId, 0xFF,
												   pTransCfg->zbClusterId, HA_PROFILE_ID,
												   0xFF, 0xFFFF);
				if(!pTransEntry){
					translation.index = 0xFF;
					translation.gpdCmdId = pTransCfg->gpdCmdId;
					translation.endpoint = g_gpsCtx.gpsAppEndpoint;//0xFF;
					translation.profile = HA_PROFILE_ID;
					translation.cluster = 0xFFFF;
					translation.zbCmdId = pTransCfg->zbCmdId;
					translation.zbCmdPayloadLen = pTransCfg->zbCmdPayloadLen;
					translation.pZbCmdPayload = pTransCfg->zbCmdPayload;
					translation.addInfoLen = 0;
					translation.pAdditionalInfo = NULL;

					gpTransTabUpdate(appId, gpdId, endpoint, pTransEntry, &translation);
				}
			}
		}
	}
}

void gp_gpdfTranslate(u8 appId, gpdId_t gpdId, u8 endpoint,
					  u8 gpdCmdId, u8 dataLen, u8 *pData)
{
	//printf("gpdfTranslate: gpdCmd = %x\n", gpdCmdId);

	transTabEntry_t *pTransEntry = gp_transTabEntryFind(appId, gpdId, endpoint,
														gpdCmdId, 0xFF,
														0xFFFF, 0xFFFF,
														0xFF, 0xFFFF);

	if(!pTransEntry){
		return;
	}

	//printf("GPDF_CMD = [0x%x]\n", gpdCmdId);
	//printf("pTransEntry = %x, clusterId = %x\n", pTransEntry, pTransEntry->clusterId);
	//printf("ZB_CMD = [0x%x]\n", pTransEntry->zbCommandId);

	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
	dstEpInfo.dstAddr.shortAddr = 0x0001;//TODO: group id
	dstEpInfo.profileId = HA_PROFILE_ID;

	switch(pTransEntry->gpdCommand){
		case GPDF_CMD_ID_ZCL_TUNNELING:
			//TODO:
			break;
		case GPDF_CMD_ID_ANY_SENSOR_REPORT_COMMAND:
			//TODO:
			break;
		case GPDF_CMD_ID_ONOFF_OFF:
			zcl_onOff_offCmd(pTransEntry->endpoint, &dstEpInfo, FALSE);
			break;
		case GPDF_CMD_ID_ONOFF_ON:
			zcl_onOff_onCmd(pTransEntry->endpoint, &dstEpInfo, FALSE);
			break;
		case GPDF_CMD_ID_ONOFF_TOGGLE:
			zcl_onOff_toggleCmd(pTransEntry->endpoint, &dstEpInfo, FALSE);
			break;
		case GPDF_CMD_ID_LEVEL_MOVE_UP:
		case GPDF_CMD_ID_LEVEL_MOVE_DOWN:
			{
				move_t move;
				TL_SETSTRUCTCONTENT(move, 0);

				move.moveMode = pTransEntry->zbCmdPayload[0];
				move.rate = pTransEntry->zbCmdPayload[1];

				zcl_level_moveCmd(pTransEntry->endpoint, &dstEpInfo, FALSE, &move);
			}
			break;
		case GPDF_CMD_ID_LEVEL_STEP_UP:
		case GPDF_CMD_ID_LEVEL_STEP_DOWN:
			{
				step_t step;
				TL_SETSTRUCTCONTENT(step, 0);

				step.stepMode = pTransEntry->zbCmdPayload[0];
				step.stepSize = pTransEntry->zbCmdPayload[1];
				step.transitionTime = BUILD_U16(pTransEntry->zbCmdPayload[2], pTransEntry->zbCmdPayload[3]);

				zcl_level_stepCmd(pTransEntry->endpoint, &dstEpInfo, FALSE, &step);
			}
			break;
		case GPDF_CMD_ID_LEVEL_STOP:
			{
				stop_t stop;
				TL_SETSTRUCTCONTENT(stop, 0);

				zcl_level_stopCmd(pTransEntry->endpoint, &dstEpInfo, FALSE, &stop);
			}
			break;
		case GPDF_CMD_ID_LEVEL_MOVE_UP_WITH_ONOFF:
		case GPDF_CMD_ID_LEVEL_MOVE_DOWN_WITH_ONOFF:
			{
				move_t move;
				TL_SETSTRUCTCONTENT(move, 0);

				move.moveMode = pTransEntry->zbCmdPayload[0];
				move.rate = pTransEntry->zbCmdPayload[1];

				zcl_level_moveWithOnOffCmd(pTransEntry->endpoint, &dstEpInfo, FALSE, &move);
			}
			break;
		case GPDF_CMD_ID_LEVEL_STEP_UP_WITH_ONOFF:
		case GPDF_CMD_ID_LEVEL_STEP_DOWN_WITH_ONOFF:
			{
				step_t step;
				TL_SETSTRUCTCONTENT(step, 0);

				step.stepMode = pTransEntry->zbCmdPayload[0];
				step.stepSize = pTransEntry->zbCmdPayload[1];
				step.transitionTime = BUILD_U16(pTransEntry->zbCmdPayload[2], pTransEntry->zbCmdPayload[3]);

				zcl_level_stepWithOnOffCmd(pTransEntry->endpoint, &dstEpInfo, FALSE, &step);
			}
			break;
		default:
			break;
	}
}

#endif	/* GP_SUPPORT_ENABLE */
