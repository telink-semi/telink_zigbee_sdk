/********************************************************************************************************
 * @file     zb_api.h
 *
 * @brief    Telink Stack API Interfaces
 *
 * @author
 * @date     Dec. 1, 2016
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
 * 				0 - EndDevice
 * 				1 - Router
 * 				2 - Coordinator
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
 * @return		None
 *
 **************************************************************************/
void zb_endDeviceSyncReq(void);

/***********************************************************************//**
 * @brief      	Set poll rate, only for End-Device.
 *
 * @param[in]	newRate - in millisecond
 *
 * @return		None
 *
 **************************************************************************/
void zb_setPollRate(u32 newRate);

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
 *
 * @param[in]	scanChannels
 * @param[in]	scanDuration
 *
 * @return      RET_OK or not
 *
 **************************************************************************/
u8 zb_nwkFormation(u32 scanChannels, u8 scanDuration);

/***********************************************************************//**
 * @brief       Perform network discovery request.
 *
 * @param[in]   pReq
 * @param[in]   state
 *
 * @return      RET_OK or not
 *
 **************************************************************************/
u8 zb_nwkDiscReq(nlme_nwkDisc_req_t *pReq, nlme_state_t state);

/***********************************************************************//**
 * @brief   	Perform router start.
 *
 * @param   	None
 *
 * @return		None
 *
 **************************************************************************/
void zb_routerStart(void);

/***********************************************************************//**
 * @brief       Set rejoin mode.
 *
 * @param[in]   mode: REJOIN_INSECURITY or REJOIN_SECURITY
 *
 * @return		None
 *
 **************************************************************************/
void zb_rejoin_mode_set(u8 mode);

/***********************************************************************//**
 * @brief       Perform rejoin request.
 *
 * @param[in]   method
 * @param[in]   chm
 *
 * @return		status
 *
 **************************************************************************/
zdo_status_t zb_rejoinReq(rejoinNwk_method_t method, u32 chm);

/***********************************************************************//**
 * @brief   	Perform direct join request.
 *
 * @param[in]   req
 *
 * @return  	RET_OK or not
 *
 **************************************************************************/
u8 zb_nwkDirectJoin(nlme_directJoin_req_t req);

/***********************************************************************//**
 * @brief       Send a mac layer scan request.
 *
 * @param[in]   scanChannels - scan channels
 *
 * @return      RET_OK or not
 *
 **************************************************************************/
u8 zb_mlmeScanRequest(u32 scanChannels);

/***********************************************************************//**
 * @brief       Send a network layer associate join request.
 *
 * @param[in]   channel  - channel to join
 * @param[in]   extPanId - extended pan id to join
 *
 * @return      RET_OK or not
 *
 **************************************************************************/
u8 zb_nwkAssociateJoinRequest(u8 channel, extPANId_t extPanId);

/***********************************************************************//**
 * @brief       Perform a leave request.
 *
 * @param[in]   req
 *
 * @return      RET_OK or not
 *
 **************************************************************************/
u8 zb_nlmeLeaveReq(nlme_leave_req_t *req);

/***********************************************************************//**
 * @brief       Perform permit join request to local device.
 * 				Only for Coordinator and Router.
 *
 * @param[in]   permitDuration - The time allowed for new device to join (in seconds).
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_nlmePermitJoiningRequest(u8 permitDuration);

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
zdo_status_t zb_mgmtNwkUpdateReq(u16 dstNwkAddr, zdo_mgmt_nwk_update_req_t *pReq, u8 *seqNo);

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
 * @param[in]	parentAddr - parent address of the device to be removed (64-bit)
 * @param[in]	devExt     - address of the device to be removed
 *
 * @return		Status
 *
 **************************************************************************/
aps_status_t zb_apsmeRemoveDevReq(addrExt_t parentAddr, addrExt_t devExt);

/***********************************************************************//**
 * @brief		APSME request key command.
 *
 * @param[in]	keyType 	- the type of key being requested
 * @param[in]	dstAddr     - the extend address of the device which the request-key command should be sent
 * @param[in]	partnerAddr - if the key type is SS_KEYREQ_TYPE_APPLK, this parameter shall indicate an
 * 							  extend 64-bit address of a device that shall receive the same key as the
 * 							  device requesting the key
 *
 * @return		Status
 *
 **************************************************************************/
aps_status_t zb_apsmeRequestKeyReq(ss_keyReqType_e keyType, addrExt_t dstAddr, addrExt_t partnerAddr);

/***********************************************************************//**
 * @brief       Perform a route request.
 *
 * @param[in]   pRouteDiscReq
 *
 * @return      Status
 *
 **************************************************************************/
aps_status_t zb_routeRequest(nlme_routeDisc_req_t *pRouteDiscReq);

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
 *
 * @return      None
 *
 **************************************************************************/
void zb_joinAFixedNetwork(u8 channel, u16 panId, u16 shortAddr, u8 *extPanId, u8 *nwkKey);

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
