/********************************************************************************************************
 * @file     zbhciCmdProcess.c
 *
 * @brief	 process all the commands from HCI
 *
 * @author
 * @date     June. 12, 2017
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

/**********************************************************************
 * INCLUDES
 */
#include "../zcl/zcl_include.h"
#include "../ota/ota.h"
#include "zbhci.h"


#if ZBHCI_EN
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	u16			targetAddr;
	addrExt_t	macAddr;
	u8			apsCnt;
	u8			rejoin;
}zb_hciCmdInfo_t;

/**********************************************************************
 * LOCAL VARIABLES
 */
zb_hciCmdInfo_t  g_hciCmd;

zbhci_afTestReq_t g_afTestReq;

/**********************************************************************
 * GLOBAL VARIABLES
 */
workingMode_e zbhciWorkingMode = ZBHCI_NORMAL_MODE;

/**********************************************************************
 * EXTERNED FUNCTIONS
 */
extern void zbhci_zclIdentifyCmdHandle(void *arg);
extern void zbhci_zclOnoffCmdHandle(void *arg);
extern void zbhci_zclLevelCtrlCmdHandle(void *arg);
extern void zbhci_zclColorCtrlCmdHandle(void *arg);
extern void zbhci_clusterGroupHandle(void *arg);
extern void zbhci_clusterSceneHandle(void *arg);
extern void zbhci_clusterOTAHandle(void *arg);
extern void zbhci_clusterBasicHandle(void *arg);
extern void zbhci_clusterCommonCmdHandle(void *arg);
extern void zbhci_clusterOTAHandle(void *arg);


/**********************************************************************
 * FUNCTIONS
 */
static u8 zbhciResolveAddrRspMsg(zdo_ieee_addr_resp_t *rsp){
	u8 len = OFFSETOF(zdo_ieee_addr_resp_t, num_assoc_dev);
	ZB_LEBESWAP(rsp->ieee_addr_remote,EXT_ADDR_LEN);
	ZB_LEBESWAP(((u8 *)&rsp->nwk_addr_remote), 2);
	for(u8 i=0;(i<rsp->num_assoc_dev)&&(i<MAX_RSPNUM);i++){
		if(i==0){
			len++;
		}
		ZB_LEBESWAPU16(rsp->nwk_addr_assoc_dev_lst[i]);
		len += 2;
	}
	return len;
}

static void zbhciNwkAddrRspMsgPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_nwk_addr_resp_t *rsp = (zdo_nwk_addr_resp_t *)p->zpdu;

	if(p->length == OFFSETOF(zdo_nwk_addr_resp_t, num_assoc_dev)){
		rsp->num_assoc_dev = 0;
	}
	zbhciResolveAddrRspMsg(rsp);
	zbhciTx(ZBHCI_CMD_DISCOVERY_NWK_ADDR_RSP, p->length, (u8 *)rsp);
}

static void zbhciIeeeAddrRspMsgPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_ieee_addr_resp_t *rsp = (zdo_ieee_addr_resp_t*)p->zpdu;

	if(p->length == OFFSETOF(zdo_nwk_addr_resp_t, num_assoc_dev)){
		rsp->num_assoc_dev = 0;
	}
	zbhciResolveAddrRspMsg(rsp);
	zbhciTx(ZBHCI_CMD_DISCOVERY_IEEE_ADDR_RSP, p->length, (u8 *)rsp);
}

static void zbhciMatchDescRspPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_match_descriptor_resp_t *rsp = (zdo_match_descriptor_resp_t *)p->zpdu;
	ZB_LEBESWAP(((u8 *)&rsp->nwk_addr_interest), 2);
	zbhciTx(ZBHCI_CMD_DISCOVERY_MATCH_DESC_RSP, p->length, (u8 *)rsp);
}

static void zbhciActiveEpRspMsgPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_active_ep_resp_t *rsp = (zdo_active_ep_resp_t *)p->zpdu;
	ZB_LEBESWAPU16(rsp->nwk_addr_interest);
	zbhciTx(ZBHCI_CMD_DISCOVERY_ACTIVE_EP_RSP, p->length, (u8 *)rsp);
}

static void zbhciNodeDescRspMsgPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_node_descript_resp_t *rsp = (zdo_node_descript_resp_t*)p->zpdu;
	ZB_LEBESWAP(((u8 *)&rsp->nwk_addr_interest), 2);

	u8 mcL = rsp->node_descriptor.mcL8;
	rsp->node_descriptor.mcL8 = rsp->node_descriptor.mcH8;
	rsp->node_descriptor.mcH8 = mcL;

	ZB_LEBESWAP(((u8 *)&(rsp->node_descriptor.max_in_tr_size)), 2);
	ZB_LEBESWAP(((u8 *)&(rsp->node_descriptor.max_out_tr_size)), 2);

	zbhciTx(ZBHCI_CMD_DISCOVERY_NODE_DESC_RSP, p->length, (u8 *)rsp);
}

static void zbhciSimpleDescRspMsgPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_simple_descriptor_resp_t *rsp = (zdo_simple_descriptor_resp_t *)p->zpdu;

	ZB_LEBESWAP(((u8 *)&(rsp->nwk_addr_interest)), 2);

	if(rsp->status == ZDO_SUCCESS && rsp->length){
		ZB_LEBESWAP(((u8 *)&(rsp->simple_descriptor.app_profile_id)), 2);
		ZB_LEBESWAP(((u8 *)&(rsp->simple_descriptor.app_dev_id)), 2);
		u8 *listInfo = rsp->simple_descriptor.listInfo;
		u8 inClusterNum = *listInfo++;  			//	in-cluster count
		for(u8 i = 0; i < inClusterNum; i++){		//	in-cluster list
			ZB_LEBESWAP(listInfo, 2);
			listInfo += 2;
		}

		u8 outClusterNum = *listInfo++; 			//	out-cluster count
		for(u8 i = 0; i < outClusterNum; i++){		//	out-cluster list
			ZB_LEBESWAP(listInfo, 2);
			listInfo += 2;
		}
	}

	zbhciTx(ZBHCI_CMD_DISCOVERY_SIMPLE_DESC_RSP, p->length, (u8 *)rsp);
}

static void zbhciBindRspPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_bind_resp_t *rsp = (zdo_bind_resp_t*)p->zpdu;
	zbhciTx(ZBHCI_CMD_BINDING_RSP, ZDO_ZDP_RSP_FRAME_HEADERSIZE, (u8 *)rsp);
}

static void zbhciUnbindRspPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_unbind_resp_t *rsp = (zdo_unbind_resp_t*)p->zpdu;
	zbhciTx(ZBHCI_CMD_UNBINDING_RSP, ZDO_ZDP_RSP_FRAME_HEADERSIZE, (u8 *)rsp);
}

static void zbhciMgmtLqiRspMsgPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_mgmt_lqi_resp_t *rsp = (zdo_mgmt_lqi_resp_t *)p->zpdu;
	u8 num = rsp->neighbor_tbl_lst_cnt;
	for(s32 i = 0; i < num; i++){
		u8 *addr = rsp->neighbor_tbl_lst[i].ext_addr;
		ZB_LEBESWAP(addr, 8);

		addr = rsp->neighbor_tbl_lst[i].ext_pan_id;
		ZB_LEBESWAP(addr, 8);

		addr = (u8 *)(&rsp->neighbor_tbl_lst[i].network_addr);
		ZB_LEBESWAP(addr, 2);
	}
	zbhciTx(ZBHCI_CMD_MGMT_LQI_RSP, p->length, (u8 *)rsp);
}

static void zbhciMgmtBindRspMsgPush(void* arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_mgmt_bind_resp_t *rsp = (zdo_mgmt_bind_resp_t *)p->zpdu;
	u8 num = rsp->bind_tbl_lst_cnt;

	for(s32 n = 0; n < num; n++){
		ZB_LEBESWAP((rsp->bind_tbl_lst[n].src_addr), 8);
		u8 cidH = rsp->bind_tbl_lst[n].cid16_h;
		u8 cidL = rsp->bind_tbl_lst[n].cid16_l;
		rsp->bind_tbl_lst[n].cid16_l = cidH;
		rsp->bind_tbl_lst[n].cid16_h = cidL;

		if(rsp->bind_tbl_lst[n].dst_addr_mode == 0x01){
			ZB_LEBESWAP(((u8 *)&rsp->bind_tbl_lst[n].dst_group_addr), 2);
		}else{
			ZB_LEBESWAP((rsp->bind_tbl_lst[n].dst_ext_addr), 8);
		}
	}
	zbhciTx(ZBHCI_CMD_MGMT_BIND_RSP, p->length, (u8 *)rsp);
}

static void zbhciMgmtLeaveRspMsgPush(void *arg){
	zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)arg;
	zdo_mgmt_leave_resp_t *rsp = (zdo_mgmt_leave_resp_t *)p->zpdu;
	u16 leaveNodeCnt = 0;
	zbhci_nodeLeaveInd_t ind;
	if(g_hciCmd.apsCnt == rsp->seq_num && rsp->status == 0){
		if(!g_hciCmd.rejoin){
			zdo_nlmeForgetDev(g_hciCmd.macAddr, g_hciCmd.rejoin);
		}
		ind.totalCnt = leaveNodeCnt++;
		memcpy(ind.macAddr, g_hciCmd.macAddr, 8);
		zbhciTx(ZBHCI_CMD_MGMT_LEAVE_RSP, sizeof(zbhci_nodeLeaveInd_t), (u8 *)&ind);
	}
}

s32 rxtx_performance_result_start(void *arg){
	txrx_performce_test_rsp_t rsp;

	COPY_BUFFERTOU16_BE(rsp.dstAddr, (u8 *)&g_afTestReq.dstAddr);
	COPY_BUFFERTOU16_BE(rsp.sendCnt, (u8 *)&g_afTestReq.sendSuccessCnt);
	COPY_BUFFERTOU16_BE(rsp.ackCnt, (u8 *)&g_afTestReq.rcvTotalCnt);

	zbhciTx(ZBHCI_CMD_TXRX_PERFORMANCE_TEST_RSP, sizeof(txrx_performce_test_rsp_t), (u8 *)&rsp);

	memset((u8 *)&g_afTestReq, 0, sizeof(zbhci_afTestReq_t));

	g_afTestReq.performaceTestTmrEvt = NULL;
	return -1;
}

void zbhciAfDataPerformanceResultPush(void){
#if AF_TEST_ENABLE
	if(!g_afTestReq.performaceTest){
		return;
	}

	g_afTestReq.rcvTotalCnt++;

	if(g_afTestReq.performaceTestTmrEvt){
		TL_ZB_TIMER_CANCEL(&g_afTestReq.performaceTestTmrEvt);
	}
	g_afTestReq.performaceTestTmrEvt = TL_ZB_TIMER_SCHEDULE(rxtx_performance_result_start, NULL, 1 * 1000 * 1000);
#endif
}

void zbhciAfDataRcvIndPush(void *arg){
#if AF_TEST_ENABLE
	apsdeDataInd_t *pApsdeInd = (apsdeDataInd_t *)arg;

	if(g_afTestReq.performaceTest){
		return;
	}

	zbhci_afDataSend_rsp_t *pRsp = (zbhci_afDataSend_rsp_t *)ev_buf_allocate(sizeof(zbhci_afDataSend_rsp_t) + pApsdeInd->asduLen);
	if(pRsp){
		COPY_BUFFERTOU16_BE(pRsp->clusterId, (u8 *)&(pApsdeInd->indInfo.cluster_id));
		pRsp->srcAddr = pApsdeInd->indInfo.src_short_addr;
		pRsp->srcEp = pApsdeInd->indInfo.src_ep;
		pRsp->dstEp = pApsdeInd->indInfo.dst_ep;
		pRsp->dataLen = pApsdeInd->asduLen;
		if(pApsdeInd->asduLen){
			memcpy(pRsp->payload, pApsdeInd->asdu, pApsdeInd->asduLen);
		}

		zbhciTx(ZBHCI_CMD_AF_DATA_SEND_TEST_RSP, sizeof(zbhci_afDataSend_rsp_t) + pApsdeInd->asduLen, (u8 *)pRsp);

		ev_buf_free((u8 *)pRsp);
	}
#endif
}

void zbhciAfDataCnfPush(void *arg){
#if AF_TEST_ENABLE
	apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

	if( (pApsDataCnf->dstEndpoint == g_afTestReq.dstEp) &&
		(pApsDataCnf->dstAddr.addr_short == g_afTestReq.dstAddr) &&
		(pApsDataCnf->status == APS_STATUS_SUCCESS) ){
		g_afTestReq.sendSuccessCnt++;
	}
#endif
}

void zbhciAppDataSendConfirmPush(void *arg){
	apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

	zbhci_app_data_confirm_t *conf = (zbhci_app_data_confirm_t *)ev_buf_allocate(sizeof(zbhci_app_data_confirm_t));
	if(conf){
		conf->ep = pApsDataCnf->srcEndpoint;
		conf->status = pApsDataCnf->status;
		conf->apsCnt = pApsDataCnf->apsCnt;

		zbhciTx(ZBHCI_CMD_DATA_CONFIRM, sizeof(zbhci_app_data_confirm_t), (u8 *)conf);

		ev_buf_free((u8 *)conf);
	}
}

bool zbhciMacAddrGetPush(addrExt_t devExtAddr){
	if(zbhciWorkingMode == ZBHCI_GET_MAC_ADDR_MODE){
		zbhciTx(ZBHCI_CMD_MAC_ADDR_IND, EXT_ADDR_LEN, (u8 *)devExtAddr);
		return FALSE;
	}
	return TRUE;
}

void zbhciAppNodeLeaveIndPush(void *arg){
	zbhci_nodeLeaveInd_t *ind = (zbhci_nodeLeaveInd_t *)arg;
	ZB_LEBESWAP((ind->macAddr), 8);
	zbhciTx(ZBHCI_CMD_NODE_LEAVE_IND, sizeof(zbhci_nodeLeaveInd_t), (u8 *)ind);
}

static void match_desc_req_handler(void *arg){
	u8 *ptr = arg;

	u16 targetAddr;
	COPY_BUFFERTOU16_BE(targetAddr, ptr);
	ptr += 2;

	u8 inNum = ptr[4];
	u8 outNum = ptr[5];

	zdo_match_descriptor_req_t *req = (zdo_match_descriptor_req_t*)ev_buf_allocate(6+(inNum+outNum)*2);
	if(req){
		u8 i = 0;
		COPY_BUFFERTOU16_BE(req->nwk_addr_interest, ptr);
		ptr += 2;
		COPY_BUFFERTOU16_BE(req->profile_id,ptr);
		ptr += 2;
		req->num_in_clusters  = inNum;  //*ptr++;
		req->num_out_clusters  = outNum; //*ptr++;
		ptr += 2;
		while (i < req->num_in_clusters){
			COPY_BUFFERTOU16_BE(req->cluster_list[i], ptr);
			ptr += 2;
			i++;
		}

		i = 0;
		while (i < req->num_out_clusters){
			COPY_BUFFERTOU16_BE(req->cluster_list[i+req->num_in_clusters], ptr);
			ptr += 2;
			i++;
		}

		u8 sn = 0;
		zb_zdoMatchDescReq(targetAddr, req, &sn, zbhciMatchDescRspPush);
		ev_buf_free((u8*)req);
	}
}

static void zbhci_bdbCmdHandler(void *arg){
	zbhci_cmdHandler_t *cmdInfo = arg;
	u16 cmdID = cmdInfo->cmdId;
	u8 *p = cmdInfo->payload;

	if(cmdID == ZBHCI_CMD_BDB_COMMISSION_FORMATION){
		bdb_networkFormationStart();
	}else if(cmdID == ZBHCI_CMD_BDB_COMMISSION_STEER){
		bdb_networkSteerStart();
	}else if(cmdID == ZBHCI_CMD_BDB_COMMISSION_TOUCHLINK){
		bdb_networkTouchLinkStart(p[0]);
	}else if(cmdID == ZBHCI_CMD_BDB_COMMISSION_FINDBIND){
		if(g_bdbAttrs.nodeIsOnANetwork){
			bdb_findAndBindStart(p[0]);
		}
	}else if(cmdID == ZBHCI_CMD_BDB_FACTORY_RESET){
		zb_resetDevice2FN();
	}else if(cmdID == ZBHCI_CMD_BDB_PRE_INSTALL_CODE){
		ss_dev_pair_set_t keyPair;

		ZB_IEEE_ADDR_REVERT(keyPair.device_address, &p[0]);
		ZB_LINK_KEY_REVERT(keyPair.linkKey, &p[8]);

		keyPair.incomingFrmaeCounter = keyPair.outgoingFrameCounter = 0;
		keyPair.apsLinkKeyType = SS_UNIQUE_LINK_KEY;
		keyPair.keyAttr = SS_UNVERIFIED_KEY;
		ss_devKeyPairSave(&keyPair);
	}else if(cmdID == ZBHCI_CMD_BDB_CHANNEL_SET){
		zb_apsChannelMaskSet((1 << p[0]));
	}else if(cmdID == ZBHCI_CMD_BDB_DONGLE_WORKING_MODE_SET){
		zbhciWorkingMode = p[0];
	}else if(cmdID == ZBHCI_CMD_BDB_NODE_DELETE){
		zbhci_mgmt_nodeDeleteReq_t delNodeAddr;
		ZB_IEEE_ADDR_REVERT(delNodeAddr.macAddr, &p[0]);

		zdo_nlmeForgetDev(delNodeAddr.macAddr, 0);
	}else if(cmdID == ZBHCI_CMD_BDB_TX_POWER_SET){
		/* Set TX power, value is RF_PowerTypeDef. */
		rf_setTxPower(p[0]);
	}

	ev_buf_free(arg);
}


static void zbhci_discoveryCmdHandler(void *arg){
	zbhci_cmdHandler_t *cmdInfo = arg;
	u16 cmdID = cmdInfo->cmdId;
	u8 *p = cmdInfo->payload;

	if(cmdID == ZBHCI_CMD_DISCOVERY_NWK_ADDR_REQ){
		u8 sn = 0;
		zdo_nwk_addr_req_t req;
		u16 targetAddr;
		COPY_BUFFERTOU16_BE(targetAddr, p);
		ZB_IEEE_ADDR_REVERT(req.ieee_addr_interest, p+2);
		req.req_type = p[10];
		req.start_index = p[11];
		zb_zdoNwkAddrReq(targetAddr, &req, &sn, zbhciNwkAddrRspMsgPush);
	}else if(cmdID == ZBHCI_CMD_DISCOVERY_IEEE_ADDR_REQ){
		u8 sn = 0;
		zdo_ieee_addr_req_t req;
		u16 targetAddr;
		COPY_BUFFERTOU16_BE(targetAddr, p);
		COPY_BUFFERTOU16_BE(req.nwk_addr_interest, p+2);
		req.req_type = p[4];
		req.start_index = p[5];
		zb_zdoIeeeAddrReq(targetAddr, &req, &sn, zbhciIeeeAddrRspMsgPush);
	}else if(cmdID == ZBHCI_CMD_DISCOVERY_NODE_DESC_REQ){
		zdo_node_descriptor_req_t req;
		u16 targetAddr;
		u8 sn = 0;
		COPY_BUFFERTOU16_BE(targetAddr, p);
		COPY_BUFFERTOU16_BE(req.nwk_addr_interest, p+2);
		zb_zdoNodeDescReq(targetAddr, &req, &sn, zbhciNodeDescRspMsgPush);
	}else if(cmdID == ZBHCI_CMD_DISCOVERY_SIMPLE_DESC_REQ){
		zdo_simple_descriptor_req_t req;
		u16 targetAddr;
		u8 sn = 0;
		COPY_BUFFERTOU16_BE(targetAddr, p);
		COPY_BUFFERTOU16_BE(req.nwk_addr_interest, p+2);
		req.endpoint = p[4];
		zb_zdoSimpleDescReq(targetAddr, &req, &sn, zbhciSimpleDescRspMsgPush);
	}else if(cmdID == ZBHCI_CMD_DISCOVERY_MATCH_DESC_REQ){
		match_desc_req_handler(p);
	}else if(cmdID == ZBHCI_CMD_DISCOVERY_ACTIVE_EP_REQ){
		u8 sn = 0;
		zdo_active_ep_req_t req;
		u16 targetAddr;
		COPY_BUFFERTOU16_BE(targetAddr, p);
		COPY_BUFFERTOU16_BE(req.nwk_addr_interest, p+2);
		zb_zdoActiveEpReq(targetAddr, &req, &sn, zbhciActiveEpRspMsgPush);
	}else if(cmdID == ZBHCI_CMD_DISCOVERY_LEAVE_REQ){
#if 0
		nlme_leave_req_t req;
		ZB_IEEE_ADDR_REVERT(req.deviceAddr, p);
		req.rejoin = p[8];
		req.removeChildren = p[9];
		zb_nlmeLeaveReq(&req);
#endif
		u8 sn;
		u16 targetAddr = 0xFFFE;

		zdo_mgmt_leave_req_t req;
		ZB_IEEE_ADDR_REVERT(req.device_addr, p);

		if(zb_getNwkAddrByExtAddr(req.device_addr, &targetAddr) == SUCCESS){
			req.lr_bitfields.rejoin = p[8];
			req.lr_bitfields.remove_children = p[9];
			req.lr_bitfields.reserved = 0;

			memcpy(g_hciCmd.macAddr, req.device_addr, 8);
			g_hciCmd.rejoin = req.lr_bitfields.rejoin;
			zb_mgmtLeaveReq(targetAddr, &req, &sn, zbhciMgmtLeaveRspMsgPush);
			g_hciCmd.apsCnt = sn;
		}
	}

	ev_buf_free(arg);
}


static void zbhci_mgmtCmdHandler(void *arg){
	zbhci_cmdHandler_t *cmdInfo = arg;
	u16 cmdID = cmdInfo->cmdId;
	u8 *p = cmdInfo->payload;
	u16 targetAddr;

	if(cmdID == ZBHCI_CMD_MGMT_LQI_REQ){
		u8 sn = 0;
		zdo_mgmt_lqi_req_t req;
		u16 dstAddr;
		COPY_BUFFERTOU16_BE(dstAddr, p);
		req.start_index = p[2];
		zb_mgmtLqiReq(dstAddr, &req, &sn, zbhciMgmtLqiRspMsgPush);
	}else if(cmdID == ZBHCI_CMD_MGMT_BIND_REQ){
		u8 sn = 0;
		zdo_mgmt_bind_req_t req;
		u16 dstAddr = (p[0] << 8) | p[1];
		req.start_index = p[2];
		zb_mgmtBindReq(dstAddr, &req, &sn, zbhciMgmtBindRspMsgPush);
	}else if(cmdID == ZBHCI_CMD_MGMT_LEAVE_REQ){
		u8 sn;
		COPY_BUFFERTOU16_BE(targetAddr,p);

		zdo_mgmt_leave_req_t req;
		ZB_IEEE_ADDR_REVERT(req.device_addr, p+2);
		req.lr_bitfields.rejoin = p[10];
		req.lr_bitfields.remove_children = p[11];
		req.lr_bitfields.reserved = 0;
		zb_mgmtLeaveReq(targetAddr, &req, &sn, zbhciMgmtLeaveRspMsgPush);
		g_hciCmd.apsCnt = sn;
		memcpy(g_hciCmd.macAddr, req.device_addr, 8);
	}else if(cmdID == ZBHCI_CMD_MGMT_DIRECT_JOIN_REQ){

	}else if(cmdID == ZBHCI_CMD_MGMT_PERMIT_JOIN_REQ){
		u8 sn = 0;
		COPY_BUFFERTOU16_BE(targetAddr, p);
		zb_mgmtPermitJoinReqTx(targetAddr,p[2],p[3],&sn,NULL);
	}else if(cmdID == ZBHCI_CMD_MGMT_NWK_UPDATE_REQ){
		u8 sn = 0;
		u16 dstAddr;
		zdo_mgmt_nwk_update_req_t req;
		COPY_BUFFERTOU16_BE(dstAddr, p);
		COPY_BUFFERTOU16_BE(req.nwk_manager_addr, &p[2]);
		COPY_BUFFERTOU32_BE(req.scan_ch, &p[4]);
		req.scan_duration = p[8];
		req.scan_cnt = p[9];
		zb_mgmtNwkUpdateReq(dstAddr, &req, &sn);
	}

	ev_buf_free(arg);
}

static void zbhci_bindCmdHandler(void *arg){
	zbhci_cmdHandler_t *cmdInfo = arg;
	u16 cmdID = cmdInfo->cmdId;
	u8 *p = cmdInfo->payload;

	zdo_bind_req_t req;
	u8 *ptr = p;

	ZB_IEEE_ADDR_REVERT(req.src_addr,ptr);
	ptr += EXT_ADDR_LEN;
	req.src_endpoint = *ptr++;

	req.cid16_h = *ptr++;
	req.cid16_l = *ptr++;
	req.dst_addr_mode = *ptr++;

	if(req.dst_addr_mode == 0x3)
	{
		ZB_IEEE_ADDR_REVERT(req.dst_ext_addr,ptr);
		ptr += EXT_ADDR_LEN;
		req.dst_endpoint =  *ptr++;
	}else{
		COPY_BUFFERTOU16_BE(req.dst_group_addr,ptr);
		ptr += 2;
	}

	bool bindOrUnbind = (cmdID == ZBHCI_CMD_BINDING_REQ) ? 1 : 0;
	u8 sn = 0;
	if(bindOrUnbind){
		zb_zdoBindUnbindReq(bindOrUnbind, &req, &sn, zbhciBindRspPush);
	}else{
		zb_zdoBindUnbindReq(bindOrUnbind, &req, &sn, zbhciUnbindRspPush);
	}

	ev_buf_free(arg);
}

ev_time_event_t *hci_nodeToggleTestEvt = NULL;
s32 node_toggle_test(void *arg){
	//u32 onOff = (u32)arg;
	static s32 startIdx = 0;
	static u32 onOff = 0;
	itemIfno_t itemInfo;
	ss_dev_pair_set_t keyPair;
	addrExt_t extAddr;

	TL_SETSTRUCTCONTENT(itemInfo,0);
	nv_flashReadNew(0, NV_MODULE_KEYPAIR, ITEM_FIELD_IDLE, sizeof(ss_dev_pair_set_t), (u8 *)&itemInfo);

	s32 i = 0;
	for(i = startIdx; i <= itemInfo.opIndex; i++){
		if(NV_SUCC == nv_flashReadByIndex(NV_MODULE_KEYPAIR, NV_ITEM_SS_KEY_PAIR, itemInfo.opSect, i, sizeof(ss_dev_pair_set_t), (u8 *)&keyPair)){
			ZB_IEEE_ADDR_COPY(extAddr, keyPair.device_address);
			break;
		}
	}
	startIdx = i+1;

	u8 srcEp = 1;
	epInfo_t dstEpInfo;
	memset(&dstEpInfo, 0, sizeof(epInfo_t));
	dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
	ZB_IEEE_ADDR_COPY(dstEpInfo.dstAddr.extAddr,extAddr);
	dstEpInfo.dstEp = 1;
	dstEpInfo.profileId = HA_PROFILE_ID;
	//dstEpInfo.txOptions |= APS_TX_OPT_ACK_TX;

	if(onOff){
		zcl_onOff_onCmd(srcEp, &dstEpInfo, 0);
	}else{
		zcl_onOff_offCmd(srcEp, &dstEpInfo, 0);
	}

	if(i >= itemInfo.opIndex){
		startIdx = 0;
		zbhciTx(ZBHCI_CMD_NODES_TOGLE_TEST_RSP, 0, NULL);
		onOff ^= 1;
		return 0;
	}

	return 0;
}


s32 rxtx_performance_test(void *arg){
	txrx_performce_test_req_t *txrxTest = (txrx_performce_test_req_t *)arg;
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	u16 dstAddr = 0xffff;
	COPY_BUFFERTOU16_BE(dstAddr, ((u8 *)&(txrxTest->dstAddr)));

	u16 totalNum = 0;
	COPY_BUFFERTOU16_BE(totalNum, ((u8 *)&(txrxTest->sendCnt)));

	if(g_afTestReq.sendTotalCnt >= totalNum){
		ev_buf_free((u8 *)arg);
		return -1;
	}

	u8 dataLen = 50;
	u8 *pBuf = (u8 *)ev_buf_allocate(dataLen);
	if(pBuf){
		g_afTestReq.sendTotalCnt++;
		g_afTestReq.dstAddr = dstAddr;
		g_afTestReq.dstEp = txrxTest->dstEp;

		dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
		dstEpInfo.dstAddr.shortAddr = dstAddr;
		dstEpInfo.dstEp = txrxTest->dstEp;
		dstEpInfo.profileId = HA_PROFILE_ID;
		dstEpInfo.radius = 0;

		u8 *pData = pBuf;

		*pData++ = LO_UINT16(g_afTestReq.sendTotalCnt);
		*pData++ = HI_UINT16(g_afTestReq.sendTotalCnt);

		for(u8 i = 0; i < dataLen - 2; i++){
			*pData++ = i;
		}

		af_dataSend(txrxTest->srcEp, &dstEpInfo, ZCL_CLUSTER_TELINK_SDK_TEST_REQ, dataLen, pBuf, &g_afTestReq.dataApsCnt);

		ev_buf_free(pBuf);
	}

	return (txrxTest->interval * 10 * 1000);
}

s32 zbhci_nodeManageCmdHandler(void *arg){
	zbhci_cmdHandler_t *cmdInfo = arg;
	u16 cmdID = cmdInfo->cmdId;
	u8 *p = cmdInfo->payload;
	u8 len = 0;

	if(cmdID == ZBHCI_CMD_NODES_JOINED_GET_REQ){
#if ZB_COORDINATOR_ROLE
		u16 startIdx = (p[0] << 8) + p[1];

		u8 bufFree = 0;
		zbhci_mgmt_nodesJoined_rsp_hdr_t hdr;
		zbhci_mgmt_nodesJoined_rsp_t *rsp = (zbhci_mgmt_nodesJoined_rsp_t*) ev_buf_allocate(sizeof(zbhci_mgmt_nodesJoined_rsp_t));
		if(rsp){
			u8 validcnt = 0;
			u16 totalCnt = ss_nodeMacAddrFromdevKeyPair(startIdx, 6, &validcnt, rsp->macAddrList);

			addrExt_t *pMacAddr = rsp->macAddrList;
			for(s32 i = 0; i < validcnt; i++){
				ZB_LEBESWAP(((u8 *)pMacAddr), 8);
				pMacAddr++;
			}

			rsp->hdr.status = ZBHCI_MSG_STATUS_SUCCESS;
			COPY_BUFFERTOU16_BE(rsp->hdr.totalCnt, (u8 *)&totalCnt);
			COPY_BUFFERTOU16_BE(rsp->hdr.startIndex, (u8 *)&startIdx);
			rsp->hdr.listCnt = validcnt;
			len = OFFSETOF(zbhci_mgmt_nodesJoined_rsp_t, macAddrList) + sizeof(addrExt_t) * validcnt;
			bufFree = 1;
		}else{
			hdr.listCnt = 0;
			hdr.startIndex = startIdx;
			hdr.status = ZBHCI_MSG_STATUS_NO_MEMORY;
			len = sizeof(zbhci_mgmt_nodesJoined_rsp_hdr_t);
			rsp = (zbhci_mgmt_nodesJoined_rsp_t *)&hdr;
		}

		zbhciTx(ZBHCI_CMD_NODES_JOINED_GET_RSP, len, (u8 *)rsp);

		if(bufFree){
			ev_buf_free((u8 *)rsp);
		}
#endif
	}else if(cmdID == ZBHCI_CMD_NODES_TOGLE_TEST_REQ){
#if ZB_COORDINATOR_ROLE
		u32 onOff = *p;
		u8 interval = *(p+1);
		if(hci_nodeToggleTestEvt){
			TL_ZB_TIMER_CANCEL(&hci_nodeToggleTestEvt);
		}
		if(interval){
			hci_nodeToggleTestEvt = TL_ZB_TIMER_SCHEDULE(node_toggle_test, (void *)onOff, interval * 10 * 1000);
		}
#endif
	}else if(cmdID == ZBHCI_CMD_TXRX_PERFORMANCE_TEST_REQ){
#if AF_TEST_ENABLE
		if(!g_afTestReq.performaceTest){
			g_afTestReq.performaceTest = 1;

			txrx_performce_test_req_t *txrxTest = (txrx_performce_test_req_t *)ev_buf_allocate(sizeof(txrx_performce_test_req_t));
			if(txrxTest){
				memcpy(txrxTest, p, sizeof(txrx_performce_test_req_t));
				TL_ZB_TIMER_SCHEDULE(rxtx_performance_test, txrxTest, 10 * 1000);
			}
		}
#endif
	}else if(cmdID == ZBHCI_CMD_AF_DATA_SEND_TEST_REQ){
#if AF_TEST_ENABLE
		u8 len = p[6];
		zbhci_afDataSend_req_t *pAfDataSendReq = (zbhci_afDataSend_req_t *)ev_buf_allocate(sizeof(zbhci_afDataSend_req_t) + len);
		if(pAfDataSendReq){
			memcpy((u8 *)pAfDataSendReq, p, sizeof(zbhci_afDataSend_req_t));
			memcpy(pAfDataSendReq->payload, &p[7], pAfDataSendReq->dataLen);

			u16 clusterId = 0;
			epInfo_t dstEpInfo;
			TL_SETSTRUCTCONTENT(dstEpInfo, 0);

			dstEpInfo.dstEp = pAfDataSendReq->dstEp;
			dstEpInfo.profileId = HA_PROFILE_ID;
			dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
			COPY_BUFFERTOU16_BE(dstEpInfo.dstAddr.shortAddr, (u8 *)&(pAfDataSendReq->dstAddr));
			COPY_BUFFERTOU16_BE(clusterId, (u8 *)&(pAfDataSendReq->clusterId));

			u8 apsCnt = 0;
			af_dataSend(pAfDataSendReq->srcEp, &dstEpInfo, clusterId, pAfDataSendReq->dataLen, pAfDataSendReq->payload, &apsCnt);

			ev_buf_free((u8 *)pAfDataSendReq);
		}
#endif
	}

	ev_buf_free(arg);

	return -1;
}

void zbhciCmdHandler(u16 msgType, u16 msgLen, u8 *p){
	u8 ret[4] = {0,0,0,0};
	u8 seqNum = 0;//pdu tx seq num
	u8 st = 0;

	zbhci_cmdHandler_t *cmdInfo = (zbhci_cmdHandler_t*)ev_buf_allocate(msgLen+4);
	if(cmdInfo){
		cmdInfo->cmdId = msgType;
		memcpy(cmdInfo->payload, p, msgLen);
		switch (msgType)
		{
			case ZBHCI_CMD_BDB_COMMISSION_FORMATION:
			case ZBHCI_CMD_BDB_COMMISSION_STEER:
			case ZBHCI_CMD_BDB_COMMISSION_TOUCHLINK:
			case ZBHCI_CMD_BDB_COMMISSION_FINDBIND:
			case ZBHCI_CMD_BDB_FACTORY_RESET:
			case ZBHCI_CMD_BDB_PRE_INSTALL_CODE:
			case ZBHCI_CMD_BDB_CHANNEL_SET:
			case ZBHCI_CMD_BDB_DONGLE_WORKING_MODE_SET:
			case ZBHCI_CMD_BDB_NODE_DELETE:
			case ZBHCI_CMD_BDB_TX_POWER_SET:
				TL_SCHEDULE_TASK(zbhci_bdbCmdHandler, cmdInfo);
				break;


			case ZBHCI_CMD_DISCOVERY_NWK_ADDR_REQ:
			case ZBHCI_CMD_DISCOVERY_IEEE_ADDR_REQ:
			case ZBHCI_CMD_DISCOVERY_NODE_DESC_REQ:
			case ZBHCI_CMD_DISCOVERY_SIMPLE_DESC_REQ:
			case ZBHCI_CMD_DISCOVERY_MATCH_DESC_REQ:
			case ZBHCI_CMD_DISCOVERY_ACTIVE_EP_REQ:
			case ZBHCI_CMD_DISCOVERY_LEAVE_REQ:
				TL_SCHEDULE_TASK(zbhci_discoveryCmdHandler, cmdInfo);
				break;

			case ZBHCI_CMD_BINDING_REQ:
			case ZBHCI_CMD_UNBINDING_REQ:
				TL_SCHEDULE_TASK(zbhci_bindCmdHandler, cmdInfo);
				break;

			case ZBHCI_CMD_MGMT_LQI_REQ:
			case ZBHCI_CMD_MGMT_BIND_REQ:
			case ZBHCI_CMD_MGMT_LEAVE_REQ:
			case ZBHCI_CMD_MGMT_DIRECT_JOIN_REQ:
			case ZBHCI_CMD_MGMT_PERMIT_JOIN_REQ:
			case ZBHCI_CMD_MGMT_NWK_UPDATE_REQ:
				TL_SCHEDULE_TASK(zbhci_mgmtCmdHandler, cmdInfo);
				break;

			case ZBHCI_CMD_NODES_JOINED_GET_REQ:
			case ZBHCI_CMD_NODES_TOGLE_TEST_REQ:
			case ZBHCI_CMD_TXRX_PERFORMANCE_TEST_REQ:
			case ZBHCI_CMD_AF_DATA_SEND_TEST_REQ:
				TL_ZB_TIMER_SCHEDULE(zbhci_nodeManageCmdHandler, cmdInfo, 100 * 1000);
				break;

			case ZBHCI_CMD_ZCL_ATTR_READ:
			case ZBHCI_CMD_ZCL_ATTR_WRITE:
			case ZBHCI_CMD_ZCL_CONFIG_REPORT:
			case ZBHCI_CMD_ZCL_READ_REPORT_CFG:
				TL_SCHEDULE_TASK(zbhci_clusterCommonCmdHandle, cmdInfo);
				break;

			case ZBHCI_CMD_ZCL_BASIC_RESET:
				TL_SCHEDULE_TASK(zbhci_clusterBasicHandle, cmdInfo);
				break;

			case ZBHCI_CMD_ZCL_GROUP_ADD:
			case ZBHCI_CMD_ZCL_GROUP_VIEW:
			case ZBHCI_CMD_ZCL_GROUP_GET_MEMBERSHIP:
			case ZBHCI_CMD_ZCL_GROUP_REMOVE:
			case ZBHCI_CMD_ZCL_GROUP_REMOVE_ALL:
			case ZBHCI_CMD_ZCL_GROUP_ADD_IF_IDENTIFY:
				TL_SCHEDULE_TASK(zbhci_clusterGroupHandle, cmdInfo);
				break;

			case ZBHCI_CMD_ZCL_IDENTIFY:
			case ZBHCI_CMD_ZCL_IDENTIFY_QUERY:
				TL_SCHEDULE_TASK(zbhci_zclIdentifyCmdHandle, cmdInfo);
				break;

			case ZBHCI_CMD_ZCL_ONOFF_ON:
			case ZBHCI_CMD_ZCL_ONOFF_OFF:
			case ZBHCI_CMD_ZCL_ONOFF_TOGGLE:
				TL_SCHEDULE_TASK(zbhci_zclOnoffCmdHandle, cmdInfo);
				break;

			case ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL:
			case ZBHCI_CMD_ZCL_LEVEL_MOVE:
			case ZBHCI_CMD_ZCL_LEVEL_STEP:
			case ZBHCI_CMD_ZCL_LEVEL_STOP:
			case ZBHCI_CMD_ZCL_LEVEL_MOVE2LEVEL_WITHONOFF:
			case ZBHCI_CMD_ZCL_LEVEL_MOVE_WITHONOFF:
			case ZBHCI_CMD_ZCL_LEVEL_STEP_WITHONOFF:
			case ZBHCI_CMD_ZCL_LEVEL_STOP_WITHONOFF:
				TL_SCHEDULE_TASK(zbhci_zclLevelCtrlCmdHandle, cmdInfo);
				break;

			case ZBHCI_CMD_ZCL_SCENE_ADD:
			case ZBHCI_CMD_ZCL_SCENE_VIEW:
			case ZBHCI_CMD_ZCL_SCENE_REMOVE:
			case ZBHCI_CMD_ZCL_SCENE_REMOVE_ALL:
			case ZBHCI_CMD_ZCL_SCENE_STORE:
			case ZBHCI_CMD_ZCL_SCENE_RECALL:
			case ZBHCI_CMD_ZCL_SCENE_GET_MEMBERSHIP:
				TL_SCHEDULE_TASK(zbhci_clusterSceneHandle, cmdInfo);
				break;

            case ZBHCI_CMD_ZCL_COLOR_MOVE2HUE:
            case ZBHCI_CMD_ZCL_COLOR_MOVE2COLOR:
            case ZBHCI_CMD_ZCL_COLOR_MOVE2SAT:
            case ZBHCI_CMD_ZCL_COLOR_MOVE2TEMP:
                TL_SCHEDULE_TASK(zbhci_zclColorCtrlCmdHandle, cmdInfo);
				break;

			case ZBHCI_CMD_ZCL_OTA_IMAGE_NOTIFY:
				TL_SCHEDULE_TASK(zbhci_clusterOTAHandle, cmdInfo);
				break;

			default:
				ev_buf_free((u8*)cmdInfo);
				st = ZBHCI_MSG_STATUS_UNHANDLED_COMMAND;
				break;

		}
	}else{
		st = ZBHCI_MSG_STATUS_NO_MEMORY;
	}

	COPY_U16TOBUFFER_BE(ret,msgType);
	ret[2] = st;
	ret[3] =  seqNum;
	zbhciTx ( ZBHCI_CMD_ACK,
					   4,
					   ret );
}

#endif	/* ZBHCI_EN */
