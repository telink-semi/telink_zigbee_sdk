/********************************************************************************************************
 * @file    zdo_api.h
 *
 * @brief   This is the header file for zdo_api
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

#ifndef ZDO_API_H
#define	ZDO_API_H




/***********************************************************************************
 * @brief	The structure for indication of data reception. A pointer to an instance of
 * 			this type is passed to a data indication callback registered for a given endpoint
 * 			when the data is received destined to the endpoint. The structure definition follows
 * 			APSDE-DATA indication primitive described in  Zigbee Specification r18, 2.2.4.1.3 APSDE-DATA.indication,
 * 			page 29.
 */
#define	MAC_HEADER_MAX_SIZE			23
#define	NWK_HEADER_MAX_SIZE			25
#define	SECURE_HEADER_SIZE			14
#define	APS_HEADER_MAX_SIZE			8
#define	APS_MAXPACKET_LENGTH		(127 - MAC_HEADER_MAX_SIZE - NWK_HEADER_MAX_SIZE - SECURE_HEADER_SIZE - APS_HEADER_MAX_SIZE)


/**********************************************************************************
 * @brief		zdo result status primitives.
*/
typedef enum{
	ZDO_SUCCESS								= 0x00,

	ZDO_NETWORK_LOST						= 0x60,//Non-standard status

	ZDO_INVALID_REQUEST						= 0x80,
	ZDO_DEVICE_NOT_FOUND					= 0x81,
	ZDO_INVALID_EP							= 0x82,
	ZDO_NOT_ACTIVE							= 0x83,
	ZDO_NOT_SUPPORTED						= 0x84,
	ZDO_TIMEOUT								= 0x85,
	ZDO_NO_MATCH							= 0x86,

	ZDO_NO_ENTRY							= 0x88,
	ZDO_NO_DESCRIPTOR						= 0x89,
	ZDO_INSUFFICIENT_SPACE					= 0x8A,
	ZDO_NOT_PERMITTED						= 0x8B,
	ZDO_TABLE_FULL							= 0x8C,
	ZDO_NOT_AUTHORIZED						= 0x8D,
	ZDO_DEVICE_BINDING_TABLE_FALL			= 0x8E,
	ZDO_INVALID_INDEX						= 0x8F,
}zdo_status_t;


typedef struct{
	extPANId_t	nwkExtendedPANID;
	extPANId_t	apsUseExtendedPANID;
	u32	apsChannelMask;
	u8	apsDesignatedCoordinator;
	u8	apsUseInsecureJoin;
}zdo_startup_attr_t;

typedef struct{
	/* Integer value representing the time duration (in OctetDurations) between each NWK discovery attempt.
	 * It is employed within ZDO to provide a time duration between the NLME-NETWORKDISCOVERY.request attempts.
	 */
	u32	config_nwk_indirectPollRate;//In ms
	u16	config_nwk_time_btwn_scans;//In ms, default value, 100ms on 2.4GHz

	/* The :Config_NWK_Scan_Attempts is employed within ZDO to call the NLME-NETWORKDISCOVERY.
	 * request primitive the indicated number of times (for routers and end devices).Integer value
	 * representing the number of scan attempts to make before the NWK layer decides which ZigBee
	 * coordinator or router to associate with.
	 */
	u8	config_nwk_scan_attempts;//This attribute has default value of 5 and valid values between 1 and 255.
	u8	config_permit_join_duration;//Permit join duration, 0x00 - disable join, 0xff - join is allowed forever
	u8	config_parent_link_retry_threshold;//Number of retry parent sync before judged as connection lost and the default value is ZDO_MAX_PARENT_THRESHOLD_RETRY

	/* Addition rejoin with backoff timer for rejoin device.
	 * |-------------------------------------------------|
	 * |<---- rejoin times = 2 ----->|<- first backoff ->|
	 * |<- duration ->|<- duration ->|<- backoff time  ->|
	 * |---------------------------------------------------------------------|
	 * |<----- rejoin times = 2 ---->|<----------- second backoff ---------->|
	 * |<- duration ->|<- duration ->|<- backoff time  ->|<- backoff time  ->|
	 * |-----------------------------------------------------------------------------------------|
	 * |<----- rejoin times = 2 ---->|<---------------------- third backoff -------------------->|
	 * |<- duration ->|<- duration ->|<- backoff time  ->|<- backoff time  ->|<- backoff time  ->|
	 */
	u8	config_rejoin_times;//The number of rejoin attempts during the fast rejoin.
	u16	config_rejoin_duration;//The amount of time between each rejoin attempt while the device is in Fast Rejoin mode, in seconds.
							   //If 0, config_rejoin_times will be ignored and only one Fast Rejoin will be performed.
	u16	config_rejoin_backoff_time;//The amount of time to sleep after the Fast Rejoin attempts before performing the next attempt, in seconds.
								   //If 0 means no Rejoin backoff/retry.
	u16	config_max_rejoin_backoff_time;//Upper limit of the config_rejoin_backoff_time.
	u16 config_rejoin_backoff_iteration;//The number of iterations of the Fast Rejoin backoff, in times.
										//If 0 means do not reset the backoff duration.

	u16 config_accept_nwk_update_pan_id;
	u8	config_accept_nwk_update_channel;
	bool config_mgmt_leave_use_aps_sec;
	bool config_use_tc_sec_on_nwk_key_rotation;
	u8	config_nwk_scan_duration;
}zdo_attrCfg_t;



/**
 *  @brief Structure for parameter of startDev callback function
 */
typedef struct{
    u8 status;
    u8 channel_num;
    u16	pan_id;
    u16	short_addr;
}zdo_start_device_confirm_t;

typedef struct{
	addrExt_t parentIeeeAddr;
	addrExt_t devIeeeAddr;
	u16 devShortAddr;
	u8 sta;//ss_apsmeUpdateDevStatus_e
}zdo_tc_join_ind_t;

typedef void (*zdo_startDveCnfCb_t)(zdo_start_device_confirm_t *p);
typedef void (*zdo_nlmeResetCnfCb_t)(nlme_reset_cnf_t *p);
typedef void (*zdo_dveAnnceIndCb_t)(zdo_device_annce_req_t *p);
typedef void (*zdo_leaveIndCb_t)(nlme_leave_ind_t *p);
typedef void (*zdo_leaveCnfCb_t)(nlme_leave_cnf_t *p);
typedef bool (*zdo_nwkUpdateCb_t)(nwkCmd_nwkUpdate_t *p);
typedef void (*zdo_permitJoinIndCb_t)(nlme_permitJoining_req_t *p);
typedef void (*zdo_nlmeSyncCnfCb_t)(nlme_sync_cnf_t *p);
typedef bool (*zdo_tcJoinIndCb_t)(zdo_tc_join_ind_t *p);
typedef void (*zdo_tcFrameCntReachedCb_t)(void);

typedef struct{
	zdo_startDveCnfCb_t				zdpStartDevCnfCb;
	zdo_nlmeResetCnfCb_t			zdpResetCnfCb;
	zdo_dveAnnceIndCb_t				zdpDevAnnounceIndCb;
	zdo_leaveIndCb_t				zdpLeaveIndCb;
	zdo_leaveCnfCb_t				zdpLeaveCnfCb;
	zdo_nwkUpdateCb_t				zdpNwkUpdateIndCb;
	zdo_permitJoinIndCb_t			zdpPermitJoinIndCb;
	zdo_nlmeSyncCnfCb_t				zdoNlmeSyncCnfCb;
	zdo_tcJoinIndCb_t				zdoTcJoinIndCb;			//only for ZC
	zdo_tcFrameCntReachedCb_t		ssTcFrameCntReachedCb;	//only for ZC
}zdo_appIndCb_t;

typedef void (*nwkDiscoveryUserCb_t)(void);
typedef bool (*zdo_touchLinkleaveCnfCb_t)(nlme_leave_cnf_t *p);
typedef void (*nwk_touchLinkAttrClearCb_t)(void);

typedef struct{
	zdo_touchLinkleaveCnfCb_t	leaveCnfCb;
	nwk_touchLinkAttrClearCb_t	attrClearCb;
}zdo_touchLinkCb_t;


extern zdo_appIndCb_t *zdoAppIndCbLst;
extern zdo_touchLinkCb_t *zdoTouchLinkCb;
extern zdo_attrCfg_t zdo_cfg_attributes;
extern u32 TRANSPORT_NETWORK_KEY_WAIT_TIME;




void zdo_zdpCbTblRegister(zdo_appIndCb_t *cbTbl);

void zdo_touchLinkCbRegister(zdo_touchLinkCb_t *cbTbl);

/****************************************************************************************************
 * @brief	Theb:Config_Parent_Link_Retry_Thres hold is either created when the  application is first loaded or
			initialized with a commissioning tool. It is used for the ZED to decide how many times it should
			retry to connect to the parent router before initiating the rejoin process.
 *
 * @param	none
 *
 * @return	max retry number
 */
u8 zdo_af_get_link_retry_threshold(void);
void zdo_af_set_link_retry_threshold(u8 threshold);

/****************************************************************************************************
 * @brief	Interface to get the NWK indirect poll rate parameter, The value for this configuration attribute
 * 			is established by the application profile deployed on the device.
 *
 * @param	none
 *
 * @return	poll rate in ms
 */
u32 zdo_af_get_syn_rate(void);
void zdo_af_set_syn_rate(u32 newRate);

/****************************************************************************************************
 * @brief	The :Config_Rejoin_Interval is either created when the application is first loaded or initialized with a
 *			commissioning tool. It is used by the ZED to decide how often it should initiate the rejoin process.
 *
 * @param	none
 *
 * @return	rejoin interval
 */
u8 zdo_af_get_rejoin_times(void);
void zdo_af_set_rejoin_times(u8 times);

u16 zdo_af_get_rejoin_duration(void);
void zdo_af_set_rejoin_duration(u16 duration);

u16 zdo_af_get_rejoin_backoff_time(void);
void zdo_af_set_rejoin_backoff_time(u16 interval);

u16 zdo_af_get_max_rejoin_backoff_time(void);
void zdo_af_set_max_rejoin_backoff_time(u16 interval);

u16 zdo_af_get_rejoin_backoff_iteration(void);
void zdo_af_set_rejoin_backoff_iteration(u16 iteration);

/****************************************************************************************************
 * @brief	The :Config_NWK_Scan_Attempts is employed within ZDO to call the NLME-NETWORKDISCOVERY. request
 * 			primitive the indicated number of times (for routers and end devices). The attribute has
 * 			default value of 5 and valid values between 1 and 255
 *
 * @param	none
 *
 * @return	scan attempts
 */
u8 zdo_af_get_scan_attempts(void);
void zdo_af_set_scan_attempts(u8 attempts);

/**********************************************************************************************************
 * @brief	Interface to get time between scans attribute
 *
 * @param	none
 *
 * @return	value in ms
 */
u16 zdo_af_get_nwk_time_btwn_scans(void);

/**********************************************************************************************************
 * @brief	The default value for :Config_Permit_Join_Duration is 0x00, however, this value can be  established
 * 			differently according  to the needs of the profile.
 *
 * @param	none
 *
 * @return	value
 */
u8 zdo_af_get_permit_join_duration(void);


void zdo_af_set_accept_nwk_update_pan_id(u16 panId);
u16 zdo_af_get_accept_nwk_update_pan_id(void);

void zdo_af_set_accept_nwk_update_channel(u8 channel);
u8 zdo_af_get_accept_nwk_update_channel(void);

void zdo_af_set_mgmtLeave_use_aps_sec(bool enable);
bool zdo_af_get_mgmtLeave_use_aps_sec(void);

void zdo_af_set_use_tc_sec_on_nwk_key_rotation(bool enable);
bool zdo_af_get_use_tc_sec_on_nwk_key_rotation(void);

bool zb_isUnderRejoinMode();

/************************************************************************************
 * @brief	Internal used interface to translate the channel map to the logic channel num
 *
 * @param	chp: channel map
 *
 * @return	logic channel num
 */
u8 zdo_channel_page2num(u32 chp);

/************************************************************************
 * @brief	zdo layer initiate function. This function reset Application layer attributes
 *
 * @param	none
 *
 * @return	none
 */
void zdo_init(void);

/******************************************************************************************
 * @brief	Start network formation.
 * 			Only for Coordinator and Router device.
 *
 * @param	scanChannels
 * 			scanDuration
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkFormationStart(u32 scanChannels, u8 scanDuration);

/******************************************************************************************
 * @brief	Start router request.
 * 			Only for Coordinator and Router device network recovery.
 *
 * @param
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkRouterStart(void);

/******************************************************************************************
 * @brief	Start network discovery.
 * 			Only for Router and End Device.
 *
 * @param	pReq: the NWK-Discovery.req primitive
 * @param	cb
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkDiscoveryStart(nlme_nwkDisc_req_t *pReq, nwkDiscoveryUserCb_t cb);

/******************************************************************************************
 * @brief	Stop network discovery.
 * 			Only for Router and End Device.
 *
 * @param
 *
 * @return	none
 */
void zdo_nwkDiscoveryStop(void);

/******************************************************************************************
 * @brief	Start associate join.
 * 			Only for Router and End Device.
 *
 * @param
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkAssocJoinStart(void);

/******************************************************************************************
 * @brief	Start network rejoin.
 * 			Only for Router and End Device.
 *
 * @param	scanChannels
 * @param	scanDuration
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkRejoinStart(u32 scanChannels, u8 scanDuration);

/******************************************************************************************
 * @brief	Start network rejoin with back off timer.
 * 			Only for Router and End Device.
 *
 * @param	scanChannels
 * @param	scanDuration
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkRejoinWithBackOff(u32 scanChannels, u8 scanDuration);

/******************************************************************************************
 * @brief	Stop network rejoin with back off timer.
 * 			Only for Router and End Device.
 *
 * @param	none
 *
 * @return	none
 */
void zdo_nwkRejoinWithBackOffStop(void);

/******************************************************************************************
 * @brief	Start direct join.
 * 			Only for Router and End Device.
 *
 * @param	scanChannels
 * @param	scanDuration
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkDirectJoinStart(u32 scanChannels, u8 scanDuration);

/******************************************************************************************
 * @brief	Direct join accept.
 * 			Only for Coordinator and Router.
 *
 * @param	pReq - the device information which will join network through direct join mode.
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwkDirectJoinAccept(nlme_directJoin_req_t *pReq);

/******************************************************************************************
 * @brief	Interface for forgetting all information about the specified device.
 *
 * @param	nodeIeeeAddr: 	ieee address of the specified device
 * @param	rejoin: 		whether the device will do rejoin
 *
 * @return	none
 */
void zdo_nlmeForgetDev(addrExt_t nodeIeeeAddr, bool rejoin);



/***********************************************************************//**
 * @brief		Shift to and save the logic channel.
 *
 * @param[in]	none
 *
 * @return	TRUE--IDLE; FALSE--other state
 *
 **************************************************************************/
bool zdo_ifZdoNwkManagerIdle(void);
#endif	/* ZDO_API_H */
