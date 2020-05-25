/********************************************************************************************************
 * @file     zcl_reporting.c
 *
 * @brief
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


/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "../zbapi/zb_api.h"
#include "zcl_include.h"

/**********************************************************************
 * GLOBAL VARIABLES
 */
zcl_reportingTab_t reportingTab;

/**********************************************************************
 * LOCAL VARIABLES
 */
ev_time_event_t *reportAttrTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryClear
 *
 * @brief
 *
 * @param   pEntry
 *
 * @return	NULL
 */
_CODE_ZCL_ void zcl_reportCfgInfoEntryClear(reportCfgInfo_t *pEntry)
{
	if(pEntry){
		memset((u8 *)pEntry, 0, sizeof(reportCfgInfo_t));
	}
}

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryClear
 *
 * @brief
 *
 * @param   pEntry
 *
 * @return	NULL
 */
_CODE_ZCL_ void zcl_reportingTabInit(void)
{
	if(zcl_reportingTab_restore() != NV_SUCC){
		reportingTab.reportNum = 0;

		for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
			reportCfgInfo_t *pEntry = &reportingTab.reportCfgInfo[i];

			zcl_reportCfgInfoEntryClear(pEntry);
		}
	}
}

/*********************************************************************
 * @fn      zcl_reportingEntryActiveNumGet
 *
 * @brief
 *
 * @param   NULL
 *
 * @return	Number of active reporting table
 */
_CODE_ZCL_ u8 zcl_reportingEntryActiveNumGet(void)
{
	u8 cnt = 0;

	if(reportingTab.reportNum){
		for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
			if(reportingTab.reportCfgInfo[i].used && (reportingTab.reportCfgInfo[i].maxInterval != 0xFFFF)){
				cnt++;
			}
		}
	}

	return cnt;
}

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryFind
 *
 * @brief
 *
 * @param   clusterId
 * 			attrID
 *
 * @return	Reporting table entry
 */
_CODE_ZCL_ reportCfgInfo_t *zcl_reportCfgInfoEntryFind(u8 endpoint, u16 clusterId, u16 attrID)
{
	for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
		if(reportingTab.reportCfgInfo[i].used){
			if( (reportingTab.reportCfgInfo[i].clusterID == clusterId) &&
				(reportingTab.reportCfgInfo[i].attrID == attrID) &&
				(reportingTab.reportCfgInfo[i].endPoint == endpoint) ){
					return &reportingTab.reportCfgInfo[i];
			}
		}
	}

	return NULL;
}

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryFreeGet
 *
 * @brief
 *
 * @param   NULL
 *
 * @return	Reporting table entry
 */
_CODE_ZCL_ reportCfgInfo_t *zcl_reportCfgInfoEntryFreeGet(void)
{
	for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
		if(reportingTab.reportCfgInfo[i].used == 0){
			return &reportingTab.reportCfgInfo[i];
		}
	}

	return NULL;
}

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryRst2Default
 *
 * @brief
 *
 * @param   pEntry
 *
 * @return	NULL
 */
_CODE_ZCL_ void zcl_reportCfgInfoEntryRst(reportCfgInfo_t *pEntry)
{
	if(pEntry->used){
		pEntry->minInterval = pEntry->minIntDft;
		pEntry->maxInterval = pEntry->maxIntDft;
		pEntry->minIntCnt = pEntry->minIntDft;
		pEntry->maxIntCnt = pEntry->maxIntDft;
		memset(pEntry->reportableChange, 0, REPORTABLE_CHANGE_MAX_ANALOG_SIZE);
	}
}

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryUpdate
 *
 * @brief
 *
 * @param   pEntry
 * 			endPoint
 * 			profileId
 * 			clusterId
 * 			pCfgReportRec
 *
 * @return	NULL
 */
_CODE_ZCL_ void zcl_reportCfgInfoEntryUpdate(reportCfgInfo_t *pEntry, u8 endPoint, u16 profileId, u16 clusterId, zclCfgReportRec_t *pCfgReportRec)
{
	if(!pEntry->used){
		//add new
		pEntry->profileID = profileId;
		pEntry->clusterID = clusterId;
		pEntry->endPoint = endPoint;
		pEntry->attrID = pCfgReportRec->attrID;
		pEntry->minIntDft = pCfgReportRec->minReportInt;
		pEntry->maxIntDft = pCfgReportRec->maxReportInt;
		pEntry->minInterval = pCfgReportRec->minReportInt;
		pEntry->maxInterval = pCfgReportRec->maxReportInt;
		pEntry->dataType = pCfgReportRec->dataType;
		if(zcl_analogDataType(pEntry->dataType)){
			memcpy(pEntry->reportableChange, pCfgReportRec->reportableChange, zcl_getDataTypeLen(pEntry->dataType));
		}

		zclAttrInfo_t *pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
		if(!pAttrEntry){
			//should not happen.
			ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
		}
		//u8 len = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);
		//memcpy(pEntry->prevData, pAttrEntry->data, len);

		pEntry->minIntCnt = pCfgReportRec->minReportInt;
		pEntry->maxIntCnt = pCfgReportRec->maxReportInt;
		pEntry->used = 1;

		reportingTab.reportNum++;
	}else{
		//update
		pEntry->minInterval = pCfgReportRec->minReportInt;
		pEntry->maxInterval = pCfgReportRec->maxReportInt;
		pEntry->minIntCnt = pCfgReportRec->minReportInt;
		pEntry->maxIntCnt = pCfgReportRec->maxReportInt;
		if(zcl_analogDataType(pEntry->dataType)){
			memcpy(pEntry->reportableChange, pCfgReportRec->reportableChange, zcl_getDataTypeLen(pEntry->dataType));
		}
	}
}

/*********************************************************************
 * @fn      reportableChangeValueChk
 *
 * @brief
 *
 * @param   dataType
 * 			curValue
 * 			prevValue
 * 			reportableChange
 *
 * @return	TRUE / FALSE
 */
_CODE_ZCL_ bool reportableChangeValueChk(u8 dataType, u8 *curValue, u8 *prevValue, u8 *reportableChange)
{
	bool needReport = FALSE;

	switch(dataType)
	{
		case ZCL_DATA_TYPE_UINT8:
			{
				u8 P = prevValue[0];
				u8 C = curValue[0];
				u8 R = reportableChange[0];
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_UINT16:
			{
				u16 P = BUILD_U16(prevValue[0], prevValue[1]);
				u16 C = BUILD_U16(curValue[0], curValue[1]);
				u16 R = BUILD_U16(reportableChange[0], reportableChange[1]);

				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_UINT24:
			{
				u24 P = BUILD_U24(prevValue[0], prevValue[1], prevValue[2]);//*((u24 *)prevValue);
				u24 C = BUILD_U24(curValue[0], curValue[1], curValue[2]);//*((u24 *)curValue);
				u24 R = BUILD_U24(reportableChange[0], reportableChange[1], reportableChange[2]);//*((u24 *)reportableChange);
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_UINT32:
			{
				u32 P = BUILD_U32(prevValue[0], prevValue[1], prevValue[2], prevValue[3]);//*((u32 *)prevValue);
				u32 C = BUILD_U32(curValue[0], curValue[1], curValue[2], curValue[3]);//*((u32 *)curValue);
				u32 R = BUILD_U32(reportableChange[0], reportableChange[1], reportableChange[2], reportableChange[3]);//*((u32 *)reportableChange);
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_INT8:
			{
				s8 P = (s8)prevValue[0];
				s8 C = (s8)curValue[0];
				s8 R = (s8)reportableChange[0];
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_INT16:
			{
				s16 P = BUILD_S16(prevValue[0], prevValue[1]);
				s16 C = BUILD_S16(curValue[0], curValue[1]);
				s16 R = BUILD_S16(reportableChange[0], reportableChange[1]);

				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_INT24:
			{
				s24 P = BUILD_S24(prevValue[0], prevValue[1], prevValue[2]);//*((s24 *)prevValue);
				s24 C = BUILD_S24(curValue[0], curValue[1], curValue[2]);//*((s24 *)curValue);
				s24 R = BUILD_S24(reportableChange[0], reportableChange[1], reportableChange[2]);//*((s24 *)reportableChange);
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_INT32:
			{
				s32 P = BUILD_S32(prevValue[0], prevValue[1], prevValue[2], prevValue[3]);//*((s32 *)prevValue);
				s32 C = BUILD_S32(curValue[0], curValue[1], curValue[2], curValue[3]);//*((s32 *)curValue);
				s32 R = BUILD_S32(reportableChange[0], reportableChange[1], reportableChange[2], reportableChange[3]);//*((s32 *)reportableChange);
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
			/*
		case ZCL_DATA_TYPE_SINGLE_PREC:
			{
				float P = *((float *)prevValue);
				float C = *((float *)curValue);
				float R = *((float *)reportableChange);
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
		case ZCL_DATA_TYPE_DOUBLE_PREC:
			{
				double P = *((double *)prevValue);
				double C = *((double *)curValue);
				double R = *((double *)reportableChange);
				if(P >= C){
					needReport = ((P - C) > R) ? TRUE : FALSE;
				}else{
					needReport = ((C - P) > R) ? TRUE : FALSE;
				}
			}
			break;
			*/
		default:
			break;
	}

	return needReport;
}

/*********************************************************************
 * @fn      reportAttr
 *
 * @brief
 *
 * @param   pEntry
 *
 * @return	NULL
 */
_CODE_ZCL_ void reportAttr(reportCfgInfo_t *pEntry)
{
	if(!zb_bindingTblSearched(pEntry->clusterID, pEntry->endPoint)){
		return;
	}

	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
	dstEpInfo.profileId = pEntry->profileID;

	zclAttrInfo_t *pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
	if(!pAttrEntry){
		//should not happen.
		ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
		return;
	}

	u16 len = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);

	len = (len>8) ? (8):(len);

	//store for next compare
	memcpy(pEntry->prevData, pAttrEntry->data, len);

	zcl_sendReportCmd(pEntry->endPoint, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
					  pEntry->clusterID, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
}

/*********************************************************************
 * @fn      reportNoMinLimit
 *
 * @brief	check if there is no minimum limit report.
 *
 * @param   NULL
 *
 * @return	NULL
 */
_CODE_ZCL_ void reportNoMinLimit(void)
{
	if(zcl_reportingEntryActiveNumGet()){
		zclAttrInfo_t *pAttrEntry = NULL;
		u16 len = 0;

		for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
			reportCfgInfo_t *pEntry = &reportingTab.reportCfgInfo[i];
			if(pEntry->used && (pEntry->maxInterval != 0xFFFF) && (pEntry->minInterval == 0)){
				//there is no minimum limit
				pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
				if(!pAttrEntry){
					//should not happen.
					ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
					return;
				}

				len = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);
				len = (len>8) ? (8):(len);

				if( (!zcl_analogDataType(pAttrEntry->type) && (memcmp(pEntry->prevData, pAttrEntry->data, len) != SUCCESS))
					|| ((zcl_analogDataType(pAttrEntry->type)
						&& reportableChangeValueChk(pAttrEntry->type, pAttrEntry->data, pEntry->prevData, pEntry->reportableChange)))
				){
					reportAttr(pEntry);

					pEntry->minIntCnt = pEntry->minInterval;
					pEntry->maxIntCnt = pEntry->maxInterval;
				}
			}
		}
	}
}

/*********************************************************************
 * @fn      reportAttrTimerCb
 *
 * @brief
 *
 * @param   arg
 *
 * @return	0 -- continue; -1 -- cancel
 */
_CODE_ZCL_ s32 reportAttrTimerCb(void *arg)
{
	if(zcl_reportingEntryActiveNumGet()){
		for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
			reportCfgInfo_t *pEntry = &reportingTab.reportCfgInfo[i];
			if(pEntry->used && (pEntry->minInterval || (pEntry->maxInterval && (pEntry->maxInterval != 0xFFFF)))){
				if(pEntry->minIntCnt){
					pEntry->minIntCnt--;
				}
				if(pEntry->maxIntCnt){
					pEntry->maxIntCnt--;
				}

				if(pEntry->maxInterval && !pEntry->maxIntCnt){
					reportAttr(pEntry);

					pEntry->minIntCnt = pEntry->minInterval;
					pEntry->maxIntCnt = pEntry->maxInterval;
				}else if(pEntry->minInterval && !pEntry->minIntCnt){
					zclAttrInfo_t *pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
					if(!pAttrEntry){
						//should not happen.
						ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
						return -1;
					}

					u8 len = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);

					len = (len>8) ? (8):(len);

					if( (!zcl_analogDataType(pAttrEntry->type) && (memcmp(pEntry->prevData, pAttrEntry->data, len) != SUCCESS))
						|| ((zcl_analogDataType(pAttrEntry->type)
							&& reportableChangeValueChk(pAttrEntry->type, pAttrEntry->data, pEntry->prevData, pEntry->reportableChange)))
					){
						reportAttr(pEntry);

						pEntry->minIntCnt = pEntry->minInterval;
						pEntry->maxIntCnt = pEntry->maxInterval;
					}
				}
			}
		}

		return 0;
	}else{
		reportAttrTimerEvt = NULL;
		return -1;
	}
}

/*********************************************************************
 * @fn      reportAttrTimerStart
 *
 * @brief
 *
 * @param   second
 *
 * @return	NULL
 */
_CODE_ZCL_ void reportAttrTimerStart(u16 seconds)
{
	if(zcl_reportingEntryActiveNumGet() && !reportAttrTimerEvt){
		for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
			reportCfgInfo_t *pEntry = &reportingTab.reportCfgInfo[i];
			if(pEntry->used && (pEntry->minInterval || (pEntry->maxInterval && (pEntry->maxInterval != 0xFFFF)))){
				if(zb_bindingTblSearched(pEntry->clusterID, pEntry->endPoint)){
					reportAttrTimerEvt = TL_ZB_TIMER_SCHEDULE(reportAttrTimerCb, NULL, seconds * 1000 * 1000);
					break;
				}
			}
		}
	}
}

/*********************************************************************
 * @fn      reportAttrTimerStop
 *
 * @brief
 *
 * @param   NULL
 *
 * @return	NULL
 */
_CODE_ZCL_ void reportAttrTimerStop(void)
{
	if(reportAttrTimerEvt){
		TL_ZB_TIMER_CANCEL(&reportAttrTimerEvt);
	}
}

