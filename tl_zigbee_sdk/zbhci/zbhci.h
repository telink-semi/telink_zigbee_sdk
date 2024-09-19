/********************************************************************************************************
 * @file    zbhci.h
 *
 * @brief   This is the header file for zbhci
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

#ifndef ZBHCI_H
#define	ZBHCI_H



/** Macro to send a log message to the host machine
 *  First byte of the message is the level (0-7).
 *  Remainder of message is char buffer containing ascii message
 */

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
#define ZBHCI_MSG_STATUS_ERROR_START_CHAR    0xE0
#define ZBHCI_MSG_STATUS_MSG_OVERFLOW        0xE1
#define ZBHCI_MSG_STATUS_ERROR_END_CHAR      0xE2
#define ZBHCI_MSG_STATUS_BAD_MSG             0xE3
#define ZBHCI_MSG_STATUS_UART_EXCEPT         0xE4
#define ZBHCI_MSG_STATUS_CRC_ERROR			 0xE5

#define	ZBHCI_MSG_START_FLAG				 0x55
#define	ZBHCI_MSG_END_FLAG					 0xAA
#define	ZBHCI_MSG_HDR_LEN					 0x07

#define HCI_OTA_BLOCK_SIZE_MAX					40
#define HCI_OTA_BLOCK_INTERVAL_NORMAL			10
#define HCI_OTA_BLOCK_REQUEST_RETRY_INTERVAL	5*100
#define HCI_OTA_BLOCK_REQUEST_RETRY_CNT_MAX		5

typedef enum{
	ZBHCI_GET_MAC_ADDR_MODE,
	ZBHCI_NORMAL_MODE
}workingMode_e;

typedef enum{
	ZBHCI_CMD_BDB_COMMISSION_FORMATION  	= 0x0001,
	ZBHCI_CMD_BDB_COMMISSION_STEER      	,//= 0x0002,
	ZBHCI_CMD_BDB_COMMISSION_TOUCHLINK  	,//= 0x0003,
	ZBHCI_CMD_BDB_COMMISSION_FINDBIND   	,//= 0x0004,
	ZBHCI_CMD_BDB_FACTORY_RESET         	,//= 0x0005,
	ZBHCI_CMD_BDB_PRE_INSTALL_CODE      	,//= 0x0006,
	ZBHCI_CMD_BDB_CHANNEL_SET,
	ZBHCI_CMD_BDB_DONGLE_WORKING_MODE_SET,
	ZBHCI_CMD_BDB_NODE_DELETE,
	ZBHCI_CMD_BDB_TX_POWER_SET,
	ZBHCI_CMD_ACKNOWLEDGE					= 0x8000,

	ZBHCI_CMD_DISCOVERY_NWK_ADDR_REQ		= 0x0010,
	ZBHCI_CMD_DISCOVERY_IEEE_ADDR_REQ   	,//= 0x0011,
	ZBHCI_CMD_DISCOVERY_NODE_DESC_REQ		,//= 0x0012,
	ZBHCI_CMD_DISCOVERY_SIMPLE_DESC_REQ		,//= 0x0013,
	ZBHCI_CMD_DISCOVERY_MATCH_DESC_REQ		,//= 0x0014,
	ZBHCI_CMD_DISCOVERY_ACTIVE_EP_REQ		,//= 0x0015,

	ZBHCI_CMD_DISCOVERY_NWK_ADDR_RSP		= 0x8010,
	ZBHCI_CMD_DISCOVERY_IEEE_ADDR_RSP   	,//= 0x0011,
	ZBHCI_CMD_DISCOVERY_NODE_DESC_RSP		,//= 0x0012,
	ZBHCI_CMD_DISCOVERY_SIMPLE_DESC_RSP		,//= 0x0013,
	ZBHCI_CMD_DISCOVERY_MATCH_DESC_RSP		,//= 0x0014,
	ZBHCI_CMD_DISCOVERY_ACTIVE_EP_RSP		,//= 0x0015,

	ZBHCI_CMD_BINDING_REQ					= 0x0020,
	ZBHCI_CMD_UNBINDING_REQ					,//= 0x0021,

	ZBHCI_CMD_BINDING_RSP					= 0x8020,
	ZBHCI_CMD_UNBINDING_RSP					,//= 0x0021,

	ZBHCI_CMD_MGMT_LQI_REQ					= 0x0030,
	ZBHCI_CMD_MGMT_BIND_REQ					,//= 0x0031,
	ZBHCI_CMD_MGMT_LEAVE_REQ				,//= 0x0032,
	ZBHCI_CMD_MGMT_DIRECT_JOIN_REQ			,//= 0x0033,
	ZBHCI_CMD_MGMT_PERMIT_JOIN_REQ			,//= 0x0034,
	ZBHCI_CMD_MGMT_NWK_UPDATE_REQ			,//= 0x0035,

	ZBHCI_CMD_MGMT_LQI_RSP					= 0x8030,
	ZBHCI_CMD_MGMT_BIND_RSP					,//= 0x0031,
	ZBHCI_CMD_MGMT_LEAVE_RSP				,//= 0x0032,
	ZBHCI_CMD_MGMT_DIRECT_JOIN_RSP			,//= 0x0033,
	ZBHCI_CMD_MGMT_PERMIT_JOIN_RSP			,//= 0x0034,
	ZBHCI_CMD_MGMT_NWK_UPDATE_NOTIFY		,//= 0x0035,

	ZBHCI_CMD_NODES_JOINED_GET_REQ			= 0x0040,
	ZBHCI_CMD_NODES_TOGGLE_TEST_REQ			= 0x0041,
	ZBHCI_CMD_TXRX_PERFORMANCE_TEST_REQ		= 0x0042,
	ZBHCI_CMD_AF_DATA_SEND_TEST_REQ			= 0x0044,
	ZBHCI_CMD_GET_LOCAL_NWK_INFO_REQ		= 0x0045,
	ZBHCI_CMD_GET_CHILD_NODES_REQ           = 0x0046,
	ZBHCI_CMD_REMOVE_ALL_CHILD_NODES_REQ    = 0x0047,

	ZBHCI_CMD_NODES_JOINED_GET_RSP			= 0x8040,
	ZBHCI_CMD_NODES_TOGGLE_TEST_RSP			= 0x8041,
	ZBHCI_CMD_TXRX_PERFORMANCE_TEST_RSP		= 0x8042,
	ZBHCI_CMD_NODES_DEV_ANNCE_IND			= 0x8043,
	ZBHCI_CMD_AF_DATA_SEND_TEST_RSP			= 0x8044,
	ZBHCI_CMD_GET_LOCAL_NWK_INFO_RSP		= 0x8045,
	ZBHCI_CMD_GET_CHILD_NODES_RSP           = 0x8046,

	ZBHCI_CMD_ZCL_ATTR_READ					= 0x0100,
	ZBHCI_CMD_ZCL_ATTR_WRITE				,//= 0x0101,
	ZBHCI_CMD_ZCL_CONFIG_REPORT				,//= 0x0102,
	ZBHCI_CMD_ZCL_READ_REPORT_CFG			,//= 0x0103,

	ZBHCI_CMD_ZCL_ATTR_READ_RSP				= 0x8100,
	ZBHCI_CMD_ZCL_ATTR_WRITE_RSP			,//= 0x0101,
	ZBHCI_CMD_ZCL_CONFIG_REPORT_RSP			,//= 0x0102,
	ZBHCI_CMD_ZCL_READ_REPORT_CFG_RSP		,//= 0x0103,
	ZBHCI_CMD_ZCL_REPORT_MSG_RCV,
	ZBHCI_CMD_ZCL_DEFAULT_RSP,

	ZBHCI_CMD_ZCL_BASIC						= 0x0110,
	ZBHCI_CMD_ZCL_BASIC_RESET				= 0x0110,

	ZBHCI_CMD_ZCL_GROUP						= 0x0120,
	ZBHCI_CMD_ZCL_GROUP_ADD					= 0x0120,
	ZBHCI_CMD_ZCL_GROUP_VIEW,
	ZBHCI_CMD_ZCL_GROUP_GET_MEMBERSHIP,
	ZBHCI_CMD_ZCL_GROUP_REMOVE,
	ZBHCI_CMD_ZCL_GROUP_REMOVE_ALL,
	ZBHCI_CMD_ZCL_GROUP_ADD_IF_IDENTIFY,

	ZBHCI_CMD_ZCL_GROUP_ADD_RSP				= 0x8120,
	ZBHCI_CMD_ZCL_GROUP_VIEW_RSP,
	ZBHCI_CMD_ZCL_GROUP_GET_MEMBERSHIP_RSP,
	ZBHCI_CMD_ZCL_GROUP_REMOVE_RSP,

	ZBHCI_CMD_ZCL_IDENTIFY 					= 0x0130,
	ZBHCI_CMD_ZCL_IDENTIFY_QUERY,

	ZBHCI_CMD_ZCL_IDENTIFY_QUERY_RSP		= 0x8131,

	ZBHCI_CMD_ZCL_ONOFF						= 0x0140,
	ZBHCI_CMD_ZCL_ONOFF_ON					= 0x0140,
	ZBHCI_CMD_ZCL_ONOFF_OFF,
	ZBHCI_CMD_ZCL_ONOFF_TOGGLE,

	ZBHCI_CMD_ZCL_LEVEL						= 0x0150,
	ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL			= 0x0150,
	ZBHCI_CMD_ZCL_LEVEL_MOVE,
	ZBHCI_CMD_ZCL_LEVEL_STEP,
	ZBHCI_CMD_ZCL_LEVEL_STOP,
	ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL_WITHONOFF,
	ZBHCI_CMD_ZCL_LEVEL_MOVE_WITHONOFF,
	ZBHCI_CMD_ZCL_LEVEL_STEP_WITHONOFF,
	ZBHCI_CMD_ZCL_LEVEL_STOP_WITHONOFF,

	ZBHCI_CMD_ZCL_SCENE						= 0x0160,
	ZBHCI_CMD_ZCL_SCENE_ADD					= 0x0160,
	ZBHCI_CMD_ZCL_SCENE_VIEW,
	ZBHCI_CMD_ZCL_SCENE_REMOVE,
	ZBHCI_CMD_ZCL_SCENE_REMOVE_ALL,
	ZBHCI_CMD_ZCL_SCENE_STORE,
	ZBHCI_CMD_ZCL_SCENE_RECALL,
	ZBHCI_CMD_ZCL_SCENE_GET_MEMBERSHIP,

	ZBHCI_CMD_ZCL_SCENE_ADD_RSP				= 0x8160,
	ZBHCI_CMD_ZCL_SCENE_VIEW_RSP,
	ZBHCI_CMD_ZCL_SCENE_REMOVE_RSP,
	ZBHCI_CMD_ZCL_SCENE_REMOVE_ALL_RSP,
	ZBHCI_CMD_ZCL_SCENE_STORE_RSP,
	ZBHCI_CMD_ZCL_SCENE_GET_MEMBERSHIP_RSP	= 0x8166,

	ZBHCI_CMD_ZCL_COLOR						= 0x0170,
	ZBHCI_CMD_ZCL_COLOR_MOVE2HUE        	= 0x0170,
	ZBHCI_CMD_ZCL_COLOR_MOVE2COLOR,
	ZBHCI_CMD_ZCL_COLOR_MOVE2SAT,
	ZBHCI_CMD_ZCL_COLOR_MOVE2TEMP,

	ZBHCI_CMD_ZCL_IAS_ZONE					= 0x0180,

	ZBHCI_CMD_ZCL_OTA_IMAGE_NOTIFY			= 0x0190,

	ZBHCI_CMD_DATA_CONFIRM					= 0x8200,//data confirm
	ZBHCI_CMD_MAC_ADDR_IND					= 0x8201,
	ZBHCI_CMD_NODE_LEAVE_IND				= 0x8202,

	ZBHCI_CMD_OTA_START_REQUEST				= 0x0210,
	ZBHCI_CMD_OTA_BLOCK_RESPONSE			= 0x0211,

	ZBHCI_CMD_OTA_START_RESPONSE			= 0x8210,
	ZBHCI_CMD_OTA_BLOCK_REQUEST				= 0x8211,
	ZBHCI_CMD_OTA_END_STATUS				= 0x8212,
}teHCI_MsgType;

/** Status message */
typedef enum
{
    ZBHCI_MSG_STATUS_SUCCESS,
    ZBHCI_MSG_STATUS_INCORRECT_PARAMETERS,
    ZBHCI_MSG_STATUS_UNHANDLED_COMMAND,
    ZBHCI_MSG_STATUS_BUSY,
    ZBHCI_MSG_STATUS_NO_MEMORY,
    ZBHCI_MSG_STATUS_STACK_ALREADY_STARTED,
}zbhci_msgStatus_e;

typedef enum{
	ZBHCI_OTA_SUCCESS,
    ZBHCI_OTA_GET_BLOCK_TIMEOUT,
    ZBHCI_OTA_IN_PROGRESS,
    ZBHCI_OTA_INCORRECT_OFFSET,
    ZBHCI_OTA_FILE_OVERSIZE,
    ZBHCI_OTA_INCORRECT_DATA,
    ZBHCI_OTA_MATCH_BOOT_FLAG,
	ZBHCI_OTA_BOOT_ADDR_ERROR,
}zbhci_ota_status_e;

typedef enum{
	ZBHCI_TX_SUCCESS,
	ZBHCI_TX_BUFFERFULL,
	ZBHCI_TX_BUSY,
	ZBHCI_TX_FAILED,
	ZBHCI_TX_TOO_LONG,
}zbhciTx_e;

typedef enum{
	ZBHCI_ADDRMODE_BOUND,
	ZBHCI_ADDRMODE_GROUP,
	ZBHCI_ADDRMODE_SHORT,
	ZBHCI_ADDRMODE_IEEE,
	ZBHCI_ADDRMODE_BRC,
	ZBHCI_ADDRMODE_BOUNDNOACK,
	ZBHCI_ADDRMODE_SHORTNOACK,
	ZBHCI_ADDRMODE_IEEENOACK
}zbhciTxMode_e;

typedef enum{
	ZBHCI_OTA_REMOTE_OTA_BIN,
	ZBHCI_OTA_LOCAL_BIN
}zbhciOtaType_e;

#define ZB_LEBESWAP(ptr,len)								\
	for(int i=0; i<(len>>1);i++){							\
		unsigned char temp = ptr[len - i - 1];				\
		ptr[len - i - 1] = ptr[i];							\
		ptr[i] = temp;										\
	}														\


#define ZB_IEEE_ADDR_REVERT(tar,addr)						\
	for(int i=0; i<EXT_ADDR_LEN;i++){						\
		(tar)[i]=(addr)[EXT_ADDR_LEN - i - 1];				\
	}														\

#define ZB_LINK_KEY_REVERT(tar,addr)						\
	for(int i=0; i<SEC_KEY_LEN;i++){						\
		(tar)[i]=(addr)[SEC_KEY_LEN - i - 1];				\
	}

#define ZB_16BIT_REVERT(tar,addr)							\
	for(int i=0; i<2;i++){									\
		(tar)[i]=(addr)[2 - i - 1];							\
	}

#define	ZB_LEBESWAPU16(u16Value)	(u16Value = (u16Value>>8)|(u16Value<<8))


typedef struct{
	u8  startFlag;
	u8	msgType16H;
	u8	msgType16L;
	u8	msgLen16H;
	u8	msgLen16L;
	u8	checkSum;
	u8	pData[1];
}zbhci_msg_t;

typedef struct{
	u16 cmdId;
	u16 payloadLen;
	u8  payload[1];
}zbhci_cmdHandler_t;


/*
 * @brief  the hci command format to get the MAC address of the nodes which have joined the network
 *
 * */
typedef struct{
	u16 startIndex;
}zbhci_mgmt_nodesJoined_req_t;


/*
 * @brief  the hci response command carryin  the MAC address of the nodes which have joined the network
 *
 * */
typedef struct _attribute_packed_{
	u16 totalCnt;				/*!	the total count of the joined nodes */
	u16 startIndex;				/*!	the start index */
	u8  listCnt;				/*!	the count of the mac address list */
	u8  status;					/*!	the status */
}zbhci_mgmt_nodesJoined_rsp_hdr_t;

typedef struct _attribute_packed_{
	addrExt_t macAddr;		// /*!	the mac address list */
	u16 nwkAddr;
}zbhci_mgmt_nodesJoined_info_t;

typedef struct _attribute_packed_{
	zbhci_mgmt_nodesJoined_rsp_hdr_t hdr;
	zbhci_mgmt_nodesJoined_info_t addrList[5];		//[5];	/*!	the mac address list */
}zbhci_mgmt_nodesJoined_rsp_t;


/*
 * @brief  the hci command format to set test data to get the percent received
 *
 * */
typedef struct{
	u16 dstAddr;
	u8	srcEp;
	u8	dstEp;
	u16	sendCnt;
	u8	interval;    //unit: 10ms
	u8	txPowerSet;
	u8	payload[6];
}txrx_performce_test_req_t;

typedef struct{
	u16 dstAddr;
	u16	sendCnt;
	u16	ackCnt;
}txrx_performce_test_rsp_t;

typedef struct{
	u16 dstAddr;
	u8	srcEp;
	u8	dstEp;
	u16 clusterId;
	u16 dataLen;
	u8  payload[];
}zbhci_afDataSend_req_t;

typedef struct{
	u16 srcAddr;
	u8	srcEp;
	u8	dstEp;
	u16 clusterId;
	u16 dataLen;
	u8  payload[];
}zbhci_afDataSend_rsp_t;

typedef struct{
	addrExt_t	macAddr;
}zbhci_mgmt_nodeDeleteReq_t;

typedef struct{
	u16			totalCnt;
	addrExt_t	macAddr;
}zbhci_nodeLeaveInd_t;

typedef struct _attribute_packed_{
	u8 dstMode;
	tl_zb_addr_t dstAddr; //addrMode = APS_DSTADDR_EP_NOTPRESETNT
	u8 srcEp;			//
	u8 dstEp;			//addrMode = APS_DSTADDR_EP_NOTPRESETNT APS_SHORT_GROUPADDR_NOEP
	u16 clusterId;
	u8 status;
	u8 apsCnt;
}zbhci_app_data_confirm_t;

typedef struct{
	ev_timer_event_t *performanceTestTmrEvt;
	u16 sendTotalCnt;
	u16 sendSuccessCnt;
	u16 rcvTotalCnt;
	u16 dstAddr;
	u8	dstEp;
	u8  dataApsCnt;
	u8	performanceTest;
}zbhci_afTestReq_t;

typedef struct{
	u32 otaFlashAddrStart;
	u32 otaFileTotalSize;
	u32 otaFileOffset;
	u32 otaCrcValue;
	u8  otaBootFlag[4];
	u8  otaFindBootFlag;
	u8  otaProcessStart;
	u8  blockRequestCnt;
	u8  binType;//zbhciOtaType_e, 1--local bin, 0--remote OTA bin
}hci_ota_info_t;

typedef struct{
	u8 startIdx;             /*! start of the child table index */
}zbhci_childNodeGetReq_t;

extern zbhci_afTestReq_t g_afTestReq;


u8 crc8Calculate(u16 type, u16 length, u8 *data);
void zbhciCmdHandler(u16 msgType, u16 msgLen, u8 *p);
void zbhciAppDataSendConfirmPush(void *arg);
void zbhciAppNodeLeaveIndPush(void *arg);
void zbhciAfDataPerformanceResultPush(void);
void zbhciAfDataRcvIndPush(void *arg);
void zbhciAfDataCnfPush(void *arg);
bool zbhciMacAddrGetPush(addrExt_t devExtAddr);

extern zbhciTx_e zbhciTx(u16 u16Type, u16 u16Length, u8 *pu8Data);
extern void zbhciInit(void);
extern void zbhciTask(void);

#endif

