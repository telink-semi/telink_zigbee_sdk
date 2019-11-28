#include "../include/zb_common.h"


#if defined ZB_COORDINATOR_ROLE && (ZB_COORDINATOR_ROLE == 1)
	#define LOGICAL_TYPE			DEVICE_TYPE_COORDINATOR
	#define SERVER_MASK				(ND_SERVER_MASK_REV21 | ND_SERVER_MASK_PRIMARY_TRUST_CENTER | ND_SERVER_MASK_NETWORK_MANAGER)
	#define MAC_CAPABILITY_FLAGS	(MAC_CAP_DEVICE_TYPE_FFD | MAC_CAP_POWER_SOURCE_MAINS | MAC_CAP_RX_ON_WHEN_IDLE | MAC_CAP_ALLOCATE_ADDRESS | MAC_CAP_ALTERNATE_PAN_COORDINATOR)
#elif defined ZB_ROUTER_ROLE && (ZB_ROUTER_ROLE == 1)
	#define LOGICAL_TYPE			DEVICE_TYPE_ROUTER
	#define SERVER_MASK				(ND_SERVER_MASK_REV21)
	#define MAC_CAPABILITY_FLAGS	(MAC_CAP_DEVICE_TYPE_FFD | MAC_CAP_POWER_SOURCE_MAINS | MAC_CAP_RX_ON_WHEN_IDLE | MAC_CAP_ALLOCATE_ADDRESS)
#elif defined ZB_ED_ROLE && (ZB_ED_ROLE == 1)
	#define LOGICAL_TYPE			DEVICE_TYPE_END_DEVICE
	#define SERVER_MASK				(ND_SERVER_MASK_REV21)
#if ZB_MAC_RX_ON_WHEN_IDLE
	#define MAC_CAPABILITY_FLAGS	(MAC_CAP_DEVICE_TYPE_RFD | MAC_CAP_RX_ON_WHEN_IDLE | MAC_CAP_ALLOCATE_ADDRESS)
#else
	#define MAC_CAPABILITY_FLAGS	(MAC_CAP_DEVICE_TYPE_RFD | MAC_CAP_ALLOCATE_ADDRESS)
#endif
#else
	#error	"DEVICE ROLE ERROR!"
#endif

static node_descriptor_t ndt = {
	.logical_type =				LOGICAL_TYPE,
	.server_mask = 				SERVER_MASK,
	.mac_capability_flag = 		MAC_CAPABILITY_FLAGS,
	.complex_descriptor_ava = 	FALSE,
	.user_descriptor_ava = 		FALSE,
	.reserved = 				0,
	.aps_flag = 				0,
	.freq_band = 				NODE_FREQ_2400,
	.max_buff_size = 			AF_NSDU_MAX_LEN,
	.max_in_tr_size = 			AF_NSDU_MAX_LEN,
	.max_out_tr_size = 			AF_NSDU_MAX_LEN,
	.mcL8 =   					(TELINK_MANUFACTURER_CODE & 0xff),
	.mcH8 =   					((TELINK_MANUFACTURER_CODE >> 8) & 0xff),
};

//power descriptor strut
static power_descriptor_t pdt = {
#if defined (ZB_ROUTER_ROLE) && (ZB_ROUTER_ROLE == TRUE)
	.current_power_mode = POWER_MODE_RECEIVER_SYNCRONIZED_WHEN_ON_IDLE,
	.available_power_sources = POWER_SRC_MAINS_POWER,
	.current_power_source = POWER_SRC_MAINS_POWER,
	.current_power_source_level = POWER_LEVEL_PERCENT_100,
#elif defined (ZB_ED_ROLE) && (ZB_ED_ROLE == TRUE)
	.current_power_mode = POWER_MODE_RECEIVER_COMES_PERIODICALLY,
	.available_power_sources = POWER_SRC_RECHARGEABLE_BATTERY,
	.current_power_source = POWER_SRC_RECHARGEABLE_BATTERY,
	.current_power_source_level = POWER_LEVEL_PERCENT_66,
#endif
};

//The number of available active endpoint numbers, used for simple descriptor and active ep response
u8	available_active_ep_num = 0;

//Application framework layer flags, the enum definication can be find zdo_af_flag_enum
u8	app_framework_flag = 0;

static u8 	afHandle = 0;

#define		AF_HANDLE_MASK		0x3f

//AF endpoint descriptor strut
af_endpoint_descriptor_t aed[MAX_ACTIVE_EP_NUMBER];
af_endpoint_descriptor_t zdo_epDesc;

const zdo_attrCfg_t afAttrDefault = {
		.config_nwk_indirectPollRate 		= 0,
		.config_parent_link_retry_threshold = ZDO_MAX_PARENT_THRESHOLD_RETRY,
		.config_nwk_scan_attempts 			= ZDO_NWK_SCAN_ATTEMPTS,
		.config_nwk_time_btwn_scans			= ZDO_NWK_TIME_BTWN_SCANS,
		.config_permit_join_duration 		= ZDO_PERMIT_JOIN_DURATION,
		.config_rejoin_interval 			= ZDO_REJOIN_INTERVAL,
		.config_max_rejoin_interval			= ZDO_MAX_REJOIN_INTERVAL,
};

zdo_attrCfg_t zdo_cfg_attributes;

_CODE_ZDO_ u8 af_handleGet(){
	return (afHandle++)&(AF_HANDLE_MASK);
}

_CODE_ZDO_ u8 af_handleIs(void){
	return (afHandle-1)&(AF_HANDLE_MASK);
}

_CODE_ZDO_ void af_epDescInit(void ){
	memset((u8 *)aed,	0,	sizeof(aed));
}

/****************************************************************************************************
 * @brief	AF layer node power descriptor set interface
 *
 * @param	value: power_descriptor_t
 *
 * @return	none
 */
_CODE_ZDO_ void af_powerDescriptorSet(power_descriptor_t *value){
	memcpy((u8 *)&pdt,	value,	sizeof(*value));
}


/****************************************************************************************************
 * @brief	AF layer node descriptor set interface
 *
 * @param	value: node_descriptor_t
 *
 * @return	none
 */
_CODE_ZDO_ void af_nodeDescriptorSet(node_descriptor_t *n){
	memcpy((u8 *)&ndt,	n,	sizeof(*n));
}

/**************************************************************************************************************
 * @brief	Internal used, descriptor init function
 */
void af_nodeDescManuCodeUpdate(u16 manuCode){
	ndt.mcL8 = LO_UINT16(manuCode);
	ndt.mcH8 = HI_UINT16(manuCode);
}

void af_nodeDescRxOnWhenIdleUpdate(bool enable){
	if(enable){
		ndt.mac_capability_flag |= BIT(3);
	}else{
		ndt.mac_capability_flag &= ~BIT(3);
	}
}

void af_nodeDescStackRevisionSet(u8 revision){
	ndt.server_mask &= (~(0x7f << 9));
	ndt.server_mask |= (revision << 9);
}

u8 af_nodeDescStackRevisionGet(void){
	return ((u8)((ndt.server_mask & 0xfe00) >> 9));
}


u8 af_nodeMacCapabilityGet(void){
	return ndt.mac_capability_flag;
}

void af_powerDescPowerModeUpdate(power_mode_t mode){
#if defined (ZB_ED_ROLE) && (ZB_ED_ROLE == TRUE)
	pdt.current_power_mode = mode;
#endif
}

device_type_t af_nodeDevTypeGet(void){
	return ndt.logical_type;
}


_CODE_ZDO_ bool af_endpointUnregister(u8 ep){
	for(u8 i=0;i<available_active_ep_num;i++){
		if(aed[i].ep == ep){
			TL_SETSTRUCTCONTENT(aed[i],0);
			if((i + 1) != available_active_ep_num){
				memcpy((u8 *)&aed[i],(u8 *)&aed[i+1],sizeof(aed[i])*(available_active_ep_num - i));
			}
			available_active_ep_num--;
			return TRUE;
		}
	}
	return FALSE;
}

/****************************************************************************************************
 * @brief	AF layer endpoint descriptor register interface, call this function to add ep descriptor to the AF layer
 *
 * @param	ep: end point number
 * 			simple_desc: af_simple_descriptor_t
 * 			cb: call back function
 *
 * @return	TRUE: add success
 * 			FALSE: endpoint full, no space, add fail
 */
_CODE_ZDO_ bool af_endpointRegister(u8 ep, af_simple_descriptor_t *simple_desc, af_endpoint_cb_t rx_cb, af_dataCnf_cb_t cnfCb)
{
	if(ep == ZDO_EP){
		zdo_epDesc.cb_cnf = cnfCb;
		zdo_epDesc.cb_rx = rx_cb;
		zdo_epDesc.correspond_simple_desc = (af_simple_descriptor_t *)simple_desc;
		return TRUE;
	}

	for(u8 i = 0; i < MAX_ACTIVE_EP_NUMBER; i++){
		if(aed[i].ep == ep){
			return FALSE;
		}
	}

	if (available_active_ep_num < MAX_ACTIVE_EP_NUMBER) {
		aed[available_active_ep_num].ep = ep;
		aed[available_active_ep_num].correspond_simple_desc = (af_simple_descriptor_t *)simple_desc;
		aed[available_active_ep_num].cb_cnf = cnfCb;
		aed[available_active_ep_num++].cb_rx = rx_cb;
	}else{
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************************************
 * @brief	AF layer init function, called during ZDO init
 */
_CODE_ZDO_ void zdo_af_init(void){
	afHandle = 0;
	memcpy((u8 *)&zdo_cfg_attributes, (u8 *)&afAttrDefault, sizeof(zdo_cfg_attributes));

#if defined ZB_COORDINATOR_ROLE
	zdo_af_flag_rise(ZDO_AF_FLAG_AUTH_ALLOW);
#endif
}


/**************************************************************************************************
 * @brief	Check whether simple descriptor request's end-point matched with local registered end point info
 *
 * @param	ep - end point to compare
 *
 * @return	INVALID_CMP_RESULT - Not matched
 * 			i - idnex of the local information base
 */

_CODE_ZDO_ u8 af_endpointMatchedLocal(u8 ep){
	for(u8 i=0; i<available_active_ep_num; i++){
		if(aed[i].ep == ep){
			return i;
		}
	}
	return INVALID_CMP_RESULT;
}

_CODE_ZDO_ zdo_status_t af_clusterMatchedLocal(u16 clusterID, u8 ep){
	for(u8 i=0; i<available_active_ep_num;i++){
		if(aed[i].ep == ep){
			for(u8 j=0; j< aed[i].correspond_simple_desc->app_in_cluster_count;j++){
				if(aed[i].correspond_simple_desc->app_in_cluster_lst[j] == clusterID){
					return ZDO_SUCCESS;
				}
			}

			for(u8 j=0; j< aed[i].correspond_simple_desc->app_out_cluster_count;j++){
				if(aed[i].correspond_simple_desc->app_out_cluster_lst[j] == clusterID){
					return ZDO_SUCCESS;
				}
			}
		}
	}
	return ZDO_NO_ENTRY;
}


_CODE_ZDO_ zdo_status_t af_profileMatchedLocal(u16 profileID, u8 ep){
	if(profileID == LL_PROFILE_ID){
		return ZDO_SUCCESS;
	}

	if((ep == ZDO_EP) && (profileID == ZDO_PROFILE_ID)){
		return ZDO_SUCCESS;
	}

	for(u8 i=0; i < available_active_ep_num; i++){
		if(((aed[i].ep == ep) || (ep == 0xFF)) && ((aed[i].correspond_simple_desc->app_profile_id == profileID) || (profileID == 0xFFFF))){
			return ZDO_SUCCESS;
		}
	}

	return ZDO_NO_DESCRIPTOR;
}

/*****************************************************************************************
 * @brief	copy simple descriptor interface used building simple descriptor rsp message
 *
 * @param	*dst - destination address to store simple descriptor info
 * 			index - local simple descriptor index
 *
 * @return	length of the simple descriptor info
 */
_CODE_ZDO_ u8 af_simpleDescriptorCopy(u8 *dst, u8 index){
	u8 rt_len = 0;
	if(index < available_active_ep_num){
		u8 *ptr = dst;
		af_simple_descriptor_t *sd = aed[index].correspond_simple_desc;
		*ptr++ = sd->endpoint;
		COPY_U16TOBUFFER(ptr,sd->app_profile_id);
		ptr += 2;
		COPY_U16TOBUFFER(ptr,sd->app_dev_id);
		ptr += 2;

		*ptr = 0;
		*ptr++ = sd->app_dev_ver&0x0f;
		*ptr++ = sd->app_in_cluster_count;

		u8 copyLen = sd->app_in_cluster_count*2;
		memcpy(ptr,sd->app_in_cluster_lst,copyLen);
		ptr += copyLen;

		*ptr++ = sd->app_out_cluster_count;
		copyLen = sd->app_out_cluster_count*2;
		memcpy(ptr,sd->app_out_cluster_lst,copyLen);
		ptr += copyLen;
		rt_len = ptr - dst;
	}
	return rt_len;
}

/*************************************************************************************
 * @brief	Copy node descriptor information
 * @param	*dst - dst address to hold node descriptor info
 * @return	none
 */

_CODE_ZDO_ void af_nodeDescriptorCopy(node_descriptor_t *dst){
	memcpy((u8 *)dst, (u8 *)&ndt, sizeof(ndt));
}

/*************************************************************************************
 * @brief	Copy power descriptor information
 * @param	*dst - dst address to hold node descriptor info
 * @return	none
 */
_CODE_ZDO_ void af_powerDescriptorCopy(power_descriptor_t *dst){
	memcpy((u8 *)dst, (u8 *)&pdt, sizeof(pdt));
}

/**************************************************************************************
 * @brief	Interface to get currently available end point numbers of the device
 *
 * @param	none
 *
 * @return	number of active end point
 */
_CODE_ZDO_ af_endpoint_descriptor_t *af_epDescriptorGet(void){
	return aed;
}


_CODE_ZDO_ af_endpoint_descriptor_t *af_zdoSimpleDescriptorGet(void ){
	return &zdo_epDesc;
}


/**************************************************************************************************
 * @brief	Interface used to get available EP number
 *
 * @param	none
 *
 * @return	ep num
 */
_CODE_ZDO_ inline u8 af_availableEpNumGet(void)
{
	return available_active_ep_num;
}
/**************************************************************************************
 * @brief	Interface to get active end point of the device according to the index
 *
 * @param	index
 *
 * @return	end point descriptor
 */
_CODE_ZDO_ u8 af_activeEpGet(u8 index){
	return aed[index].ep;
}

_CODE_ZDO_ bool af_clsuterIdMatched(u16 clusterID, af_simple_descriptor_t *pSimpleDesc){
	if(!pSimpleDesc){
		return FALSE;
	}

	for(u8 i = 0; i < pSimpleDesc->app_in_cluster_count; i++){
		if(clusterID == pSimpleDesc->app_in_cluster_lst[i]){
			return TRUE;
		}
	}

	for(u8 i = 0; i < pSimpleDesc->app_out_cluster_count; i++){
		if(clusterID == pSimpleDesc->app_out_cluster_lst[i]){
			return TRUE;
		}
	}

	return FALSE;
}

_CODE_ZDO_ bool af_profileIdMatched(u16 profileID, af_simple_descriptor_t *pSimpleDesc){
	if(!pSimpleDesc){
		return FALSE;
	}

	if((profileID == 0xFFFF) || (profileID == pSimpleDesc->app_profile_id)){
		return TRUE;
	}else{
		return FALSE;
	}
}

_CODE_ZDO_ void af_dataCnfHandler(void *arg){
	apsdeDataConf_t *pCnf = (apsdeDataConf_t *)arg;

	if(zdo_epDesc.cb_cnf && (pCnf->srcEndpoint == ZDO_EP)){
		zdo_epDesc.cb_cnf(pCnf);   //zdp for EP0
	}else if(available_active_ep_num){
		for(u8 i=0;i<available_active_ep_num;i++){
			if(aed[i].cb_cnf&&(aed[i].ep == pCnf->srcEndpoint)){
				aed[i].cb_cnf(pCnf); //other clusters
				break;
			}
		}
	}

	ev_buf_free((u8 *)arg);
}

_CODE_ZDO_ void af_dataSendCnfCb(apsdeDataConf_t *pApsDataCnf){
	apsdeDataConf_t *pCnf = (apsdeDataConf_t *)ev_buf_allocate(sizeof(apsdeDataConf_t));
	if(pCnf){
		memset((u8 *)pCnf, 0, sizeof(apsdeDataConf_t));

		memcpy((u8 *)pCnf, (u8 *)pApsDataCnf, sizeof(apsdeDataConf_t));

		TL_SCHEDULE_TASK(af_dataCnfHandler, pCnf);
	}
}

u8 af_dataSend(u8 srcEp, epInfo_t *pDstEpInfo, u16 clusterId, u16 cmdPldLen, u8 *cmdPld, u8 *apsCnt){
	u8 status = RET_OK;

	aps_data_req_t apsdeData;
	TL_SETSTRUCTCONTENT(apsdeData, 0);

	/* Build APS Data request */
	apsdeData.src_endpoint = srcEp;
	apsdeData.dst_addr_mode = pDstEpInfo->dstAddrMode;

	if((APS_SHORT_DSTADDR_WITHEP == pDstEpInfo->dstAddrMode) || (APS_LONG_DSTADDR_WITHEP == pDstEpInfo->dstAddrMode)){
		apsdeData.aps_addr.dst_endpoint = pDstEpInfo->dstEp;
		if(APS_SHORT_DSTADDR_WITHEP == pDstEpInfo->dstAddrMode){
			apsdeData.aps_addr.dst_short_addr = pDstEpInfo->dstAddr.shortAddr;
		}else{
			ZB_IEEE_ADDR_COPY(apsdeData.aps_addr.dst_ext_addr, pDstEpInfo->dstAddr.extAddr);
		}
	}else if(APS_SHORT_GROUPADDR_NOEP == pDstEpInfo->dstAddrMode){
		apsdeData.aps_addr.dst_group_addr = pDstEpInfo->dstAddr.shortAddr;
	}else{
		//APS_DSTADDR_EP_NOTPRESETNT
	}

    apsdeData.cluster_id = clusterId;
	apsdeData.profile_id = pDstEpInfo->profileId; //TODO: which place to decide the profile ID
	apsdeData.tx_options = pDstEpInfo->txOptions;
	apsdeData.radius = pDstEpInfo->radius;
	apsdeData.enableNWKsecurity = (pDstEpInfo->txOptions & APS_TX_OPT_DISABLE_NWK_KEY) ? 0 : 1;
	apsdeData.useAlias = pDstEpInfo->useAlias;
	apsdeData.aliasSrcAddr = pDstEpInfo->aliasSrcAddr;
	apsdeData.aliasSeqNum = pDstEpInfo->aliasSeqNum;
	apsdeData.handle = (apsdeData.profile_id == ZDO_PROFILE_ID && apsdeData.cluster_id == DEVICE_ANNCE_CLID) ? APS_CMD_HANDLE_DEVICE_ANNOUNCE : af_handleGet();
	apsdeData.apsCnt = aps_get_counter_value();

	*apsCnt = apsdeData.apsCnt;

	if(apsdeData.tx_options & APS_TX_OPT_FRAG_PERMITTED){
		if(aps_ib.aps_fragment_payload_size){
			if((apsdeData.dst_addr_mode == APS_SHORT_GROUPADDR_NOEP) ||
				((apsdeData.dst_addr_mode == APS_SHORT_DSTADDR_WITHEP) && ZB_NWK_IS_ADDRESS_BROADCAST(apsdeData.aps_addr.dst_short_addr))){
				/* Multicast and broadcast transmissions are not permitted to use fragmentation. */
				status = RET_ERROR;
			}else{
				if(cmdPldLen > aps_ib.aps_fragment_payload_size){
					/* Fragmentation need aps acknowledgements. */
					apsdeData.tx_options |= APS_TX_OPT_ACK_TX;
				}else{
					/* If the payload length is less then fragment payload size, clear it. */
					apsdeData.tx_options &= (u8)(~(u16)APS_TX_OPT_FRAG_PERMITTED);
				}
			}
		}else{
			/* Not support. */
			status = RET_ERROR;
		}
	}

	if(status == RET_OK){
		if(apsdeData.tx_options & APS_TX_OPT_FRAG_PERMITTED){
			/* Fragmented transmissions. */
			//TODO, not support
			status = apsDataFragmentRequest(&apsdeData, cmdPld, cmdPldLen);
		}else{
			/* Single frame request. */
			status = apsDataRequest(&apsdeData, cmdPld, (u8)cmdPldLen);
		}
	}

	return status;
}


/**********************************************************************************************************
 * @brief	Interface for rising the AF layer flag external
 *
 * @param	f: flag, see details of zdo_af_flag_enum
 */
_CODE_ZDO_ inline void zdo_af_flag_rise(zdo_af_flag_enum f){
	app_framework_flag |= f;
}


/**********************************************************************************************************
 * @brief	Interface for clear the AF layer flag external
 *
 * @param	f: flag, see details of zdo_af_flag_enum
 */
_CODE_ZDO_ inline void zdo_af_flag_clr(zdo_af_flag_enum f){
	app_framework_flag &= (~f);
}

/**********************************************************************************************************
 * @brief	Interface to check whether the flag has set or not
 *
 * @param	f: flag, see details of zdo_af_flag_enum
 *
 * @return	TRUE or False
 */
_CODE_ZDO_ inline bool zdo_af_check_flag(zdo_af_flag_enum f){

	return (bool)(app_framework_flag & f);

}

/****************************************************************************************************
 * @brief	Theb:Config_Parent_Link_Retry_Thres hold is either created when the  application is first loaded or
			initialized with a commissioning tool. It is used for the ZED to decide how many times it should
			retry to connect to the parent router before initiating the rejoin process.
 *
 * @param	none
 *
 * @return	max retry number
 */
_CODE_ZDO_ void zdo_af_set_link_retry_threshold(u8 threshold){
	zdo_cfg_attributes.config_parent_link_retry_threshold = threshold;
}

_CODE_ZDO_ u8 zdo_af_get_link_retry_threshold(void){
	return zdo_cfg_attributes.config_parent_link_retry_threshold;
}


/****************************************************************************************************
 * @brief	Interface to get the NWK indirect poll rate parameter, The value for this configuration attribute
 * 			is established by the application profile deployed on the device.
 *
 * @param	none
 *
 * @return	poll rate in superframe
 */
_CODE_ZDO_ inline u32 zdo_af_get_syn_rate(void){
	return zdo_cfg_attributes.config_nwk_indirectPollRate;
}

_CODE_ZDO_ inline void zdo_af_set_syn_rate(u32 newRate){
	zdo_cfg_attributes.config_nwk_indirectPollRate = newRate;
}

/****************************************************************************************************
 * @brief	The :Config_Rejoin_Interval is either created when the application is first loaded or initialized with a
 *			commissioning tool. It is used by the ZED to decide how often it should initiate the rejoin process.
 *
 * @param	none
 *
 * @return	rejoin interval
 */
_CODE_ZDO_ inline void zdo_af_set_rejoin_interval(u16 interval){
	zdo_cfg_attributes.config_rejoin_interval = interval;
}

_CODE_ZDO_ inline void zdo_af_set_max_rejoin_interval(u16 interval){
	zdo_cfg_attributes.config_max_rejoin_interval = interval;
}

_CODE_ZDO_ inline u16 zdo_af_get_rejoin_interval(void){
	return zdo_cfg_attributes.config_rejoin_interval;
}

_CODE_ZDO_ inline u16 zdo_af_get_max_rejoin_interval(void){
	return zdo_cfg_attributes.config_max_rejoin_interval;
}

/****************************************************************************************************
 * @brief	The :Config_NWK_Scan_Attempts is employed within ZDO to call the NLME-NETWORKDISCOVERY. request
 * 			primitive the indicated number of times (for routers and end devices). The attribute has
 * 			default value of 5 and valid values between 1 and 255
 *
 * @param	none
 *
 * @return	scan attempts
 */
_CODE_ZDO_ inline u8 zdo_af_get_scan_attempts(void){
	return zdo_cfg_attributes.config_nwk_scan_attempts;
}

_CODE_ZDO_ inline void zdo_af_set_scan_attempts(u8 attempts){
	zdo_cfg_attributes.config_nwk_scan_attempts = attempts;
}

/**********************************************************************************************************
 * @brief	Interface to get scan duration attribute
 *
 * @param	none
 *
 * @return	value in superframe
 */
_CODE_ZDO_ inline u16 zdo_af_get_nwk_time_btwn_scans(void){
	return zdo_cfg_attributes.config_nwk_time_btwn_scans;
}


/**********************************************************************************************************
 * @brief	The default value for :Config_Permit_Join_Duration is 0x00, however, this value can be  established
 * 			differently according  to the needs of the profile.
 *
 * @param	none
 *
 * @return	value
 */
_CODE_ZDO_ inline u8 zdo_af_get_permit_join_duration(void){
	return zdo_cfg_attributes.config_permit_join_duration;
}

