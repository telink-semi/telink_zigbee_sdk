/********************************************************************************************************
 * @file    zcl_reporting.c
 *
 * @brief   This is the source file for zcl_reporting
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
#include "zcl_include.h"

#define REPORT_DEBUG_ENABLE     0

/**********************************************************************
 * TYPEDEFS
 */
typedef struct {
    reportCfgInfo_t *pEntry;
    u16 seconds;
    u8 minOrMax; //0 - min; 1 - max
    ev_timer_event_t timer;
} zcl_reportTimerEvt_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
zcl_reportingTab_t reportingTab;

/**********************************************************************
 * LOCAL VARIABLES
 */
static zcl_reportTimerEvt_t reportTimer[ZCL_REPORTING_TABLE_NUM];
static ev_timer_event_t reportingTimer;

/**********************************************************************
 * FUNCTIONS
 */
static void reportAttrTimerStart(void);
static void reportAttrTimerStop(zcl_reportTimerEvt_t *pTimerEvt);

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryClear
 *
 * @brief
 *
 * @param   pEntry
 *
 * @return  None
 */
_CODE_ZCL_ void zcl_reportCfgInfoEntryClear(reportCfgInfo_t *pEntry)
{
    if (pEntry) {
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
 * @return  None
 */
_CODE_ZCL_ void zcl_reportingTabInit(void)
{
    if (zcl_reportingTab_restore() != NV_SUCC) {
        reportingTab.reportNum = 0;

        for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
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
 * @param   None
 *
 * @return  Number of active reporting table
 */
_CODE_ZCL_ u8 zcl_reportingEntryActiveNumGet(void)
{
    u8 cnt = 0;

    if (reportingTab.reportNum) {
        for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
            if (reportingTab.reportCfgInfo[i].used &&
               (reportingTab.reportCfgInfo[i].maxInterval != 0xFFFF)) {
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
 * @param   endpoint
 *          clusterId
 *          attrID
 *
 * @return  Reporting table entry
 */
_CODE_ZCL_ reportCfgInfo_t *zcl_reportCfgInfoEntryFind(u8 endpoint, u16 clusterId, u16 attrID)
{
    for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
        if (reportingTab.reportCfgInfo[i].used) {
            if ((reportingTab.reportCfgInfo[i].clusterID == clusterId) &&
                (reportingTab.reportCfgInfo[i].attrID == attrID) &&
                (reportingTab.reportCfgInfo[i].endPoint == endpoint)) {
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
 * @param   None
 *
 * @return  Reporting table entry
 */
_CODE_ZCL_ reportCfgInfo_t *zcl_reportCfgInfoEntryFreeGet(void)
{
    for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
        if (reportingTab.reportCfgInfo[i].used == 0) {
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
 * @return  None
 */
_CODE_ZCL_ void zcl_reportCfgInfoEntryRst(reportCfgInfo_t *pEntry)
{
    if (pEntry->used) {
        pEntry->minInterval = pEntry->minIntDft;
        pEntry->maxInterval = pEntry->maxIntDft;
        pEntry->minIntCnt = pEntry->minIntDft;
        pEntry->maxIntCnt = pEntry->maxIntDft;
        memset(pEntry->reportableChange, 0, REPORTABLE_CHANGE_MAX_ANALOG_SIZE);

        for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
            zcl_reportTimerEvt_t *pTimerEvt = &reportTimer[i];
            if (pTimerEvt->pEntry == pEntry) {
                reportAttrTimerStop(pTimerEvt);
                break;
            }
        }

        reportAttrTimerStart();
    }
}

/*********************************************************************
 * @fn      zcl_reportCfgInfoEntryUpdate
 *
 * @brief
 *
 * @param   pEntry
 *          endPoint
 *          profileId
 *          clusterId
 *          pCfgReportRec
 *
 * @return  None
 */
_CODE_ZCL_ void zcl_reportCfgInfoEntryUpdate(reportCfgInfo_t *pEntry, u8 endPoint, u16 profileId, u16 clusterId, zclCfgReportRec_t *pCfgReportRec)
{
    if (!pEntry->used) {
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
        if (zcl_analogDataType(pEntry->dataType)) {
            memcpy(pEntry->reportableChange, pCfgReportRec->reportableChange, zcl_getDataTypeLen(pEntry->dataType));
        }

        zclAttrInfo_t *pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
        if (!pAttrEntry) {
            //should not happen.
            ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
        }
        //u8 len = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);
        //memcpy(pEntry->prevData, pAttrEntry->data, len);

        pEntry->minIntCnt = pCfgReportRec->minReportInt;
        pEntry->maxIntCnt = pCfgReportRec->maxReportInt;
        pEntry->used = 1;

        reportingTab.reportNum++;
    } else {
        //update
        pEntry->minInterval = pCfgReportRec->minReportInt;
        pEntry->maxInterval = pCfgReportRec->maxReportInt;
        pEntry->minIntCnt = pCfgReportRec->minReportInt;
        pEntry->maxIntCnt = pCfgReportRec->maxReportInt;
        if (zcl_analogDataType(pEntry->dataType)) {
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
 *          curValue
 *          prevValue
 *          reportableChange
 *
 * @return  TRUE or FALSE
 */
_CODE_ZCL_ bool reportableChangeValueChk(u8 dataType, u8 *curValue, u8 *prevValue, u8 *reportableChange)
{
    bool needReport = FALSE;

    switch (dataType) {
    case ZCL_DATA_TYPE_UINT8:
        {
            u8 P = prevValue[0];
            u8 C = curValue[0];
            u8 R = reportableChange[0];
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_UINT16:
        {
            u16 P = BUILD_U16(prevValue[0], prevValue[1]);
            u16 C = BUILD_U16(curValue[0], curValue[1]);
            u16 R = BUILD_U16(reportableChange[0], reportableChange[1]);

            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_UINT24:
        {
            u24 P = BUILD_U24(prevValue[0], prevValue[1], prevValue[2]);//*((u24 *)prevValue);
            u24 C = BUILD_U24(curValue[0], curValue[1], curValue[2]);//*((u24 *)curValue);
            u24 R = BUILD_U24(reportableChange[0], reportableChange[1], reportableChange[2]);//*((u24 *)reportableChange);
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_UINT32:
        {
            u32 P = BUILD_U32(prevValue[0], prevValue[1], prevValue[2], prevValue[3]);//*((u32 *)prevValue);
            u32 C = BUILD_U32(curValue[0], curValue[1], curValue[2], curValue[3]);//*((u32 *)curValue);
            u32 R = BUILD_U32(reportableChange[0], reportableChange[1], reportableChange[2], reportableChange[3]);//*((u32 *)reportableChange);
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_INT8:
        {
            s8 P = (s8)prevValue[0];
            s8 C = (s8)curValue[0];
            s8 R = (s8)reportableChange[0];
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_INT16:
        {
            s16 P = BUILD_S16(prevValue[0], prevValue[1]);
            s16 C = BUILD_S16(curValue[0], curValue[1]);
            s16 R = BUILD_S16(reportableChange[0], reportableChange[1]);

            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_INT24:
        {
            s24 P = BUILD_S24(prevValue[0], prevValue[1], prevValue[2]);//*((s24 *)prevValue);
            s24 C = BUILD_S24(curValue[0], curValue[1], curValue[2]);//*((s24 *)curValue);
            s24 R = BUILD_S24(reportableChange[0], reportableChange[1], reportableChange[2]);//*((s24 *)reportableChange);
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_INT32:
        {
            s32 P = BUILD_S32(prevValue[0], prevValue[1], prevValue[2], prevValue[3]);//*((s32 *)prevValue);
            s32 C = BUILD_S32(curValue[0], curValue[1], curValue[2], curValue[3]);//*((s32 *)curValue);
            s32 R = BUILD_S32(reportableChange[0], reportableChange[1], reportableChange[2], reportableChange[3]);//*((s32 *)reportableChange);
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    /*
    case ZCL_DATA_TYPE_SINGLE_PREC:
        {
            float P = *((float *)prevValue);
            float C = *((float *)curValue);
            float R = *((float *)reportableChange);
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
            }
        }
        break;
    case ZCL_DATA_TYPE_DOUBLE_PREC:
        {
            double P = *((double *)prevValue);
            double C = *((double *)curValue);
            double R = *((double *)reportableChange);
            if (P > C) {
                needReport = ((P - C) >= R) ? TRUE : FALSE;
            } else if (P < C) {
                needReport = ((C - P) >= R) ? TRUE : FALSE;
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
 * @fn      reportAttrs
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
_CODE_ZCL_ static void reportAttrs(void)
{
    struct report_t {
        u8 numAttr;
        zclReport_t attr[2];
    };

    struct report_t report;

    bool again = 0;
    u16 profileID = 0xFFFF;
    u16 clusterID = 0xFFFF;
    u8 endpoint = 0;
    reportCfgInfo_t *pEntry = NULL;
    zclAttrInfo_t *pAttrEntry = NULL;

    do {
        pEntry = NULL;
        pAttrEntry = NULL;

        clusterID = 0xFFFF;
        endpoint = 0;
        again = 0;
        memset((u8 *)&report, 0, sizeof(report));

        for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
            pEntry = &reportingTab.reportCfgInfo[i];

            if (pEntry->used && (pEntry->maxInterval != 0xFFFF) &&
                zb_bindingTblSearched(pEntry->clusterID, pEntry->endPoint)) {
                pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
                if (pAttrEntry) {
                    bool valid = 0;

                    u8 dataLen = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);
                    dataLen = (dataLen > REPORTABLE_CHANGE_MAX_ANALOG_SIZE) ? (REPORTABLE_CHANGE_MAX_ANALOG_SIZE) : (dataLen);

                    DEBUG(REPORT_DEBUG_ENABLE, "RP: cID = %x, aID = %x, min = %d, max = %d\n",
                          pEntry->clusterID, pEntry->attrID, pEntry->minIntCnt, pEntry->maxIntCnt);

                    if (!pEntry->maxIntCnt && pEntry->maxInterval) {
                        valid = 1;
                    } else if (!pEntry->minIntCnt) {
                        valid = 1;
                    }

                    if (valid) {
                        if (clusterID == 0xFFFF) {
                            clusterID = pEntry->clusterID;
                            profileID = pEntry->profileID;
                            endpoint = pEntry->endPoint;
                        } else if ((clusterID != pEntry->clusterID) ||
                                   (profileID != pEntry->profileID) ||
                                   (endpoint != pEntry->endPoint)) {
                            again = 1;
                            continue;
                        }

                        report.attr[report.numAttr].attrID = pAttrEntry->id;
                        report.attr[report.numAttr].dataType = pAttrEntry->type;
                        report.attr[report.numAttr].attrData = pAttrEntry->data;
                        report.numAttr++;

                        //update for the next comparison
                        memcpy(pEntry->prevData, pAttrEntry->data, dataLen);
                        pEntry->minIntCnt = pEntry->minInterval ? pEntry->minInterval : 0xFFFF;
                        pEntry->maxIntCnt = pEntry->maxInterval ? pEntry->maxInterval : 0xFFFF;

                        if (report.numAttr >= 2) {
                            again = 1;
                            break;
                        }
                    }
                }
            }
        }

        if (clusterID != 0xFFFF) {
            epInfo_t dstEpInfo;
            TL_SETSTRUCTCONTENT(dstEpInfo, 0);

            dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
            dstEpInfo.profileId = profileID;

            zcl_sendReportAttrsCmd(endpoint, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, clusterID, (zclReportCmd_t *)&report);
        }
    } while (again);
}

_CODE_ZCL_ static s32 reportingTimerCb(void *arg)
{
    reportAttrs();

    return -1;
}

_CODE_ZCL_ static void reportingTimerStart(void)
{
    if (!ev_timer_exist(&reportingTimer)) {
        reportingTimer.cb = reportingTimerCb;
        reportingTimer.data = NULL;
        ev_on_timer(&reportingTimer, 100);
    }
}

/*********************************************************************
 * @fn      reportAttrTimerCb
 *
 * @brief
 *
 * @param   arg
 *
 * @return  0 -- continue; -1 -- cancel
 */
_CODE_ZCL_ static s32 reportAttrTimerCb(void *arg)
{
    zcl_reportTimerEvt_t *pReportTimer = (zcl_reportTimerEvt_t *)arg;

    DEBUG(REPORT_DEBUG_ENABLE, "reportAttrTimerCb: %x\n", (u32)pReportTimer);

    if (pReportTimer->pEntry) {
        reportCfgInfo_t *pEntry = pReportTimer->pEntry;

        if (pEntry->used && (pEntry->maxInterval != 0xFFFF) &&
            zb_bindingTblSearched(pEntry->clusterID, pEntry->endPoint)) {
            if (pEntry->minIntCnt) {
                if (pEntry->minIntCnt >= pReportTimer->seconds) {
                    pEntry->minIntCnt -= pReportTimer->seconds;
                } else {
                    pEntry->minIntCnt = 0;
                }
            }
            if (pEntry->maxIntCnt) {
                if (pEntry->maxIntCnt >= pReportTimer->seconds) {
                    pEntry->maxIntCnt -= pReportTimer->seconds;
                } else {
                    pEntry->maxIntCnt = 0;
                }
            }
        }

        DEBUG(REPORT_DEBUG_ENABLE, "cID = %x, attrID = %x, min = %d, max = %d, sec = %d\n",
              pEntry->clusterID, pEntry->attrID, pEntry->minIntCnt, pEntry->maxIntCnt, pReportTimer->seconds);

        reportingTimerStart();

        pReportTimer->pEntry = NULL;
        pReportTimer->seconds = 0;
        pReportTimer->minOrMax = 0;
    }

    return -1;
}

/*********************************************************************
 * @fn      reportAttrTimerStop
 *
 * @brief
 *
 * @param   pTimerEvt
 *
 * @return  None
 */
_CODE_ZCL_ static void reportAttrTimerStop(zcl_reportTimerEvt_t *pTimerEvt)
{
    if (pTimerEvt->pEntry) {
        if (ev_timer_exist(&pTimerEvt->timer)) {
            ev_unon_timer(&pTimerEvt->timer);
        }

        pTimerEvt->pEntry = NULL;
        pTimerEvt->seconds = 0;
        pTimerEvt->minOrMax = 0;
    }
}

/*********************************************************************
 * @fn      reportAttrTimerStart
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
_CODE_ZCL_ static void reportAttrTimerStart(void)
{
    for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
        reportCfgInfo_t *pEntry = &reportingTab.reportCfgInfo[i];
        zcl_reportTimerEvt_t *pTimerEvt = &reportTimer[i];

        if (pEntry->used && (pEntry->maxInterval != 0xFFFF) &&
            zb_bindingTblSearched(pEntry->clusterID, pEntry->endPoint)) {
            u16 seconds = 0;
            u8 minOrMax = 1;

            zclAttrInfo_t *pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
            u8 dataLen = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);
            dataLen = (dataLen > REPORTABLE_CHANGE_MAX_ANALOG_SIZE) ? (REPORTABLE_CHANGE_MAX_ANALOG_SIZE) : (dataLen);

            if ((!zcl_analogDataType(pAttrEntry->type) && memcmp(pEntry->prevData, pAttrEntry->data, dataLen)) ||
                (zcl_analogDataType(pAttrEntry->type) && reportableChangeValueChk(pAttrEntry->type, pAttrEntry->data,
                                                                                  pEntry->prevData, pEntry->reportableChange))) {
                //update for the next comparison
                memcpy(pEntry->prevData, pAttrEntry->data, dataLen);

                if (pEntry->minInterval) {
                    if (pTimerEvt->pEntry && (pTimerEvt->minOrMax == 1)) {
                        reportAttrTimerStop(pTimerEvt);
                    }

                    seconds = pEntry->minInterval;
                    minOrMax = 0;
                } else {
                    reportAttrTimerStop(pTimerEvt);

                    pEntry->minIntCnt = 0;

                    reportingTimerStart();
                    continue;
                }
            } else {
                if (pEntry->maxInterval) {
                    if (pTimerEvt->pEntry && (pTimerEvt->minOrMax == 1) &&
                       (pTimerEvt->seconds != pEntry->maxInterval)) {
                        reportAttrTimerStop(pTimerEvt);
                    }

                    seconds = pEntry->maxInterval;
                    minOrMax = 1;
                } else {
                    if (pTimerEvt->pEntry && (pTimerEvt->minOrMax == 1)) {
                        reportAttrTimerStop(pTimerEvt);
                    }

                    continue;
                }
            }

            if (!pTimerEvt->pEntry) {
                DEBUG(REPORT_DEBUG_ENABLE, "SET_Timer: cID = %x, attrID = %x, sec = %d\n",
                      pEntry->clusterID, pEntry->attrID, seconds);

                pTimerEvt->pEntry = pEntry;
                pTimerEvt->seconds = seconds;
                pTimerEvt->minOrMax = minOrMax;
                pTimerEvt->timer.cb = reportAttrTimerCb;
                pTimerEvt->timer.data = (void *)pTimerEvt;
                ev_on_timer(&pTimerEvt->timer, seconds * 1000);
            }
        } else {
            reportAttrTimerStop(pTimerEvt);
        }
    }
}

/*********************************************************************
 * @fn      report_handler
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
_CODE_ZCL_ void report_handler(void)
{
    if (zb_isDeviceJoinedNwk()) {
        reportAttrTimerStart();
    }
}
