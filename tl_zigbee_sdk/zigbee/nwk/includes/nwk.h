/********************************************************************************************************
 * @file    nwk.h
 *
 * @brief   This is the header file for nwk
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
#ifndef NWK_H
#define NWK_H


/***************************************************************************
* @brief	NWK Mesh route stuff: routing table size
*/
#if ZB_COORDINATOR_ROLE
#ifndef NWK_ROUTE_RECORD_TABLE_NUM
    #define NWK_ROUTE_RECORD_TABLE_NUM          128
#endif
#endif

#if ZB_ROUTER_ROLE
#ifndef ROUTING_TABLE_NUM
    #define ROUTING_TABLE_NUM                   48
#endif

#ifndef NWK_BRC_TRANSTBL_NUM
    #define NWK_BRC_TRANSTBL_NUM                12//8
#endif
#endif

#define ZB_NWK_IS_ADDRESS_BROADCAST(addr)       ( ((addr) & 0xFFF8) == 0xFFF8 )

/***************************************************************************
* @brief	The maximum number of retries allowed after a broadcast transmission failure
*/
#define NWK_MAX_BROADCAST_RETRIES               3

#define NWK_ENDDEV_TIMEOUT_DEFAULT              8//REQTIMEOUTENUM_256_MINUTES

/***************************************************************************
* @brief	The maximum time duration in OctetDurations allowed for the parent and all
* 			child devices to retransmit a broadcast message (passive ack timeout)
*/
#define NWK_PASSIVE_ACK_TIMEOUT                 500000//us

/***************************************************************************
* @brief	The maximum broadcast jitter time measured in milliseconds
*/
#define NWK_MAX_BROADCAST_JITTER                0x40//ms

/***************************************************************************
* @brief	The broadcast delivery time, ms*
* 		nwkBroadcastDeliveryTime =  2*nwkMaxDepth*
* 					((0.05+(nwkcMaxBroadcastJitter/2))+
* 					nwkPassiveAckTimeout*nwkBroadcastRetries/
* 					1000)
*
* 	nwkcMaxBroadcastJitter :	ms
* 	nwkPassiveAckTimeout:		us
* 	nwkBroadcastDeliveryTime: 	ms
*/
#if 0
//#define NWK_BROADCAST_DELIVERY_TIME           (2*NWK_NIB().maxDepth*(((50+(NWK_MAX_BROADCAST_JITTER*1000)/2)) + NWK_NIB().passiveAckTimeout*NWK_MAX_BROADCAST_RETRIES)/1000)
#define NWK_BROADCAST_DELIVERY_TIME             (2*NWK_MAX_DEPTH*(((50+(NWK_MAX_BROADCAST_JITTER*1000)/2)) + NWK_PASSIVE_ACK_TIMEOUT*NWK_MAX_BROADCAST_RETRIES)/1000)
#else
#define NWK_BROADCAST_DELIVERY_TIME             5000//ms
#endif

/***************************************************************************
* @brief	The number of times the first broadcast route request command
*/
#define NWK_INITIAL_RREQ_RETRIES                3

/***************************************************************************
* @brief	The number of times the broadcast transmission of a route request
* 			command frame is retries on relay by an intermediate ZigBee router
* 			or ZigBee coordinator
*/
#define NWK_RREQ_RETRIES                        2

/***************************************************************************
 *  @brief  NWK: size os the long-short panid translation table
 */
#define PANID_TABLE_SIZE                        8

/****************************************************************************
* @brief	Network layer primitive id
*/
#define NWK_NLDE_DATA_REQ                       0x70 //!< NLDE Data Request
#define NWK_NLDE_DATA_CONF                      0x71 //!< NLDE Data Confirm
#define NWK_NLDE_DATA_IND                       0x72 //!< NLDE Data Indication
#define NWK_NLME_NWK_DISCOVERY_REQ              0x73 //!< NLME Network Discovery Request
#define NWK_NLME_NWK_DISCOVERY_CNF              0x74 //!< NLME Network Discovery Confirm
#define NWK_NLME_NWK_FORMATION_REQ              0x75 //!< NLME Network Formation Request
#define NWK_NLME_NWK_FORMATION_CNF              0x76 //!< NLME Network Formation Confirm
#define NWK_NLME_PERMIT_JOINING_REQ             0x77 //!< NLME Permit Joining Request
#define NWK_NLME_PERMIT_JOINING_CNF             0x78 //!< NLME Permit Joining Confirm
#define NWK_NLME_START_ROUTER_REQ               0x79 //!< NLME Start Router Request
#define NWK_NLME_START_ROUTER_CNF               0x7a //!< NLME Start Router Confirm
#define NWK_NLME_ED_SCAN_REQ                    0x7b //!< NLME ED Scan Request
#define NWK_NLME_ED_SCAN_CNF                    0x7c //!< NLME ED Scan Confirm
#define NWK_NLME_JOIN_REQ                       0x7d //!< NLME Join Request
#define NWK_NLME_JOIN_IND                       0x7e //!< NLME Join Indication
#define NWK_NLME_JOIN_CNF                       0x7f //!< NLME Join Confirm
#define NWK_NLME_DIRECT_JOIN_REQ                0x80 //!< NLME Direct Join Request
#define NWK_NLME_DIRECT_JOIN_CNF                0x81 //!< NLME Direct Join Confirm
#define NWK_NLME_LEAVE_REQ                      0x82 //!< NLME Leave Request
#define NWK_NLME_LEAVE_IND                      0x83 //!< NLME Leave Indication
#define NWK_NLME_LEAVE_CNF                      0x84 //!< NLME Leave Confirm
#define NWK_NLME_RESET_REQ                      0x85 //!< NLME Reset Request
#define NWK_NLME_RESET_CNF                      0x86 //!< NLME Reset Confirm
#define NWK_NLME_SYNC_REQ                       0x87 //!< NLME SYNC Request
#define NWK_NLME_SYNC_LOSS_IND                  0x88 //!< NLME SYNC LOSS Indication
#define NWK_NLME_SYNC_CNF                       0x89 //!< NLME SYNC Confirm
#define NWK_NLME_GET_REQ                        0x8a //!< NLME Get Request
#define NWK_NLME_GET_CNF                        0x8b //!< NLME Get Confirm
#define NWK_NLME_SET_REQ                        0x8c //!< NLME Set Request
#define NWK_NLME_SET_CNF                        0x8d //!< NLME Set Confirm
#define NWK_NLME_NWK_STATUS_IND                 0x8e //!< NLME Network Status Indication
#define NWK_NLME_ROUTE_DISCOVERY_REQ		0x8f //!< NLME Route Discovery Request
#define NWK_NLME_ROUTE_DISCOVERY_CNF		0x90 //!< NLME Route Discovery Confirm


/****************************************************************************
* @brief	Network layer command handle
*/
typedef enum {
    NWK_INTERNAL_NSDU_HANDLE                    = 0xC0,
    NWK_INTERNAL_DATA_RECEIVED_HANDLE,
    NWK_INTERNAL_LINK_STATUS_CMD_HANDLE,
    NWK_INTERNAL_REJOIN_REQ_CMD_HANDLE,
    NWK_INTERNAL_REJOIN_RESP_CMD_HANDLE,
    NWK_INTERNAL_NETWORK_UPDATE_CMD_PAN_ID_UPDATE_HANDLE,
    NWK_INTERNAL_LEAVE_REQ_CMD_HANDLE,
    NWK_INTERNAL_LEAVE_REQ_CMD_INDIRECT_HANDLE,
    NWK_INTERNAL_NETWORK_REPORT_CMD_HANDLE,
    NWK_INTERNAL_NETWORK_STATUS_CMD_HANDLE,
    NWK_INTERNAL_ROUTE_REQ_CMD_HANDLE,
    NWK_INTERNAL_ROUTE_REPLY_CMD_HANDLE,
    NWK_INTERNAL_ROUTE_RECORD_CMD_HANDLE,
    NWK_INTERNAL_ENDDEVTIMEOUT_REQ_CMD_HANDLE,
    NWK_INTERNAL_ENDDEVTIMEOUT_RSP_CMD_HANDLE,
    //MAC_HANDLE_MIN = 0xDF,
} nwk_internal_handle_t;

/****************************************************************************
* @brief   Network device type
*/
typedef enum {
    NWK_DEVICE_TYPE_COORDINATOR,
    NWK_DEVICE_TYPE_ROUTER,
    NWK_DEVICE_TYPE_ED,
} nwk_deviceType_t;

/****************************************************************************
* @brief	Network broadcast addresses types
*/
typedef enum {
    NWK_BROADCAST_ALL_DEVICES           = 0xFFFF, /*!< All devices in PAN */
    NWK_BROADCAST_RESERVED              = 0xFFFE,
    NWK_BROADCAST_RX_ON_WHEN_IDLE       = 0xFFFD, /*!< macRxOnWhenIdle = TRUE */
    NWK_BROADCAST_ROUTER_COORDINATOR    = 0xFFFC, /*!< All routers and coordinator */
    NWK_BROADCAST_LOW_POWER_ROUTER      = 0xFFFB  /*!< Low power routers only */
} nwk_broadcast_address_t;

/**
   NWK address allocation method.
*/
typedef enum {
    NWK_ADDRESS_ALLOC_METHOD_DISTRIBUTED,
    NWK_ADDRESS_ALLOC_METHOD_RESERVER,
    NWK_ADDRESS_ALLOC_METHOD_STOCHASTIC
} nwk_addrAllocMethod_t;

/****************************************************************************
* @brief	NWK layer status values
*/
typedef enum {
    NWK_STATUS_SUCCESS                  = 0x00,
    NWK_STATUS_ADDR_CONFLICT            = 0x01,
    NWK_STATUS_INVALID_PARAMETER        = 0xC1,
    NWK_STATUS_INVALID_REQUEST          = 0xC2,
    NWK_STATUS_NOT_PERMITTED            = 0xC3,
    NWK_STATUS_STARTUP_FAILURE          = 0xC4,
    NWK_STATUS_ALREADY_PRESENT          = 0xC5,
    NWK_STATUS_SYNC_FAILURE             = 0xC6,
    NWK_STATUS_NEIGHBOR_TABLE_FULL      = 0xC7,
    NWK_STATUS_UNKNOWN_DEVICE           = 0xC8,
    NWK_STATUS_UNSUPPORTED_ATTRIBUTE    = 0xC9,
    NWK_STATUS_NO_NETWORKS              = 0xCA,
    NWK_STATUS_MAX_FRM_COUNTER          = 0xCC,
    NWK_STATUS_NO_KEY                   = 0xCD,
    NWK_STATUS_BAD_CCM_OUTPUT           = 0xCE,
    NWK_STATUS_NO_ROUTING_CAPACITY      = 0xCF,
    NWK_STATUS_ROUTE_DISCOVERY_FAILED   = 0xD0,
    NWK_STATUS_ROUTE_ERROR              = 0xD1,
    NWK_STATUS_BT_TABLE_FULL            = 0xD2,
    NWK_STATUS_FRAME_NOT_BUFFERED       = 0xD3,
    NWK_STATUS_DECRYPT_ERROR            = 0xD4
} zb_nwk_status_t;

/****************************************************************************
* @brief	status codes for Network status command frame
*/
typedef enum {
    NWK_COMMAND_STATUS_NO_ROUTE_AVAILABLE               = 0x00, /*!< No route available */
    NWK_COMMAND_STATUS_TREE_LINK_FAILURE                = 0x01, /*!< Tree link failure */
    NWK_COMMAND_STATUS_NONE_TREE_LINK_FAILURE           = 0x02, /*!< None-tree link failure */
    NWK_COMMAND_STATUS_LOW_BATTERY_LEVEL                = 0x03, /*!< Low battery level */
    NWK_COMMAND_STATUS_NO_ROUTING_CAPACITY              = 0x04, /*!< No routing capacity */
    NWK_COMMAND_STATUS_NO_INDIRECT_CAPACITY             = 0x05, /*!< No indirect capacity */
    NWK_COMMAND_STATUS_INDIRECT_TRANSACTION_EXPIRY      = 0x06, /*!< Indirect transaction expiry */
    NWK_COMMAND_STATUS_TARGET_DEVICE_UNAVAILABLE        = 0x07, /*!< Target device unavailable */
    NWK_COMMAND_STATUS_TARGET_ADDRESS_UNALLOCATED       = 0x08, /*!< Target address unallocated */
    NWK_COMMAND_STATUS_PARENT_LINK_FAILURE              = 0x09, /*!< Parent link failure */
    NWK_COMMAND_STATUS_VALIDATE_ROUTE                   = 0x0a, /*!< Validate route */
    NWK_COMMAND_STATUS_SOURCE_ROUTE_FAILURE             = 0x0b, /*!< Source route failure */
    NWK_COMMAND_STATUS_MANY_TO_ONE_ROUTE_FAILURE        = 0x0c, /*!< Many-to-one route failure */
    NWK_COMMAND_STATUS_ADDRESS_CONFLICT                 = 0x0d, /*!< Address conflict */
    NWK_COMMAND_STATUS_VERIFY_ADDRESS                   = 0x0e, /*!< Verify address */
    NWK_COMMAND_STATUS_PAN_IDENTIFIER_UPDATE            = 0x0f, /*!< Pan identifier update */
    NWK_COMMAND_STATUS_NETWORK_ADDRESS_UPDATE           = 0x10, /*!< Network address update */
    NWK_COMMAND_STATUS_BAD_FRAME_COUNTER                = 0x11, /*!< Bad frame counter  */
    NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER          = 0x12  /*!< Bad key sequence number */
} nwk_statusCode_t;

/***************************************************************************
* @brief	NWK NIB Attributes IDs
*/
typedef enum {
    NIB_ATTRIBUTE_SEQUENCE_NUMBER                       = 0x81,
    NIB_ATTRIBUTE_PASSIVE_ASK_TIMEOUT                   = 0x82,
    NIB_ATTRIBUTE_MAX_BROADCAST_RETRIES                 = 0x83,
    NIB_ATTRIBUTE_MAX_CHILDREN                          = 0x84,
    NIB_ATTRIBUTE_MAX_DEPTH                             = 0x85,
    NIB_ATTRIBUTE_MAX_ROUTERS                           = 0x86,
    NIB_ATTRIBUTE_NEIGHBOR_TABLE                        = 0x87,
    NIB_ATTRIBUTE_BROADCAST_DELIVERY_TIME               = 0x88,
    NIB_ATTRIBUTE_REPORT_CONSTANT_COST                  = 0x89,
    NIB_ATTRIBUTE_ROUTE_DISCOVERY_RETRIES_PERMITTED     = 0x8A,
    NIB_ATTRIBUTE_ROUTE_TABLE                           = 0x8B,
    NIB_ATTRIBUTE_SYM_LINK                              = 0x8E,
    NIB_ATTRIBUTE_CAPABILITY_INFORMATION                = 0x8F,
    NIB_ATTRIBUTE_ADDR_ALLOC                            = 0x90,
    NIB_ATTRIBUTE_USE_TREE_ROUTING                      = 0x91,
    NIB_ATTRIBUTE_MANAGER_ADDR                          = 0x92,
    NIB_ATTRIBUTE_MAX_SOURCE_ROUTE                      = 0x93,
    NIB_ATTRIBUTE_UPDATE_ID                             = 0x94,
    NIB_ATTRIBUTE_TRANSACTION_PERSISTENCE_TIME          = 0x95,
    NIB_ATTRIBUTE_NETWORK_ADDRESS                       = 0x96,
    NIB_ATTRIBUTE_STACK_PROFILE                         = 0x97,
    NIB_ATTRIBUTE_BROADCAST_TRANSACTION_TABLE           = 0x98,
    NIB_ATTRIBUTE_GROUP_ID_TABLE                        = 0x99,
    NIB_ATTRIBUTE_EXTENDED_PANID                        = 0x9A,
    NIB_ATTRIBUTE_USE_MULTICAST                         = 0x9B,
    NIB_ATTRIBUTE_ROUTE_RECORD_TABLE                    = 0x9C,
    NIB_ATTRIBUTE_IS_CONCENTRATOR                       = 0x9D,
    NIB_ATTRIBUTE_CONCENTRATOR_RADIUS                   = 0x9E,
    NIB_ATTRIBUTE_CONCENTRATOR_DISCOVERY_TIME           = 0x9F,
    NIB_ATTRIBUTE_SECURITY_LEVEL                        = 0xA0,
    NIB_ATTRIBUTE_SECURITY_MATERIAL_SET                 = 0xA1,
    NIB_ATTRIBUTE_ACTIVE_KEY_SEQ_NUMBER                 = 0xA2,
    NIB_ATTRIBUTE_ALL_FRESH                             = 0xA3,
    NIB_ATTRIBUTE_SECURE_ALL_FRAMES                     = 0xA5,
    NIB_ATTRIBUTE_LINK_STATUS_PERIOD                    = 0xA6,
    NIB_ATTRIBUTE_ROUTER_AGE_LIMIT                      = 0xA7,
    NIB_ATTRIBUTE_UNIQUE_ADDR                           = 0xA8,
    NIB_ATTRIBUTE_ADDRESS_MAP                           = 0xA9,
    NIB_ATTRIBUTE_TIME_STAMP                            = 0x8C,
    NIB_ATTRIBUTE_PAN_ID                                = 0x80,
    NIB_ATTRIBUTE_TX_TOTAL                              = 0x8D,
    NIB_ATTRIBUTE_LEAVEREQALLOWED                       = 0xAA,
    NIB_ATTRIBUTE_PARENT_INFORMATION                    = 0xAB,
    NIB_ATTRIBUTE_END_DEVICE_TIMEOUT_DEFAULT            = 0xAC,
    NIB_ATTRIBUTE_LEAVE_REQ_WITHOUT_REJOIN_ALLOWED      = 0xAD,
    NIB_ATTRIBUTE_IEEE_ADDRESS                          = 0xAE,
} nib_attribute_t;

/***************************************************************************
* @brief	RejoinNetwork
* 			This parameter controls the method of joining the network
*/
typedef enum {
    NLME_REJOIN_METHOD_ASSOCIATION                      = 0x00,
    NLME_REJOIN_METHOD_DIRECT                           = 0x01,
    NLME_REJOIN_METHOD_REJOIN                           = 0x02,
    NLME_REJOIN_METHOD_CHANGE_CHANNEL                   = 0x03
} rejoinNwk_method_t;

/*Network header*/
/***************************************************************************
* @brief	Network header frame type enum
*/
typedef enum {
    FRAME_TYPE_DATA     = 0,
    FRAME_TYPE_COMMAND  = 1,
    FRAME_TYPE_INTERPAN = 3
} frameType_t;

/***************************************************************************
* @brief	Holds the data to assemble the frame control field
*/
typedef struct {
    u8 frameType:2;             /*!< Frame type of this frame */
    u8 protocolVer:4;           /*!< Protocol version of this frame */
    u8 discRoute:2;             /*!< Discover route enable */
    u8 multicastFlg:1;          /*!< Multicast enabled */
    u8 security:1;              /*!< Security enabled */
    u8 srcRoute:1;              /*!< Source route enabled */
    u8 dstIEEEAddr:1;           /*!< Dst IEEE address present */
    u8 srcIEEEAddr:1;           /*!< Src IEEE address present */
    u8 endDevInitiator:1;       /*!< End device initiator */
    u8 reserved:2;
} nwkFrameCtrl_t;

/***************************************************************************
* @brief	Multicast control
*/
typedef struct {
    u8 multicastMode:2;
    u8 nonmemberRadius:3;
    u8 maxNonmemberRadius:3;
} nwkMcastCtrl_t;

/***************************************************************************
* @brief	Source route subframe
*/
typedef struct {
    u8 relayCnt;
    u8 relayIdx;
    u8 *relayList;
} srcRouteSubframe_t;

/***************************************************************************
* @brief	The main network header struct
*/
typedef struct {
    u16 dstAddr;
    u16 srcAddr;

    nwkFrameCtrl_t frameControl;
    u8 radius;
    u8 seqNum;

    extAddr_t dstIeeeAddr;
    extAddr_t srcIeeeAddr;

    nwkMcastCtrl_t mcastControl;
    u8 frameHdrLen;
    srcRouteSubframe_t srcRouteSubframe;
} nwk_hdr_t;

/***************************************************************************
* @brief	NLDE-DATA.request
*/
typedef struct {
    addrExt_t ieeAddr;  //for interPan

    u16 dstAddr;
    u8 radius;
    u8 addrMode;

    u8 nonmemberRadius;
    u8 discoverRoute;
    bool securityEnable;
    u8 ndsuHandle;

    u8 *nsdu;

    u8 nsduLen;
    bool useAlias;
    u16 aliasSrcAddr;

    u8 aliasSeqNum;
    bool unicastSkipRouting;
} nlde_data_req_t;

/***************************************************************************
* @brief	NLDE-DATA.confirm
*/
typedef struct {
    u32 txTime;
    u8 status;
    u8 nsduHandle;
} nlde_data_cnf_t;

/***************************************************************************
* @brief	NLDE-DATA.indication
*/
typedef struct {
    u8 *nsdu;

    u8 nsduLen;
    u8 dstAddrMode;
    bool securityUse;
    u8 lqi;

    u16 dstAddr;
    u16 srcAddr;

    u32 rxTime;
    u16 srcMacAddr;
} nlde_data_ind_t;

/***************************************************************************
* @brief	NLME-NETWORK-DISCOVERY.request
*/
typedef struct {
    u32	scanChannels;
    u8 scanDuration;
} nlme_nwkDisc_req_t;

/***************************************************************************
* @brief   Network descriptor.
*		   This is a part of network discovery confirm result.
*/
typedef struct zb_nlme_network_descriptor_s {
    extPANId_t extPANId;

    u8 logicalChannel;
    u8 stackProfile;
    u8 zigbeeVer;
    u8 beaconOrder;

    u8 superframeOrder;
    bool permitJoining;
    bool routerCapacity;
    bool endDevCapacity;
} nwk_descriptor_t;

/***************************************************************************
* @brief	NLME-NETWORK-DISCOVERY.confirm
*/
typedef struct {
    nwk_descriptor_t nwkDescriptor[PANID_TABLE_SIZE];
    u8 status;
    u8 nwkCount;
} nlme_nwkDisc_cnf_t;

/***************************************************************************
* @brief	NLME-NETWORK-FORMATION.request
*/
typedef struct {
    u32 scanChannels;
    u16 distributedNwkAddr;
    bool distributedNetwork;
    bool batteryLifeExt;
    u8 scanDuration;
    u8 beaconOrder;
    u8 superframeOrder;
} nlme_nwkFormation_req_t;

/***************************************************************************
* @brief	NLME-NETWORK-FORMATION.confirm
*/
typedef struct {
    u8 status;
} nlme_nwkFormation_cnf_t;

/***************************************************************************
* @brief	NLME-PERMIT_JOINING.request
*/
typedef struct {
    u8 permitDuration;
} nlme_permitJoining_req_t;

/***************************************************************************
* @brief	NLME-PERMIT_JOINING.confirm
*/
typedef struct {
    u8 status;
} nlme_permitJoining_cnf_t;

/***************************************************************************
* @brief	NLME-START-ROUTER.request
*/
typedef struct {
    u8 beaconOrder;
    u8 superframeOrder;
    bool batteryLifeExt;
} nlme_startRouter_req_t;

/***************************************************************************
* @brief	NLME-START-ROUTER.confirm
*/
typedef struct {
    u8 status;
} nlme_startRouter_cnf_t;

/***************************************************************************
* @brief	NLME-ED-SCAN.request
*/
typedef struct {
    u32 scanChannels;
    u8 scanDuration;
} nlme_edScan_req_t;

/***************************************************************************
* @brief	NLME-ED-SCAN.confirm
*/
typedef struct {
    u8 status;
    u8 edList[MAX_ED_SCAN_RESULTS_SUPPORTED];
} nlme_edScan_cnf_t;

/***************************************************************************
* @brief	CapabilityInformation
* @brief	The operating capabilities of the device
*/
#if 0
typedef struct {
    u8 altPanCoord:1;
    u8 devType:1;
    u8 powerSrc:1;
    u8 rcvOnWhenIdle:1;
    u8 reserved:2;
    u8 secuCapability:1;
    u8 allocAddr:1;
} capability_info_t;
#endif

/***************************************************************************
* @brief	NLME-JOIN.request
*/
typedef struct {
    extPANId_t extPANId;
    u32 scanChannels;
    u8 rejoinNwk;//rejoinNwk_method_t
    u8 scanDuration;
    capability_info_t capabilityInfo;
    bool securityEnabled;
} nlme_join_req_t;

/***************************************************************************
* @brief	NLME-JOIN.indication
*/
typedef struct {
    extAddr_t extAddr;
    u16 nwkAddr;
    capability_info_t capabilityInfo;
    u8 rejoinNwk;//rejoinNwk_method_t
    bool secureRejoin;
} nlme_join_ind_t;

/***************************************************************************
* @brief	NLME-JOIN.confirm
*/
typedef struct {
    u16 nwkAddr;
    u8 status;
    u8 activeChannel;
    extPANId_t extPANId;
} nlme_join_cnf_t;

/***************************************************************************
* @brief	NLME-DIRECT-JOIN.request
*/
typedef struct {
    extAddr_t deviceAddr;
    u16 nwkAddr;        //spec no this one
    capability_info_t capabilityInfo;
} nlme_directJoin_req_t;

/***************************************************************************
* @brief	NLME-DIRECT-JOIN.confirm
*/
typedef struct {
    extAddr_t deviceAddr;
    u8 status;
} nlme_directJoin_cnf_t;

/***************************************************************************
* @brief	NLME-LEAVE.request
*/
typedef struct {
    extAddr_t deviceAddr;
    bool removeChildren;
    bool rejoin;
} nlme_leave_req_t;

/***************************************************************************
* @brief	NLME-LEAVE.indication
*/
typedef struct {
    //sending device Address
    extAddr_t deviceAddr;
    bool rejoin;
} nlme_leave_ind_t;

/***************************************************************************
* @brief	NLME-LEAVE.confirm
*/
typedef struct {
    extAddr_t deviceAddr;
    u8 status;
} nlme_leave_cnf_t;

/***************************************************************************
* @brief	NLME-RESET.request
*/
typedef struct {
    bool warmStart;
} nlme_reset_req_t;

/***************************************************************************
* @brief	NLME-RESET.confirm
*/
typedef struct {
    u8 status;
} nlme_reset_cnf_t;

/***************************************************************************
* @brief	NLME-SYNC.request
*/
typedef struct {
    bool track;
} nlme_sync_req_t;

/***************************************************************************
* @brief	NLME-SYNC.confirm
*/
typedef struct {
    u8 status;
} nlme_sync_cnf_t;

/***************************************************************************
* @brief	NLME-GET.request
*/
typedef struct {
    u8 nibAttr;//nib_attribute_t
} nlme_get_req_t;

/***************************************************************************
* @brief	NLME-GET.confirm
*/
typedef struct {
    u8 status;
    u8 nibAttr;//nib_attribute_t
    u16 nibAttrLen;
    u8 *nibAttrVal;
} nlme_get_cnf_t;

/***************************************************************************
* @brief	NLME-SET.request
*/
typedef struct {
    u8 *nibAttrVal;
    u16 nibAttrLen;
    u8 nibAttr;//nib_attribute_t
} nlme_set_req_t;

/***************************************************************************
* @brief	NLME-SET.confirm
*/
typedef struct {
    u8 status;
    u8 nibAttr;//nib_attribute_t
} nlme_set_cnf_t;

/***************************************************************************
* @brief	NLME-NWK-STATUS.indication
*/
typedef struct {
    u16 nwkAddr;
    u8 status;//nwk_statusCode_t
} nlme_nwkStatus_ind_t;

/***************************************************************************
* @brief	NLME-ROUTE-DISCOVERY.request
*/
typedef struct {
    u16 dstAddr;
    u8 dstAddrMode;//zb_addr_mode_t
    u8 radius;
    bool noRouteCache;
} nlme_routeDisc_req_t;

/***************************************************************************
* @brief	NLME-ROUTE-DISCOVERY.confirm
*/
typedef struct {
    u8 status;
    u8 nwkStatusCode;//nwk_statusCode_t
} nlme_routeDisc_cnf_t;

/****************************************************************************
* @brief	Network command id
*/
typedef enum {
    NWK_CMD_ROUTE_REQUEST               = 0x01,
    NWK_CMD_ROUTE_REPLY                 = 0x02,
    NWK_CMD_NETWORK_STATUS              = 0x03,
    NWK_CMD_LEAVE                       = 0x04,
    NWK_CMD_ROUTE_RECORD                = 0x05,
    NWK_CMD_REJOIN_REQUEST              = 0x06,
    NWK_CMD_REJOIN_RESPONSE             = 0x07,
    NWK_CMD_LINK_STATUS                 = 0x08,
    NWK_CMD_NETWORK_REPORT              = 0x09,
    NWK_CMD_NETWORK_UPDATE              = 0x0a,
    NWK_CMD_ENDDEVTIMEOUT_REQUEST       = 0x0b,
    NWK_CMD_ENDDEVTIMEOUT_RESPONSE      = 0x0c,
} nwk_cmdId_t;

/****************************************************************************
* @brief	Route request command payload
*/
typedef struct {
    u8 reservedL:3;
    u8 manyToOne:2;
    u8 dstIeeeAddr:1;
    u8 multicast:1;
    u8 reservedH:1;
} rreq_opt_t;

typedef struct {
    extAddr_t dstIeeeAddr;

    rreq_opt_t options;
    u8 routeReqId;
    u16 dstAddr;

    u8 pathCost;
} nwkCmd_routeReq_t;

/****************************************************************************
* @brief	Route reply command payload
*/
typedef struct {
    extAddr_t originatorIeeeAddr;
    extAddr_t responderIeeeAddr;
    u16 originatorAddr;
    u16 responderAddr;
    struct {
        u8 reservedL:4;
        u8 originatorIeeeAddr:1;
        u8 responderIeeeAddr:1;
        u8 multicast:1;
        u8 reservedH:1;
    } options;
    u8 routeReqId;
    u8 pathCost;
} nwkCmd_routeReply_t;

/****************************************************************************
* @brief	Network status command payload
*/
typedef struct {
    u16 dstAddr;
    u8 statusCode;//nwk_statusCode_t
} nwkCmd_nwkStatus_t;

/****************************************************************************
* @brief	Network status command payload
*/
typedef struct {
    u8 cmdId;//nwk_cmdId_t
    u8 statusCode;//nwk_statusCode_t
    u8 srcIEEEAddrPresent;
    u8 dstIEEEAddrPresent;
    extAddr_t dstIeeeAddr;
    u16 dstAddr;
    u8 handle;
    u8 payloadLen;
    u8 payload[1];
} myNwkCmd_nwkStatus_t;


/****************************************************************************
* @brief	Leave command payload
*/
typedef struct {
    struct {
        u8 reserved:5;
        u8 rejoin:1;
        u8 request:1;
        u8 removeChildren:1;
    } options;
} nwkCmd_leave_t;

/****************************************************************************
* @brief	Route record command payload
*/
typedef struct {
    u8 *relayList;
    u8 relayCnt;
} nwkCmd_routeRecord_t;

/****************************************************************************
* @brief	Rejoin request command payload
*/
typedef struct {
    capability_info_t capabilityInfo;
} nwkCmd_rejoinReq_t;

/****************************************************************************
* @brief	Rejoin response command payload
*/
typedef struct {
    u16 nwkAddr;
    u8 rejoinStatus;
} nwkCmd_rejoinResp_t;

/****************************************************************************
* @brief	Link status entry
*/
typedef struct _attribute_packed_ {
    u16 neighborNwkAddr;
    struct {
        u8 incomingCost:3;
        u8 reservedL:1;
        u8 outingCost:3;
        u8 reservedH:1;
    } linkStatus;
} linkStatus_entry_t;

/****************************************************************************
* @brief	Link status command payload
*/
typedef struct {
    linkStatus_entry_t *linkStatusList;
    struct {
        u8 entryCnt:5;
        u8 firstFrame:1;
        u8 lastFrame:1;
        u8 reserved:1;
    } options;
} nwkCmd_linkStatus_t;

/****************************************************************************
* @brief	Network report command payload
*/
typedef struct {
    extPANId_t epid;
    u8 *panIds;
    struct {
        u8 reportInfoCnt:5;
        u8 reportCmdId:3;
    } options;
} nwkCmd_nwkReport_t;

/****************************************************************************
* @brief	Network update command payload
*/
typedef struct {
    extPANId_t epid;
    u16 newPanId;
    struct {
        u8 updateInfoCnt:5;
        u8 updateCmdId:3;
    } options;
    u8 updateId;
} nwkCmd_nwkUpdate_t;


/****************************************************************************
* @brief	Requested timeout enumeration value
*/
typedef enum {
    REQTIMEOUTENUM_10_SECONDS           = 0,
    REQTIMEOUTENUM_2_MINUTES,
    REQTIMEOUTENUM_4_MINUTES,
    REQTIMEOUTENUM_8_MINUTES,
    REQTIMEOUTENUM_16_MINUTES,
    REQTIMEOUTENUM_32_MINUTES,
    REQTIMEOUTENUM_64_MINUTES,
    REQTIMEOUTENUM_128_MINUTES,
    REQTIMEOUTENUM_256_MINUTES,
    REQTIMEOUTENUM_512_MINUTES,
    REQTIMEOUTENUM_1024_MINUTES,
    REQTIMEOUTENUM_2048_MINUTES,
    REQTIMEOUTENUM_4096_MINUTES,
    REQTIMEOUTENUM_8192_MINUTES,
    REQTIMEOUTENUM_16384_MINUTES,
    REQTIMEOUTENUM_INVALID              = 15,
} reqTimeoutEnum_t;

/****************************************************************************
* @brief	Actual timeout value
*/
#define REQUESTED_TIMEOUT_VALUE_GET(idx)        ( (idx) ? (((unsigned int)(1 << (idx))) * 60) : (10) ) //seconds

/****************************************************************************
* @brief	End device timeout request command payload
*/
typedef struct {
    u8 reqTimeoutEnum;//reqTimeoutEnum_t
    u8 endDevCfg;
} nwkCmd_endDevTimeoutReq_t;

/****************************************************************************
* @brief	End device timeout response status
*/
typedef enum {
    TIMEOUT_RSP_STATUS_SUCCESS,
    TIMEOUT_RSP_STATUS_INCORRECT_VALUE,
} endDevTimeoutRspStatus_t;

/****************************************************************************
* @brief	Value of the parent information bitmask
*/
typedef enum {
    MAC_DATA_POLL_KEEPALIVE_BIT         = 1,
    END_DEV_TIMEOUT_REQ_KEEPALIVE_BIT   = 2,
} keepaliveSupportBit_t;

/****************************************************************************
* @brief	End device timeout response command payload
*/
typedef struct {
    u8 status;//endDevTimeoutRspStatus_t
    u8 parentInfo;
} nwkCmd_endDevTimeoutResp_t;

/***************************************************************************
* @brief	Union of all the command frame data structures.
*/
typedef struct {
    u8 cmdId;                                                   /*!< nwk_cmdId_t, Nwk cmd frame command ID */
    u8 reserved[3];
    union {
        nwkCmd_routeReq_t               rreq;                   /*!< Route request command */
        nwkCmd_routeReply_t             rrep;                   /*!< Route reply command */
        nwkCmd_nwkStatus_t              nwkStatus;              /*!< Network status command */
        nwkCmd_leave_t                  leave;                  /*!< Leave command */
        nwkCmd_routeRecord_t            rrec;                   /*!< Route record command */
        nwkCmd_rejoinReq_t              rejoinReq;              /*!< Rejoin request command */
        nwkCmd_rejoinResp_t             rejoinRsp;              /*!< Rejoin response command */
        nwkCmd_linkStatus_t             linkSt;                 /*!< Link status command */
        nwkCmd_nwkReport_t              nwkReport;              /*!< Network report command */
        nwkCmd_nwkUpdate_t              nwkUpdate;              /*!< Network update command */
        nwkCmd_endDevTimeoutReq_t       endDevTimeoutReq;       /*!< End device timeout request command */
        nwkCmd_endDevTimeoutResp_t      endDevTimeoutRsp;       /*!< End device timeout response command */
    };
} nwkCmd_t;

/***************************************************************************
* @brief	Pending table entry
*/
typedef struct {
    void *srcBuf;       //pending source buffer for NWK layer
    u16 srcAddr;        //srcAddr of this packet
    u16 dstAddr;        //dstAddr of this packet
    u8 handle;
    u8 seqNum;
    u8 routeReqId;
    u8 expirationTime:6;
    u8 needRouteDisc:1; //indicate this buffer need route discovery
    u8 used:1;
} nwk_txDataPendEntry_t;

/***************************************************************************
* @brief	broadcast transaction record
*/
typedef struct {
    tl_zb_normal_neighbor_entry_t *edEntry;
    nwk_txDataPendEntry_t *entry;
    ev_timer_event_t *retryTimer;
    u16 *passiveAckAddr;        /* keep track of which of its neighboring devices have successfully relayed.
                                 * it will malloc an ev_buffer to trace, be care of ev_buffer's setting, in ev_buffer.h.
                                 * u16 *passiveAckAddr = (u16 *)ev_buf_allocate(TL_ZB_NEIGHBOR_TABLE_SIZE)
                                 */
    u16 srcAddr;
    u8 seqNum;
    u8 retries;                 /* broadcast retries times */
    u8 activeNum;
    u8 expirationTime;
    u8 status;
    u8 used;
} nwk_brcTransRecordEntry_t;

/***************************************************************************
* @brief	network route state
*/
typedef enum {
    NWK_ROUTE_STATE_ACTIVE,
    NWK_ROUTE_STATE_DISCOVERY_UNDERWAY,
    NWK_ROUTE_STATE_DISCOVERY_FAILED,
    NWK_ROUTE_STATE_DISCOVERY_INACTIVE,
    NWK_ROUTE_STATE_VALIDATION_UNDERWAY,
} nwk_routeState_t;

/***************************************************************************
* @brief	The maximum number of hops in a source route
*/
#define NWK_MAX_SOURCE_ROUTE    0x08

/***************************************************************************
* @brief	network route record table entry
*/
typedef struct {
    u16 nwkAddr;
    u16 path[NWK_MAX_SOURCE_ROUTE];
    u8 relayCnt:7;
    u8 used:1;
    u8 forgetCnt;
} nwk_routeRecordTabEntry_t;

/***************************************************************************
* @brief	network routing table entry
*/
typedef struct {
    u16 dstAddr;
    u16 nextHopAddr;
    u8 status;
    u8 noRouteCache:1;
    u8 manyToOne:1;
    u8 routeRecordRequired:1;
    u8 groupIdFlag:1;
    u8 transFail:4;
    u8 residCost;
    u8 forgetCnt;
} nwk_routingTabEntry_t;

/***************************************************************************
* @brief	Route discovery table entry. It is used to keep track of route
* 			request information during a route discovery operation.
*/
typedef struct {
    void *buf;
    ev_timer_event_t *retryTimer;
    u16 srcAddr;
    u16 senderAddr;
    u16 dstAddr;
    u16 expiry;
    u8 routeReqId;
    u8 forwardCost;
    u8 residCost;
    u8 lastCost;
    u8 retries;
    u8 used;
} nwk_routeDiscEntry_t;

/***************************************************************************
* @brief	Route request table entry. It is used to hold information about
* 			route discoveries in progress.
*/
typedef struct {
    u16 originator;
    u16 interval;
    u8 retries;
    u8 radius;
    u8 seqNum;
    nwkCmd_t routeReqCmd;
} nwk_routeReqEntry_t;



extern u8 NWKC_TRANSFAILURE_CNT_THRESHOLD;
extern u8 NWKC_INITIAL_RREQ_RETRIES;
extern u8 NWKC_RREQ_RETRIES;
extern u16 ROUTING_TABLE_SIZE;
extern u8 NWK_BRC_TRANSTBL_SIZE;
extern bool NWK_BRC_PASSIVE_ACK_ENABLE;
extern u32 NWK_BRC_JITTER;
extern nwk_routingTabEntry_t g_routingTab[];
extern nwk_brcTransRecordEntry_t g_brcTransTab[];
#if ZB_COORDINATOR_ROLE
extern u16 NWK_ROUTE_RECORD_TABLE_SIZE;
extern nwk_routeRecordTabEntry_t g_routeRecTab[];
#endif
extern bool AUTO_QUICK_DATA_POLL_ENABLE;
extern u32 AUTO_QUICK_DATA_POLL_INTERVAL;
extern u8 AUTO_QUICK_DATA_POLL_TIMES;
extern bool PRE_PARENT_FIRST_WHEN_REJOIN;

extern u8 NWK_COST_THRESHOLD_ONEHOP;
extern u8 NWK_NEIGHBOR_SEND_OUTGOING_THRESHOLD;
extern u16 TL_ZB_ASSOCJOIN_FILTER_PANID;
extern u16 TL_ZB_ASSOCJOIN_PERMIT_PANID;
extern u32 LONG_UPTIME_THRESHOLD;
extern bool NWK_HEADER_SRC_IEEE_INCLUDE;

u16 tl_zbNwkStochasticAddrCal(void);
void tl_zbNwkStatusAddrConflictInd(void *arg);
bool tl_zbNwkAddrConflictDetect(void *arg, u16 nwkAddr, addrExt_t ieeeAddr);

void tl_zbNwkAddrMapInit(void);
zb_nwk_status_t tl_zbNwkAddrMapAdd(u16 shortAddr, addrExt_t extAddr, u16 *ref);
u8 tl_idxByShortAddr(u16 *idx, u16 shortAddr);
u8 tl_idxByExtAddr(u16 *idx, addrExt_t extAddr);
void tl_zbNwkAddrMapDelete(u16 idx);
u8 tl_zbShortAddrByExtAddr(u16 *shortAddr, addrExt_t extAddr, u16 *idx);
u8 tl_zbExtAddrByShortAddr(u16 shortAddr, addrExt_t extAddr, u16 *idx);
void tl_zbExtAddrByIdx(u16 idx, addrExt_t extAddr);
u16 tl_zbshortAddrByIdx(u16 idx);

void tl_zbNwkAddrConflictStatusSend(u16 dstAddr, u16 statusAddr, u8 forceSeqNum);
void tl_zbNwkSendNwkStatusCmd(void *arg, nwk_hdr_t *pNwkHdr, nwkCmd_t *pNwkStatus, u8 handle);
void tl_zbNwkNlmeNwkStatusInd(void *arg, u16 nwkAddr, nwk_statusCode_t status);

void tl_zbNwkInit(u8 coldReset);
void tl_zbNwkTaskProc(void);

u8 nwkHdrParse(nwk_hdr_t *pNwkHdr, u8 *msdu);
u8 getNwkHdrSize(nwk_hdr_t *pNwkHdr);
u8 *nwkHdrBuilder(u8 *buf, nwk_hdr_t *pNwkHdr);

void tl_zbNwkLinkStatusStart(void);
void tl_zbNwkLinkStatusStop(void);
void tl_zbNwkSendLinkStatus(void);

void nwkEndDevTimeoutReqSend(reqTimeoutEnum_t reqTimeoutEnum, u8 endDevCfg);
void keepaliveMsgSendStop(void);
/**
   Confirms start procedure.
   @param - reference to buffer.
 */
void tl_zbMacMlmeStartConfirmHandler(void *arg);
void tl_zbMacMlmePollConfirmHandler(void *arg);
void tl_zbMacMcpsDataConfirmHandler(void *arg);
void tl_zbMacMcpsDataIndicationHandler(void *buf);
void tl_zbMacMlmeAssociateConfirmHandler(void *arg);
void tl_zbMacMlmeCommStatusIndicationHandler(void *arg);
void tl_zbMacMlmeScanConfirmHandler(void *arg);
void tl_zbMacMlmeBeaconNotifyIndicationHandler(void *arg);
void tl_zbMacMlmeAssociateIndicationHandler(void *arg);
void tl_zbMacMlmeResetConfirmHandler (void *arg);
void tl_zbMacMlmeOrphanIndicationHandler(void *arg);
void tl_zbMacMlmeSyncLossIndicationHandler(void *arg);
void tl_zbMacMlmePollIndicationHandler(void *arg);


void tl_zbNwkNldeDataRequestHandler(void *arg);
void tl_zbNwkNlmeNetworkFormationRequestHandler(void *arg);
void tl_zbNwkNlmePermitJoiningRequestHandler(void *arg);
void tl_zbNwkNlmeNwkDiscRequestHandler(void *arg);
void tl_zbNwkNlmeStartRouterRequestHandler(void *arg);
void tl_zbNwkNlmeRouteDiscRequestHandler(void *arg);
void tl_zbNwkNlmeJoinRequestHandler(void *arg);
void tl_zbNwkNlmeDirectJoinRequestHandler(void *arg);
void tl_zbNwkNlmeEDScanRequestHandler(void *arg);
void tl_zbNwkNlmeLeaveRequestHandler(void *arg);
void tl_zbNwkNlmeResetRequestHandler(void *arg);
void tl_zbNwkNlmeSyncRequestHandler(void *arg);


/*
 * high layer to NWK layer primitive
 * */

#define tl_zbNwkNldeDataRequest(p)              tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLDE_DATA_REQ, p)
#define tl_zbNwkNlmeNwkDiscRequest(p)           tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_NWK_DISCOVERY_REQ, p)
#define tl_zbNwkNlmeJoinRequest(p)              tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_JOIN_REQ, p)
#define tl_zbNwkNlmeNwkFormationRequest(p)      tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_NWK_FORMATION_REQ, p)
#define tl_zbNwkNlmePermitJoiningRequest(p)     tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_PERMIT_JOINING_REQ, p)
#define tl_zbNwkNlmeRouteDiscRequest(p)         tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_ROUTE_DISCOVERY_REQ, p)
#define tl_zbNwkNlmeStartRouterRequest(p)       tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_START_ROUTER_REQ, p)
#define tl_zbNwkNlmeDirectJoinRequest(p)        tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_DIRECT_JOIN_REQ, p)
#define tl_zbNwkNlmeEDScanRequest(p)            tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_ED_SCAN_REQ, p)
#define tl_zbNwkNlmeLeaveRequest(p)             tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_LEAVE_REQ, p)
#define tl_zbNwkNlmeResetRequest(p)             tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_RESET_REQ, p)
#define tl_zbNwkNlmeSyncRequest(p)              tl_zbPrimitivePost(TL_Q_HIGH2NWK, NWK_NLME_SYNC_REQ, p)



typedef void (*nwkDataIndCb_t)(void *p);
void tl_nwkDataIndRegister(nwkDataIndCb_t cb);



u8 is_device_factory_new(void);

void tl_edBrcDataSkipParentSet(bool skip);

#if ZB_TEST_ENABLE
extern u8 nwkSecurityEn;
#define NWK_SECURITY_EN_SET(v)  nwkSecurityEn = v
#endif


#endif /* NWK_H */
