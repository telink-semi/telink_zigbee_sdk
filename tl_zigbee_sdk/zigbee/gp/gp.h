/********************************************************************************************************
 * @file     gp.h
 *
 * @brief    header file for Green Power
 *
 * @author
 * @date     May. 27, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
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
#ifndef GP_H
#define GP_H

#include "dGP_stub.h"
#include "../zdo/zb_af.h"
#include "../zcl/zcl.h"
#include "../zcl/general/zcl_greenPower.h"


#define GREEN_POWER_ENDPOINT							0xF2//242
#define GP_PROFILE_ID									0xA1E0

//for Green Power device frame
#define GP_ZB_PROTOCOL_VERSION							0x03

//GP Shared key
#define GP_SHARED_KEY             						{ 0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,\
                                    	  	  	  	  	  0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF }


//A.3.6.1.2.1, the time the GP endpoint of the sink and the proxy keeps the information on the received GPDF, in order to filter out duplicates.
#define GP_DUPLICATE_TIMEOUT_DEFAULT		2//2s

//A.3.6.3.2
#define GPP_COMMISSIONING_WINDOW_DEFAULT	180//180s

//A.3.9.1, step 8, time that the basic proxy device will be absent of the operational network due to bidirectional commissioning.
#define GP_TRANSMIT_CHANNEL_TIMEOUT			5//5s


//GPP functionality
/************************************************************************************************************
b0    ||  GP feature                                                                              ||    0b1
b1    ||  Direct communication (reception of GPDF via GP stub)                                    ||    0b1
b2    ||  Derived groupcast communication                                                         ||    0b1
b3    ||  Pre-commissioned groupcast communication                                                ||    0b1
b4    ||  Full unicast communication                                                              ||    0b0
b5    ||  Lightweight unicast communication                                                       ||    0b1
b6    ||  Reserved                                                                                ||    0b0
b7    ||  Bidirectional operation                                                                 ||    0b0
b8    ||  Proxy Table maintenance (active and passive, for GPD mobility and GPP robustness)       ||    0b0
b9    ||  Reserved                                                                                ||    0b0
b10   ||  GP commissioning                                                                        ||    0b1
b11   ||  CT-based commissioning                                                                  ||    0b1
b12   ||  Maintenance of GPD (deliver channel/key during operation)                               ||    0b0
b13   ||  gpdSecurityLevel = 0b00                                                                 ||    0b1
b14   ||  Deprecated: gpdSecurityLevel = 0b01                                                     ||    0b0
b15   ||  gpdSecurityLevel = 0b10                                                                 ||    0b1
b16   ||  gpdSecurityLevel = 0b11                                                                 ||    0b1
b17   ||  Reserved                                                                                ||    0b0
b18   ||  Reserved                                                                                ||    0b0
b19   ||  GPD IEEE address                                                                        ||    0b1
b20   ||  Reserved		                                                                          ||    0b0
      ||  b21 - b23 Reserved                                                                      ||    0b0
************************************************************************************************************/
#define GPP_FUNCTIONALITY                  0x09AC2F  // Table 42 - Format of the gppFunctionality attribute

//GPS functionality
/************************************************************************************************************
b0    ||  GP feature                                                                              ||    0b1
b1    ||  Direct communication (reception of GPDF via GP stub)                                    ||    0b1
b2    ||  Derived groupcast communication                                                         ||    0b1
b3    ||  Pre-commissioned groupcast communication                                                ||    0b1
b4    ||  Full unicast communication                                                              ||    0b0
b5    ||  Lightweight unicast communication                                                       ||    0b1
b6    ||  Proximity bidirectional operation                                                       ||    0b0
b7    ||  Multi-hop bidirectional operation                                                       ||    0b0
b8    ||  Proxy Table maintenance (active and passive, for GPD mobility and GPP robustness)       ||    0b0
b9    ||  Proximity commissioning (unidirectional and bidirectional)                              ||    0b1
b10   ||  Multi-hop commissioning (unidirectional and bidirectional)                              ||    0b1
b11   ||  CT-based commissioning                                                                  ||    0b1
b12   ||  Maintenance of GPD (deliver channel/key during operation)                               ||    0b0
b13   ||  gpdSecurityLevel = 0b00 in operation                                                    ||    0b1
b14   ||  Deprecated: gpdSecurityLevel = 0b01                                                     ||    0b0
b15   ||  gpdSecurityLevel = 0b10                                                                 ||    0b1
b16   ||  gpdSecurityLevel = 0b11                                                                 ||    0b1
b17   ||  Sink Table-based groupcast forwarding                                                   ||    0b0
b18   ||  Translation Table                                                                       ||    0b0
b19   ||  GPD IEEE address                                                                        ||    0b1
b20	  ||  Compact attribute reporting															  ||	0b0
      ||  b21 - b23 Reserved                                                                      ||    0b0
************************************************************************************************************/
#define GPS_FUNCTIONALITY                  0x09AE2F  // Table 28 - Format of the gpsFunctionality attribute

//GGP active functionality, A.3.4.2.8
#define GPP_ACTIVE_FUNCTIONALITY           0xFFFFFF
#define GPS_ACTIVE_FUNCTIONALITY           0xFFFFFF


#define GPP_MAX_PROXY_TABLE_ENTRIES		   0x05
#define GPS_MAX_SINK_TABLE_ENTRIES		   0x05


/***************************************************************************
*  @brief	Define for GP Response
*/
typedef union
{
	u8	channel;
	struct
	{
		u8	operationChannel:4;
		u8	basic:1;
		u8	reserved:3;
	}bits;
}gpd_channelConfigurationCmd_payload_t;

/***************************************************************************
* @brief	Define for GP (Commissioning) Notification
*/
typedef struct
{
	epInfo_t dstEpInfo;
	union{
		zcl_gp_notificationCmd_t gpNotificationCmd;
		zcl_gp_commissioningNotificationCmd_t gpCommissioningNotificationCmd;
	};
}gp_noti_t;

/***************************************************************************
* @brief	Define for APP layer
*/
typedef bool (*gpChangleChannelReqCb_t)(void);
typedef void (*gpCommissioningModeCb_t)(bool isCommissioningMode);

typedef struct
{
	gpChangleChannelReqCb_t 	gpChangleChannelReqCb;
	gpCommissioningModeCb_t 	gpCommissioningModeCb;
}gp_appCb_t;


/***************************************************************************
* @brief	Define for GP Proxy Table
*/
typedef struct
{
	gpProxyTabEntry_t gpProxyTab[GPP_MAX_PROXY_TABLE_ENTRIES];
	u8 gpProxyTabNum;
}gp_proxyTab_t;

/***************************************************************************
* @brief	Define for GP
*/
typedef struct
{
	ev_time_event_t *transmitChannelTimeoutEvt;
	ev_time_event_t *aliasConflictTimeoutEvt;
	ev_time_event_t *commissioningWindowTimeoutEvt;
	u16 gpAliasConflictAddr;
	u16 gpCommissionerAddr; //addr to which send the notifications during commissioning mode
	u16 gpCommissioningWindow;//default value 180 seconds
	gpProxyCommModeOpt_t gpCommissioningModeOpt;//commissioning option copy from Commissioning Mode command
	u8	gpInCommMode;		//global flag, in commissioning mode or in operational mode
	u8	gpDuplicateTimeout;	//the default value of 2 seconds can be modified by the application profile
	u8	gpOperationalChannel;//holder of the operational channel
	bool firstToForward;
}gp_ctx_t;




//global value
extern gp_ctx_t g_gpCtx;
extern gp_proxyTab_t g_gpProxyTab;


/**
 *  @brief  External variable for GP Attribute
 */
extern u8 zclGpAttr_gppMaxProxyTabEntries;
extern u16 zclGpAttr_clusterRev;
extern u8 zclGpAttr_proxyTabEntry[];
extern u8 zclGpAttr_gppFunc[];
extern u8 zclGpAttr_gppActiveFunc[];

extern u8 zclGpAttr_gpSharedSecKeyType;
extern u8 zclGpAttr_gpSharedSecKey[];
extern u8 zclGpAttr_gpLinkKey[];

extern u8 GP_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_gpClusterList[];
extern const af_simple_descriptor_t gp_simpleDesc;



void gp_init(void);
void gp_registerAppCb(gp_appCb_t *cb);
status_t zcl_gpCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

void gpStubInit(void);
void gpExitCommissioningMode(void);
void gpSwitchToTransmitChannel(gpd_channelConfigurationCmd_payload_t channel, u8 tempMasterTxChannel);
u16 gpAliasSrcAddrDerived(u8 appId, gpdId_t gpdId);
void gpTranimitGPDF(gp_data_req_t *pGpDataReq);


void gp_ProxyTabInit(void);
void gpProxyTabUpdate(void);
void gp_proxyTabEntryClear(gpProxyTabEntry_t *pEntry);
u8 gp_getProxyTabEntryTotalNum(void);
u8 gp_getProxyTabEntryLen(gpProxyTabEntry_t *pEntry);
gpProxyTabEntry_t *gp_proxyTabEntryFreeGet(void);
gpProxyTabEntry_t *gp_getProxyTabByGpdId(u8 appId, gpdId_t gpdId);
u8 lwSinkAddrListAdd(gpProxyTabEntry_t *pEntry, addrExt_t sinkIeeeAddr, u16 sinkNwkAddr);
u8 lwSinkAddrListRemove(gpProxyTabEntry_t *pEntry, addrExt_t sinkIeeeAddr, u16 sinkNwkAddr);
u8 sinkGroupListAdd(gpProxyTabEntry_t *pEntry, u16 sinkGroupID, u16 alias);
u8 sinkGroupListRemove(gpProxyTabEntry_t *pEntry, u16 sinkGroupID);
u8 gp_buildProxyTabEntryFormat(gpProxyTabEntry_t *pEntry, u8 *pBuf);



#endif	/* GP_H */
