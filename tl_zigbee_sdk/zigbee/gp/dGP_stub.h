/********************************************************************************************************
 * @file	dGP_stub.h
 *
 * @brief	This is the header file for dGP_stub
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
#ifndef DGP_STUB_H
#define DGP_STUB_H



//A GP Basic Proxy device shall have a minimum length of 1 entry
#define GP_TX_QUEUE_MAX_NUM					1
#define GP_DATA_IND_SEC_REQ_TAB_NUM			4

#define GP_NONCE_SEC_CTRL						0x05
#define GP_NONCE_SEC_CTRL_OUTGOING_APP_ID_GP	0xc5

//A.1.5.2.1.2
#define GP_TX_OFFSET						20//ms
//A.1.5.2.1.3
#define GP_MAX_TX_OFFSET_VARIATION			5//ms
//A.1.5.2.1.4
#define GP_TX_DURATION						10//ms


//GP handle
typedef enum{
	GP_HANDLE_MIN = 0x70,
	GP_HANDLE_MAX = 0xBE,
	GP_HANDLE_CHANNEL_CONFIGURATION = 0xBF,
	//NWK_INTERNAL_NSDU_HANDLE = 0xC0,
}gp_handle_e;

/***************************************************************************
* @brief	dGP-DATA.indication
*/
typedef struct
{
	u8	*gpMpdu;
	u16	srcPanId;
	u16	dstPanId;
	tl_zb_addr_t srcAddr;
	tl_zb_addr_t dstAddr;
	u8	srcAddrMode;
	u8	dstAddrMode;
	s8	rssi;
	u8	lqi;
	u8	seqNum;
	u8	gpMpduLen;
}dgp_data_ind_t;


/************************************************************************************************
 ******************************** dGP stub service specification ********************************
 ***********************************************************************************************/

typedef enum
{
	GP_DATA_IND_STATUS_SEC_SUCCESS,
	GP_DATA_IND_STATUS_NO_SECURITY,
	GP_DATA_IND_STATUS_CNT_FAILURE,
	GP_DATA_IND_STATUS_AUTH_FAILURE,
	GP_DATA_IND_STATUS_UNPROCESSED
}gpDataInd_status_t;

/***************************************************************************
* @brief	gpSecurityKeyType values
*/
typedef enum
{
	GP_SEC_KEY_TYPE_NO_KEY	=	0x00,
	GP_SEC_KEY_TYPE_NWK_KEY,
	GP_SEC_KEY_TYPE_GPD_GROUP_KEY,
	GP_SEC_KEY_TYPE_NWK_KEY_DERIVED_GPD_GROUP_KEY,
	GP_SEC_KEY_TYPE_OUT_OF_THE_BOX_GPD_KEY,
	GP_SEC_KEY_TYPE_DERIVED_INDIVIDUAL_GPD_KEY	=	0x07,
}gpSecKeyType_t;

/***************************************************************************
* @brief	GP-DATA.indication
*/
typedef struct
{
	u8	*gpdAsdu;
	u32	srcId;
	u32	gpdSecFrameCnt;
	u32	mic;
	tl_zb_addr_t srcAddr;
	u16	srcPanId;
	bool autoCommissioning;
	bool rxAfterTx;
	u8	status;
	s8	rssi;
	u8	lqi;
	u8	seqNum;
	u8	srcAddrMode;
	u8	appId;
	u8	gpdfSecurityLevel;//0x00,0x02,0x03
	u8	gpdfKeyType;//0x00 - 0x07
	u8	endpoint;
	u8	gpdCmdId;
	u8	gpdAsduLen;
	u8	frameType;
}gp_data_ind_t;

/***************************************************************************
* @brief	GP-DATA.request txOptions
*/
typedef struct
{
	u8	useGpTxQueue:1;
	u8	useCSMACA:1;
	u8	useMACACK:1;
	u8	txFrameType:2;
	u8	txOnMatchingEndpoint:1;
	u8	reserved:2;
}gpDataReq_txOpt_t;

/***************************************************************************
* @brief	GP-DATA.request
*/
typedef struct
{
	u8	*gpdAsdu;
	u24	gpTxQueueEntryLifetime;
	gpdId_t gpdId;
	gpDataReq_txOpt_t	txOptions;
	bool action;
	u8	appId;
	u8	endpoint;
	u8	gpdCmdId;
	u8	gpdAsduLen;
	u8	gpepHandle;
}gp_data_req_t;

typedef enum
{
	GP_DATA_CNF_STATUS_TX_QUEUE_FULL,
	GP_DATA_CNF_STATUS_ENTRY_REPLACED,
	GP_DATA_CNF_STATUS_ENTRY_ADDED,
	GP_DATA_CNF_STATUS_ENTRY_EXPIRED,
	GP_DATA_CNF_STATUS_ENTRY_REMOVED,
	GP_DATA_CNF_STATUS_GPDF_SENDING_FINALIZED
}gpDataCnf_status_t;

/***************************************************************************
* @brief	GP-DATA.confirm
*/
typedef struct
{
	u8	status;
	u8	gpepHandle;
}gp_data_cnf_t;

/***************************************************************************
* @brief	GP-SEC.request
*/
typedef struct
{
	gpdId_t gpdId;
	u32	gpdSecFrameCnt;
	u8	appId;
	u8	endpoint;
	u8	gpdfSecurityLevel;//0x00,0x02,0x03
	u8	gpdfKeyType;//0x00,0x01
	u8	dgpStubHandle;
}gp_sec_req_t;

typedef enum
{
	GP_SEC_RSP_STATUS_DROP_FRAME,
	GP_SEC_RSP_STATUS_MATCH,
	GP_SEC_RSP_STATUS_PASS_UNPROCESSED,
	GP_SEC_RSP_STATUS_TX_THEN_DROP,
	GP_SEC_RSP_ERROR
}gpSecRsp_status_t;

/***************************************************************************
* @brief	GP-SEC.response
*/
typedef struct
{
	gpdId_t gpdId;
	u32	gpdSecFrameCnt;
	u8	status;
	u8	dgpStubHandle;
	u8	appId;
	u8	endpoint;
	u8	gpdKey[16];
	u8	gpdfSecurityLevel;
	u8	gpdfKeyType;//0x00 - 0x07
}gp_sec_rsp_t;

/***************************************************************************
* @brief	GP application payload for appId 0b000 and 0b010
*/
typedef struct
{
	u8	*gpdCmdPayload;
	u8	gpdCmdId;
}gp_appPayload_t;

/***************************************************************************
* @brief	GP data request pending
*/
typedef struct
{
	gp_data_req_t *buf; //hold gpDataReq buffer
	u8  gpepHandle;
	u8	used;
}gp_data_req_pending_t;

/***************************************************************************
* @brief	GP data indication buffer pending, waiting for security process
*/
typedef struct
{
	gp_data_ind_t *buf;	//hold gpDataInd buffer
	u8	timeout;
	u8	dGpStubHandle;
	u8	used;
}gp_data_ind_entry_t;


typedef u8 (*gpDeviceAnnounceCheckCb_t)(u16 sinkNwkAddr, addrExt_t sinkIeeeAddr);

/***************************************************************************
* @brief	Define for dStub notify GPEP
*/
typedef void (*gpDataCnfCb_t)(gp_data_cnf_t gpDataCnf);
typedef void (*gpDataIndCb_t)(void *arg);
typedef void (*gpSecReqCb_t)(void *arg);

typedef struct
{
	gpDataCnfCb_t 	gpDataCnfCb;
	gpDataIndCb_t 	gpDataIndCb;
	gpSecReqCb_t	gpSecReqCb;
}gp_epCb_t;



extern gp_data_ind_entry_t g_gpDataIndSecReqTab[];


void gpStubCbInit(gp_epCb_t *cb);
void gpTxQueueFree(void);
void gpDataIndSecReqEntryClear(gp_data_ind_entry_t *pEntry);
u8 dGpStubHandleGet(void);
gp_data_ind_entry_t *gpDataIndGet(u8 handle);

void gpDataReq(void *arg);
void gpSecRsp(void *arg);
void dGp_dataInd(void *arg);
void cGpDataCnfHandler(void *arg);

#endif	/* DGP_STUB_H */
