/********************************************************************************************************
 * @file     zb_afTestCb.c
 *
 * @brief    call back function for AF
 *
 * @author
 * @date     Dec. 5, 2016
 *
 * @par      Copyright (c) 2016, Telink Semiconductor (Shanghai) Co., Ltd.
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
