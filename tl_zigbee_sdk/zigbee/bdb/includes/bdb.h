/********************************************************************************************************
 * @file     bdb.h
 *
 * @brief    header file for base device behavior
 *
 * @author
 * @date     May. 27, 2017
 *
 * @par      Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd.
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
#ifndef BDB_H
#define BDB_H

#include "tl_common.h"
#include "../../zdo/zb_af.h"
#include "../../zdo/zdo_api.h"


/** @addtogroup  TELINK_ZIGBEE_STACK TELINK ZigBee Stack
 *  @{
 */

/** @addtogroup  TELINK_ZIGBEE_BDB telink bdb
 *  @{
 */


/** @addtogroup zb_bdb_constant BDB Constants
 *  Definition for Constants used by all nodes
 * @{
 */
#define BDBC_MAX_SAME_NETWROK_RETRY_ATTEMPTS	10	//<! the maximum number of join or key exchange attempts made to the same network
#define	BDBC_MIN_COMMISSIONING_TIME				(180) /**1000)*/ //!< the minimum duration in seconds for which a network is opened to allow new nodes to join or for a device to identify itself
#define BDBC_REC_SAME_NETWROK_RETRY_ATTEMPTS	3	//!< the RECOMMENDED maximum number of join or key exchange attempts made to the same network
#define BDBC_TC_LINK_KEY_EXCHANGE_TIMEOUT		(5)	//!< the maximum time in seconds a joining node will wait for a response when sending an APS request key to the Trust Center
/** @} end of group zb_bdb_constant */


/** @addtogroup zb_bdb_constant_tl BDB Constants for zb_bdb_constant_touchlink
 *  Definition for Constants used by all nodes supporting touchlink
 * @{
 */
#define BDBC_TL_INTRP_TRANSId_LIFETIME		(8*1000)	//<! the maximum length of time an inter-PAN transaction ID remains valid
#define BDBC_TL_MIN_STARTUP_DELAY_TIME		(2*1000)	//<! the length of time an initiator waits to ensure the target has completed its network startup procedure
#define	BDBC_TL_PRIMARY_CHANNEL_SET			0x02108800  //!< the bitmask for the channel set comprised of channels 11, 15, 20 and 25, that will be used for a non-extended touchlink scan.
#define BDBC_TL_RX_WINDOW_DURATION			(5*1000)	//!< the maximum duration that a node leaves its receiver enabled during touchlink for subsequent responses
#define BDBC_TL_SCAN_TIME_BASE_DURATION 	250			//!< the base duration for a touchlink scan operation during which the receiver is enabled for scan responses after having transmitted a scan request
#define BDBC_TL_SECONDARY_CHANNEL_SET 		(0x07fff800^BDBC_TL_PRIMARY_CHANNEL_SET)	//!< the bitmask for the channel set comprised of the remaining IEEE 802.15.4-2003 channels except BDBC_TL_PRIMARY_CHANNEL_SET
/** @} end of group zb_bdb_constant_tl */

enum ll_device_id {
	LL_DEV_ONOFF_LIGHT                  = 0x0000,
	LL_DEV_ONOFF_PLUGIN_UNIT            = 0x0010,
	LL_DEV_DIMMABLE_LIGHT               = 0x0100,
	LL_DEV_DIMMABLE_PLUGIN_UNIT         = 0x0110,
	LL_DEV_COLOR_LIGHT                  = 0x0200,
	LL_DEV_EXTENDED_COLOR_LIGHT         = 0x0210,
	LL_DEV_COLOR_TEMPERATURE_LIGHT      = 0x0220,
	LL_DEV_COLOR_CONTROLLER             = 0x0800,
	LL_DEV_COLOR_SCENE_CONTROLLER       = 0x0810,
	LL_DEV_NON_COLOR_CONTROLLER         = 0x0820,
	LL_DEV_NON_COLOR_SCENE_CONTROLLER   = 0x0830,
	LL_DEV_CONTROL_BRIDGE               = 0x0840,
	LL_DEV_ONOFF_SENSOR                 = 0x0850,
};


enum ha_device_id {
	HA_DEV_ONOFF_SWITCH                     = 0x0000,
	HA_DEV_LEVEL_CTRL_SWITCH                = 0x0001,
	HA_DEV_ONOFF_OUTPUT                     = 0x0002,
	HA_DEV_LEVEL_CTRLABLE_OUTPUT            = 0x0003,
	HA_DEV_SCENE_SELECTOR                   = 0x0004,
	HA_DEV_CONFIG_TOOL                      = 0x0005,
	HA_DEV_REMOTE_CTRL                      = 0x0006,
	HA_DEV_COMBINED_INTERFACE               = 0x0007,
	HA_DEV_RANGE_EXTENDER                   = 0x0008,
	HA_DEV_MAINS_PWR_OUTLET                 = 0x0009,
	HA_DEV_DOOR_LOCK                        = 0x000A,
	HA_DEV_DOOR_LOCK_CTRLER                 = 0x000B,
	HA_DEV_SIMPLE_SENSOR                    = 0x000C,
	HA_DEV_CONSUMPTION_AWARENESS            = 0x000D,
	HA_DEV_HOME_GATEWAY                     = 0x0050,
	HA_DEV_SMART_PLUG                       = 0x0051,
	HA_DEV_WHITE_GOOD                       = 0x0052,
	HA_DEV_METER_INTERFACE                  = 0x0053,

	HA_DEV_ONOFF_LIGHT                      = 0x0100,
	HA_DEV_DIMMABLE_LIGHT                   = 0x0101,
	HA_DEV_COLOR_DIMMABLE_LIGHT             = 0x0102,
	HA_DEV_ONOFF_LIGHT_SWITCH               = 0x0103,
	HA_DEV_DIMMER_SWITCH                    = 0x0104,
	HA_DEV_COLOR_DIMMER_SWITCH              = 0x0105,
	HA_DEV_LIGHT_SENSOR                     = 0x0106,
	HA_DEV_OCCUPANCY_SENSOR                 = 0x0107,
	HA_DEV_ONOFF_BALLAST	                = 0x0108,
	HA_DEV_DIMMABLE_BALLAST					= 0x0109,
	HA_DEV_ONOFF_PLUG_IN_UNIT				= 0x010A,
	HA_DEV_DIMMANLE_PLUG_IN_UNIT			= 0x010B,
	HA_DEV_COLOR_TEMPERATURE_LIGHT			= 0x010C,
	HA_DEV_EXTENDED_COLOR_LIGHT				= 0x010D,
	HA_DEV_LIGHT_LEVEL_SENSOR				= 0x010E,
	HA_DEV_COLOR_CONTROLLER					= 0x0800,
	HA_DEV_COLOR_SCENE_CONTROLLER			= 0x0810,
	HA_DEV_NON_COLOR_CONTROLLER				= 0x0820,
	HA_DEV_NON_COLOR_SCENE_CONTROLLER		= 0x0830,
	HA_DEV_CONTROL_BRIDGE					= 0x0840,
	HA_DEV_ONOFF_SENSOR						= 0x0850,

	HA_DEV_SHADE                            = 0x0200,
	HA_DEV_SHADE_CTRLER                     = 0x0201,
	HA_DEV_WINDOW_COVERING_DEVICE           = 0x0202,
	HA_DEV_WINDOW_COVERING_CTRLER           = 0x0203,

	HA_DEV_HEATING_COOLING_UNIT             = 0x0300,
	HA_DEV_THERMOSTAT                       = 0x0301,
	HA_DEV_TEMPERATURE_SENSOR               = 0x0302,
	HA_DEV_PUMP                             = 0x0303,
	HA_DEV_PUMP_CTRLER                      = 0x0304,
	HA_DEV_PRESSURE_SENSOR                  = 0x0305,
	HA_DEV_FLOW_SENSOR                      = 0x0306,

	HA_DEV_IAS_CIE                          = 0x0400,
	HA_DEV_IAS_ACE                          = 0x0401,
	HA_DEV_IAS_ZONE                         = 0x0402,
	HA_DEV_IAS_WD                           = 0x0403,
};

/** @addtogroup zb_bdb_tc_key_exchange_method TCLinkKeyExchangeMethod
 *  Definition for trust center key exchange method
 * @{
 */
enum{
	TCKEY_EXCHANGE_METHOD_APSRK = 0,		//<! APS Request Key
	TCKEY_EXCHANGE_METHOD_CBKE = 1,			//<! Certificate Based Key Exchange
	TCKEY_EXCHANGE_METHOD_RESV				//<! Reserved
};
/** @} end of group zb_bdb_tc_key_exchange_method */

/** @addtogroup NodeJoinLinkKeyType
 *  Definition for link key type with which the node was able to decrypt the network key when the node joins a new network.
 * @{
 */
enum{
	LINKKEY_TYPE_TC = 0,			//<! Default global Trust Center link key
	LINKKEY_TYPE_GB	= 1, 			//<! Distributed security global link key
	LINKKEY_TYPE_PRECONFIG = 2,		//<! Install code derived preconfigured link key
	LINKKEY_TYPE_TL_PRECONFIG =3	//<! Touchlink preconfigured link key
};
/** @} end of group NodeJoinLinkKeyType */



/** @addtogroup CommissioningStatus
 *  Definition for Commissioning status.
 * @{
 */
enum{
	BDB_COMMISSION_STA_SUCCESS = 0,			//<! The commissioning sub-procedure was successful.
	BDB_COMMISSION_STA_IN_PROGRESS, 		//<! One of the commissioning sub-procedures has started but is not yet complete
	BDB_COMMISSION_STA_NOT_AA_CAPABLE,		//<! The initiator is not address assignment capable during touchlink.
	BDB_COMMISSION_STA_NO_NETWORK,			//<! A network has not been found during network steering or touchlink
	BDB_COMMISSION_STA_TARGET_FAILURE,		//<! A node has not joined a network when requested during touchlink
	BDB_COMMISSION_STA_FORMATION_FAILURE,	//<! A network could not be formed during network formation.
	BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE,//<! No response to an identify query command has been received during finding & binding
	BDB_COMMISSION_STA_BINDING_TABLE_FULL,  //<! A binding table entry could not be created due to insufficient space in the binding table during finding & binding
	BDB_COMMISSION_STA_NO_SCAN_RESPONSE,	//<! No response to a scan request inter-PAN command has been received during touchlink
	BDB_COMMISSION_STA_NOT_PERMITTED,		//<! A touchlink (steal) attempt was made when a node is already connected to a centralized security network
	BDB_COMMISSION_STA_TCLK_EX_FAILURE,		//<! The Trust Center link key exchange procedure has failed attempting to join a centralized security network.

	BDB_COMMISSION_STA_PARENT_LOST,
	BDB_COMMISSION_STA_REJOIN_FAILURE,
	BDB_COMMISSION_STA_FORMATION_DONE,
};
/** @} end of group CommissioningStatus */

enum{
	BDB_INIT_STATUS_SUCCESS = 0,
	BDB_INIT_STATUS_FAILURE,
};


/** @addtogroup bdbAttrId
 *  Definition for bdb attribute identifier
 * @{
 */
enum{
	BDB_ATTR_PRIMARY_CHANNEL_SET = 0x60,	//!< attribute Identifier: primaryChannelSet
	BDB_ATTR_SECONDARY_CHANNEL_SET,			//!< attribute Identifier: secondaryChannelSet
	BDB_ATTR_COMMISSIONING_GROUP_ID,		//<! attribute Identifier: commissioningGroupId
	BDB_ATTR_COMMISSIONING_MODE,			//<! attribute Identifier: commissioningMode
	BDB_ATTR_COMMISSIONING_STATUS,			//<! attribute Identifier: commissioningStatus
	BDB_ATTR_JOINING_NODE_EUI64,			//<! attribute Identifier: joiningNodeEui64
	BDB_ATTR_JOINING_NODE_TC_LNKKEY, 		//!< attribute Identifier: joiningNodeNewTCLinkKey
	BDB_ATTR_COMMISSIONING_CAP,				//!< attribute Identifier: nodeCommissioningCapability
	BDB_ATTR_JOIN_USE_INSTALL_CODE_KEY,		//!< attribute Identifier: joinUsesInstallCodeKey
	BDB_ATTR_NODE_IS_ON_NETWORK, 			//!< attribute Identifier: nodeIsOnANetwork
	BDB_ATTR_NODE_JOIN_LNKKEY_TYPE,			//!< attribute Identifier: nodeJoinLinkKeyType
	BDB_ATTR_TC_REQUIRE_KEY_EXCHANGE,		//!< attribute Identifier: tcRequireKeyExchange
	BDB_ATTR_SCAN_DURATION,					//!< attribute Identifier: scanDuration
	BDB_ATTR_TC_LNKKEY_EXCHANGE_ATTEMPT,	//!< attribute Identifier: tcLinkKeyExchangeAttempts
	BDB_ATTR_TC_LNKKEY_EXCHANGE_ATTEMPT_MAX,//!< attribute Identifier: tcLinkKeyExchangeAttemptsMax
	BDB_ATTR_TC_LNKKEY_EXCHANGE_METHOD,		//!< attribute Identifier: tcLinkKeyExchangeMethod
	BDB_ATTR_TC_NODE_JOIN_TIMEOUT			//!< attribute Identifier: tcNodeJoinTimeout
};
/** @} end of group bdbAttrId */

enum{
	BDB_STATE_IDLE = 0,
	BDB_STATE_INIT,
	BDB_STATE_COMMISSIONING_TOUCHLINK,
	BDB_STATE_COMMISSIONING_NETWORK_STEER,
	BDB_STATE_COMMISSIONING_NETWORK_FORMATION,
	BDB_STATE_COMMISSIONING_FINDORBIND,
	BDB_STATE_COMMISSIONING_BUSY,
	BDB_STATE_REJOIN_DONE,
};


enum{
	BDB_EVT_IDLE,
	BDB_EVT_INIT_DONE,
	BDB_EVT_COMMISSIONING_START,
	BDB_EVT_COMMISSIONING_TOUCHLINK_NO_RESP,
	BDB_EVT_COMMISSIONING_TOUCHLINK_FINISH,
	BDB_EVT_COMMISSIONING_NETWORK_STEER_PERMITJOIN,
	BDB_EVT_COMMISSIONING_NETWORK_STEER_RETRIEVE_TCLINK_KEY,
	BDB_EVT_COMMISSIONING_NETWORK_STEER_FINISH,
	BDB_EVT_COMMISSIONING_NETWORK_FORMATION_PERMITJOIN,
	BDB_EVT_COMMISSIONING_NETWORK_FORMATION_FINISH,
	BDB_EVT_COMMISSIONING_FINDORBIND_FINISH,
	BDB_EVT_COMMISSIONING_FINDORBIND_SIMPLE_DESC_REQ,
};


/**
 *  Definition for network type
 *
 */
enum{
	NETWORK_TYPE_DISTRIBUTED,
	NETWORK_TYPE_CENTRALIZED,
};

/**
 *  Definition for the role during commissioning
 *
 */
enum{
	BDB_COMMISSIONING_ROLE_NONE = 0,
	BDB_COMMISSIONING_ROLE_INITIATOR,
	BDB_COMMISSIONING_ROLE_TARGET
};

/** @addtogroup bdbAttributeIdentifierStruct
 *  Definition struct for bdb Attribute Identifier table
 * @{
 */
typedef struct{
	u8 offset;			//!< the offset of the attribute in the bdbAttribute struct
	u8 len;				//!< the length of the attribute in the bdbAttribute struct
}bdb_attrTbl_t;
/** @} end of bdbAttributeIdentifierStruct */

/** @addtogroup bdbCommissioningMode
 *  Definition for Commissioning Mode.
 * @{
 */
typedef struct{
	u8 touchlink:1;			//<! whether attempt Touchlink commissioning
	u8 networkSteer:1; 		//<! whether attempt network steering
	u8 networkFormation:1;	//<! whether attempt to form a network
	u8 findOrBind:1;		//<! whether attempt finding & binding
	u8 resv:4;				//<! reserved
}bdb_commissioningMode_t;
/** @} end of group bdbCommissioningMode */


/** @addtogroup NodeCommissioningCapability
 *  Definition for commissioning capabilities struct.
 * @{
 */
typedef enum{
	BDB_NODE_COMMISSION_CAP_STEER = (1 << 0),
	BDB_NODE_COMMISSION_CAP_FORMATION = (1 << 1),
	BDB_NODE_COMMISSION_CAP_FINDORBIND = (1 << 2),
	BDB_NODE_COMMISSION_CAP_TOUCHLINK = (1 << 3),
}bdb_nodeCommissioningCap_e;
/** @} end of group NodeCommissioningCapability */


/** @addtogroup zb_bdb_attr BDB Attributes
 *  Definition for Attributes for bdb
 * @{
 */
typedef struct{
	u32 primaryChannelSet;		//!< the channel set, defined by the application
	u32	secondaryChannelSet;	//!< the channel set, defined by the application
	u16	commissioningGroupId;	//<! the identifier of the group on which the initiator applies finding & binding.
	bdb_commissioningMode_t	commissioningMode;		//<! the commissioning methods and options taken when commissioning is invoked
	u8	commissioningStatus;	//<! the status of its commissioning attempt
	addrExt_t joiningNodeEui64;	//<! the EUI-64 of the node joining the centralized security network
	u8	joiningNodeNewTCLinkKey[16]; 	//!< the new link key established with the joining node but which has not yet been confirmed
	bdb_nodeCommissioningCap_e	nodeCommissioningCapability;	//!< the commissioning capabilities of the node
	u8	joinUsesInstallCodeKey;		//!< indicates whether it requires an install code derived preconfigured link key to be preinstalled, only for ZC
	u8	nodeIsOnANetwork;				//!< indicates whether a node is joined to a network
	u8  nodeJoinLinkKeyType;			//!< indicates the type of link key,only for ZR/ZED
	u8	tcRequireKeyExchange;			//!< whether the Trust Center requires a joining device to exchange its initial link key with a new link key generated by the Trust Center
	u8	scanDuration;					//!< the duration of an IEEE 802.15.4 scan operation per channel
	u8  tcLinkKeyExchangeAttempts;		//!< the number of key establishment attempts that have been made to establish a new link key after joining
	u8  tcLinkKeyExchangeAttemptsMax;	//!< the maximum number of key establishment attempts that will be made before giving up on the key establishment
	u8	tcLinkKeyExchangeMethod;		//!< the method used to establish a new link key
	u8  tcNodeJoinTimeout;		//<! a timeout in seconds for the Trust Center to remove the Trust Center link key f the newly joined node that did not successfully establish a new link key
	u8  resv[2];
}bdb_attr_t;
/** @} end of group zb_bdb_attr */


#define FIND_BIND_QUEUE_SIZE	5

typedef struct{
	u16	addr;
	u8	endpoint;
	u8  resv;
}findBindDst_t;

typedef struct{
	findBindDst_t findBindDstInfo[FIND_BIND_QUEUE_SIZE];
	u8	front;
	u8	rear;
}findBindQ_t;


typedef void (*bdb_initAppCb_t)(u8 status, u8 joinedNetwork);
typedef void (*bdb_commissioningAppCb_t)(u8 status, void *arg);
typedef void (*bdb_zclIdentifyCb_t)(u8 endpoint, u16 srcAddr, u16 identifyTime);
typedef void (*bdb_findBindSuccessCb_t)(findBindDst_t *pDstInfo);

/*
 * @brief  Definition for zll commissioning type
 *
 * */
typedef struct{
	bdb_initAppCb_t 			 bdbInitCb;
	bdb_commissioningAppCb_t	 bdbcommissioningCb;
	bdb_zclIdentifyCb_t			 bdbIdentifyCb;
	bdb_findBindSuccessCb_t		 bdbFindBindSuccessCb;
}bdb_appCb_t;



typedef struct{
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 *key;	/* the key used */
}bdb_linkKey_t;

typedef struct{
	bdb_linkKey_t tcLinkKey;			/* pre-configure tc link key for central network:
											global tc link key: ZC/ZR/ZED;
											unique tc link key: only for ZR/ZED,
											if ZC is using unique link key, need to access paieKeySet to set/get link key */
	bdb_linkKey_t distributeLinkKey;	/* link key for distribute network, for distribute network  */
	bdb_linkKey_t touchLinkKey;			/* touch link  key, for distribute network */
}bdb_linkKey_info_t;


/** @addtogroup bdb_joinNetworkkSetting_t BDB commissioning settings
 *  Definition for commissioning settings for bdb
 * @{
 */
typedef struct{
	u8 usedInstallCode;						/* only for ZC  */
	bdb_linkKey_info_t linkKey;

#if 0
	u8 distributeLinkKeyMode;  	/* distribute link key mode: MASTER_KEY or CERTIFICATION_KEY */
	u8 linkKeyType;				/* linkKeyType: Default global Trust Center link key,
								   used for the node which is ready to form a network  */
#endif

	u8 touchlinkEnable;			/* enable/disable touch-link */
	u8 touchlinkChannel;		/* operation channel for touch-link target */
	u8 touchlinkLqiThreshold;	/* threshold for touch-link scan req/resp command */
}bdb_commissionSetting_t;
/** @} end of group bdb_joinNetworkkSetting_t */


/** @addtogroup zb_bdb_ctx BDB context
 *  Definition for context for bdb
 * @{
 */
typedef struct{
	af_simple_descriptor_t *simpleDesc;
	bdb_commissionSetting_t *commisionSettings;
	union{
		ev_time_event_t *identifyTimer;
		ev_time_event_t *retrieveTcLkKeyTimer;
	};
	bdb_appCb_t *bdbAppCb;
	findBindQ_t	*pFindBindQ;
	u16 *clusterList;
	u16	*matchClusterList;
	findBindDst_t findDstInfo;
	findBindDst_t bindDstInfo;
	u8 clusterNum;
	u8 matchClusterNum;

	u8 networkType;
	u8 state;
	u8 status;
	u8 role;
	u8 channel;
	u8 inited:1;
	u8 factoryNew:1;
	u8 leaveDoing:1;
	u8 resv:5;
	u8 initResult;
}bdb_ctx_t;
/** @} end of group zb_bdb_ctx */


extern bdb_attr_t g_bdbAttrs;
extern bdb_ctx_t  g_bdbCtx;

#define	BDB_ATTR()		g_bdbAttrs

#define PRE_INSTALL_KEY_ENABLE(type)		do{ \
												g_bdbAttrs.joinUsesInstallCodeKey = 1;  \
												g_bdbAttrs.nodeJoinLinkKeyType = type; \
											}while(0)


#define NODE_COMMISSIONING_CAPABILITY_SET(v)  	g_bdbAttrs.nodeCommissioningCapability = v;

#define BDB_STATE_SET(v)						g_bdbCtx.state = v
#define BDB_STATE_GET()							g_bdbCtx.state

#define BDB_STATUS_SET(v)						g_bdbCtx.status = v
#define BDB_STATUS_GET()						g_bdbCtx.status

#define BDB_ATTR_GROUP_ID_SET(v)				g_bdbAttrs.commissioningGroupId = v

/**
 * @brief       bdb initialization
 *
 * @param[in]   simple_desc 		- the pointer to device's simple description
 * @param[in]   setting				- bdb commissioning settings
 * @param[in]   cb 					- call back for bdb
 * @param[in]   repower 			- 1: real power-on, 0: from deep sleep
 *
 * @return      None
 */
u8 bdb_init(af_simple_descriptor_t *simple_desc, bdb_commissionSetting_t *setting, bdb_appCb_t *cb, u8 repower);

/**
 * @brief      commissioning: form a network
 *
 * @param[in]   none
 * @param[in]   repower 			- 1: real power-on, 0: from deep sleep
 *
 * @return      None
 */
u8 bdb_networkFormationStart(void);

/**
 * @brief      commissioning: steer a network
 *
 * @param[in]   none
 *
 * @return      None
 */
u8 bdb_networkSteerStart(void);

/**
 * @brief      commissioning: touch-link, only used for touch-link initiator,
 * 				for touch-link target, only need to call bdb_touchlinkEnable() before bdb_init()
 *
 * @param[in]   role: BDB_COMMISSIONING_ROLE_INITIATOR or BDB_COMMISSIONING_ROLE_TARGET
 *
 * @return      None
 */
u8 bdb_networkTouchLinkStart(u8 role);

/**
 * @brief      commissioning: finding & binding
 * 			   initiator: it will be timeout if it can't receive identify response within 5s
 * 			   target:	  it will close identify timer after 3mins
 *
 * @param[in]   role: BDB_COMMISSIONING_ROLE_INITIATOR or BDB_COMMISSIONING_ROLE_TARGET
 *
 * @return      None
 */
u8 bdb_findAndBindStart(u8 role);

/**
 * @brief      bdb finding and binding match cluster list set, only for initiator
 *
 * @param[in]   clusterNum:
 * @param[in]   clusterId:
 *
 * @return      None
 */
void bdb_findBindMatchClusterSet(u8 clusterNum, u16 clusterId[]);

/**
 * @brief      bdb finding and binding active endpoint add, only for initiator
 *
 * @param[in]   dstInfo
 *
 * @return      None
 */
bool bdb_addIdentifyActiveEpForFB(findBindDst_t dstInfo);

/**
 * @brief      bdb default reporting config
 *
 * @param[in]   endpoint:
 * @param[in]   profileID:
 * @param[in]   clusterID:
 * @param[in]   attrID:
 * @param[in]   minReportInt:
 * @param[in]   maxReportInt:
 * @param[in]   reportableChange:
 *
 * @return      status_t
 */
status_t bdb_defaultReportingCfg(u8 endpoint, u16 profileID, u16 clusterID, u16 attrID, u16 minReportInt, u16 maxReportInt, u8 *reportableChange);

/*
 * @fn      bdb_zdoStartDevCnf
 *
 * @brief	node start confirm handle
 *
 * @param 	startDevCnf
 *
 */
void bdb_zdoStartDevCnf(void *arg);   //zdo_start_device_confirm_t* startDevCnf);

/**
 * @brief      bdb attribute initialization
 *
 * @param[in]   none
 *
 * @return      None
 */
void tl_bdbAttrInit(void);

/**
 * @brief      bdb use install code
 *
 * @param[in]   pInstallCode
 * @param[out]  pKey
 *
 * @return      None
 */
void tl_bdbUseInstallCode(u8 *pInstallCode, u8 *pKey);

/**
 * @brief      bdb re-start
 *
 * @param[in]   none
 *
 * @return      None
 */
void tl_bdbReset(void);

/**
 * @brief      reset to factory-new device
 *
 * @param[in]   none
 *
 * @return      None
 */
void tl_bdbReset2FN(void);

/**
 * @brief      bdb_retrieveTcLinkKeyDone
 *
 * @param[in]   status
 *
 * @return      None
 */
void bdb_retrieveTcLinkKeyDone(u8 status);

/**
 * @brief	check the bdb state
 *
 * @param   1: idle, 0: busy
 *
 * @return
 */
bool bdb_isIdle(void);

/**
 * @brief      bdb_linkKeyCfg
 *
 * @param[in]   setting
 * @param[in]   isFactoryNew
 *
 * @return      None
 */
void bdb_linkKeyCfg(bdb_commissionSetting_t *setting, u8 isFactoryNew);

/** @} end of group zb_bdb_attr */

/** @} end of group TELINK_ZIGBEE_INTERPAN */

/** @} end of group TELINK_ZIGBEE_STACK */

#endif
