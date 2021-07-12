/********************************************************************************************************
 * @file	zdp.c
 *
 * @brief	This is the source file for zdp
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
#include "../common/includes/zb_common.h"


typedef void (*zdp_funcCb)(void *ind);

typedef struct{
	zdp_clusterId_e clusterId;
	u16				restricted;
	zdp_funcCb		func;
}zdp_funcList_t;

const zdp_funcList_t g_zdpClientFunc[] = {
	{NWK_ADDR_REQ_CLID,  				FALSE,	zdo_nwkAddrIndicate},
	{IEEE_ADDR_REQ_CLID,  				FALSE,	zdo_ieeeAddrIndicate},
	{NODE_DESC_REQ_CLID, 				FALSE,	zdo_descriptorsIndicate},
	{POWER_DESC_REQ_CLID, 				FALSE,	zdo_descriptorsIndicate},
	{SIMPLE_DESC_REQ_CLID,				FALSE,	zdo_descriptorsIndicate},
	{ACTIVE_EP_REQ_CLID, 				FALSE,	zdo_activeEpIndicate},
	{MATCH_DESC_REQ_CLID, 				FALSE,	zdo_matchDescriptorIndicate},
//	{COMPLEX_DESC_REQ_CLID,   			FALSE,	zdo_complexDescIndicate},		//DEPRECATED
//	{USER_DESC_REQ_CLID,				FALSE,	zdo_userDescIndicate},			//DEPRECATED
#ifdef ZB_ROUTER_ROLE
	{DEVICE_ANNCE_CLID,					FALSE,	zdo_deviceAnnounceIndicate},
	{PARENT_ANNCE_CLID,					FALSE,	zdo_parentAnnounceIndicate},
#endif
	{SYSTEM_SERVER_DISCOVERY_REQ_CLID,	FALSE,	zdo_SysServerDiscoveryIndiate},
	{END_DEVICE_BIND_REQ_CLID,			FALSE,	zdo_endDeviceBindIndicate},
	{BIND_REQ_CLID,						TRUE,	zdo_bindOrUnbindIndicate},
	{UNBIND_REQ_CLID,                   TRUE,	zdo_bindOrUnbindIndicate},
	{MGMT_LQI_REQ_CLID,					FALSE,	zdo_mgmtLqiIndictate},
	{MGMT_BIND_REQ_CLID,               	FALSE,	zdo_mgmtBindIndicate},
	{MGMT_LEAVE_REQ_CLID,               TRUE,	zdo_mgmtLeaveIndicate},
#ifdef ZB_ROUTER_ROLE
	{MGMT_PERMIT_JOINING_REQ_CLID,		FALSE,	zdo_mgmtPermitJoinIndicate},
#endif
	{MGMT_NWK_UPDATE_REQ_CLID,	    	FALSE,	zdo_mgmNwkUpdateIndicate}
};


const u16 zdoIncluster[7] = {
	NWK_ADDR_RSP_CLID,
	IEEE_ADDR_RSP_CLID,
	NODE_DESC_RSP_CLID,
	POWER_DESC_RSP_CLID,
	SIMPLE_DESC_RSP_CLID,
	ACTIVE_EP_RSP_CLID,
	MATCH_DESC_RSP_CLID
};

const u16 zdoOutCluster[8] = {
	NWK_ADDR_REQ_CLID,
	IEEE_ADDR_REQ_CLID,
	NODE_DESC_REQ_CLID,
	POWER_DESC_REQ_CLID,
	SIMPLE_DESC_REQ_CLID,
	ACTIVE_EP_REQ_CLID,
	MATCH_DESC_REQ_CLID,
	DEVICE_ANNCE_CLID
};

const af_simple_descriptor_t zdoSd = {
	ZDO_PROFILE_ID,			/* Profile id */
	0,              		/* Application device identifier */
	ZDO_EP,					/* EP */
	0,                      /* Application device version */
	0,						/* Reserved */
	7,              		/* Application input cluster count */
	8,             			/* Application output cluster count */
	(u16 *)zdoIncluster,    /* Application input cluster list */
	(u16 *)zdoOutCluster,   /* Application output cluster list */
};



_CODE_ZDO_ static void zdp_txCnfCb(void *arg){
	//apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

}

_CODE_ZDO_ void zdp_serverCmdHandler(void *ind){
	aps_data_ind_t *p = (aps_data_ind_t *)ind;

#ifdef ZB_ROUTER_ROLE
	if(p->cluster_id == PARENT_ANNCE_RSP_CLID){
		zdo_parentAnnounceNotify(ind);
		zb_buf_free((zb_buf_t *)ind);
		return;
	}else if(p->cluster_id == NWK_ADDR_RSP_CLID || p->cluster_id == IEEE_ADDR_RSP_CLID){
		zdo_remoteAddrNotify(ind);
	}
#endif

	zdo_zdpDataInd_t zdpInd;
	TL_SETSTRUCTCONTENT(zdpInd, 0);

	zdpInd.src_addr = p->src_short_addr;
	zdpInd.clusterId = p->cluster_id;
	zdpInd.seq_num = p->asdu[0];
	zdpInd.status = p->asdu[1];
	zdpInd.length = p->asduLength;
	zdpInd.zpdu = p->asdu;

	zdo_zdpDataInd_t *pZdpInd = (zdo_zdpDataInd_t *)ind;
	memcpy((u8 *)pZdpInd, (u8 *)&zdpInd, sizeof(zdo_zdpDataInd_t));

	zdp_cb_process(pZdpInd->seq_num, ind);
	zb_buf_free((zb_buf_t *)ind);
}

_CODE_ZDO_ static void zdp_clientCmdHandler(void *ind){
	aps_data_ind_t *p = (aps_data_ind_t *)ind;
	zdo_status_t sta = ZDO_NOT_SUPPORTED;

	for(u32 i = 0; i < sizeof(g_zdpClientFunc)/sizeof(zdp_funcList_t); i++){
		if((g_zdpClientFunc[i].clusterId == p->cluster_id) && g_zdpClientFunc[i].func){
			/* Check configuration mode. */
			if((g_zdpClientFunc[i].restricted == TRUE) && (APS_IB().aps_zdo_restricted_mode == TRUE) &&
			  ((p->src_short_addr != NWK_NIB().managerAddr) || !(p->security_status & SECURITY_IN_APSLAYER))){
				sta = ZDO_NOT_AUTHORIZED;
				break;
			}

			g_zdpClientFunc[i].func(ind);
			return;
		}
	}

	if((p->dst_addr_mode == APS_SHORT_DSTADDR_WITHEP) && ZB_NWK_IS_ADDRESS_BROADCAST(p->dst_addr)){
		zb_buf_free((zb_buf_t *)ind);
		return;
	}

	zdo_zdp_req_t zzr;
	TL_SETSTRUCTCONTENT(zzr, 0);

	TL_BUF_INITIAL_ALLOC((zb_buf_t *)ind, 2, zzr.zdu, u8 *);
	u8 *ptr = zzr.zdu;
	*ptr++ = p->asdu[0];//seqNum
	*ptr++ = sta;

	zzr.cluster_id = (p->cluster_id | 0x8000);
	zzr.zduLen = ptr - zzr.zdu;
	zzr.buff_addr = ind;
	zzr.dst_addr_mode = SHORT_ADDR_MODE;
	zzr.dst_nwk_addr = p->src_short_addr;
	zzr.zdoRspReceivedIndCb = NULL;

	zdo_send_req(&zzr);
	zb_buf_free((zb_buf_t *)ind);
}

_CODE_ZDO_ static void zdp_rxDataIndication(void *ind){
	aps_data_ind_t *p = (aps_data_ind_t *)ind;

	if(p->cluster_id & BIT(15)){
		zdp_serverCmdHandler(ind);
	}else{
		zdp_clientCmdHandler(ind);
	}
}

_CODE_ZDO_ void zdp_init(void){
	af_endpointRegister(ZDO_EP, (af_simple_descriptor_t *)&zdoSd, zdp_rxDataIndication, zdp_txCnfCb);
}

#if 0
_CODE_ZDO_ void zdo_complexDescIndicate(void *buf){
	zb_buf_t *zbuff = buf;
	aps_data_ind_t *ad = buf;
	u8 *payload = ad->asdu;
	u8 seqNum = *payload++;

	u16 nwkAddrReq;
	COPY_BUFFERTOU16(nwkAddrReq, payload);
	payload += 2;

	zdo_zdp_req_t zzr;
	TL_SETSTRUCTCONTENT(zzr, 0);

	u8 offset = OFFSETOF(zdo_complex_descriptor_resp_t, compDesc);
	TL_BUF_INITIAL_ALLOC(zbuff, offset, zzr.zdu, u8 *);

	u8 *ptr = zzr.zdu;
	*ptr++ = seqNum;
	*ptr++ = ZDO_NOT_SUPPORTED;
	COPY_U16TOBUFFER(ptr, nwkAddrReq);
	ptr += 2;
	*ptr++ = 0;//length

	zzr.cluster_id = COMPLEX_DESC_RSP_CLID;
	zzr.zduLen = ptr - zzr.zdu;
	zzr.buff_addr = buf;
	zzr.dst_addr_mode = SHORT_ADDR_MODE;
	zzr.dst_nwk_addr = ad->src_short_addr;
	zzr.zdoRspReceivedIndCb = NULL;

	zdo_send_req(&zzr);
	zb_buf_free((zb_buf_t *)zzr.buff_addr);
}

_CODE_ZDO_ void zdo_userDescIndicate(void *buf){
	zb_buf_t *zbuff = buf;
	aps_data_ind_t *ad = buf;
	u8 *payload = ad->asdu;
	u8 seqNum = *payload++;

	u16 nwkAddrReq;
	COPY_BUFFERTOU16(nwkAddrReq, payload);
	payload += 2;

	zdo_zdp_req_t zzr;
	TL_SETSTRUCTCONTENT(zzr, 0);

	u8 offset = OFFSETOF(zdo_user_descriptor_resp_t, user_desc);
	TL_BUF_INITIAL_ALLOC(zbuff, offset, zzr.zdu, u8 *);

	u8 *ptr = zzr.zdu;
	*ptr++ = seqNum;
	*ptr++ = ZDO_NOT_SUPPORTED;
	COPY_U16TOBUFFER(ptr, nwkAddrReq);
	ptr += 2;
	*ptr++ = 0;//length

	zzr.cluster_id = USER_DESC_RSP_CLID;
	zzr.zduLen = ptr - zzr.zdu;
	zzr.buff_addr = buf;
	zzr.dst_addr_mode = SHORT_ADDR_MODE;
	zzr.dst_nwk_addr = ad->src_short_addr;
	zzr.zdoRspReceivedIndCb = NULL;

	zdo_send_req(&zzr);
	zb_buf_free((zb_buf_t *)zzr.buff_addr);
}
#endif

