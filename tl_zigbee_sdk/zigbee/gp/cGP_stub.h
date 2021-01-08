/********************************************************************************************************
 * @file	cGP_stub.h
 *
 * @brief	This is the header file for cGP_stub
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
