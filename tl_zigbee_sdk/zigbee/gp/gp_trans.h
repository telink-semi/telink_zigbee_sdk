/********************************************************************************************************
 * @file    gp_trans.h
 *
 * @brief   This is the header file for gp_trans
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

#ifndef GP_TRANS_H
#define GP_TRANS_H


#define GPS_MAX_TRANS_TABLE_ENTRIES				0x05

#define ZB_CMD_PAYLOAD_LEN_MAX					8

#define ZCL_GP_MAX_TRANS_TABLE_ATTR_LEN			50


typedef union
{
	u8 opt;
	struct
	{
		u8 optLen:4;
		u8 optId:4;
	}bits;
}optSelector_t;

typedef union
{
	u8 opt;
	struct
	{
		u8 side:1;
		u8 manuIdPresent:1;
		u8 resv:6;
	}bits;
}attrOpt_t;

typedef struct
{
	u8 contactStatus;
	u8 contactBitmask;
}genSwitchCmd_t;

typedef struct _attribute_packed_{
	u8 reportId;
	u8 attrOffsetWithinReport;
	u16 clusterId;
	u16 attrId;
	u8 attrDataType;
	attrOpt_t attrOpt;
	u16 manuId;
}compactAttrReport_t;

typedef struct _attribute_packed_{
	optSelector_t optSelector;
	genSwitchCmd_t cmd;
	gpGenericSwCfg_t switchCfg;
}optRecordVectorCmd_t;

typedef struct _attribute_packed_{
	optSelector_t optSelector;
	compactAttrReport_t report;
}optRecordCompactAttrReport_t;

typedef union _attribute_packed_{
	optRecordVectorCmd_t vectorCmd;	//4-bytes
	optRecordCompactAttrReport_t compactReport;	//11-bytes
}optRecord_t;

/*
 *  @brief	Sink translation table entry
 */
typedef struct _attribute_packed_{
	gpdId_t	gpdId;
	transOpt_t options;
	u8 gpdEndpoint;
	u8 gpdCommand;
	u8 endpoint;
	u16 profileId;
	u16 clusterId;
	u8 zbCommandId;
	u8 zbCmdPayloadLen;
	u8 optRecordUsed;
	u8 used;		//20-bytes
	u8 zbCmdPayload[ZB_CMD_PAYLOAD_LEN_MAX];	//8-bytes
	optRecord_t optRecord;
}transTabEntry_t;	//39-bytes

/*
 *  @brief	Sink translation configuration
 */
typedef struct
{
	u8 gpdDevId;
	u8 gpdCmdId;
	u16 zbClusterId;
	u8 zbCmdId;
	u8 zbCmdPayloadLen;
	u8 zbCmdPayload[ZB_CMD_PAYLOAD_LEN_MAX];
}gp_transCfg_t;

/***************************************************************************
* @brief	Define for GP Sink Translation Table
*/
typedef struct _attribute_packed_{
	transTabEntry_t transTab[GPS_MAX_TRANS_TABLE_ENTRIES];
	u8 transTabNum;
}gp_translationTab_t;

extern u8 GP_TRANS_CFG_LIST_NUM;
extern const gp_transCfg_t g_gpTransCfgList[];
extern gp_translationTab_t g_gpTransTab;


void gp_transTabInit(void);
void gp_transTabNvUpdate(void);
u8 gp_transTabNumGet(void);
u8 gp_transTabEntryLenGet(transTabEntry_t *pTransEntry);
u8 gp_buildTransTabEntryFormat(transTabEntry_t *pTransEntry, u8 *pBuf);
void gp_transTabEntryClear(transTabEntry_t *pTransEntry);
void gp_transTabEntryRemove(u8 appId, gpdId_t gpdId, u8 endpoint);
transTabEntry_t *gp_transTabReportUpdate(u8 appId, gpdId_t gpdId, u8 endpoint,
					   	   	 	 	 	 u8 reportId, bool side,
					   	   	 	 	 	 u16 clusterId, u16 manuId,
					   	   	 	 	 	 gpAttrRecord_t *pAttrRecord);
transTabEntry_t *gp_transTabGenSwitchUpdate(u8 appId, gpdId_t gpdId, u8 endpoint,
											gpSwitchInfo_t *pSwitchInfo);
transTabEntry_t *gp_transTabEntryFind(u8 appId, gpdId_t gpdId, u8 endpoint,
		  	  	  	  	  	  	  	  u8 gpdCommand, u8 zbEndpoint,
		  	  	  	  	  	  	  	  u16 clusterId, u16 profileId,
									  u8 reportId, u16 attrId);
transTabEntry_t *gpTransTabUpdate(u8 appId, gpdId_t gpdId, u8 endpoint,
								  transTabEntry_t *pTransTabEntry,
								  gpTranslation_t *pTransLation);
void gp_transTabUpdateWithCfg(u8 appId, gpdId_t gpdId, u8 endpoint,
							  u8 devId, u8 numOfCmd,
							  u8 *pCmdList, u8 *pClusterList);
void gp_gpdfTranslate(u8 appId, gpdId_t gpdId, u8 endpoint,
					  u8 gpdCmdId, u8 dataLen, u8 *pData);

#endif	/* GP_TRANS_H */
