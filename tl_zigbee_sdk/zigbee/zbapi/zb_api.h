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
#pragma once

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "../aps/aps_api.h"
#include "../nwk/includes/nwk.h"
#include "../nwk/includes/nwk_ctx.h"
#include "../zdo/zdo_api.h"
#include "../mac/includes/tl_zb_mac.h"


/**        
 *  @brief Structure for parameter of apsdeDataInd callback function
 */
typedef struct apsdeDataInd_s{
	aps_data_ind_t indInfo;
    u8 asduLen;
    u8 asdu[1];
}apsdeDataInd_t;



/***********************************************************************//**
 * @brief       Start device
 *
 * @param       None
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_zdoStartDevReq(void);

/***********************************************************************//**
 * @brief       Factory reset. Leave than SYSTEM_RESET.
 *
 * @param       None
 *
 * @return      Status
 *
 **************************************************************************/
void zb_factoryReset(void);

/***********************************************************************//**
 * @brief       Get device is a factory new device
 *
 * @param       None
 *
 * @return      TRUE or FALSE
 *
 **************************************************************************/
bool zb_isDeviceFactoryNew(void);

/*********************************************************************
 * @fn      zb_deviceFactoryNewSet
 *
 * @brief   Factory New device Set
 *
 * @param   new- 1: set as factory new device, 0: set as non-factory new device
 *
 * @return  none
 */
void zb_deviceFactoryNewSet(bool new);

/***********************************************************************//**
 * @brief       Get device is joined a network
 *
 * @param       None
 *
 * @return      TRUE or FALSE
 *
 **************************************************************************/
bool zb_isDeviceJoinedNwk(void);

/*********************************************************************
 * @fn      zb_nldeDataDiscoverRouteSet
 *
 * @brief   Enable or Disable AODV
 *
 * @param   TRUE  - Enable
 * 			FALSE - Disable
 *
 * @return  none
 */
void zb_nldeDataDiscoverRouteSet(bool enable);

/***********************************************************************//**
 * @brief       Get AODV Enable or Disable
 *
 * @param       None
 *
 * @return      TRUE or FALSE
 *
 **************************************************************************/
bool zb_nldeDataDiscoverRouteGet(void);

/***********************************************************************//**
 * @brief       reset device
 *
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_zdoResetDevReq();

void zb_zdoNodeDescManuCodeSet(u16 manuCode);
void zb_zdoNodeDescStackRevSet(u8 revision);

void zb_resetDevice(void );
void zb_resetDevice2FN(void );

void zb_setPollRate(u32 newRate);

/*********************************************************************
 * @fn      zb_setPollRate
 *
 * @brief
 *
 *
 * @return
 */
u32 zb_getPollRate(void);


/*********************************************************************
 * @fn      send data request directly
 *
 * @brief
 *
 * @return
 */
void zb_endDeviceSyncReq(void);

/***********************************************************************//**
 * @brief       Send a mac layer scan request
 *
 * @param[in]   scan_channels - scan channels
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_mlmeScanRequest(u32 scan_channels);

/***********************************************************************//**
 * @brief       Send a network layer associate join request
 *
 * @param[in]   channel 		- channel to join
 *
 * @param[in]   extended_pan_id - extended pan id to join
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_nwkAssociateJoinRequest(u8 channel,  extPANId_t extended_pan_id);

/***********************************************************************//**
 * @brief       Send device announce
 *
 * @param       None
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_zdoSendDevAnnance(void);

u8 zb_rejoinReq(rejoinNwk_method_t method, u32 chm);

u8 zb_routeRequest(nlme_routeDisc_req_t *pRouteDiscReq);


/***********************************************************************//**
 * @brief       get device type
 *
 * @param       None
 *
 * @return      device type (nwk_deviceType_t)
 * 				0 - EndDevice
 * 				1 - Router
 * 				2 - Coordinator
 * 				3 - None, not on network
 *
 **************************************************************************/
u8 zb_getDeviceType(void);

/***********************************************************************//**
 * @brief       Get local MAC short address
 *
 * @param       None
 *
 * @return      2 bytes MAC short address
 *
 **************************************************************************/
u16 zb_getLocalShortAddr(void);

/***********************************************************************//**
 * @brief       Get parent short address
 *
 * @param       None
 *
 * @return      2 bytes short address
 *
 **************************************************************************/
u16 zb_getParentShortAddr(void);

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
 * @brief       Get network short address based on the EXT address
 *
 * @param[in]   extAddr
 * @param[out]  nwkAddr
 *
 * @return      SUCCESS - 0; FAILED - 0xFF
 *
 **************************************************************************/
u8 zb_getNwkAddrByExtAddr(addrExt_t extAddr, u16 *nwkAddr);

/***********************************************************************//**
 * @brief       Get EXT address based on the network short address
 *
 * @param[in]   nwkAddr
 * @param[out]  extAddr
 *
 * @return      SUCCESS - 0; FAILED - 0xFF
 *
 **************************************************************************/
u8 zb_getExtAddrByNwkAddr(u16 nwkAddr, addrExt_t extAddr);

/***********************************************************************//**
 * @brief       get MAC association permit status (only for ZC ZR)
 *
 * @param[in]   None
 *
 * @return      Status
 *
 **************************************************************************/
bool zb_getMacAssocPermit(void);

/***********************************************************************//**
 * @brief       send permit join request to local device (only for ZC ZR)
 *
 * @param[in]   permit_duration - Time in seconds during which the device allows to join
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_nlmePermitJoiningRequest(u8 permit_duration);

/***********************************************************************//**
 * @brief       Perform a leave to  a device or self
 *
 * @param[in]   req - Pointer to the leave request structure
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_nlmeLeaveReq(nlme_leave_req_t * req);

/***********************************************************************//**
 * @brief       Perform a network discovery request
 *
 * @param[in]   pReq
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_nwkDiscReq(nlme_nwkDisc_req_t *pReq, nlme_state_t state);

/*********************************************************************
 * @fn      zb_routerStart
 *
 * @brief   send NWK_ROUTE_START command
 *
 * @param   void
 *
 * @return
 */
void zb_routerStart(void);

/*********************************************************************
 * @fn      zb_nwkDirectJoin
 *
 * @brief   NWK_DirectJoinReq
 *
 * @param   void
 *
 * @return  status
 */
u8 zb_nwkDirectJoin(nlme_directJoin_req_t req);

/***********************************************************************//**
 * @brief       Perform a network formation request
 *
 * @param[in]
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_nwkFormation(u32 scanChannels, u8 scanDuration);

/***********************************************************************//**
 * @brief       Set link status period
 *
 * @param[in]   period_in_sec - Time in seconds between two link status msg
 *
 * @return      None
 *
 **************************************************************************/
void zb_nlmeSetLinkStsPeriod(u8 period_in_sec);

/*******************************************************************************************************
 * @brief	ZDP remove device request cmd send interface which used by up layers
 *
 * @param	parentAddr: parent address of the device to be removed (64-bit)
 * 			devExt: Address of the device to be removed
 *
 * @return	ZDO_SUCCESS OR ZDO_INSUFFICIENT_SPACE when TX packet runout
 */
zdo_status_t zb_zdpRemoveDevReq(addrExt_t parentAddr, addrExt_t devExt);

/***********************************************************************//**
 * @brief       Send address request to target device for short address
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq    		- parameter of network address request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- network address response call back function
 *
 * @return      zdo_status_t
 *
 **************************************************************************/
zdo_status_t zb_zdoNwkAddrReq(u16 dstNwkAddr, zdo_nwk_addr_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send address request to target device for IEEE address
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq    		- parameter of IEEE address request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- IEEE address response call back function
 *
 * @return      zdo_status_t
 *
 **************************************************************************/
zdo_status_t zb_zdoIeeeAddrReq(u16 dstNwkAddr, zdo_ieee_addr_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send simple descriptor request
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq            - parameter of simple descriptor request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- simple descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoSimpleDescReq(u16 dstNwkAddr, zdo_simple_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send node descriptor request
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq            - parameter of node descriptor request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- node descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoNodeDescReq(u16 dstNwkAddr, zdo_node_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send power descriptor request
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq            - parameter of power descriptor request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- power descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoPowerDescReq(u16 dstNwkAddr, zdo_power_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send active endpoint request
 *
 * @param[in]	dstNwkAddr		- this command will be send to
 *  			pReq            - parameter of active endpoint request
 *  			seqNo			- the sequence number used by this command
 * 				indCb			- active endpoint response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoActiveEpReq(u16 dstNwkAddr, zdo_active_ep_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send match descriptor request
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq            - parameter of match descriptor request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- match descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoMatchDescReq(u16 dstNwkAddr, zdo_match_descriptor_req_t* pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send complex descriptor request
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq            - parameter of complex descriptor request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- complex descriptor response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoComplexDescReq(u16 dstNwkAddr, zdo_complex_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/*********************************************************************
 * @brief       Send user descriptor request
 *
 * @param[in]   dstNwkAddr		- this command will be send to
 * 				pReq            - parameter of user descriptor request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- user escriptor response call back function
 *
 * @return      Status
 */
zdo_status_t zb_zdoUserDescReq(u16 dstNwkAddr, zdo_user_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send zdo system server discovery request, broadcasting to 0xfffd
 *
 ** @param[in]  serverMask      - server mask bit assignment
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- system server discovery response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoSystemServerDiscoveryReq(u16 serverMask, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send bind request, destination address is based on the srcAddr in the request command
 *
 * @param[in]   isBinding		- 1:bind, 0:unbind
 * 				pReq            - parameter of bind/unbind request
 *  			seqNo			- the sequence number used by this command
 * 				indCb			- bind/unbind response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoBindUnbindReq(bool isBinding,zdo_bind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send end_device bind request
 *
 * @param[in]   pReq            - parameter of end device bind request
 * 				seqNo			- the sequence number used by this command
 * 				indCb			- end device bind response call back function
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_zdoEndDeviceBindReq(zdo_edBindReq_user_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send the command of mgmt_permitJoin_req
 *
 * @param[in]   dstNwkAddr            - Short address of the target device
 *
 * @param[in]   permitJoinDuration    - Time in seconds during which the device allows to join
 *
 * @param[in]   tc_significance       - tc significance
 *
 * @param[in]   seqNo				  - the sequence number used by this command
 *
 * @param[in]   cb				      - mgmtPermitJoin.confirm callback
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtPermitJoinReqTx(u16 dstNwkAddr, u8 permitJoinDuration, u8 tc_significance, u8 *seqNo, zdo_callback rspCb);

/*******************************************************************************************************
 * @brief	Send the command of mgmt_leave_req
 *
 * @param[in]	dstAddr: 		-target address to receive this cmd
 *
 * @param[in]   pReq            - parameter of mgmt_leave_req
 *
 * @param[in]   seqNo  			-the sequence number used by this command
 *
 * @param[in]   indCb			- the indicate callback for mgmt_leave_rsp
 *
 * @return	ZDO_SUCCESS OR ZDO_INSUFFICIENT_SPACE when TX packet runout
 */
zdo_status_t zb_mgmtLeaveReq(u16 dstNwkAddr, zdo_mgmt_leave_req_t *req, u8 *seqNo, zdo_callback indCb);


/***********************************************************************//**
 * @brief       Send the command of mgmt_network_update_req
 *
 * @param[in]	dstNwkAddr		- destination address of the request
 *
 * @param[in]   pReq            - parameter of mgmt_network_update_req
 *
 * @param[in]   seqNo		    - the sequence number used by this command
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtNwkUpdateReq(u16 dstNwkAddr, zdo_mgmt_nwk_update_req_t *pReq, u8 *seqNo);

/***********************************************************************//**
 * @brief       Send the command of mgmt_lqi_req
 *
 * @param[in]	dstNwkAddr		- destination address of the request
 *
 * @param[in]   pReq            - parameter of mgmt_lqi_req
 *
 * @param[in]   seqNo		    - the sequence number used by this command
 *
 * @param[in]   indCb			- the indicate callback for mgmt_lqi_rsp
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtLqiReq(u16 dstNwkAddr, zdo_mgmt_lqi_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send the command mgmt_bind_req
 *
 * @param[in]	dstNwkAddr		- destination address of the request
 *
 * @param[in]   pReq            - parameter of mgmt_bind_req
 *
 * param[in]    seqNo		    - the sequence number used by this command
 *
 * @param[in]   indCb			- the indicate callback for mgmt_bind_rsp
 *
 * @return      Status
 *
 **************************************************************************/
zdo_status_t zb_mgmtBindReq(u16 dstNwkAddr, zdo_mgmt_bind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/***********************************************************************//**
 * @brief       Send device announce
 *
 * @param       None
 *
 * @return      Status
 *
 **************************************************************************/
u8 zb_zdoSendDevAnnance(void);

/***********************************************************************//**
 * @brief       Send parent announce
 *
 * @param       None
 *
 * @return      Status
 *
 **************************************************************************/
void zb_zdoSendParentAnnce(void);

/***********************************************************************//**
 * @brief       Search binding table
 *
 * @param[in]   clusterID
 * @param[in]   srcEp
 *
 * @return      TRUE or FALSE
 *
 **************************************************************************/
bool zb_bindingTblSearched(u16 clusterID, u8 srcEp);

/***********************************************************************//**
 * @brief       Register MAC layer callback function

 * @param[in]   cb           - Callback functions
 *
 * @return      NULL
 *
 **************************************************************************/
void zb_macCbRegister(mac_appIndCb_t *cb);

/***********************************************************************//**
 * @brief       Register ZDO layer callback function

 * @param[in]   cb           - Callback functions
 *
 * @return      NULL
 *
 **************************************************************************/
void zb_zdoCbRegister(zdo_appIndCb_t *cb);

/***********************************************************************//**
 * @brief       node force join a fixed network
 *
 * @param       channel: operation channel
 *
 ** @param       panId:  the network panID
 *
 ** @param       shortAddr: network address allocated by itself
 *
 ** @param       extPanId: external panID of the network
 *
 ** @param       nwkKey:   the network key of the network
 *
 * @return      Status
 *
 **************************************************************************/
void zb_joinAFixedNetwork(u8 channel,  u16 panId, u16 shortAddr, u8 *extPanId, u8 *nwkKey);


/***********************************************************************//**
 * @brief       set extPan Id for direct join
 *
 * @param       panId: ext PANID
 *
 **************************************************************************/
void zb_apsExtPanidSet(extPANId_t panId);

/***********************************************************************//**
 * @brief       set channel  mask for active scan
 *
 * @param       mask:  channel mask
 *
 **************************************************************************/
u8 zb_apsChannelMaskSet(u32 mask);

/***********************************************************************//**
 * @brief       get channel  mask
 *
 * @param
 *
 * @return  	channel mask
 **************************************************************************/
u32 zb_apsChannelMaskGet(void);

/***********************************************************************//**
 * @brief       set rejoin mode
 *
 * @param       mode: REJOIN_INSECURITY or REJOIN_SECURITY
 *
 **************************************************************************/
void zb_rejoin_mode_set(u8 mode);

/***********************************************************************//**
 * @brief       extPan Id rejoin(only for for the factory new device), called in the bdbInitCb() or UI
 *
 * @param       panId: ext PANID
 *
 **************************************************************************/
void zb_extPanIdRejoin(extPANId_t extPanId);

/***********************************************************************//**
 * @brief       pre-configure network key(only for for the factory new device), called in the bdbInitCb() or UI
 * 				and then start bdb_networkSteerStart
 *
 * @nwkKey      the pre-configured network key
 *
 * @enTransKey	0: join network once receiving associate response
 * 				1: join network follow the standard flow(need process transport key)
 *
 **************************************************************************/
void zb_preConfigNwkKey(u8 *nwkKey, bool enTransKey);


/* @brief       get poo rate for end device
 *
 * @
 *
 * @return		poll rate, unit: s
 *
 **************************************************************************/
u32 zb_getPollRate(void);

