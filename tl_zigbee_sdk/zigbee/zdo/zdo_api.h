/********************************************************************************************************
 * @file	zdo_api.h
 *
 * @brief	This is the header file for zdo_api
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
	ZDO_INVALID_PARAMETERS					= 0x01,
	ZDO_RESPONSE_WAIT_TIMEOUT			    = 0x02,
	ZDO_BUSY                           		= 0x04,
	ZDO_NO_NETWORKS                         = 0x05,
	ZDO_EXTPANID_ALREADY_EXIST	            = 0x06,
	ZDO_FAIL								= 0x07,
	ZDO_AUTHENTICATION_SUCCESS				= 0x08,
	ZDO_AUTHENTICATION_FAIL					= 0x09,
	ZDO_SECURITY_SUCCESS					= 0x0A,
	ZDO_SECURITY_FAIL						= 0x0B,
	ZDO_SECURITY_NOT_SUPPORTED				= 0x0C,
	ZDO_CMD_COMPLETED						= 0x0D,
	ZDO_KEY_ESTABLISHMENT_NOT_SUPPORTED		= 0x0E,

	ZDO_NO_KEY_PAIR_DESCRIPTOR				= 0x50,
	ZDO_UPDATE_LINK_KEY						= 0x51,
	ZDO_UPDATE_MASTER_KEY					= 0x52,
	ZDO_DELETE_KEY_PAIR						= 0x53,
	ZDO_DELETE_LINK_KEY						= 0x54,

	ZDO_TC_NOT_AVAILABLE					= 0x60,

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
	ZDO_NETWORK_STARTED						= 0x8F,
	ZDO_NETWORK_LOST						= 0x90,
	ZDO_NETWORK_LEFT						= 0x91,
	ZDO_CHILD_JOINED						= 0x92,
	ZDO_CHILD_REMOVED						= 0x93,
	ZDO_USER_DESCRIPTOR_UPDATE				= 0x94,
	ZDO_STATIC_ADDRESS_CONFLICT				= 0x95
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

	/* Addition rejoin backoff for devices.
	 * If the config_rejoin_times is ZERO, it will not start Fast Rejoin and Rejoin Backoff behavior.
	 */
	u8	config_rejoin_times;//The number of rejoin attempts during the fast rejoin.
							//If 0 means disable Fast Rejoin and Rejoin Backoff;
							//if 1, the config_rejoin_duration setting is not useful.
	u16	config_rejoin_duration;//The amount of time between each rejoin attempt while the device is in Fast Rejoin mode, in seconds.
							   //If 0, config_rejoin_times will be ignored and only one Fast Rejoin will be performed.
	u16	config_rejoin_backoff_time;//The amount of time to sleep after the Fast Rejoin attempts before performing the next attempt, in seconds.
								   //If 0 means no Rejoin backoff/retry.
	u16	config_max_rejoin_backoff_time;//Upper limit of the config_rejoin_backoff_time.
	u16 config_rejoin_backoff_iteration;//The number of iterations of the Fast Rejoin backoff, in seconds.
										//If 0 means do not reset the backoff duration.
}zdo_attrCfg_t;



/**
 *  @brief Structure for parameter of startDev callback function
 */
typedef struct{
    zdo_status_t status;
    u8	channel_num;
    u16	pan_id;
    u16	short_addr;
}zdo_start_device_confirm_t;

typedef struct{
	addrExt_t parentIeeeAddr;
	addrExt_t devIeeeAddr;
	u16 devShortAddr;
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

typedef struct{
	zdo_startDveCnfCb_t		zdpStartDevCnfCb;
	zdo_nlmeResetCnfCb_t	zdpResetCnfCb;
	zdo_dveAnnceIndCb_t		zdpDevAnnounceIndCb;
	zdo_leaveIndCb_t		zdpLeaveIndCb;
	zdo_leaveCnfCb_t		zdpLeaveCnfCb;
	zdo_nwkUpdateCb_t		zdpNwkUpdateIndCb;
	zdo_permitJoinIndCb_t	zdpPermitJoinIndCb;
	zdo_nlmeSyncCnfCb_t		zdoNlmeSyncCnfCb;
	zdo_tcJoinIndCb_t		zdoTcJoinIndCb;
}zdo_appIndCb_t;


typedef bool (*zdo_touchLinkleaveCnfCb_t)(nlme_leave_cnf_t *p);

typedef struct{
	zdo_touchLinkleaveCnfCb_t	zdpLeaveCnfCb;
}zdo_touchLinkCb_t;


extern zdo_attrCfg_t zdo_cfg_attributes;
extern u32 TRANSPORT_NETWORK_KEY_WAIT_TIME;




void zdo_zdpCbTblRegister(zdo_appIndCb_t *cbTbl);

void zdo_touchLinkCbRegister(zdo_touchLinkCb_t *cb);

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
 * @brief	Interface to get scan duration attribute
 *
 * @param	none
 *
 * @return	value in superframe
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
 * @brief	Start network discovery
 *
 * @param	pReq: the NWK-Discovery.req primitive
 *
 * @return	none
 */
void zdo_nwk_discovery_Start(nlme_nwkDisc_req_t *pReq, nwkDiscoveryUserCb_t cb);

/******************************************************************************************
 * @brief	Interface called when need to join or rejoin a network. This interface would construct
 * 			a nlme_join_req_t info to call nwk_nlme_join_req function to start the join procedure.
 *
 * @param	method:
 * @param	chm: 	channel map used to rejoin
 *
 * @return	zdo_status_t
 */
zdo_status_t zdo_nwk_rejoin_req(rejoinNwk_method_t method, u32 chm);

/******************************************************************************************
 * @brief	Interface for forgetting all information about the specified device.
 *
 * @param	nodeIeeeAddr: 	ieee address of the specified device
 * @param	rejoin: 		whether the device will do rejoin
 *
 * @return	none
 */
void zdo_nlmeForgetDev(addrExt_t nodeIeeeAddr, bool rejoin);

/******************************************************************************************
 * @brief	Start rejoin backoff timer.
 *
 * @param	none
 *
 * @return	none
 */
void zdo_rejoin_backoff_timer_start(void);

/******************************************************************************************
 * @brief	Stop rejoin backoff timer.
 *
 * @param	none
 *
 * @return	none
 */
void zdo_rejoin_backoff_timer_stop(void);

#endif	/* ZDO_API_H */
