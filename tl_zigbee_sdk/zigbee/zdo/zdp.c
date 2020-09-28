#include "zb_common.h"


typedef void (*zdp_funcCb)(void *ind);

typedef struct{
	zdp_clusterId_e clusterId;
	zdp_funcCb		func;
}zdp_funcList_t;

const zdp_funcList_t g_zdpClientFunc[] = {
	{NWK_ADDR_CLID,  				zdo_nwkAddrIndicate},
	{IEEE_ADDR_CLID,  				zdo_ieeeAddrIndicate},
	{NODE_DESCRIPTOR_CLID, 			zdo_descriptorsIndicate},
	{POWER_DESCRIPTOR_CLID, 		zdo_descriptorsIndicate},
	{SIMPLE_DESCRIPTOR_CLID,		zdo_descriptorsIndicate},
	{ACTIVE_ENDPOINTS_CLID, 		zdo_activeEpIndicate},
	{MATCH_DESCRIPTOR_CLID, 		zdo_matchDescriptorIndicate},
	{COMPLEX_DESCRIPTOR_CLID,   	zdo_complexDescIndicate},
	{USER_DESCRIPTOR_CLID,			zdo_userDescIndicate},
#ifdef ZB_ROUTER_ROLE
	{DEVICE_ANNCE_CLID,				zdo_deviceAnnounceIndicate},
	{PARENT_ANNCE_CLID,				zdo_parentAnnounceIndicate},
#endif
	{SYSTEM_SERVER_DISCOVERY_CLID,	zdo_SysServerDiscoveryIndiate},
	{END_DEVICE_BIND_CLID,			zdo_endDeviceBindIndicate},
	{BIND_CLID,						zdo_bindOrUnbindIndicate},
	{UNBIND_CLID,                   zdo_bindOrUnbindIndicate},
	{MGMT_LQI_CLID,					zdo_mgmtLqiIndictate},
	{MGMT_BIND_CLID,               	zdo_mgmtBindIndicate},
	{MGMT_LEAVE_CLID,               zdo_mgmtLeaveIndicate},
#ifdef ZB_ROUTER_ROLE
	{MGMT_PERMIT_JOINING_CLID,		zdo_mgmtPermitJoinIndicate},
#endif
	{MGMT_NWK_UPDATE_CLID,	    	zdo_mgmNwkUpdateIndicate}
};


const u16 zdoIncluster[7] = {
	NWK_ADDR_RESP_CLID,
	IEEE_ADDR_RESP_CLID,
	NODE_DESC_RESP_CLID,
	POWER_DESC_RESP_CLID,
	SIMPLE_DESC_RESP_CLID,
	ACTIVE_EP_RESP_CLID,
	MATCH_DESC_RESP_CLID
};

const u16 zdoOutCluster[8] = {
	NWK_ADDR_CLID,
	IEEE_ADDR_CLID,
	NODE_DESCRIPTOR_CLID,
	POWER_DESCRIPTOR_CLID,
	SIMPLE_DESCRIPTOR_CLID,
	ACTIVE_ENDPOINTS_CLID,
	MATCH_DESCRIPTOR_CLID,
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



_CODE_ZDO_ zdo_status_t zdp_clientCmdHandler(void *ind){
	aps_data_ind_t *p = (aps_data_ind_t *)ind;
	for(u32 i = 0; i < sizeof(g_zdpClientFunc)/sizeof(zdp_funcList_t); i++){
		if(g_zdpClientFunc[i].clusterId == p->cluster_id){
			if(g_zdpClientFunc[i].func){
				g_zdpClientFunc[i].func(ind);
				return ZDO_SUCCESS;
			}
		}
	}

	return ZDO_NOT_SUPPORTED;
}

_CODE_ZDO_ void zdp_serverCmdHandler(void *ind){
	aps_data_ind_t *p = (aps_data_ind_t *)ind;

#ifdef ZB_ROUTER_ROLE
	if(p->cluster_id == PARENT_ANNCE_RESP_CLID){
		zdo_parentAnnounceNotify(ind);
		return;
	}else if(p->cluster_id == NWK_ADDR_RESP_CLID || p->cluster_id == IEEE_ADDR_RESP_CLID){
		zdo_remoteAddrNotify(ind);
	}
#endif

	zdo_zdpDataInd_t *zdpInd = (zdo_zdpDataInd_t *)p;
	u8 *pld = (u8 *)p->asdu;
	zdpInd->src_addr = p->src_short_addr;
	zdpInd->seq_num = pld[0];
	zdpInd->status = pld[1];
	zdpInd->clusterId = p->cluster_id;
	zdpInd->length = p->asduLength;
	zdpInd->zpdu = p->asdu;
	zdp_cb_process(zdpInd->seq_num, p);
}

_CODE_ZDO_ static void zdp_txCnfCb(void *arg){
	//apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

}

_CODE_ZDO_ void zdp_rxDataIndication(void *ind){
	aps_data_ind_t *p = ind;
	bool need_free_buff = 1;

	if(p->cluster_id & BIT(15)){
		zdp_serverCmdHandler(ind);
	}else{
		if(zdp_clientCmdHandler(ind) == ZDO_SUCCESS){
			need_free_buff = 0;
		}
	}

	if(need_free_buff){
		zb_buf_free((zb_buf_t *)ind);
	}
}

_CODE_ZDO_ void zdp_init(void){
	af_endpointRegister(ZDO_EP, (af_simple_descriptor_t *)&zdoSd, zdp_rxDataIndication, zdp_txCnfCb);
}

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

	zzr.cluster_id = COMPLEX_DESC_RESP_CLID;
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

	zzr.cluster_id = USER_DESCRIPTOR_RESP_CLID;
	zzr.zduLen = ptr - zzr.zdu;
	zzr.buff_addr = buf;
	zzr.dst_addr_mode = SHORT_ADDR_MODE;
	zzr.dst_nwk_addr = ad->src_short_addr;
	zzr.zdoRspReceivedIndCb = NULL;

	zdo_send_req(&zzr);
	zb_buf_free((zb_buf_t *)zzr.buff_addr);
}


