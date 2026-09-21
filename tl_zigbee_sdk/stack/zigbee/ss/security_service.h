/********************************************************************************************************
 * @file    security_service.h
 *
 * @brief   This is the header file for security_service
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
#ifndef SECURITY_SERVICE_H
#define SECURITY_SERVICE_H

//NLS5 - All devices shall maintain at least 2 NWK keys with the frame
//counters consistent with the security mode of the network (Standard or High).
#define	SECUR_N_SECUR_MATERIAL          2

//SECUR: CCM M parameter. Fixed to 4 for security level 5
#define	ZB_CCM_M                        4

#if ZB_SECURITY
#define SECURITY_MODE_STANDARD          1
#define SECURITY_MODE_HIGH              (!SECURITY_MODE_STANDARD)
#define SS_UPDATE_FRAMECOUNT_THRES      1024
#endif

typedef enum {
    SS_KEYREQ_TYPE_APPLK                = 0x02,
    SS_KEYREQ_TYPE_TCLK                 = 0x04,
} ss_keyReqType_e;

typedef enum {
    LINKKEY_PUBLINK_KEY_DIS             = (0),
    LINKKEY_MASTER_KEY_EN               = (1 << 0),
    LINKKEY_CERTIFICATION_KEY_EN        = (1 << 1),
} ss_pubLinkKeyOpt_e;

enum {
    SS_SECUR_NO_ENCR,
    SS_SECUR_NWK_ENCR,
    SS_SECUR_APS_ENCR,
    SS_SECUR_MAC_ENCR
};

//Key id - see 4.5.1.1.2
enum {
    SS_SECUR_DATA_KEY,
    SS_SECUR_NWK_KEY,
    SS_SECUR_KEY_TRANSPORT_KEY,
    SS_SECUR_KEY_LOAD_KEY
};

//Key types
typedef enum {
    SS_TC_MASTER_KEY                    = 0x00, //removed from zb 3.0
    SS_STANDARD_NETWORK_KEY             = 0x01,
    SS_APP_MASTER_KEY                   = 0x02, //removed from zb 3.0
    SS_APP_LINK_KEY                     = 0x03,
    SS_TC_LINK_KEY                      = 0x04,
    SS_HIGH_SECUR_NETWORK_KEY,                  //removed from zb 3.0
    SS_EPH_GLOBAL_AUTH_KEY              = 0xB0, //Ephemeral Global Authorization Key, for zigbee direct
    SS_EPH_UNIQUE_AUTH_KEY              = 0xB1, //Ephemeral Unique Authorization Key, for zigbee direct
    SS_BASIC_AUTH_KEY                   = 0xB2,	//Basic Authorization Key, for zigbee direct
    SS_ADMIN_AUTH_KEY                   = 0xB3,	//Administrative Authorization Key, for zigbee direct
} ss_seKeyType_e;

typedef enum {
    SS_UNIQUE_LINK_KEY                  = 0x00,
    SS_GLOBAL_LINK_KEY                  = 0x01
} ss_linkKeytype_e;

typedef enum {
    SS_SEMODE_CENTRALIZED,
    SS_SEMODE_DISTRIBUTED
} ss_securityMode_e;

typedef enum {
    SS_PROVISIONAL_KEY,
    SS_UNVERIFIED_KEY,
    SS_VERIFIED_KEY
} ss_keyAttributes_e;

typedef enum {
    SS_NO_AUTH,
    SS_INSTALL_CODE_KEY,
    SS_ANONYMOUS_KEY_NEGO,
    SS_KEY_NEGO_WITH_AUTH
} ss_initJoinAuth_e;

typedef enum {
    SS_STATIC_KEY,
    SS_C25519_HASH_AES_MMO_128,
    SS_C25519_HASH_SHA_256,
} ss_keyNegoMethod_e;

typedef enum {
    SS_NO_KEY_NEGO,
    SS_START_KEY_NEGO,
    SS_COMPLETE_KEY_NEGO
} ss_keyNegoState_e;

typedef enum {
    SS_NO_UPDATE,
    SS_KEY_REQ_METHOD,
    SS_UNAUTH_KEY_NEGO,
    SS_AUTH_KEY_NEGO,
    SS_APP_CERTIFICATE_BASED_MUTUAL_AUTH
} ss_joinKeyUpdate_e;

typedef struct {
    u8 key[CCM_KEY_SIZE];
    u8 keySeqNum;
    u8 keyType;
} ss_material_set_t;

typedef enum {
    SS_PRECONFIGURED_NOKEY              = 0,
    SS_PRECONFIGURED_GLOBALLINKKEY      = BIT(0),
    SS_PRECONFIGURED_UNIQUELLINKKEY     = BIT(1),
    SS_PRECONFIGURED_NWKKEY             = BIT(2),
} ss_preconfiguredKey_e;

typedef enum {
    SS_GLB_LK_PRECONF                   = 0,
    SS_GLB_LK_TC_DEFAULT                = 1,
    SS_GLB_LK_MASTER                    = 2,
    SS_GLB_LK_CERTIFICATION             = 3,
} ss_glbLKId_e;

typedef struct {
    //This value set to TRUE will only allow devices know to TC join or rejoin
    bool useWhiteList;
    // 0 ~ not support; 1 ~ support but not require; 2 ~ require the use of install code by JD, the useWhiteList would set to TRUE
    //interface need to be supported to input install code to TC
    u8 allowInstallCode;
    //Indicates whether or not devices are required to attempt to update their TCLK after joining. In centralized security network,
    //this value must be set to TRUE, the joining device must attempt TCLK update after joining the network.
    bool updateTCLKrequired;
#if ZB_COORDINATOR_ROLE
    //This values indicates whether or not TC allow new device join to the network, set to false in centralized security NWK will
    //reject any join request
    bool allowJoins;
    //If TC allows rejoin using well known or default keys, a setting of FALSE means rejoins are only allowed with TCLK where the
    //KeyAttributes of the apsDeviceKeyPairSet entry indicates VERIFIED_KEY. This value set to FALSE in centralized security NWK.
    bool allowRejoins;
    //0 ~ never; 1 ~ any device may request; 2 ~ only devices in the apsDeviceKeyPaireSet.
    //Set to 0 in network with higher level protocols for establishing link keys. Set to 1/2 in centralized security networks
    u8 allowTCLKrequest;

    u8 allowAppLKrequest;
#endif
} ss_tcPolicy_t;

typedef struct {
    bool allowJoins;
    bool allowRejoins;
    bool allowVirtualDevices;
    u8 allowTCLKRequest;
    u8 allowAppLKRequest;
    u8 reqICorPrePassphrase;
} ss_tc_policy_t;

typedef enum {
    KEYTYPE_PRO_MASK                    = BIT(0),
    KEYTYPE_UNVERIFY_MASK               = BIT(1),
    KEYTYPE_VERIFIED_MASK               = BIT(2)
} nvKeytpeMask_e;

typedef enum {
    SS_DEVKEYPAIR_SYNID_KEYATTR,
    SS_DEVKEYPAIR_SYNID_INCOMINGFRAMECNT,
    SS_DEVKEYPAIR_SYNID_ALL
} ss_devKeyPairSyn_id;

typedef union {
    u8 byte;
    struct {
        u8 frameCntSync:1;
        u8 reserved:7;
    } bits;
} ss_featCapa_t;

typedef enum {
    STATIC_KEY_REQUEST                  = BIT(0),
    CURVE_25519_WITH_HASH_AES_MMO_128   = BIT(1),
    CURVE_25519_WITH_HASH_SHA_256       = BIT(2)
} ss_suppKeyNegoBitmask_e;

typedef union {
    u8 byte;
    struct {
        u8 staticKeyReq:1;
        u8 c25519HashAESMMO128:1;
        u8 c25519HashSHA256:1;
        u8 reserved:5;
    } bits;
} ss_suppKeyNegoMethods_t;

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
} ss_suppPreSharedSec_t;

typedef struct _attribute_packed_ {
    addrExt_t device_address;

    u8 linkKey[CCM_KEY_SIZE];   /* The actual value of the link key. */

    u8 keyAttr;                 //ss_keyAttributes_e
    u8 apsLinkKeyType;          //ss_linkKeytype_e
    u8 used;
    bool newFeatFlag;

    u32 outgoingFrameCounter;
    u32 incomingFrameCounter;
} ss_dev_pair_set_t; //36-bytes

typedef struct _attribute_packed_ {
    addrExt_t deviceAddr;

    u8 initJoinAuth;            //ss_initJoinAuth_e
    u8 keyNegoMethod;           //ss_keyNegoMethod_e
    u8 keyNegoState;            //ss_keyNegoState_e
    u8 postJoinKeyUpdateMethod; //ss_joinKeyUpdate_e

    u16 maxIncomingTxSize;
    ss_featCapa_t featCapabilities;
    u8 passphraseLen;

    u8 passphrase[16];
    u8 tcSwapOutLinkKey[16];

    u8 rsv;
    u8 used:1;
    u8 passphraseUpdateAllowed:1;
    u8 verifiedFrameCounter:1;
    u8 isVirtualDev:1;
    u8 fragmentSupported:1;
    u8 reserved:3;
} ss_dev_pair_set_add_t;

typedef struct _attribute_packed_ {
    u32 ssTimeoutPeriod;
    u32 outgoingFrameCounter;
    u32 prevOutgoingFrameCounter;
    u8 *keyPairSetNew;
    ss_material_set_t nwkSecurMaterialSet[SECUR_N_SECUR_MATERIAL];//36
    u16 devKeyPairNum;
    addrExt_t trust_center_address;
    u8 securityLevel:3;
    u8 secureAllFresh:1;
    u8 activeSecureMaterialIndex:2;
    u8 reserved:2;//flag
    u8 activeKeySeqNum;
    u8 preConfiguredKeyType;//ss_preconfiguredKey_e, should be set during init state which used for ZDO auth
    ss_tcPolicy_t tcPolicy; //3 or 7
    u8 *touchLinkKey;
    u8 *distributeLinkKey;
    u8 tcLinkKeyType;
    u8 *tcLinkKey;
} ss_info_base_t; //81 or 85 bytes

typedef struct {
    addrExt_t apsChallengeTargetEui64;
    u8 apsChallengeValue[8];
    u32 apsChallengeFrameCounter;
    u8 apsChallengePeriodTimeout;
    u8 apsChallengePeriodRemainTime;
    u8 apsDevInterviewTimeoutPeriod;
    ss_suppKeyNegoMethods_t apsSupportedKeyNegoMethods;
    bool apsDynamicLinkKeyComplete;
    u8 reserved[3];
} ss_info_base_add_t;

typedef struct {
    addrExt_t deviceAddr;
    u8 tcSwapOutLinkKey[16];
    u8 passphrase[16];
    u8 keyAttr;         //ss_keyAttributes_e
    u8 initJoinAuth;    //ss_initJoinAuth_e
    u8 keyNegoMethod;   //ss_keyNegoMethod_e
} ss_devKeyPairBackUp_t;

//Parameters for APSME-TRANSPORT-KEY.request primitive
typedef struct {
    /* The dstAddr is all zeros, means broadcast, for SS_STANDARD_NETWORK_KEY.
     */
    addrExt_t dstAddr;
    u8 keyType;
    u8 key[CCM_KEY_SIZE];
    u8 relayByParent;
    u8 keySeqNum;
    addrExt_t partnerAddr; //for application key
    u8 initiatorFlag;
    u8 nwkSecurity;
} ss_apsmeTransportKeyReq_t;

//Parameters for APSME-TRANSPORT-KEY.indication primitive
typedef struct {
    addrExt_t srcAddr;
    u8 keyType;
    u8 key[CCM_KEY_SIZE];
    union {
        u8 keySeqNum;
        addrExt_t partnerAddr;
    };
} ss_apsmeTransportKeyInd_t;

//Parameters for APSME-UPDATE-DEVICE.request primitive
typedef struct {
    //The extended 64-bit address of the device
    //that shall be sent the update information.
    addrExt_t dstAddr;
    /*The extended 64-bit address of the device
    whose status is being updated.*/
    addrExt_t devAddr;
    u8 *pJoinDevTLVs;//TODO
    u16 devShortAddr;
    u8 status;
    u8 tlvLen;
} ss_apsmeUpdateDeviceReq_t;

typedef enum {
    SS_STANDARD_DEV_SECURED_REJOIN      = 0,
    SS_STANDARD_DEV_UNSECURED_JOIN      = 1,
    SS_DEV_LEFT                         = 2,
    SS_STANDARD_DEV_TC_REJOIN           = 3
} ss_apsmeUpdateDevStatus_e;

//Parameters for APSME-UPDATE-DEVICE.indication primitive
typedef struct {
    /*The extended 64-bit address of the
    device originating the update-device
    command.*/
    addrExt_t srcAddr;
    /*The extended 64-bit address of the device
    whose status is being updated.*/
    addrExt_t devAddr;
    u8 *pJoinDevTLVs;//TODO
    u16 devShortAddr;
    u8 status;//ss_apsmeUpdateDevStatus_e
    u8 tlvLen;
} ss_apsmeUpdateDeviceInd_t;

//Parameters for APSME-REMOVE-DEVICE.request primitive
typedef struct {
    /*The extended 64-bit address of the device
    that is the parent of the child device that is
    requested to be removed, or the router
    device that is requested to be removed.*/
    addrExt_t parentAddr;
    /*The extended 64-bit address of the target
    device that is requested to be removed. If a
    router device is requested to be removed,
    then the ParentAddress shall be the same
    as the TargetAddress.*/
    addrExt_t targetExtAddr;
} ss_apsmeRemoveDeviceReq_t;

//Parameters for APSME-REMOVE-DEVICE.indication primitive
typedef struct {
    /*The extended 64-bit address of the device
     requesting that a child device be removed.*/
    addrExt_t tcAddr;
    /*The extended 64-bit address of the child
    device that is requested to be removed*/
    addrExt_t childExtAddr;
} ss_apsmeRemoveDeviceInd_t;

//Parameters for APSME-REQUEST-KEY.request primitive
typedef struct {
    /* The address of the device which the request-key command should be sent. */
    tl_zb_addr_t dstAddr;
    /* If the key type is SS_KEYREQ_TYPE_APPLK, this parameter shall indicate an
     * extend 64-bit address of a device that shall receive the same key as the
     * device requesting the key.
     */
    addrExt_t partnerAddr;
    /* ZB_ADDR_16BIT_DEV_OR_BROADCAST or ZB_ADDR_64BIT_DEV. */
    u8 dstAddrMode;//zb_addr_mode_t
    /* the type of key being requested. */
    u8 keyType;//ss_keyReqType_e
} ss_apsmeRequestKeyReq_t;

//Parameters for APSME-REQUEST-KEY.indication primitive
typedef struct {
    /* The address of the device that sent the request-key command. */
    tl_zb_addr_t srcAddr;
    /* If the key type is SS_KEYREQ_TYPE_APPLK, this parameter shall indicate an
     * extend 64-bit address of a device that shall receive the same key as the
     * device requesting the key.
     */
    addrExt_t partnerAddr;
    /* ZB_ADDR_16BIT_DEV_OR_BROADCAST or ZB_ADDR_64BIT_DEV. */
    u8 dstAddrMode;//zb_addr_mode_t
    /* the type of key being requested. */
    u8 keyType;//ss_keyReqType_e
} ss_apsmeRequestKeyInd_t;

//Parameters for APSME-SWITCH-KEY.request primitive
typedef struct {
    /* The extended 64-bit address of the device to which the switch-key command is sent.
     * This may be the broadcast address 0xFFFFFFFFFFFFFFFF.
     */
    addrExt_t dstAddr;
    /* A sequence number assigned to a network key by the TC
     * and used to distinguish network keys.
     */
    u8 keySeqNum;
} ss_apsmeSwitchKeyReq_t;

//Parameters for APSME-SWITCH-KEY.indication primitive
typedef struct {
    addrExt_t srcAddr;
    u8 keySeqNum;
} ss_apsmeSwitchKeyInd_t;

//Parameters for APSME-VERIFY-KEY.request primitive
typedef struct {
    addrExt_t dstAddr;
    u8 keyType;
    bool relayCmd;
    addrExt_t relayLongAddr;
    addrExt_t joinerLongAddr;
} ss_apsmeVerifyKeyReq_t;

//Parameters for APSME-VERIFY-KEY.indication primitive
typedef struct {
    addrExt_t srcAddr;
    u8 hashVal[16];
    u8 keyType;
} ss_apsmeVerifyKeyInd_t;

//Parameters for APSME-CONFIRM-KEY.request primitive
typedef struct {
    addrExt_t dstAddr;
    u8 keyType;
    u8 status;
} ss_apsmeConfirmKeyReq_t;

//Parameters for APSME-CONFIRM-KEY.indication primitive
typedef struct {
    addrExt_t srcAddr;
    u8 keyType;
    u8 status;
} ss_apsmeConfirmKeyInd_t;

//Parameters for APSME-KEY-NEGOTIATION.request primitive
typedef struct {
    addrExt_t partnerLongAddr;
    addrExt_t relayLongAddr;
    u8 reqKeyNegoMethod;
    u8 reqPreSharedSecType;
    bool relayCmd;
} ss_apsmeKeyNegotiationReq_t;

//Parameters for APSME-KEY-NEGOTIATION.indication primitive
typedef struct {
    addrExt_t partnerLongAddr;
    addrExt_t relayLongAddr;
    u8 pubPointData[32];
    u8 reqKeyNegoMethod;
    u8 reqPreSharedSecType;
    bool relayCmd;
} ss_apsmeKeyNegotiationInd_t;

//Parameters for APSME-KEY-NEGOTIATION.response primitive
typedef struct {
    addrExt_t partnerLongAddr;
    addrExt_t relayLongAddr;
    u8 status;
    u8 keyNegoMethod;
    u8 preSharedSecType;
    bool relayCmd;
    u8 partnerPubPointData[];
} ss_apsmeKeyNegotiationRsp_t;

//Parameters for APSME-KEY-NEGOTIATION.conform primitive
typedef struct {
    addrExt_t partnerLongAddr;
    addrExt_t relayLongAddr;
    u8 status;
    bool relayCmd;
    u8 partnerPubPointData[];
} ss_apsmeKeyNegotiationCnf_t;

typedef struct {
    addrExt_t dstAddr;
    u8 key[CCM_KEY_SIZE];
} ss_tcUpdateNwkKey_t;

typedef struct {
    ss_dev_pair_set_t keyPair;
    ss_dev_pair_set_add_t keyPairAdd;
    u32 timeout;
} ss_tc_keyPair_t;

typedef struct {
    ev_timer_event_t *pTimeoutEvt;
    u8 privateKey[32];
    ss_dev_pair_set_t keyPair;
    ss_dev_pair_set_add_t keyPairAdd;
} ss_dev_keyPair_t;

typedef void (*apsConfirmKeyCb_t)(u8 status);

typedef void (*ss_retrieveAuthTokenRspCb_t)(u8 status);

extern ss_info_base_t ss_ib;
extern ss_info_base_add_t ss_ib_add;

#if ZB_COORDINATOR_ROLE
extern bool SS_ALLOW_REMOTE_TC_POLICY_CHANGE;
#endif

extern u16 NV_MODULE_KEY_PAIR_ID;

#define SS_IB()                         ss_ib
#define SSIB_GLK_FLAG_SET(v)            ss_ib.reserved = v
#define SSIB_GLK_FLAG_GET()             ss_ib.reserved

#if ZB_COORDINATOR_ROLE
#define SS_TC_KEY_PAIR_CACHE_NUM        10
extern ss_tc_keyPair_t g_ssTcKeyPair[];
extern ss_tc_policy_t g_ss_tcPolicy;
#else
extern ss_dev_keyPair_t g_ssDevKeyPair;
#endif
extern bool joinUsesInstallCodeKey;

u8 ss_hmacAesMmo(u8 len, u8 *padV, u8 *key, u8 *hashOut);
void ss_mmoHash(u8 *data, u8 len, u8 *result);
u8 ss_aesDeriveKey(u8 preLen, u8 *preV, u8 *addr, u8 keyLen, u8 *key);

u8 ss_hamcSha256(u8 dataLen, u8 *dataIn, u8 *key, u8 keyLen, u8 *dataOut);
u8 ss_hashSha256(u8 *key, u8 keyLen, u8 *hashOut);

bool ss_completeDLK(void);
u16 ss_devKeyPairInfoGet(void);
u8 ss_devKeyPairFind(addrExt_t extAddr, ss_dev_pair_set_t *keyPair);
u8 ss_devKeyPairDelete(addrExt_t extAddr);
void ss_devKeyPairSave(ss_dev_pair_set_t *keyPair);
u8 ss_devKeyPairAddFind(addrExt_t extAddr, ss_dev_pair_set_add_t *keyPairAdd);
u8 ss_devKeyPairAddDelete(addrExt_t extAddr);
void ss_devKeyPairAddSave(ss_dev_pair_set_add_t *keyPairAdd);
u8 ss_tcDevKeyPairBackUp(addrExt_t extAddr, ss_devKeyPairBackUp_t *keyPairBackUp);
u8 ss_tcDevKeyPairImport(ss_devKeyPairBackUp_t *keyPairBackUp);

u32 ss_outgoingFrameCntGet(void);
u8 ss_unverifiedKeypairDelete(void);

/**
 * @brief  get the MAC address form key pair table so as to get the information of the node which have joined the network
 *
 * @param  start_idx       - the index of the key pair table
 * @param  num             - the count of the node want to be read out
 * @param  validNum        - the valid count will be read out
 * @param  nodeMacAddrList - the node MAC address list
 *
 * @return total number of key paired
 */
u16 ss_nodeMacAddrFromDevKeyPair(u16 start_idx, u8 num, u8 *validNum, addrExt_t *nodeMacAddrList);

/**
 * @brief save ss_ib to flash
 */
void zdo_ssInfoSaveToFlash(void);

/**
 * @brief get ss_ib from flash
 */
u8 zdo_ssInfoInit(void);

/**
 * @brief update ss_ib to flash
 */
void zdo_ssInfoUpdate(void);

/**
 * @brief get network key index from flash
 *
 * @param  none
 *
 * @return NV_ITEM_NOT_FOUND or index
 */
u8 zdo_ssInfoKeyGet(void);


/* generate network key, and then store it in the ib table */
void ss_nwkKeyGenerate(u8 *nwkKey);

/* store network key in ib table */
void ss_nwkKeyStore(u8 *nwkKey);

/*
 * @brief	security configuration
 *
 * @param	enSecurity  security enable
 *
 * */
void ss_zdoInit(bool enSecurity);

/*
 * @brief	security configuration
 *
 * @param	m  security mode, distribute network or central network
 *
 * */
void ss_securityModeSet(ss_securityMode_e m);

/*
 * @brief	judge if it's a distribute security mode
 *
 * */
bool ss_securityModeIsDistributed(void);

bool ss_keyPreconfigured(void);

/*
 * @brief	the public link and certification link key is supported
 *          even if the pre-install link key is used
 *
 * @param	ks  LINKKEY_MASTER_KEY_EN, LINKKEY_CERTIFICATION_KEY_EN,
 *              or LINKKEY_MASTER_KEY_EN|LINKKEY_CERTIFICATION_KEY_EN
 *
 *
 * */
void ss_pubLinkKeySelect(ss_pubLinkKeyOpt_e ks);

/*
 * @brief:  get base point(G point) via pre-shared key(PSK) or Simple Password exponential (SPE)
 * @input  method(8bits):	key negotiation method
 * @input  param key(128bits):      PSK or SPE
 * @output param g_out(256bits):    the base point(G point): 32Bytes
 * */
void ss_x25519_basePointGet(u8 method, u8 *key, u8 *g_out);

/*
 * @brief:  get public point(Q) and base point(G) via PSK/SPE and private key(d)
 *          G = H*(preKey),  Q=d*G
 * @input  method(8bits):	key negotiation method
 * @input  param preKey(128bits):   PSK or SPE
 * @input  param d(128bits):   		private key
 * @output param G(256bits):   		base point
 * @output param Q(256bits):        the public point(Q point)
 * */
u8 ss_x25519_GQGet(u8 method, u8 *preKey, u8 *d, u8 *G, u8 *Q);

/*
 * @brief:  get shared ECDH shared public point(xk) via private key(d) and public point(Q), xk=da*Qb
 * @input param priKey(128bits):   private key
 * @input param q(256bits):        the public point(Q point)
 * */
u8 ss_x25519_ECDHPointGet(u8 *priKey, u8 *q, u8 *xk);

/*
 * @brief:  get final aps link via ieeeAddress, public point, ECDH shared-key and base point
 * @input param method(8bits): key negotiation method
 * @input param Ar(64bits):    node-r's ieee address key
 * @input param Qr(256bits):   node-r's public point
 * @input param Ai(64bits):    node-i's ieee address key
 * @input param Qi(256bits):   node-i's public point
 * @input param xk(256bits):   EDCH public point
 * @output param keyout(128bits):    the final link key
 * */
u8 ss_x25519_sessionKeyGet(u8 method, u8 *Ar, u8 *Qr, u8 *Ai, u8 *Qi, u8 *xk, u8 *g, u8 *keyOut);

#endif	/* SECURITY_SERVICE_H */
