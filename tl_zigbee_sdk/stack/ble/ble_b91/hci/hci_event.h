/********************************************************************************************************
 * @file     hci_event.h
 *
 * @brief    This is the header file for b91 BLE SDK
 *
 * @author	 BLE GROUP
 * @date         12,2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#ifndef HCI_EVENT_H_
#define HCI_EVENT_H_


#include "stack/ble/ble_b91/ble_common.h"


/**
 *  @brief  Definition for general HCI event packet
 */
typedef struct {
	hci_type_t type;
	u8         eventCode;
	u8         paraLen;
	u8         parameters[1];
} hci_event_t;


typedef struct {
	u8         numHciCmds;
	u8         opCode_OCF;
	u8		   opCode_OGF;
	u8         returnParas[1];
} hci_cmdCompleteEvt_t;


typedef struct {
	u8         status;
	u8         numHciCmds;
	u8         opCode_OCF;
	u8		   opCode_OGF;
} hci_cmdStatusEvt_t;

typedef struct{
	u16 		connHandle;
	u16 		numOfCmpPkts;
}numCmpPktParamRet_t;

typedef struct {
	u8         numHandles;
	numCmpPktParamRet_t retParams[1];//TODO
} hci_numOfCmpPktEvt_t;

typedef struct{
	u8  status;
	u16 connHandle;
	u8  verNr;
	u16 compId;
	u16 subVerNr;
}hci_readRemVerInfoCmplEvt_t;

typedef struct {
	hci_type_t type;
	u8         eventCode;
	u8         paraLen;
	u8         subEventCode;
	u8         parameters[1];
} hci_le_metaEvt_t;



/**
 *  @brief  Event Parameters for "7.7.5 Disconnection Complete event"
 */
typedef struct {
	u8	status;
	u16	connHandle;
	u8	reason;
} event_disconnection_t;





/**
 *  @brief  Event Parameters for "7.7.8 Encryption Change event"
 */
typedef struct {
	u8	status;
	u16	handle;
	u8  enc_enable;
} event_enc_change_t;


/**
 *  @brief  Event Parameters for "7.7.39 Encryption Key Refresh Complete event"
 */
typedef struct {
	u8	status;
	u16	handle;
} event_enc_refresh_t;

typedef struct {
	u8         status;
	u16        connHandle;
	u8         reason;
} hci_disconnectionCompleteEvt_t;

typedef struct {
	u8         status;
	u16        connHandle;
	u8         encryption_enable;
} hci_le_encryptEnableEvt_t;

typedef struct {
	u8	subcode;
	u8	status;
	u16	handle;
	u8	role;
	u8	peer_adr_type;
	u8	mac[6];
	u16	interval;
	u16	latency;
	u16	timeout;
	u8	accuracy;
} event_connection_complete_t;			//20-byte

typedef struct {
	u8	subcode;
	u8	status;
	u16	handle;
	u16	interval;
	u16	latency;
	u16	timeout;
} event_connection_update_t;			//20-byte

/**
 *  @brief  Definition for HCI Encryption Key Refresh Complete event
 */
typedef struct {
	u8         status;
	u16        connHandle;
} hci_le_encryptKeyRefreshEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.1 LE Connection Complete event"
 */
typedef struct {
	u8         subEventCode;
	u8         status;
	u16        connHandle;
	u8         role;
	u8         peerAddrType;
	u8         peerAddr[6];
	u16        connInterval;
	u16        slaveLatency;
	u16        supervisionTimeout;
	u8         masterClkAccuracy;
} hci_le_connectionCompleteEvt_t;


/* ACL Connection Role */
typedef enum {
	LL_ROLE_MASTER 	= 0,
	LL_ROLE_SLAVE 	= 1,
} acl_connection_role_t;

/**
 *  @brief  Event Parameters for "7.7.65.2 LE Advertising Report event"
 */
typedef struct {
	u8	subcode;
	u8	nreport;
	u8	event_type;
	u8	adr_type;
	u8	mac[6];
	u8	len;
	u8	data[1];
} event_adv_report_t;

/* Advertise report event type */
typedef enum {
	ADV_REPORT_EVENT_TYPE_ADV_IND 		= 0x00,
	ADV_REPORT_EVENT_TYPE_DIRECT_IND 	= 0x01,
	ADV_REPORT_EVENT_TYPE_SCAN_IND 		= 0x02,
	ADV_REPORT_EVENT_TYPE_NONCONN_IND 	= 0x03,
	ADV_REPORT_EVENT_TYPE_SCAN_RSP 		= 0x04,
} advReportEventType_t;



/**
 *  @brief  Event Parameters for "7.7.65.3 LE Connection Update Complete event"
 */
typedef struct {
	u8         subEventCode;
	u8         status;
	u16        connHandle;
	u16        connInterval;
	u16        connLatency;
	u16        supervisionTimeout;
} hci_le_connectionUpdateCompleteEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.4 LE Read Remote Features Complete event"
 */
#define LL_FEATURE_SIZE                                      			8
typedef struct {
	u8         subEventCode;
	u8         status;
	u16        connHandle;
	u8		   feature[LL_FEATURE_SIZE];
} hci_le_readRemoteFeaturesCompleteEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.5 LE Long Term Key Request event"
 */
typedef struct {
	u8         subEventCode;
	u16        connHandle;
	u8         random[8];
	u16        ediv;
} hci_le_longTermKeyRequestEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.6 LE Remote Connection Parameter Request event"
 */
typedef struct {
	u8         subEventCode;
	u16        connHandle;
	u16        IntervalMin;
	u16        IntervalMax;
	u16		   latency;
	u16		   timeout;
} hci_le_remoteConnParamReqEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.7 LE Data Length Change event"
 */
typedef struct {
	u8         subEventCode;
	u16        connHandle;  //no aligned, can not be used as pointer
	u16  	   maxTxOct;
	u16		   maxTxtime;
	u16  	   maxRxOct;
	u16		   maxRxtime;
} hci_le_dataLengthChangeEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.8 LE Read Local P-256 Public Key Complete event"
 */
typedef struct {
	u8         subEventCode;
	u8         status;
	u8         localP256Key[64];
} hci_le_readLocalP256KeyCompleteEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.9 LE Generate DHKey Complete event"
 */
typedef struct {
	u8         subEventCode;
	u8         status;
	u8         DHKey[32];
} hci_le_generateDHKeyCompleteEvt_t;

#if (LL_FEATURE_ENABLE_LL_PRIVACY)
typedef struct {
	u8         subEventCode;
	u8         status;
	u16        connHandle;
	u8         role;
	u8         peerAddrType;
	u8         peerAddr[BLE_ADDR_LEN];
	u8		   localRpa[BLE_ADDR_LEN];
	u8		   peerRpa[BLE_ADDR_LEN];
	u16        connInterval;
	u16        connLatency;
	u16        supervisionTimeout;
	u8         masterClkAccuracy;
} hci_le_enhancedConnectionCompleteEvt_t;
#endif

/**
 *  @brief  Event Parameters for "7.7.65.10 LE Enhanced Connection Complete event"
 */
typedef struct {
	u8         subEventCode;
	u8         status;
	u16		   connHandle;
	u8		   role;
	u8		   PeerAddrType;
	u8         PeerAddr[6];
	u8         localRslvPrivAddr[6];
	u8         Peer_RslvPrivAddr[6];
	u16        connInterval;
	u16        conneLatency;
	u16        superTimeout;
	u8         mca;
} hci_le_enhancedConnCompleteEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.12 LE PHY Update Complete event"
 */
typedef struct {
	u8         subEventCode;
	u8         status;
	u16        connHandle;
	u8 		   tx_phy;
	u8		   rx_phy;
} hci_le_phyUpdateCompleteEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.13 LE Extended Advertising Report event"
 */
typedef struct{
	u16		event_type;
	u8		address_type;
	u8		address[6];
	u8		primary_phy;
	u8		secondary_phy;
	u8		advertising_sid;
	u8		tx_power;
	u8		rssi;
	u16		perd_adv_inter;	// Periodic_Advertising_Interval
	u8		direct_address_type;
	u8		direct_address[6];
	u8		data_length;  //24
	u8		data[1];
} extAdvEvt_info_t;

typedef struct{
	u8		subEventCode;
	u8		num_reports;
	u8		advEvtInfo[1];
} hci_le_extAdvReportEvt_t;


/* Extended Advertising Report Event Event_Type mask*/
typedef enum{
	EXT_ADV_RPT_EVT_MASK_CONNECTABLE				=	BIT(0),
	EXT_ADV_RPT_EVT_MASK_SCANNABLE     				=	BIT(1),
	EXT_ADV_RPT_EVT_MASK_DIRECTED  				    =   BIT(2),
	EXT_ADV_RPT_EVT_MASK_SCAN_RESPONSE     			=	BIT(3),
	EXT_ADV_RPT_EVT_MASK_LEGACY          			=	BIT(4),
	//EXT_ADV_RPT_EVT_MASK_DATA_STATUS          		=	(BIT(5) | BIT(6)),
}extAdvRptEvtMask_t;


/* Extended Advertising Report Event_Type */
typedef enum{
	EXT_ADV_RPT_EVT_TYPE_LEGACY_ADV_IND 				       	= 0x0013,		//  0001 0011'b
	EXT_ADV_RPT_EVT_TYPE_LEGACY_ADV_DIRECT_IND			       	= 0x0015,		//  0001 0101'b
	EXT_ADV_RPT_EVT_TYPE_LEGACY_ADV_SCAN_IND	 				= 0x0012,		//  0001 0010'b
	EXT_ADV_RPT_EVT_TYPE_LEGACY_ADV_NONCONN_IND					= 0x0010,		//  0001 0000'b
	EXT_ADV_RPT_EVT_TYPE_LEGACY_SCAN_RSP_2_ADV_IND				= 0x001B,		//  0001 1011'b
	EXT_ADV_RPT_EVT_TYPE_LEGACY_SCAN_RSP_2_ADV_SCAN_IND			= 0x001A,		//  0001 1010'b


	//TODO
//	EXT_ADV_RPT_EVT_TYPE_EXTENDED_NON_CONNECTABLE_NON_SCANNABLE_UNDIRECTED    	    = 0x0000,		//  0000 0000'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
//	EXT_ADV_RPT_EVT_TYPE_EXTENDED_CONNECTABLE_UNDIRECTED       				 	  	= 0x0001,		//  0000 0001'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
//	EXT_ADV_RPT_EVT_TYPE_SCANNABLE_UNDIRECTED						        = 0x0002,		//  0000 0010'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
//	EXT_ADV_RPT_EVT_TYPE_NON_CONNECTABLE_NON_SCANNABLE_DIRECTED				= 0x0004,		//  0000 0100'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
//	EXT_ADV_RPT_EVT_TYPE_CONNECTABLE_DIRECTED			       				= 0x0005,		//  0000 0101'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
//	EXT_ADV_RPT_EVT_TYPE_SCANNABLE_DIRECTED								    = 0x0006,		//  0000 0110'b		ADV_EXT_IND + AUX_ADV_IND/AUX_CHAIN_IND
}extAdvRptEvtType_t;  //extended advertising report event type


/* Address type */
typedef enum{
	EXTADV_RPT_PUBLIC_DEVICE_ADDRESS				=	0x00,
	EXTADV_RPT_RANDOM_DEVICE_ADDRESS     			=	0x01,
	EXTADV_RPT_PUBLIC_IDENTITY_ADDRESS  			=   0x02,
	EXTADV_RPT_RANDOM_IDENTITY_ADDRESS     			=	0x03,
}ext_adv_adr_type_t;


/**
 *  @brief  Event Parameters for "7.7.65.14 LE Periodic Advertising Sync Established event"
 */
typedef struct {
	//TODO
} hci_le_PeriodicAdvSyncEstablishedEvt_t;





/**
 *  @brief  Event Parameters for "7.7.65.14 LE Periodic Advertising Sync Established event"
 */


/**
 *  @brief  Event Parameters for "7.7.65.15 LE Periodic Advertising Report event"
 */
typedef struct {
	//TODO
} hci_le_periodicAdvReportEvt_t;


/**
 *  @brief  Event Parameters for "7.7.65.16 LE Periodic Advertising Sync Lost event"
 */
typedef struct {
	//TODO
} hci_le_periodicAdvSyncLostEvt_t;



typedef struct {
	//TODO
} hci_le_scanTimeoutEvt_t;




/**
 *  @brief  Event Parameters for "7.7.65.18 LE Advertising Set Terminated event"
 */
typedef struct {
	u8         subEventCode;
	u8         status;
	u8         advHandle;
	u16		   connHandle;
	u8		   num_compExtAdvEvt;
} hci_le_advSetTerminatedEvt_t;


/**
 *  @brief  Event Parameters for "7.7.65.19 LE Scan Request Received event"
 */
typedef struct {
	//TODO
} hci_le_scanReqRcvdEvt_t;


/**
 *  @brief  Event Parameters for "7.7.65.20 LE Channel Selection Algorithm event"
 */
typedef struct {
	u8         subEventCode;
	u16        connHandle;
	u8		   channel_selection_algorithm;
} hci_le_chnSelectAlgorithmEvt_t;






/**
 *  @brief  Event Parameters for "7.7.65.25 LE CIS Established event"
 */
typedef struct {
	u8        	subEventCode;
	u8        	status;
	u16			cisHandle;
	u8          cigSyncDly[3];
	u8          cisSyncDly[3];
	u8          transLaty_m2s[3];
	u8          transLaty_s2m[3];
	u8			phy_m2s;
	u8			phy_s2m;
	u8			nse;
	u8			bn_m2s;
	u8			bn_s2m;
	u8			ft_m2s;
	u8			ft_s2m;
	u16			maxPDU_m2s;
	u16			maxPDU_s2m;
	u16			isoIntvl;
} hci_le_cisEstablishedEvt_t;


/**
 *  @brief  Event Parameters for "7.7.65.26 LE CIS Request event"
 */
typedef struct {
	u8        	subEventCode;
	u16        	aclHandle;
	u16        	cisHandle;
	u8			cigId;
	u8			cisId;
} hci_le_cisReqEvt_t;


/**
 *  @brief  Event Parameters for "7.7.65.27 LE Create BIG Complete event"
 */
typedef struct {
	u8        	subEventCode;
	u8        	status;
	u8			bigHandle;
	u8        	bigSyncDly[3];
	u8          transLatyBig[3];
	u8			phy;
	u8			nse;
	u8			bn;
	u8			pto;
	u8			irc;
	u16			maxPDU;
	u16			isoIntvl;
	u8			numBis;
	u16		    bisHandles[1];//BIS_IN_BIG_NUM_MAX];
} hci_le_createBigCompleteEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.28 LE Terminate BIG Complete event"
 */
typedef struct {
	u8        	subEventCode;
	u8			bigHandle;
	u8			reason;
} hci_le_terminateBigCompleteEvt_t;


/**
 *  @brief  Event Parameters for "7.7.65.20 LE Channel Selection Algorithm event"
 */
typedef struct {
	u8        	subEventCode;
	u8        	status;
	u8			bigHandle;
	u8          transLatyBig[3];
	u8			nse;
	u8			bn;
	u8			pto;
	u8			irc;
	u16			maxPDU;
	u16			isoIntvl;
	u8			numBis;
	u16         bisHandles[1];//BIS_IN_BIG_NUM_MAX];
} hci_le_bigSyncEstablishedEvt_t;


/**
 *  @brief  Event Parameters for "7.7.65.29 LE BIG Sync Established event"
 */
typedef struct {
	u8        	subEventCode;
	u8			bigHandle;
	u8			reason;
} hci_le_bigSyncLostEvt_t;



/**
 *  @brief  Event Parameters for "7.7.65.30 LE BIG Sync Lost event"
 */




int	 hci_le_cisEstablished_evt(u8 status, u16 cisHandle, u8 cigSyncDly[3], u8 cisSyncDly[3], u8 transLaty_m2s[3], u8 transLaty_s2m[3], u8 phy_m2s,
		                      u8 phy_s2m, u8 nse, u8 bn_m2s, u8 bn_s2m, u8 ft_m2s, u8 ft_s2m, u16 maxPDU_m2s, u16 maxPDU_s2m, u16 isoIntvl );
int  hci_le_cisReq_evt(u16 aclHandle, u16 cisHandle, u8 cigId, u8 cisId);

int  hci_le_creatBigComplete_evt(u8 status, u8 bigHandle, u8 bigSyncDly[3], u8 transLatyBig[3], u8 phy, u8 nse,
								  u8 bn, u8 pto, u8 irc, u16 maxPDU, u16 isoIntvl, u8 numBis, u16* bisHandles);
int  hci_le_terminateBigComplete_evt(u8 bigHandle, u8 reason);
int  hci_le_bigSyncEstablished_evt(u8 status, u8 bigHandle, u8 transLatyBig[3], u8 nse, u8 bn, u8 pto, u8 irc,
		                          u16 maxPDU, u16 isoIntvl,  u8 numBis, u16* bisHandles);
int  hci_le_bigSyncLost_evt(u8 bigHandle, u8 reason);

void hci_disconnectionComplete_evt(u8 status, u16 connHandle, u8 reason);
int  hci_cmdComplete_evt(u8 numHciCmds, u8 opCode_ocf, u8 opCode_ogf, u8 paraLen, u8 *para, u8 *result);
void hci_cmdStatus_evt(u8 numHciCmds, u8 opCode_ocf, u8 opCode_ogf, u8 status, u8 *result);


void hci_le_connectionComplete_evt(u8 status, u16 connHandle, u8 role, u8 peerAddrType, u8 *peerAddr,
                                   u16 connInterval, u16 slaveLatency, u16 supervisionTimeout, u8 masterClkAccuracy);
void hci_le_connectionUpdateComplete_evt(u8 status, u16 connHandle, u16 connInterval,
        									u16 connLatency, u16 supervisionTimeout);
void hci_le_readRemoteFeaturesComplete_evt(u8 status, u16 connHandle, u8 * feature);

void hci_le_channel_selection_algorithm_evt(u16 connhandle, u8 channel_selection_alg);
void hci_le_phyUpdateComplete_evt(u16 connhandle,u8 status, u8 new_phy);
void hci_le_data_len_update_evt(u16 connhandle,u16 effTxOctets, u16 effRxOctets, u16 maxtxtime, u16 maxrxtime);



int hci_le_longTermKeyRequest_evt(u16 connHandle, u8* random, u16 ediv, u8* result);
int hci_le_readLocalP256KeyComplete_evt(u8* localP256Key, u8 status);
int hci_le_generateDHKeyComplete_evt(u8* DHkey, u8 status);

#if (LL_FEATURE_ENABLE_LL_PRIVACY)
void hci_le_enhancedConnectionComplete_evt(u8 status, u16 connHandle, u8 role, u8 peerAddrType, u8 *peerAddr, u8 *loaclRpa, u8 *peerRpa,
                                           u16 connInterval, u16 connLatency, u16 supervisionTimeout, u8 masterClkAccuracy);
#endif
int hci_le_encryptChange_evt(u16 connhandle,  u8 encrypt_en);
int hci_le_encryptKeyRefresh_evt(u16 connhandle);

int hci_remoteNateReqComplete_evt (u8* bd_addr);


#endif /* HCI_EVENT_H_ */





