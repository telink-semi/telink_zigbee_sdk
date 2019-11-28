/********************************************************************************************************
 * @file     cGP_stub.h
 *
 * @brief    header file for common Green Power
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
#ifndef CGP_STUB_H
#define CGP_STUB_H

#include "tl_common.h"


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
