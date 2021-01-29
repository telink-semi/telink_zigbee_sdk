/********************************************************************************************************
 * @file	zb_af.c
 *
 * @brief	This is the source file for zb_af
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
	.logical_type =					LOGICAL_TYPE,
	.server_mask = 					SERVER_MASK,
	.mac_capability_flag = 			MAC_CAPABILITY_FLAGS,
	.complex_descriptor_ava = 		FALSE,
	.user_descriptor_ava = 			FALSE,
	.reserved = 					0,
	.aps_flag = 					0,
	.freq_band = 					NODE_FREQ_2400,
	.max_buff_size = 				AF_NSDU_MAX_LEN,
	.max_in_tr_size = 				AF_NSDU_MAX_LEN,
	.max_out_tr_size = 				AF_NSDU_MAX_LEN,
	.mcL8 =   						(MANUFACTURER_CODE_TELINK & 0xff),
	.mcH8 =   						((MANUFACTURER_CODE_TELINK >> 8) & 0xff),
};

//power descriptor strut
static power_descriptor_t pdt = {
#if defined (ZB_ROUTER_ROLE) && (ZB_ROUTER_ROLE == TRUE)
	.current_power_mode = 			POWER_MODE_RECEIVER_SYNCRONIZED_WHEN_ON_IDLE,
	.available_power_sources = 		POWER_SRC_MAINS_POWER,
	.current_power_source = 		POWER_SRC_MAINS_POWER,
	.current_power_source_level = 	POWER_LEVEL_PERCENT_100,
#elif defined (ZB_ED_ROLE) && (ZB_ED_ROLE == TRUE)
	.current_power_mode = 			POWER_MODE_RECEIVER_COMES_PERIODICALLY,
	.available_power_sources = 		POWER_SRC_RECHARGEABLE_BATTERY,
	.current_power_source = 		POWER_SRC_RECHARGEABLE_BATTERY,
	.current_power_source_level = 	POWER_LEVEL_PERCENT_66,
#endif
};




//AF endpoint descriptor strut
static af_endpoint_descriptor_t aed[MAX_ACTIVE_EP_NUMBER];
static af_endpoint_descriptor_t zdo_epDesc;

//The number of available active endpoint numbers, used for simple descriptor and active ep response
static u8 available_active_ep_num = 0;



/**************************************************************************************************
 * @brief	APP framework layer initialization
 *
 * @param	none
 *
 * @return	none
 */
_CODE_AF_ void af_init(void){
	available_active_ep_num = 0;

	memset((u8 *)aed, 0, sizeof(aed));
	memset((u8 *)&zdo_epDesc, 0, sizeof(zdo_epDesc));
}

/****************************************************************************************************
 * @brief	AF layer power descriptor set interface
 *
 * @param	value: power_descriptor_t
 *
 * @return	none
 */
_CODE_AF_ void af_powerDescriptorSet(power_descriptor_t *value){
	memcpy((u8 *)&pdt, value, sizeof(*value));
}

/****************************************************************************************************
 * @brief	AF layer power descriptor (powerMode) set interface
 *
 * @param	mode: power mode
 *
 * @return	none
 */
_CODE_AF_ void af_powerDescPowerModeUpdate(power_mode_t mode){
#if defined (ZB_ED_ROLE) && (ZB_ED_ROLE == TRUE)
	pdt.current_power_mode = mode;
#endif
}

/****************************************************************************************************
 * @brief	AF layer node descriptor set interface
 *
 * @param	value: node_descriptor_t
 *
 * @return	none
 */
_CODE_AF_ void af_nodeDescriptorSet(node_descriptor_t *value){
	memcpy((u8 *)&ndt, value, sizeof(*value));
}

/****************************************************************************************************
 * @brief	AF layer node descriptor (manuCode) set interface
 *
 * @param	manuCode: manufacturer code
 *
 * @return	none
 */
_CODE_AF_ void af_nodeDescManuCodeUpdate(u16 manuCode){
	ndt.mcL8 = LO_UINT16(manuCode);
	ndt.mcH8 = HI_UINT16(manuCode);
}

/****************************************************************************************************
 * @brief	AF layer node descriptor (rxOnWhenIdle) set interface
 *
 * @param	enable
 *
 * @return	none
 */
_CODE_AF_ void af_nodeDescRxOnWhenIdleUpdate(bool enable){
	if(enable){
		ndt.mac_capability_flag |= BIT(3);
	}else{
		ndt.mac_capability_flag &= ~BIT(3);
	}
}

/****************************************************************************************************
 * @brief	AF layer node descriptor (stackVer) set interface
 *
 * @param	revision
 *
 * @return	none
 */
_CODE_AF_ void af_nodeDescStackRevisionSet(u8 revision){
	ndt.server_mask &= (~(0x7f << 9));
	ndt.server_mask |= (revision << 9);
}

/****************************************************************************************************
 * @brief	AF layer node descriptor (stackVer) get interface
 *
 * @param	none
 *
 * @return	revision
 */
_CODE_AF_ u8 af_nodeDescStackRevisionGet(void){
	return ((u8)((ndt.server_mask & 0xfe00) >> 9));
}

/****************************************************************************************************
 * @brief	AF layer node descriptor (macCapability) get interface
 *
 * @param	none
 *
 * @return	revision
 */
_CODE_AF_ u8 af_nodeMacCapabilityGet(void){
	return ndt.mac_capability_flag;
}

/****************************************************************************************************
 * @brief	AF layer node descriptor (deviceType) get interface
 *
 * @param	none
 *
 * @return	device_type_t
 */
_CODE_AF_ device_type_t af_nodeDevTypeGet(void){
	return ndt.logical_type;
}

/****************************************************************************************************
 * @brief	Copy simple descriptor interface used building simple descriptor rsp message
 *
 * @param	*dst - destination address to store simple descriptor info
 * 			*sd  - local simple descriptor
 *
 * @return	length of the simple descriptor info
 */
_CODE_AF_ u8 af_simpleDescriptorCopy(u8 *dst, af_simple_descriptor_t *sd){
	u8 *ptr = dst;

	*ptr++ = sd->endpoint;
	COPY_U16TOBUFFER(ptr, sd->app_profile_id);
	ptr += 2;
	COPY_U16TOBUFFER(ptr, sd->app_dev_id);
	ptr += 2;

	*ptr = 0;
	*ptr++ = sd->app_dev_ver & 0x0f;
	*ptr++ = sd->app_in_cluster_count;

	u8 copyLen = sd->app_in_cluster_count * 2;
	memcpy(ptr, sd->app_in_cluster_lst, copyLen);
	ptr += copyLen;

	*ptr++ = sd->app_out_cluster_count;
	copyLen = sd->app_out_cluster_count * 2;
	memcpy(ptr, sd->app_out_cluster_lst, copyLen);
	ptr += copyLen;

	return (u8)(ptr - dst);
}

/****************************************************************************************************
 * @brief	Copy node descriptor information
 *
 * @param	*dst - dst address to hold node descriptor info
 *
 * @return	none
 */
_CODE_AF_ void af_nodeDescriptorCopy(node_descriptor_t *dst){
	memcpy((u8 *)dst, (u8 *)&ndt, sizeof(ndt));
}

/****************************************************************************************************
 * @brief	Copy power descriptor information
 *
 * @param	*dst - dst address to hold node descriptor info
 *
 * @return	none
 */
_CODE_AF_ void af_powerDescriptorCopy(power_descriptor_t *dst){
	memcpy((u8 *)dst, (u8 *)&pdt, sizeof(pdt));
}





/**************************************************************************************************
 * @brief	Interface used to get available EP number
 *
 * @param	none
 *
 * @return	available EP
 */
_CODE_AF_ u8 af_availableEpNumGet(void){
	return available_active_ep_num;
}

/**************************************************************************************************
 * @brief	Interface to get the af endpoint descriptor
 *
 * @param	none
 *
 * @return	the point to the af endpoint descriptor
 */
_CODE_AF_ af_endpoint_descriptor_t *af_epDescriptorGet(void){
	return aed;
}

/**************************************************************************************************
 * @brief	Interface to get ZDO simple descriptor
 *
 * @param	none
 *
 * @return	the point to the ZDO simple descriptor
 */
_CODE_AF_ af_endpoint_descriptor_t *af_zdoSimpleDescriptorGet(void){
	return &zdo_epDesc;
}

/**************************************************************************************************
 * @brief	Get simple descriptor according to the endpoint
 *
 * @param	ep - end point to compare
 *
 * @return	point to the simple descriptor info
 */
_CODE_AF_ af_simple_descriptor_t *af_simpleDescGet(u8 ep){
	for(u8 i = 0; i < available_active_ep_num; i++){
		if(aed[i].ep == ep){
			return aed[i].correspond_simple_desc;
		}
	}

	return NULL;
}

_CODE_AF_ bool af_profileMatchedLocal(u16 profileID, u8 ep){
	if((ep == ZDO_EP) && (profileID == ZDO_PROFILE_ID)){
		return TRUE;
	}

	if((ep != ZDO_EP) && (profileID == LL_PROFILE_ID)){
		return TRUE;
	}

	for(u8 i = 0; i < available_active_ep_num; i++){
		if(((aed[i].ep == ep) || (ep == 0xFF)) &&
			((aed[i].correspond_simple_desc->app_profile_id == profileID) || (profileID == WILDCARD_PROFILE_ID))){
			return TRUE;
		}
	}

	return FALSE;
}

_CODE_AF_ bool af_clusterMatchedLocal(u16 clusterID, u8 ep){
	for(u8 i = 0; i < available_active_ep_num; i++){
		if(aed[i].ep == ep){
			for(u8 j = 0; j < aed[i].correspond_simple_desc->app_in_cluster_count; j++){
				if(aed[i].correspond_simple_desc->app_in_cluster_lst[j] == clusterID){
					return TRUE;
				}
			}

			for(u8 j = 0; j < aed[i].correspond_simple_desc->app_out_cluster_count; j++){
				if(aed[i].correspond_simple_desc->app_out_cluster_lst[j] == clusterID){
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

_CODE_AF_ bool af_clsuterIdMatched(u16 clusterID, af_simple_descriptor_t *pSimpleDesc){
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

_CODE_AF_ bool af_profileIdMatched(u16 profileID, af_simple_descriptor_t *pSimpleDesc){
	if(!pSimpleDesc){
		return FALSE;
	}

	if((profileID == 0xFFFF) || (profileID == pSimpleDesc->app_profile_id)){
		return TRUE;
	}else{
		return FALSE;
	}
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
_CODE_AF_ bool af_endpointRegister(u8 ep, af_simple_descriptor_t *simple_desc, af_endpoint_cb_t rx_cb, af_dataCnf_cb_t cnfCb)
{
	if(ep == ZDO_EP){
		zdo_epDesc.ep = ep;
		zdo_epDesc.correspond_simple_desc = simple_desc;
		zdo_epDesc.cb_cnf = cnfCb;
		zdo_epDesc.cb_rx = rx_cb;
		return TRUE;
	}

	for(u8 i = 0; i < MAX_ACTIVE_EP_NUMBER; i++){
		if(aed[i].ep == ep){
			return FALSE;
		}
	}

	if(available_active_ep_num < MAX_ACTIVE_EP_NUMBER){
		aed[available_active_ep_num].ep = ep;
		aed[available_active_ep_num].correspond_simple_desc = simple_desc;
		aed[available_active_ep_num].cb_cnf = cnfCb;
		aed[available_active_ep_num++].cb_rx = rx_cb;
	}else{
		return FALSE;
	}
	return TRUE;
}

_CODE_AF_ bool af_endpointUnregister(u8 ep){
	for(u8 i = 0; i < available_active_ep_num; i++){
		if(aed[i].ep == ep){
			TL_SETSTRUCTCONTENT(aed[i], 0);
			if((i + 1) != available_active_ep_num){
				memcpy((u8 *)&aed[i], (u8 *)&aed[i + 1], sizeof(aed[i]) * (available_active_ep_num - i));
			}
			available_active_ep_num--;
			return TRUE;
		}
	}
	return FALSE;
}

