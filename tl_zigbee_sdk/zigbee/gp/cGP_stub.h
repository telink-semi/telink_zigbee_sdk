/********************************************************************************************************
 * @file    cGP_stub.h
 *
 * @brief   This is the header file for cGP_stub
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

#ifndef CGP_STUB_H
#define CGP_STUB_H



//for application id
#define GP_APP_ID_SRC_ID			0x00
#define GP_APP_ID_LPED				0x01
#define GP_APP_ID_GPD				0x02

//for GPD srcID field
#define GPD_SRC_ID_UNSPECIFIED		0x00000000
#define GPD_SRC_ID_RESERVED_L		0xFFFFFFF9
#define GPD_SRC_ID_RESERVED_H		0xFFFFFFFE
#define GPD_SRC_ID_ALL				0xFFFFFFFF

//for endPoint field
#define GP_ENDPOINT_RESERVED_L		0xF1
#define GP_ENDPOINT_RESERVED_H		0xFE
#define GP_ENDPOINT_ALL				0xFF

//GPD command range 0xf0 - 0xff is reserved
#define GPD_CMD_ID_IS_INVALID(cmdId)	(((cmdId) & 0xF0) == 0xF0)

/***************************************************************************
* @brief	GPDF NWK frame type
*/
enum
{
	GP_NWK_FRAME_TYPE_DATA			= 0,
	GP_NWK_FRAME_TYPE_MAINTENANCE	= 1
};

/***************************************************************************
* @brief	Values of securityLevel
*/
enum
{
	GP_SEC_LEVEL_NO_SECURITY,
	GP_SEC_LEVEL_RESERVED,
	GP_SEC_LEVEL_4BFC_4BMIC,
	GP_SEC_LEVEL_4BFC_4BMIC_ENCRYPTION
};

/***************************************************************************
* @brief	Values of securityKey
*/
enum
{
	GP_SEC_SHARED_KEY,
	GP_SEC_INDIVIDUAL_KEY,
};

/***************************************************************************
* @brief	NWK frame control field of GPDF
*/
typedef struct
{
	u8	frameType:2;
	u8	protocolVer:4;
	u8	autoCommissioning:1;
	u8	nwkFrameCtrlExtension:1;
}gp_nwkHdrFrameCtrl_t;

/***************************************************************************
* @brief	Extended NWK frame control field of GPDF
*/
typedef struct
{
	u8	appId:3;
	u8	securityLevel:2;
	u8	securityKey:1;
	u8	rxAfterTx:1;
	u8	direction:1;
}gp_extNwkFrameCtrl_t;

/***************************************************************************
* @brief	GP stub NWK header
*/
typedef struct
{
	u32	gpdSrcId;
	u32	secFrameCnt;
	gp_nwkHdrFrameCtrl_t	nwkFrameCtrl;
	gp_extNwkFrameCtrl_t	extNwkFrameCtrl;
	u8	endpoint;
}gp_nwkHdr_t;

/************************************************************************************************
 ******************************** cGP stub service specification ********************************
 ***********************************************************************************************/

/***************************************************************************
* @brief	CGP-DATA.request txOptions
*/
typedef struct
{
	u8	useCSMACA:1;
	u8	useMACACK:1;
	u8	reserved:6;
}cgpDataReq_txOpt_t;

/***************************************************************************
* @brief	CGP-DATA.request
*/
typedef struct
{
	u8 	*gpMpdu;
	u16	srcPanId;
	u16	dstPanId;
	tl_zb_addr_t srcAddr;
	tl_zb_addr_t dstAddr;
	u8	srcAddrMode;
	u8	dstAddrMode;
	u8	gpMpduLen;
	u8	gpMpduHandle;
	cgpDataReq_txOpt_t	txOptions;
}cgp_data_req_t;

/***************************************************************************
* @brief	CGP-DATA.confirm
*/
typedef struct
{
	u8	status;
	u8	gpMpduHandle;
}cgp_data_cnf_t;

s32 cGp_dataReq(void *arg);
void cGp_dataCnf(void *arg);
void cGp_mcpsDataInd(void *arg);

#endif	/* CGP_STUB_H */
