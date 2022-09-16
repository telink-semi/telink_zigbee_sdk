/********************************************************************************************************
 * @file    zb_api.h
 *
 * @brief   This is the header file for zb_api
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#ifndef ZB_API_H
#define ZB_API_H

/**********************************************************************
 * INCLUDES
 */
#include "zb_common.h"


/**        
 *  @brief Structure for parameter of apsdeDataInd callback function
 */
typedef struct apsdeDataInd_s{
	aps_data_ind_t indInfo;
    u16 asduLen;
    u8  asdu[];
}apsdeDataInd_t;


static inline void zb_resetDevice(void){
	SYSTEM_RESET();
}

/***********************************************************************//**
 * @brief       Factory reset.
 * 				This interface should be called by Router or End-Device.
 * 				If it is a not factory new device, it will broadcast
 * 				a Leave Command before factory new reset.
 *
 * @param       None
 *
 * @return      None
 *
 **************************************************************************/
void zb_factoryReset(void);

/***********************************************************************//**
 * @brief       Reset device to factory new.
 * 				This interface could be called by Coordinator, Router or End-Device.
 * 				If coordinator:
 * 					It will erase all NV information and reset all layer settings.
 * 				if router or end device:
 * 					If it is a not factory new device, it will broadcast
 * 					a Leave Command before factory new reset.
 *
 * @param       None
 *
 * @return      None
 *
 **************************************************************************/
void zb_resetDevice2FN(void);

/***********************************************************************//**
 * @brief       This interface is used to obtain whether the device is factory new.
 *
 * @param       None
 *
 * @return      TRUE or FALSE
 *
 **************************************************************************/
bool zb_isDeviceFactoryNew(void);

/***********************************************************************//**
 * @brief   	Factory New device Set
 *
 * @param[in]  	new - 1: set as factory new device, 0: set as non-factory new device
 *
 * @return  	None
 *
 **************************************************************************/
void zb_deviceFactoryNewSet(bool new);

/***********************************************************************//**
 * @brief       This interface is used to obtain whether the device has joined a network.
 *
 * @param       None
 *
 * @return      TRUE or FALSE
 *
 **************************************************************************/
bool zb_isDeviceJoinedNwk(void);

/***********************************************************************//**
 * @brief       Get MAC association permit status (only for ZC ZR)
 *
 * @param		None
 *
 * @return      Status
 *
 **************************************************************************/
bool zb_getMacAssocPermit(void);

/***********************************************************************//**
 * @brief   	Enable or Disable AODV
 *
 * @param[in]  	enable - Enable or Disable
 *
 * @return  	None
 *
 **************************************************************************/
void zb_nldeDataDiscoverRouteSet(bool enable);

/***********************************************************************//**
 * @brief       Get AODV enabled or disabled status.
 *
 * @param       None
 *
 * @return  	TRUE or FALSE
 *
 **************************************************************************/
bool zb_nldeDataDiscoverRouteGet(void);

/***********************************************************************//**
 * @brief       Set extend PAN ID to APS_IB.
 *
 * @param[in]   panId - extend PAN ID
 *
 * @return      None
 *
 **************************************************************************/
void zb_apsExtPanidSet(extPANId_t panId);

/***********************************************************************//**
 * @brief       Set channel mask to APS_IB.
 *
 * @param[in]   mask:  channel mask
 *
 * @return      Status
 *
 **************************************************************************/
aps_status_t zb_apsChannelMaskSet(u32 mask);

/***********************************************************************//**
 * @brief       Get channel mask from APS_IB.
 *
 * @param		None
 *
 * @return  	channel mask
 *
 **************************************************************************/
u32 zb_apsChannelMaskGet(void);

/***********************************************************************//**
 * @brief       Get device type
 *
 * @param       None
 *
 * @return      device type (nwk_deviceType_t)
 * 				0 - Coordinator
 * 				1 - Router
 * 				2 - EndDevice
 *
 **************************************************************************/
device_type_t zb_getDeviceType(void);

/***********************************************************************//**
 * @brief       Get local IEEE address
 *
 * @param[in]   extAddr
 *
 * @return      None
 *
 **************************************************************************/
void zb_getLocalExtAddr(addrExt_t extAddr);

/***********************************************************************//**
 * @brief       Get local short address
 *
 * @param       None
 *
 * @return      2 bytes of short address
 *
 **************************************************************************/
u16 zb_getLocalShortAddr(void);

/***********************************************************************//**
 * @brief       Get parent short address
 *
 * @param       None
 *
 * @return      2 bytes of parent's short address
 *
 **************************************************************************/
u16 zb_getParentShortAddr(void);

/***********************************************************************//**
 * @brief       Get network short address according to the EXT address
 *
 * @param[in]   extAddr
 * @param[out]  nwkAddr
 *
 * @return      SUCCESS - 0; FAILED - 0xFF
 *
 **************************************************************************/
u8 zb_getNwkAddrByExtAddr(addrExt_t extAddr, u16 *nwkAddr);

/***********************************************************************//**
 * @brief       Get EXT address according to the network short address
 *
 * @param[in]   nwkAddr
 * @param[out]  extAddr
 *
 * @return      SUCCESS - 0; FAILED - 0xFF
 *
 **************************************************************************/
u8 zb_getExtAddrByNwkAddr(u16 nwkAddr, addrExt_t extAddr);

/***********************************************************************//**
 * @brief       Set the period time of the network layer link status command.
 * 				Only for Coordinator and Router.
 *
 * @param[in]   periodInSec - Time between two link status commands (in seconds)
 *
 * @return      None
 *
 **************************************************************************/
void zb_nlmeSetLinkStsPeriod(u8 periodInSec);

/***********************************************************************//**
 * @brief      	Send data request manually, only for End-Device.
 *
 * @param		None
 *
 * @return		RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_endDeviceSyncReq(void);

/***********************************************************************//**
 * @brief      	Set poll rate, only for End-Device.
 *
 * @param[in]	newRate - in millisecond
 *
 * @return		RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_setPollRate(u32 newRate);

/***********************************************************************//**
 * @brief		Get poll rate, only for End-Device.
 *
 * @param
 *
 * @return		in millisecond
 *
 **************************************************************************/
u32 zb_getPollRate(void);

/***********************************************************************//**
 * @brief       Perform network formation request.
 * 				Only for Coordinator and Router.
 *
 * @param[in]	scanChannels
 * @param[in]	scanDuration
 *
 * @return      RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_nwkFormation(u32 scanChannels, u8 scanDuration);

/***********************************************************************//**
 * @brief   	Perform router start to recover network.
 * 				Only for Coordinator and Router.
 *
 * @param   	None
 *
 * @return		RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_routerStart(void);

/***********************************************************************//**
 * @brief       Perform network discovery request.
 * 				Only for Router and End Device.
 *
 * @param[in]   scanChannels
 * @param[in]   scanDuration
 * @param[in]   cb
 *
 * @return      RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_nwkDiscovery(u32 scanChannels, u8 scanDuration, nwkDiscoveryUserCb_t cb);

/***********************************************************************//**
 * @brief       Cancel network discovery request.
 * 				Only for Router and End Device.
 *
 * @param[in]   None
 *
 * @return      None
 *
 **************************************************************************/
void zb_nwkDiscoveryStop(void);

/***********************************************************************//**
 * @brief       Perform associate join request.
 * 				Only for Router and End Device.
 *
 * @param[in]   none
 *
 * @return      RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_assocJoinReq(void);

/***********************************************************************//**
 * @brief       Perform rejoin request.
 * 				Only for Router and End Device.
 *
 * @param[in]   scanChannels
 * @param[in]   scanDuration
 *
 * @return		RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_rejoinReq(u32 scanChannels, u8 scanDuration);

/***********************************************************************//**
 * @brief       Perform rejoin request with backoff timer.
 * 				Only for Router and End Device.
 * 				NOTE: Please refer to the related definition of
 * 					  zdo_cfg_attributes for the parameter configuration
 * 					  of backoff, in zdo_api.h
 *
 * @param[in]   scanChannels
 * @param[in]   scanDuration
 *
 * @return		RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_rejoinReqWithBackOff(u32 scanChannels, u8 scanDuration);

/***********************************************************************//**
 * @brief       Rejoin with security or insecurity mode.
 *
 * @param[in]   mode: REJOIN_INSECURITY or REJOIN_SECURITY
 *
 * @return		None
 *
 **************************************************************************/
void zb_rejoinSecModeSet(u8 mode);

/***********************************************************************//**
 * @brief       Perform direct join request.
 * 				Only for Router and End Device.
 *
 * @param[in]   scanChannels
 * @param[in]   scanDuration
 *
 * @return		RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_directJoinReq(u32 scanChannels, u8 scanDuration);

/***********************************************************************//**
 * @brief   	Accept a remote device as a child node,
 * 				who will join the network through direct join mode.
 * 				Only for Coordinator and Router.
 *
 * @param[in]   req - the remote device information
 *
 * @return  	RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_nwkDirectJoinAccept(nlme_directJoin_req_t *pReq);

/***********************************************************************//**
 * @brief       Set local permit join duration.
 * 				Only for Coordinator and Router.
 *
 * @param[in]   permitDuration - The time allowed for new device to join (in seconds).
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_nlmePermitJoiningRequest(u8 permitDuration);

/***********************************************************************//**
 * @brief       Perform route discovery request.
 * 				Only for Coordinator and Router.
 *
 * @param[in]   pRouteDiscReq
 *
 * @return      RET_ILLEGAL_REQUEST or zdo_status_t
 *
 **************************************************************************/
u8 zb_routeDiscReq(nlme_routeDisc_req_t *pRouteDiscReq);

/***********************************************************************//**
 * @brief       Perform leave request.
 *
 * @param[in]   pLeaveReq
 *
 * @return      zdo_status_t
 *
 **************************************************************************/
u8 zb_nlmeLeaveReq(nlme_leave_req_t *pLeaveReq);

/***********************************************************************//**
 * @brief       Send address request to target device for short address.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq    	- parameter of network address request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- network address response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoNwkAddrReq(u16 dstNwkAddr, zdo_nwk_addr_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send address request to target device for IEEE address.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq    	- parameter of IEEE address request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- IEEE address response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoIeeeAddrReq(u16 dstNwkAddr, zdo_ieee_addr_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send simple descriptor request.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq        - parameter of simple descriptor request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- simple descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoSimpleDescReq(u16 dstNwkAddr, zdo_simple_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send node descriptor request.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq        - parameter of node descriptor request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- node descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoNodeDescReq(u16 dstNwkAddr, zdo_node_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send power descriptor request.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq        - parameter of power descriptor request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- power descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoPowerDescReq(u16 dstNwkAddr, zdo_power_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send active endpoint request.
 *
 * @param[in]	dstNwkAddr	- this command will be send to
 *  			pReq        - parameter of active endpoint request
 *  			seqNo		- the sequence number used by this command
 * 				indCb		- active endpoint response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoActiveEpReq(u16 dstNwkAddr, zdo_active_ep_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send match descriptor request.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq        - parameter of match descriptor request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- match descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoMatchDescReq(u16 dstNwkAddr, zdo_match_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send complex descriptor request.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq        - parameter of complex descriptor request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- complex descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoComplexDescReq(u16 dstNwkAddr, zdo_complex_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send user descriptor request.
 *
 * @param[in]   dstNwkAddr	- this command will be send to
 * 				pReq        - parameter of user descriptor request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- user escriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoUserDescReq(u16 dstNwkAddr, zdo_user_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send zdo system server discovery request, broadcasting to 0xfffd
 *
 ** @param[in]  serverMask  - server mask bit assignment
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- system server discovery response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoSystemServerDiscoveryReq(u16 serverMask, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send bind request, destination address is based on the srcAddr in the request command
 *
 * @param[in]   isBinding	- 1:bind, 0:unbind
 * 				pReq        - parameter of bind/unbind request
 *  			seqNo		- the sequence number used by this command
 * 				indCb		- bind/unbind response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoBindUnbindReq(bool isBinding, zdo_bind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send end device bind request.
 *
 * @param[in]   pReq        - parameter of end device bind request
 * 				seqNo		- the sequence number used by this command
 * 				indCb		- end device bind response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoEndDeviceBindReq(zdo_edBindReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send the command of mgmt_permitJoin_req.
 *
 * @param[in]   dstNwkAddr            - Short address of the target device
 * @param[in]   permitJoinDuration    - Time in seconds during which the device allows to join
 * @param[in]   tcSignificance        - tc significance
 * @param[in]   seqNo				  - the sequence number used by this command
 * @param[in]   indCb				  - mgmtPermitJoin.confirm callback
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtPermitJoinReq(u16 dstNwkAddr, u8 permitJoinDuration, u8 tcSignificance, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief	Send the command of mgmt_leave_req.
 *
 * @param[in]	dstNwkAddr:	- target address to receive this cmd
 * @param[in]   pReq        - parameter of mgmt_leave_req
 * @param[in]   seqNo  		- the sequence number used by this command
 * @param[in]   indCb		- the indicate callback for mgmt_leave_rsp
 *
 * @return		Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtLeaveReq(u16 dstNwkAddr, zdo_mgmt_leave_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send the command of mgmt_network_update_req.
 *
 * @param[in]	dstNwkAddr	- destination address of the request
 * @param[in]   pReq        - parameter of mgmt_network_update_req
 * @param[in]   seqNo		- the sequence number used by this command
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtNwkUpdateReq(u16 dstNwkAddr, zdo_mgmt_nwk_update_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send the command of mgmt_lqi_req.
 *
 * @param[in]	dstNwkAddr	- destination address of the request
 * @param[in]   pReq        - parameter of mgmt_lqi_req
 * @param[in]   seqNo	 	- the sequence number used by this command
 * @param[in]   indCb		- the indicate callback for mgmt_lqi_rsp
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtLqiReq(u16 dstNwkAddr, zdo_mgmt_lqi_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send the command mgmt_bind_req.
 *
 * @param[in]	dstNwkAddr	- destination address of the request
 * @param[in]   pReq        - parameter of mgmt_bind_req
 * param[in]    seqNo	 	- the sequence number used by this command
 * @param[in]   indCb		- the indicate callback for mgmt_bind_rsp
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtBindReq(u16 dstNwkAddr, zdo_mgmt_bind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief		Remove device request command.
 *
 * @param[in]	pRemoveDevReq
 *
 * @return		Status
 *
 **************************************************************************/
u8 zb_apsmeRemoveDevReq(ss_apsmeRemoveDeviceReq_t *pRemoveDevReq);

/***********************************************************************//**
 * @brief		APSME request key command.
 *
 * @param[in]	pRequestKeyReq
 *
 * @return		Status
 *
 **************************************************************************/
u8 zb_apsmeRequestKeyReq(ss_apsmeRequestKeyReq_t *pRequestKeyReq);

/***********************************************************************//**
 * @brief		APSME transport key command.
 *
 * @param[in]	pTransportKeyReq
 *
 * @return		Status
 *
 **************************************************************************/
u8 zb_apsmeTransportKeyReq(ss_apsmeTransportKeyReq_t *pTransportKeyReq);

/***********************************************************************//**
 * @brief		APSME switch key command.
 *
 * @param[in]	pSwitchKeyReq
 *
 * @return		Status
 *
 **************************************************************************/
u8 zb_apsmeSwitchKeyReq(ss_apsmeSwitchKeyReq_t *pSwitchKeyReq);

/***********************************************************************//**
 * @brief		API for Trust center to update NWK key.
 *
 * @param[in]	pTcUpdateNwkKey
 * 				dstAddr all zero means boradcast.
 *
 * @return		Status
 *
 **************************************************************************/
u8 zb_tcUpdateNwkKey(ss_tcUpdateNwkKey_t *pTcUpdateNwkKey);

/***********************************************************************//**
 * @brief       Search binding table.
 *
 * @param[in]   clusterID
 * @param[in]   srcEp
 *
 * @return      TRUE or FALSE
 *
 **************************************************************************/
bool zb_bindingTblSearched(u16 clusterID, u8 srcEp);

/***********************************************************************//**
 * @brief       Send device announce.
 *
 * @param       None
 *
 * @return      RET_OK or not
 *
 **************************************************************************/
u8 zb_zdoSendDevAnnance(void);

/***********************************************************************//**
 * @brief       Send parent announce.
 *
 * @param       None
 *
 * @return      Status
 *
 **************************************************************************/
void zb_zdoSendParentAnnce(void);

/***********************************************************************//**
 * @brief       Register MAC layer callback function.

 * @param[in]   cb      	- Callback functions
 *
 * @return      None
 *
 **************************************************************************/
void zb_macCbRegister(mac_appIndCb_t *cb);

/***********************************************************************//**
 * @brief       Register ZDO layer callback function.

 * @param[in]   cb      	- Callback functions
 *
 * @return      None
 *
 **************************************************************************/
void zb_zdoCbRegister(zdo_appIndCb_t *cb);

/***********************************************************************//**
 * @brief       Force join a fixed network.
 *
 * @param[in]   channel		- operation channel
 * @param[in]   panId		- the network panID
 * @param[in]   shortAddr	- network address allocated by itself
 * @param[in]   extPanId	- external panID of the network
 * @param[in]   nwkKey		- the network key of the network
 * @param[in]   tcAddr		- the trust center's address of the network, only for central network, normally it's same with extPanId
 *                            NULL for distribute network
 *
 * @return      None
 *
 **************************************************************************/
void zb_joinAFixedNetwork(u8 channel, u16 panId, u16 shortAddr, u8 *extPanId, u8 *nwkKey, u8 *tcAddr);

/***********************************************************************//**
 * @brief       Extend PAN ID rejoin. Only for the factory new device.
 *
 * @param[in]   extPanId	- extend PAN ID
 *
 * @return      None
 *
 **************************************************************************/
void zb_extPanIdRejoin(extPANId_t extPanId);

/***********************************************************************//**
 * @brief       Pre-configure network key. Only for the factory new device.
 * 				Called after the bdb_init() function.
 *
 * @param[in]	nwkKey      - the pre-configured network key
 * @param[in]	enTransKey	- 0: join network once receiving associate response
 * 							  1: join network follow the standard flow (need process transport key)
 *
 * @return      None
 *
 **************************************************************************/
void zb_preConfigNwkKey(u8 *nwkKey, bool enTransKey);

/***********************************************************************//**
 * @brief		Shift to and save the logic channel.
 *
 * @param[in]	ch			- 11 ~ 26
 *
 * @return	none
 *
 **************************************************************************/
void zdo_nlmeChannelShift(u8 ch);


#endif	/* ZB_API_H */
