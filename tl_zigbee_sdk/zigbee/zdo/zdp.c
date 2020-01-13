#include "../include/zb_common.h"

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
	{COMPLEX_DESCRIPTOR_CLID,   	NULL},
	{USER_DESCRIPTOR_CLID,			NULL},
	{DISCOVERY_CASH_CLID,       	NULL},
#ifdef	ZB_ROUTER_ROLE
	{DEVICE_ANNCE_CLID,				zdo_deviceAnnounceIndicate},
	{PARENT_ANNCE_CLID,				zdo_parentAnnounceIndicate},
#endif
	{SYSTEM_SERVER_DISCOVERY_CLID,	zdo_SysServerDiscoveryIndiate},
	{END_DEVICE_BIND_CLID,			zdo_endDeviceBindIndicate},
	{BIND_CLID,						zdo_bindOrUnbindIndicate},
	{UNBIND_CLID,                   zdo_bindOrUnbindIndicate},
	{MGMT_LQI_CLID,					zdo_mgmtLqiIndictate},
	{MGMT_RTG_CLID,                 NULL},
	{MGMT_BIND_CLID,               	zdo_mgmtBindIndicate},
	{MGMT_LEAVE_CLID,               zdo_mgmtLeaveIndicate},
#ifdef	ZB_ROUTER_ROLE
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


_CODE_ZDO_ zdo_status_t zdp_serverCmdHandler(void *ind){
	aps_data_ind_t *p = (aps_data_ind_t *)ind;

#ifdef	ZB_ROUTER_ROLE
	if(p->cluster_id == PARENT_ANNCE_RESP_CLID){
		zdo_parentAnnounceNotify(ind);
	}

	if(p->cluster_id == NWK_ADDR_RESP_CLID || p->cluster_id == IEEE_ADDR_RESP_CLID){
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
	return zdp_cb_process(zdpInd->seq_num, p);
}


_CODE_ZDO_ static void zdp_txCnfCb(void *arg){
	//apsdeDataConf_t *pApsDataCnf = (apsdeDataConf_t *)arg;

}

_CODE_ZDO_ void zdp_rxDataIndication(void *ind){
	aps_data_ind_t *p = ind;
	bool need_free_buff = 0;

	if(p->cluster_id >= NWK_ADDR_CLID && p->cluster_id <= MGMT_NWK_UPDATE_CLID){
		if(ZDO_NOT_SUPPORTED == zdp_clientCmdHandler(ind)){
			need_free_buff = 1;
		}
	}else if(p->cluster_id >= NWK_ADDR_RESP_CLID && p->cluster_id <= MGMT_NWK_UPDATE_NOTIFY_CLID){
		zdp_serverCmdHandler(ind);
		need_free_buff = 1;
	}else{
		zdo_unsupportedCmdIndicate(ind);
	}

	if(need_free_buff){
		zb_buf_free((zb_buf_t *)ind);
	}
}


void zdo_zdp_init(void){
	af_endpointRegister(ZDO_EP, (af_simple_descriptor_t *)&zdoSd, zdp_rxDataIndication, zdp_txCnfCb);
}
