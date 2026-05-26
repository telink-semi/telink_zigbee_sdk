/********************************************************************************************************
 * @file    hci_event.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef HCI_EVENT_H_
#define HCI_EVENT_H_


#include "stack/ble/ble_multi/ble_common.h"

/**
 *  @brief  Definition for general HCI event packet
 */
typedef struct __attribute__((packed))
{
    u8 type;
    u8 eventCode;
    u8 paraLen;
    u8 parameters[1];
} hci_event_t;

typedef struct __attribute__((packed))
{
    u8 numHciCmds;
    u8 opCode_OCF;
    u8 opCode_OGF;
    u8 returnParas[1];
} hci_cmdCompleteEvt_evtParam_t;

typedef struct __attribute__((packed))
{
    u8 status;
    u8 numHciCmds;
    u8 opCode_OCF;
    u8 opCode_OGF;
} hci_cmdStatusEvt_evtParam_t;

typedef struct __attribute__((packed))
{
    u16 connHandle;
    u16 numOfCmpPkts;
} numCmpPktParamRet_t;

typedef struct __attribute__((packed))
{
    u8                  numHandles;
    numCmpPktParamRet_t retParams[1];
} hci_numOfCmpPktEvt_t;

typedef struct __attribute__((packed))
{
    u8  status;
    u16 connHandle;
    u8  verNr;
    u16 compId;
    u16 subVerNr;
} hci_readRemVerInfoCmplEvt_t;

typedef struct __attribute__((packed))
{
    u8 type;
    u8 eventCode;
    u8 paraLen;
    u8 subEventCode;
    u8 parameters[1];
} hci_le_metaEvt_t;

/**
 *  @brief  Event Parameters for "7.7.5 Disconnection Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  status;
    u16 connHandle;
    u8  reason;
} hci_disconnectionCompleteEvt_t;

/* compatible with previous released SDK */
#define event_disconnection_t hci_disconnectionCompleteEvt_t

/**
 *  @brief  Event Parameters for "7.7.8 Encryption Change event"
 */
typedef struct __attribute__((packed))
{
    u8  status;
    u16 handle;
    u8  enc_enable;
} event_enc_change_t;

typedef struct __attribute__((packed))
{
    u8  status;
    u16 connHandle;
    u8  encryption_enable;
} hci_le_encryptEnableEvt_t;

/**
 *  @brief  Event Parameters for "7.7.39 Encryption Key Refresh Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  status;
    u16 handle;
} event_enc_refresh_t;

typedef struct __attribute__((packed))
{
    u8  status;
    u16 connHandle;
} hci_le_encryptKeyRefreshEvt_t;


//    HDT EVENT    //
typedef struct __attribute__((packed))
{
    hci_le_encryptEnableEvt_t encryptEnable;
    u8 encryptionKeySize;
    u8 micLength;
    u8 pfs_dbg_key;
} hci_encryptEnableV3Evt_t;

typedef struct __attribute__((packed))
{
    hci_le_encryptKeyRefreshEvt_t keyFresh;
    u8  micLength;
    u8  pfs_dbg_key;
} hci_encryptKeyRefreshV2Evt_t;

/**
 *  @brief  Event Parameters for "7.7.65.1 LE Connection Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u8  role;
    u8  peerAddrType;
    u8  peerAddr[6];
    u16 connInterval;
    u16 peripheralLatency;
    u16 supervisionTimeout;
    u8  masterClkAccuracy;
} hci_le_connectionCompleteEvt_t;

/* ACL Connection Role */
typedef enum
{
    ACL_ROLE_CENTRAL    = 0,
    ACL_ROLE_PERIPHERAL = 1,
} acl_connection_role_t;

/* compatible with previous released SDK */
#define LL_ROLE_MASTER ACL_ROLE_CENTRAL
#define LL_ROLE_SLAVE  ACL_ROLE_PERIPHERAL

/**
 *  @brief  Event Parameters for "7.7.65.2 LE Advertising Report event"
 */
typedef struct __attribute__((packed))
{
    u8 subcode;
    u8 nreport;
    u8 event_type;
    u8 adr_type;
    u8 mac[6];
    u8 len;
    u8 data[1];
} event_adv_report_t;

/* Advertise report event type */
typedef enum
{
    ADV_REPORT_EVENT_TYPE_ADV_IND     = 0x00,
    ADV_REPORT_EVENT_TYPE_DIRECT_IND  = 0x01,
    ADV_REPORT_EVENT_TYPE_SCAN_IND    = 0x02,
    ADV_REPORT_EVENT_TYPE_NONCONN_IND = 0x03,
    ADV_REPORT_EVENT_TYPE_SCAN_RSP    = 0x04,
} advReportEventType_t;

/**
 *  @brief  Event Parameters for "7.7.65.3 LE Connection Update Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u16 connInterval;
    u16 connLatency;
    u16 supervisionTimeout;
} hci_le_connectionUpdateCompleteEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.4 LE Read Remote Features Complete event"
 */
#define LL_FEATURE_SIZE 8

typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u8  feature[LL_FEATURE_SIZE];
} hci_le_readRemoteFeaturesCompleteEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.5 LE Long Term Key Request event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 connHandle;
    u8  random[8];
    u16 ediv;
} hci_le_longTermKeyRequestEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.6 LE Remote Connection Parameter Request event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 connHandle;
    u16 IntervalMin;
    u16 IntervalMax;
    u16 latency;
    u16 timeout;
} hci_le_remoteConnParamReqEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.7 LE Data Length Change event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 connHandle; //no aligned, can not be used as pointer
    u16 maxTxOct;
    u16 maxTxtime;
    u16 maxRxOct;
    u16 maxRxtime;
} hci_le_dataLengthChangeEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.8 LE Read Local P-256 Public Key Complete event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 status;
    u8 localP256Key[64];
} hci_le_readLocalP256KeyCompleteEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.9 LE Generate DHKey Complete event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 status;
    u8 DHKey[32];
} hci_le_generateDHKeyCompleteEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.10 LE Enhanced Connection Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u8  role;
    u8  PeerAddrType;
    u8  PeerAddr[6];
    u8  localRslvPrivAddr[6];
    u8  Peer_RslvPrivAddr[6];
    u16 connInterval;
    u16 connLatency;
    u16 superTimeout;
    u8  masterClkAccuracy;
} hci_le_enhancedConnCompleteEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.10 LE Enhanced Connection Complete event V2"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u8  role;
    u8  PeerAddrType;
    u8  PeerAddr[6];
    u8  localRslvPrivAddr[6];
    u8  Peer_RslvPrivAddr[6];
    u16 connInterval;
    u16 connLatency;
    u16 superTimeout;
    u8  masterClkAccuracy;
    u8  Advertising_Handle;
    u16 Sync_Handle;
} hci_le_enhancedConnCompleteEvt_t_V2;

/**
 *  @brief  Event Parameters for "7.7.65.11 LE Directed Advertising Report event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 num_reports;
    u8 event_type;
    u8 addr_type;
    u8 address[6];
    u8 direct_addr_type;
    u8 direct_address[6];
    s8 rssi;
} hci_le_directAdvRptEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.12 LE PHY Update Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u8  tx_phy;
    u8  rx_phy;
} hci_le_phyUpdateCompleteEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.13 LE Extended Advertising Report event"
 */
typedef struct __attribute__((packed))
{
    u16 event_type;          // 2
    u8  address_type;        // 1
    u8  address[6];          // 6
    u8  primary_phy;         // 1
    u8  secondary_phy;       // 1
    u8  advertising_sid;     // 1
    u8  tx_power;            // 1
    s8  rssi;                // 1
    u16 perd_adv_inter;      // 2   Periodic_Advertising_Interval
    u8  direct_address_type; // 1
    u8  direct_address[6];   // 6
    u8  data_length;         // 1
    u8  data[1];
} extAdvEvt_info_t;

typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  num_reports;
    u16 event_type; // 2
    //4~11
    u8 address_type; // 1
    u8 address[6];   // 6
    u8 primary_phy;  // 1

    //12~15
    u8 secondary_phy;   // 1
    u8 advertising_sid; // 1
    u8 tx_power;        // 1
    u8 rssi;            // 1

    //16~25
    u16 perd_adv_inter;      // 2   Periodic_Advertising_Interval
    u8  direct_address_type; // 1
    u8  direct_address[6];   // 6
    u8  data_length;         // 1   //offset=25
    u8  data[2];             // 2 indicate align 4B
} extAdvRptEvt_t;            //need align 4B

/* Direct_Address_Type*/
typedef enum
{
    DIRECT_ADDR_PUBLIC      = 0x00, //Public Device Address
    DIRECT_ADDR_NRPA_STATIC = 0x01, //Non-resolvable Private Address or Static Device Address
    DIRECT_ADDR_RPA_PUBLIC  = 0x02, //Resolvable Private Address (resolved by Controller; Own_Address_ress_Type_Type was 0x00 or 0x02)
    DIRECT_ADDR_RPA_RANDOM  = 0x03, //Resolvable Private Address (resolved by Controller; Own_Address_ress_Type_Type was 0x01 or 0x03)
    DIRECT_ADDR_RPA_FAIL    = 0xFE, //Resolvable Private Address (Controller unable to resolve)
} direct_addr_type_t;

/**
 *  @brief  Event Parameters for "7.7.65.15 LE Periodic Advertising Report event"
 */
typedef struct __attribute__((packed))
{
    u8  sub_code;
    u16 sync_handle;
    u8  tx_power;

    u8 rssi;
    u8 cte_type;
    u8 data_status;
    u8 data_len; // 0 to 247

    u8 data[1];
} le_periodAdvReportEvt_t;

typedef struct __attribute__((packed))
{
    u8  sub_code;
    u16 sync_handle;
    u8  tx_power;

    u8  rssi;
    u8  cte_type;
    u16 periodic_evt_cnt;

    u8 subevent;
    u8 data_status;
    u8 data_len; // 0 to 247

    u8 data[1];
} le_periodAdvReportEvt_t_v2;

typedef struct __attribute__((packed))
{
    u8 I_sample;
    u8 Q_sample;
} IQ_sample_t;

/**
 *  @brief  Event Parameters for "7.7.65.21 LE Connectionless IQ Report event"
 */
typedef struct __attribute__((packed))
{
    u8  sub_code;
    u16 sync_handle;
    u8  channel_index;

    s16 rssi; //Units: 0.1 dBm
    u8  rssi_antenna_id;
    u8  cte_type;

    u8  slot_durations;
    u8  packet_status;
    u16 periodic_event_counter;

    u8          sample_count; // 9 to 82
    IQ_sample_t IQ_sample[82];
} hci_le_connectionlessIQReportEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.22 LE Connection IQ Report event"
 */
typedef struct __attribute__((packed))
{
    u8  sub_code;
    u16 conn_handle;
    u8  rx_phy;

    u8  data_channel_index;
    s16 rssi; //Units: 0.1 dBm
    u8  rssi_antenna_id;

    u8  cte_type;
    u8  slot_durations;
    u8  packet_status;
    u16 conn_event_counter;

    u8          sample_count; // 9 to 82
    IQ_sample_t IQ_sample[82];
} hci_le_connectionIQReportEvt_t;

#define PDAADV_INFO_LENGTH      8
#define PDAADV_RPT_DATA_LEN_MAX 247
#define EXTADV_INFO_LENGTH      24  //byte number from "event_type" to "data_length"
#define EXTADV_RPT_DATA_LEN_MAX 229 //253 - 24 = 229

typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 num_reports;
    u8 advEvtInfo[1];
} hci_le_extAdvReportEvt_t;

/* Extended Advertising Report Event Event_Type mask*/
typedef enum
{
    EXTADV_RPT_EVT_MASK_CONNECTABLE   = BIT(0),
    EXTADV_RPT_EVT_MASK_SCANNABLE     = BIT(1),
    EXTADV_RPT_EVT_MASK_DIRECTED      = BIT(2),
    EXTADV_RPT_EVT_MASK_SCAN_RESPONSE = BIT(3),
    EXTADV_RPT_EVT_MASK_LEGACY        = BIT(4),

    EXTADV_RPT_DATA_COMPLETE                = 0x00,
    EXTADV_RPT_DATA_INCOMPLETE_MORE_TO_COME = 0x20,
    EXTADV_RPT_DATA_INCOMPLETE_TRUNCATED    = 0x40,
    EXTADV_RPT_DATA_RFU                     = 0x60,

    EXTADV_RPT_EVTTYPE_MASK     = 0x1F,
    EXTADV_RPT_DATA_STATUS_MASK = 0x60,
} extAdvRptEvtMask_t;

/* Extended Advertising Report Event_Type */
typedef enum
{
    //Legacy
    EXTADV_RPT_EVTTYPE_LEGACY_ADV_IND                 = 0x0013, //  0001 0011'b
    EXTADV_RPT_EVTTYPE_LEGACY_ADV_DIRECT_IND          = 0x0015, //  0001 0101'b
    EXTADV_RPT_EVTTYPE_LEGACY_ADV_SCAN_IND            = 0x0012, //  0001 0010'b
    EXTADV_RPT_EVTTYPE_LEGACY_ADV_NONCONN_IND         = 0x0010, //  0001 0000'b
    EXTADV_RPT_EVTTYPE_LEGACY_SCAN_RSP_2_ADV_IND      = 0x001B, //  0001 1011'b
    EXTADV_RPT_EVTTYPE_LEGACY_SCAN_RSP_2_ADV_SCAN_IND = 0x001A, //  0001 1010'b

    //Extended
    EXTADV_RPT_EVTTYPE_EXT_NON_CONN_NON_SCAN_UNDIRECTED = 0x0000, // Extended, Non_Connectable Non_Scannable Undirected
    EXTADV_RPT_EVTTYPE_EXT_CONNECTABLE_UNDIRECTED       = 0x0001, // Extended, Connectable Undirected
    EXTADV_RPT_EVTTYPE_EXT_SCANNABLE_UNDIRECTED         = 0x0002, // Extended, Scannable Undirected
    EXTADV_RPT_EVTTYPE_EXT_NON_CONN_NON_SCAN_DIRECTED   = 0x0004, // Extended, Non_Connectable Non_Scannable Directed
    EXTADV_RPT_EVTTYPE_EXT_CONNECTABLE_DIRECTED         = 0x0005, // Extended, Connectable Directed
    EXTADV_RPT_EVTTYPE_EXT_SCANNABLE_DIRECTED           = 0x0006, // Extended, Scannable Directed
    EXTADV_RPT_EVTTYPE_EXT_SCAN_RESPONSE                = 0x0008, // Extended, AUX_SCAN_RESPONSE
} extAdvRptEvtType_t;                                             //extended advertising report event type

/* Address type */
typedef enum
{
    EXTADV_RPT_PUBLIC_DEVICE_ADDRESS   = 0x00,
    EXTADV_RPT_RANDOM_DEVICE_ADDRESS   = 0x01,
    EXTADV_RPT_PUBLIC_IDENTITY_ADDRESS = 0x02,
    EXTADV_RPT_RANDOM_IDENTITY_ADDRESS = 0x03,
    EXTADV_RPT_ANONYMOUS_ADV           = 0xFF,
} ext_adv_adr_type_t;

#define PERIODIC_ADV_INTER_NO_PERIODIC_ADV           0
#define SECONDARY_PHY_NO_PACKET_ON_SECONDARY_ADV_CHN 0
#define ADVERTISING_SID_NO_ADI_FIELD                 0xFF
#define TX_POWER_INFO_NOT_AVAILABLE                  127

/**
 *  @brief  Event Parameters for "7.7.65.14 LE Periodic Advertising Sync Established event v1"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 syncHandle;
    //  extadv_id_t adverting_id;
    u8  advSID;
    u8  advAddrType;
    u8  advAddr[6];
    u8  advPHY;
    u16 perdAdvItvl;
    u8  advClkAccuracy;
} hci_le_periodicAdvSyncEstablishedEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.14 LE Periodic Advertising Sync Established event v2"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 syncHandle;
    //  extadv_id_t adverting_id;
    u8  advSID;
    u8  advAddrType;
    u8  advAddr[6];
    u8  advPHY;
    u16 perdAdvItvl;
    u8  advClkAccuracy;

    ///the following is for pawr
    u8 num_subevent;
    u8 subevent_intvl;
    u8 rsp_slot_delay;
    u8 rsp_slot_spacing;
} hci_le_periodicAdvSyncEstablishedEvtV2_t;

/**
 *  @brief  Event Parameters for "7.7.65.15 LE Periodic Advertising Report event V1"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 syncHandle;
    u8  txPower;
    u8  RSSI;
    u8  cteType;
    u8  dataStatus;
    u8  dataLength; // 0 to 247 Length of the Data field
    u8  data[1];
} hci_le_periodicAdvReportEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.15 LE Periodic Advertising Report event V2"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 syncHandle;
    u8  txPower;

    u8  RSSI;
    u8  cteType;
    u16 paEventCounter;

    u8 subevent;
    u8 dataStatus;
    u8 dataLength; // 0 to 247 Length of the Data field
    u8 data[1];
} hci_le_periodicAdvReportEvtV2_t;

/**
 *  @brief  Event Parameters for "7.7.65.16 LE Periodic Advertising Sync Lost event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 syncHandle;
} hci_le_periodicAdvSyncLostEvt_t;

typedef struct __attribute__((packed))
{
    u8 subEventCode;
} hci_le_scanTimeoutEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.18 LE Advertising Set Terminated event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u8  advHandle;
    u16 connHandle;
    u8  num_compExtAdvEvt;
} hci_le_advSetTerminatedEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.19 LE Scan Request Received event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 advHandle;
    u8 scannerAddrType;
    u8 scannerAddr[6];
} hci_le_scanReqRcvdEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.20 LE Channel Selection Algorithm event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 connHandle;
    u8  channel_selection_algorithm;
} hci_le_chnSelectAlgorithmEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.24 LE Periodic Advertising Sync Transfer Received event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u16 serviceData;
    u16 syncHandle;
    u8  advSID;
    u8  advAddrType;
    u8  advAddr[6];
    u8  advPHY;
    u16 perdAdvItvl;
    u8  advClkAccuracy;
} hci_le_periodicAdvSyncTransferRcvdEvt_t;

typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u16 serviceData;
    u16 syncHandle;
    u8  advSID;
    u8  advAddrType;
    u8  advAddr[6];
    u8  advPHY;
    u16 perdAdvItvl;
    u8  advClkAccuracy;

    u8 num_subevt;
    u8 subevent_intvl;
    u8 rsp_slot_delay;
    u8 rsp_slot_spacing;
} hci_le_periodicAdvSyncTransferRcvdEvt_V2_t;

/**
 *  @brief  Event Parameters for "7.7.65.25 LE CIS Established event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 cisHandle;
    u8  cigSyncDly[3];
    u8  cisSyncDly[3];
    u8  transLaty_m2s[3];
    u8  transLaty_s2m[3];
    u8  phy_m2s; // le_phy_type_t: 0x01/0x02/0x03
    u8  phy_s2m; // le_phy_type_t: 0x01/0x02/0x03
    u8  nse;
    u8  bn_m2s;
    u8  bn_s2m;
    u8  ft_m2s;
    u8  ft_s2m;
    u16 maxPDU_m2s;
    u16 maxPDU_s2m;
    u16 isoIntvl;
} hci_le_cisEstablishedEvt_t;

typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 cisHandle;
    u8  cigSyncDly[3];
    u8  cisSyncDly[3];
    u8  transLaty_m2s[3];
    u8  transLaty_s2m[3];
    u8  phy_m2s; // le_phy_type_t: 0x01/0x02/0x03
    u8  phy_s2m; // le_phy_type_t: 0x01/0x02/0x03
    u8  nse;
    u8  bn_m2s;
    u8  bn_s2m;
    u8  ft_m2s;
    u8  ft_s2m;
    u16 maxPDU_m2s;
    u16 maxPDU_s2m;
    u16 isoIntvl;
    u8  sub_interval[3];
    u16 maxSdu_c2p;
    u16 maxSdu_p2c;
    u8  sduIntvl_c2p[3];
    u8  sduIntvl_p2c[3];
    u8  framing;
    u8  rates_c2p;
    u8  rates_p2c;
    u8  encrypt_en;
    u8  mic_length;
} hci_le_cisEstablishedV4Evt_t;

/**
 *  @brief  Event Parameters for "7.7.65.26 LE CIS Request event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 aclHandle;
    u16 cisHandle;
    u8  cigId;
    u8  cisId;
} hci_le_cisReqEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.27 LE Create BIG Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u8  bigHandle;
    u8  bigSyncDly[3];
    u8  transLatyBig[3];
    u8  phy;
    u8  nse;
    u8  bn;
    u8  pto;
    u8  irc;
    u16 maxPDU;
    u16 isoIntvl;
    u8  numBis;
    u16 bisHandles[1]; //LL_BIS_IN_PER_BIG_BCST_NUM_MAX];
} hci_le_createBigCompleteEvt_t;

typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u8  bigHandle;
    u8  bigSyncDly[3];
    u8  transLatyBig[3];
    u8  phy;
    u8  nse;
    u8  bn;
    u8  pto;
    u8  irc;
    u16 maxPDU;
    u16 isoIntvl;
    u16 rates;
    u8  encrypt_en;
    u8  mic_length;
    u8  numBis;
    u16 bisHandles[1]; //LL_BIS_IN_PER_BIG_BCST_NUM_MAX];
} hci_le_createBigCompleteV2Evt_t;

/**
 *  @brief  Event Parameters for "7.7.65.28 LE Terminate BIG Complete event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 bigHandle;
    u8 reason;
} hci_le_terminateBigCompleteEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.20 LE Channel Selection Algorithm event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u8  bigHandle;
    u8  transLatyBig[3];
    u8  nse;
    u8  bn;
    u8  pto;
    u8  irc;
    u16 maxPDU;
    u16 isoIntvl;
    u8  numBis;
    u16 bisHandles[1]; //BIS_IN_PER_BIG_SYNC_NUM_MAX];
} hci_le_bigSyncEstablishedEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.29 LE BIG Sync Established event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 bigHandle;
    u8 reason;
} hci_le_bigSyncLostEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.30 LE BIG Sync Lost event"
 */


/**
 *  @brief  Event Parameters for "7.7.65.34 LE BIGInfo Advertising Report event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 syncHandle;
    u8  numBis;
    u8  nse;
    u16 IsoItvl; //in units of 1.25 ms.
    u8  bn;
    u8  pto;
    u8  irc;
    u16 maxPdu;
    u8  sduItvl[3];
    u16 maxSdu;
    u8  phy;
    u8  framing;
    u8  enc;
} hci_le_bigInfoAdvReportEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.32 LE Path Loss Threshold event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u16 connHandle;
    u8  currPathLoss;
    u8  zoneEntered;
} hci_le_pathLossThresholdEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.33 LE Transmit Power Reporting event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u8  reason;
    u8  phy;
    s8  txPwrLvl;
    u8  txPwrLvlFlg;
    s8  delta;
} hci_le_transmitPwrRptingEvt_t;

/**
 * @brief  Event Parameters for "7.7.65.35 LE Subrate Change event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u16 subrate_factor;
    u16 peripheral_latency;
    u16 conti_num;
    u16 subrate_timeout;
} hci_le_subrateChangeEvt_t;

/**
 * @brief  Event Parameters for "7.7.65.36 LE Periodic Advertising Subevent Data Request event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 advHandle;
    u8 subevtStart;
    u8 subevtDataCount;
} hci_le_periodicAdvSubevtDataReqEvt_t;

/**
 * @brief  Event Parameters for "77.7.65.37 LE Periodic Advertising Response Report event"
 */
typedef struct __attribute__((packed))
{
    s8 txPower;
    s8 RSSI;
    u8 cteType;
    u8 responseSlot;
    u8 dataStatus;
    u8 dataLength; // 0 to 247 Length of the Data field
    u8 data[0];
} pawrRspReportDat_t;

typedef struct __attribute__((packed))
{
    u8                 subEventCode;
    u8                 advHandle;
    u8                 Subevent;
    u8                 Tx_Status;
    u8                 Num_Responses;
    pawrRspReportDat_t rspReportDat[0];
} hci_le_periodicAdvRspReportEvt_t;

typedef struct __attribute__((packed))
{
    u8  Subevent_Code;
    u8  Status;
    u16 Connection_Handle;
    u8  Num_Config_Supported;
    u16 Max_Consecutive_Procedures_Supported;
    u8  Num_Antennas_Supported;
    u8  Max_Antenna_Paths_Supported;
    u8  Roles_Supported;
    u8  Mode_Types;
    u8  RTT_Capability;
    u8  RTT_AA_Only_N;
    u8  RTT_Sounding_N;
    u8  RTT_Random_Payload_N;
    u16 Optional_NADM_Sounding_Capability;
    u16 Optional_NADM_Random_Capability;
    u8  Optional_CS_SYNC_PHYs_Supported;
    u16 Optional_Subfeatures_Supported;
    u16 Optional_T_IP1_Times_Supported;
    u16 Optional_T_IP2_Times_Supported;
    u16 Optional_T_FCS_Times_Supported;
    u16 Optional_T_PM_Times_Supported;
    u8  T_SW_Time_Supported;
    u8  Optional_TX_SNR_Capability;
} hci_le_readRemoteSupCapCompleteEvt_t;

typedef struct __attribute__((packed))
{
    u8  Subevent_Code;
    u8  Status;
    u16 Connection_Handle;
    u8  Remote_FAE_Table[72];
} hci_le_readRemoteFAETableCompleteEvt_t;

typedef struct __attribute__((packed))
{
    u8  Subevent_Code;
    u8  Status;
    u16 Connection_Handle;
} hci_le_csSecurityEnableCompleteEvt_t;

typedef struct __attribute__((packed))
{
    u8  Subevent_Code;
    u8  Status;
    u16 Connection_Handle;
    u8  Config_ID;
    u8  Action;
    u8  Main_Mode;
    u8  Sub_Mode;
    u8  Main_Mode_Min_Steps;
    u8  Main_Mode_Max_Steps;
    u8  Main_Mode_Repetition;
    u8  Mode_0_Steps;
    u8  Role;
    u8  RTT_Type;
    u8  CS_SYNC_PHY;
    u8  Channel_Map[10];
    u8  Channel_Map_Repetition;
    u8  ChSel;
    u8  Ch3c_Shape;
    u8  Ch3c_Jump;
    u8  Companion_Signal_Enable;
    u8  T_IP1_Time;
    u8  T_IP2_Time;
    u8  T_FCS_Time;
    u8  T_PM_Time;
} hci_le_csConfigCompleteEvt_t;

typedef struct __attribute__((packed))
{
    u8  Subevent_Code;
    u8  Status;
    u16 Connection_Handle;
    u8  Config_ID;
    u8  state;
    u8  Tone_Antenna_Config_Selection;
    u8  Selected_TX_Power;
    u8  Subevent_Len[3];
    u8  Subevents_Per_Event;
    u16 Subevent_Interval;
    u16 Event_Interval;
    u16 Procedure_Interval;
    u16 Procedure_Count;
    u16 Max_Procedure_Len;
} hci_le_csProcedureEnableCompleteEvt_t;

typedef struct __attribute__((packed))
{
    u8 mode;
    u8 channel;
    u8 len;
    u8 data[0];
} cs_step_value_t;

typedef struct __attribute__((packed))
{
    u8  Packet_Quality;
    s8  Packet_RSSI;
    u8  Packet_Antenna;
    s16 Measured_Freq_Offset;
} cs_step_mode0_t;

typedef struct __attribute__((packed))
{
    u8 Packet_Quality;
    u8 Packet_NADM;
    s8 Packet_RSSI;
    u8 ToA_ToD[2];
    u8 Packet_Antenna;
    u8 Packet_PCT1[4];
    u8 Packet_PCT2[4];
} cs_step_mode1_t;

typedef struct __attribute__((packed))
{
    u8 Tone_PCT[3];
    u8 Tone_Quality_Indicator;
} cs_step_tone_t;

typedef struct __attribute__((packed))
{
    u8             Antenna_Permutation_Index;
    cs_step_tone_t Tone[0];
} cs_step_mode2_t;

typedef struct __attribute__((packed))
{
    u8              Packet_Quality;
    u8              Packet_NADM;
    s8              Packet_RSSI;
    u8              ToA_ToD[2];
    u8              Packet_Antenna;
    u8              Packet_PCT1[4];
    u8              Packet_PCT2[4];
    cs_step_mode2_t Tone_Info;
} cs_step_mode3_t;

typedef struct __attribute__((packed))
{
    u8              Subevent_Code;
    u16             Connection_Handle;
    u8              Config_ID;
    u16             Start_ACL_Conn_Event;
    u16             Procedure_Counter;
    u16             Frequency_Compensation;
    s8              Reference_Power_Level;
    u8              Procedure_Done_Status;
    u8              Subevent_Done_Status;
    u8              Abort_Reason;
    u8              Num_Antenna_Paths;
    u8              Num_Steps_Reported;
    cs_step_value_t Step_Mode[0];

} hci_le_csSubeventResultEvt_t;

typedef struct __attribute__((packed))
{
    u8              Subevent_Code;
    u16             Connection_Handle;
    u8              Config_ID;
    u8              Procedure_Done_Status;
    u8              Subevent_Done_Status;
    u8              Abort_Reason;
    u8              Num_Antenna_Paths;
    u8              Num_Steps_Reported;
    cs_step_value_t Step_Mode[0];

} hci_le_csSubeventResultContinueEvt_t;

typedef struct __attribute__((packed))
{
    u8 Subevent_Code;
    u8 Status;
} hci_le_csTestEndCompleteEvt_t;


enum fsu_initiator {
    FSU_INITIATOR_LOCAL_HOST = 0x00,
    FSU_INITIATOR_LOCAL_CONTROLLER = 0x01,
    FSU_INITIATOR_PEER = 0x02,
};

/* Monitored Advertisers report event condition type */
typedef enum
{
    RSSI_BELOW_LOW_RSSI_THRESHOLD_TIMEOUT     = 0x00,
    SRRI_GREATER_HIGH_THRESHOLD  = 0x01,
} monitoredAdvertisersReportEventConditionType_t;

typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 addr_type;
    u8 address[6];
    u8 condition;   //see monitoredAdvertisersReportEventConditionType_t
} hci_le_monitioredAdvertisersReportEvt_t;

/**
 *  @brief  Event Parameters for "7.7.65.48 LE Frame Space Update Complete event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;

    u8  initiator;   //refer to 'enum fsu_initiator'
    u16 frame_space; //The new frame space value being used, in microseconds Range: 0x0000 to 0x2710
    u8  phy_mask;
    u16 spacing_types;
} hci_le_frameSpaceUpdateCompleteEvt_t;

/**
 *  @brief  Event Parameters for Telink Private "LE Connection Establish event"
 */
typedef struct __attribute__((packed))
{
    u8  subEventCode;
    u8  status;
    u16 connHandle;
    u8  role;
    u8  peerAddrType;
    u8  peerAddr[6];
    u16 connInterval;
    u16 peripheralLatency;
    u16 supervisionTimeout;
    u8  masterClkAccuracy;
} hci_tlk_connectionEstablishEvt_t;

/**
 *  @brief  Event Parameters for Telink Private "LE Create Connection Fail event"
 */
typedef struct __attribute__((packed))
{
    u8 subEventCode;
    u8 fail_reason;
    u8 create_conn_cnt;
} hci_tlk_createConnFailEvt_t;

/* create connection fail reason */
typedef enum
{
    INIT_TIMEOUT = 0x01,
    CONNECT_FAIL = 0x02,
} crt_conn_fail_reason_t;

int hci_numberOfCompletePacket_evt(u16 connHandle, u8 numOfCmpConn);

int hci_le_periodicAdvSyncEstablished_evt(u8 status, u16 syncHandle, u8 advSID, u8 advAddrType, u8 advAddress[6], u8 advPHY, u16 perdAdvItvl, u8 advClkAccuracy);
int hci_le_periodicAdvSyncEstablished_evt_v2(u8 status, u16 syncHandle, u8 advSID, u8 advAddrType, u8 advAddress[6], u8 advPHY, u16 perdAdvItvl, u8 advClkAccuracy, u8 num_subevent, u8 subevent_intvl, u8 rsp_slot_delay, u8 rsp_slot_spacing);
//int       hci_le_periodicAdvSyncEstablished_evt (u8 status, u16 syncHandle, extadv_id_t *pId, u8 advPHY, u16 perdAdvItvl, u8 advClkAccuracy);

int hci_le_periodicAdvReport_evt(u8 subEventCode, u16 syncHandle, u8 txPower, u8 RSSI, u8 cteType, u8 dataStatus, u8 dataLength, u8 *data);
int hci_le_periodicAdvSyncLost_evt(u16 syncHandle);
int hci_le_periodicAdvSyncTransferRcvd_evt(u8 status, u16 connHandle, u16 serviceData, u16 syncHandle, u8 advSID, u8 advAddrType, u8 advAddr[6], u8 advPHY, u16 perdAdvItvl, u8 advClkAccuracy);
int hci_le_cisEstablished_evt(u8 status, u16 cisHandle, u8 cigSyncDly[3], u8 cisSyncDly[3], u8 transLaty_m2s[3], u8 transLaty_s2m[3], u8 phy_m2s, u8 phy_s2m, u8 nse, u8 bn_m2s, u8 bn_s2m, u8 ft_m2s, u8 ft_s2m, u16 maxPDU_m2s, u16 maxPDU_s2m, u16 isoIntvl);
int hci_le_cisReq_evt(u16 aclHandle, u16 cisHandle, u8 cigId, u8 cisId);
int hci_le_createBigComplete_evt(u8 status, u8 bigHandle, u8 bigSyncDly[3], u8 transLatyBig[3], u8 phy, u8 nse, u8 bn, u8 pto, u8 irc, u16 maxPDU, u16 isoIntvl, u8 numBis, u16 *bisHandles);
int hci_le_terminateBigComplete_evt(u8 bigHandle, u8 reason);
int hci_le_bigSyncEstablished_evt(u8 status, u8 bigHandle, u8 transLatyBig[3], u8 nse, u8 bn, u8 pto, u8 irc, u16 maxPDU, u16 isoIntvl, u8 numBis, u16 *bisHandles);
int hci_le_bigSyncLost_evt(u8 bigHandle, u8 reason);
int hci_le_BigInfoAdvReport_evt(u16 syncHandle, u8 numBis, u8 nse, u16 IsoItvl, u8 bn, u8 pto, u8 irc, u16 maxPdu, u8 sduItvl[3], u16 maxSdu, u8 phy, u8 framing, u8 enc);
int hci_disconnectionComplete_evt(u8 status, u16 connHandle, u8 reason);
int hci_cmdComplete_evt(u8 numHciCmds, u8 opCode_ocf, u8 opCode_ogf, u8 paraLen, u8 *para, u8 *result);
int hci_cmdStatus_evt(u8 numHciCmds, u8 opCode_ocf, u8 opCode_ogf, u8 status, u8 *result);
int hci_le_connectionComplete_evt(u8 status, u16 connHandle, u8 role, u8 peerAddrType, u8 *peerAddr, u16 connInterval, u16 periphr_Latency, u16 supervisionTimeout, u8 masterClkAccuracy);
int hci_le_enhancedConnectionComplete_evt(u8 status, u16 connHandle, u8 role, u8 peerAddrType, u8 *peerAddr, u8 *localRpa, u8 *peerRpa, u16 connInterval, u16 connLatency, u16 supervisionTimeout, u8 masterClkAccuracy);
int hci_le_enhancedConnectionComplete_evt_v2(u8 status, u16 connHandle, u8 role, u8 peerAddrType, u8 *peerAddr, u8 *localRpa, u8 *peerRpa, u16 connInterval, u16 connLatency, u16 supervisionTimeout, u8 masterClkAccuracy, u8 advertisingHandle, u16 syncHandle);
int hci_le_connectionUpdateComplete_evt(u8 status, u16 connHandle, u16 connInterval, u16 connLatency, u16 supervisionTimeout);

int hci_le_directAdvertisingReport_evt(u8 addr_type, u8 *addr, u8 *direct_addr, s8 rssi);
int hci_le_readRemoteFeaturesComplete_evt(u8 status, u16 connHandle, u8 *feature);
int hci_le_channel_selection_algorithm_evt(u16 connhandle, u8 channel_selection_alg);
int hci_le_phyUpdateComplete_evt(u16 connhandle, u8 status, u8 new_phy);
int hci_le_data_len_update_evt(u16 connhandle, u16 effTxOctets, u16 effRxOctets, u16 maxtxtime, u16 maxrxtime);
int hci_le_longTermKeyRequest_evt(u16 connHandle, u8 *random, u16 ediv, u8 *result);
int hci_le_readLocalP256KeyComplete_evt(u8 *localP256Key, u8 status);
int hci_le_generateDHKeyComplete_evt(u8 *DHkey, u8 status);
int hci_le_encryptChange_evt(u16 connhandle, u8 encrypt_en);
int hci_le_encryptKeyRefresh_evt(u16 connhandle);
int hci_remoteNateReqComplete_evt(u8 *bd_addr);
int hci_le_pathLossThreshold_evt(u16 connHandle, u8 currPathLoss, u8 zoneEntered);
int hci_le_transmitPwrRpting_evt(u8 status, u16 connHandle, u8 reason, u8 phy, s8 txPwrLvl, u8 txPwrLvlFlg, s8 delta);
int hci_le_authPayloadTimeoutExpired_evt(u16 connHandle);

int hci_le_readRemoteSupCapComplete_evt(u8 status, u16 connHandle, u8 *data);
int hci_le_csProcedureEnableComplete_evt(u8 status, u16 connHandle, u8 *data);
int hci_le_readRemoteFAETableComplete_evt(u8 status, u16 connHandle, u8 *table);
int hci_le_csConfigComplete_evt(u8 status, u16 connHandle, u8 *data);
int hci_le_csSecurityEnableComplete_evt(u8 status, u16 connHandle);
int hci_le_csSubeventResult_evt(u16 connhandle, u8 config_id, u8 *data, u32 data_length);
int hci_le_csSubeventResultContinue_evt(u16 connhandle, u8 config_id, u8 *dtat, u32 data_length);
int hci_le_csTestEndComplete_evt(u8 status);

int hci_le_monitoredAdvertisersReport_evt(u8 addr_type, u8* addr, u8 condition);


int hci_tlk_connectionEstablish_evt(u8 status, u16 connHandle, u8 role, u8 peerAddrType, u8 *peerAddr, u16 connInterval, u16 periphr_Latency, u16 supervisionTimeout, u8 masterClkAccuracy);

int hci_tlk_createConnectionFail_evt(u8 fail_reason, u8 create_conn_cnt);

int hci_encryptChangeV3_evt(u16 connhandle, u8 encrypt_en, u8 encrypt_keysize, u8 mic_length, u8 pfs_dbg_key);
int hci_encryptKeyRefreshV2_evt(u16 connhandle, u8 mic_length ,u8 pfs_dbg_key);
int hci_le_cisEstablishedV4_evt(u8 status, u16 cisHandle, u8 cigSyncDly[3], u8 cisSyncDly[3], u8 transLaty_m2s[3], u8 transLaty_s2m[3],
                                u8 phy_m2s, u8 phy_s2m, u8 nse, u8 bn_m2s, u8 bn_s2m, u8 ft_m2s, u8 ft_s2m, u16 maxPDU_m2s,
                                u16 maxPDU_s2m, u16 isoIntvl, u8 sub_interval[3], u16 maxSdu_c2p, u16 maxSdu_p2c, u8 sduIntvl_c2p[3],
                                u8 sduIntvl_p2c[3], u8 framing, u8 rates_c2p, u8 rates_p2c, u8 encrypt_en, u8 mic_length);
int hci_le_createBigCompleteV2_evt(u8 status, u8 bigHandle, u8 bigSyncDly[3], u8 transLatyBig[3], u8 phy, u8 nse,u8 bn, u8 pto, u8 irc,
                                   u16 maxPDU, u16 isoIntvl, u16 rates, u8 encrypt_en, u8 mic_length, u8 numBis, u16 *bisHandles);
#endif /* HCI_EVENT_H_ */
