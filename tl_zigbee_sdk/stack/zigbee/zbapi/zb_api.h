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

typedef enum {
    L_TLV_SELECTED_KEY_NEGO_METHODS     = 0,
    L_TLV_CLEAR_ALL_BIND_REQ            = 0,
    L_TLV_BEACON_SURVEY_CFG             = 0,
    L_TLV_BEACON_SURVEY_RESULTS         = 1,
    L_TLV_POTENTIAL_PARENTS             = 2,
    L_TLV_C25519_PUBLIC_POINT           = 0,
    L_TLV_AUTH_TOKEN_ID                 = 0,
    L_TLV_PROCESSING_STATUS             = 0,
    L_TLV_RELAY_MSG                     = 0,
    L_TLV_APS_FC_CHALLENGE              = 0,
    L_TLV_APS_FC_RESPONSE               = 0,
    L_TLV_LINK_KEY_FEATURES             = 0,
    L_TLV_DECOMM_DEV_LIST               = 0,
    L_TLV_TARGET_IEEE_ADDR              = 0,
    L_TLV_DEVICE_AUTH_LEVEL             = 0,

    G_TLV_MANUFACTURER_SPEC             = 0x40,//64
    G_TLV_SUPPORTED_KEY_NEGO_METHODS    = 0x41,//65
    G_TLV_PAN_ID_CONFLICT_REPORT        = 0x42,//66
    G_TLV_NEXT_PAN_ID_CHANGE            = 0x43,//67
    G_TLV_NEXT_CHANNEL_CHANGE           = 0x44,//68
    G_TLV_SYMMETRIC_PASSPHRASE          = 0x45,//69
    G_TLV_ROUTER_INFO                   = 0x46,//70
    G_TLV_FRAGMENT_PARAM                = 0x47,//71
    G_TLV_JOINER_ENCAP                  = 0x48,//72
    G_TLV_BEACON_APPENDIX_ENCAP         = 0x49,//73
    G_TLV_CONFIG_PARAM                  = 0x4B,//75
    G_TLV_DEVICE_CAPAB_EXT              = 0x4C,//76
} tlvTypeId_e;

typedef struct {
    u8 typeId;//tlvTypeId_e
    u8 len;
    u8 value[];
} tlv_t;


/**
 *  @brief Structure for Clear All Bindings Req EUI64 TLV (ID 0)
 */
typedef struct {
    u8 count;
    extAddr_t devEUI64[];
} tlvClrAllBindReq_t;

/**
 *  @brief Structure for Beacon Survey Configuration TLV (ID 0)
 */
typedef struct {
    u32 channels;
} chnPageList_t;

typedef union {
    u8 byte;
    struct {
        u8 enhancedScan:1;
        u8 reserved:7;
    } bits;
} chnCfgBitmask_t;

typedef struct {
    chnPageList_t *pList;
    u8 chnPageListCnt;
    chnCfgBitmask_t cfg;
} tlvBeaconSurveyCfg_t;

/**
 *  @brief Structure for Beacon Survey Results TLV (ID 0x01)
 */
typedef struct {
    u8 totalReceived;
    u8 onNwkBeacon;
    u8 potentialBeacon;
    u8 otherNwkBeacon;
} tlvBeaconSurveyResult_t;

/**
 *  @brief Structure for Potential Parents TLV(ID 0x02)
 */
typedef struct _attribute_packed_ {
    u16 addr;
    u8 LQA;
} potenParent_t;

typedef struct _attribute_packed_ {
    u16 currParentAddr;
    u8 currParentLQA;
    u8 potentialCnt;
    potenParent_t potenParent[];
} tlvPotentialParents_t;

/**
 *  @brief Structure for Curve25519 Public Point TLV (ID 0)
 */
typedef struct {
    extAddr_t devEUI64;
    u8 publicPoint[32];
} tlvC25519PublicPoint_t;

/**
 *  @brief Structure for Authentication Token ID TLV (ID 0)
 */
typedef struct {
    u8 id;
} tlvAuthTokenId_t;

/**
 *  @brief Structure for Target IEEE Address TLV (ID 0)
 */
typedef struct {
    extAddr_t ieeeAddrOfInterest;
} tlvTargetIeeeAddr_t;

/**
 *  @brief Structure for Device Authentication Level TLV (ID 0)
 */
typedef struct {
    extAddr_t ieeeAddr;
    u8 initJoinMethod;
    u8 activeLinkKeyType;
} tlvDevAuthLevel_t;

/**
 *  @brief Structure for Processing Status TLV (ID 0)
 */
typedef struct {
    u8 tagId;
    u8 status;
} prcStatus_t;

typedef struct {
    u8 count;
    prcStatus_t prcStatus[];
} tlvPrcStatus_t;

/**
 *  @brief Structure for Selected Key Negotiation Method (ID 0)
 */
typedef enum {
    STATIC_KEY,
    C25519_HASH_AES_MMO_128,
    C25519_HASH_SHA_256,
} selKeyNegoProt_e;

typedef enum {
    SYMMETRIC_AUTH_TOKEN,
    INSTALL_CODE_KEY,
    PASSCODE_KEY,
    BASIC_AUTH_KEY,
    ADMIN_AUTH_KEY,
    ANONYMOUS_WELL_KNOWN_SEC = 255,
} preSharedSec_e;

typedef struct {
    u8 protocol;//selKeyNegoProt_e
    u8 preSharedSec;//preSharedSec_e
    extAddr_t sendingDevEUI64;
} tlvSelectKeyNegoMethod_t;

/**
 *  @brief Structure for Device EUI64 List TLV (ID 0)
 */
typedef struct {
    u8 count;
    extAddr_t devEUI64[];
} tlvDevEUI64List_t;

/**
 *  @brief Structure for APS Frame Counter Challenge TLV (ID 0)
 */
typedef struct {
    extAddr_t senderEUI64;
    u8 challengeValue[8];
} tlvAPSFrameCntChallenge_t;

/**
 *  @brief Structure for APS Frame Counter Response TLV (ID 0)
 */
typedef struct {
    extAddr_t senderEUI64;
    u8 challengeValue[8];
    u32 apsFrameCnt;
    u32 secFrameCnt;
    u8 mic[8];
} tlvAPSFrameCntRsp_t;

/**
 *  @brief Structure for Manufacturer Specific Global TLV (Id 64)
 */
typedef struct {
    u16 manuId;
    u8 addData[];
} tlvManuSpec_t;

/**
 *  @brief Structure for Supported Key Negotiation Methods Global TLV (ID 65)
 */
typedef union {
    u8 byte;
    struct {
        u8 staticKeyReq:1;
        u8 c25519HashAESMMO128:1;
        u8 c25519HashSHA256:1;
        u8 reserved:5;
    } bits;
} keyNegoProt_t;

typedef union {
    u8 byte;
    struct {
        u8 symmetricAuthToken:1;
        u8 installCodeKey:1;
        u8 passcodeKey:1;
        u8 basicAccessKey:1;
        u8 adminAccessKey:1;
        u8 reserved:3;
    } bits;
} preSharedSec_t;

typedef struct _attribute_packed_ {
    keyNegoProt_t protocolBitmask;
    preSharedSec_t secBitmask;
    extAddr_t srcDevEUI64;
} tlvSupportKeyNegoMethod_t;

/**
 *  @brief Structure for PAN ID Conflict Report Global TLV (ID 66)
 */
typedef struct {
    u16 count;
} tlvPANIDConflictReport_t;

/**
 *  @brief Structure for Next PAN ID Change Global TLV (ID 67)
 */
typedef struct {
    u16 panId;
} tlvNextPANIDChange_t;

/**
 *  @brief Structure for Next Channel Change Global TLV (ID 68)
 */
typedef struct {
    u32 chnMask;
} tlvNextChnChange_t;

/**
 *  @brief Structure for Symmetric Passphrase Global TLV (ID 69)
 */
typedef struct {
    u8 passphrase[16];
} tlvSymmetericPassphrase_t;

/**
 *  @brief Structure for Router Information Global TLV (ID 70)
 */
typedef union {
    u16 info;
    struct {
        u16 hubConnectivity:1;
        u16 uptime:1;
        u16 preferredParent:1;
        u16 batteryBackup:1;
        u16 enhBeaconReqSupport:1;
        u16 macDataPollKeepalive:1;
        u16 endDeviceKeepalive:1;
        u16 powerNegotiation:1;
        u16 reserved:8;
    } bits;
} tlvRouterInfo_t;

/**
 *  @brief Structure for Fragmentation Parameters Global TLV (ID 71)
 */
typedef union {
    u8 opts;
    struct {
        u8 support:1;
        u8 reserved:7;
    } bits;
} fragmentOpt_t;

typedef struct _attribute_packed_ {
    u16 nodeId;
    u16 maxInTransUnit;
    fragmentOpt_t fragmentOpt;
} tlvFragmentParam_t;

/**
 *  @brief Structure for Joiner Encapsulation Global TLV (ID 72)
 */
typedef struct {
    u8 tlvs[1];
} tlvJoinerEncap_t;

/**
 *  @brief Structure for Beacon Appendix Encapsulation Global TLV (ID 73)
 */
typedef struct {
    u8 tlvs[1];
} tlvBeaconAppendixEncap_t;

/**
 *  @brief Structure for Configuration Parameters Global TLV (ID 75)
 */
typedef union {
    u16 cfg;
    struct {
        u16 apsZdoRestrictedMode:1;
        u16 linkKeyEncryptForApsTransKey:1;
        u16 nwkLeaveReqAllowed:1;
        u16 reserved:13;
    } bits;
} tlvConfigParam_t;

enum {
    REJOIN_INSECURITY,
    REJOIN_SECURITY
};

/**        
 *  @brief Structure for parameter of apsdeDataInd callback function
 */
typedef struct apsdeDataInd_s {
    aps_data_ind_t indInfo;
    u16 asduLen;
    u8  asdu[];
} apsdeDataInd_t;


static inline void zb_resetDevice(void)
{
    SYSTEM_RESET();
}

/**
 * @brief  Factory reset.
 *         This interface should be called by Router or End-Device.
 *         If it is a not factory new device, it will broadcast
 *         a Leave Command before factory new reset.
 *
 * @param  none
 *
 * @return none
 */
void zb_factoryReset(void);

/**
 * @brief  Reset device to factory new.
 *         This interface could be called by Coordinator, Router or End-Device.
 *         coordinator:
 *             It will erase all NV information and reset all layer settings.
 *         router or end device:
 *             If it is a not factory new device, it will broadcast
 *             a Leave Command before factory new reset.
 *
 * @param  none
 *
 * @return none
 */
void zb_resetDevice2FN(void);

/**
 * @brief  This interface is used to obtain whether the device is factory new.
 *         Check if it is a new device (without network parameters).
 *
 * @param  none
 *
 * @return TRUE or FALSE
 */
bool zb_isDeviceFactoryNew(void);

/**
 * @brief  Factory New device Set, internal used.
 *
 * @param  new - 1: set as factory new device, 0: set as non-factory new device
 *
 * @return none
 */
void zb_deviceFactoryNewSet(bool new);

/**
 * @brief  This interface is used to obtain whether the device has joined a network
 *         and is connected to the PAN.
 *
 * @param  none
 *
 * @return TRUE or FALSE
 */
bool zb_isDeviceJoinedNwk(void);

/**
 * @brief  Get MAC association permit status (only for ZC ZR)
 *
 * @param  none
 *
 * @return TRUE or FALSE
 */
bool zb_getMacAssocPermit(void);

/**
 * @brief  Enable or Disable AODV
 *
 * @param  enable - Enable or Disable
 *
 * @return none
 */
void zb_nldeDataDiscoverRouteSet(bool enable);

/**
 * @brief  Get AODV enabled or disabled status.
 *
 * @param  none
 *
 * @return TRUE or FALSE
 */
bool zb_nldeDataDiscoverRouteGet(void);

/**
 * @brief  Set extend PAN ID to APS_IB.
 *
 * @param  panId - extend PAN ID
 *
 * @return none
 */
void zb_apsExtPanidSet(extPANId_t panId);

/**
 * @brief  Set channel mask to APS_IB.
 *
 * @param  mask - channel mask
 *
 * @return status
 */
aps_status_t zb_apsChannelMaskSet(u32 mask);

/**
 * @brief  Get channel mask from APS_IB.
 *
 * @param  none
 *
 * @return channel mask
 */
u32 zb_apsChannelMaskGet(void);

/**
 * @brief  Get device type
 *
 * @param  none
 *
 * @return device type (nwk_deviceType_t)
 *              0 - Coordinator
 *              1 - Router
 *              2 - EndDevice
 */
device_type_t zb_getDeviceType(void);

/**
 * @brief  Get local IEEE address
 *
 * @param  extAddr - 64-bits long address
 *
 * @return none
 */
void zb_getLocalExtAddr(addrExt_t extAddr);

/**
 * @brief  Get local short address
 *
 * @param  none
 *
 * @return 2 bytes of short address
 */
u16 zb_getLocalShortAddr(void);

/**
 * @brief  Get parent short address
 *
 * @param  none
 *
 * @return 2 bytes of parent's short address
 */
u16 zb_getParentShortAddr(void);

/**
 * @brief  Get network short address according to the EXT address
 *
 * @param  extAddr
 * @param  nwkAddr
 *
 * @return 0 - SUCCESS; 0xFF - FAILED
 */
u8 zb_getNwkAddrByExtAddr(addrExt_t extAddr, u16 *nwkAddr);

/**
 * @brief  Get EXT address according to the network short address
 *
 * @param  nwkAddr
 * @param  extAddr
 *
 * @return 0 - SUCCESS; 0xFF - FAILED
 */
u8 zb_getExtAddrByNwkAddr(u16 nwkAddr, addrExt_t extAddr);

/**
 * @brief  Set the period time of the network layer link status command.
 *         Only for Coordinator and Router.
 *
 * @param  periodInSec - Time between two link status commands (in seconds)
 *
 * @return none
 */
void zb_nlmeSetLinkStsPeriod(u8 periodInSec);

/**
 * @brief  Send data request manually, only for End-Device.
 *
 * @param  none
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_endDeviceSyncReq(void);

/**
 * @brief  Set poll rate, only for End-Device.
 *
 * @param  newRate - in millisecond
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_setPollRate(u32 newRate);

/**
 * @brief  Get poll rate, only for End-Device.
 *
 * @param  none
 *
 * @return in millisecond
 */
u32 zb_getPollRate(void);

/**
 * @brief  Perform network formation request.
 *         Only for Coordinator and Router.
 *
 * @param  scanChannels
 * @param  scanDuration
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_nwkFormation(u32 scanChannels, u8 scanDuration);

/**
 * @brief  Perform router start to recover network.
 *         Only for Coordinator and Router.
 *
 * @param  none
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_routerStart(void);

/**
 * @brief  Perform network discovery request.
 *         Only for Router and End Device.
 *
 * @param  scanChannels
 * @param  scanDuration
 * @param  cb
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_nwkDiscovery(u32 scanChannels, u8 scanDuration, nwkDiscoveryUserCb_t cb);

/**
 * @brief  Cancel network discovery request.
 *         Only for Router and End Device.
 *
 * @param  none
 *
 * @return none
 */
void zb_nwkDiscoveryStop(void);

/**
 * @brief  Perform associate join request.
 *         Only for Router and End Device.
 *
 * @param  none
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_assocJoinReq(void);

/**
 * @brief  Perform rejoin request.
 *         Only for Router and End Device.
 *
 * @param  scanChannels
 * @param  scanDuration
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_rejoinReq(u32 scanChannels, u8 scanDuration);

/**
 * @brief Perform rejoin request with backoff timer.
 *        Only for Router and End Device.
 *        NOTE: Please refer to the related definition of
 *              zdo_cfg_attributes for the parameter configuration
 *              of backoff, in zdo_api.h
 *
 * @param  scanChannels
 * @param  scanDuration
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_rejoinReqWithBackOff(u32 scanChannels, u8 scanDuration);

/**
 * @brief  Rejoin with security or insecurity mode.
 *
 * @param  mode - REJOIN_INSECURITY or REJOIN_SECURITY
 *
 * @return none
 */
void zb_rejoinSecModeSet(u8 mode);

/**
 * @brief  Perform direct join request.
 *         Only for Router and End Device.
 *
 * @param  scanChannels
 * @param  scanDuration
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_directJoinReq(u32 scanChannels, u8 scanDuration);

/**
 * @brief  Accept a remote device as a child node,
 *         who will join the network through direct join mode.
 *         Only for Coordinator and Router.
 *
 * @param  pReq - the remote device information
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_nwkDirectJoinAccept(nlme_directJoin_req_t *pReq);

/**
 * @brief  Set local permit join duration.
 *         Only for Coordinator and Router.
 *
 * @param  permitDuration - The time allowed for new device to join (in seconds).
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_nlmePermitJoiningRequest(u8 permitDuration);

/**
 * @brief  Perform route discovery request.
 *         Only for Coordinator and Router.
 *
 * @param  pRouteDiscReq
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_routeDiscReq(nlme_routeDisc_req_t *pRouteDiscReq);

/**
 * @brief  Perform leave request.
 *
 * @param  pLeaveReq
 *
 * @return zdo_status_t
 */
u8 zb_nlmeLeaveReq(nlme_leave_req_t *pLeaveReq);

/**
 * @brief  Perform end device timeout request.
 *
 * @param  pTLVs
 *
 * @return RET_ILLEGAL_REQUEST or zdo_status_t
 */
u8 zb_nlmeEndDevNegotiateReq(nlme_endDevNegotiate_req_t *pTLVs);

/**
 * @brief  Send address request to target device for short address.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of network address request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - network address response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoNwkAddrReq(u16 dstNwkAddr, zdo_nwk_addr_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send address request to target device for IEEE address.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of IEEE address request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - IEEE address response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoIeeeAddrReq(u16 dstNwkAddr, zdo_ieee_addr_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send simple descriptor request.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of simple descriptor request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - simple descriptor response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoSimpleDescReq(u16 dstNwkAddr, zdo_simple_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send node descriptor request.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of node descriptor request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - node descriptor response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoNodeDescReq(u16 dstNwkAddr, zdo_node_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send power descriptor request.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of power descriptor request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - power descriptor response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoPowerDescReq(u16 dstNwkAddr, zdo_power_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send active endpoint request.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of active endpoint request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - active endpoint response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoActiveEpReq(u16 dstNwkAddr, zdo_active_ep_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send match descriptor request.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of match descriptor request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - match descriptor response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoMatchDescReq(u16 dstNwkAddr, zdo_match_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send complex descriptor request.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of complex descriptor request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - complex descriptor response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoComplexDescReq(u16 dstNwkAddr, zdo_complex_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send user descriptor request.
 *
 * @param  dstNwkAddr - this command will be send to
 * @param  pReq       - parameter of user descriptor request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - user descriptor response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoUserDescReq(u16 dstNwkAddr, zdo_user_descriptor_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send zdo system server discovery request, broadcasting to 0xfffd
 *
 * @param  serverMask  - server mask bit assignment
 * @param  seqNo       - the sequence number used by this command
 * @param  indCb       - system server discovery response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoSystemServerDiscoveryReq(u16 serverMask, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send bind request, destination address is based on the srcAddr in the request command
 *
 * @param  isBinding - 1:bind, 0:unbind
 * @param  pReq      - parameter of bind/unbind request
 * @param  seqNo     - the sequence number used by this command
 * @param  indCb     - bind/unbind response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoBindUnbindReq(bool isBinding, zdo_bind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send bind request, destination address is based on the srcAddr in the request command
 *
 * @param  isBinding - 1:bind, 0:unbind
 * @param  addr      - target address
 * @param  pReq      - parameter of bind/unbind request
 * @param  seqNo     - the sequence number used by this command
 * @param  indCb     - bind/unbind response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoBindUnbindWithAddrReq(bool isBinding, u16 addr, zdo_bind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send end device bind request.
 *
 * @param  pReq  - parameter of end device bind request
 * @param  seqNo - the sequence number used by this command
 * @param  indCb - end device bind response call back function
 *
 * @return status
 */
zdo_status_t zb_zdoEndDeviceBindReq(zdo_edBindReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send the command of mgmt_permitJoin_req.
 *
 * @param  dstNwkAddr         - Short address of the target device
 * @param  permitJoinDuration - Time in seconds during which the device allows to join
 * @param  tcSignificance     - tc significance
 * @param  seqNo              - the sequence number used by this command
 * @param  indCb              - mgmtPermitJoin.confirm callback
 *
 * @return status
 */
zdo_status_t zb_mgmtPermitJoinReq(u16 dstNwkAddr, u8 permitJoinDuration, u8 tcSignificance, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send the command of zb_zdoClearAllBindReq.
 *
 * @param  dstNwkAddr - Short address of the target device
 * @param  pReq       - parameter of zdo_clearAllBind_req_t
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - mgmtPermitJoin.confirm callback
 *
 * @return status
 */
zdo_status_t zb_zdoClearAllBindReq(u16 dstNwkAddr, zdo_clearAllBind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send the command of mgmt_leave_req.
 *
 * @param  dstNwkAddr - target address to receive this cmd
 * @param  pReq       - parameter of mgmt_leave_req
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - the indicate callback for mgmt_leave_rsp
 *
 * @return status
 */
zdo_status_t zb_mgmtLeaveReq(u16 dstNwkAddr, zdo_mgmt_leave_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send the command of mgmt_network_update_req.
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of mgmt_network_update_req
 * @param  seqNo      - the sequence number used by this command
 *
 * @return status
 */
zdo_status_t zb_mgmtNwkUpdateReq(u16 dstNwkAddr, zdo_mgmt_nwk_update_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send the command of mgmt_lqi_req.
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of mgmt_lqi_req
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - the indicate callback for mgmt_lqi_rsp
 *
 * @return status
 */
zdo_status_t zb_mgmtLqiReq(u16 dstNwkAddr, zdo_mgmt_lqi_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send the command of mgmt_rtg_req.
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of mgmt_rtg_req
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - the indicate callback for mgmt_rtg_rsp
 *
 * @return status
 */
zdo_status_t zb_mgmtRtgReq(u16 dstNwkAddr, zdo_mgmt_rtg_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send the command mgmt_bind_req.
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of mgmt_bind_req
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - the indicate callback for mgmt_bind_rsp
 *
 * @return status
 */
zdo_status_t zb_mgmtBindReq(u16 dstNwkAddr, zdo_mgmt_bind_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Beacon Survey command
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of mgmt_nwkBeaconSurvey_req
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - callback for the response
 *
 * @return status
 */
zdo_status_t zb_mgmtNwkBeaconSurveyReq(u16 dstNwkAddr, zdo_mgmt_nwkBeaconSurvey_req_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Start Key Negotiation Request command
 *
 * @param  pAddrInfo - destination address of the request
 * @param  pReq      - parameter of request
 * @param  seqNo     - the sequence number used by this command
 * @param  indCb     - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secStartKeyNegotiationReq(zdo_sec_addrInfo_t *pAddrInfo, zdo_sec_startKeyNegoReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Retrieve Auth Token Request command
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secRetrieveAuthTokenReq(u16 dstNwkAddr, zdo_sec_retrieveAuthTokenReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Get Auth Level Request command
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secGetAuthLevelReq(u16 dstNwkAddr, zdo_sec_getAuthLevelReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Set Configuration Request command
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secSetConfigurationReq(u16 dstNwkAddr, zdo_sec_setConfigReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Get Configuration Request command
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secGetConfigurationReq(u16 dstNwkAddr, zdo_sec_getConfigReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Start Key Update Request command
 *
 * @param  pAddrInfo - destination address of the request
 * @param  pReq      - parameter of request
 * @param  seqNo     - the sequence number used by this command
 * @param  indCb     - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secStartKeyUpdateReq(zdo_sec_addrInfo_t *pAddrInfo, zdo_sec_startKeyUpdateReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Challenge Request command
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secChallengeReq(u16 dstNwkAddr, zdo_sec_challengeReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Send Decommission Request command
 *
 * @param  dstNwkAddr - destination address of the request
 * @param  pReq       - parameter of request
 * @param  seqNo      - the sequence number used by this command
 * @param  indCb      - callback for the response
 *
 * @return status
 */
zdo_status_t zb_secDecommissionReq(u16 dstNwkAddr, zdo_sec_decommissionReq_t *pReq, u8 *seqNo, zdo_callback indCb);

/**
 * @brief  Remove device request command.
 *
 * @param  pRemoveDevReq
 *
 * @return RET_OK or others
 */
u8 zb_apsmeRemoveDevReq(ss_apsmeRemoveDeviceReq_t *pRemoveDevReq);

/**
 * @brief  APSME request key command.
 *
 * @param  pRequestKeyReq
 * @param  cb
 *
 * @return RET_OK or others
 */
u8 zb_apsmeRequestKeyReq(ss_apsmeRequestKeyReq_t *pRequestKeyReq, apsConfirmKeyCb_t cb);

/**
 * @brief  Dynamic link key with TC
 *
 * @param  pBuf
 * @param  bufSize
 * @param  cb
 *
 * @return RET_OK or others
 */
u8 zb_dynamicLinkKeyWithTC(u8 *pBuf, u16 bufSize, apsConfirmKeyCb_t cb);

/**
 * @brief  Update passphrase with TC
 *
 * @param  cb - callback once the retrieveAuthTokenRsp is received
 *
 * @return RET_OK or others
 */
u8 zb_updatePassphraseReq(ss_retrieveAuthTokenRspCb_t cb);

/**
 * @brief  APSME transport key command.
 *
 * @param  pTransportKeyReq
 *
 * @return RET_OK or others
 */
u8 zb_apsmeTransportKeyReq(ss_apsmeTransportKeyReq_t *pTransportKeyReq);

/**
 * @brief  APSME switch key command.
 *
 * @param  pSwitchKeyReq
 *
 * @return RET_OK or others
 */
u8 zb_apsmeSwitchKeyReq(ss_apsmeSwitchKeyReq_t *pSwitchKeyReq);

/**
 * @brief  API for Trust center to update NWK key.
 *
 * @param  pTcUpdateNwkKey
 *             dstAddr all zero means broadcast.
 *
 * @return RET_OK or others
 */
u8 zb_tcUpdateNwkKey(ss_tcUpdateNwkKey_t *pTcUpdateNwkKey);

/**
 * @brief  TC schedule a security start key update
 *
 * @param  dstAddr
 *
 * @return RET_OK or others
 */
u8 zb_tcSecurityStartKeyUpdate(u16 dstAddr);

/**
 * @brief  Swap out set
 *
 * @param  extPanId
 *
 * @return RET_OK or others
 */
u8 zb_tcSwapOutNwkSet(extPANId_t extPanId);

/**
 * @brief  Import swap out key
 *
 * @param  keyPairBackUp
 *
 * @return RET_OK or others
 */
u8 zb_tcSwapOutKeyPairImport(ss_devKeyPairBackUp_t *keyPairBackUp);

/**
 * @brief  Backup swap out key
 *
 * @param  extAddr
 * @param  keyPairBackUp
 *
 * @return RET_OK or others
 */
u8 zb_tcSwapOutKeyPairBackUp(addrExt_t extAddr, ss_devKeyPairBackUp_t *keyPairBackUp);

/**
 * @brief  Search binding table.
 *
 * @param  clusterID
 * @param  srcEp
 *
 * @return TRUE or FALSE
 */
bool zb_bindingTblSearched(u16 clusterID, u8 srcEp);

/**
 * @brief  Send device announce.
 *
 * @param  none
 *
 * @return RET_OK or others
 */
u8 zb_zdoSendDevAnnance(void);

/**
 * @brief  Send parent announce.
 *
 * @param  none
 *
 * @return RET_OK or others
 */
u8 zb_zdoSendParentAnnce(void);

/**
 * @brief  Register MAC layer callback function.
 *
 * @param  cb - Callback functions
 *
 * @return none
 */
void zb_macCbRegister(mac_appIndCb_t *cb);

/**
 * @brief  Register ZDO layer callback function.
 *
 * @param  cb - Callback functions
 *
 * @return none
 */
void zb_zdoCbRegister(zdo_appIndCb_t *cb);

/**
 * @brief  Force join a fixed network.
 *
 * @param  channel   - operation channel
 * @param  panId     - the network panID
 * @param  shortAddr - network address allocated by itself
 * @param  extPanId  - external panID of the network
 * @param  nwkKey    - the network key of the network
 * @param  tcAddr    - the trust center's address of the network, only for central network,
 *                     normally it's same with extPanId NULL for distribute network
 *
 * @return none
 */
void zb_joinAFixedNetwork(u8 channel, u16 panId, u16 shortAddr, u8 *extPanId, u8 *nwkKey, u8 *tcAddr);

/**
 * @brief  Extend PAN ID rejoin. Only for the factory new device.
 *
 * @param  extPanId - extend PAN ID
 *
 * @return none
 */
void zb_extPanIdRejoin(extPANId_t extPanId);

/**
 * @brief  Pre-configure network key. Only for the factory new device.
 *         Called after the bdb_init() function.
 *
 * @param  nwkKey     - the pre-configured network key
 * @param  enTransKey - 0: join network once receiving associate response
 *                      1: join network follow the standard flow (need process transport key)
 *
 * @return none
 */
void zb_preConfigNwkKey(u8 *nwkKey, bool enTransKey);

/**
 * @brief  Shift to and save the logic channel.
 *
 * @param  chn - 11 ~ 26
 *
 * @return none
 */
void zdo_nlmeChannelShift(u8 chn);

/**
 * @brief  Search TLV fields
 *
 * @param  pBuf
 * @param  bufSize
 * @param  typeId
 * @param  minLen
 *
 * @return tlv_t
 */
tlv_t *zb_tlvFind(u8 *pBuf, u16 bufSize, tlvTypeId_e typeId, u8 minLen);

#endif /* ZB_API_H */
