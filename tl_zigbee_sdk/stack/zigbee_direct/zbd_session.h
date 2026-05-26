/********************************************************************************************************
 * @file    zbd_session.h
 *
 * @brief   This is the header file for zbd_session
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/
#ifndef _ZBD_SESSION_H_
#define _ZBD_SESSION_H_

#define ZDD_PROVISIONING_TIMEOUT    120//no smaller than 60s

/**
 * @brief Session type
 */
typedef enum {
    ZBD_SESS_TYPE_NONE              = 0x0,
    ZBD_SESS_TYPE_PROV              = BIT(0),   /*! provisioning session */
    ZBD_SESS_TYPE_BASIC_AUTH        = BIT(1),   /*! basic authorization session*/
    ZBD_SESS_TYPE_ADMIN_AUTH        = BIT(2),   /*! admin authorization session*/
} ZBD_SESS_TYPE_e;

/**
 * @brief Session state
 */
typedef enum {
    ZBD_BLE_INTERFACE_OFF           = 0x0,
    ZBD_OPEN_TO_BE_PROVISIONED      = 0x1,
    ZBD_OPEN_TO_CONNECT_ZVD         = 0x2,
    ZBD_SESS_ESTABLISH_ZVD          = 0x3,
} ZBD_SESS_STATE_e;

/**
 * @brief Selected Key Negotiation Method Enumeration
 */
typedef enum {
    ZBD_SS_SKNM_RESERVED                = 0, /*! Reserved */
    ZBD_SS_SKNM_X25519_HASH_AES_MMO128  = 1, /*! ECDHE using Curve25519 with Hash AES-MM-128 */
    ZBD_SS_SKNM_X25519_HASH_SHA256      = 2, /*! ECDHE using Curve25519 with Hash SHA-256 */
    ZBD_SS_SKNM_P256_HASH_SHA256        = 3, /*! ECDHE using curve P-256 with Hash SHA-256 */
} ZBD_SS_SKNM_e;

/**
 * @brief Selected Pre-shared Secret Enumeration
 */
typedef enum {
    ZBD_SS_PSK_SAT      = 0, /*! Symmetric Authentication Token*/
    ZBD_SS_PSK_KEISD    = 1, /*! Pre-configured link-key derived from installation code*/
    ZBD_SS_PSK_KEPSD    = 2, /*! Variable-length pass code (for PAKE protocols)*/
    ZBD_SS_PSK_BAK      = 3, /*! Basic Authorization Key
                                 The Basic key grants the ZVD with access to the network
                                 KDF(s,{0x03}) where s = H(ZVD IEEE || Zigbee NWK key) */
    ZBD_SS_PSK_AAK      = 4, /*! Administrative Authorization Key
                                 The Admin key grants the ZVD with access to the Commissioning Service of
                                 a provisioned ZDD and access to the network
                                 KDF(s,{0x04}) where s = H(ZVD IEEE || ZDD TC Link key)*/
    ZBD_SS_PSK_AWKS     = 0xff,/*! Anonymous Well-Known Secret "ZigbeeAlliance18" */
} ZBD_SS_PSKID_e;

typedef struct _attribute_packed_ {
    addrExt_t srcAddr;
    u32 frameCnt;
    u8 secureCtrl;
} zbd_securityCcmNonce_t;

#if 0
/**
 * @brief Pre-Shared-Key (PSK) for provisioning session
 */
typedef enum {
    ZBD_SESS_PROV_PSK_INSTCODE   = 0x0,   /*! installation Code */
    ZBD_SESS_PROV_PSK_KEPSCODE   = 0x1,   /*! Key Exchange with Passcode */
    ZBD_SESS_PROV_PSK_VLPSCODE   = 0x2,   /*! variable-length Passcode */
} ZBD_SESS_PROV_PSK_e;


/**
 * @brief Provisioning session context
 */
typedef struct {
    ZBD_SESS_PROV_PSK_e pskType;
} zbd_sess_prov_t;

/**
 * @brief Authorization key type
 */
typedef enum {
    ZBD_SESS_AUTH_KEY_BASIC     = 0x0,   /*! Basic authorization key. The Basic key
                                             grants the ZVD with access to the network
                                             KDF(s,{0x03}) where s = H(ZVD IEEE || Zigbee NWK key) */
    ZBD_SESS_AUTH_KEY_ADMIN     = 0x1,   /*! The TC Link key may be ZDD specific. The Admin key
                                             grants the ZVD with access to the Commissioning Service of
                                             a provisioned ZDD and access to the network
                                             KDF(s,{0x04}) where s = H(ZVD IEEE || ZDD TC Link key) */
} ZBD_SESS_AUTH_KEY_e;

/**
 * @brief Authorization session context
 */
typedef struct {
    ZBD_SESS_AUTH_KEY_e akType;
} zbd_sess_auth_t;
#endif

/**
 * @brief ZBD session context
 */
typedef struct {
    ZBD_SESS_TYPE_e session;
    ZBD_SS_SKNM_e sknm;         /*! Selected Key Negotiation Method */
    ZBD_SS_PSKID_e pskId;       /*! Pre-shared key Type */
    u8 isLimited;               /*! ZDD can basic authorization key from a transport key message */
    u8 sk[16];                  /*! session key */
    u32 incomingFrameCounter;
    u32 outgoingFrameCounter;
    u8 rmtExtAddr[8];           /*! remote ieee address */
    bool unproZdd;
} zbd_sess_t;

/**
 * @brief ZBD session authentication data
 */
typedef struct {
    u8 serviceUuid[16];
    u8 serviceInId;
    u8 charUuid[16];
    u8 charInId;
} zbd_authData_t;

typedef int (*app_zbd_optTimeoutCb_t)(void);
typedef int (*app_zbd_ss_result_cb_t)(ZBD_SESS_TYPE_e type, u8 ret);

extern zbd_sess_t g_zbdSession;

/**
 * @brief  Set the session state.
 *
 * @param  state
 *
 * @return none
 */
void zdb_sessionStateSet(ZBD_SESS_STATE_e state);

/**
 * @brief  Get the session state.
 *
 * @param  none
 *
 * @return state
 */
ZBD_SESS_STATE_e zbd_sessionStateGet(void);

/**
 * @brief  Set the flag whether the AnonymousJoinTimeout parameter is 0.
 *
 * @param  flag - TRUE or FALSE
 *
 * @return none
 */
void zbd_sessionAnjFlagSet(bool flag);

/**
 * @brief  Get the flag whether the AnonymousJoinTimeout parameter is 0.
 *
 * @param  none
 *
 * @return TRUE or FALSE
 */
bool zbd_sessionAnjFlagGet(void);

/**
 * @brief To reset the session parameters when the BLE connection is exit.
 */
void zbd_sessionExit(void);

/**
 * @brief  To register the callback function for the session.
 *
 * @param  cb    - open to be provisioned timeout callback
 * @param  retCb - callback when session build finish
 * @param  p - the ble attribute table
 *
 * @return none
 */
void zbd_sessionInit(app_zbd_optTimeoutCb_t cb, app_zbd_ss_result_cb_t retCb, u8 *p);

/**
 * @brief  To encrypt the commissioning service data.
 *
 * @param  attHandle - characteristic handle id
 * @param  serviceUuid128 - service 128bit uuid
 * @param  sessLevel - support session level
 * @param  data      - the data to encrypt or decrypt
 * @param  dataLen   - length
 * @param  enDeFlag  - 1 - encrypt; 0 - encrypt or decrypt
 *
 * @return state
 */
u8 zbd_sessionSecureFrameProcess(u16 attHandle, u8* serviceUuid128, ZBD_SESS_TYPE_e sessLevel, u8 *data, u8 *dataLen, bool enDeFlag);

/**
 * @brief  Indication to the app of the session build result.
 *
 * @param  sessType - session type
 * @param  ret      - the result
 *
 * @return none
 */
void zbd_sessionFinished(ZBD_SESS_TYPE_e sessType, u8 ret);

/**
 * @brief  Get the type of current session.
 *
 * @param  none
 *
 * @return ZBD_SESS_TYPE_e
 */
ZBD_SESS_TYPE_e zbd_sessionTypeGet(void);

/**
 * @brief  Get the limited of current session.
 *
 * @param  none
 *
 * @return TRUE or FALSE
 */
bool zbd_sessionIsLimitedGet(void);

/**
 * @brief  Delete the pre-configured admin key form the flash.
 *
 * @param  none
 *
 * @return status
 */
nv_sts_t zbd_preAdminKeyDelete(void);

/**
 * @brief  Get the admin key from flash.
 *
 * @param  adminKey - the admin key
 *
 * @return status
 */
nv_sts_t zbd_preAdminKeyRestoreFromFlash(u8 *adminKey);

/**
 * @brief  Save the admin key to flash.
 *
 * @param  adminKey - the admin key
 *
 * @return status
 */
nv_sts_t zbd_preAdminKeySave2Flash(u8 *adminKey);

#endif
