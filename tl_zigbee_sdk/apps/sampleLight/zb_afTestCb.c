/********************************************************************************************************
 * @file	zb_afTestCb.c
 *
 * @brief	This is the source file for zb_afTestCb
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
#if (__PROJECT_TL_DIMMABLE_LIGHT__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleLight.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif

#if AF_TEST_ENABLE
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
u16 g_afTest_rcvReqCnt = 0;

/**********************************************************************
 * FUNCTIONS
 */
static void afTest_testReqPrc(apsdeDataInd_t *pApsdeInd)
{
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pApsdeInd->indInfo.src_ep;
	dstEp.profileId = pApsdeInd->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;

	u8 dataLen = 50;
	u8 *pBuf = (u8 *)ev_buf_allocate(dataLen);
	if(pBuf){
		u8 *pData = pBuf;

		*pData++ = LO_UINT16(g_afTest_rcvReqCnt);
		*pData++ = HI_UINT16(g_afTest_rcvReqCnt);

		for(u8 i = 0; i < dataLen - 2; i++){
			*pData++ = i;
		}

		u8 apsCnt = 0;
#if ZBHCI_EN
		zbhciTx(ZCL_CLUSTER_TELINK_SDK_TEST_RSP, pApsdeInd->asduLen, (u8 *)pApsdeInd->asdu);
#else
		af_dataSend(pApsdeInd->indInfo.dst_ep, &dstEp, ZCL_CLUSTER_TELINK_SDK_TEST_RSP, dataLen, pBuf, &apsCnt);
#endif

		ev_buf_free(pBuf);
	}
}

static void afTest_testClearReqPrc(apsdeDataInd_t *pApsdeInd)
{
	epInfo_t dstEp;
	TL_SETSTRUCTCONTENT(dstEp, 0);

	dstEp.dstEp = pApsdeInd->indInfo.src_ep;
	dstEp.profileId = pApsdeInd->indInfo.profile_id;
	dstEp.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
	dstEp.dstAddr.shortAddr = pApsdeInd->indInfo.src_short_addr;

	u8 st = SUCCESS;

	u8 apsCnt = 0;
	af_dataSend(pApsdeInd->indInfo.dst_ep, &dstEp, ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_RSP, 1, &st, &apsCnt);
}

void afTest_rx_handler(void *arg)
{
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)arg;

	switch(pApsdeInd->indInfo.cluster_id){
		case ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_REQ:
			g_afTest_rcvReqCnt = 0;
			afTest_testClearReqPrc(pApsdeInd);
			break;
		case ZCL_CLUSTER_TELINK_SDK_TEST_REQ:
			g_afTest_rcvReqCnt++;
			afTest_testReqPrc(pApsdeInd);
			break;
		case ZCL_CLUSTER_TELINK_SDK_TEST_RSP:

			break;
		default:
			break;
	}

	/* Must be free here. */
	ev_buf_free((u8 *)arg);
}

void afTest_dataSendConfirm(void *arg)
{
//	apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

}

#endif	/* AF_TEST_ENABLE */
#endif  /* __PROJECT_TL_DIMMABLE_LIGHT__ */
