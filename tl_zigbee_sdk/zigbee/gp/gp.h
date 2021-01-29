/********************************************************************************************************
 * @file	gp.h
 *
 * @brief	This is the header file for gp
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
#ifndef GP_H
#define GP_H




#define GREEN_POWER_ENDPOINT							0xF2//242

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
	ev_timer_event_t *transmitChannelTimeoutEvt;
	ev_timer_event_t *aliasConflictTimeoutEvt;
	ev_timer_event_t *commissioningWindowTimeoutEvt;
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
